import { SuiteParser } from './parser/SuiteParser';
import * as path from 'path';
import * as fs from 'fs';
const Parser = require('tree-sitter');
const Cpp = require('tree-sitter-cpp');

async function main() {
    const headerPath = path.join(__dirname, '../../plugin/sdk/AIArt.h');

    // 1. Check API availability
    const p = new Parser();
    p.setLanguage(Cpp);
    const t = p.parse('struct A { int x; };');
    const root = t.rootNode;
    console.log("Root prototype has childForFieldName:", typeof Object.getPrototypeOf(root).childForFieldName);

    // 2. Dump Tree Structure for AIArtSuite
    console.log(`\nParsing ${headerPath}...`);
    const code = fs.readFileSync(headerPath, 'utf8').replace(/AIAPI /g, '');
    const tree = p.parse(code);

    // Find the definition of AIArtSuite
    const cursor = tree.walk();
    let found = false;

    // Helper to print node
    const printNode = (node: any, depth = 0) => {
        const indent = '  '.repeat(depth);
        console.log(`${indent}${node.type} [${node.startPosition.row}:${node.startPosition.column} - ${node.endPosition.row}:${node.endPosition.column}]`);
        // if (node.text.length < 50) console.log(`${indent}  "${node.text}"`);
        for (const c of node.children) {
            printNode(c, depth + 1);
        }
    }

    // Naive search for the struct
    const q = new Parser.Query(Cpp, `(struct_specifier name: (type_identifier) @name)`);
    const matches = q.matches(tree.rootNode);
    for (const m of matches) {
        const node = m.captures[0].node;
        if (node.text === 'AIArtSuite') {
            console.log("\nFOUND AIArtSuite Struct Specifier:");
            printNode(node.parent, 0);
            break;
        }
    }

    // 3. Run the actual SuiteParser
    console.log("\n--- Running SuiteParser ---");
    const configPath = path.join(__dirname, 'config/type-map.json');
    const parser = new SuiteParser(configPath);
    const suites = await parser.parseFile(headerPath);
    console.log(`Found ${suites.length} suites.`);
    if (suites.length === 0) {
        console.log("No suites found via SuiteParser query.");
    } else {
        console.log(JSON.stringify(suites[0], null, 2));
    }
}

main();
