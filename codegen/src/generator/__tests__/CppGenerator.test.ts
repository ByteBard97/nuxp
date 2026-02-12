import { CppGenerator, GeneratedFile } from '../CppGenerator';
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
    } = {}
): ParamInfo {
    const {
        isPointer = false,
        isConst = false,
        isReference = false,
        isOutput = false,
        category,
        registryName,
        baseType
    } = options;

    const effectiveBaseType = baseType || type.replace(/[*&]/g, '').replace('const ', '').trim();

    const classification: TypeDefinition | undefined = category ? {
        raw: type,
        baseType: effectiveBaseType,
        category: category,
        isPointer,
        isConst,
        isReference,
        registryName,
        cppType: effectiveBaseType,
        jsonType: getJsonType(category, effectiveBaseType)
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
 * Helper to get JSON type for classification
 */
function getJsonType(category: TypeCategory, baseType: string): string {
    switch (category) {
        case 'Handle':
            return 'int32_t';
        case 'String':
            return 'std::string';
        case 'Primitive':
            if (baseType === 'AIBoolean') return 'bool';
            if (baseType === 'AIReal' || baseType === 'ASReal') return 'double';
            if (baseType === 'ai::int16' || baseType === 'short') return 'int16_t';
            return 'int32_t';
        case 'Struct':
            return 'nlohmann::json';
        case 'Enum':
            return 'int32_t';
        case 'Error':
            return 'int32_t';
        case 'Void':
            return 'void';
        default:
            return 'any';
    }
}

/**
 * Helper to create a mock function
 */
function mockFunction(name: string, params: ParamInfo[], suiteName: string = 'AIArtSuite'): FunctionInfo {
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
function mockSuite(functions: FunctionInfo[], name: string = 'AIArtSuite', version: number = 21): SuiteInfo {
    return {
        name,
        version,
        functions
    };
}

describe('CppGenerator', () => {
    let generator: CppGenerator;

    beforeAll(() => {
        generator = new CppGenerator(CONFIG_PATH);
    });

    describe('generate', () => {
        it('should generate header and source files', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                }),
                mockParam('type', 'short*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'short'
                })
            ]);

            const suite = mockSuite([func]);
            const files = generator.generate(suite);

            expect(files).toHaveLength(2);
            expect(files[0].filename).toBe('FloraAIArtSuiteWrapper.h');
            expect(files[1].filename).toBe('FloraAIArtSuiteWrapper.cpp');
        });

        it('should return GeneratedFile objects with content', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [header, source] = generator.generate(suite);

            expect(header.content).toBeTruthy();
            expect(header.content.length).toBeGreaterThan(0);
            expect(source.content).toBeTruthy();
            expect(source.content.length).toBeGreaterThan(0);
        });

        it('should generate files for empty suite with no functions', () => {
            const suite = mockSuite([]);
            const [header, source] = generator.generate(suite);

            expect(header.filename).toBe('FloraAIArtSuiteWrapper.h');
            expect(source.filename).toBe('FloraAIArtSuiteWrapper.cpp');
            expect(header.content).toContain('namespace Flora');
        });
    });

    describe('Header file generation', () => {
        it('should contain #pragma once directive', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [header] = generator.generate(suite);

            expect(header.content).toContain('#pragma once');
        });

        it('should contain correct namespace Flora::SuiteName', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [header] = generator.generate(suite);

            expect(header.content).toContain('namespace Flora {');
            expect(header.content).toContain('namespace AIArtSuite {');
        });

        it('should contain function declarations', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);
            const suite = mockSuite([func]);
            const [header] = generator.generate(suite);

            expect(header.content).toContain('nlohmann::json GetArtType(const nlohmann::json& params);');
        });

        it('should contain nlohmann/json include', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [header] = generator.generate(suite);

            expect(header.content).toContain('#include <nlohmann/json.hpp>');
        });

        it('should contain documentation for input parameters', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);
            const suite = mockSuite([func]);
            const [header] = generator.generate(suite);

            expect(header.content).toContain('@param params["art"]');
        });

        it('should contain documentation for output parameters', () => {
            const func = mockFunction('GetArtType', [
                mockParam('type', 'short*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'short'
                })
            ]);
            const suite = mockSuite([func]);
            const [header] = generator.generate(suite);

            expect(header.content).toContain('@returns ["type"]');
        });
    });

    describe('Source file generation', () => {
        it('should include HandleManager.hpp', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('#include "HandleManager.hpp"');
        });

        it('should include IllustratorSDK.h', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('#include "IllustratorSDK.h"');
        });

        it('should contain extern suite accessor with short name', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('extern "C" AIArtSuite* sArt;');
        });

        it('should return nlohmann::json response', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('nlohmann::json response;');
            expect(source.content).toContain('return response;');
        });
    });

    describe('Handle input marshaling', () => {
        it('should generate HandleManager::registryName.Get for input handles', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    isPointer: false,
                    isConst: false,
                    isOutput: false,
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('HandleManager::art.Get');
        });

        it('should use direct _val variable for handle lookup', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            // Generator uses a single-step pattern: _val = HandleManager::registry.Get(...)
            expect(source.content).toContain('art_val = HandleManager::art.Get');
        });

        it('should throw error for invalid handles', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('throw std::runtime_error');
            expect(source.content).toContain('Invalid AIArtHandle handle');
        });

        it('should assign handle directly from registry Get', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            // Generator assigns _val directly from HandleManager::Get (no intermediate _ptr)
            expect(source.content).toContain('AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>())');
        });

        it('should pass _val variable to SDK function call', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                }),
                mockParam('type', 'short*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'short'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('sArt->GetArtType(art_val,');
        });

        it('should handle optional handles with null check', () => {
            const func = mockFunction('NewArt', [
                mockParam('type', 'short', {
                    category: 'Primitive',
                    baseType: 'short'
                }),
                mockParam('prepArt', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                }),
                mockParam('paintOrder', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            // Optional handles should have special handling with contains check
            expect(source.content).toContain('params.contains("prepArt")');
            expect(source.content).toContain('nullptr');
        });

        it('should use correct registry name for different handle types', () => {
            const func = mockFunction('GetLayerArt', [
                mockParam('layer', 'AILayerHandle', {
                    category: 'Handle',
                    registryName: 'layers'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('HandleManager::layers.Get');
        });
    });

    describe('Handle output marshaling', () => {
        it('should generate HandleManager::registryName.Register for output handles', () => {
            const func = mockFunction('NewArt', [
                mockParam('newArt', 'AIArtHandle*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'art',
                    baseType: 'AIArtHandle'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('HandleManager::art.Register');
        });

        it('should return registered handle ID in response', () => {
            const func = mockFunction('NewArt', [
                mockParam('newArt', 'AIArtHandle*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'art',
                    baseType: 'AIArtHandle'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('response["newArt"] = HandleManager::art.Register(newArt)');
        });

        it('should declare output handle initialized to nullptr', () => {
            const func = mockFunction('NewArt', [
                mockParam('newArt', 'AIArtHandle*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'art',
                    baseType: 'AIArtHandle'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIArtHandle newArt = nullptr;');
        });

        it('should pass output handle with address-of operator', () => {
            const func = mockFunction('NewArt', [
                mockParam('newArt', 'AIArtHandle*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'art',
                    baseType: 'AIArtHandle'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('&newArt');
        });

        it('should return -1 for null output handles', () => {
            const func = mockFunction('NewArt', [
                mockParam('newArt', 'AIArtHandle*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'art',
                    baseType: 'AIArtHandle'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('response["newArt"] = -1');
        });
    });

    describe('String marshaling', () => {
        it('should generate correct marshaling for ai::UnicodeString params', () => {
            const func = mockFunction('GetArtName', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    baseType: 'AIArtHandle',
                    registryName: 'art',
                    isPointer: true,
                    isOutput: false,
                }),
                mockParam('name', 'ai::UnicodeString', {
                    category: 'String',
                    baseType: 'ai::UnicodeString',
                    isReference: true,
                    isOutput: true,
                    isPointer: false,
                    isConst: false,
                }),
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            // UnicodeString functions should now be generated (not blocked)
            expect(source.content).toContain('GetArtName');
            // Output UnicodeString should use .as_UTF8() for marshaling
            expect(source.content).toContain('.as_UTF8()');
            // Output reference param should NOT have & prefix in call args
            expect(source.content).not.toContain('&name');
        });

        it('should handle const char* input strings', () => {
            const func = mockFunction('SetString', [
                mockParam('str', 'const char*', {
                    isPointer: true,
                    isConst: true,
                    category: 'String',
                    baseType: 'const char*'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('std::string str_str = params["str"].get<std::string>()');
            expect(source.content).toContain('const char* str = str_str.c_str()');
        });

        it('should generate correct input marshaling for ai::UnicodeString', () => {
            const func = mockFunction('SetArtName', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    baseType: 'AIArtHandle',
                    registryName: 'art',
                    isPointer: true,
                    isOutput: false,
                }),
                mockParam('name', 'const ai::UnicodeString&', {
                    category: 'String',
                    baseType: 'ai::UnicodeString',
                    isReference: true,
                    isConst: true,
                    isOutput: false,
                    isPointer: false,
                }),
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            // UnicodeString input functions should be generated (not blocked)
            expect(source.content).toContain('SetArtName');
            // Input UnicodeString should construct from std::string
            expect(source.content).toContain('ai::UnicodeString');
        });
    });

    describe('Primitive marshaling', () => {
        it('should use .get<type>() for JSON extraction', () => {
            const func = mockFunction('SetValue', [
                mockParam('value', 'ai::int32', {
                    category: 'Primitive',
                    baseType: 'ai::int32'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('.get<int32_t>()');
        });

        it('should correctly map AIBoolean to bool', () => {
            const func = mockFunction('SetVisible', [
                mockParam('visible', 'AIBoolean', {
                    category: 'Primitive',
                    baseType: 'AIBoolean'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIBoolean visible = params["visible"].get<bool>()');
        });

        it('should correctly map ai::int32', () => {
            const func = mockFunction('SetCount', [
                mockParam('count', 'ai::int32', {
                    category: 'Primitive',
                    baseType: 'ai::int32'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('ai::int32 count = params["count"].get<int32_t>()');
        });

        it('should correctly map AIReal to double', () => {
            const func = mockFunction('SetOpacity', [
                mockParam('opacity', 'AIReal', {
                    category: 'Primitive',
                    baseType: 'AIReal'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIReal opacity = params["opacity"].get<double>()');
        });

        it('should correctly map short to int16_t', () => {
            const func = mockFunction('SetType', [
                mockParam('type', 'short', {
                    category: 'Primitive',
                    baseType: 'short'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('short type = params["type"].get<int16_t>()');
        });

        it('should initialize output primitives with default value', () => {
            const func = mockFunction('GetCount', [
                mockParam('count', 'ai::int32*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'ai::int32'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('ai::int32 count{};');
        });

        it('should cast AIBoolean output to bool for JSON', () => {
            const func = mockFunction('GetVisible', [
                mockParam('visible', 'AIBoolean*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'AIBoolean'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('static_cast<bool>(visible)');
        });
    });

    describe('Struct marshaling', () => {
        it('should correctly marshal AIRealRect with left/top/right/bottom fields', () => {
            const func = mockFunction('GetArtBounds', [
                mockParam('bounds', 'AIRealRect*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Struct',
                    baseType: 'AIRealRect'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('"left"');
            expect(source.content).toContain('"top"');
            expect(source.content).toContain('"right"');
            expect(source.content).toContain('"bottom"');
        });

        it('should use nested JSON for struct fields', () => {
            const func = mockFunction('GetArtBounds', [
                mockParam('bounds', 'AIRealRect*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Struct',
                    baseType: 'AIRealRect'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('response["bounds"] = {');
            expect(source.content).toContain('bounds.left');
            expect(source.content).toContain('bounds.top');
            expect(source.content).toContain('bounds.right');
            expect(source.content).toContain('bounds.bottom');
        });

        it('should unmarshal input struct from nested JSON', () => {
            const func = mockFunction('SetArtBounds', [
                mockParam('bounds', 'AIRealRect', {
                    category: 'Struct',
                    baseType: 'AIRealRect'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('params["bounds"]["left"]');
            expect(source.content).toContain('params["bounds"]["top"]');
            expect(source.content).toContain('params["bounds"]["right"]');
            expect(source.content).toContain('params["bounds"]["bottom"]');
        });

        it('should initialize output struct with default value', () => {
            const func = mockFunction('GetArtBounds', [
                mockParam('bounds', 'AIRealRect*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Struct',
                    baseType: 'AIRealRect'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIRealRect bounds{};');
        });

        it('should correctly marshal AIRealPoint with h/v fields', () => {
            const func = mockFunction('GetPosition', [
                mockParam('pos', 'AIRealPoint*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Struct',
                    baseType: 'AIRealPoint'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('"h"');
            expect(source.content).toContain('"v"');
        });

        it('should correctly marshal AIRealMatrix with all fields', () => {
            const func = mockFunction('GetTransform', [
                mockParam('matrix', 'AIRealMatrix*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Struct',
                    baseType: 'AIRealMatrix'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('"a"');
            expect(source.content).toContain('"b"');
            expect(source.content).toContain('"c"');
            expect(source.content).toContain('"d"');
            expect(source.content).toContain('"tx"');
            expect(source.content).toContain('"ty"');
        });
    });

    describe('SDK call generation', () => {
        it('should contain sShortName->FunctionName(...) pattern', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                }),
                mockParam('type', 'short*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Primitive',
                    baseType: 'short'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('sArt->GetArtType(');
        });

        it('should check AIErr return and throw on error', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIErr err = sArt->GetArtType');
            expect(source.content).toContain('if (err != kNoErr)');
            expect(source.content).toContain('throw std::runtime_error("GetArtType failed with error:');
        });

        it('should use std::to_string for error code', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('std::to_string(err)');
        });

        it('should extract correct short name from AIArtSuite', () => {
            const func = mockFunction('GetArtType', []);
            const suite = mockSuite([func], 'AIArtSuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('sArt->');
        });

        it('should extract correct short name from AILayerSuite', () => {
            const func = mockFunction('GetLayerCount', [], 'AILayerSuite');
            const suite = mockSuite([func], 'AILayerSuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('sLayer->');
        });

        it('should extract correct short name from AIDocumentSuite', () => {
            const func = mockFunction('GetDocumentCount', [], 'AIDocumentSuite');
            const suite = mockSuite([func], 'AIDocumentSuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('sDocument->');
        });
    });

    describe('Enum marshaling', () => {
        it('should cast input enum from int32_t', () => {
            const func = mockFunction('SetArtType', [
                mockParam('artType', 'AIArtType', {
                    category: 'Enum',
                    baseType: 'AIArtType'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('static_cast<AIArtType>(params["artType"].get<int32_t>())');
        });

        it('should cast output enum to int32_t for JSON', () => {
            const func = mockFunction('GetArtType', [
                mockParam('artType', 'AIArtType*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Enum',
                    baseType: 'AIArtType'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('static_cast<int32_t>(artType)');
        });
    });

    describe('Error type marshaling', () => {
        it('should handle AIErr input parameters', () => {
            const func = mockFunction('SetError', [
                mockParam('error', 'AIErr', {
                    category: 'Error',
                    baseType: 'AIErr'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIErr error = params["error"].get<int32_t>()');
        });

        it('should initialize output error to kNoErr', () => {
            const func = mockFunction('GetError', [
                mockParam('error', 'AIErr*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Error',
                    baseType: 'AIErr'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AIErr error = kNoErr;');
        });
    });

    describe('Multiple parameters', () => {
        it('should handle function with mixed parameter types', () => {
            const func = mockFunction('TransformArt', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                }),
                mockParam('matrix', 'AIRealMatrix', {
                    category: 'Struct',
                    baseType: 'AIRealMatrix'
                }),
                mockParam('scale', 'AIReal', {
                    category: 'Primitive',
                    baseType: 'AIReal'
                }),
                mockParam('result', 'AIArtHandle*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'Handle',
                    registryName: 'art',
                    baseType: 'AIArtHandle'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            // Check all types are handled
            expect(source.content).toContain('HandleManager::art.Get');
            expect(source.content).toContain('AIRealMatrix matrix;');
            expect(source.content).toContain('AIReal scale = params["scale"].get<double>()');
            expect(source.content).toContain('HandleManager::art.Register');
        });

        it('should separate call arguments with commas', () => {
            const func = mockFunction('MultipleParams', [
                mockParam('a', 'ai::int32', {
                    category: 'Primitive',
                    baseType: 'ai::int32'
                }),
                mockParam('b', 'ai::int32', {
                    category: 'Primitive',
                    baseType: 'ai::int32'
                }),
                mockParam('c', 'ai::int32', {
                    category: 'Primitive',
                    baseType: 'ai::int32'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('sArt->MultipleParams(a, b, c)');
        });
    });

    describe('Unknown types', () => {
        it('should add warning comment for unknown types', () => {
            const func = mockFunction('UnknownFunc', [
                mockParam('unknown', 'SomeUnknownType', {})
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('unknown type');
        });
    });

    describe('FilePath marshaling', () => {
        it('should generate double-wrap constructor for ai::FilePath input', () => {
            const func = mockFunction('WriteDocument', [
                mockParam('path', 'ai::FilePath', {
                    category: 'String',
                    baseType: 'ai::FilePath',
                    isReference: true,
                    isConst: true,
                    isOutput: false,
                }),
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('WriteDocument');
            expect(source.content).toContain('ai::FilePath path(ai::UnicodeString(params["path"].get<std::string>()))');
        });

        it('should generate GetFullPath().as_UTF8() for ai::FilePath output', () => {
            const func = mockFunction('GetDocumentFileSpecification', [
                mockParam('path', 'ai::FilePath', {
                    category: 'String',
                    baseType: 'ai::FilePath',
                    isReference: true,
                    isOutput: true,
                    isPointer: false,
                }),
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('GetDocumentFileSpecification');
            expect(source.content).toContain('ai::FilePath path;');
            expect(source.content).toContain('path.GetFullPath().as_UTF8()');
        });

        it('should not filter out ai::FilePath functions (appears in dispatch)', () => {
            const func = mockFunction('WriteDocument', [
                mockParam('path', 'ai::FilePath', {
                    category: 'String',
                    baseType: 'ai::FilePath',
                    isConst: true,
                    isReference: true,
                }),
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('method == "WriteDocument"');
        });
    });

    describe('Non-standard return types', () => {
        it('should generate response["result"] = result for AIReal return', () => {
            const func: FunctionInfo = {
                name: 'GetOpacity',
                returnType: 'AIReal',
                params: [
                    mockParam('art', 'AIArtHandle', {
                        category: 'Handle',
                        registryName: 'art',
                    }),
                ],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('GetOpacity');
            expect(source.content).toContain('AIReal result = sMask->GetOpacity(');
            expect(source.content).toContain('response["result"] = result');
        });

        it('should generate HandleManager::Register for AIArtHandle return', () => {
            const func: FunctionInfo = {
                name: 'GetArt',
                returnType: 'AIArtHandle',
                params: [],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('GetArt');
            expect(source.content).toContain('AIArtHandle result = sMask->GetArt(');
            expect(source.content).toContain('HandleManager::art.Register(result)');
            expect(source.content).toContain('response["result"] = -1');
        });

        it('should generate response["result"] = result for primitive enum return (AIKnockout)', () => {
            const func: FunctionInfo = {
                name: 'GetKnockout',
                returnType: 'AIKnockout',
                params: [
                    mockParam('art', 'AIArtHandle', {
                        category: 'Handle',
                        registryName: 'art',
                    }),
                ],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('GetKnockout');
            expect(source.content).toContain('AIKnockout result = sMask->GetKnockout(');
            expect(source.content).toContain('response["result"] = result');
        });

        it('should generate response["result"] = result for ai::int32 return', () => {
            const func: FunctionInfo = {
                name: 'AddRef',
                returnType: 'ai::int32',
                params: [],
                suiteName: 'AIDictionarySuite',
            };

            const suite = mockSuite([func], 'AIDictionarySuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('AddRef');
            expect(source.content).toContain('ai::int32 result = sDictionary->AddRef(');
            expect(source.content).toContain('response["result"] = result');
        });

        it('should still filter out unknown return types', () => {
            const func: FunctionInfo = {
                name: 'GetSomething',
                returnType: 'SomeUnknownType',
                params: [],
                suiteName: 'AIArtSuite',
            };

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).not.toContain('GetSomething');
        });

        it('should document non-standard return types in header', () => {
            const func: FunctionInfo = {
                name: 'GetOpacity',
                returnType: 'AIReal',
                params: [],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const [header] = generator.generate(suite);

            expect(header.content).toContain('@returns ["result"] - AIReal value');
        });

        it('should use as_UTF8() for ai::UnicodeString return', () => {
            const func: FunctionInfo = {
                name: 'GetName',
                returnType: 'ai::UnicodeString',
                params: [],
                suiteName: 'AIDictionarySuite',
            };

            const suite = mockSuite([func], 'AIDictionarySuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('ai::UnicodeString result = sDictionary->GetName(');
            expect(source.content).toContain('result.as_UTF8()');
            expect(source.content).not.toContain('std::string(result)');
        });

        it('should use GetFullPath().as_UTF8() for ai::FilePath return', () => {
            const func: FunctionInfo = {
                name: 'GetPath',
                returnType: 'ai::FilePath',
                params: [],
                suiteName: 'AIDictionarySuite',
            };

            const suite = mockSuite([func], 'AIDictionarySuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('ai::FilePath result = sDictionary->GetPath(');
            expect(source.content).toContain('result.GetFullPath().as_UTF8()');
            expect(source.content).not.toContain('std::string(result)');
        });

        it('should use std::string(result) for const char* return', () => {
            const func: FunctionInfo = {
                name: 'GetCString',
                returnType: 'const char*',
                params: [],
                suiteName: 'AIDictionarySuite',
            };

            const suite = mockSuite([func], 'AIDictionarySuite');
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('result ? std::string(result) : ""');
        });

        it('should document handle return types in header', () => {
            const func: FunctionInfo = {
                name: 'GetArt',
                returnType: 'AIArtHandle',
                params: [],
                suiteName: 'AIMaskSuite',
            };

            const suite = mockSuite([func], 'AIMaskSuite');
            const [header] = generator.generate(suite);

            expect(header.content).toContain('@returns ["result"] - handle ID (from AIArtHandle return)');
        });
    });

    describe('Multiple functions in suite', () => {
        it('should generate wrappers for all functions', () => {
            const func1 = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);
            const func2 = mockFunction('SetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                }),
                mockParam('type', 'short', {
                    category: 'Primitive',
                    baseType: 'short'
                })
            ]);

            const suite = mockSuite([func1, func2]);
            const [header, source] = generator.generate(suite);

            expect(header.content).toContain('nlohmann::json GetArtType(const nlohmann::json& params);');
            expect(header.content).toContain('nlohmann::json SetArtType(const nlohmann::json& params);');
            expect(source.content).toContain('nlohmann::json GetArtType(const nlohmann::json& params)');
            expect(source.content).toContain('nlohmann::json SetArtType(const nlohmann::json& params)');
        });
    });
});
