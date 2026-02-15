/**
 * AIFOConversionSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIFOConversionSuite';

/**
 * Performs the convert to native operation.
 * @param art - Handle to the art object
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns The options value
 */
export async function ConvertToNative(art: number, paintOrder: number, prep: number): Promise<any> {
    const result = await callCpp<{ options: any }>(SUITE_NAME, 'ConvertToNative', { art, paintOrder, prep });
    return result.options;
}

/**
 * Retrieves the conversion port of an object.
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns An object containing: options, port
 */
export async function GetConversionPort(paintOrder: number, prep: number): Promise<{ options: any; port: any }> {
    const result = await callCpp<{ options: any; port: any }>(SUITE_NAME, 'GetConversionPort', { paintOrder, prep });
    return result;
}

/**
 * Performs the release conversion port operation.
 * @param port - The Port value
 */
export async function ReleaseConversionPort(port: any): Promise<void> {
    await callCpp(SUITE_NAME, 'ReleaseConversionPort', { port });
}

/**
 * Performs the enumerate fonts operation.
 * @param art - Handle to the art object
 * @param visitor - The Visitor value
 */
export async function EnumerateFonts(art: number, visitor: any): Promise<void> {
    await callCpp(SUITE_NAME, 'EnumerateFonts', { art, visitor });
    const result = await callCpp<{ data: void }>(SUITE_NAME, 'EnumerateFonts', { art, visitor });
    return result.data;
}

/**
 * Performs the enumerate contents operation.
 * @param art - Handle to the art object
 * @param visitor - The Visitor value
 */
export async function EnumerateContents(art: number, visitor: any): Promise<void> {
    await callCpp(SUITE_NAME, 'EnumerateContents', { art, visitor });
    const result = await callCpp<{ data: void }>(SUITE_NAME, 'EnumerateContents', { art, visitor });
    return result.data;
}

/**
 * Performs the enumerate contents ex operation.
 * @param art - Handle to the art object
 * @param visitor - The Visitor value
 * @param flags - The Flags value
 */
export async function EnumerateContentsEx(art: number, visitor: any, flags: any): Promise<void> {
    await callCpp(SUITE_NAME, 'EnumerateContentsEx', { art, visitor, flags });
    const result = await callCpp<{ data: void }>(SUITE_NAME, 'EnumerateContentsEx', { art, visitor, flags });
    return result.data;
}

/**
 * Performs the outline legacy text operation.
 * @param art - Handle to the art object
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 */
export async function OutlineLegacyText(art: number, paintOrder: number, prep: number): Promise<void> {
    await callCpp(SUITE_NAME, 'OutlineLegacyText', { art, paintOrder, prep });
}

/**
 * Performs the enumerate embedded fonts operation.
 * @param art - Handle to the art object
 * @param visitor - The Visitor value
 */
export async function EnumerateEmbeddedFonts(art: number, visitor: any): Promise<void> {
    await callCpp(SUITE_NAME, 'EnumerateEmbeddedFonts', { art, visitor });
    const result = await callCpp<{ data: void }>(SUITE_NAME, 'EnumerateEmbeddedFonts', { art, visitor });
    return result.data;
}
