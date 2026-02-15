/**
 * AIMaskSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIMaskSuite';

/**
 * Retrieves the mask of an object.
 * @param object - The Object value
 * @returns The mask value
 */
export async function GetMask(object: number): Promise<number> {
    const result = await callCpp<{ mask: number }>(SUITE_NAME, 'GetMask', { object });
    return result.mask;
}

/**
 * Creates a new mask.
 * @param object - The Object value
 */
export async function CreateMask(object: number): Promise<void> {
    await callCpp(SUITE_NAME, 'CreateMask', { object });
}

/**
 * Removes the mask.
 * @param object - The Object value
 */
export async function DeleteMask(object: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DeleteMask', { object });
}

/**
 * Retrieves the linked of an object.
 * @param mask - The Mask value
 * @returns True if the condition is met, false otherwise
 */
export async function GetLinked(mask: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetLinked', { mask });
    return result.result;
}

/**
 * Sets the linked of an object.
 * @param mask - The Mask value
 * @param linked - The Linked value
 */
export async function SetLinked(mask: number, linked: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLinked', { mask, linked });
}

/**
 * Retrieves the disabled of an object.
 * @param mask - The Mask value
 * @returns True if the condition is met, false otherwise
 */
export async function GetDisabled(mask: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetDisabled', { mask });
    return result.result;
}

/**
 * Sets the disabled of an object.
 * @param mask - The Mask value
 * @param disabled - The Disabled value
 */
export async function SetDisabled(mask: number, disabled: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDisabled', { mask, disabled });
}

/**
 * Retrieves the inverted of an object.
 * @param mask - The Mask value
 * @returns True if the condition is met, false otherwise
 */
export async function GetInverted(mask: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetInverted', { mask });
    return result.result;
}

/**
 * Sets the inverted of an object.
 * @param mask - The Mask value
 * @param inverted - The Inverted value
 */
export async function SetInverted(mask: number, inverted: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetInverted', { mask, inverted });
}

/**
 * Copies the object.
 * @param source - The Source value
 * @param destination - The Destination value
 */
export async function Copy(source: number, destination: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Copy', { source, destination });
}

/**
 * Retrieves the art of an object.
 * @param mask - The Mask value
 * @returns Handle ID for the returned AIArtHandle
 */
export async function GetArt(mask: number): Promise<number> {
    const result = await callCpp<{ result: number }>(SUITE_NAME, 'GetArt', { mask });
    return result.result;
}

/**
 * Checks if is editing art.
 * @param mask - The Mask value
 * @returns True if the condition is met, false otherwise
 */
export async function IsEditingArt(mask: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'IsEditingArt', { mask });
    return result.result;
}

/**
 * Sets the editing art of an object.
 * @param mask - The Mask value
 * @param isedit - The Isedit value
 */
export async function SetEditingArt(mask: number, isedit: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetEditingArt', { mask, isedit });
}

/**
 * Retrieves the masked art of an object.
 * @param mask - The Mask value
 * @returns The masked value
 */
export async function GetMaskedArt(mask: number): Promise<number> {
    const result = await callCpp<{ masked: number }>(SUITE_NAME, 'GetMaskedArt', { mask });
    return result.masked;
}

/**
 * Retrieves the clipping of an object.
 * @param mask - The Mask value
 * @returns True if the condition is met, false otherwise
 */
export async function GetClipping(mask: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetClipping', { mask });
    return result.result;
}

/**
 * Sets the clipping of an object.
 * @param mask - The Mask value
 * @param clipping - The Clipping value
 */
export async function SetClipping(mask: number, clipping: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetClipping', { mask, clipping });
}
