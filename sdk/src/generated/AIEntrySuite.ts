/**
 * AIEntrySuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';
import { AIRealMatrix, AIRealPoint } from './types';

const SUITE_NAME = 'AIEntrySuite';

/**
 * Performs the to boolean operation.
 * @param entry - The Entry value
 * @returns True if the condition is met, false otherwise
 */
export async function ToBoolean(entry: number): Promise<boolean> {
    const result = await callCpp<{ value: boolean }>(SUITE_NAME, 'ToBoolean', { entry });
    return result.value;
}

/**
 * Performs the to integer operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToInteger(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'ToInteger', { entry });
    return result.value;
}

/**
 * Performs the to real operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToReal(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'ToReal', { entry });
    return result.value;
}

/**
 * Performs the to real point operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToRealPoint(entry: number): Promise<AIRealPoint> {
    const result = await callCpp<{ value: AIRealPoint }>(SUITE_NAME, 'ToRealPoint', { entry });
    return result.value;
}

/**
 * Performs the to real matrix operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToRealMatrix(entry: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ value: AIRealMatrix }>(SUITE_NAME, 'ToRealMatrix', { entry });
    return result.value;
}

/**
 * Performs the to dict operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToDict(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'ToDict', { entry });
    return result.value;
}

/**
 * Performs the to art operation.
 * @param entry - The Entry value
 * @returns Handle to the art object
 */
export async function ToArt(entry: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'ToArt', { entry });
    return result.art;
}

/**
 * Performs the to array operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToArray(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'ToArray', { entry });
    return result.value;
}

/**
 * Performs the to custom color operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToCustomColor(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'ToCustomColor', { entry });
    return result.value;
}

/**
 * Performs the to plugin object operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToPluginObject(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToPluginObject', { entry });
    return result.value;
}

/**
 * Performs the to fill style operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToFillStyle(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToFillStyle', { entry });
    return result.value;
}

/**
 * Performs the to stroke style operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToStrokeStyle(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToStrokeStyle', { entry });
    return result.value;
}

/**
 * Performs the to u i d operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToUID(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToUID', { entry });
    return result.value;
}

/**
 * Performs the to u i d r e f operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToUIDREF(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToUIDREF', { entry });
    return result.value;
}

/**
 * Performs the to x m l node operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToXMLNode(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToXMLNode', { entry });
    return result.value;
}

/**
 * Performs the to s v g filter handle operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToSVGFilterHandle(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'ToSVGFilterHandle', { entry });
    return result.value;
}

/**
 * Performs the as boolean operation.
 * @param entry - The Entry value
 * @returns True if the condition is met, false otherwise
 */
export async function AsBoolean(entry: number): Promise<boolean> {
    const result = await callCpp<{ value: boolean }>(SUITE_NAME, 'AsBoolean', { entry });
    return result.value;
}

/**
 * Performs the as integer operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function AsInteger(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'AsInteger', { entry });
    return result.value;
}

/**
 * Performs the as real operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function AsReal(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'AsReal', { entry });
    return result.value;
}

/**
 * Performs the as u i d r e f operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function AsUIDREF(entry: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'AsUIDREF', { entry });
    return result.value;
}

/**
 * Performs the to art style operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToArtStyle(entry: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'ToArtStyle', { entry });
    return result.value;
}

/**
 * Performs the to unicode string operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function ToUnicodeString(entry: number): Promise<string> {
    const result = await callCpp<{ value: string }>(SUITE_NAME, 'ToUnicodeString', { entry });
    return result.value;
}

/**
 * Performs the as unicode string operation.
 * @param entry - The Entry value
 * @returns The value value
 */
export async function AsUnicodeString(entry: number): Promise<string> {
    const result = await callCpp<{ value: string }>(SUITE_NAME, 'AsUnicodeString', { entry });
    return result.value;
}
