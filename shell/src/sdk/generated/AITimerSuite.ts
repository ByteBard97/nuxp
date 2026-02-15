/**
 * AITimerSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AITimerSuite';

/**
 * Retrieves the timer name of an object.
 * @param timer - The Timer value
 * @returns The name string
 */
export async function GetTimerName(timer: number): Promise<string> {
    const result = await callCpp<{ name: string }>(SUITE_NAME, 'GetTimerName', { timer });
    return result.name;
}

/**
 * Retrieves the timer active of an object.
 * @param timer - The Timer value
 * @returns True if the condition is met, false otherwise
 */
export async function GetTimerActive(timer: number): Promise<boolean> {
    const result = await callCpp<{ active: boolean }>(SUITE_NAME, 'GetTimerActive', { timer });
    return result.active;
}

/**
 * Sets the timer active of an object.
 * @param timer - The Timer value
 * @param active - The Active value
 */
export async function SetTimerActive(timer: number, active: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetTimerActive', { timer, active });
}

/**
 * Retrieves the timer period of an object.
 * @param timer - The Timer value
 * @returns The period value
 */
export async function GetTimerPeriod(timer: number): Promise<number> {
    const result = await callCpp<{ period: number }>(SUITE_NAME, 'GetTimerPeriod', { timer });
    return result.period;
}

/**
 * Sets the timer period of an object.
 * @param timer - The Timer value
 * @param period - The Period value
 */
export async function SetTimerPeriod(timer: number, period: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetTimerPeriod', { timer, period });
}

/**
 * Retrieves the timer plugin of an object.
 * @param timer - The Timer value
 * @returns The plugin value
 */
export async function GetTimerPlugin(timer: number): Promise<any> {
    const result = await callCpp<{ plugin: any }>(SUITE_NAME, 'GetTimerPlugin', { timer });
    return result.plugin;
}

/**
 * Counts the number of timers objects.
 * @returns The count value
 */
export async function CountTimers(): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountTimers', {  });
    return result.count;
}

/**
 * Retrieves the nth timer of an object.
 * @param n - The N value
 * @returns The timer value
 */
export async function GetNthTimer(n: number): Promise<number> {
    const result = await callCpp<{ timer: number }>(SUITE_NAME, 'GetNthTimer', { n });
    return result.timer;
}
