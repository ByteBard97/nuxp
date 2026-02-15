/**
 * AIArtSetSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIArtSetSuite';

/**
 * Creates a new art set.
 * @returns The art set value
 */
export async function NewArtSet(): Promise<any> {
    const result = await callCpp<{ artSet: any }>(SUITE_NAME, 'NewArtSet', {  });
    return result.artSet;
}

/**
 * Removes the art set.
 * @returns The art set value
 */
export async function DisposeArtSet(): Promise<any> {
    const result = await callCpp<{ artSet: any }>(SUITE_NAME, 'DisposeArtSet', {  });
    return result.artSet;
}

/**
 * Counts the number of art set objects.
 * @param artSet - The Art Set value
 * @returns The count value
 */
export async function CountArtSet(artSet: any): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountArtSet', { artSet });
    return result.count;
}

/**
 * Performs the index art set operation.
 * @param artSet - The Art Set value
 * @param index - The index value
 * @returns Handle to the art object
 */
export async function IndexArtSet(artSet: any, index: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'IndexArtSet', { artSet, index });
    return result.art;
}

/**
 * Performs the array art set operation.
 * @param artSet - The Art Set value
 * @param count - The count value
 * @returns Handle to the art object
 */
export async function ArrayArtSet(artSet: any, count: number): Promise<number> {
    const result = await callCpp<{ artArray: number }>(SUITE_NAME, 'ArrayArtSet', { artSet, count });
    return result.artArray;
}

/**
 * Selects the object.
 * @param artSet - The Art Set value
 */
export async function SelectedArtSet(artSet: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SelectedArtSet', { artSet });
}

/**
 * Performs the matching art set operation.
 * @param numSpecs - The Num Specs value
 * @param artSet - The Art Set value
 * @returns The specs value
 */
export async function MatchingArtSet(numSpecs: number, artSet: any): Promise<any> {
    const result = await callCpp<{ specs: any }>(SUITE_NAME, 'MatchingArtSet', { numSpecs, artSet });
    return result.specs;
}

/**
 * Performs the layer art set operation.
 * @param layer - Handle to the layer
 * @param artSet - The Art Set value
 */
export async function LayerArtSet(layer: number, artSet: any): Promise<void> {
    await callCpp(SUITE_NAME, 'LayerArtSet', { layer, artSet });
}

/**
 * Performs the not art set operation.
 * @param src - The Src value
 * @param dst - The Dst value
 */
export async function NotArtSet(src: any, dst: any): Promise<void> {
    await callCpp(SUITE_NAME, 'NotArtSet', { src, dst });
}

/**
 * Performs the union art set operation.
 * @param src0 - The Src0 value
 * @param src1 - The Src1 value
 * @param dst - The Dst value
 */
export async function UnionArtSet(src0: any, src1: any, dst: any): Promise<void> {
    await callCpp(SUITE_NAME, 'UnionArtSet', { src0, src1, dst });
}

/**
 * Performs the intersect art set operation.
 * @param src0 - The Src0 value
 * @param src1 - The Src1 value
 * @param dst - The Dst value
 */
export async function IntersectArtSet(src0: any, src1: any, dst: any): Promise<void> {
    await callCpp(SUITE_NAME, 'IntersectArtSet', { src0, src1, dst });
}

/**
 * Performs the next in art set operation.
 * @param artSet - The Art Set value
 * @param prevArt - Handle to the art object
 * @returns Handle to the art object
 */
export async function NextInArtSet(artSet: any, prevArt: number): Promise<number> {
    const result = await callCpp<{ nextArt: number }>(SUITE_NAME, 'NextInArtSet', { artSet, prevArt });
    return result.nextArt;
}

/**
 * Adds a new item.
 * @param artSet - The Art Set value
 * @param art - Handle to the art object
 */
export async function AddArtToArtSet(artSet: any, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'AddArtToArtSet', { artSet, art });
}

/**
 * Removes the art from art set.
 * @param artSet - The Art Set value
 * @param art - Handle to the art object
 */
export async function RemoveArtFromArtSet(artSet: any, art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'RemoveArtFromArtSet', { artSet, art });
}

/**
 * Performs the replace art in art set operation.
 * @param artSet - The Art Set value
 * @param oldArt - Handle to the art object
 * @param newArt - Handle to the art object
 */
export async function ReplaceArtInArtSet(artSet: any, oldArt: number, newArt: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ReplaceArtInArtSet', { artSet, oldArt, newArt });
}

/**
 * Performs the clear art set operation.
 * @param artSet - The Art Set value
 */
export async function ClearArtSet(artSet: any): Promise<void> {
    await callCpp(SUITE_NAME, 'ClearArtSet', { artSet });
}
