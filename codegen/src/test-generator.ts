import { CppGenerator, GeneratedFile } from './generator/CppGenerator';
import { SuiteInfo, FunctionInfo, ParamInfo } from './parser/types';
import { TypeDefinition, TypeClassifier } from './parser/TypeClassifier';
import * as path from 'path';

// Create a mock suite to test the generator
function createMockSuite(): SuiteInfo {
    const configPath = path.join(__dirname, 'config/type-map.json');
    const classifier = new TypeClassifier(configPath);

    // Mock AIArtSuite with various parameter types
    const suite: SuiteInfo = {
        name: 'AIArtSuite',
        version: 1,
        functions: [
            // Function with input handle and output handle
            {
                name: 'NewArt',
                returnType: 'AIErr',
                suiteName: 'AIArtSuite',
                params: [
                    {
                        name: 'type',
                        type: 'ai::int16',
                        isPointer: false,
                        isConst: false,
                        isOutput: false,
                        classification: classifier.classify('ai::int16', 'type')
                    },
                    {
                        name: 'paintOrder',
                        type: 'ai::int16',
                        isPointer: false,
                        isConst: false,
                        isOutput: false,
                        classification: classifier.classify('ai::int16', 'paintOrder')
                    },
                    {
                        name: 'prep',
                        type: 'AIArtHandle',
                        isPointer: true,
                        isConst: false,
                        isOutput: false, // Input handle (we read from it)
                        classification: classifier.classify('AIArtHandle', 'prep')
                    },
                    {
                        name: 'newArt',
                        type: 'AIArtHandle',
                        isPointer: true,
                        isConst: false,
                        isOutput: true, // Output handle (SDK writes to it)
                        classification: classifier.classify('AIArtHandle', 'newArt')
                    }
                ]
            },
            // Function with input handle and output primitive
            {
                name: 'GetArtType',
                returnType: 'AIErr',
                suiteName: 'AIArtSuite',
                params: [
                    {
                        name: 'art',
                        type: 'AIArtHandle',
                        isPointer: true,
                        isConst: true, // const means input
                        isOutput: false,
                        classification: classifier.classify('AIArtHandle', 'art')
                    },
                    {
                        name: 'type',
                        type: 'short',
                        isPointer: true,
                        isConst: false,
                        isOutput: true,
                        classification: classifier.classify('short', 'type')
                    }
                ]
            },
            // Function with struct input and output
            {
                name: 'GetArtBounds',
                returnType: 'AIErr',
                suiteName: 'AIArtSuite',
                params: [
                    {
                        name: 'art',
                        type: 'AIArtHandle',
                        isPointer: true,
                        isConst: true,
                        isOutput: false,
                        classification: classifier.classify('AIArtHandle', 'art')
                    },
                    {
                        name: 'bounds',
                        type: 'AIRealRect',
                        isPointer: true,
                        isConst: false,
                        isOutput: true,
                        classification: classifier.classify('AIRealRect', 'bounds')
                    }
                ]
            },
            // Function with string input
            {
                name: 'SetArtName',
                returnType: 'AIErr',
                suiteName: 'AIArtSuite',
                params: [
                    {
                        name: 'art',
                        type: 'AIArtHandle',
                        isPointer: true,
                        isConst: false,
                        isOutput: false,
                        classification: classifier.classify('AIArtHandle', 'art')
                    },
                    {
                        name: 'name',
                        type: 'ai::UnicodeString',
                        isPointer: false,
                        isConst: true,
                        isOutput: false,
                        classification: classifier.classify('ai::UnicodeString', 'name')
                    }
                ]
            },
            // Function with string output
            {
                name: 'GetArtName',
                returnType: 'AIErr',
                suiteName: 'AIArtSuite',
                params: [
                    {
                        name: 'art',
                        type: 'AIArtHandle',
                        isPointer: true,
                        isConst: true,
                        isOutput: false,
                        classification: classifier.classify('AIArtHandle', 'art')
                    },
                    {
                        name: 'name',
                        type: 'ai::UnicodeString',
                        isPointer: true,
                        isConst: false,
                        isOutput: true,
                        classification: classifier.classify('ai::UnicodeString', 'name')
                    }
                ]
            }
        ]
    };

    return suite;
}

async function main() {
    console.log('Testing CppGenerator...\n');

    const configPath = path.join(__dirname, 'config/type-map.json');
    const generator = new CppGenerator(configPath);

    const mockSuite = createMockSuite();
    console.log(`Generating code for suite: ${mockSuite.name}`);
    console.log(`Number of functions: ${mockSuite.functions.length}\n`);

    const files = generator.generate(mockSuite);

    for (const file of files) {
        console.log(`\n${'='.repeat(60)}`);
        console.log(`FILE: ${file.filename}`);
        console.log('='.repeat(60));
        console.log(file.content);
    }

    console.log('\nGenerator test completed successfully!');
}

main().catch(console.error);
