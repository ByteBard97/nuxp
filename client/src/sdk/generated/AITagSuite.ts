/**
 * AITagSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AITagSuite';

/**
 * Sets the tag of an object.
 * @param name - The name string
 * @param type - The type value
 */
export async function SetTag(name: any, type: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetTag', { name, type });
    const result = await callCpp<{ object: void }>(SUITE_NAME, 'SetTag', { name, type });
    return result.object;
}

/**
 * Retrieves the tag of an object.
 * @param name - The name string
 * @param type - The type value
 */
export async function GetTag(name: any, type: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetTag', { name, type });
    const result = await callCpp<{ object: void }>(SUITE_NAME, 'GetTag', { name, type });
    return result.object;
}

/**
 * Retrieves the tag type of an object.
 * @param name - The name string
 * @param type - The type value
 */
export async function GetTagType(name: any, type: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetTagType', { name, type });
    const result = await callCpp<{ object: void }>(SUITE_NAME, 'GetTagType', { name, type });
    return result.object;
}

/**
 * Retrieves the tag change count of an object.
 * @param name - The name string
 * @returns An object containing: object, count
 */
export async function GetTagChangeCount(name: any): Promise<{ object: void; count: number }> {
    const result = await callCpp<{ object: void; count: number }>(SUITE_NAME, 'GetTagChangeCount', { name });
    return result;
}

/**
 * Removes the tag.
 * @param name - The name string
 */
export async function RemoveTag(name: any): Promise<void> {
    await callCpp(SUITE_NAME, 'RemoveTag', { name });
    const result = await callCpp<{ object: void }>(SUITE_NAME, 'RemoveTag', { name });
    return result.object;
}

/**
 * Counts the number of tags objects.
 * @returns An object containing: object, count
 */
export async function CountTags(): Promise<{ object: void; count: number }> {
    const result = await callCpp<{ object: void; count: number }>(SUITE_NAME, 'CountTags', {  });
    return result;
}

/**
 * Retrieves the nth tag of an object.
 * @param n - The N value
 * @returns An object containing: object, name
 */
export async function GetNthTag(n: number): Promise<{ object: void; name: any }> {
    const result = await callCpp<{ object: void; name: any }>(SUITE_NAME, 'GetNthTag', { n });
    return result;
}
