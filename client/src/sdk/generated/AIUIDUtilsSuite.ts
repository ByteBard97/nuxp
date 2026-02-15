/**
 * AIUIDUtilsSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIUIDUtilsSuite';

/**
 * Retrieves the art u i d of an object.
 * @param art - Handle to the art object
 * @param create - The Create value
 * @returns The uid value
 */
export async function GetArtUID(art: number, create: boolean): Promise<any> {
    const result = await callCpp<{ uid: any }>(SUITE_NAME, 'GetArtUID', { art, create });
    return result.uid;
}

/**
 * Sets the art u i d of an object.
 * @param art - Handle to the art object
 * @param uid - The Uid value
 */
export async function SetArtUID(art: number, uid: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtUID', { art, uid });
}

/**
 * Performs the transfer art u i d operation.
 * @param srcart - Handle to the art object
 * @param dstart - Handle to the art object
 */
export async function TransferArtUID(srcart: number, dstart: number): Promise<void> {
    await callCpp(SUITE_NAME, 'TransferArtUID', { srcart, dstart });
}

/**
 * Creates a new art u i d r e f.
 * @param art - Handle to the art object
 * @returns The uidref value
 */
export async function NewArtUIDREF(art: number): Promise<any> {
    const result = await callCpp<{ uidref: any }>(SUITE_NAME, 'NewArtUIDREF', { art });
    return result.uidref;
}

/**
 * Retrieves the referenced art of an object.
 * @param uidref - The Uidref value
 * @returns Handle to the art object
 */
export async function GetReferencedArt(uidref: any): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'GetReferencedArt', { uidref });
    return result.art;
}

/**
 * Retrieves the art u i d name of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 */
export async function GetArtUIDName(art: number, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetArtUIDName', { art, name });
}

/**
 * Sets the art u i d name of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 */
export async function SetArtUIDName(art: number, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtUIDName', { art, name });
}

/**
 * Retrieves the art name or u i d of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 * @returns The is default name value
 */
export async function GetArtNameOrUID(art: number, name: string): Promise<any> {
    const result = await callCpp<{ isDefaultName: any }>(SUITE_NAME, 'GetArtNameOrUID', { art, name });
    return result.isDefaultName;
}

/**
 * Sets the art name or u i d of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 */
export async function SetArtNameOrUID(art: number, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtNameOrUID', { art, name });
}

/**
 * Performs the make u i d from base operation.
 * @param base - The Base value
 * @returns The uid value
 */
export async function MakeUIDFromBase(base: any): Promise<any> {
    const result = await callCpp<{ uid: any }>(SUITE_NAME, 'MakeUIDFromBase', { base });
    return result.uid;
}

/**
 * Performs the make unique name from base operation.
 * @param base - The Base value
 * @param name - The name string
 */
export async function MakeUniqueNameFromBase(base: string, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'MakeUniqueNameFromBase', { base, name });
}

/**
 * Retrieves the story u i d of an object.
 * @param art - Handle to the art object
 * @param create - The Create value
 * @returns The ruid value
 */
export async function GetStoryUID(art: number, create: boolean): Promise<any> {
    const result = await callCpp<{ ruid: any }>(SUITE_NAME, 'GetStoryUID', { art, create });
    return result.ruid;
}

/**
 * Sets the story u i d of an object.
 * @param art - Handle to the art object
 * @param uid - The Uid value
 */
export async function SetStoryUID(art: number, uid: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetStoryUID', { art, uid });
}

/**
 * Performs the transfer story u i d operation.
 * @param srcart - Handle to the art object
 * @param dstart - Handle to the art object
 */
export async function TransferStoryUID(srcart: number, dstart: number): Promise<void> {
    await callCpp(SUITE_NAME, 'TransferStoryUID', { srcart, dstart });
}

/**
 * Creates a new story u i d r e f.
 * @param art - Handle to the art object
 * @returns The uidref value
 */
export async function NewStoryUIDREF(art: number): Promise<any> {
    const result = await callCpp<{ uidref: any }>(SUITE_NAME, 'NewStoryUIDREF', { art });
    return result.uidref;
}

/**
 * Performs the transfer story u i d to art operation.
 * @param frame - The Frame value
 * @param art - Handle to the art object
 */
export async function TransferStoryUIDToArt(frame: number, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'TransferStoryUIDToArt', { frame, art });
}

/**
 * Performs the transfer art u i d to story operation.
 * @param art - Handle to the art object
 * @param frame - The Frame value
 */
export async function TransferArtUIDToStory(art: number, frame: number): Promise<void> {
    await callCpp(SUITE_NAME, 'TransferArtUIDToStory', { art, frame });
}

/**
 * Performs the find equiv u i d operation.
 * @param target - The Target value
 * @param equiv - The Equiv value
 */
export async function FindEquivUID(target: any, equiv: any): Promise<void> {
    await callCpp(SUITE_NAME, 'FindEquivUID', { target, equiv });
}
