import * as fs from 'fs-extra';
import * as path from 'path';

export type TypeCategory = 'Handle' | 'ManagedHandle' | 'Primitive' | 'String' | 'Struct' | 'Enum' | 'Error' | 'Void' | 'Unknown';

export interface TypeDefinition {
    raw: string;           // "AIArtHandle *"
    baseType: string;      // "AIArtHandle"
    category: TypeCategory;
    isPointer: boolean;
    isConst: boolean;
    isReference: boolean; // "&"
    registryName?: string; // "art" for AIArtHandle
    cppType: string;       // "AIArtHandle" (cleaned)
    jsonType: string;      // "int32_t" type in JSON
}

export class TypeClassifier {
    private config: any;

    constructor(configPath: string) {
        this.config = JSON.parse(fs.readFileSync(configPath, 'utf-8'));
    }

    classify(rawType: string, paramName: string): TypeDefinition {
        // 1. Clean the type string
        // rawType comes from tree-sitter, might contain "const", "*", "&", "ai::"
        let clean = rawType.trim();

        const isConst = clean.includes('const');
        const isPointer = clean.includes('*');
        const isReference = clean.includes('&');

        // Strip modifiers to get base type
        let base = clean.replace(/const/g, '').replace(/[*&]/g, '').trim();

        // Default result
        const result: TypeDefinition = {
            raw: rawType,
            baseType: base,
            category: 'Unknown',
            isPointer,
            isConst,
            isReference,
            cppType: base,
            jsonType: 'any'
        };

        // 2. Logic to determine Category
        if (base === 'void' || base === 'void *') {
            result.category = 'Void';
            result.jsonType = 'void';
            return result;
        }

        if (base === 'AIErr') {
            result.category = 'Error';
            result.jsonType = 'int32_t';
            return result;
        }

        // Check Strings
        if (this.config.string_types.includes(base)) {
            result.category = 'String';
            result.jsonType = 'std::string';
            return result;
        }

        // Check for char/char* types
        // Note: "char **name" gets parsed as type="char *" or type="char" with isPointer=true
        // These can be either input strings or output string pointers depending on context
        // The CppGenerator checks isOutput to determine which case applies
        if (base === 'char' || base === 'char *') {
            result.category = 'String';
            result.baseType = 'char*';
            result.jsonType = 'std::string';
            return result;
        }

        // Check ManagedHandles (RAII objects, not opaque pointers)
        if (this.config.managed_handles && this.config.managed_handles[base]) {
            result.category = 'ManagedHandle';
            result.registryName = this.config.managed_handles[base];
            result.jsonType = 'int32_t';
            return result;
        }

        // Check Handles
        if (this.config.handles[base]) {
            result.category = 'Handle';
            result.registryName = this.config.handles[base];
            result.jsonType = 'int32_t'; // Handles are passed as IDs
            return result;
        }

        // Check Primitives
        if (this.config.primitives[base]) {
            result.category = 'Primitive';
            result.jsonType = this.config.primitives[base];
            // Special case for AIBoolean -> bool
            if (base === 'AIBoolean') result.jsonType = 'bool';
            return result;
        }

        // Check Structs
        if (this.config.structs[base]) {
            result.category = 'Struct';
            result.jsonType = 'nlohmann::json'; // Passed as object
            return result;
        }

        // Heuristic for Enums: usually start with AI... and not in other lists?
        // Or if tree-sitter told us it was an enum.
        // simpler: If it maps to int/short in C++, treat as Primitive?
        // User might need to help with specific Enums.
        // For now, if unknown, default to Unknown.

        return result;
    }
}
