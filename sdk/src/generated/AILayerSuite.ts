/**
 * AILayerSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';

const SUITE_NAME = 'AILayerSuite';

/**
 * Counts the number of layers objects.
 * @returns The count value
 */
export async function CountLayers(): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountLayers', {  });
    return result.count;
}

/**
 * Retrieves the nth layer of an object.
 * @param n - The N value
 * @returns Handle to the layer
 */
export async function GetNthLayer(n: number): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'GetNthLayer', { n });
    return result.layer;
}

/**
 * Retrieves the current layer of an object.
 * @returns Handle to the layer
 */
export async function GetCurrentLayer(): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'GetCurrentLayer', {  });
    return result.layer;
}

/**
 * Sets the current layer of an object.
 * @param layer - Handle to the layer
 */
export async function SetCurrentLayer(layer: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetCurrentLayer', { layer });
}

/**
 * Retrieves the first layer of an object.
 * @returns The first value
 */
export async function GetFirstLayer(): Promise<number> {
    const result = await callCpp<{ first: number }>(SUITE_NAME, 'GetFirstLayer', {  });
    return result.first;
}

/**
 * Retrieves the next layer of an object.
 * @param prev - The Prev value
 * @returns The next value
 */
export async function GetNextLayer(prev: number): Promise<number> {
    const result = await callCpp<{ next: number }>(SUITE_NAME, 'GetNextLayer', { prev });
    return result.next;
}

/**
 * Adds a new item.
 * @param layer - Handle to the layer
 * @param paintOrder - The order position
 * @returns Handle to the layer
 */
export async function InsertLayer(layer: number, paintOrder: number): Promise<number> {
    const result = await callCpp<{ newLayer: number }>(SUITE_NAME, 'InsertLayer', { layer, paintOrder });
    return result.newLayer;
}

/**
 * Removes the layer.
 * @param layer - Handle to the layer
 */
export async function DeleteLayer(layer: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DeleteLayer', { layer });
}

/**
 * Retrieves the layer title of an object.
 * @param layer - Handle to the layer
 * @returns The title value
 */
export async function GetLayerTitle(layer: number): Promise<string> {
    const result = await callCpp<{ title: string }>(SUITE_NAME, 'GetLayerTitle', { layer });
    return result.title;
}

/**
 * Sets the layer title of an object.
 * @param layer - Handle to the layer
 * @param newTitle - The New Title value
 */
export async function SetLayerTitle(layer: number, newTitle: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerTitle', { layer, newTitle });
}

/**
 * Retrieves the layer color of an object.
 * @param layer - Handle to the layer
 * @returns The color value
 */
export async function GetLayerColor(layer: number): Promise<any> {
    const result = await callCpp<{ color: any }>(SUITE_NAME, 'GetLayerColor', { layer });
    return result.color;
}

/**
 * Retrieves the layer visible of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerVisible(layer: number): Promise<boolean> {
    const result = await callCpp<{ visible: boolean }>(SUITE_NAME, 'GetLayerVisible', { layer });
    return result.visible;
}

/**
 * Sets the layer visible of an object.
 * @param layer - Handle to the layer
 * @param visible - The visibility flag
 */
export async function SetLayerVisible(layer: number, visible: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerVisible', { layer, visible });
}

/**
 * Retrieves the layer preview of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerPreview(layer: number): Promise<boolean> {
    const result = await callCpp<{ preview: boolean }>(SUITE_NAME, 'GetLayerPreview', { layer });
    return result.preview;
}

/**
 * Sets the layer preview of an object.
 * @param layer - Handle to the layer
 * @param preview - The Preview value
 */
export async function SetLayerPreview(layer: number, preview: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerPreview', { layer, preview });
}

/**
 * Retrieves the layer editable of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerEditable(layer: number): Promise<boolean> {
    const result = await callCpp<{ editable: boolean }>(SUITE_NAME, 'GetLayerEditable', { layer });
    return result.editable;
}

/**
 * Sets the layer editable of an object.
 * @param layer - Handle to the layer
 * @param editable - The Editable value
 */
export async function SetLayerEditable(layer: number, editable: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerEditable', { layer, editable });
}

/**
 * Retrieves the layer printed of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerPrinted(layer: number): Promise<boolean> {
    const result = await callCpp<{ printed: boolean }>(SUITE_NAME, 'GetLayerPrinted', { layer });
    return result.printed;
}

/**
 * Sets the layer printed of an object.
 * @param layer - Handle to the layer
 * @param printed - The Printed value
 */
export async function SetLayerPrinted(layer: number, printed: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerPrinted', { layer, printed });
}

/**
 * Retrieves the layer dim placed images of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerDimPlacedImages(layer: number): Promise<boolean> {
    const result = await callCpp<{ dimmed: boolean }>(SUITE_NAME, 'GetLayerDimPlacedImages', { layer });
    return result.dimmed;
}

/**
 * Sets the layer dim placed images of an object.
 * @param layer - Handle to the layer
 * @param dimmed - The Dimmed value
 */
export async function SetLayerDimPlacedImages(layer: number, dimmed: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerDimPlacedImages', { layer, dimmed });
}

/**
 * Retrieves the layer selected of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerSelected(layer: number): Promise<boolean> {
    const result = await callCpp<{ selected: boolean }>(SUITE_NAME, 'GetLayerSelected', { layer });
    return result.selected;
}

/**
 * Sets the layer selected of an object.
 * @param layer - Handle to the layer
 * @param selected - The selection state
 */
export async function SetLayerSelected(layer: number, selected: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerSelected', { layer, selected });
}

/**
 * Retrieves the layer by title of an object.
 * @param title - The Title value
 * @returns Handle to the layer
 */
export async function GetLayerByTitle(title: string): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'GetLayerByTitle', { title });
    return result.layer;
}

/**
 * Performs the layer has art operation.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function LayerHasArt(layer: number): Promise<boolean> {
    const result = await callCpp<{ hasArt: boolean }>(SUITE_NAME, 'LayerHasArt', { layer });
    return result.hasArt;
}

/**
 * Performs the layer has selected art operation.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function LayerHasSelectedArt(layer: number): Promise<boolean> {
    const result = await callCpp<{ hasSel: boolean }>(SUITE_NAME, 'LayerHasSelectedArt', { layer });
    return result.hasSel;
}

/**
 * Deselects the object.
 * @param layer - Handle to the layer
 */
export async function DeselectArtOnLayer(layer: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DeselectArtOnLayer', { layer });
}

/**
 * Selects the object.
 * @param layer - Handle to the layer
 */
export async function SelectArtOnLayer(layer: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SelectArtOnLayer', { layer });
}

/**
 * Retrieves the layer is template of an object.
 * @param layer - Handle to the layer
 * @returns True if the condition is met, false otherwise
 */
export async function GetLayerIsTemplate(layer: number): Promise<boolean> {
    const result = await callCpp<{ isTemplate: boolean }>(SUITE_NAME, 'GetLayerIsTemplate', { layer });
    return result.isTemplate;
}

/**
 * Sets the layer is template of an object.
 * @param layer - Handle to the layer
 * @param isTemplate - The Is Template value
 */
export async function SetLayerIsTemplate(layer: number, isTemplate: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerIsTemplate', { layer, isTemplate });
}

/**
 * Retrieves the prev layer of an object.
 * @param next - The Next value
 * @returns The prev value
 */
export async function GetPrevLayer(next: number): Promise<number> {
    const result = await callCpp<{ prev: number }>(SUITE_NAME, 'GetPrevLayer', { next });
    return result.prev;
}

/**
 * Retrieves the layer dimmed percent of an object.
 * @param layer - Handle to the layer
 * @returns The percent value
 */
export async function GetLayerDimmedPercent(layer: number): Promise<number> {
    const result = await callCpp<{ percent: number }>(SUITE_NAME, 'GetLayerDimmedPercent', { layer });
    return result.percent;
}

/**
 * Sets the layer dimmed percent of an object.
 * @param layer - Handle to the layer
 * @param percent - The Percent value
 */
export async function SetLayerDimmedPercent(layer: number, percent: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLayerDimmedPercent', { layer, percent });
}

/**
 * Retrieves the layer first child of an object.
 * @param layer - Handle to the layer
 * @returns The child value
 */
export async function GetLayerFirstChild(layer: number): Promise<number> {
    const result = await callCpp<{ child: number }>(SUITE_NAME, 'GetLayerFirstChild', { layer });
    return result.child;
}

/**
 * Retrieves the layer parent of an object.
 * @param layer - Handle to the layer
 * @returns The parent value
 */
export async function GetLayerParent(layer: number): Promise<number> {
    const result = await callCpp<{ parent: number }>(SUITE_NAME, 'GetLayerParent', { layer });
    return result.parent;
}

/**
 * Adds a new item.
 * @param art - Handle to the art object
 * @param paintOrder - The order position
 * @returns Handle to the layer
 */
export async function InsertLayerAtArt(art: number, paintOrder: any): Promise<number> {
    const result = await callCpp<{ newLayer: number }>(SUITE_NAME, 'InsertLayerAtArt', { art, paintOrder });
    return result.newLayer;
}

/**
 * Performs the change layer to group operation.
 * @param layer - Handle to the layer
 * @param group - The Group value
 */
export async function ChangeLayerToGroup(layer: number, group: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ChangeLayerToGroup', { layer, group });
}

/**
 * Retrieves the next preorder layer of an object.
 * @param prev - The Prev value
 * @returns The next value
 */
export async function GetNextPreorderLayer(prev: number): Promise<number> {
    const result = await callCpp<{ next: number }>(SUITE_NAME, 'GetNextPreorderLayer', { prev });
    return result.next;
}

/**
 * Retrieves the next non child preorder layer of an object.
 * @param prev - The Prev value
 * @returns The next value
 */
export async function GetNextNonChildPreorderLayer(prev: number): Promise<number> {
    const result = await callCpp<{ next: number }>(SUITE_NAME, 'GetNextNonChildPreorderLayer', { prev });
    return result.next;
}
