import { TypeClassifier } from './parser/TypeClassifier';
import * as path from 'path';

function main() {
    const configPath = path.join(__dirname, 'config/type-map.json');
    const classifier = new TypeClassifier(configPath);

    const inputs = [
        "AIArtHandle",
        "AIArtHandle *",
        "const AIArtHandle",
        "AIBoolean",
        "ai::int32",
        "AIErr",
        "ai::UnicodeString &",
        "AIRealRect *"
    ];

    console.log("Testing Type Classifier...");
    for (const input of inputs) {
        const def = classifier.classify(input, "testParam");
        console.log(`\nInput: "${input}"`);
        console.log(`  Category: ${def.category}`);
        console.log(`  Base:     ${def.baseType}`);
        console.log(`  IsPtr:    ${def.isPointer}`);
        console.log(`  Registry: ${def.registryName || 'N/A'}`);
    }
}

main();
