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
    primitives: Record<string, string>;
    structs: Record<string, string>;
    string_types: string[];
    ignored_types?: string[];
}

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
 * Wrapper for {{suiteName}}::{{name}}
{{#inputParams}}
 * @param params["{{name}}"] - {{type}}{{#isHandle}} (handle ID{{#isOptional}}, optional{{/isOptional}}){{/isHandle}}
{{/inputParams}}
{{#outputParams}}
 * @returns ["{{name}}"] - {{type}}{{#isHandle}} (handle ID){{/isHandle}}
{{/outputParams}}
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

    // Call SDK function
    AIErr err = s{{suiteShortName}}->{{name}}({{#callArgs}}{{{argName}}}{{#hasMore}}, {{/hasMore}}{{/callArgs}});
    if (err != kNoErr) {
        throw std::runtime_error("{{name}} failed with error: " + std::to_string(err));
    }

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
        const headerData = this.prepareHeaderData(suite);
        const sourceData = this.prepareSourceData(suite);

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
            functions: suite.functions.map(func => ({
                name: func.name,
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
                    }))
            }))
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
     */
    private generateDispatchCases(functions: FunctionInfo[]): string[] {
        const cases: string[] = [];

        for (let i = 0; i < functions.length; i++) {
            const func = functions[i];
            if (i === 0) {
                cases.push(`    if (method == "${func.name}") {`);
            } else {
                cases.push(`    } else if (method == "${func.name}") {`);
            }
            cases.push(`        return ${func.name}(params);`);
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

        // Process each parameter
        for (let i = 0; i < func.params.length; i++) {
            const param = func.params[i];
            const classification = param.classification;

            if (!classification) {
                // Unknown type - use generic handling
                if (param.isOutput) {
                    unmarshalLines.push(`    // Output parameter: ${param.name} (unknown type ${param.type})`);
                    unmarshalLines.push(`    ${param.type} ${param.name};`);
                    callArgs.push({ argName: `&${param.name}`, hasMore: i < func.params.length - 1 });
                    marshalLines.push(`    // Unable to marshal unknown type: ${param.name}`);
                } else {
                    unmarshalLines.push(`    // Input parameter: ${param.name} (unknown type ${param.type})`);
                    unmarshalLines.push(`    // WARNING: Unknown type, using default`);
                    callArgs.push({ argName: param.name, hasMore: i < func.params.length - 1 });
                }
                continue;
            }

            const category = classification.category;

            if (param.isOutput) {
                // Output parameter handling
                this.generateOutputUnmarshal(param, classification, unmarshalLines);
                this.generateOutputMarshal(param, classification, marshalLines);
                callArgs.push({ argName: `&${param.name}`, hasMore: i < func.params.length - 1 });
            } else {
                // Input parameter handling
                this.generateInputUnmarshal(param, classification, unmarshalLines);
                const argName = this.getCallArgName(param, classification);
                callArgs.push({ argName, hasMore: i < func.params.length - 1 });
            }
        }

        return {
            name: func.name,
            suiteShortName,
            unmarshalCode: unmarshalLines,
            marshalCode: marshalLines,
            callArgs
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
                // Check if this is an optional handle (can be null/-1)
                if (this.isOptionalHandle(param)) {
                    lines.push(`    // Input handle (optional): ${param.name}`);
                    lines.push(`    ${baseType} ${param.name}_val = nullptr;`);
                    lines.push(`    if (params.contains("${param.name}") && !params["${param.name}"].is_null()) {`);
                    lines.push(`        int32_t ${param.name}_id = params["${param.name}"].get<int32_t>();`);
                    lines.push(`        if (${param.name}_id >= 0) {`);
                    lines.push(`            auto ${param.name}_ptr = HandleManager::${registryName}.Get(${param.name}_id);`);
                    lines.push(`            if (${param.name}_ptr) {`);
                    lines.push(`                ${param.name}_val = *${param.name}_ptr;`);
                    lines.push(`            }`);
                    lines.push(`        }`);
                    lines.push(`    }`);
                } else {
                    lines.push(`    // Input handle: ${param.name}`);
                    lines.push(`    auto ${param.name}_ptr = HandleManager::${registryName}.Get(params["${param.name}"].get<int32_t>());`);
                    lines.push(`    if (!${param.name}_ptr) {`);
                    lines.push(`        throw std::runtime_error("Invalid ${baseType} handle for parameter '${param.name}'");`);
                    lines.push(`    }`);
                    lines.push(`    ${baseType} ${param.name}_val = *${param.name}_ptr;`);
                }
                break;

            case 'String':
                // Input string - convert from std::string
                if (baseType === 'ai::UnicodeString') {
                    lines.push(`    // Input string: ${param.name}`);
                    lines.push(`    ai::UnicodeString ${param.name}(params["${param.name}"].get<std::string>());`);
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
