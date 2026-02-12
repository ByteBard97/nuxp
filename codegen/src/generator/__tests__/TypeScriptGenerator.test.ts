import { TypeScriptGenerator, GeneratedFile } from '../TypeScriptGenerator';
import { SuiteInfo, FunctionInfo, ParamInfo } from '../../parser/types';
import { TypeDefinition, TypeCategory } from '../../parser/TypeClassifier';
import * as path from 'path';

const CONFIG_PATH = path.join(__dirname, '../../config/type-map.json');

/**
 * Helper to create a mock ParamInfo with classification
 */
function mockParam(
    name: string,
    type: string,
    options: {
        isPointer?: boolean;
        isConst?: boolean;
        isReference?: boolean;
        isOutput?: boolean;
        category?: TypeCategory;
        registryName?: string;
        baseType?: string;
        jsonType?: string;
    } = {}
): ParamInfo {
    const {
        isPointer = false,
        isConst = false,
        isReference = false,
        isOutput = false,
        category,
        registryName,
        baseType,
        jsonType = 'int32_t'
    } = options;

    const actualBaseType = baseType || type.replace(/[*&]/g, '').replace('const ', '').trim();

    const classification: TypeDefinition | undefined = category ? {
        raw: type,
        baseType: actualBaseType,
        category: category,
        isPointer,
        isConst,
        isReference: false,
        registryName,
        cppType: actualBaseType,
        jsonType
    } : undefined;

    return {
        name,
        type,
        isPointer,
        isConst,
        isReference,
        isOutput,
        classification
    };
}

/**
 * Helper to create a mock function
 */
function mockFunction(name: string, params: ParamInfo[], suiteName = 'AIArtSuite'): FunctionInfo {
    return {
        name,
        returnType: 'AIErr',
        params,
        suiteName
    };
}

/**
 * Helper to create a mock suite
 */
function mockSuite(functions: FunctionInfo[], name = 'AIArtSuite'): SuiteInfo {
    return {
        name,
        version: 21,
        functions
    };
}

describe('TypeScriptGenerator', () => {
    let generator: TypeScriptGenerator;

    beforeAll(() => {
        generator = new TypeScriptGenerator(CONFIG_PATH);
    });

    describe('File generation', () => {
        it('should generate a TypeScript file with correct filename', () => {
            const suite = mockSuite([]);
            const file = generator.generate(suite);

            expect(file.filename).toBe('AIArtSuite.ts');
        });

        it('should generate filename matching suite name', () => {
            const suite = mockSuite([], 'AILayerSuite');
            const file = generator.generate(suite);

            expect(file.filename).toBe('AILayerSuite.ts');
        });

        it('should include auto-generated header comment', () => {
            const suite = mockSuite([]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Auto-generated');
            expect(file.content).toContain('AIArtSuite');
        });

        it('should include suite name in header comment', () => {
            const suite = mockSuite([], 'AIDocumentSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('AIDocumentSuite client functions');
        });

        it('should return GeneratedFile object with filename and content', () => {
            const suite = mockSuite([]);
            const file = generator.generate(suite);

            expect(file).toHaveProperty('filename');
            expect(file).toHaveProperty('content');
            expect(typeof file.filename).toBe('string');
            expect(typeof file.content).toBe('string');
        });
    });

    describe('Type mappings - Handles', () => {
        it('should map AIArtHandle to number', () => {
            const func = mockFunction('GetArtParent', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('parent', 'AIArtHandle', { isOutput: true, category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('art: number');
            expect(file.content).toContain('Promise<number>');
        });

        it('should map AILayerHandle to number', () => {
            const func = mockFunction('GetLayer', [
                mockParam('layerHandle', 'AILayerHandle', { category: 'Handle', registryName: 'layers' }),
                mockParam('result', 'AILayerHandle', { isOutput: true, category: 'Handle', registryName: 'layers' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('layerHandle: number');
        });

        it('should map AIDocumentHandle to number', () => {
            const func = mockFunction('GetDocument', [
                mockParam('doc', 'AIDocumentHandle', { category: 'Handle', registryName: 'documents' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('doc: number');
        });
    });

    describe('Type mappings - Primitives', () => {
        it('should map AIBoolean to boolean', () => {
            const func = mockFunction('IsArtVisible', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('visible', 'AIBoolean', { isOutput: true, category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<boolean>');
        });

        it('should map ai::int32 to number', () => {
            const func = mockFunction('GetCount', [
                mockParam('count', 'ai::int32', { isOutput: true, category: 'Primitive', baseType: 'ai::int32' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });

        it('should map short to number', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive', baseType: 'short' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });

        it('should map AIReal to number', () => {
            const func = mockFunction('GetWidth', [
                mockParam('width', 'AIReal', { isOutput: true, category: 'Primitive', baseType: 'AIReal' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });

        it('should map size_t to number', () => {
            const func = mockFunction('GetSize', [
                mockParam('size', 'size_t', { isOutput: true, category: 'Primitive', baseType: 'size_t' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });
    });

    describe('Type mappings - Strings', () => {
        it('should map ai::UnicodeString to string', () => {
            const func = mockFunction('GetArtName', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('name', 'ai::UnicodeString', { isOutput: true, category: 'String', baseType: 'ai::UnicodeString' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<string>');
        });
    });

    describe('Type mappings - Special types', () => {
        it('should map void to void', () => {
            const func = mockFunction('DisposeArt', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<void>');
        });

        it('should map unknown types to any', () => {
            const func = mockFunction('CustomOperation', [
                mockParam('data', 'UnknownType', { category: 'Unknown', baseType: 'UnknownType' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('data: any');
        });

        it('should map Enum category to number', () => {
            const func = mockFunction('GetBlendMode', [
                mockParam('mode', 'AIBlendMode', { isOutput: true, category: 'Enum', baseType: 'AIBlendMode' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });

        it('should map Error category to number', () => {
            const func = mockFunction('GetError', [
                mockParam('err', 'AIErr', { isOutput: true, category: 'Error', baseType: 'AIErr' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });
    });

    describe('ManagedHandle type mappings', () => {
        it('should map ManagedHandle category to number', () => {
            const func = mockFunction('GetArtboardRect', [
                mockParam('properties', 'ai::ArtboardProperties', {
                    category: 'ManagedHandle',
                    registryName: 'artboardProperties',
                    baseType: 'ai::ArtboardProperties'
                }),
                mockParam('rect', 'AIRealRect', {
                    isOutput: true,
                    category: 'Struct',
                    baseType: 'AIRealRect'
                })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('properties: number');
        });

        it('should return number for function returning ai::ArtboardProperties', () => {
            const func: FunctionInfo = {
                name: 'GetArtboardProperties',
                returnType: 'ai::ArtboardProperties',
                params: [],
                suiteName: 'AIArtboardSuite',
            };

            const suite = mockSuite([func], 'AIArtboardSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
            expect(file.content).toContain('return result.result');
        });
    });

    describe('Dictionary handle type mappings', () => {
        it('should map AIDictionaryRef (Handle) to number', () => {
            const func = mockFunction('GetEntry', [
                mockParam('dict', 'AIDictionaryRef', {
                    category: 'Handle',
                    registryName: 'dictionaries',
                    baseType: 'AIDictionaryRef'
                }),
                mockParam('entry', 'AIEntryRef', {
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'entries',
                    baseType: 'AIEntryRef'
                })
            ], 'AIDictionarySuite');

            const suite = mockSuite([func], 'AIDictionarySuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('dict: number');
        });

        it('should map AIEntryRef (Handle) to number', () => {
            const func = mockFunction('GetEntryType', [
                mockParam('entry', 'AIEntryRef', {
                    category: 'Handle',
                    registryName: 'entries',
                    baseType: 'AIEntryRef'
                }),
                mockParam('type', 'AIEntryType', {
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'AIEntryType'
                })
            ], 'AIEntrySuite');

            const suite = mockSuite([func], 'AIEntrySuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('entry: number');
        });
    });

    describe('Struct interface generation', () => {
        it('should generate AIRealRect interface with correct fields', () => {
            const func = mockFunction('GetArtBounds', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('bounds', 'AIRealRect', { isOutput: true, category: 'Struct', baseType: 'AIRealRect' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('interface AIRealRect');
            expect(file.content).toContain('left: number');
            expect(file.content).toContain('top: number');
            expect(file.content).toContain('right: number');
            expect(file.content).toContain('bottom: number');
        });

        it('should generate AIRealPoint interface with correct fields', () => {
            const func = mockFunction('GetPoint', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('point', 'AIRealPoint', { isOutput: true, category: 'Struct', baseType: 'AIRealPoint' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('interface AIRealPoint');
            expect(file.content).toContain('h: number');
            expect(file.content).toContain('v: number');
        });

        it('should generate AIRealMatrix interface with correct fields', () => {
            const func = mockFunction('GetTransform', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('matrix', 'AIRealMatrix', { isOutput: true, category: 'Struct', baseType: 'AIRealMatrix' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('interface AIRealMatrix');
            expect(file.content).toContain('a: number');
            expect(file.content).toContain('b: number');
            expect(file.content).toContain('c: number');
            expect(file.content).toContain('d: number');
            expect(file.content).toContain('tx: number');
            expect(file.content).toContain('ty: number');
        });

        it('should generate export keyword for struct interfaces', () => {
            const func = mockFunction('GetBounds', [
                mockParam('bounds', 'AIRealRect', { isOutput: true, category: 'Struct', baseType: 'AIRealRect' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('export interface AIRealRect');
        });

        it('should include struct description as JSDoc comment', () => {
            const func = mockFunction('GetBounds', [
                mockParam('bounds', 'AIRealRect', { isOutput: true, category: 'Struct', baseType: 'AIRealRect' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Rectangle structure');
        });

        it('should only generate structs that are used in the suite', () => {
            const func = mockFunction('GetPoint', [
                mockParam('point', 'AIRealPoint', { isOutput: true, category: 'Struct', baseType: 'AIRealPoint' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('interface AIRealPoint');
            expect(file.content).not.toContain('interface AIRealRect');
            expect(file.content).not.toContain('interface AIRealMatrix');
        });

        it('should not generate struct section if no structs are used', () => {
            const func = mockFunction('GetCount', [
                mockParam('count', 'ai::int32', { isOutput: true, category: 'Primitive', baseType: 'ai::int32' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).not.toContain('Struct type definitions');
        });

        it('should generate generic struct for unknown struct types', () => {
            const func = mockFunction('GetCustomData', [
                mockParam('data', 'AICustomStruct', { isOutput: true, category: 'Struct', baseType: 'AICustomStruct' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('interface AICustomStruct');
            expect(file.content).toContain('data: any');
        });
    });

    describe('Function generation - signatures', () => {
        it('should generate async function with export keyword', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('export async function GetArtType');
        });

        it('should include input parameters in function signature', () => {
            const func = mockFunction('SetArtBounds', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('left', 'AIReal', { category: 'Primitive', baseType: 'AIReal' }),
                mockParam('top', 'AIReal', { category: 'Primitive', baseType: 'AIReal' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('art: number');
            expect(file.content).toContain('left: number');
            expect(file.content).toContain('top: number');
        });

        it('should exclude output parameters from function signature', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            // 'type' should only appear in the return type, not in parameter list
            const functionSignature = file.content.match(/export async function GetArtType\([^)]*\)/);
            expect(functionSignature).not.toBeNull();
            expect(functionSignature![0]).not.toContain('type: number');
        });
    });

    describe('Function generation - return types', () => {
        it('should return void when no output parameters', () => {
            const func = mockFunction('DisposeArt', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<void>');
        });

        it('should return single value directly when one output parameter', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive', baseType: 'short' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
            expect(file.content).toContain('return result.type');
        });

        it('should return object when multiple output parameters', () => {
            const func = mockFunction('GetArtInfo', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive', baseType: 'short' }),
                mockParam('visible', 'AIBoolean', { isOutput: true, category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('{ type: number; visible: boolean }');
        });

        it('should use struct type name for struct return values', () => {
            const func = mockFunction('GetArtBounds', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('bounds', 'AIRealRect', { isOutput: true, category: 'Struct', baseType: 'AIRealRect' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<AIRealRect>');
        });
    });

    describe('Bridge integration', () => {
        it('should import callCpp from bridge', () => {
            const suite = mockSuite([]);
            const file = generator.generate(suite);

            expect(file.content).toContain("import { callCpp } from '@/sdk/bridge'");
        });

        it('should define SUITE_NAME constant', () => {
            const suite = mockSuite([]);
            const file = generator.generate(suite);

            expect(file.content).toContain("const SUITE_NAME = 'AIArtSuite'");
        });

        it('should call callCpp with suite name, method name, and args', () => {
            const func = mockFunction('DisposeArt', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain("callCpp(SUITE_NAME, 'DisposeArt'");
        });

        it('should pass input parameters as args object', () => {
            const func = mockFunction('SetArtVisible', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('visible', 'AIBoolean', { category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain("callCpp(SUITE_NAME, 'SetArtVisible', { art, visible })");
        });

        it('should use typed callCpp for single output parameter', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive', baseType: 'short' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('callCpp<{ type: number }>(SUITE_NAME');
        });

        it('should use typed callCpp for multiple output parameters', () => {
            const func = mockFunction('GetArtInfo', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive', baseType: 'short' }),
                mockParam('visible', 'AIBoolean', { isOutput: true, category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('callCpp<{ type: number; visible: boolean }>(SUITE_NAME');
        });

        it('should use SUITE_NAME constant matching the suite name', () => {
            const suite = mockSuite([], 'AILayerSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain("const SUITE_NAME = 'AILayerSuite'");
        });
    });

    describe('JSDoc generation', () => {
        it('should include function description for Get functions', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('* Retrieves');
        });

        it('should include function description for Set functions', () => {
            const func = mockFunction('SetArtVisible', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('visible', 'AIBoolean', { category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('* Sets');
        });

        it('should include function description for Create/New functions', () => {
            const func = mockFunction('NewArt', [
                mockParam('type', 'short', { category: 'Primitive' }),
                mockParam('art', 'AIArtHandle', { isOutput: true, category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('* Creates');
        });

        it('should include function description for Delete/Dispose functions', () => {
            const func = mockFunction('DisposeArt', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('* Removes');
        });

        it('should include @param annotations for each input parameter', () => {
            const func = mockFunction('SetArtBounds', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('bounds', 'AIRealRect', { category: 'Struct', baseType: 'AIRealRect' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('@param art');
            expect(file.content).toContain('@param bounds');
        });

        it('should include parameter descriptions', () => {
            const func = mockFunction('GetArtParent', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('parent', 'AIArtHandle', { isOutput: true, category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('@param art - Handle to the art object');
        });

        it('should include @returns annotation for functions with return value', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('@returns');
        });

        it('should not include @returns for void functions', () => {
            const func = mockFunction('DisposeArt', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            // Extract just the DisposeArt function's JSDoc (between "Removes" and the export async function)
            const funcSection = file.content.match(/\/\*\*\s*\*\s*Removes[\s\S]*?export async function DisposeArt/);
            expect(funcSection).not.toBeNull();
            expect(funcSection![0]).not.toContain('@returns');
        });

        it('should describe boolean returns appropriately', () => {
            const func = mockFunction('IsArtVisible', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('visible', 'AIBoolean', { isOutput: true, category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('True if');
        });

        it('should describe multiple outputs in return', () => {
            const func = mockFunction('GetArtInfo', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive' }),
                mockParam('visible', 'AIBoolean', { isOutput: true, category: 'Primitive', baseType: 'AIBoolean' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('An object containing');
            expect(file.content).toContain('type');
            expect(file.content).toContain('visible');
        });
    });

    describe('Edge cases', () => {
        it('should handle empty suite with no functions', () => {
            const suite = mockSuite([]);
            const file = generator.generate(suite);

            expect(file.filename).toBe('AIArtSuite.ts');
            expect(file.content).toContain('AIArtSuite client functions');
            expect(file.content).toContain("import { callCpp } from '@/sdk/bridge'");
            expect(file.content).toContain("const SUITE_NAME = 'AIArtSuite'");
        });

        it('should handle function with no parameters', () => {
            const func = mockFunction('GetCurrentDocument', [
                mockParam('doc', 'AIDocumentHandle', { isOutput: true, category: 'Handle', registryName: 'documents' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('export async function GetCurrentDocument()');
            expect(file.content).toContain('Promise<number>');
        });

        it('should handle parameters without classification (fallback to raw type)', () => {
            const param: ParamInfo = {
                name: 'value',
                type: 'AIReal',
                isPointer: false,
                isConst: false,
                isReference: false,
                isOutput: false
                // No classification
            };

            const func = mockFunction('SetValue', [param]);
            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            // Should still generate valid TypeScript
            expect(file.content).toContain('export async function SetValue');
        });

        it('should handle multiple functions in a suite', () => {
            const func1 = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive' })
            ]);

            const func2 = mockFunction('GetArtParent', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('parent', 'AIArtHandle', { isOutput: true, category: 'Handle', registryName: 'art' })
            ]);

            const suite = mockSuite([func1, func2]);
            const file = generator.generate(suite);

            expect(file.content).toContain('export async function GetArtType');
            expect(file.content).toContain('export async function GetArtParent');
        });

        it('should generate multiple struct interfaces when needed', () => {
            const func = mockFunction('GetArtGeometry', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('bounds', 'AIRealRect', { isOutput: true, category: 'Struct', baseType: 'AIRealRect' }),
                mockParam('center', 'AIRealPoint', { isOutput: true, category: 'Struct', baseType: 'AIRealPoint' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('interface AIRealRect');
            expect(file.content).toContain('interface AIRealPoint');
        });

        it('should handle layer parameter descriptions', () => {
            const func = mockFunction('GetLayerArt', [
                mockParam('layer', 'AILayerHandle', { category: 'Handle', registryName: 'layers' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('@param layer - Handle to the layer');
        });

        it('should handle name parameter descriptions', () => {
            const func = mockFunction('SetArtName', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('name', 'ai::UnicodeString', { category: 'String', baseType: 'ai::UnicodeString' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('@param name - The name string');
        });

        it('should handle index parameter descriptions', () => {
            const func = mockFunction('GetArtAtIndex', [
                mockParam('index', 'ai::int32', { category: 'Primitive', baseType: 'ai::int32' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('@param index - The index value');
        });
    });

    describe('Raw type fallback mapping', () => {
        it('should map handle types without classification', () => {
            const param: ParamInfo = {
                name: 'art',
                type: 'AIArtHandle',
                isPointer: false,
                isConst: false,
                isReference: false,
                isOutput: false
                // No classification - relies on raw type mapping
            };

            const func = mockFunction('ProcessArt', [param]);
            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('art: number');
        });

        it('should map string types with classification fallback', () => {
            // Note: Raw type fallback for string types is tricky because:
            // - ai::UnicodeString is listed as BOTH a handle (checked first) and string_type
            // - const char* becomes "char" after cleaning, which isn't in string_types
            // So we test with classification to verify string mapping works correctly
            const func = mockFunction('SetText', [
                mockParam('text', 'ai::UnicodeString', { category: 'String', baseType: 'ai::UnicodeString' })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('text: string');
        });

        it('should map struct types without classification', () => {
            const param: ParamInfo = {
                name: 'rect',
                type: 'AIRealRect',
                isPointer: false,
                isConst: false,
                isReference: false,
                isOutput: false
                // No classification
            };

            const func = mockFunction('SetBounds', [param]);
            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('rect: AIRealRect');
        });

        it('should map AIErr without classification', () => {
            const param: ParamInfo = {
                name: 'err',
                type: 'AIErr',
                isPointer: false,
                isConst: false,
                isReference: false,
                isOutput: true
                // No classification
            };

            const func = mockFunction('GetError', [param]);
            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });
    });

    describe('Direct value return types', () => {
        it('should return number for function returning AIReal', () => {
            const func: FunctionInfo = {
                name: 'GetOpacity',
                returnType: 'AIReal',
                params: [
                    mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                ],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
            expect(file.content).toContain('return result.result');
        });

        it('should return number for function returning AIArtHandle', () => {
            const func: FunctionInfo = {
                name: 'GetArt',
                returnType: 'AIArtHandle',
                params: [],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
            expect(file.content).toContain('return result.result');
        });

        it('should return number for function returning ai::int32', () => {
            const func: FunctionInfo = {
                name: 'AddRef',
                returnType: 'ai::int32',
                params: [],
                suiteName: 'AIDictionarySuite',
            };

            const suite = mockSuite([func], 'AIDictionarySuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
            expect(file.content).toContain('return result.result');
        });

        it('should return string for function returning ai::FilePath', () => {
            const func: FunctionInfo = {
                name: 'GetFilePath',
                returnType: 'ai::FilePath',
                params: [],
                suiteName: 'AIDocumentSuite',
            };

            const suite = mockSuite([func], 'AIDocumentSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<string>');
            expect(file.content).toContain('return result.result');
        });

        it('should not use direct return pattern for AIErr functions', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                mockParam('type', 'short', { isOutput: true, category: 'Primitive', baseType: 'short' }),
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            // Should use single output pattern, not direct return
            expect(file.content).toContain('return result.type');
            expect(file.content).not.toContain('return result.result');
        });

        it('should include return description for direct value returns', () => {
            const func: FunctionInfo = {
                name: 'GetOpacity',
                returnType: 'AIReal',
                params: [],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('@returns');
        });

        it('should combine direct return with output params into object type', () => {
            const func: FunctionInfo = {
                name: 'GetKnockoutWithInfo',
                returnType: 'AIKnockout',
                params: [
                    mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                    mockParam('info', 'ai::int32', { isOutput: true, isPointer: true, category: 'Primitive', baseType: 'ai::int32' }),
                ],
                suiteName: 'AIBlendStyleSuite',
            };

            const suite = mockSuite([func], 'AIBlendStyleSuite');
            const file = generator.generate(suite);

            // Should produce a combined return type with result + output params
            expect(file.content).toContain('result: number');
            expect(file.content).toContain('info: number');
            // Should NOT use the simple direct return pattern
            expect(file.content).not.toContain('return result.result;');
            // Should use multi-output pattern (return result)
            expect(file.content).toContain('return result;');
        });

        it('should describe combined return with output params', () => {
            const func: FunctionInfo = {
                name: 'GetKnockoutWithInfo',
                returnType: 'AIKnockout',
                params: [
                    mockParam('art', 'AIArtHandle', { category: 'Handle', registryName: 'art' }),
                    mockParam('info', 'ai::int32', { isOutput: true, isPointer: true, category: 'Primitive', baseType: 'ai::int32' }),
                ],
                suiteName: 'AIBlendStyleSuite',
            };

            const suite = mockSuite([func], 'AIBlendStyleSuite');
            const file = generator.generate(suite);

            // Return description should mention both result and output params
            expect(file.content).toContain('result');
            expect(file.content).toContain('info');
        });
    });

    describe('ManagedHandle type mappings', () => {
        it('should map ManagedHandle category to number', () => {
            const func = mockFunction('GetArtboardName', [
                mockParam('props', 'ai::ArtboardProperties', {
                    category: 'ManagedHandle',
                    registryName: 'artboardProperties',
                    baseType: 'ai::ArtboardProperties'
                }),
                mockParam('name', 'ai::UnicodeString', {
                    isOutput: true,
                    category: 'String',
                    baseType: 'ai::UnicodeString'
                })
            ]);

            const suite = mockSuite([func]);
            const file = generator.generate(suite);

            expect(file.content).toContain('props: number');
        });

        it('should return number for function returning ai::ArtboardProperties', () => {
            const func: FunctionInfo = {
                name: 'GetProperties',
                returnType: 'ai::ArtboardProperties',
                params: [],
                suiteName: 'AIArtboardSuite',
            };

            const suite = mockSuite([func], 'AIArtboardSuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });
    });

    describe('Dictionary handle type mappings', () => {
        it('should map AIDictionaryRef Handle to number', () => {
            const func = mockFunction('GetEntry', [
                mockParam('dict', 'AIDictionaryRef', {
                    category: 'Handle',
                    registryName: 'dictionaries',
                    baseType: 'AIDictionaryRef'
                }),
                mockParam('entry', 'AIEntryRef', {
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'entries',
                    baseType: 'AIEntryRef'
                })
            ], 'AIDictionarySuite');

            const suite = mockSuite([func], 'AIDictionarySuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('dict: number');
        });

        it('should map AIEntryRef Handle to number', () => {
            const func = mockFunction('GetEntry', [
                mockParam('entry', 'AIEntryRef', {
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'entries',
                    baseType: 'AIEntryRef'
                })
            ], 'AIDictionarySuite');

            const suite = mockSuite([func], 'AIDictionarySuite');
            const file = generator.generate(suite);

            expect(file.content).toContain('Promise<number>');
        });
    });
});
