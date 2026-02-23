#!/usr/bin/env node
/**
 * Runner script for NUXP CustomRouteGenerator.
 * Reads routes.json and writes generated C++ registration, declarations,
 * and TypeScript files to the output directory.
 *
 * Usage: node codegen/generate-routes.cjs [path/to/routes.json]
 */
const path = require('path');
const fs = require('fs');

// ts-node register so we can require .ts source files directly
require('ts-node').register({
    project: path.resolve(__dirname, 'tsconfig.json'),
    transpileOnly: true,
});

const { CustomRouteGenerator } = require('./src/generator/CustomRouteGenerator');

// Config path: CLI arg or default to routes.json in this directory
const configPath = process.argv[2]
    ? path.resolve(process.argv[2])
    : path.resolve(__dirname, 'routes.json');

const outDir = path.resolve(__dirname, 'output');

if (!fs.existsSync(outDir)) {
    fs.mkdirSync(outDir, { recursive: true });
}

const gen = new CustomRouteGenerator(configPath);

const reg = gen.generateCppRegistration();
fs.writeFileSync(path.join(outDir, reg.filename), reg.content, 'utf-8');
console.log(`Wrote ${reg.filename}`);

const decl = gen.generateCppDeclarations();
fs.writeFileSync(path.join(outDir, decl.filename), decl.content, 'utf-8');
console.log(`Wrote ${decl.filename}`);

const ts = gen.generateTypeScript();
fs.writeFileSync(path.join(outDir, ts.filename), ts.content, 'utf-8');
console.log(`Wrote ${ts.filename}`);

console.log(`Done — ${gen.config.routes.length} routes generated.`);
