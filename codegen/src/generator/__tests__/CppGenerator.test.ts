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
        isOutput?: boolean;
        category?: TypeCategory;
        registryName?: string;
        baseType?: string;
    } = {}
): ParamInfo {
    const {
        isPointer = false,
        isConst = false,
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
        isReference: false,
        registryName,
        cppType: effectiveBaseType,
        jsonType: getJsonType(category, effectiveBaseType)
    } : undefined;

    return {
        name,
        type,
        isPointer,
        isConst,
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

        it('should use _ptr variable for handle lookup', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('art_ptr');
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

        it('should extract handle value with _val suffix', () => {
            const func = mockFunction('GetArtType', [
                mockParam('art', 'AIArtHandle', {
                    category: 'Handle',
                    registryName: 'art'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('art_val = *art_ptr');
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
        it('should construct ai::UnicodeString from JSON string', () => {
            const func = mockFunction('SetArtName', [
                mockParam('name', 'ai::UnicodeString', {
                    category: 'String',
                    baseType: 'ai::UnicodeString'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('ai::UnicodeString name(params["name"].get<std::string>())');
        });

        it('should use .as_UTF8() for output UnicodeString', () => {
            const func = mockFunction('GetArtName', [
                mockParam('name', 'ai::UnicodeString*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'String',
                    baseType: 'ai::UnicodeString'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('.as_UTF8()');
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

        it('should declare output UnicodeString properly', () => {
            const func = mockFunction('GetArtName', [
                mockParam('name', 'ai::UnicodeString*', {
                    isPointer: true,
                    isOutput: true,
                    category: 'String',
                    baseType: 'ai::UnicodeString'
                })
            ]);

            const suite = mockSuite([func]);
            const [, source] = generator.generate(suite);

            expect(source.content).toContain('ai::UnicodeString name;');
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
