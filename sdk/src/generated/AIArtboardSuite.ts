/**
 * AIArtboardSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';
import { AIRealPoint, AIRealRect } from './types';

const SUITE_NAME = 'AIArtboardSuite';

/**
 * Performs the init operation.
 * @returns Handle to the art object
 */
export async function Init(): Promise<number> {
    const result = await callCpp<{ artboard: number }>(SUITE_NAME, 'Init', {  });
    return result.artboard;
}

/**
 * Performs the clone artboard operation.
 * @param newArtboard - Handle to the art object
 * @returns Handle to the art object
 */
export async function CloneArtboard(newArtboard: number): Promise<number> {
    const result = await callCpp<{ artboard: number }>(SUITE_NAME, 'CloneArtboard', { newArtboard });
    return result.artboard;
}

/**
 * Removes the object.
 * @returns The properties value
 */
export async function Dispose(): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'Dispose', {  });
    return result.properties;
}

/**
 * Retrieves the position of an object.
 * @param properties - The Properties value
 * @returns The bounding rectangle
 */
export async function GetPosition(properties: number): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetPosition', { properties });
    return result.bounds;
}

/**
 * Sets the position of an object.
 * @param bounds - The bounding rectangle
 * @returns The properties value
 */
export async function SetPosition(bounds: AIRealRect): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'SetPosition', { bounds });
    return result.properties;
}

/**
 * Retrieves the p a r of an object.
 * @param properties - The Properties value
 * @returns The par value
 */
export async function GetPAR(properties: number): Promise<number> {
    const result = await callCpp<{ par: number }>(SUITE_NAME, 'GetPAR', { properties });
    return result.par;
}

/**
 * Sets the p a r of an object.
 * @param par - The Par value
 * @returns The properties value
 */
export async function SetPAR(par: number): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'SetPAR', { par });
    return result.properties;
}

/**
 * Retrieves the name of an object.
 * @param properties - The Properties value
 * @returns The name string
 */
export async function GetName(properties: number): Promise<string> {
    const result = await callCpp<{ name: string }>(SUITE_NAME, 'GetName', { properties });
    return result.name;
}

/**
 * Sets the name of an object.
 * @param name - The name string
 * @returns The properties value
 */
export async function SetName(name: string): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'SetName', { name });
    return result.properties;
}

/**
 * Retrieves the show display mark of an object.
 * @param properties - The Properties value
 * @param type - The type value
 * @returns True if the condition is met, false otherwise
 */
export async function GetShowDisplayMark(properties: number, type: any): Promise<boolean> {
    const result = await callCpp<{ show: boolean }>(SUITE_NAME, 'GetShowDisplayMark', { properties, type });
    return result.show;
}

/**
 * Sets the show display mark of an object.
 * @param type - The type value
 * @param show - The Show value
 * @returns The properties value
 */
export async function SetShowDisplayMark(type: any, show: boolean): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'SetShowDisplayMark', { type, show });
    return result.properties;
}

/**
 * Retrieves the artboard list of an object.
 * @returns Handle to the art object
 */
export async function GetArtboardList(): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'GetArtboardList', {  });
    return result.artboardList;
}

/**
 * Performs the release artboard list operation.
 * @returns Handle to the art object
 */
export async function ReleaseArtboardList(): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'ReleaseArtboardList', {  });
    return result.artboardList;
}

/**
 * Adds a new item.
 * @returns An object containing: artboardList, newArtboard, index
 */
export async function AddNew(): Promise<{ artboardList: number; newArtboard: number; index: number }> {
    const result = await callCpp<{ artboardList: number; newArtboard: number; index: number }>(SUITE_NAME, 'AddNew', {  });
    return result;
}

/**
 * Removes the object.
 * @param index - The index value
 * @returns Handle to the art object
 */
export async function Delete(index: number): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'Delete', { index });
    return result.artboardList;
}

/**
 * Retrieves the count of an object.
 * @param artboardList - Handle to the art object
 * @returns The count value
 */
export async function GetCount(artboardList: number): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'GetCount', { artboardList });
    return result.count;
}

/**
 * Retrieves the active of an object.
 * @param artboardList - Handle to the art object
 * @returns The index value
 */
export async function GetActive(artboardList: number): Promise<number> {
    const result = await callCpp<{ index: number }>(SUITE_NAME, 'GetActive', { artboardList });
    return result.index;
}

/**
 * Sets the active of an object.
 * @param index - The index value
 * @returns Handle to the art object
 */
export async function SetActive(index: number): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'SetActive', { index });
    return result.artboardList;
}

/**
 * Updates the object state.
 * @param index - The index value
 * @param properties - The Properties value
 * @returns Handle to the art object
 */
export async function Update(index: number, properties: number): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'Update', { index, properties });
    return result.artboardList;
}

/**
 * Retrieves the artboard properties of an object.
 * @param index - The index value
 * @returns An object containing: artboardList, properties
 */
export async function GetArtboardProperties(index: number): Promise<{ artboardList: number; properties: number }> {
    const result = await callCpp<{ artboardList: number; properties: number }>(SUITE_NAME, 'GetArtboardProperties', { index });
    return result;
}

/**
 * Retrieves the ruler origin of an object.
 * @param properties - The Properties value
 * @returns The ruler origin value
 */
export async function GetRulerOrigin(properties: number): Promise<AIRealPoint> {
    const result = await callCpp<{ rulerOrigin: AIRealPoint }>(SUITE_NAME, 'GetRulerOrigin', { properties });
    return result.rulerOrigin;
}

/**
 * Sets the ruler origin of an object.
 * @param rulerOrigin - The Ruler Origin value
 * @returns The properties value
 */
export async function SetRulerOrigin(rulerOrigin: AIRealPoint): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'SetRulerOrigin', { rulerOrigin });
    return result.properties;
}

/**
 * Adds a new item.
 * @returns An object containing: artboardList, artboard, index
 */
export async function Insert(): Promise<{ artboardList: number; artboard: number; index: number }> {
    const result = await callCpp<{ artboardList: number; artboard: number; index: number }>(SUITE_NAME, 'Insert', {  });
    return result;
}

/**
 * Checks if is default name.
 * @param properties - The Properties value
 * @returns True if the condition is met, false otherwise
 */
export async function IsDefaultName(properties: number): Promise<boolean> {
    const result = await callCpp<{ isDefault: boolean }>(SUITE_NAME, 'IsDefaultName', { properties });
    return result.isDefault;
}

/**
 * Sets the is default name of an object.
 * @param isDefault - The Is Default value
 * @returns The properties value
 */
export async function SetIsDefaultName(isDefault: boolean): Promise<number> {
    const result = await callCpp<{ properties: number }>(SUITE_NAME, 'SetIsDefaultName', { isDefault });
    return result.properties;
}

/**
 * Checks if is selected.
 * @param properties - The Properties value
 * @returns True if the condition is met, false otherwise
 */
export async function IsSelected(properties: number): Promise<boolean> {
    const result = await callCpp<{ isSelected: boolean }>(SUITE_NAME, 'IsSelected', { properties });
    return result.isSelected;
}

/**
 * Selects the object.
 * @param artboardID - Handle to the art object
 * @param exclusively - The Exclusively value
 * @returns Handle to the art object
 */
export async function SelectArtboard(artboardID: number, exclusively: boolean): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'SelectArtboard', { artboardID, exclusively });
    return result.artboardList;
}

/**
 * Selects the object.
 * @param artboardIDs - The Artboard I Ds value
 * @param exclusively - The Exclusively value
 * @returns Handle to the art object
 */
export async function SelectArtboards(artboardIDs: any, exclusively: boolean): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'SelectArtboards', { artboardIDs, exclusively });
    return result.artboardList;
}

/**
 * Selects the object.
 * @returns Handle to the art object
 */
export async function SelectAllArtboards(): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'SelectAllArtboards', {  });
    return result.artboardList;
}

/**
 * Removes the artboards.
 * @param artboardIDs - The Artboard I Ds value
 * @returns Handle to the art object
 */
export async function DeleteArtboards(artboardIDs: any): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'DeleteArtboards', { artboardIDs });
    return result.artboardList;
}

/**
 * Deselects the object.
 * @param artboardID - Handle to the art object
 * @returns Handle to the art object
 */
export async function DeselectArtboard(artboardID: number): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'DeselectArtboard', { artboardID });
    return result.artboardList;
}

/**
 * Deselects the object.
 * @returns Handle to the art object
 */
export async function DeselectAllArtboards(): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'DeselectAllArtboards', {  });
    return result.artboardList;
}

/**
 * Performs the are any artboards overlapping operation.
 * @returns An object containing: artboardList, isOverlapping
 */
export async function AreAnyArtboardsOverlapping(): Promise<{ artboardList: number; isOverlapping: boolean }> {
    const result = await callCpp<{ artboardList: number; isOverlapping: boolean }>(SUITE_NAME, 'AreAnyArtboardsOverlapping', {  });
    return result;
}

/**
 * Retrieves the u u i d of an object.
 * @param properties - The Properties value
 * @returns The uuid value
 */
export async function GetUUID(properties: number): Promise<any> {
    const result = await callCpp<{ uuid: any }>(SUITE_NAME, 'GetUUID', { properties });
    return result.uuid;
}

/**
 * Retrieves the u u i d as string of an object.
 * @param properties - The Properties value
 * @returns The uuid value
 */
export async function GetUUIDAsString(properties: number): Promise<string> {
    const result = await callCpp<{ uuid: string }>(SUITE_NAME, 'GetUUIDAsString', { properties });
    return result.uuid;
}

/**
 * Adds a new item.
 * @returns An object containing: artboardList, artboard, index
 */
export async function InsertUsingArtboardPropertiesUUID(): Promise<{ artboardList: number; artboard: number; index: number }> {
    const result = await callCpp<{ artboardList: number; artboard: number; index: number }>(SUITE_NAME, 'InsertUsingArtboardPropertiesUUID', {  });
    return result;
}

/**
 * Retrieves the locked of an object.
 * @param artboardList - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function GetLocked(artboardList: number): Promise<boolean> {
    const result = await callCpp<{ isLocked: boolean }>(SUITE_NAME, 'GetLocked', { artboardList });
    return result.isLocked;
}

/**
 * Sets the locked of an object.
 * @param isLocked - The lock state
 * @returns Handle to the art object
 */
export async function SetLocked(isLocked: boolean): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'SetLocked', { isLocked });
    return result.artboardList;
}

/**
 * Retrieves the hide of an object.
 * @param artboardList - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function GetHide(artboardList: number): Promise<boolean> {
    const result = await callCpp<{ isHidden: boolean }>(SUITE_NAME, 'GetHide', { artboardList });
    return result.isHidden;
}

/**
 * Sets the hide of an object.
 * @param isHidden - The Is Hidden value
 * @returns Handle to the art object
 */
export async function SetHide(isHidden: boolean): Promise<number> {
    const result = await callCpp<{ artboardList: number }>(SUITE_NAME, 'SetHide', { isHidden });
    return result.artboardList;
}
