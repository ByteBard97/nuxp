import * as fs from 'fs-extra';
import * as Mustache from 'mustache';
import { SuiteInfo, FunctionInfo, ParamInfo } from '../parser/types';
import { TypeDefinition, TypeCategory } from '../parser/TypeClassifier';

/**
 * Represents a generated file with filename and content
 */
export interface GeneratedFile {
    filename: string;
    content: string;
}

/**
 * Type configuration loaded from type-map.json
 */
interface TypeMapConfig {
    handles: Record<string, string>;
    managed_handles?: Record<string, string>;
    primitives: Record<string, string>;
    structs: Record<string, string>;
    string_types: string[];
    ignored_types?: string[];
}

/**
 * Return type categories for code generation
 */
type ReturnTypeCategory = 'Error' | 'Boolean' | 'Void' | 'Handle' | 'ManagedHandle' | 'Primitive' | 'String' | 'Unsupported';

/**
 * Mustache templates for C++ code generation
 */
const templates = {
    header: `#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace {{suiteName}} {

{{#functions}}
/**
 * Wrapper for {{suiteName}}::{{sdkName}}
{{#inputParams}}
 * @param params["{{name}}"] - {{type}}{{#isHandle}} (handle ID{{#isOptional}}, optional{{/isOptional}}){{/isHandle}}
{{/inputParams}}
{{#outputParams}}
 * @returns ["{{name}}"] - {{type}}{{#isHandle}} (handle ID){{/isHandle}}
{{/outputParams}}
{{#returnsBoolean}}
 * @returns ["result"] - bool (from {{returnCppType}} return)
{{/returnsBoolean}}
{{#returnsHandle}}
 * @returns ["result"] - handle ID (from {{returnCppType}} return)
{{/returnsHandle}}
{{#returnsManagedHandle}}
 * @returns ["result"] - managed handle ID (from {{returnCppType}} return)
{{/returnsManagedHandle}}
{{#returnsPrimitive}}
 * @returns ["result"] - {{returnCppType}} value
{{/returnsPrimitive}}
{{#returnsString}}
 * @returns ["result"] - string (from {{returnCppType}} return)
{{/returnsString}}
 */
nlohmann::json {{name}}(const nlohmann::json& params);

{{/functions}}
/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace {{suiteName}}
} // namespace Flora
`,

    source: `#include "Flora{{suiteName}}Wrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" {{suiteTypeName}}* s{{suiteShortName}};

namespace Flora {
namespace {{suiteName}} {

{{#functions}}
nlohmann::json {{name}}(const nlohmann::json& params) {
    nlohmann::json response;

{{#unmarshalCode}}
{{{.}}}
{{/unmarshalCode}}

{{#returnsError}}
    // Call SDK function (returns AIErr)
    AIErr err = s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    if (err != kNoErr) {
        throw std::runtime_error("{{sdkName}} failed with error: " + std::to_string(err));
    }
{{/returnsError}}
{{#returnsBoolean}}
    // Call SDK function (returns boolean)
    {{returnCppType}} result = s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    response["result"] = static_cast<bool>(result);
{{/returnsBoolean}}
{{#returnsVoid}}
    // Call SDK function (returns void)
    s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
{{/returnsVoid}}
{{#returnsHandle}}
    // Call SDK function (returns handle)
    {{returnCppType}} result = s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    if (result) {
        response["result"] = HandleManager::{{returnRegistryName}}.Register(result);
    } else {
        response["result"] = -1;
    }
{{/returnsHandle}}
{{#returnsManagedHandle}}
    // Call SDK function (returns managed object)
    {{returnCppType}} result = s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    response["result"] = HandleManager::{{returnRegistryName}}.Register(std::move(result));
{{/returnsManagedHandle}}
{{#returnsPrimitive}}
    // Call SDK function (returns primitive)
    {{returnCppType}} result = s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    response["result"] = result;
{{/returnsPrimitive}}
{{#returnsString}}
    // Call SDK function (returns string)
    {{returnCppType}} result = s{{suiteShortName}}->{{sdkName}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    response["result"] = {{{returnStringMarshalExpr}}};
{{/returnsString}}

{{#marshalCode}}
{{{.}}}
{{/marshalCode}}

    return response;
}

{{/functions}}
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
{{#dispatchCases}}
{{{.}}}
{{/dispatchCases}}
    throw std::runtime_error("Unknown method: " + method + " in {{suiteName}}");
}

} // namespace {{suiteName}}
} // namespace Flora
`
};

/**
 * Information about a struct field for marshaling
 */
interface StructFieldInfo {
    name: string;
    cppType: string;
}

/**
 * Function names that conflict with system macros (macOS CarbonCore/FixMath.h)
 * These are prefixed with "AI_" in generated code to avoid collision
 */
const MACRO_CONFLICTING_NAMES: Set<string> = new Set([
    'FractToFloat',
    'FloatToFract',
    'FixedToFloat',
    'FloatToFixed',
    'FixRatio',
    'FixMul',
    'FixDiv',
    'FixRound',
]);

/**
 * Get safe function name, prefixing if it conflicts with system macros
 */
function getSafeFunctionName(name: string): string {
    if (MACRO_CONFLICTING_NAMES.has(name)) {
        return `AI_${name}`;
    }
    return name;
}

/**
 * Struct definitions for known SDK structs
 */
const structDefinitions: Record<string, StructFieldInfo[]> = {
    'AIRealRect': [
        { name: 'left', cppType: 'AIReal' },
        { name: 'top', cppType: 'AIReal' },
        { name: 'right', cppType: 'AIReal' },
        { name: 'bottom', cppType: 'AIReal' }
    ],
    'AIRealPoint': [
        { name: 'h', cppType: 'AIReal' },
        { name: 'v', cppType: 'AIReal' }
    ],
    'AIRealMatrix': [
        { name: 'a', cppType: 'AIReal' },
        { name: 'b', cppType: 'AIReal' },
        { name: 'c', cppType: 'AIReal' },
        { name: 'd', cppType: 'AIReal' },
        { name: 'tx', cppType: 'AIReal' },
        { name: 'ty', cppType: 'AIReal' }
    ]
};

/**
 * CppGenerator transforms parsed SDK suite information into C++ wrapper code
 */
export class CppGenerator {
    private config: TypeMapConfig;

    /**
     * Create a new CppGenerator
     * @param configPath Path to type-map.json configuration file
     */
    constructor(configPath: string) {
        const configContent = fs.readFileSync(configPath, 'utf-8');
        this.config = JSON.parse(configContent);
    }

    /**
     * Generate C++ wrapper files for a suite
     * @param suite The parsed suite information
     * @returns Array of generated files (header and source)
     */
    generate(suite: SuiteInfo): GeneratedFile[] {
        // Filter out functions with unsupported parameter types
        const filteredSuite: SuiteInfo = {
            ...suite,
            functions: this.filterFunctions(suite.functions)
        };

        const headerData = this.prepareHeaderData(filteredSuite);
        const sourceData = this.prepareSourceData(filteredSuite);

        const headerContent = Mustache.render(templates.header, headerData);
        const sourceContent = Mustache.render(templates.source, sourceData);

        return [
            {
                filename: `Flora${suite.name}Wrapper.h`,
                content: headerContent
            },
            {
                filename: `Flora${suite.name}Wrapper.cpp`,
                content: sourceContent
            }
        ];
    }

    /**
     * Prepare data for header template
     */
    private prepareHeaderData(suite: SuiteInfo): object {
        return {
            suiteName: suite.name,
            functions: suite.functions.map(func => {
                const returnTypeCategory = this.classifyReturnType(func);
                return {
                    name: getSafeFunctionName(func.name),
                    sdkName: func.name,  // Original SDK name for documentation
                    inputParams: func.params
                        .filter(p => !p.isOutput)
                        .map(p => ({
                            name: p.name,
                            type: p.type,
                            isHandle: p.classification?.category === 'Handle',
                            isOptional: this.isOptionalHandle(p)
                        })),
                    outputParams: func.params
                        .filter(p => p.isOutput)
                        .map(p => ({
                            name: p.name,
                            type: p.type,
                            isHandle: p.classification?.category === 'Handle'
                        })),
                    returnsBoolean: returnTypeCategory === 'Boolean',
                    returnsHandle: returnTypeCategory === 'Handle',
                    returnsManagedHandle: returnTypeCategory === 'ManagedHandle',
                    returnsPrimitive: returnTypeCategory === 'Primitive',
                    returnsString: returnTypeCategory === 'String',
                    returnCppType: func.returnType,
                    returnRegistryName: returnTypeCategory === 'Handle'
                        ? (this.config.handles[func.returnType] || '')
                        : returnTypeCategory === 'ManagedHandle'
                        ? (this.config.managed_handles?.[func.returnType] || '')
                        : '',
                };
            })
        };
    }

    /**
     * Check if a handle parameter is optional (can be null)
     * Common pattern: "prep" handles are often optional in SDK
     */
    private isOptionalHandle(param: ParamInfo): boolean {
        if (param.classification?.category !== 'Handle') return false;
        // Common optional handle parameter names in Illustrator SDK
        const optionalNames = ['prep', 'prepArt', 'paintOrder', 'parent'];
        return optionalNames.some(n => param.name.toLowerCase().includes(n.toLowerCase()));
    }

    /**
     * Check if a function has parameters that cannot be serialized to JSON
     * Returns true if function should be skipped
     */
    private hasUnsupportedParams(func: FunctionInfo): boolean {
        for (const param of func.params) {
            // Skip functions with void* parameters (can't serialize opaque pointers)
            // Parser stores pointer info separately, so check both type and isPointer
            if (param.type === 'void' && param.isPointer) {
                return true;
            }
            // Also check for void** in raw type string
            if (param.type.includes('void') && param.type.includes('*')) {
                return true;
            }
            // Skip triple pointers (e.g., AIArtHandle*** for array outputs)
            // These require complex memory management that we don't handle
            if (param.type.includes('***') || param.type.includes('** *')) {
                return true;
            }
            // Skip callback function pointers
            if (param.type.includes('(*)') || param.type.includes('Proc')) {
                return true;
            }
            // Skip C-style void parameters: func(void) means no params
            if (param.name === 'void') {
                continue;
            }
            // Skip forward-declared struct types (can't instantiate)
            if (param.type.includes('struct _')) {
                return true;
            }
            // Skip complex struct types that have explicit constructors or are difficult to marshal
            const complexStructTypes = [
                'AINewDocumentPreset',
                'ActionDialogStatus',
                'AIDocumentColorProfile',
                'AIRasterizeSettings',
                'AIColorConversionOptions',
                'AIColor',       // Complex union type
                'AIGradient',    // Complex structure
                'AIPattern',     // Complex structure
                'AIPathStyle',   // Complex nested structure
                'AISuspendedAppContext', // Opaque type
            ];
            if (complexStructTypes.some(t => param.type.includes(t))) {
                return true;
            }
            // char** outputs (SDK-managed strings) are now supported for non-const outputs
            // The SDK allocates the string, we copy it to std::string before returning
            // (handled in generateOutputUnmarshal/generateOutputMarshal)
            // Note: const char** outputs are misparsed as inputs and must be in BLOCKED_FUNCTIONS
            // Skip malformed const char (should be const char*)
            if (param.type === 'const char' && param.isPointer) {
                // This should be classified as string, but parser might miss it
                // Skip for now if it's not being handled correctly
                if (param.classification?.category !== 'String') {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Check if a function has an unsupported return type
     * Returns true if function should be skipped
     */
    private hasUnsupportedReturnType(func: FunctionInfo): boolean {
        return this.classifyReturnType(func) === 'Unsupported';
    }

    /**
     * Classify the return type of a function
     */
    private classifyReturnType(func: FunctionInfo): ReturnTypeCategory {
        // Check all boolean primitive types (AIBoolean, ASBoolean, bool)
        if (func.returnType === 'AIBoolean' ||
            this.config.primitives[func.returnType] === 'bool') {
            return 'Boolean';
        }
        if (func.returnType === 'void') {
            return 'Void';
        }
        if (func.returnType === 'AIErr' || func.returnType === 'ASErr') {
            return 'Error';
        }

        // Use type-map.json for non-standard return types
        if (this.config.handles[func.returnType]) {
            return 'Handle';
        }
        if (this.config.managed_handles && this.config.managed_handles[func.returnType]) {
            return 'ManagedHandle';
        }
        if (this.config.primitives[func.returnType]) {
            return 'Primitive';
        }
        if (this.config.string_types.includes(func.returnType)) {
            return 'String';
        }

        return 'Unsupported';
    }

    /**
     * Filter suite functions to remove those with unsupported parameters or return types
     */
    private filterFunctions(functions: FunctionInfo[]): FunctionInfo[] {
        return functions.filter(func =>
            !this.hasUnsupportedParams(func) &&
            !this.hasUnsupportedReturnType(func)
        );
    }

    /**
     * Prepare data for source template
     */
    private prepareSourceData(suite: SuiteInfo): object {
        // Extract short name (e.g., "AIArtSuite" -> "Art")
        const suiteShortName = this.extractShortName(suite.name);

        // Generate dispatch cases for method routing
        const dispatchCases = this.generateDispatchCases(suite.functions);

        return {
            suiteName: suite.name,
            suiteTypeName: suite.name,
            suiteShortName: suiteShortName,
            functions: suite.functions.map(func => this.prepareFunctionData(func, suiteShortName)),
            dispatchCases
        };
    }

    /**
     * Generate dispatch case statements for all functions in a suite
     * Maps original SDK method names to (possibly prefixed) function names
     */
    private generateDispatchCases(functions: FunctionInfo[]): string[] {
        const cases: string[] = [];

        for (let i = 0; i < functions.length; i++) {
            const func = functions[i];
            const safeName = getSafeFunctionName(func.name);
            if (i === 0) {
                cases.push(`    if (method == "${func.name}") {`);
            } else {
                cases.push(`    } else if (method == "${func.name}") {`);
            }
            cases.push(`        return ${safeName}(params);`);
        }

        if (functions.length > 0) {
            cases.push(`    }`);
        }

        return cases;
    }

    /**
     * Extract short name from suite name (e.g., "AIArtSuite" -> "Art")
     */
    private extractShortName(suiteName: string): string {
        // Remove "AI" prefix and "Suite" suffix
        let short = suiteName;
        if (short.startsWith('AI')) {
            short = short.substring(2);
        }
        if (short.endsWith('Suite')) {
            short = short.substring(0, short.length - 5);
        }
        return short;
    }

    /**
     * Prepare data for a single function
     */
    private prepareFunctionData(func: FunctionInfo, suiteShortName: string): object {
        const unmarshalLines: string[] = [];
        const marshalLines: string[] = [];
        const callArgs: { argName: string; hasMore: boolean }[] = [];
        const safeName = getSafeFunctionName(func.name);

        // Classify return type for template branching
        const returnTypeCategory = this.classifyReturnType(func);

        // Filter out C-style void parameters: func(void) means no params
        const params = func.params.filter(p => !(p.name === 'void' && p.type === 'void'));

        // Process each parameter
        for (let i = 0; i < params.length; i++) {
            const param = params[i];
            const classification = param.classification;

            if (!classification) {
                // Unknown type - use generic handling
                if (param.isOutput) {
                    unmarshalLines.push(`    // Output parameter: ${param.name} (unknown type ${param.type})`);
                    unmarshalLines.push(`    ${param.type} ${param.name};`);
                    callArgs.push({ argName: `&${param.name}`, hasMore: i < params.length - 1 });
                    marshalLines.push(`    // Unable to marshal unknown type: ${param.name}`);
                } else {
                    unmarshalLines.push(`    // Input parameter: ${param.name} (unknown type ${param.type})`);
                    unmarshalLines.push(`    // WARNING: Unknown type, using default`);
                    callArgs.push({ argName: param.name, hasMore: i < params.length - 1 });
                }
                continue;
            }

            const category = classification.category;

            if (param.isOutput) {
                // Output parameter handling
                this.generateOutputUnmarshal(param, classification, unmarshalLines);
                this.generateOutputMarshal(param, classification, marshalLines);
                // ManagedHandle outputs pass by reference (no & needed), regular handles need &
                const argPrefix = (category === 'ManagedHandle') ? '' : (param.isPointer ? '&' : '');
                callArgs.push({ argName: `${argPrefix}${param.name}`, hasMore: i < params.length - 1 });
            } else {
                // Input parameter handling
                this.generateInputUnmarshal(param, classification, unmarshalLines);
                const argName = this.getCallArgName(param, classification);
                callArgs.push({ argName, hasMore: i < params.length - 1 });
            }
        }

        // For handle returns, get the registry name
        let returnRegistryName = '';
        const returnCppType = func.returnType;
        if (returnTypeCategory === 'Handle') {
            returnRegistryName = this.config.handles[func.returnType] || '';
        } else if (returnTypeCategory === 'ManagedHandle') {
            returnRegistryName = this.config.managed_handles?.[func.returnType] || '';
        }

        // For string returns, determine the correct marshal expression
        let returnStringMarshalExpr = '';
        if (returnTypeCategory === 'String') {
            if (func.returnType === 'ai::FilePath') {
                returnStringMarshalExpr = 'result.GetFullPath().as_UTF8()';
            } else if (func.returnType === 'ai::UnicodeString') {
                returnStringMarshalExpr = 'result.as_UTF8()';
            } else {
                // const char* and similar pointer types
                returnStringMarshalExpr = 'result ? std::string(result) : ""';
            }
        }

        return {
            name: safeName,           // Safe (prefixed) name for C++ function
            sdkName: func.name,       // Original SDK name for the actual call
            suiteShortName,
            unmarshalCode: unmarshalLines,
            marshalCode: marshalLines,
            callArgs,
            // Return type flags for template branching
            returnsError: returnTypeCategory === 'Error',
            returnsBoolean: returnTypeCategory === 'Boolean',
            returnsVoid: returnTypeCategory === 'Void',
            returnsHandle: returnTypeCategory === 'Handle',
            returnsManagedHandle: returnTypeCategory === 'ManagedHandle',
            returnsPrimitive: returnTypeCategory === 'Primitive',
            returnsString: returnTypeCategory === 'String',
            returnCppType,
            returnRegistryName,
            returnStringMarshalExpr,
        };
    }

    /**
     * Generate unmarshaling code for input parameters
     */
    private generateInputUnmarshal(
        param: ParamInfo,
        classification: TypeDefinition,
        lines: string[]
    ): void {
        const category = classification.category;
        const baseType = classification.baseType;
        const registryName = classification.registryName;

        switch (category) {
            case 'Handle':
                // Input handle - get from registry by ID
                // HandleRegistry<StructType>::Get() returns StructType* which IS the handle type
                // (e.g., HandleRegistry<ArtObject>::Get() returns ArtObject* = AIArtHandle)
                // Check if this is an optional handle (can be null/-1)
                if (this.isOptionalHandle(param)) {
                    lines.push(`    // Input handle (optional): ${param.name}`);
                    lines.push(`    ${baseType} ${param.name}_val = nullptr;`);
                    lines.push(`    if (params.contains("${param.name}") && !params["${param.name}"].is_null()) {`);
                    lines.push(`        int32_t ${param.name}_id = params["${param.name}"].get<int32_t>();`);
                    lines.push(`        if (${param.name}_id >= 0) {`);
                    lines.push(`            ${param.name}_val = HandleManager::${registryName}.Get(${param.name}_id);`);
                    lines.push(`        }`);
                    lines.push(`    }`);
                } else {
                    lines.push(`    // Input handle: ${param.name}`);
                    lines.push(`    ${baseType} ${param.name}_val = HandleManager::${registryName}.Get(params["${param.name}"].get<int32_t>());`);
                    lines.push(`    if (!${param.name}_val) {`);
                    lines.push(`        throw std::runtime_error("Invalid ${baseType} handle for parameter '${param.name}'");`);
                    lines.push(`    }`);
                }
                break;

            case 'String':
                // Input string - convert from std::string
                if (baseType === 'ai::UnicodeString') {
                    lines.push(`    // Input string: ${param.name}`);
                    lines.push(`    ai::UnicodeString ${param.name}(params["${param.name}"].get<std::string>());`);
                } else if (baseType === 'ai::FilePath') {
                    lines.push(`    // Input file path: ${param.name}`);
                    lines.push(`    ai::FilePath ${param.name}(ai::UnicodeString(params["${param.name}"].get<std::string>()));`);
                } else {
                    // const char* or other string types
                    lines.push(`    // Input string: ${param.name}`);
                    lines.push(`    std::string ${param.name}_str = params["${param.name}"].get<std::string>();`);
                    lines.push(`    const char* ${param.name} = ${param.name}_str.c_str();`);
                }
                break;

            case 'Primitive':
                // Input primitive - get with appropriate JSON type
                lines.push(`    // Input primitive: ${param.name}`);
                const jsonType = this.getPrimitiveJsonType(baseType);
                lines.push(`    ${baseType} ${param.name} = params["${param.name}"].get<${jsonType}>();`);
                break;

            case 'Struct':
                // Input struct - unmarshal from JSON object
                lines.push(`    // Input struct: ${param.name}`);
                lines.push(`    ${baseType} ${param.name};`);
                this.generateStructUnmarshal(param.name, baseType, lines);
                break;

            case 'Enum':
                // Enums are typically passed as integers
                lines.push(`    // Input enum: ${param.name}`);
                lines.push(`    ${baseType} ${param.name} = static_cast<${baseType}>(params["${param.name}"].get<int32_t>());`);
                break;

            case 'Void':
                // void parameters (rare, usually void* for user data)
                lines.push(`    // Void parameter: ${param.name} - not supported`);
                lines.push(`    void* ${param.name} = nullptr;`);
                break;

            case 'Error':
                // Error type as input (rare)
                lines.push(`    // Input error code: ${param.name}`);
                lines.push(`    AIErr ${param.name} = params["${param.name}"].get<int32_t>();`);
                break;

            case 'ManagedHandle':
                lines.push(`    // Input managed handle: ${param.name}`);
                lines.push(`    auto* ${param.name}_ptr = HandleManager::${registryName}.Get(params["${param.name}"].get<int32_t>());`);
                lines.push(`    if (!${param.name}_ptr) {`);
                lines.push(`        throw std::runtime_error("Invalid managed handle for parameter '${param.name}'");`);
                lines.push(`    }`);
                break;

            default:
                // Unknown type
                lines.push(`    // Unknown type: ${param.name} (${param.type})`);
                lines.push(`    // WARNING: Using default initialization`);
                lines.push(`    ${param.type} ${param.name}{};`);
        }
    }

    /**
     * Generate unmarshaling code for output parameters (declaration only)
     */
    private generateOutputUnmarshal(
        param: ParamInfo,
        classification: TypeDefinition,
        lines: string[]
    ): void {
        const category = classification.category;
        const baseType = classification.baseType;

        switch (category) {
            case 'Handle':
                lines.push(`    // Output handle: ${param.name}`);
                lines.push(`    ${baseType} ${param.name} = nullptr;`);
                break;

            case 'String':
                if (baseType === 'ai::UnicodeString') {
                    lines.push(`    // Output string: ${param.name}`);
                    lines.push(`    ai::UnicodeString ${param.name};`);
                } else if (baseType === 'ai::FilePath') {
                    lines.push(`    // Output file path: ${param.name}`);
                    lines.push(`    ai::FilePath ${param.name};`);
                } else if (baseType === 'char*') {
                    // char** output - SDK allocates and manages the string
                    // Use non-const char* since SDK expects char** not const char**
                    lines.push(`    // Output string pointer (SDK-managed): ${param.name}`);
                    lines.push(`    char* ${param.name} = nullptr;`);
                } else {
                    lines.push(`    // Output string buffer: ${param.name}`);
                    lines.push(`    char ${param.name}[1024];`);
                    lines.push(`    ${param.name}[0] = '\\0';`);
                }
                break;

            case 'Primitive':
                lines.push(`    // Output primitive: ${param.name}`);
                lines.push(`    ${baseType} ${param.name}{};`);
                break;

            case 'Struct':
                lines.push(`    // Output struct: ${param.name}`);
                lines.push(`    ${baseType} ${param.name}{};`);
                break;

            case 'Enum':
                lines.push(`    // Output enum: ${param.name}`);
                lines.push(`    ${baseType} ${param.name}{};`);
                break;

            case 'Error':
                lines.push(`    // Output error: ${param.name}`);
                lines.push(`    AIErr ${param.name} = kNoErr;`);
                break;

            case 'ManagedHandle':
                lines.push(`    // Output managed handle: ${param.name}`);
                lines.push(`    ${baseType} ${param.name};`);
                break;

            default:
                lines.push(`    // Output unknown: ${param.name}`);
                lines.push(`    ${param.type.replace('*', '')} ${param.name}{};`);
        }
    }

    /**
     * Generate marshaling code for output parameters
     */
    private generateOutputMarshal(
        param: ParamInfo,
        classification: TypeDefinition,
        lines: string[]
    ): void {
        const category = classification.category;
        const baseType = classification.baseType;
        const registryName = classification.registryName;

        switch (category) {
            case 'Handle':
                // Output handle - register and return ID
                lines.push(`    // Marshal output handle: ${param.name}`);
                lines.push(`    if (${param.name}) {`);
                lines.push(`        response["${param.name}"] = HandleManager::${registryName}.Register(${param.name});`);
                lines.push(`    } else {`);
                lines.push(`        response["${param.name}"] = -1;`);
                lines.push(`    }`);
                break;

            case 'String':
                if (baseType === 'ai::UnicodeString') {
                    lines.push(`    // Marshal output string: ${param.name}`);
                    lines.push(`    response["${param.name}"] = ${param.name}.as_UTF8();`);
                } else if (baseType === 'ai::FilePath') {
                    lines.push(`    // Marshal output file path: ${param.name}`);
                    lines.push(`    response["${param.name}"] = ${param.name}.GetFullPath().as_UTF8();`);
                } else if (baseType === 'char*') {
                    // char** output - copy SDK-managed string to JSON
                    lines.push(`    // Marshal output string pointer: ${param.name}`);
                    lines.push(`    response["${param.name}"] = ${param.name} ? std::string(${param.name}) : "";`);
                } else {
                    lines.push(`    // Marshal output string: ${param.name}`);
                    lines.push(`    response["${param.name}"] = std::string(${param.name});`);
                }
                break;

            case 'Primitive':
                lines.push(`    // Marshal output primitive: ${param.name}`);
                if (baseType === 'AIBoolean') {
                    lines.push(`    response["${param.name}"] = static_cast<bool>(${param.name});`);
                } else {
                    lines.push(`    response["${param.name}"] = ${param.name};`);
                }
                break;

            case 'Struct':
                lines.push(`    // Marshal output struct: ${param.name}`);
                this.generateStructMarshal(param.name, baseType, lines);
                break;

            case 'Enum':
                lines.push(`    // Marshal output enum: ${param.name}`);
                lines.push(`    response["${param.name}"] = static_cast<int32_t>(${param.name});`);
                break;

            case 'Error':
                lines.push(`    // Marshal output error: ${param.name}`);
                lines.push(`    response["${param.name}"] = static_cast<int32_t>(${param.name});`);
                break;

            case 'ManagedHandle':
                lines.push(`    // Marshal output managed handle: ${param.name}`);
                lines.push(`    response["${param.name}"] = HandleManager::${registryName}.Register(std::move(${param.name}));`);
                break;

            default:
                lines.push(`    // Unable to marshal unknown type: ${param.name}`);
        }
    }

    /**
     * Generate struct unmarshaling code
     */
    private generateStructUnmarshal(paramName: string, structType: string, lines: string[]): void {
        const fields = structDefinitions[structType];
        if (!fields) {
            lines.push(`    // WARNING: Unknown struct type ${structType}, fields not unmarshaled`);
            return;
        }

        for (const field of fields) {
            const jsonType = this.getStructFieldJsonType(field.cppType);
            lines.push(`    ${paramName}.${field.name} = params["${paramName}"]["${field.name}"].get<${jsonType}>();`);
        }
    }

    /**
     * Generate struct marshaling code
     */
    private generateStructMarshal(paramName: string, structType: string, lines: string[]): void {
        const fields = structDefinitions[structType];
        if (!fields) {
            lines.push(`    // WARNING: Unknown struct type ${structType}, fields not marshaled`);
            lines.push(`    response["${paramName}"] = nlohmann::json::object();`);
            return;
        }

        lines.push(`    response["${paramName}"] = {`);
        for (let i = 0; i < fields.length; i++) {
            const field = fields[i];
            const comma = i < fields.length - 1 ? ',' : '';
            lines.push(`        {"${field.name}", ${paramName}.${field.name}}${comma}`);
        }
        lines.push(`    };`);
    }

    /**
     * Get the JSON type for primitive unmarshaling
     */
    private getPrimitiveJsonType(baseType: string): string {
        const mapping = this.config.primitives[baseType];
        if (mapping) {
            return mapping;
        }
        // Default mappings for common types
        switch (baseType) {
            case 'AIBoolean':
                return 'bool';
            case 'ai::int32':
            case 'long':
            case 'int':
                return 'int32_t';
            case 'ai::int16':
            case 'short':
                return 'int16_t';
            case 'size_t':
                return 'uint32_t';
            case 'AIReal':
            case 'ASReal':
            case 'double':
                return 'double';
            case 'float':
                return 'float';
            default:
                return 'int32_t';
        }
    }

    /**
     * Get the JSON type for struct field unmarshaling
     */
    private getStructFieldJsonType(cppType: string): string {
        switch (cppType) {
            case 'AIReal':
            case 'ASReal':
            case 'double':
                return 'double';
            case 'float':
                return 'float';
            case 'int':
            case 'long':
            case 'ai::int32':
                return 'int32_t';
            case 'short':
            case 'ai::int16':
                return 'int16_t';
            case 'bool':
            case 'AIBoolean':
                return 'bool';
            default:
                return 'double'; // Default for most SDK struct fields
        }
    }

    /**
     * Get the argument name for SDK function call
     */
    private getCallArgName(param: ParamInfo, classification: TypeDefinition): string {
        const category = classification.category;

        // For handles passed to SDK, we pass the value extracted from registry
        if (category === 'Handle') {
            return `${param.name}_val`;
        }

        if (category === 'ManagedHandle') {
            return `*${param.name}_ptr`;
        }

        // For strings, we might need to pass reference or pointer
        if (category === 'String') {
            if (classification.baseType === 'ai::UnicodeString') {
                // ai::UnicodeString is typically passed by const reference
                return param.name;
            } else {
                // const char* is passed directly
                return param.name;
            }
        }

        // For structs, check if passed by reference or pointer
        if (category === 'Struct') {
            if (param.isPointer) {
                return `&${param.name}`;
            }
            return param.name;
        }

        // For primitives, pass by value
        return param.name;
    }
}
