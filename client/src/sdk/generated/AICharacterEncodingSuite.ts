/**
 * AICharacterEncodingSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AICharacterEncodingSuite';

/**
 * Performs the convert buffer operation.
 * @param srcBuffer - The Src Buffer value
 * @param srcBytes - The Src Bytes value
 * @param srcEncoding - The Src Encoding value
 * @param dstBytes - The Dst Bytes value
 * @param dstEncoding - The Dst Encoding value
 * @param errIfCantMap - The Err If Cant Map value
 * @returns An object containing: dstBuffer, dstLength
 */
export async function ConvertBuffer(srcBuffer: void, srcBytes: number, srcEncoding: any, dstBytes: number, dstEncoding: any, errIfCantMap: boolean): Promise<{ dstBuffer: void; dstLength: number }> {
    const result = await callCpp<{ dstBuffer: void; dstLength: number }>(SUITE_NAME, 'ConvertBuffer', { srcBuffer, srcBytes, srcEncoding, dstBytes, dstEncoding, errIfCantMap });
    return result;
}

/**
 * Performs the convert japanese char operation.
 * @param srcEncoding - The Src Encoding value
 * @param dstEncoding - The Dst Encoding value
 * @returns The ch value
 */
export async function ConvertJapaneseChar(srcEncoding: any, dstEncoding: any): Promise<any> {
    const result = await callCpp<{ ch: any }>(SUITE_NAME, 'ConvertJapaneseChar', { srcEncoding, dstEncoding });
    return result.ch;
}

/**
 * Checks if is punct.
 * @param inChar - The In Char value
 */
export async function IsPunct(inChar: any): Promise<void> {
    await callCpp(SUITE_NAME, 'IsPunct', { inChar });
}

/**
 * Checks if is space.
 * @param inChar - The In Char value
 */
export async function IsSpace(inChar: any): Promise<void> {
    await callCpp(SUITE_NAME, 'IsSpace', { inChar });
}

/**
 * Retrieves the checked preferred encoding of an object.
 * @param inUnicodeText - The In Unicode Text value
 * @param inDefaultEncoding - The In Default Encoding value
 * @returns The out m b c s encoding value
 */
export async function GetCheckedPreferredEncoding(inUnicodeText: any, inDefaultEncoding: any): Promise<any> {
    const result = await callCpp<{ outMBCSEncoding: any }>(SUITE_NAME, 'GetCheckedPreferredEncoding', { inUnicodeText, inDefaultEncoding });
    return result.outMBCSEncoding;
}
