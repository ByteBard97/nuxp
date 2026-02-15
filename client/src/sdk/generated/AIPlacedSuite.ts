/**
 * AIPlacedSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

// Struct type definitions

/**
 * Transformation matrix structure
 */
export interface AIRealMatrix {
    a: number;
    b: number;
    c: number;
    d: number;
    tx: number;
    ty: number;
}

/**
 * Point structure for coordinates
 */
export interface AIRealPoint {
    h: number;
    v: number;
}

/**
 * Rectangle structure for bounds and regions
 */
export interface AIRealRect {
    left: number;
    top: number;
    right: number;
    bottom: number;
}

const SUITE_NAME = 'AIPlacedSuite';

/**
 * Retrieves the placed file specification of an object.
 * @param placed - The Placed value
 * @param file - The File value
 */
export async function GetPlacedFileSpecification(placed: number, file: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetPlacedFileSpecification', { placed, file });
}

/**
 * Sets the placed file specification of an object.
 * @param placed - The Placed value
 * @param file - The File value
 */
export async function SetPlacedFileSpecification(placed: number, file: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetPlacedFileSpecification', { placed, file });
}

/**
 * Retrieves the placed matrix of an object.
 * @param placed - The Placed value
 * @returns The matrix value
 */
export async function GetPlacedMatrix(placed: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'GetPlacedMatrix', { placed });
    return result.matrix;
}

/**
 * Sets the placed matrix of an object.
 * @param placed - The Placed value
 * @returns The matrix value
 */
export async function SetPlacedMatrix(placed: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'SetPlacedMatrix', { placed });
    return result.matrix;
}

/**
 * Retrieves the placed bounding box of an object.
 * @param placed - The Placed value
 * @returns The bbox value
 */
export async function GetPlacedBoundingBox(placed: number): Promise<AIRealRect> {
    const result = await callCpp<{ bbox: AIRealRect }>(SUITE_NAME, 'GetPlacedBoundingBox', { placed });
    return result.bbox;
}

/**
 * Sets the placed object of an object.
 * @param placed - The Placed value
 * @returns The group value
 */
export async function SetPlacedObject(placed: number): Promise<number> {
    const result = await callCpp<{ group: number }>(SUITE_NAME, 'SetPlacedObject', { placed });
    return result.group;
}

/**
 * Counts the number of placed custom colors objects.
 * @param art - Handle to the art object
 * @returns The count value
 */
export async function CountPlacedCustomColors(art: number): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountPlacedCustomColors', { art });
    return result.count;
}

/**
 * Retrieves the nth placed custom color name of an object.
 * @param art - Handle to the art object
 * @param num - The Num value
 * @param name - The name string
 */
export async function GetNthPlacedCustomColorName(art: number, num: number, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetNthPlacedCustomColorName', { art, num, name });
}

/**
 * Performs the make placed object native operation.
 * @param placed - The Placed value
 * @param askForParam - The Ask For Param value
 * @returns The native value
 */
export async function MakePlacedObjectNative(placed: number, askForParam: boolean): Promise<number> {
    const result = await callCpp<{ native: number }>(SUITE_NAME, 'MakePlacedObjectNative', { placed, askForParam });
    return result.native;
}

/**
 * Retrieves the placed type of an object.
 * @param placed - The Placed value
 * @returns The type value
 */
export async function GetPlacedType(placed: number): Promise<number> {
    const result = await callCpp<{ pPlacedType: number }>(SUITE_NAME, 'GetPlacedType', { placed });
    return result.pPlacedType;
}

/**
 * Retrieves the placed child of an object.
 * @param placed - The Placed value
 * @returns The group value
 */
export async function GetPlacedChild(placed: number): Promise<number> {
    const result = await callCpp<{ group: number }>(SUITE_NAME, 'GetPlacedChild', { placed });
    return result.group;
}

/**
 * Performs the exec place request operation.
 * @param placeRequestData - The Place Request Data value
 */
export async function ExecPlaceRequest(placeRequestData: any): Promise<void> {
    await callCpp(SUITE_NAME, 'ExecPlaceRequest', { placeRequestData });
}

/**
 * Retrieves the placed file info from art of an object.
 * @param placed - The Placed value
 * @returns The sp file info value
 */
export async function GetPlacedFileInfoFromArt(placed: number): Promise<any> {
    const result = await callCpp<{ spFileInfo: any }>(SUITE_NAME, 'GetPlacedFileInfoFromArt', { placed });
    return result.spFileInfo;
}

/**
 * Retrieves the placed file info from file of an object.
 * @param placed - The Placed value
 * @returns The sp file info value
 */
export async function GetPlacedFileInfoFromFile(placed: number): Promise<any> {
    const result = await callCpp<{ spFileInfo: any }>(SUITE_NAME, 'GetPlacedFileInfoFromFile', { placed });
    return result.spFileInfo;
}

/**
 * Retrieves the placed file path from art of an object.
 * @param placed - The Placed value
 * @param path - The Path value
 */
export async function GetPlacedFilePathFromArt(placed: number, path: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetPlacedFilePathFromArt', { placed, path });
}

/**
 * Performs the concat placed matrix operation.
 * @param placed - The Placed value
 * @returns The concat value
 */
export async function ConcatPlacedMatrix(placed: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ concat: AIRealMatrix }>(SUITE_NAME, 'ConcatPlacedMatrix', { placed });
    return result.concat;
}

/**
 * Sets the place options of an object.
 * @param placed - The Placed value
 * @param method - The Method value
 * @param alignment - The Alignment value
 * @param clip - The Clip value
 */
export async function SetPlaceOptions(placed: number, method: any, alignment: any, clip: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetPlaceOptions', { placed, method, alignment, clip });
}

/**
 * Retrieves the place options of an object.
 * @param placed - The Placed value
 * @returns An object containing: method, alignment, clip
 */
export async function GetPlaceOptions(placed: number): Promise<{ method: any; alignment: any; clip: boolean }> {
    const result = await callCpp<{ method: any; alignment: any; clip: boolean }>(SUITE_NAME, 'GetPlaceOptions', { placed });
    return result;
}

/**
 * Retrieves the placed dimensions of an object.
 * @param placed - The Placed value
 * @returns An object containing: size, viewBounds, viewMatrix, imageBounds, imageMatrix
 */
export async function GetPlacedDimensions(placed: number): Promise<{ size: AIRealPoint; viewBounds: AIRealRect; viewMatrix: AIRealMatrix; imageBounds: AIRealRect; imageMatrix: AIRealMatrix }> {
    const result = await callCpp<{ size: AIRealPoint; viewBounds: AIRealRect; viewMatrix: AIRealMatrix; imageBounds: AIRealRect; imageMatrix: AIRealMatrix }>(SUITE_NAME, 'GetPlacedDimensions', { placed });
    return result;
}

/**
 * Sets the placed bounding box of an object.
 * @param art - Handle to the art object
 * @returns The bbox value
 */
export async function SetPlacedBoundingBox(art: number): Promise<AIRealRect> {
    const result = await callCpp<{ bbox: AIRealRect }>(SUITE_NAME, 'SetPlacedBoundingBox', { art });
    return result.bbox;
}

/**
 * Retrieves the raster info of an object.
 * @param art - Handle to the art object
 * @returns An object containing: info, israster
 */
export async function GetRasterInfo(art: number): Promise<{ info: any; israster: boolean }> {
    const result = await callCpp<{ info: any; israster: boolean }>(SUITE_NAME, 'GetRasterInfo', { art });
    return result;
}

/**
 * Retrieves the process plates info of an object.
 * @param art - Handle to the art object
 * @returns The plate info value
 */
export async function GetProcessPlatesInfo(art: number): Promise<any> {
    const result = await callCpp<{ plateInfo: any }>(SUITE_NAME, 'GetProcessPlatesInfo', { art });
    return result.plateInfo;
}

/**
 * Checks if is placed file damaged.
 * @param art - Handle to the art object
 */
export async function IsPlacedFileDamaged(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'IsPlacedFileDamaged', { art });
}

/**
 * Checks if is proxy used for rendering.
 * @param art - Handle to the art object
 */
export async function IsProxyUsedForRendering(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'IsProxyUsedForRendering', { art });
}
