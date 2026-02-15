/**
 * AIUIDREFSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIUIDREFSuite';

/**
 * Adds a new item.
 */
export async function AddRef(): Promise<void> {
    await callCpp(SUITE_NAME, 'AddRef', {  });
    const result = await callCpp<{ uidref: void }>(SUITE_NAME, 'AddRef', {  });
    return result.uidref;
}

/**
 * Performs the release operation.
 */
export async function Release(): Promise<void> {
    await callCpp(SUITE_NAME, 'Release', {  });
    const result = await callCpp<{ uidref: void }>(SUITE_NAME, 'Release', {  });
    return result.uidref;
}

/**
 * Checks if is in use.
 * @param uidref - The Uidref value
 */
export async function IsInUse(uidref: any): Promise<void> {
    await callCpp(SUITE_NAME, 'IsInUse', { uidref });
}

/**
 * Retrieves the pool of an object.
 * @param uidref - The Uidref value
 * @returns The pool value
 */
export async function GetPool(uidref: any): Promise<any> {
    const result = await callCpp<{ pool: any }>(SUITE_NAME, 'GetPool', { uidref });
    return result.pool;
}

/**
 * Retrieves the name of an object.
 * @param uidref - The Uidref value
 * @param name - The name string
 */
export async function GetName(uidref: any, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetName', { uidref, name });
}

/**
 * Retrieves the container of an object.
 * @param uidref - The Uidref value
 */
export async function GetContainer(uidref: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetContainer', { uidref });
}

/**
 * Retrieves the u i d of an object.
 * @param uidref - The Uidref value
 * @returns The uid value
 */
export async function GetUID(uidref: any): Promise<any> {
    const result = await callCpp<{ uid: any }>(SUITE_NAME, 'GetUID', { uidref });
    return result.uid;
}
