/**
 * AIToolSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIToolSuite';

/**
 * Retrieves the tool name of an object.
 * @param tool - The Tool value
 * @returns The name string
 */
export async function GetToolName(tool: number): Promise<string> {
    const result = await callCpp<{ name: string }>(SUITE_NAME, 'GetToolName', { tool });
    return result.name;
}

/**
 * Retrieves the tool options of an object.
 * @param tool - The Tool value
 * @returns The options value
 */
export async function GetToolOptions(tool: number): Promise<number> {
    const result = await callCpp<{ options: number }>(SUITE_NAME, 'GetToolOptions', { tool });
    return result.options;
}

/**
 * Sets the tool options of an object.
 * @param tool - The Tool value
 * @param options - The point coordinates
 */
export async function SetToolOptions(tool: number, options: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetToolOptions', { tool, options });
}

/**
 * Retrieves the tool plugin of an object.
 * @param tool - The Tool value
 * @returns The plugin value
 */
export async function GetToolPlugin(tool: number): Promise<any> {
    const result = await callCpp<{ plugin: any }>(SUITE_NAME, 'GetToolPlugin', { tool });
    return result.plugin;
}

/**
 * Retrieves the selected tool of an object.
 * @returns The tool value
 */
export async function GetSelectedTool(): Promise<number> {
    const result = await callCpp<{ tool: number }>(SUITE_NAME, 'GetSelectedTool', {  });
    return result.tool;
}

/**
 * Sets the selected tool of an object.
 * @param tool - The Tool value
 */
export async function SetSelectedTool(tool: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetSelectedTool', { tool });
}

/**
 * Counts the number of tools objects.
 * @returns The count value
 */
export async function CountTools(): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountTools', {  });
    return result.count;
}

/**
 * Retrieves the nth tool of an object.
 * @param n - The N value
 * @returns The tool value
 */
export async function GetNthTool(n: number): Promise<number> {
    const result = await callCpp<{ tool: number }>(SUITE_NAME, 'GetNthTool', { n });
    return result.tool;
}

/**
 * Retrieves the tool handle from number of an object.
 * @param toolNum - The Tool Num value
 * @returns The tool value
 */
export async function GetToolHandleFromNumber(toolNum: number): Promise<number> {
    const result = await callCpp<{ tool: number }>(SUITE_NAME, 'GetToolHandleFromNumber', { toolNum });
    return result.tool;
}

/**
 * Retrieves the tool number from name of an object.
 * @param name - The name string
 * @returns The tool num value
 */
export async function GetToolNumberFromName(name: string): Promise<number> {
    const result = await callCpp<{ toolNum: number }>(SUITE_NAME, 'GetToolNumberFromName', { name });
    return result.toolNum;
}

/**
 * Retrieves the tool number from handle of an object.
 * @param tool - The Tool value
 * @returns The tool num value
 */
export async function GetToolNumberFromHandle(tool: number): Promise<number> {
    const result = await callCpp<{ toolNum: number }>(SUITE_NAME, 'GetToolNumberFromHandle', { tool });
    return result.toolNum;
}

/**
 * Retrieves the tool name from number of an object.
 * @param toolNum - The Tool Num value
 * @returns The name string
 */
export async function GetToolNameFromNumber(toolNum: number): Promise<string> {
    const result = await callCpp<{ name: string }>(SUITE_NAME, 'GetToolNameFromNumber', { toolNum });
    return result.name;
}

/**
 * Retrieves the tool title of an object.
 * @param tool - The Tool value
 * @returns The title value
 */
export async function GetToolTitle(tool: number): Promise<string> {
    const result = await callCpp<{ title: string }>(SUITE_NAME, 'GetToolTitle', { tool });
    return result.title;
}

/**
 * Sets the tool title of an object.
 * @param tool - The Tool value
 * @param title - The Title value
 */
export async function SetToolTitle(tool: number, title: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetToolTitle', { tool, title });
}

/**
 * Retrieves the tooltip of an object.
 * @param tool - The Tool value
 * @returns The tooltip value
 */
export async function GetTooltip(tool: number): Promise<string> {
    const result = await callCpp<{ tooltip: string }>(SUITE_NAME, 'GetTooltip', { tool });
    return result.tooltip;
}

/**
 * Sets the tooltip of an object.
 * @param tool - The Tool value
 * @param tooltip - The Tooltip value
 */
export async function SetTooltip(tool: number, tooltip: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetTooltip', { tool, tooltip });
}

/**
 * Performs the system has pressure operation.
 * @returns True if the condition is met, false otherwise
 */
export async function SystemHasPressure(): Promise<boolean> {
    const result = await callCpp<{ hasPressure: boolean }>(SUITE_NAME, 'SystemHasPressure', {  });
    return result.hasPressure;
}

/**
 * Retrieves the tool null event interval of an object.
 * @param tool - The Tool value
 * @returns The out time value
 */
export async function GetToolNullEventInterval(tool: number): Promise<number> {
    const result = await callCpp<{ outTime: number }>(SUITE_NAME, 'GetToolNullEventInterval', { tool });
    return result.outTime;
}

/**
 * Sets the tool null event interval of an object.
 * @param tool - The Tool value
 * @param inTime - The In Time value
 */
export async function SetToolNullEventInterval(tool: number, inTime: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetToolNullEventInterval', { tool, inTime });
}

/**
 * Sets the soft selected tool of an object.
 * @param tool - The Tool value
 */
export async function SetSoftSelectedTool(tool: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetSoftSelectedTool', { tool });
}

/**
 * Checks if is soft mode selection.
 * @returns True if the condition is met, false otherwise
 */
export async function IsSoftModeSelection(): Promise<boolean> {
    const result = await callCpp<{ isSoftMode: boolean }>(SUITE_NAME, 'IsSoftModeSelection', {  });
    return result.isSoftMode;
}

/**
 * Sets the alternate selection tool name of an object.
 * @param tool - The Tool value
 * @param alternateTool - The Alternate Tool value
 */
export async function SetAlternateSelectionToolName(tool: number, alternateTool: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetAlternateSelectionToolName', { tool, alternateTool });
}

/**
 * Retrieves the current tool number of an object.
 * @returns The tool num value
 */
export async function GetCurrentToolNumber(): Promise<number> {
    const result = await callCpp<{ toolNum: number }>(SUITE_NAME, 'GetCurrentToolNumber', {  });
    return result.toolNum;
}

/**
 * Retrieves the current effective tool of an object.
 * @returns The tool value
 */
export async function GetCurrentEffectiveTool(): Promise<number> {
    const result = await callCpp<{ tool: number }>(SUITE_NAME, 'GetCurrentEffectiveTool', {  });
    return result.tool;
}

/**
 * Retrieves the current effective tool number of an object.
 * @returns The tool num value
 */
export async function GetCurrentEffectiveToolNumber(): Promise<number> {
    const result = await callCpp<{ toolNum: number }>(SUITE_NAME, 'GetCurrentEffectiveToolNumber', {  });
    return result.toolNum;
}

/**
 * Sets the selected tool by name of an object.
 * @param name - The name string
 */
export async function SetSelectedToolByName(name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetSelectedToolByName', { name });
}

/**
 * Sets the selected tool by number of an object.
 * @param toolNum - The Tool Num value
 */
export async function SetSelectedToolByNumber(toolNum: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetSelectedToolByNumber', { toolNum });
}

/**
 * Retrieves the tablet hardware capabilities of an object.
 * @returns The hardware capabilities value
 */
export async function GetTabletHardwareCapabilities(): Promise<number> {
    const result = await callCpp<{ hardwareCapabilities: number }>(SUITE_NAME, 'GetTabletHardwareCapabilities', {  });
    return result.hardwareCapabilities;
}

/**
 * Sets the tool icons of an object.
 * @param tool - The Tool value
 * @param normalIconResourceName - The name string
 * @param darkNormalIconResourceName - The name string
 */
export async function SetToolIcons(tool: number, normalIconResourceName: string, darkNormalIconResourceName: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetToolIcons', { tool, normalIconResourceName, darkNormalIconResourceName });
}

/**
 * Sets the document ink params of an object.
 * @param tool - The Tool value
 * @param inDocInkParams - Handle to the document
 */
export async function SetDocumentInkParams(tool: number, inDocInkParams: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentInkParams', { tool, inDocInkParams });
}

/**
 * Retrieves the tool options from number of an object.
 * @param toolNum - The Tool Num value
 * @returns The options value
 */
export async function GetToolOptionsFromNumber(toolNum: number): Promise<number> {
    const result = await callCpp<{ options: number }>(SUITE_NAME, 'GetToolOptionsFromNumber', { toolNum });
    return result.options;
}

/**
 * Retrieves the tool options from name of an object.
 * @param toolName - The name string
 * @returns The options value
 */
export async function GetToolOptionsFromName(toolName: string): Promise<number> {
    const result = await callCpp<{ options: number }>(SUITE_NAME, 'GetToolOptionsFromName', { toolName });
    return result.options;
}
