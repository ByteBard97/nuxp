/**
 * AIXMLNameUtilSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIXMLNameUtilSuite';

/**
 * Performs the name from base and number operation.
 * @param base - The Base value
 * @param buffer - The Buffer value
 * @param number - The Number value
 * @param maxlength - The Maxlength value
 */
export async function NameFromBaseAndNumber(base: string, buffer: string, number: number, maxlength: number): Promise<void> {
    await callCpp(SUITE_NAME, 'NameFromBaseAndNumber', { base, buffer, number, maxlength });
}

/**
 * Performs the base from name operation.
 * @param name - The name string
 * @param buffer - The Buffer value
 */
export async function BaseFromName(name: string, buffer: string): Promise<void> {
    await callCpp(SUITE_NAME, 'BaseFromName', { name, buffer });
}

/**
 * Checks if is valid name.
 * @param name - The name string
 */
export async function IsValidName(name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'IsValidName', { name });
}

/**
 * Performs the x m l name from string operation.
 * @param str - The Str value
 * @param buffer - The Buffer value
 */
export async function XMLNameFromString(str: string, buffer: string): Promise<void> {
    await callCpp(SUITE_NAME, 'XMLNameFromString', { str, buffer });
}

/**
 * Performs the string from x m l name operation.
 * @param name - The name string
 * @param buffer - The Buffer value
 */
export async function StringFromXMLName(name: string, buffer: string): Promise<void> {
    await callCpp(SUITE_NAME, 'StringFromXMLName', { name, buffer });
}
