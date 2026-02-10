
import * as fs from 'fs-extra';
import * as path from 'path';
import Parser from 'tree-sitter';
const Cpp = require('tree-sitter-cpp');
import { SuiteParser } from './parser/SuiteParser';
import { TypeClassifier, TypeDefinition } from './parser/TypeClassifier';
import { SuiteInfo, FunctionInfo, ParamInfo } from './parser/types'; // Import necessary types

// Configuration
const SDK_PATH = path.join(__dirname, '../../plugin/sdk');
// Need to point to config correctly relative to where this script is run or built
// Assuming run with ts-node from codegen/ , __dirname is codegen/src
const CONFIG_PATH = path.join(__dirname, 'config/type-map.json');

interface AnalysisStats {
    totalFiles: number;
    totalSuitesFound: number;
    totalFunctionsFound: number;
    functionsWithUnsupportedReturn: { suite: string, func: string, returnType: string }[];
    functionsWithUnsupportedParams: { suite: string, func: string, param: string, type: string, reason: string }[];
    ambiguousTypes: Map<string, number>;
    unknownTypes: Map<string, number>;
}

const stats: AnalysisStats = {
    totalFiles: 0,
    totalSuitesFound: 0,
    totalFunctionsFound: 0,
    functionsWithUnsupportedReturn: [],
    functionsWithUnsupportedParams: [],
    ambiguousTypes: new Map(),
    unknownTypes: new Map()
};


// Simple check for C++ keywords
const CPP_KEYWORDS = new Set([
    'alignas', 'alignof', 'and', 'and_eq', 'asm', 'atomic_cancel', 'atomic_commit', 'atomic_noexcept',
    'auto', 'bitand', 'bitor', 'bool', 'break', 'case', 'catch', 'char', 'char16_t', 'char32_t',
    'class', 'compl', 'concept', 'const', 'constexpr', 'const_cast', 'continue', 'co_await',
    'co_return', 'co_yield', 'decltype', 'default', 'delete', 'do', 'double', 'dynamic_cast',
    'else', 'enum', 'explicit', 'export', 'extern', 'false', 'float', 'for', 'friend', 'goto',
    'if', 'import', 'inline', 'int', 'long', 'module', 'mutable', 'namespace', 'new', 'noexcept',
    'not', 'not_eq', 'nullptr', 'operator', 'or', 'or_eq', 'private', 'protected', 'public',
    'register', 'reinterpret_cast', 'requires', 'return', 'short', 'signed', 'sizeof', 'static',
    'static_assert', 'static_cast', 'struct', 'switch', 'synchronized', 'template', 'this',
    'thread_local', 'throw', 'true', 'try', 'typedef', 'typeid', 'typename', 'union', 'unsigned',
    'using', 'virtual', 'void', 'volatile', 'wchar_t', 'while', 'xor', 'xor_eq'
]);

async function analyze() {
    console.log("Starting SDK Analysis...");
    console.log(`SDK Path: ${SDK_PATH}`);

    const suiteParser = new SuiteParser(CONFIG_PATH);
    const classifier = new TypeClassifier(CONFIG_PATH);

    // Get all .h files
    let files: string[] = [];
    try {
        files = (await fs.readdir(SDK_PATH)).filter(f => f.endsWith('.h'));
    } catch (e) {
        console.error(`Error reading SDK directory: ${e}`);
        return;
    }

    stats.totalFiles = files.length;
    console.log(`Found ${files.length} header files.`);

    for (const file of files) {
        const filePath = path.join(SDK_PATH, file);
        try {
            // console.log(`Parsing ${file}...`);
            const suites = await suiteParser.parseFile(filePath);

            for (const suite of suites) {
                // console.log(`  Found suite: ${suite.name}`);
                stats.totalSuitesFound++;

                for (const func of suite.functions) {
                    stats.totalFunctionsFound++;

                    // 1. Check Return Type
                    if (func.returnType !== 'AIErr' && func.returnType !== 'ASErr') {
                        stats.functionsWithUnsupportedReturn.push({
                            suite: suite.name,
                            func: func.name,
                            returnType: func.returnType
                        });
                    }

                    // 2. Check Parameters
                    for (const param of func.params) {
                        // Classify again to be sure (Parser does it, but we want full control)
                        // In SuiteParser, classification is already attached to param.
                        // We can check it directly.

                        const classification = param.classification;

                        if (!classification || classification.category === 'Unknown') {
                            const count = stats.unknownTypes.get(param.type) || 0;
                            stats.unknownTypes.set(param.type, count + 1);

                            stats.functionsWithUnsupportedParams.push({
                                suite: suite.name,
                                func: func.name,
                                param: param.name,
                                type: param.type,
                                reason: 'Unknown Type'
                            });
                        } else if (classification.category === 'Void') {
                            stats.functionsWithUnsupportedParams.push({
                                suite: suite.name,
                                func: func.name,
                                param: param.name,
                                type: param.type,
                                reason: 'Void Pointer'
                            });
                        }

                        // Check for ambiguous pointers (e.g. valid type but pointer might mean array or output)
                        // A pointer to a primitive is usually an output or array.
                        if (classification && classification.category === 'Primitive' && param.isPointer && !param.isConst) {
                            // This is ambiguous: int *count -> could be output count, or array of ints
                            const key = `${param.type} (Mutable Primitive Pointer)`;
                            const count = stats.ambiguousTypes.get(key) || 0;
                            stats.ambiguousTypes.set(key, count + 1);
                        }
                    }
                }
            }
        } catch (e) {
            console.error(`Error parsing ${file}: ${e}`);
        }
    }

    // Report Generation
    console.log("\n--- Analysis Summary ---");
    console.log(`Total Files Scanned: ${stats.totalFiles}`);
    console.log(`Total Suites Found: ${stats.totalSuitesFound}`);
    console.log(`Total Functions Found: ${stats.totalFunctionsFound}`);

    console.log(`\nFunctions with Unsupported Return Types: ${stats.functionsWithUnsupportedReturn.length}`);
    // Dedup return types
    const returnTypes = new Set(stats.functionsWithUnsupportedReturn.map(f => f.returnType));
    console.log(`Unique Unsupported Return Types: ${Array.from(returnTypes).join(', ')}`);

    console.log(`\nFunctions with Unsupported/Unknown Params: ${stats.functionsWithUnsupportedParams.length}`);

    console.log("\nTop 20 Unknown Types:");
    const sortedUnknown = [...stats.unknownTypes.entries()].sort((a, b) => b[1] - a[1]);
    sortedUnknown.slice(0, 20).forEach(([type, count]) => {
        console.log(`  ${type}: ${count}`);
    });

    console.log("\nTop Ambiguous Types (Potential Conflicts):");
    const sortedAmbiguous = [...stats.ambiguousTypes.entries()].sort((a, b) => b[1] - a[1]);
    sortedAmbiguous.slice(0, 20).forEach(([type, count]) => {
        console.log(`  ${type}: ${count}`);
    });

    // Save generated report
    const outputPath = path.join(__dirname, '../output/analysis_report.json');
    await fs.writeJson(outputPath, {
        stats: {
            ...stats,
            ambiguousTypes: Object.fromEntries(stats.ambiguousTypes),
            unknownTypes: Object.fromEntries(stats.unknownTypes)
        }
    }, { spaces: 2 });
    console.log(`\nFull report saved to ${outputPath}`);

}

analyze().catch(err => console.error(err));
