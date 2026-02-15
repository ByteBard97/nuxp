/**
 * AIUIDPoolSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIUIDPoolSuite';

/**
 * Adds a new item.
 */
export async function AddRef(): Promise<void> {
    await callCpp(SUITE_NAME, 'AddRef', {  });
    const result = await callCpp<{ pool: void }>(SUITE_NAME, 'AddRef', {  });
    return result.pool;
}

/**
 * Performs the release operation.
 */
export async function Release(): Promise<void> {
    await callCpp(SUITE_NAME, 'Release', {  });
    const result = await callCpp<{ pool: void }>(SUITE_NAME, 'Release', {  });
    return result.pool;
}

/**
 * Retrieves the pool of an object.
 * @param name - The name string
 * @returns The pool value
 */
export async function GetPool(name: any): Promise<any> {
    const result = await callCpp<{ pool: any }>(SUITE_NAME, 'GetPool', { name });
    return result.pool;
}

/**
 * Creates a new u i d.
 * @param pool - The Pool value
 * @param name - The name string
 * @returns The uid value
 */
export async function NewUID(pool: any, name: string): Promise<any> {
    const result = await callCpp<{ uid: any }>(SUITE_NAME, 'NewUID', { pool, name });
    return result.uid;
}

/**
 * Retrieves the u i d of an object.
 * @param pool - The Pool value
 * @param name - The name string
 * @returns The uid value
 */
export async function GetUID(pool: any, name: string): Promise<any> {
    const result = await callCpp<{ uid: any }>(SUITE_NAME, 'GetUID', { pool, name });
    return result.uid;
}

/**
 * Creates a new u i d r e f.
 * @param pool - The Pool value
 * @param name - The name string
 * @returns The uidref value
 */
export async function NewUIDREF(pool: any, name: string): Promise<any> {
    const result = await callCpp<{ uidref: any }>(SUITE_NAME, 'NewUIDREF', { pool, name });
    return result.uidref;
}

/**
 * Creates a new u i d from base.
 * @param pool - The Pool value
 * @param base - The Base value
 * @returns The uid value
 */
export async function NewUIDFromBase(pool: any, base: string): Promise<any> {
    const result = await callCpp<{ uid: any }>(SUITE_NAME, 'NewUIDFromBase', { pool, base });
    return result.uid;
}
