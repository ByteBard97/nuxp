/**
 * SDK Edge Case Analyzer
 *
 * Parses SDK headers and identifies functions with parameter types
 * that the code generator cannot handle. Run with:
 *
 *   cd codegen && npx ts-node scripts/analyze-edge-cases.ts
 */

import { SuiteParser } from '../src/parser/SuiteParser';
import * as glob from 'glob';
import * as path from 'path';

const ALLOWED_SUITES = new Set([
    'AIArtSuite', 'AIArtSetSuite', 'AIGroupSuite',
    'AIDocumentSuite', 'AILayerSuite', 'AILayerListSuite', 'AIArtboardSuite',
    'AIRealMathSuite', 'AITransformArtSuite',
    'AIAppContextSuite', 'AIUserSuite', 'AIUndoSuite',
    'AIMdMemorySuite', 'AIDictionarySuite', 'AIEntrySuite',
    'AINotifierSuite', 'AITimerSuite', 'AIToolSuite',
    'AIBlendStyleSuite', 'AIMaskSuite',
]);

interface Issue {
    suite: string;
    func: string;
    param?: string;
    type: string;
    reason: string;
}

const issues: Issue[] = [];

async function analyze() {
    const configPath = path.resolve(__dirname, '../src/config/type-map.json');
    const parser = new SuiteParser(configPath);
    const headers = glob.sync(path.resolve(__dirname, '../../plugin/sdk/AI*.h'));

    for (const headerPath of headers) {
        const suites = await parser.parseFile(headerPath);

        for (const suite of suites) {
            if (!ALLOWED_SUITES.has(suite.name)) continue;

            for (const func of suite.functions) {
                // Check return type
                if (func.returnType !== 'AIErr' && func.returnType !== 'ASErr') {
                    issues.push({
                        suite: suite.name,
                        func: func.name,
                        type: func.returnType,
                        reason: 'non-AIErr return type'
                    });
                }

                // Check parameters
                for (const param of func.params) {
                    // Array params
                    if (param.name.includes('[]') || param.type.includes('[]')) {
                        issues.push({
                            suite: suite.name,
                            func: func.name,
                            param: param.name,
                            type: param.type,
                            reason: 'array parameter'
                        });
                    }

                    // void params (including void* that parser sees as void)
                    if (param.type === 'void') {
                        issues.push({
                            suite: suite.name,
                            func: func.name,
                            param: param.name,
                            type: param.type + (param.isPointer ? '*' : ''),
                            reason: 'void/void* parameter'
                        });
                    }

                    // char output (likely char**)
                    if (param.type === 'char' && param.isPointer && param.isOutput) {
                        issues.push({
                            suite: suite.name,
                            func: func.name,
                            param: param.name,
                            type: 'char**',
                            reason: 'char** output (string pointer)'
                        });
                    }

                    // Complex types that can't be easily marshaled
                    const complexTypes = [
                        'AIColor', 'AIGradient', 'AIPattern', 'AIPathStyle',
                        'AINewDocumentPreset', 'AISuspendedAppContext', 'ActionDialogStatus',
                        'AIPathSegment', 'AIToolInfo', 'AIRasterizeSettings',
                        'struct _'
                    ];
                    for (const ct of complexTypes) {
                        if (param.type.includes(ct)) {
                            issues.push({
                                suite: suite.name,
                                func: func.name,
                                param: param.name,
                                type: param.type,
                                reason: `complex type: ${ct}`
                            });
                            break;
                        }
                    }
                }
            }
        }
    }

    // Group by reason
    const byReason: Record<string, Issue[]> = {};
    for (const issue of issues) {
        byReason[issue.reason] = byReason[issue.reason] || [];
        byReason[issue.reason].push(issue);
    }

    console.log('=== SDK EDGE CASE ANALYSIS ===\n');

    for (const [reason, list] of Object.entries(byReason).sort((a, b) => b[1].length - a[1].length)) {
        console.log(`### ${reason} (${list.length} instances)\n`);
        for (const item of list.slice(0, 15)) {
            const paramInfo = item.param ? `(${item.param}: ${item.type})` : `-> ${item.type}`;
            console.log(`  - ${item.suite}::${item.func} ${paramInfo}`);
        }
        if (list.length > 15) {
            console.log(`  ... and ${list.length - 15} more\n`);
        }
        console.log('');
    }

    // Summary
    const uniqueFuncs = new Set(issues.map(i => `${i.suite}::${i.func}`));
    console.log('=== SUMMARY ===');
    console.log(`Total issues found: ${issues.length}`);
    console.log(`Functions with issues: ${uniqueFuncs.size}`);
    console.log(`\nFunctions to block:`);
    for (const funcName of [...uniqueFuncs].sort()) {
        console.log(`  '${funcName.split('::')[1]}',`);
    }
}

analyze().catch(console.error);
