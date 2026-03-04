/**
 * AILayerListSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AILayerListSuite';

/**
 * Retrieves the layer of art of an object.
 * @param art - Handle to the art object
 * @returns An object containing: list, layer
 */
export async function GetLayerOfArt(art: number): Promise<{ list: any; layer: number }> {
    const result = await callCpp<{ list: any; layer: number }>(SUITE_NAME, 'GetLayerOfArt', { art });
    return result;
}

/**
 * Counts the number of  objects.
 * @returns The count value
 */
export async function Count(): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'Count', {  });
    return result.count;
}

/**
 * Retrieves the first of an object.
 * @returns The list value
 */
export async function GetFirst(): Promise<any> {
    const result = await callCpp<{ list: any }>(SUITE_NAME, 'GetFirst', {  });
    return result.list;
}

/**
 * Retrieves the last of an object.
 * @returns The list value
 */
export async function GetLast(): Promise<any> {
    const result = await callCpp<{ list: any }>(SUITE_NAME, 'GetLast', {  });
    return result.list;
}

/**
 * Retrieves the next of an object.
 * @param list - The List value
 * @returns The next value
 */
export async function GetNext(list: any): Promise<any> {
    const result = await callCpp<{ next: any }>(SUITE_NAME, 'GetNext', { list });
    return result.next;
}

/**
 * Counts the number of layers objects.
 * @param list - The List value
 * @returns The count value
 */
export async function CountLayers(list: any): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountLayers', { list });
    return result.count;
}

/**
 * Retrieves the first layer of an object.
 * @param list - The List value
 * @returns Handle to the layer
 */
export async function GetFirstLayer(list: any): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'GetFirstLayer', { list });
    return result.layer;
}

/**
 * Retrieves the last layer of an object.
 * @param list - The List value
 * @returns Handle to the layer
 */
export async function GetLastLayer(list: any): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'GetLastLayer', { list });
    return result.layer;
}

/**
 * Retrieves the next layer of an object.
 * @param list - The List value
 * @param layer - Handle to the layer
 * @returns The next value
 */
export async function GetNextLayer(list: any, layer: number): Promise<number> {
    const result = await callCpp<{ next: number }>(SUITE_NAME, 'GetNextLayer', { list, layer });
    return result.next;
}

/**
 * Retrieves the prev layer of an object.
 * @param list - The List value
 * @param layer - Handle to the layer
 * @returns The prev value
 */
export async function GetPrevLayer(list: any, layer: number): Promise<number> {
    const result = await callCpp<{ prev: number }>(SUITE_NAME, 'GetPrevLayer', { list, layer });
    return result.prev;
}

/**
 * Sets the display mode of an object.
 * @param list - The List value
 * @param mode - The Mode value
 */
export async function SetDisplayMode(list: any, mode: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDisplayMode', { list, mode });
}

/**
 * Sets the editability mode of an object.
 * @param list - The List value
 * @param mode - The Mode value
 */
export async function SetEditabilityMode(list: any, mode: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetEditabilityMode', { list, mode });
}
