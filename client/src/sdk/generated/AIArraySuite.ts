/**
 * AIArraySuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIArraySuite';

/**
 * Creates a new array.
 * @returns The array value
 */
export async function CreateArray(): Promise<any> {
    const result = await callCpp<{ array: any }>(SUITE_NAME, 'CreateArray', {  });
    return result.array;
}

/**
 * Adds a new item.
 * @param array - The Array value
 */
export async function AddRef(array: any): Promise<void> {
    await callCpp(SUITE_NAME, 'AddRef', { array });
}

/**
 * Performs the release operation.
 * @param array - The Array value
 */
export async function Release(array: any): Promise<void> {
    await callCpp(SUITE_NAME, 'Release', { array });
}

/**
 * Performs the clone operation.
 * @param src - The Src value
 * @returns The dst value
 */
export async function Clone(src: any): Promise<any> {
    const result = await callCpp<{ dst: any }>(SUITE_NAME, 'Clone', { src });
    return result.dst;
}

/**
 * Copies the object.
 * @param array - The Array value
 * @param src - The Src value
 */
export async function Copy(array: any, src: any): Promise<void> {
    await callCpp(SUITE_NAME, 'Copy', { array, src });
}

/**
 * Performs the size operation.
 * @param array - The Array value
 */
export async function Size(array: any): Promise<void> {
    await callCpp(SUITE_NAME, 'Size', { array });
}

/**
 * Removes the entry.
 * @param array - The Array value
 * @param i - The I value
 */
export async function DeleteEntry(array: any, i: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DeleteEntry', { array, i });
}

/**
 * Adds a new item.
 * @param array - The Array value
 * @param i - The I value
 */
export async function InsertEntry(array: any, i: number): Promise<void> {
    await callCpp(SUITE_NAME, 'InsertEntry', { array, i });
}

/**
 * Retrieves the entry type of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns The type value
 */
export async function GetEntryType(array: any, i: number): Promise<any> {
    const result = await callCpp<{ type: any }>(SUITE_NAME, 'GetEntryType', { array, i });
    return result.type;
}

/**
 * Copies the object.
 * @param array1 - The Array1 value
 * @param array2 - The Array2 value
 * @param position1 - The Position1 value
 * @param position2 - The Position2 value
 */
export async function CopyEntry(array1: any, array2: any, position1: number, position2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'CopyEntry', { array1, array2, position1, position2 });
}

/**
 * Moves the object.
 * @param array1 - The Array1 value
 * @param array2 - The Array2 value
 * @param position1 - The Position1 value
 * @param position2 - The Position2 value
 * @returns The newposition value
 */
export async function MoveEntry(array1: any, array2: any, position1: number, position2: number): Promise<number> {
    const result = await callCpp<{ newposition: number }>(SUITE_NAME, 'MoveEntry', { array1, array2, position1, position2 });
    return result.newposition;
}

/**
 * Performs the swap entries operation.
 * @param array1 - The Array1 value
 * @param array2 - The Array2 value
 * @param position1 - The Position1 value
 * @param position2 - The Position2 value
 */
export async function SwapEntries(array1: any, array2: any, position1: number, position2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SwapEntries', { array1, array2, position1, position2 });
}

/**
 * Retrieves the art entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns Handle to the art object
 */
export async function GetArtEntry(array: any, i: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'GetArtEntry', { array, i });
    return result.art;
}

/**
 * Creates a new art entry.
 * @param array - The Array value
 * @param i - The I value
 * @param type - The type value
 */
export async function NewArtEntry(array: any, i: number, type: number): Promise<void> {
    await callCpp(SUITE_NAME, 'NewArtEntry', { array, i, type });
}

/**
 * Moves the object.
 * @param array - The Array value
 * @param i - The I value
 * @param art - Handle to the art object
 */
export async function MoveArtToEntry(array: any, i: number, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'MoveArtToEntry', { array, i, art });
}

/**
 * Moves the object.
 * @param array - The Array value
 * @param i - The I value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function MoveEntryToArt(array: any, i: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'MoveEntryToArt', { array, i, paintOrder, prep });
    return result.art;
}

/**
 * Copies the object.
 * @param array - The Array value
 * @param i - The I value
 * @param art - Handle to the art object
 */
export async function CopyArtToEntry(array: any, i: number, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'CopyArtToEntry', { array, i, art });
}

/**
 * Copies the object.
 * @param array - The Array value
 * @param i - The I value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function CopyEntryToArt(array: any, i: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'CopyEntryToArt', { array, i, paintOrder, prep });
    return result.art;
}

/**
 * Sets the entry to layer of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param layer - Handle to the layer
 */
export async function SetEntryToLayer(array: any, i: number, layer: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetEntryToLayer', { array, i, layer });
}

/**
 * Sets the layer to entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the layer
 */
export async function SetLayerToEntry(array: any, i: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'SetLayerToEntry', { array, i, paintOrder, prep });
    return result.layer;
}

/**
 * Retrieves the  of an object.
 * @param array - The Array value
 * @param i - The I value
 */
export async function Get(array: any, i: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Get', { array, i });
}

/**
 * Sets the  of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param entry - The Entry value
 */
export async function Set(array: any, i: number, entry: any): Promise<void> {
    await callCpp(SUITE_NAME, 'Set', { array, i, entry });
}

/**
 * Retrieves the boolean entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns The value value
 */
export async function GetBooleanEntry(array: any, i: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'GetBooleanEntry', { array, i });
    return result.value;
}

/**
 * Sets the boolean entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetBooleanEntry(array: any, i: number, value: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetBooleanEntry', { array, i, value });
}

/**
 * Retrieves the integer entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns The value value
 */
export async function GetIntegerEntry(array: any, i: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'GetIntegerEntry', { array, i });
    return result.value;
}

/**
 * Sets the integer entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetIntegerEntry(array: any, i: number, value: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetIntegerEntry', { array, i, value });
}

/**
 * Retrieves the real entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns The value value
 */
export async function GetRealEntry(array: any, i: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'GetRealEntry', { array, i });
    return result.value;
}

/**
 * Sets the real entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetRealEntry(array: any, i: number, value: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetRealEntry', { array, i, value });
}

/**
 * Retrieves the string entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function GetStringEntry(array: any, i: number, value: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetStringEntry', { array, i, value });
}

/**
 * Sets the string entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetStringEntry(array: any, i: number, value: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetStringEntry', { array, i, value });
}

/**
 * Retrieves the binary entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns An object containing: value, size
 */
export async function GetBinaryEntry(array: any, i: number): Promise<{ value: void; size: any }> {
    const result = await callCpp<{ value: void; size: any }>(SUITE_NAME, 'GetBinaryEntry', { array, i });
    return result;
}

/**
 * Sets the binary entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param size - The Size value
 */
export async function SetBinaryEntry(array: any, i: number, size: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetBinaryEntry', { array, i, size });
    const result = await callCpp<{ value: void }>(SUITE_NAME, 'SetBinaryEntry', { array, i, size });
    return result.value;
}

/**
 * Retrieves the dict entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns The value value
 */
export async function GetDictEntry(array: any, i: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'GetDictEntry', { array, i });
    return result.value;
}

/**
 * Sets the dict entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetDictEntry(array: any, i: number, value: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDictEntry', { array, i, value });
}

/**
 * Retrieves the array entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @returns The value value
 */
export async function GetArrayEntry(array: any, i: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'GetArrayEntry', { array, i });
    return result.value;
}

/**
 * Sets the array entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetArrayEntry(array: any, i: number, value: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArrayEntry', { array, i, value });
}

/**
 * Retrieves the unicode string entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function GetUnicodeStringEntry(array: any, i: number, value: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetUnicodeStringEntry', { array, i, value });
}

/**
 * Sets the unicode string entry of an object.
 * @param array - The Array value
 * @param i - The I value
 * @param value - The Value value
 */
export async function SetUnicodeStringEntry(array: any, i: number, value: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetUnicodeStringEntry', { array, i, value });
}

/**
 * Performs the append entry operation.
 * @param array - The Array value
 * @param entry - The Entry value
 */
export async function AppendEntry(array: any, entry: any): Promise<void> {
    await callCpp(SUITE_NAME, 'AppendEntry', { array, entry });
}

/**
 * Performs the reserve operation.
 * @param array - The Array value
 * @param count - The count value
 */
export async function Reserve(array: any, count: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Reserve', { array, count });
}
