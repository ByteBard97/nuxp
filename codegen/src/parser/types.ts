import { TypeDefinition } from './TypeClassifier';

export interface ParamInfo {
    name: string;
    type: string;
    isPointer: boolean;
    isConst: boolean;
    isReference: boolean;
    isOutput: boolean;
    classification?: TypeDefinition;
}

export interface FunctionInfo {
    name: string;
    returnType: string; // Usually "AIErr"
    params: ParamInfo[];
    suiteName: string; // e.g. "AIArtSuite"
}

export interface SuiteInfo {
    name: string; // e.g. "AIArtSuite"
    version: number;
    functions: FunctionInfo[];
}
