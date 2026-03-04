/**
 * AIUndoSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIUndoSuite';

/**
 * Sets the undo text u s of an object.
 * @param undoText - The Undo Text value
 * @param redoText - The Redo Text value
 */
export async function SetUndoTextUS(undoText: string, redoText: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetUndoTextUS', { undoText, redoText });
}

/**
 * Sets the undo redo cmd text u s of an object.
 * @param undoText - The Undo Text value
 * @param redoText - The Redo Text value
 * @param cmdText - The Cmd Text value
 */
export async function SetUndoRedoCmdTextUS(undoText: string, redoText: string, cmdText: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetUndoRedoCmdTextUS', { undoText, redoText, cmdText });
}

/**
 * Performs the multi undo transaction operation.
 * @param document - Handle to the document
 * @param n - The N value
 */
export async function MultiUndoTransaction(document: number, n: number): Promise<void> {
    await callCpp(SUITE_NAME, 'MultiUndoTransaction', { document, n });
}

/**
 * Performs the multi redo transaction operation.
 * @param document - Handle to the document
 * @param n - The N value
 */
export async function MultiRedoTransaction(document: number, n: number): Promise<void> {
    await callCpp(SUITE_NAME, 'MultiRedoTransaction', { document, n });
}

/**
 * Performs the forget redos operation.
 * @param document - Handle to the document
 */
export async function ForgetRedos(document: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ForgetRedos', { document });
}

/**
 * Performs the clear history operation.
 * @param document - Handle to the document
 */
export async function ClearHistory(document: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ClearHistory', { document });
}

/**
 * Sets the silent of an object.
 * @param silent - The Silent value
 */
export async function SetSilent(silent: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetSilent', { silent });
}

/**
 * Sets the kind of an object.
 * @param kind - The Kind value
 */
export async function SetKind(kind: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetKind', { kind });
}

/**
 * Counts the number of transactions objects.
 * @returns An object containing: past, future
 */
export async function CountTransactions(): Promise<{ past: number; future: number }> {
    const result = await callCpp<{ past: number; future: number }>(SUITE_NAME, 'CountTransactions', {  });
    return result;
}

/**
 * Checks if is silent.
 * @returns True if the condition is met, false otherwise
 */
export async function IsSilent(): Promise<boolean> {
    const result = await callCpp<{ silent: boolean }>(SUITE_NAME, 'IsSilent', {  });
    return result.silent;
}

/**
 * Sets the tag u s of an object.
 * @param tagString - The Tag String value
 * @param tagInteger - The Tag Integer value
 */
export async function SetTagUS(tagString: string, tagInteger: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetTagUS', { tagString, tagInteger });
}

/**
 * Retrieves the tag u s of an object.
 * @returns An object containing: tagString, tagInteger
 */
export async function GetTagUS(): Promise<{ tagString: string; tagInteger: number }> {
    const result = await callCpp<{ tagString: string; tagInteger: number }>(SUITE_NAME, 'GetTagUS', {  });
    return result;
}

/**
 * Sets the nth transaction tag u s of an object.
 * @param n - The N value
 * @param tagString - The Tag String value
 * @param tagInteger - The Tag Integer value
 */
export async function SetNthTransactionTagUS(n: number, tagString: string, tagInteger: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetNthTransactionTagUS', { n, tagString, tagInteger });
}

/**
 * Retrieves the nth transaction tag u s of an object.
 * @param n - The N value
 * @returns An object containing: tagString, tagInteger
 */
export async function GetNthTransactionTagUS(n: number): Promise<{ tagString: string; tagInteger: number }> {
    const result = await callCpp<{ tagString: string; tagInteger: number }>(SUITE_NAME, 'GetNthTransactionTagUS', { n });
    return result;
}

/**
 * Sets the recording suspended of an object.
 * @param inSuspend - The In Suspend value
 */
export async function SetRecordingSuspended(inSuspend: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetRecordingSuspended', { inSuspend });
}

/**
 * Checks if is recording suspended.
 * @returns True if the condition is met, false otherwise
 */
export async function IsRecordingSuspended(): Promise<boolean> {
    const result = await callCpp<{ outIsSuspended: boolean }>(SUITE_NAME, 'IsRecordingSuspended', {  });
    return result.outIsSuspended;
}
