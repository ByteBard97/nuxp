/**
 * AIRasterSuite client functions
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
 * Rectangle structure for bounds and regions
 */
export interface AIRealRect {
    left: number;
    top: number;
    right: number;
    bottom: number;
}

const SUITE_NAME = 'AIRasterSuite';

/**
 * Retrieves the raster info of an object.
 * @param raster - The Raster value
 * @returns The info value
 */
export async function GetRasterInfo(raster: number): Promise<any> {
    const result = await callCpp<{ info: any }>(SUITE_NAME, 'GetRasterInfo', { raster });
    return result.info;
}

/**
 * Sets the raster info of an object.
 * @param raster - The Raster value
 * @returns The info value
 */
export async function SetRasterInfo(raster: number): Promise<any> {
    const result = await callCpp<{ info: any }>(SUITE_NAME, 'SetRasterInfo', { raster });
    return result.info;
}

/**
 * Retrieves the raster file specification of an object.
 * @param raster - The Raster value
 * @param file - The File value
 */
export async function GetRasterFileSpecification(raster: number, file: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetRasterFileSpecification', { raster, file });
}

/**
 * Sets the raster file specification of an object.
 * @param raster - The Raster value
 * @param  - The  value
 */
export async function SetRasterFileSpecification(raster: number, : any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetRasterFileSpecification', { raster,  });
}

/**
 * Retrieves the raster matrix of an object.
 * @param raster - The Raster value
 * @returns The matrix value
 */
export async function GetRasterMatrix(raster: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'GetRasterMatrix', { raster });
    return result.matrix;
}

/**
 * Sets the raster matrix of an object.
 * @param raster - The Raster value
 * @returns The matrix value
 */
export async function SetRasterMatrix(raster: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'SetRasterMatrix', { raster });
    return result.matrix;
}

/**
 * Retrieves the raster bounding box of an object.
 * @param raster - The Raster value
 * @returns The bbox value
 */
export async function GetRasterBoundingBox(raster: number): Promise<AIRealRect> {
    const result = await callCpp<{ bbox: AIRealRect }>(SUITE_NAME, 'GetRasterBoundingBox', { raster });
    return result.bbox;
}

/**
 * Sets the raster bounding box of an object.
 * @param raster - The Raster value
 * @returns The bbox value
 */
export async function SetRasterBoundingBox(raster: number): Promise<AIRealRect> {
    const result = await callCpp<{ bbox: AIRealRect }>(SUITE_NAME, 'SetRasterBoundingBox', { raster });
    return result.bbox;
}

/**
 * Retrieves the raster tile of an object.
 * @param raster - The Raster value
 * @returns An object containing: artSlice, workTile, workSlice
 */
export async function GetRasterTile(raster: number): Promise<{ artSlice: any; workTile: any; workSlice: any }> {
    const result = await callCpp<{ artSlice: any; workTile: any; workSlice: any }>(SUITE_NAME, 'GetRasterTile', { raster });
    return result;
}

/**
 * Sets the raster tile of an object.
 * @param raster - The Raster value
 * @returns An object containing: artSlice, workTile, workSlice
 */
export async function SetRasterTile(raster: number): Promise<{ artSlice: any; workTile: any; workSlice: any }> {
    const result = await callCpp<{ artSlice: any; workTile: any; workSlice: any }>(SUITE_NAME, 'SetRasterTile', { raster });
    return result;
}

/**
 * Retrieves the raster link of an object.
 * @param raster - The Raster value
 * @param link - The Link value
 */
export async function GetRasterLink(raster: number, link: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetRasterLink', { raster, link });
}

/**
 * Sets the raster link of an object.
 * @param raster - The Raster value
 * @param link - The Link value
 */
export async function SetRasterLink(raster: number, link: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetRasterLink', { raster, link });
}

/**
 * Performs the resolve raster link operation.
 * @param raster - The Raster value
 * @param flags - The Flags value
 */
export async function ResolveRasterLink(raster: number, flags: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ResolveRasterLink', { raster, flags });
}

/**
 * Retrieves the raster file info from art of an object.
 * @param raster - The Raster value
 * @returns The p s p file info value
 */
export async function GetRasterFileInfoFromArt(raster: number): Promise<any> {
    const result = await callCpp<{ pSPFileInfo: any }>(SUITE_NAME, 'GetRasterFileInfoFromArt', { raster });
    return result.pSPFileInfo;
}

/**
 * Retrieves the raster file info from file of an object.
 * @param raster - The Raster value
 * @returns The p s p file info value
 */
export async function GetRasterFileInfoFromFile(raster: number): Promise<any> {
    const result = await callCpp<{ pSPFileInfo: any }>(SUITE_NAME, 'GetRasterFileInfoFromFile', { raster });
    return result.pSPFileInfo;
}

/**
 * Retrieves the raster file path from art of an object.
 * @param raster - The Raster value
 * @param path - The Path value
 */
export async function GetRasterFilePathFromArt(raster: number, path: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetRasterFilePathFromArt', { raster, path });
}

/**
 * Counts the number of levels objects.
 * @param raster - The Raster value
 * @returns The count value
 */
export async function CountLevels(raster: number): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountLevels', { raster });
    return result.count;
}

/**
 * Retrieves the level info of an object.
 * @param raster - The Raster value
 * @param level - The Level value
 * @returns The info value
 */
export async function GetLevelInfo(raster: number, level: number): Promise<any> {
    const result = await callCpp<{ info: any }>(SUITE_NAME, 'GetLevelInfo', { raster, level });
    return result.info;
}

/**
 * Retrieves the level tile of an object.
 * @param raster - The Raster value
 * @param level - The Level value
 * @returns An object containing: artSlice, workTile, workSlice
 */
export async function GetLevelTile(raster: number, level: number): Promise<{ artSlice: any; workTile: any; workSlice: any }> {
    const result = await callCpp<{ artSlice: any; workTile: any; workSlice: any }>(SUITE_NAME, 'GetLevelTile', { raster, level });
    return result;
}

/**
 * Performs the extract outline operation.
 * @param raster - The Raster value
 * @param params - The Params value
 * @returns The consumer value
 */
export async function ExtractOutline(raster: number, params: any): Promise<any> {
    const result = await callCpp<{ consumer: any }>(SUITE_NAME, 'ExtractOutline', { raster, params });
    return result.consumer;
}

/**
 * Performs the concat raster matrix operation.
 * @param raster - The Raster value
 * @returns The concat value
 */
export async function ConcatRasterMatrix(raster: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ concat: AIRealMatrix }>(SUITE_NAME, 'ConcatRasterMatrix', { raster });
    return result.concat;
}

/**
 * Sets the color space info of an object.
 * @param raster - The Raster value
 * @param cs - The Cs value
 */
export async function SetColorSpaceInfo(raster: number, cs: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetColorSpaceInfo', { raster, cs });
}

/**
 * Retrieves the color space info of an object.
 * @param raster - The Raster value
 * @param cs - The Cs value
 */
export async function GetColorSpaceInfo(raster: number, cs: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetColorSpaceInfo', { raster, cs });
}

/**
 * Sets the overprint of an object.
 * @param raster - The Raster value
 * @param enable - The Enable value
 */
export async function SetOverprint(raster: number, enable: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetOverprint', { raster, enable });
}

/**
 * Retrieves the overprint of an object.
 * @param raster - The Raster value
 * @returns True if the condition is met, false otherwise
 */
export async function GetOverprint(raster: number): Promise<boolean> {
    const result = await callCpp<{ status: boolean }>(SUITE_NAME, 'GetOverprint', { raster });
    return result.status;
}

/**
 * Checks if is colorized.
 * @param raster - The Raster value
 * @returns An object containing: isColorized, isAdditive
 */
export async function IsColorized(raster: number): Promise<{ isColorized: boolean; isAdditive: boolean }> {
    const result = await callCpp<{ isColorized: boolean; isAdditive: boolean }>(SUITE_NAME, 'IsColorized', { raster });
    return result;
}

/**
 * Performs the clear colorization operation.
 * @param raster - The Raster value
 */
export async function ClearColorization(raster: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ClearColorization', { raster });
}

/**
 * Performs the convert colors operation.
 * @param dstCS - The Dst C S value
 * @returns The raster value
 */
export async function ConvertColors(dstCS: any): Promise<number> {
    const result = await callCpp<{ raster: number }>(SUITE_NAME, 'ConvertColors', { dstCS });
    return result.raster;
}

/**
 * Performs the invert colors operation.
 * @returns The raster value
 */
export async function InvertColors(): Promise<number> {
    const result = await callCpp<{ raster: number }>(SUITE_NAME, 'InvertColors', {  });
    return result.raster;
}

/**
 * Performs the split channels operation.
 * @param raster - The Raster value
 * @param flags - The Flags value
 * @returns An object containing: rasterArray, numRasters
 */
export async function SplitChannels(raster: number, flags: any): Promise<{ rasterArray: number; numRasters: number }> {
    const result = await callCpp<{ rasterArray: number; numRasters: number }>(SUITE_NAME, 'SplitChannels', { raster, flags });
    return result;
}

/**
 * Performs the interleave channels operation.
 * @param numRasters - The Num Rasters value
 * @param flags - The Flags value
 * @returns An object containing: rasterArray, raster
 */
export async function InterleaveChannels(numRasters: number, flags: any): Promise<{ rasterArray: number; raster: number }> {
    const result = await callCpp<{ rasterArray: number; raster: number }>(SUITE_NAME, 'InterleaveChannels', { numRasters, flags });
    return result;
}
