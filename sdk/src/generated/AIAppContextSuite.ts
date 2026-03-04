/**
 * AIAppContextSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIAppContextSuite';

/**
 * Retrieves the platform app window of an object.
 * @returns The app window value
 */
export async function GetPlatformAppWindow(): Promise<any> {
    const result = await callCpp<{ appWindow: any }>(SUITE_NAME, 'GetPlatformAppWindow', {  });
    return result.appWindow;
}

/**
 * Performs the allow all changes operation.
 * @param allowAllChanges - The Allow All Changes value
 * @returns True if the condition is met, false otherwise
 */
export async function AllowAllChanges(allowAllChanges: boolean): Promise<boolean> {
    const result = await callCpp<{ previousState: boolean }>(SUITE_NAME, 'AllowAllChanges', { allowAllChanges });
    return result.previousState;
}

/**
 * Performs the allow progress operation.
 * @param showProgress - The Show Progress value
 */
export async function AllowProgress(showProgress: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'AllowProgress', { showProgress });
}

/**
 * Retrieves the platform app menu of an object.
 * @returns The app menu value
 */
export async function GetPlatformAppMenu(): Promise<any> {
    const result = await callCpp<{ appMenu: any }>(SUITE_NAME, 'GetPlatformAppMenu', {  });
    return result.appMenu;
}

/**
 * Performs the sync and draw operation.
 */
export async function SyncAndDraw(): Promise<void> {
    await callCpp(SUITE_NAME, 'SyncAndDraw', {  });
}

/**
 * Retrieves the ruler width offset on active document of an object.
 * @returns An object containing: horzRulerBounds, vertRulerBounds, cntrRulerBounds
 */
export async function GetRulerWidthOffsetOnActiveDocument(): Promise<{ horzRulerBounds: any; vertRulerBounds: any; cntrRulerBounds: any }> {
    const result = await callCpp<{ horzRulerBounds: any; vertRulerBounds: any; cntrRulerBounds: any }>(SUITE_NAME, 'GetRulerWidthOffsetOnActiveDocument', {  });
    return result;
}

/**
 * Checks if is progress bar allowed.
 * @returns True if the condition is met, false otherwise
 */
export async function IsProgressBarAllowed(): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'IsProgressBarAllowed', {  });
    return result.result;
}
