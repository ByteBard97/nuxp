/**
 * AIDictionarySuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIDictionarySuite';

/**
 * Creates a new dictionary.
 * @returns The dictionary value
 */
export async function CreateDictionary(): Promise<number> {
    const result = await callCpp<{ dictionary: number }>(SUITE_NAME, 'CreateDictionary', {  });
    return result.dictionary;
}

/**
 * Creates a new dictionary from j s o n file.
 * @param file - The File value
 * @returns The dictionary value
 */
export async function CreateDictionaryFromJSONFile(file: string): Promise<number> {
    const result = await callCpp<{ dictionary: number }>(SUITE_NAME, 'CreateDictionaryFromJSONFile', { file });
    return result.dictionary;
}

/**
 * Performs the clone operation.
 * @param src - The Src value
 * @returns The dst value
 */
export async function Clone(src: number): Promise<number> {
    const result = await callCpp<{ dst: number }>(SUITE_NAME, 'Clone', { src });
    return result.dst;
}

/**
 * Copies the object.
 * @param dict - The Dict value
 * @param src - The Src value
 */
export async function Copy(dict: number, src: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Copy', { dict, src });
}

/**
 * Performs the begin operation.
 * @param dict - The Dict value
 * @returns The iterator value
 */
export async function Begin(dict: number): Promise<number> {
    const result = await callCpp<{ iterator: number }>(SUITE_NAME, 'Begin', { dict });
    return result.iterator;
}

/**
 * Removes the entry.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 */
export async function DeleteEntry(dictionary: number, key: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DeleteEntry', { dictionary, key });
}

/**
 * Retrieves the entry type of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The type value
 */
export async function GetEntryType(dictionary: number, key: number): Promise<number> {
    const result = await callCpp<{ entryType: number }>(SUITE_NAME, 'GetEntryType', { dictionary, key });
    return result.entryType;
}

/**
 * Copies the object.
 * @param dictionary1 - The Dictionary1 value
 * @param dictionary2 - The Dictionary2 value
 * @param key1 - The Key1 value
 * @param key2 - The Key2 value
 */
export async function CopyEntry(dictionary1: number, dictionary2: number, key1: number, key2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'CopyEntry', { dictionary1, dictionary2, key1, key2 });
}

/**
 * Moves the object.
 * @param dictionary1 - The Dictionary1 value
 * @param dictionary2 - The Dictionary2 value
 * @param key1 - The Key1 value
 * @param key2 - The Key2 value
 */
export async function MoveEntry(dictionary1: number, dictionary2: number, key1: number, key2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'MoveEntry', { dictionary1, dictionary2, key1, key2 });
}

/**
 * Performs the swap entries operation.
 * @param dictionary1 - The Dictionary1 value
 * @param dictionary2 - The Dictionary2 value
 * @param key1 - The Key1 value
 * @param key2 - The Key2 value
 */
export async function SwapEntries(dictionary1: number, dictionary2: number, key1: number, key2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SwapEntries', { dictionary1, dictionary2, key1, key2 });
}

/**
 * Retrieves the art entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns Handle to the art object
 */
export async function GetArtEntry(dictionary: number, key: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'GetArtEntry', { dictionary, key });
    return result.art;
}

/**
 * Creates a new art entry.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param type - The type value
 */
export async function NewArtEntry(dictionary: number, key: number, type: number): Promise<void> {
    await callCpp(SUITE_NAME, 'NewArtEntry', { dictionary, key, type });
}

/**
 * Moves the object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param art - Handle to the art object
 */
export async function MoveArtToEntry(dictionary: number, key: number, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'MoveArtToEntry', { dictionary, key, art });
}

/**
 * Moves the object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function MoveEntryToArt(dictionary: number, key: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'MoveEntryToArt', { dictionary, key, paintOrder, prep });
    return result.art;
}

/**
 * Copies the object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param art - Handle to the art object
 */
export async function CopyArtToEntry(dictionary: number, key: number, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'CopyArtToEntry', { dictionary, key, art });
}

/**
 * Copies the object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function CopyEntryToArt(dictionary: number, key: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'CopyEntryToArt', { dictionary, key, paintOrder, prep });
    return result.art;
}

/**
 * Sets the entry to layer of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param layer - Handle to the layer
 */
export async function SetEntryToLayer(dictionary: number, key: number, layer: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetEntryToLayer', { dictionary, key, layer });
}

/**
 * Sets the layer to entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the layer
 */
export async function SetLayerToEntry(dictionary: number, key: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'SetLayerToEntry', { dictionary, key, paintOrder, prep });
    return result.layer;
}

/**
 * Sets the  of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param entry - The Entry value
 */
export async function Set(dictionary: number, key: number, entry: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Set', { dictionary, key, entry });
}

/**
 * Retrieves the boolean entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns True if the condition is met, false otherwise
 */
export async function GetBooleanEntry(dictionary: number, key: number): Promise<boolean> {
    const result = await callCpp<{ value: boolean }>(SUITE_NAME, 'GetBooleanEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the boolean entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetBooleanEntry(dictionary: number, key: number, value: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetBooleanEntry', { dictionary, key, value });
}

/**
 * Retrieves the integer entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The value value
 */
export async function GetIntegerEntry(dictionary: number, key: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'GetIntegerEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the integer entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetIntegerEntry(dictionary: number, key: number, value: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetIntegerEntry', { dictionary, key, value });
}

/**
 * Retrieves the pointer entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The value value
 */
export async function GetPointerEntry(dictionary: number, key: number): Promise<any> {
    const result = await callCpp<{ value: any }>(SUITE_NAME, 'GetPointerEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the pointer entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetPointerEntry(dictionary: number, key: number, value: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetPointerEntry', { dictionary, key, value });
}

/**
 * Retrieves the real entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The value value
 */
export async function GetRealEntry(dictionary: number, key: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'GetRealEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the real entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetRealEntry(dictionary: number, key: number, value: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetRealEntry', { dictionary, key, value });
}

/**
 * Sets the string entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetStringEntry(dictionary: number, key: number, value: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetStringEntry', { dictionary, key, value });
}

/**
 * Retrieves the dict entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The value value
 */
export async function GetDictEntry(dictionary: number, key: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'GetDictEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the dict entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetDictEntry(dictionary: number, key: number, value: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDictEntry', { dictionary, key, value });
}

/**
 * Retrieves the array entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The value value
 */
export async function GetArrayEntry(dictionary: number, key: number): Promise<number> {
    const result = await callCpp<{ value: number }>(SUITE_NAME, 'GetArrayEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the array entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetArrayEntry(dictionary: number, key: number, value: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArrayEntry', { dictionary, key, value });
}

/**
 * Retrieves the unicode string entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @returns The value value
 */
export async function GetUnicodeStringEntry(dictionary: number, key: number): Promise<string> {
    const result = await callCpp<{ value: string }>(SUITE_NAME, 'GetUnicodeStringEntry', { dictionary, key });
    return result.value;
}

/**
 * Sets the unicode string entry of an object.
 * @param dictionary - The Dictionary value
 * @param key - The Key value
 * @param value - The Value value
 */
export async function SetUnicodeStringEntry(dictionary: number, key: number, value: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetUnicodeStringEntry', { dictionary, key, value });
}

/**
 * Performs the touch art operation.
 * @param dictionary - The Dictionary value
 */
export async function TouchArt(dictionary: number): Promise<void> {
    await callCpp(SUITE_NAME, 'TouchArt', { dictionary });
}

/**
 * Performs the find operation.
 * @param dict - The Dict value
 * @param key - The Key value
 * @returns The iterator value
 */
export async function Find(dict: number, key: number): Promise<number> {
    const result = await callCpp<{ iterator: number }>(SUITE_NAME, 'Find', { dict, key });
    return result.iterator;
}
