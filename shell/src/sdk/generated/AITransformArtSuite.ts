/**
 * AITransformArtSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';
import { AIRealMatrix } from './types';

const SUITE_NAME = 'AITransformArtSuite';

/**
 * Applies a transformation to the object.
 * @param art - Handle to the art object
 * @param lineScale - The Line Scale value
 * @param flags - The Flags value
 * @returns The matrix value
 */
export async function TransformArt(art: number, lineScale: number, flags: number): Promise<AIRealMatrix> {
    const result = await callCpp<{ matrix: AIRealMatrix }>(SUITE_NAME, 'TransformArt', { art, lineScale, flags });
    return result.matrix;
}
