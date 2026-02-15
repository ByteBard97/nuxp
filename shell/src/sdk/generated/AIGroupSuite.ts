/**
 * AIGroupSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIGroupSuite';

/**
 * Retrieves the group clipped of an object.
 * @param group - The Group value
 * @returns True if the condition is met, false otherwise
 */
export async function GetGroupClipped(group: number): Promise<boolean> {
    const result = await callCpp<{ clipped: boolean }>(SUITE_NAME, 'GetGroupClipped', { group });
    return result.clipped;
}

/**
 * Sets the group clipped of an object.
 * @param group - The Group value
 * @param clipped - The Clipped value
 */
export async function SetGroupClipped(group: number, clipped: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetGroupClipped', { group, clipped });
}

/**
 * Retrieves the group mask lock of an object.
 * @param group - The Group value
 * @returns True if the condition is met, false otherwise
 */
export async function GetGroupMaskLock(group: number): Promise<boolean> {
    const result = await callCpp<{ maskLocked: boolean }>(SUITE_NAME, 'GetGroupMaskLock', { group });
    return result.maskLocked;
}

/**
 * Sets the group mask lock of an object.
 * @param group - The Group value
 * @param maskLocked - The lock state
 */
export async function SetGroupMaskLock(group: number, maskLocked: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetGroupMaskLock', { group, maskLocked });
}
