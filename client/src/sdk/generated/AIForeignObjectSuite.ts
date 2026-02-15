/**
 * AIForeignObjectSuite client functions
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

const SUITE_NAME = 'AIForeignObjectSuite';

/**
 * Creates a new .
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @param copyObjects - The Copy Objects value
 * @returns Handle to the art object
 */
export async function New(paintOrder: number, prep: number, copyObjects: boolean): Promise<number> {
    const result = await callCpp<{ newArt: number }>(SUITE_NAME, 'New', { paintOrder, prep, copyObjects });
    return result.newArt;
}

/**
 * Checks if is.
 * @param art - Handle to the art object
 */
export async function Is(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Is', { art });
}

/**
 * Retrieves the bounds of an object.
 * @param art - Handle to the art object
 * @returns The bounding rectangle
 */
export async function GetBounds(art: number): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetBounds', { art });
    return result.bounds;
}

/**
 * Sets the bounds of an object.
 * @param art - Handle to the art object
 * @returns The bounding rectangle
 */
export async function SetBounds(art: number): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'SetBounds', { art });
    return result.bounds;
}

/**
 * Retrieves the matrix of an object.
 * @param art - Handle to the art object
 * @returns The matrix value
 */
export async function GetMatrix(art: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'GetMatrix', { art });
    return result.matrix;
}

/**
 * Sets the matrix of an object.
 * @param art - Handle to the art object
 * @returns The matrix value
 */
export async function SetMatrix(art: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'SetMatrix', { art });
    return result.matrix;
}

/**
 * Retrieves the display port of an object.
 * @param art - Handle to the art object
 * @returns The port value
 */
export async function GetDisplayPort(art: number): Promise<any> {
    const result = await callCpp<{ port: any }>(SUITE_NAME, 'GetDisplayPort', { art });
    return result.port;
}

/**
 * Performs the install resources operation.
 * @param art - Handle to the art object
 */
export async function InstallResources(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'InstallResources', { art });
}

/**
 * Duplicates the object.
 * @param art - Handle to the art object
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function DuplicateForConversion(art: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ newArt: number }>(SUITE_NAME, 'DuplicateForConversion', { art, paintOrder, prep });
    return result.newArt;
}

/**
 * Sets the raster info of an object.
 * @param art - Handle to the art object
 * @param info - The Info value
 * @param transferReference - The Transfer Reference value
 */
export async function SetRasterInfo(art: number, info: any, transferReference: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetRasterInfo', { art, info, transferReference });
}

/**
 * Retrieves the raster info of an object.
 * @param art - Handle to the art object
 * @param info - The Info value
 */
export async function GetRasterInfo(art: number, info: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetRasterInfo', { art, info });
}
