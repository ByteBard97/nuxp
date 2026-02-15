/**
 * AIUIDSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIUIDSuite';

/**
 * Adds a new item.
 */
export async function AddRef(): Promise<void> {
    await callCpp(SUITE_NAME, 'AddRef', {  });
    const result = await callCpp<{ uid: void }>(SUITE_NAME, 'AddRef', {  });
    return result.uid;
}

/**
 * Performs the release operation.
 */
export async function Release(): Promise<void> {
    await callCpp(SUITE_NAME, 'Release', {  });
    const result = await callCpp<{ uid: void }>(SUITE_NAME, 'Release', {  });
    return result.uid;
}

/**
 * Checks if is in use.
 * @param uid - The Uid value
 */
export async function IsInUse(uid: any): Promise<void> {
    await callCpp(SUITE_NAME, 'IsInUse', { uid });
}

/**
 * Retrieves the pool of an object.
 * @param uid - The Uid value
 * @returns The pool value
 */
export async function GetPool(uid: any): Promise<any> {
    const result = await callCpp<{ pool: any }>(SUITE_NAME, 'GetPool', { uid });
    return result.pool;
}

/**
 * Retrieves the name of an object.
 * @param uid - The Uid value
 * @param name - The name string
 */
export async function GetName(uid: any, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetName', { uid, name });
}

/**
 * Retrieves the container of an object.
 * @param uid - The Uid value
 */
export async function GetContainer(uid: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetContainer', { uid });
}

/**
 * Creates a new u i d r e f.
 * @param uid - The Uid value
 * @returns The ruidref value
 */
export async function NewUIDREF(uid: any): Promise<any> {
    const result = await callCpp<{ ruidref: any }>(SUITE_NAME, 'NewUIDREF', { uid });
    return result.ruidref;
}
