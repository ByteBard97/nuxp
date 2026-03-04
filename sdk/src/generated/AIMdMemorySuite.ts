/**
 * AIMdMemorySuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIMdMemorySuite';

/**
 * Performs the md memory new handle operation.
 * @param size - The Size value
 * @returns The h mem value
 */
export async function MdMemoryNewHandle(size: number): Promise<any> {
    const result = await callCpp<{ hMem: any }>(SUITE_NAME, 'MdMemoryNewHandle', { size });
    return result.hMem;
}

/**
 * Performs the md memory dispose handle operation.
 * @param hMem - The H Mem value
 */
export async function MdMemoryDisposeHandle(hMem: any): Promise<void> {
    await callCpp(SUITE_NAME, 'MdMemoryDisposeHandle', { hMem });
}

/**
 * Performs the md memory get size operation.
 * @param hMem - The H Mem value
 * @returns The size value
 */
export async function MdMemoryGetSize(hMem: any): Promise<number> {
    const result = await callCpp<{ size: number }>(SUITE_NAME, 'MdMemoryGetSize', { hMem });
    return result.size;
}

/**
 * Performs the md memory resize operation.
 * @param hMem - The H Mem value
 * @param newSize - The New Size value
 */
export async function MdMemoryResize(hMem: any, newSize: number): Promise<void> {
    await callCpp(SUITE_NAME, 'MdMemoryResize', { hMem, newSize });
}

/**
 * Performs the md memory un lock operation.
 * @param hMem - The H Mem value
 */
export async function MdMemoryUnLock(hMem: any): Promise<void> {
    await callCpp(SUITE_NAME, 'MdMemoryUnLock', { hMem });
}
