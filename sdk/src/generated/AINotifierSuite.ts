/**
 * AINotifierSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AINotifierSuite';

/**
 * Adds a new item.
 * @param self - The Self value
 * @param name - The name string
 * @param type - The type value
 * @param options - The point coordinates
 * @returns The notifier value
 */
export async function AddNotifierEx(self: any, name: string, type: string, options: any): Promise<number> {
    const result = await callCpp<{ notifier: number }>(SUITE_NAME, 'AddNotifierEx', { self, name, type, options });
    return result.notifier;
}

/**
 * Retrieves the notifier options of an object.
 * @param notifier - The Notifier value
 * @returns The options value
 */
export async function GetNotifierOptions(notifier: number): Promise<any> {
    const result = await callCpp<{ options: any }>(SUITE_NAME, 'GetNotifierOptions', { notifier });
    return result.options;
}

/**
 * Retrieves the notifier active of an object.
 * @param notifier - The Notifier value
 * @returns True if the condition is met, false otherwise
 */
export async function GetNotifierActive(notifier: number): Promise<boolean> {
    const result = await callCpp<{ active: boolean }>(SUITE_NAME, 'GetNotifierActive', { notifier });
    return result.active;
}

/**
 * Sets the notifier active of an object.
 * @param notifier - The Notifier value
 * @param active - The Active value
 */
export async function SetNotifierActive(notifier: number, active: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetNotifierActive', { notifier, active });
}

/**
 * Retrieves the notifier plugin of an object.
 * @param notifier - The Notifier value
 * @returns The plugin value
 */
export async function GetNotifierPlugin(notifier: number): Promise<any> {
    const result = await callCpp<{ plugin: any }>(SUITE_NAME, 'GetNotifierPlugin', { notifier });
    return result.plugin;
}

/**
 * Counts the number of notifiers objects.
 * @returns The count value
 */
export async function CountNotifiers(): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountNotifiers', {  });
    return result.count;
}

/**
 * Retrieves the nth notifier of an object.
 * @param n - The N value
 * @returns The notifier value
 */
export async function GetNthNotifier(n: number): Promise<number> {
    const result = await callCpp<{ notifier: number }>(SUITE_NAME, 'GetNthNotifier', { n });
    return result.notifier;
}
