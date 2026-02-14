import * as fs from 'fs-extra';
import * as Mustache from 'mustache';
import { SuiteInfo, FunctionInfo, ParamInfo } from '../parser/types';
import { TypeCategory } from '../parser/TypeClassifier';

/**
 * Configuration loaded from type-map.json
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
 * Represents a generated TypeScript file
 */
export interface GeneratedFile {
    filename: string;
    content: string;
}

/**
 * Struct field definition for TypeScript interface generation
 */
interface StructField {
    name: string;
    type: string;
}

/**
 * Struct definition for TypeScript interface generation
 */
interface StructDefinition {
    name: string;
    description: string;
    fields: StructField[];
}

/**
 * Parameter view for Mustache template
 */
interface ParamView {
    name: string;
    tsType: string;
    description: string;
    isLast: boolean;
}

/**
 * Output parameter view for Mustache template
 */
interface OutputParamView {
    name: string;
    tsType: string;
    isLast: boolean;
}

/**
 * Function view for Mustache template
 */
interface FunctionView {
    name: string;
    description: string;
    params: ParamView[];
    hasParams: boolean;
    paramList: string;
    returnType: string;
    returnDescription: string;
    hasReturn: boolean;
    hasMultipleOutputs: boolean;
    hasSingleOutput: boolean;
    outputParams: OutputParamView[];
    singleOutputName: string;
    callParams: string;
    returnsBooleanDirect: boolean; // For functions that return boolean types (AIBoolean, ASBoolean, bool)
    returnsDirectValue: boolean;   // For functions that return a non-error value directly
}

/**
 * Suite view for Mustache template
 */
interface SuiteView {
    suiteName: string;
    structs: StructDefinition[];
    hasStructs: boolean;
    functions: FunctionView[];
}

/**
 * Mapping of known struct types to their TypeScript field definitions
 */
const STRUCT_DEFINITIONS: Record<string, StructDefinition> = {
    'AIRealRect': {
        name: 'AIRealRect',
        description: 'Rectangle structure for bounds and regions',
        fields: [
            { name: 'left', type: 'number' },
            { name: 'top', type: 'number' },
            { name: 'right', type: 'number' },
            { name: 'bottom', type: 'number' }
        ]
    },
    'AIRealPoint': {
        name: 'AIRealPoint',
        description: 'Point structure for coordinates',
        fields: [
            { name: 'h', type: 'number' },
            { name: 'v', type: 'number' }
        ]
    },
    'AIRealMatrix': {
        name: 'AIRealMatrix',
        description: 'Transformation matrix structure',
        fields: [
            { name: 'a', type: 'number' },
            { name: 'b', type: 'number' },
            { name: 'c', type: 'number' },
            { name: 'd', type: 'number' },
            { name: 'tx', type: 'number' },
            { name: 'ty', type: 'number' }
        ]
    }
};

/**
 * TypeScriptGenerator transforms parsed Adobe Illustrator SDK suite information
 * into TypeScript client code for calling the C++ plugin via HTTP.
 */
export class TypeScriptGenerator {
    private config: TypeMapConfig;

    /**
     * Creates a new TypeScriptGenerator instance
     * @param configPath - Path to the type-map.json configuration file
     */
    constructor(configPath: string) {
        const configContent = fs.readFileSync(configPath, 'utf-8');
        this.config = JSON.parse(configContent);
    }

    /**
     * Generates a TypeScript file for a given suite
     * @param suite - The parsed suite information
     * @returns The generated TypeScript file
     */
    generate(suite: SuiteInfo): GeneratedFile {
        const usedStructs = this.collectUsedStructs(suite);
        const structDefs = this.generateStructDefinitions(usedStructs);
        const functions = this.generateFunctionViews(suite);

        const view: SuiteView = {
            suiteName: suite.name,
            structs: structDefs,
            hasStructs: structDefs.length > 0,
            functions
        };

        const content = this.renderTemplate(view);

        return {
            filename: `${suite.name}.ts`,
            content
        };
    }

    /**
     * Maps a C++ type to its corresponding TypeScript type
     * @param param - The parameter info containing type classification
     * @returns The TypeScript type string
     */
    private mapToTypeScript(param: ParamInfo): string {
        const classification = param.classification;

        if (!classification) {
            return this.mapRawTypeToTypeScript(param.type);
        }

        const category = classification.category;
        const baseType = classification.baseType;

        switch (category) {
            case 'Handle':
                // All handles are represented as numeric IDs
                return 'number';

            case 'ManagedHandle':
                // All managed handles are represented as numeric IDs
                return 'number';

            case 'Primitive':
                return this.mapPrimitiveToTypeScript(baseType);

            case 'String':
                return 'string';

            case 'Struct':
                // Return the struct type name - we generate interfaces for these
                return baseType;

            case 'Enum':
                // Enums are represented as numbers in TypeScript
                return 'number';

            case 'Error':
                // AIErr is a number
                return 'number';

            case 'Void':
                return 'void';

            case 'Unknown':
            default:
                return 'any';
        }
    }

    /**
     * Maps a raw C++ type string to TypeScript when no classification is available
     * @param rawType - The raw C++ type string
     * @returns The TypeScript type string
     */
    private mapRawTypeToTypeScript(rawType: string): string {
        const cleaned = rawType.replace(/const/g, '').replace(/[*&]/g, '').trim();

        // Check handles
        if (this.config.handles[cleaned]) {
            return 'number';
        }

        // Check managed handles
        if (this.config.managed_handles && this.config.managed_handles[cleaned]) {
            return 'number';
        }

        // Check primitives
        if (this.config.primitives[cleaned]) {
            return this.mapPrimitiveToTypeScript(cleaned);
        }

        // Check strings
        if (this.config.string_types.includes(cleaned)) {
            return 'string';
        }

        // Check structs
        if (this.config.structs[cleaned]) {
            return cleaned;
        }

        // Check special cases
        if (cleaned === 'void') {
            return 'void';
        }

        if (cleaned === 'AIErr') {
            return 'number';
        }

        return 'any';
    }

    /**
     * Maps a primitive C++ type to TypeScript
     * @param baseType - The base primitive type name
     * @returns The TypeScript type string
     */
    private mapPrimitiveToTypeScript(baseType: string): string {
        // All boolean primitives map to boolean (AIBoolean, ASBoolean, bool)
        const jsonType = this.config.primitives[baseType];
        if (jsonType === 'bool') {
            return 'boolean';
        }

        // All numeric primitives map to number
        const numericTypes = [
            'ai::int32', 'ai::int16', 'short', 'long', 'size_t',
            'AIReal', 'ASReal', 'int', 'unsigned int', 'float', 'double',
            'int32_t', 'int16_t', 'uint32_t', 'uint16_t', 'int8_t', 'uint8_t'
        ];

        if (numericTypes.includes(baseType) || this.config.primitives[baseType]) {
            return 'number';
        }

        return 'any';
    }

    /**
     * Get the TypeScript return type for a function that returns a value directly
     * (not AIErr/ASErr/void/AIBoolean). Returns null if not a direct return type.
     */
    private getDirectReturnType(returnType: string): string | null {
        // Check if return type is a handle -> number
        if (this.config.handles[returnType]) return 'number';
        // Check if return type is a managed handle -> number
        if (this.config.managed_handles && this.config.managed_handles[returnType]) return 'number';
        // Check if return type is a primitive
        if (this.config.primitives[returnType]) {
            const jsonType = this.config.primitives[returnType];
            if (jsonType === 'bool') return 'boolean';
            return 'number';
        }
        // Check if return type is a string type
        if (this.config.string_types.includes(returnType)) return 'string';
        return null;
    }

    /**
     * Collects all struct types used in the suite
     * @param suite - The suite to analyze
     * @returns Set of struct type names used
     */
    private collectUsedStructs(suite: SuiteInfo): Set<string> {
        const usedStructs = new Set<string>();

        for (const func of suite.functions) {
            for (const param of func.params) {
                const classification = param.classification;
                if (classification && classification.category === 'Struct') {
                    usedStructs.add(classification.baseType);
                } else if (this.config.structs[param.type.replace(/[*&]/g, '').trim()]) {
                    usedStructs.add(param.type.replace(/[*&]/g, '').trim());
                }
            }
        }

        return usedStructs;
    }

    /**
     * Generates struct definitions for used struct types
     * @param usedStructs - Set of struct type names to generate
     * @returns Array of struct definitions
     */
    private generateStructDefinitions(usedStructs: Set<string>): StructDefinition[] {
        const definitions: StructDefinition[] = [];

        for (const structName of usedStructs) {
            if (STRUCT_DEFINITIONS[structName]) {
                definitions.push(STRUCT_DEFINITIONS[structName]);
            } else {
                // Generate a generic struct definition for unknown structs
                definitions.push({
                    name: structName,
                    description: `${structName} structure`,
                    fields: [{ name: 'data', type: 'any' }]
                });
            }
        }

        // Sort alphabetically for consistent output
        definitions.sort((a, b) => a.name.localeCompare(b.name));

        return definitions;
    }

    /**
     * Generates function views for all functions in the suite
     * @param suite - The suite containing the functions
     * @returns Array of function views for template rendering
     */
    private generateFunctionViews(suite: SuiteInfo): FunctionView[] {
        return suite.functions.map(func => this.generateFunctionView(func));
    }

    /**
     * Generates a function view for a single function
     * @param func - The function info
     * @returns The function view for template rendering
     */
    private generateFunctionView(func: FunctionInfo): FunctionView {
        // Separate input and output parameters, filtering C-style void params
        const inputParams = func.params.filter(p => !p.isOutput && !(p.name === 'void' && p.type === 'void'));
        const outputParams = func.params.filter(p => p.isOutput);

        // Check if function returns a boolean directly (AIBoolean, ASBoolean, bool)
        const returnsBooleanDirect = func.returnType === 'AIBoolean' ||
            (this.config.primitives[func.returnType] === 'bool');
        // Check if function returns a non-error value directly (handle, primitive, string)
        const directReturnTsType = this.getDirectReturnType(func.returnType);
        const hasDirectReturn = !returnsBooleanDirect && directReturnTsType !== null;

        // Generate input parameter views
        const paramViews: ParamView[] = inputParams.map((param, index) => ({
            name: param.name,
            tsType: this.mapToTypeScript(param),
            description: this.generateParamDescription(param),
            isLast: index === inputParams.length - 1
        }));

        // Generate output parameter views
        const outputParamViews: OutputParamView[] = outputParams.map((param, index) => ({
            name: param.name,
            tsType: this.mapToTypeScript(param),
            isLast: index === outputParams.length - 1
        }));

        // Determine return type and template flags
        let returnType: string;
        let hasMultipleOutputs: boolean;
        let hasSingleOutput: boolean;
        let singleOutputName: string;
        let useDirectValue: boolean;

        if (returnsBooleanDirect) {
            returnType = 'boolean';
            hasMultipleOutputs = false;
            hasSingleOutput = false;
            singleOutputName = '';
            useDirectValue = false;
        } else if (hasDirectReturn && outputParams.length > 0) {
            // Direct return value + output params: combine into multi-output object
            const resultField = `result: ${directReturnTsType!}`;
            const outputFields = outputParams.map(p => `${p.name}: ${this.mapToTypeScript(p)}`);
            returnType = `{ ${[resultField, ...outputFields].join('; ')} }`;
            hasMultipleOutputs = true;
            hasSingleOutput = false;
            singleOutputName = '';
            useDirectValue = false;
        } else if (hasDirectReturn) {
            // Simple direct return, no output params
            returnType = directReturnTsType!;
            hasMultipleOutputs = false;
            hasSingleOutput = false;
            singleOutputName = '';
            useDirectValue = true;
        } else {
            // Standard: derive return type from output params
            returnType = this.determineReturnType(outputParams);
            hasMultipleOutputs = outputParams.length > 1;
            hasSingleOutput = outputParams.length === 1;
            singleOutputName = hasSingleOutput ? outputParams[0].name : '';
            useDirectValue = false;
        }

        const hasReturn = returnType !== 'void';

        // Generate parameter list string for function signature
        const paramList = paramViews.map(p => `${p.name}: ${p.tsType}`).join(', ');

        // Generate call params object entries
        const callParams = inputParams.map(p => p.name).join(', ');

        return {
            name: func.name,
            description: this.generateFunctionDescription(func),
            params: paramViews,
            hasParams: paramViews.length > 0,
            paramList,
            returnType,
            returnDescription: this.generateReturnDescription(func, outputParams),
            hasReturn,
            hasMultipleOutputs,
            hasSingleOutput,
            outputParams: outputParamViews,
            singleOutputName,
            callParams,
            returnsBooleanDirect,
            returnsDirectValue: useDirectValue
        };
    }

    /**
     * Determines the return type based on output parameters
     * @param outputParams - The output parameters
     * @returns The TypeScript return type string
     */
    private determineReturnType(outputParams: ParamInfo[]): string {
        if (outputParams.length === 0) {
            return 'void';
        }

        if (outputParams.length === 1) {
            return this.mapToTypeScript(outputParams[0]);
        }

        // Multiple output params - return an object type
        const fields = outputParams.map(p => `${p.name}: ${this.mapToTypeScript(p)}`);
        return `{ ${fields.join('; ')} }`;
    }

    /**
     * Generates a description for a parameter based on its name and type
     * @param param - The parameter info
     * @returns The parameter description string
     */
    private generateParamDescription(param: ParamInfo): string {
        const name = param.name;
        const tsType = this.mapToTypeScript(param);

        // Generate contextual descriptions based on common parameter patterns
        if (name.toLowerCase().includes('art') && tsType === 'number') {
            return 'Handle to the art object';
        }

        if (name.toLowerCase().includes('layer') && tsType === 'number') {
            return 'Handle to the layer';
        }

        if (name.toLowerCase().includes('document') || name.toLowerCase().includes('doc')) {
            return 'Handle to the document';
        }

        if (name.toLowerCase() === 'type') {
            return 'The type value';
        }

        if (name.toLowerCase().includes('index')) {
            return 'The index value';
        }

        if (name.toLowerCase().includes('name')) {
            return 'The name string';
        }

        if (name.toLowerCase().includes('bounds') || name.toLowerCase().includes('rect')) {
            return 'The bounding rectangle';
        }

        if (name.toLowerCase().includes('point') || name.toLowerCase().includes('pt')) {
            return 'The point coordinates';
        }

        if (name.toLowerCase().includes('matrix')) {
            return 'The transformation matrix';
        }

        if (name.toLowerCase().includes('color')) {
            return 'The color value';
        }

        if (name.toLowerCase().includes('order')) {
            return 'The order position';
        }

        if (name.toLowerCase().includes('prep')) {
            return 'The prepositional object reference';
        }

        if (name.toLowerCase().includes('count')) {
            return 'The count value';
        }

        if (name.toLowerCase().includes('visible')) {
            return 'The visibility flag';
        }

        if (name.toLowerCase().includes('lock')) {
            return 'The lock state';
        }

        if (name.toLowerCase().includes('select')) {
            return 'The selection state';
        }

        // Default description
        return `The ${this.camelToWords(name)} value`;
    }

    /**
     * Generates a description for a function based on its name
     * @param func - The function info
     * @returns The function description string
     */
    private generateFunctionDescription(func: FunctionInfo): string {
        const name = func.name;

        // Parse common function name patterns
        if (name.startsWith('Get')) {
            const property = name.slice(3);
            return `Retrieves the ${this.camelToWords(property).toLowerCase()} of an object.`;
        }

        if (name.startsWith('Set')) {
            const property = name.slice(3);
            return `Sets the ${this.camelToWords(property).toLowerCase()} of an object.`;
        }

        if (name.startsWith('New') || name.startsWith('Create')) {
            const objectType = name.replace(/^(New|Create)/, '');
            return `Creates a new ${this.camelToWords(objectType).toLowerCase()}.`;
        }

        if (name.startsWith('Delete') || name.startsWith('Dispose') || name.startsWith('Remove')) {
            const objectType = name.replace(/^(Delete|Dispose|Remove)/, '');
            if (objectType) {
                return `Removes the ${this.camelToWords(objectType).toLowerCase()}.`;
            }
            return 'Removes the object.';
        }

        if (name.startsWith('Count')) {
            const objectType = name.slice(5);
            return `Counts the number of ${this.camelToWords(objectType).toLowerCase()} objects.`;
        }

        if (name.startsWith('Has') || name.startsWith('Is')) {
            return `Checks if ${this.camelToWords(name).toLowerCase()}.`;
        }

        if (name.startsWith('Lock') || name.startsWith('Unlock')) {
            return `${name.startsWith('Lock') ? 'Locks' : 'Unlocks'} the object.`;
        }

        if (name.startsWith('Show') || name.startsWith('Hide')) {
            return `${name.startsWith('Show') ? 'Shows' : 'Hides'} the object.`;
        }

        if (name.startsWith('Select') || name.startsWith('Deselect')) {
            return `${name.startsWith('Select') ? 'Selects' : 'Deselects'} the object.`;
        }

        if (name.startsWith('Move') || name.startsWith('Copy') || name.startsWith('Duplicate')) {
            const action = name.startsWith('Move') ? 'Moves' :
                          name.startsWith('Copy') ? 'Copies' : 'Duplicates';
            return `${action} the object.`;
        }

        if (name.startsWith('Transform') || name.startsWith('Rotate') ||
            name.startsWith('Scale') || name.startsWith('Translate')) {
            return `Applies a transformation to the object.`;
        }

        if (name.startsWith('Insert') || name.startsWith('Add')) {
            return `Adds a new item.`;
        }

        if (name.startsWith('Update') || name.startsWith('Refresh')) {
            return `Updates the object state.`;
        }

        // Default description
        return `Performs the ${this.camelToWords(name).toLowerCase()} operation.`;
    }

    /**
     * Generates a return description based on output parameters
     * @param func - The function info
     * @param outputParams - The output parameters
     * @returns The return description string
     */
    private generateReturnDescription(func: FunctionInfo, outputParams: ParamInfo[]): string {
        // Handle direct value returns (non-error return types)
        const directReturnType = this.getDirectReturnType(func.returnType);
        if (directReturnType !== null && func.returnType !== 'AIBoolean') {
            if (outputParams.length > 0) {
                // Combined: direct return + output params
                const outputNames = outputParams.map(p => p.name).join(', ');
                return `An object containing: result (${func.returnType}), ${outputNames}`;
            }
            if (this.config.handles[func.returnType]) {
                return `Handle ID for the returned ${func.returnType}`;
            }
            return `The ${func.returnType} value`;
        }
        if (func.returnType === 'AIBoolean') {
            return 'True if the condition is met, false otherwise';
        }

        if (outputParams.length === 0) {
            return '';
        }

        if (outputParams.length === 1) {
            const param = outputParams[0];
            const name = param.name;
            const tsType = this.mapToTypeScript(param);

            if (name.toLowerCase().includes('art') && tsType === 'number') {
                return 'Handle to the art object';
            }

            if (name.toLowerCase().includes('layer') && tsType === 'number') {
                return 'Handle to the layer';
            }

            if (name.toLowerCase().includes('type') && tsType === 'number') {
                return 'The type value';
            }

            if (name.toLowerCase().includes('bounds') || name.toLowerCase().includes('rect')) {
                return 'The bounding rectangle';
            }

            if (name.toLowerCase().includes('count')) {
                return 'The count value';
            }

            if (name.toLowerCase().includes('name') && tsType === 'string') {
                return 'The name string';
            }

            if (tsType === 'boolean') {
                return 'True if the condition is met, false otherwise';
            }

            return `The ${this.camelToWords(name).toLowerCase()} value`;
        }

        // Multiple outputs
        const fields = outputParams.map(p => p.name).join(', ');
        return `An object containing: ${fields}`;
    }

    /**
     * Converts camelCase to space-separated words
     * @param str - The camelCase string
     * @returns Space-separated words
     */
    private camelToWords(str: string): string {
        return str
            .replace(/([A-Z])/g, ' $1')
            .replace(/^./, s => s.toUpperCase())
            .trim();
    }

    /**
     * Renders the TypeScript template with the given view
     * @param view - The suite view data
     * @returns The rendered TypeScript code
     */
    private renderTemplate(view: SuiteView): string {
        const template = this.getTemplate();
        return Mustache.render(template, view);
    }

    /**
     * Gets the Mustache template for TypeScript generation
     * @returns The template string
     */
    private getTemplate(): string {
        return `/**
 * {{suiteName}} client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';
{{#hasStructs}}

// Struct type definitions
{{#structs}}

/**
 * {{description}}
 */
export interface {{name}} {
{{#fields}}
    {{name}}: {{type}};
{{/fields}}
}
{{/structs}}
{{/hasStructs}}

const SUITE_NAME = '{{suiteName}}';
{{#functions}}

/**
 * {{description}}
{{#params}}
 * @param {{name}} - {{description}}
{{/params}}
{{#hasReturn}}
 * @returns {{returnDescription}}
{{/hasReturn}}
 */
export async function {{name}}({{paramList}}): Promise<{{returnType}}> {
{{^hasReturn}}
    await callCpp(SUITE_NAME, '{{name}}', { {{callParams}} });
{{/hasReturn}}
{{#returnsBooleanDirect}}
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, '{{name}}', { {{callParams}} });
    return result.result;
{{/returnsBooleanDirect}}
{{#returnsDirectValue}}
    const result = await callCpp<{ result: {{returnType}} }>(SUITE_NAME, '{{name}}', { {{callParams}} });
    return result.result;
{{/returnsDirectValue}}
{{#hasSingleOutput}}
    const result = await callCpp<{ {{singleOutputName}}: {{returnType}} }>(SUITE_NAME, '{{name}}', { {{callParams}} });
    return result.{{singleOutputName}};
{{/hasSingleOutput}}
{{#hasMultipleOutputs}}
    const result = await callCpp<{{returnType}}>(SUITE_NAME, '{{name}}', { {{callParams}} });
    return result;
{{/hasMultipleOutputs}}
}
{{/functions}}
`;
    }
}
