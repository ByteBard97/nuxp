import Parser from 'tree-sitter';
const Cpp = require('tree-sitter-cpp');
import * as fs from 'fs-extra';
import { SuiteInfo, FunctionInfo, ParamInfo } from './types';

import { TypeClassifier } from './TypeClassifier';
import * as path from 'path';

export class SuiteParser {
  private parser: Parser;
  private classifier: TypeClassifier;

  constructor(configPath: string) {
    this.parser = new Parser();
    this.parser.setLanguage(Cpp);
    this.classifier = new TypeClassifier(configPath);
  }

  async parseFile(filePath: string): Promise<SuiteInfo[]> {
    // 1. Preprocess: Remove AIAPI macro to fix parsing of function pointers
    let code = await fs.readFile(filePath, 'utf-8');
    code = code.replace(/AIAPI /g, '');

    const tree = this.parser.parse(code);
    // 2. Find Structs ending in Suite
    const suites: SuiteInfo[] = [];

    // Query 1: typedef struct { ... } SuiteName;
    const typedefQuery = new Parser.Query(Cpp, `
      (type_definition
        type: (struct_specifier
          name: (type_identifier)? @struct_name
          body: (field_declaration_list) @body
        )
        declarator: (type_identifier) @suite_name
      )
    `);

    // Query 2: struct SuiteName { ... };
    const structQuery = new Parser.Query(Cpp, `
      (struct_specifier
        name: (type_identifier) @suite_name
        body: (field_declaration_list) @body
      )
    `);

    const matches1 = typedefQuery.matches(tree.rootNode);
    const matches2 = structQuery.matches(tree.rootNode);

    // Process all matches
    const seenSuites = new Set<string>();

    for (const match of [...matches1, ...matches2]) {
      const suiteNameNode = match.captures.find(c => c.name === 'suite_name')?.node;
      const bodyNode = match.captures.find(c => c.name === 'body')?.node;

      if (!suiteNameNode || !bodyNode) continue;

      const suiteName = suiteNameNode.text;
      if (!suiteName.endsWith('Suite') || seenSuites.has(suiteName)) continue;

      seenSuites.add(suiteName);

      const functions = this.extractFunctions(bodyNode, suiteName);

      if (functions.length > 0) {
        suites.push({
          name: suiteName,
          version: 1,
          functions
        });
      }
    }

    return suites;
  }

  private extractFunctions(bodyNode: any, suiteName: string): FunctionInfo[] {
    const functions: FunctionInfo[] = [];

    // 3. Query for function pointers within the struct body
    // Structure: ReturnType (*FuncName)(Params...);
    const funcQuery = new Parser.Query(Cpp, `
      (field_declaration
        type: (_) @return_type
        declarator: (function_declarator
          (parenthesized_declarator
            (pointer_declarator
              (field_identifier) @func_name
            )
          )
          parameters: (parameter_list) @params
        )
      )
    `);

    const matches = funcQuery.matches(bodyNode);

    for (const match of matches) {
      const returnTypeNode = match.captures.find(c => c.name === 'return_type')?.node;
      const funcNameNode = match.captures.find(c => c.name === 'func_name')?.node;
      const paramsNode = match.captures.find(c => c.name === 'params')?.node;

      if (!funcNameNode || !paramsNode) continue;

      const funcName = funcNameNode.text;
      if (funcName.startsWith('_')) continue;

      const params: ParamInfo[] = [];

      // Iterate named children of parameter_list
      for (const child of paramsNode.namedChildren) {
        if (child.type === 'parameter_declaration') {
          // param: type: (...) declarator: (...)
          // We need to extract type and name safely

          const text = child.text;
          const isPointer = text.includes('*');
          const isConst = text.includes('const');

          // Heuristic: Name is usually the last identifier
          // But types can be complex "AIArtHandle *art"
          // child.children: [type_identifier "AIArtHandle", pointer_declarator "*art"]

          let paramName = 'param' + params.length;
          let typeName = 'void';

          if (child.namedChildren.length > 0) {
            const lastIndex = child.namedChildren.length - 1;
            const last = child.namedChildren[lastIndex];
            // strip strict pointer chars
            paramName = last.text.replace(/[*&]/g, '').trim();

            // Reconstruct type from all previous nodes
            // Example: "const", "ai::UnicodeString", "&name" -> typeName="const ai::UnicodeString"
            if (lastIndex > 0) {
              typeName = '';
              for (let i = 0; i < lastIndex; i++) {
                typeName += child.namedChildren[i].text + ' ';
              }
              typeName = typeName.trim();
            } else {
              // If only one child (unlikely for SDK params), treat as type with default name
              typeName = child.namedChildren[0].text;
            }
          }

          // Use Classifier
          const classification = this.classifier.classify(typeName, paramName);

          params.push({
            name: paramName,
            type: typeName,
            isPointer,
            isConst,
            isOutput: isPointer && !isConst,
            classification
          });
        }
      }

      functions.push({
        name: funcName,
        returnType: returnTypeNode ? returnTypeNode.text : 'AIErr',
        params,
        suiteName
      });
    }

    return functions;
  }
}
