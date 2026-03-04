/**
 * AIUserSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIUserSuite';

/**
 * Performs the i u a i real to string units operation.
 * @param value - The Value value
 * @param precision - The Precision value
 * @returns The string value
 */
export async function IUAIRealToStringUnits(value: number, precision: number): Promise<string> {
    const result = await callCpp<{ string: string }>(SUITE_NAME, 'IUAIRealToStringUnits', { value, precision });
    return result.string;
}

/**
 * Performs the i u a i real to string units without scale operation.
 * @param value - The Value value
 * @param precision - The Precision value
 * @returns The string value
 */
export async function IUAIRealToStringUnitsWithoutScale(value: number, precision: number): Promise<string> {
    const result = await callCpp<{ string: string }>(SUITE_NAME, 'IUAIRealToStringUnitsWithoutScale', { value, precision });
    return result.string;
}

/**
 * Retrieves the units string of an object.
 * @param format - The Format value
 * @returns The string value
 */
export async function GetUnitsString(format: number): Promise<string> {
    const result = await callCpp<{ string: string }>(SUITE_NAME, 'GetUnitsString', { format });
    return result.string;
}

/**
 * Retrieves the global object display name of an object.
 * @returns The name string
 */
export async function GetGlobalObjectDisplayName(): Promise<string> {
    const result = await callCpp<{ name: string }>(SUITE_NAME, 'GetGlobalObjectDisplayName', {  });
    return result.name;
}

/**
 * Performs the edit in original app operation.
 * @param art - Handle to the art object
 */
export async function EditInOriginalApp(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'EditInOriginalApp', { art });
}

/**
 * Performs the build directory menu operation.
 * @param menu - The Menu value
 * @param fileSpec - The File Spec value
 */
export async function BuildDirectoryMenu(menu: any, fileSpec: string): Promise<void> {
    await callCpp(SUITE_NAME, 'BuildDirectoryMenu', { menu, fileSpec });
}

/**
 * Retrieves the indexed directory spec of an object.
 * @param index - The index value
 * @returns The file spec value
 */
export async function GetIndexedDirectorySpec(index: number): Promise<string> {
    const result = await callCpp<{ fileSpec: string }>(SUITE_NAME, 'GetIndexedDirectorySpec', { index });
    return result.fileSpec;
}

/**
 * Performs the reveal the file operation.
 * @param fileSpec - The File Spec value
 */
export async function RevealTheFile(fileSpec: string): Promise<void> {
    await callCpp(SUITE_NAME, 'RevealTheFile', { fileSpec });
}

/**
 * Retrieves the date and time of an object.
 * @returns The out value value
 */
export async function GetDateAndTime(): Promise<any> {
    const result = await callCpp<{ outValue: any }>(SUITE_NAME, 'GetDateAndTime', {  });
    return result.outValue;
}

/**
 * Performs the launch app operation.
 * @param spFileSpec - The Sp File Spec value
 * @param openDoc - Handle to the document
 */
export async function LaunchApp(spFileSpec: string, openDoc: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'LaunchApp', { spFileSpec, openDoc });
}

/**
 * Performs the same name operation.
 * @param name1 - The name string
 * @param name2 - The name string
 * @returns True if the condition is met, false otherwise
 */
export async function SameName(name1: string, name2: string): Promise<boolean> {
    const result = await callCpp<{ same: boolean }>(SUITE_NAME, 'SameName', { name1, name2 });
    return result.same;
}

/**
 * Retrieves the directory dialog of an object.
 * @param title - The Title value
 * @returns The io file path value
 */
export async function GetDirectoryDialog(title: string): Promise<string> {
    const result = await callCpp<{ ioFilePath: string }>(SUITE_NAME, 'GetDirectoryDialog', { title });
    return result.ioFilePath;
}

/**
 * Performs the evaluate expression operation.
 * @param expr - The Expr value
 * @param options - The point coordinates
 * @returns An object containing: evaluatedExpr, isChanged, numericValue
 */
export async function EvaluateExpression(expr: string, options: any): Promise<{ evaluatedExpr: string; isChanged: boolean; numericValue: any }> {
    const result = await callCpp<{ evaluatedExpr: string; isChanged: boolean; numericValue: any }>(SUITE_NAME, 'EvaluateExpression', { expr, options });
    return result;
}

/**
 * Performs the evaluate expression without scale operation.
 * @param expr - The Expr value
 * @param options - The point coordinates
 * @returns An object containing: evaluatedExpr, isChanged, numericValue
 */
export async function EvaluateExpressionWithoutScale(expr: string, options: any): Promise<{ evaluatedExpr: string; isChanged: boolean; numericValue: any }> {
    const result = await callCpp<{ evaluatedExpr: string; isChanged: boolean; numericValue: any }>(SUITE_NAME, 'EvaluateExpressionWithoutScale', { expr, options });
    return result;
}

/**
 * Sets the cursor of an object.
 * @param cursorID - The Cursor I D value
 * @param inRscMgr - The In Rsc Mgr value
 */
export async function SetCursor(cursorID: number, inRscMgr: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetCursor', { cursorID, inRscMgr });
}

/**
 * Sets the s v g cursor of an object.
 * @param cursorID - The Cursor I D value
 * @param inRscMgr - The In Rsc Mgr value
 */
export async function SetSVGCursor(cursorID: number, inRscMgr: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetSVGCursor', { cursorID, inRscMgr });
}

/**
 * Retrieves the a i language code of an object.
 * @returns The lang value
 */
export async function GetAILanguageCode(): Promise<string> {
    const result = await callCpp<{ lang: string }>(SUITE_NAME, 'GetAILanguageCode', {  });
    return result.lang;
}

/**
 * Performs the launch folder operation.
 * @param folderPath - The Folder Path value
 */
export async function LaunchFolder(folderPath: string): Promise<void> {
    await callCpp(SUITE_NAME, 'LaunchFolder', { folderPath });
}

/**
 * Performs the launch custom app for custom uri operation.
 * @param customUri - The Custom Uri value
 * @param appPath - The App Path value
 */
export async function LaunchCustomAppForCustomUri(customUri: string, appPath: string): Promise<void> {
    await callCpp(SUITE_NAME, 'LaunchCustomAppForCustomUri', { customUri, appPath });
}

/**
 * Performs the edit in custom app operation.
 * @param art - Handle to the art object
 * @param appPath - The App Path value
 */
export async function EditInCustomApp(art: number, appPath: string): Promise<void> {
    await callCpp(SUITE_NAME, 'EditInCustomApp', { art, appPath });
}
