/**
 * NUXP Code Generator - Main Entry Point
 *
 * This tool parses Adobe Illustrator SDK headers using tree-sitter and generates:
 * 1. C++ wrapper code with HTTP endpoints for each SDK suite function
 * 2. TypeScript client code for calling the C++ plugin from Tauri frontend
 *
 * Usage:
 *   npm run generate -- [options]
 *
 * Options:
 *   -s, --sdk <path>       Path to SDK headers (default: ../plugin/sdk)
 *   -o, --output <path>    Output directory (default: ./output)
 *   --suites <names>       Comma-separated list of suites to generate
 *   --cpp-only             Only generate C++ code
 *   --ts-only              Only generate TypeScript code
 *   -v, --verbose          Enable verbose logging
 */

import { Command } from 'commander';
import { SuiteParser } from './parser/SuiteParser';
import { CppGenerator } from './generator/CppGenerator';
import { TypeScriptGenerator } from './generator/TypeScriptGenerator';
import * as fs from 'fs-extra';
import * as path from 'path';
import * as glob from 'glob';
import winston from 'winston';

// Configure logger with colored output
const logger = winston.createLogger({
    level: 'info',
    format: winston.format.combine(
        winston.format.colorize(),
        winston.format.printf(({ level, message }) => {
            return `${level}: ${message}`;
        })
    ),
    transports: [new winston.transports.Console()]
});

interface ProgramOptions {
    sdk: string;
    output: string;
    suites: string;
    cppOnly: boolean;
    tsOnly: boolean;
    verbose: boolean;
}

/**
 * Validates that required paths exist
 */
async function validatePaths(sdkPath: string, configPath: string): Promise<void> {
    if (!await fs.pathExists(sdkPath)) {
        throw new Error(`SDK path does not exist: ${sdkPath}`);
    }

    if (!await fs.pathExists(configPath)) {
        throw new Error(`Config file does not exist: ${configPath}`);
    }
}

/**
 * Finds SDK header files matching the pattern AI*.h
 */
function findSdkHeaders(sdkPath: string): string[] {
    // Look for headers in the root of sdk/ and in sdk/actions/
    const patterns = [
        path.join(sdkPath, 'AI*.h'),
        path.join(sdkPath, 'actions', 'AI*.h')
    ];

    let allHeaders: string[] = [];
    for (const pattern of patterns) {
        const headers = glob.sync(pattern);
        allHeaders = allHeaders.concat(headers);
    }

    // Sort for consistent ordering
    return allHeaders.sort();
}

/**
 * Main entry point for the code generator
 */
async function main(): Promise<void> {
    const program = new Command();

    program
        .name('nuxp-codegen')
        .description('Generate C++ and TypeScript wrappers for Adobe Illustrator SDK')
        .version('1.0.0')
        .option('-s, --sdk <path>', 'Path to SDK headers', '../plugin/sdk')
        .option('-o, --output <path>', 'Output directory', './output')
        .option('--suites <names>', 'Comma-separated list of suites to generate (default: all)', '')
        .option('--cpp-only', 'Only generate C++ code', false)
        .option('--ts-only', 'Only generate TypeScript code', false)
        .option('-v, --verbose', 'Verbose output', false);

    program.parse();
    const options = program.opts<ProgramOptions>();

    // Enable verbose logging if requested
    if (options.verbose) {
        logger.level = 'debug';
    }

    logger.info('NUXP Code Generator');
    logger.info('==================');

    // Resolve paths relative to the src directory (where index.ts is located)
    const srcDir = __dirname;
    const codegenDir = path.dirname(srcDir);
    const sdkPath = path.resolve(codegenDir, options.sdk);
    const outputPath = path.resolve(codegenDir, options.output);
    const configPath = path.resolve(srcDir, 'config', 'type-map.json');

    logger.debug(`Source directory: ${srcDir}`);
    logger.debug(`SDK path: ${sdkPath}`);
    logger.debug(`Output path: ${outputPath}`);
    logger.debug(`Config path: ${configPath}`);

    // Validate paths
    try {
        await validatePaths(sdkPath, configPath);
    } catch (err) {
        logger.error((err as Error).message);
        process.exit(1);
    }

    // Ensure output directories exist
    const cppOutputDir = path.join(outputPath, 'cpp');
    const tsOutputDir = path.join(outputPath, 'typescript');
    const docsOutputDir = path.join(outputPath, 'docs');

    await fs.ensureDir(cppOutputDir);
    await fs.ensureDir(tsOutputDir);
    await fs.ensureDir(docsOutputDir);

    logger.debug(`C++ output: ${cppOutputDir}`);
    logger.debug(`TypeScript output: ${tsOutputDir}`);

    // Find SDK headers
    const headers = findSdkHeaders(sdkPath);

    if (headers.length === 0) {
        logger.error(`No SDK headers found at ${sdkPath}`);
        logger.error('Expected files matching pattern: AI*.h');
        process.exit(1);
    }

    logger.info(`Found ${headers.length} SDK headers`);

    // Initialize parser and generators
    const parser = new SuiteParser(configPath);
    const cppGen = new CppGenerator(configPath);
    const tsGen = new TypeScriptGenerator(configPath);

    // Parse suite filter if provided
    const suiteFilter = options.suites
        ? options.suites.split(',').map((s: string) => s.trim()).filter(Boolean)
        : null;

    if (suiteFilter) {
        logger.info(`Filtering suites: ${suiteFilter.join(', ')}`);
    }

    // Track statistics
    let totalSuites = 0;
    let totalFunctions = 0;
    let totalCppFiles = 0;
    let totalTsFiles = 0;
    const errors: { header: string; error: string }[] = [];
    const generatedSuiteNames: string[] = []; // Track generated suite names for CentralDispatcher

    // Process each header file
    for (const headerPath of headers) {
        const headerName = path.basename(headerPath);
        logger.debug(`Parsing ${headerName}...`);

        try {
            const suites = await parser.parseFile(headerPath);

            for (const suite of suites) {
                // Skip if not in filter
                if (suiteFilter && !suiteFilter.includes(suite.name)) {
                    logger.debug(`  Skipping ${suite.name} (not in filter)`);
                    continue;
                }

                // Skip suites with no functions
                if (suite.functions.length === 0) {
                    logger.debug(`  Skipping ${suite.name} (no functions)`);
                    continue;
                }

                logger.info(`Generating ${suite.name} (${suite.functions.length} functions)...`);

                // Generate C++ code
                if (!options.tsOnly) {
                    try {
                        const cppFiles = cppGen.generate(suite);
                        for (const file of cppFiles) {
                            const filePath = path.join(cppOutputDir, file.filename);
                            await fs.writeFile(filePath, file.content, 'utf-8');
                            logger.debug(`  Wrote ${file.filename}`);
                            totalCppFiles++;
                        }
                        // Track suite name for CentralDispatcher generation
                        generatedSuiteNames.push(suite.name);
                    } catch (err) {
                        logger.error(`  Error generating C++ for ${suite.name}: ${err}`);
                        errors.push({ header: headerName, error: `C++ generation: ${err}` });
                    }
                }

                // Generate TypeScript code
                if (!options.cppOnly) {
                    try {
                        const tsFile = tsGen.generate(suite);
                        const filePath = path.join(tsOutputDir, tsFile.filename);
                        await fs.writeFile(filePath, tsFile.content, 'utf-8');
                        logger.debug(`  Wrote ${tsFile.filename}`);
                        totalTsFiles++;
                    } catch (err) {
                        logger.error(`  Error generating TypeScript for ${suite.name}: ${err}`);
                        errors.push({ header: headerName, error: `TypeScript generation: ${err}` });
                    }
                }

                totalSuites++;
                totalFunctions += suite.functions.length;
            }
        } catch (err) {
            const errorMessage = err instanceof Error ? err.message : String(err);
            logger.error(`Error processing ${headerName}: ${errorMessage}`);
            errors.push({ header: headerName, error: errorMessage });
        }
    }

    // Print summary
    logger.info('');
    logger.info('Generation Complete!');
    logger.info('====================');
    logger.info(`  Suites processed: ${totalSuites}`);
    logger.info(`  Functions found:  ${totalFunctions}`);

    if (!options.tsOnly) {
        logger.info(`  C++ files:        ${totalCppFiles}`);
    }
    if (!options.cppOnly) {
        logger.info(`  TypeScript files: ${totalTsFiles}`);
    }

    logger.info(`  Output directory: ${outputPath}`);

    if (errors.length > 0) {
        logger.warn('');
        logger.warn(`Errors encountered: ${errors.length}`);
        for (const { header, error } of errors) {
            logger.warn(`  ${header}: ${error}`);
        }
    }

    // Generate index file for TypeScript exports
    if (!options.cppOnly && totalTsFiles > 0) {
        await generateTypeScriptIndex(tsOutputDir);
    }

    // Generate CentralDispatcher.h for C++ routing (before CMake include so it's listed)
    if (!options.tsOnly && generatedSuiteNames.length > 0) {
        await generateCentralDispatcher(cppOutputDir, generatedSuiteNames);
        logger.debug('Generated CentralDispatcher.h');
    }

    // Generate CMake include file for C++ sources (after CentralDispatcher so it's included)
    if (!options.tsOnly && totalCppFiles > 0) {
        await generateCMakeInclude(cppOutputDir);
    }
}

/**
 * Generates an index.ts file that re-exports all generated TypeScript modules
 */
async function generateTypeScriptIndex(tsOutputDir: string): Promise<void> {
    const tsFiles = glob.sync(path.join(tsOutputDir, '*.ts'));
    const exports: string[] = [];

    for (const file of tsFiles) {
        const basename = path.basename(file, '.ts');
        if (basename !== 'index') {
            exports.push(`export * from './${basename}';`);
        }
    }

    if (exports.length > 0) {
        const indexContent = `/**
 * NUXP SDK Client - Auto-generated index
 * Re-exports all generated suite modules
 */

${exports.sort().join('\n')}
`;
        await fs.writeFile(path.join(tsOutputDir, 'index.ts'), indexContent, 'utf-8');
        logger.debug('Generated TypeScript index.ts');
    }
}

/**
 * Generates a CMake include file listing all generated C++ sources
 */
async function generateCMakeInclude(cppOutputDir: string): Promise<void> {
    const cppFiles = glob.sync(path.join(cppOutputDir, '*.cpp'));
    const hppFiles = glob.sync(path.join(cppOutputDir, '*.hpp'));
    const hFiles = glob.sync(path.join(cppOutputDir, '*.h'));

    const sources = cppFiles.map(f => path.basename(f));
    const headers = [...hppFiles, ...hFiles].map(f => path.basename(f));

    const cmakeContent = `# Auto-generated by NUXP Codegen - DO NOT EDIT
# Include this file in your CMakeLists.txt

set(GENERATED_SOURCES
${sources.map(s => `    \${CMAKE_CURRENT_LIST_DIR}/${s}`).join('\n')}
)

set(GENERATED_HEADERS
${headers.map(h => `    \${CMAKE_CURRENT_LIST_DIR}/${h}`).join('\n')}
)
`;
    await fs.writeFile(path.join(cppOutputDir, 'generated_sources.cmake'), cmakeContent, 'utf-8');
    logger.debug('Generated CMake include file');
}

/**
 * Generates CentralDispatcher.h that routes by suite name to suite-specific dispatchers
 */
async function generateCentralDispatcher(cppOutputDir: string, suiteNames: string[]): Promise<void> {
    // Sort suite names for consistent output
    const sortedSuites = [...suiteNames].sort();

    // Generate includes
    const includes = sortedSuites.map(name => `#include "Flora${name}Wrapper.h"`).join('\n');

    // Generate dispatch cases
    const dispatchCases: string[] = [];
    for (let i = 0; i < sortedSuites.length; i++) {
        const suite = sortedSuites[i];
        if (i === 0) {
            dispatchCases.push(`    if (suite == "${suite}") {`);
        } else {
            dispatchCases.push(`    } else if (suite == "${suite}") {`);
        }
        dispatchCases.push(`        return Flora::${suite}::Dispatch(method, params);`);
    }
    if (sortedSuites.length > 0) {
        dispatchCases.push(`    }`);
    }

    const content = `#pragma once
// Auto-generated by NUXP Codegen - DO NOT EDIT
// Central dispatcher that routes by suite name to suite-specific dispatchers

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

${includes}

namespace Flora {

/**
 * Central dispatch function that routes requests to suite-specific dispatchers
 * @param suite The suite name (e.g., "AIArtSuite")
 * @param method The method name within the suite (e.g., "NewArt")
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if suite or method is not found
 */
inline nlohmann::json Dispatch(const std::string& suite, const std::string& method, const nlohmann::json& params) {
${dispatchCases.join('\n')}
    throw std::runtime_error("Unknown suite: " + suite);
}

} // namespace Flora
`;

    await fs.writeFile(path.join(cppOutputDir, 'CentralDispatcher.h'), content, 'utf-8');
}

// Run the main function
main().catch(err => {
    console.error('Fatal error:', err);
    process.exit(1);
});
