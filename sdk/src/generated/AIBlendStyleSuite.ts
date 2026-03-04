/**
 * AIBlendStyleSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AIBlendStyleSuite';

/**
 * Retrieves the blending mode of an object.
 * @param art - Handle to the art object
 * @returns The AIBlendingMode value
 */
export async function GetBlendingMode(art: number): Promise<number> {
    const result = await callCpp<{ result: number }>(SUITE_NAME, 'GetBlendingMode', { art });
    return result.result;
}

/**
 * Retrieves the opacity of an object.
 * @param art - Handle to the art object
 * @returns The AIReal value
 */
export async function GetOpacity(art: number): Promise<number> {
    const result = await callCpp<{ result: number }>(SUITE_NAME, 'GetOpacity', { art });
    return result.result;
}

/**
 * Sets the opacity of an object.
 * @param art - Handle to the art object
 * @param opacity - The Opacity value
 */
export async function SetOpacity(art: number, opacity: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetOpacity', { art, opacity });
}

/**
 * Retrieves the isolated of an object.
 * @param art - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function GetIsolated(art: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetIsolated', { art });
    return result.result;
}

/**
 * Sets the isolated of an object.
 * @param art - Handle to the art object
 * @param isolated - The Isolated value
 */
export async function SetIsolated(art: number, isolated: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetIsolated', { art, isolated });
}

/**
 * Retrieves the knockout of an object.
 * @param art - Handle to the art object
 * @returns The AIKnockout value
 */
export async function GetKnockout(art: number): Promise<number> {
    const result = await callCpp<{ result: number }>(SUITE_NAME, 'GetKnockout', { art });
    return result.result;
}

/**
 * Retrieves the inherited knockout of an object.
 * @param art - Handle to the art object
 * @returns The AIKnockout value
 */
export async function GetInheritedKnockout(art: number): Promise<number> {
    const result = await callCpp<{ result: number }>(SUITE_NAME, 'GetInheritedKnockout', { art });
    return result.result;
}

/**
 * Retrieves the alpha is shape of an object.
 * @param art - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function GetAlphaIsShape(art: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetAlphaIsShape', { art });
    return result.result;
}

/**
 * Sets the alpha is shape of an object.
 * @param art - Handle to the art object
 * @param alphaIsShape - The Alpha Is Shape value
 */
export async function SetAlphaIsShape(art: number, alphaIsShape: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetAlphaIsShape', { art, alphaIsShape });
}

/**
 * Copies the object.
 * @param source - The Source value
 * @param destination - The Destination value
 */
export async function Copy(source: number, destination: number): Promise<void> {
    await callCpp(SUITE_NAME, 'Copy', { source, destination });
}

/**
 * Retrieves the art attrs of an object.
 * @param art - Handle to the art object
 * @param attrs - The Attrs value
 */
export async function GetArtAttrs(art: number, attrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'GetArtAttrs', { art, attrs });
}

/**
 * Sets the art attrs of an object.
 * @param art - Handle to the art object
 * @param attrs - The Attrs value
 */
export async function SetArtAttrs(art: number, attrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtAttrs', { art, attrs });
}

/**
 * Retrieves the style attrs of an object.
 * @param style - The Style value
 * @param attrs - The Attrs value
 */
export async function GetStyleAttrs(style: number, attrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'GetStyleAttrs', { style, attrs });
}

/**
 * Sets the style attrs of an object.
 * @param style - The Style value
 * @param attrs - The Attrs value
 * @returns The new style value
 */
export async function SetStyleAttrs(style: number, attrs: number): Promise<number> {
    const result = await callCpp<{ newStyle: number }>(SUITE_NAME, 'SetStyleAttrs', { style, attrs });
    return result.newStyle;
}

/**
 * Retrieves the current transparency of an object.
 * @param styleAttrs - The Style Attrs value
 * @param fillAttrs - The Fill Attrs value
 * @param strokeAttrs - The Stroke Attrs value
 */
export async function GetCurrentTransparency(styleAttrs: number, fillAttrs: number, strokeAttrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'GetCurrentTransparency', { styleAttrs, fillAttrs, strokeAttrs });
}

/**
 * Sets the current transparency of an object.
 * @param styleAttrs - The Style Attrs value
 * @param fillAttrs - The Fill Attrs value
 * @param strokeAttrs - The Stroke Attrs value
 */
export async function SetCurrentTransparency(styleAttrs: number, fillAttrs: number, strokeAttrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetCurrentTransparency', { styleAttrs, fillAttrs, strokeAttrs });
}

/**
 * Retrieves the focal fill attrs of an object.
 * @param artStyle - Handle to the art object
 * @param attrs - The Attrs value
 */
export async function GetFocalFillAttrs(artStyle: number, attrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'GetFocalFillAttrs', { artStyle, attrs });
}

/**
 * Retrieves the focal stroke attrs of an object.
 * @param artStyle - Handle to the art object
 * @param attrs - The Attrs value
 */
export async function GetFocalStrokeAttrs(artStyle: number, attrs: number): Promise<void> {
    await callCpp(SUITE_NAME, 'GetFocalStrokeAttrs', { artStyle, attrs });
}

/**
 * Sets the focal fill attrs of an object.
 * @param artStyle - Handle to the art object
 * @param attrs - The Attrs value
 * @returns The new style value
 */
export async function SetFocalFillAttrs(artStyle: number, attrs: number): Promise<number> {
    const result = await callCpp<{ newStyle: number }>(SUITE_NAME, 'SetFocalFillAttrs', { artStyle, attrs });
    return result.newStyle;
}

/**
 * Sets the focal stroke attrs of an object.
 * @param artStyle - Handle to the art object
 * @param attrs - The Attrs value
 * @returns The new style value
 */
export async function SetFocalStrokeAttrs(artStyle: number, attrs: number): Promise<number> {
    const result = await callCpp<{ newStyle: number }>(SUITE_NAME, 'SetFocalStrokeAttrs', { artStyle, attrs });
    return result.newStyle;
}

/**
 * Performs the contains non isolated blending operation.
 * @param object - The Object value
 * @returns True if the condition is met, false otherwise
 */
export async function ContainsNonIsolatedBlending(object: number): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'ContainsNonIsolatedBlending', { object });
    return result.result;
}

/**
 * Retrieves the document isolated of an object.
 * @returns True if the condition is met, false otherwise
 */
export async function GetDocumentIsolated(): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetDocumentIsolated', {  });
    return result.result;
}

/**
 * Sets the document isolated of an object.
 * @param isolated - The Isolated value
 */
export async function SetDocumentIsolated(isolated: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentIsolated', { isolated });
}

/**
 * Retrieves the document knockout of an object.
 * @returns True if the condition is met, false otherwise
 */
export async function GetDocumentKnockout(): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'GetDocumentKnockout', {  });
    return result.result;
}

/**
 * Sets the document knockout of an object.
 * @param knockout - The Knockout value
 */
export async function SetDocumentKnockout(knockout: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentKnockout', { knockout });
}
