/**
 * AIArtSuite client functions
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

const SUITE_NAME = 'AIArtSuite';

/**
 * Creates a new art.
 * @param type - The type value
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function NewArt(type: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ newArt: number }>(SUITE_NAME, 'NewArt', { type, paintOrder, prep });
    return result.newArt;
}

/**
 * Removes the art.
 * @param art - Handle to the art object
 */
export async function DisposeArt(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DisposeArt', { art });
}

/**
 * Performs the reorder art operation.
 * @param thisArt - Handle to the art object
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 */
export async function ReorderArt(thisArt: number, paintOrder: number, prep: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ReorderArt', { thisArt, paintOrder, prep });
}

/**
 * Duplicates the object.
 * @param thisArt - Handle to the art object
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @returns Handle to the art object
 */
export async function DuplicateArt(thisArt: number, paintOrder: number, prep: number): Promise<number> {
    const result = await callCpp<{ newArt: number }>(SUITE_NAME, 'DuplicateArt', { thisArt, paintOrder, prep });
    return result.newArt;
}

/**
 * Retrieves the first art of layer of an object.
 * @param layer - Handle to the layer
 * @returns Handle to the art object
 */
export async function GetFirstArtOfLayer(layer: number): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'GetFirstArtOfLayer', { layer });
    return result.art;
}

/**
 * Retrieves the layer of art of an object.
 * @param art - Handle to the art object
 * @returns Handle to the layer
 */
export async function GetLayerOfArt(art: number): Promise<number> {
    const result = await callCpp<{ layer: number }>(SUITE_NAME, 'GetLayerOfArt', { art });
    return result.layer;
}

/**
 * Retrieves the art type of an object.
 * @param art - Handle to the art object
 * @returns The type value
 */
export async function GetArtType(art: number): Promise<number> {
    const result = await callCpp<{ type: number }>(SUITE_NAME, 'GetArtType', { art });
    return result.type;
}

/**
 * Retrieves the art user attr of an object.
 * @param art - Handle to the art object
 * @param whichAttr - The Which Attr value
 * @returns The attr value
 */
export async function GetArtUserAttr(art: number, whichAttr: number): Promise<number> {
    const result = await callCpp<{ attr: number }>(SUITE_NAME, 'GetArtUserAttr', { art, whichAttr });
    return result.attr;
}

/**
 * Sets the art user attr of an object.
 * @param art - Handle to the art object
 * @param whichAttr - The Which Attr value
 * @param attr - The Attr value
 */
export async function SetArtUserAttr(art: number, whichAttr: number, attr: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtUserAttr', { art, whichAttr, attr });
}

/**
 * Retrieves the art parent of an object.
 * @param art - Handle to the art object
 * @returns The parent value
 */
export async function GetArtParent(art: number): Promise<number> {
    const result = await callCpp<{ parent: number }>(SUITE_NAME, 'GetArtParent', { art });
    return result.parent;
}

/**
 * Retrieves the art first child of an object.
 * @param art - Handle to the art object
 * @returns The child value
 */
export async function GetArtFirstChild(art: number): Promise<number> {
    const result = await callCpp<{ child: number }>(SUITE_NAME, 'GetArtFirstChild', { art });
    return result.child;
}

/**
 * Retrieves the art sibling of an object.
 * @param art - Handle to the art object
 * @returns The sibling value
 */
export async function GetArtSibling(art: number): Promise<number> {
    const result = await callCpp<{ sibling: number }>(SUITE_NAME, 'GetArtSibling', { art });
    return result.sibling;
}

/**
 * Retrieves the art prior sibling of an object.
 * @param art - Handle to the art object
 * @returns The sibling value
 */
export async function GetArtPriorSibling(art: number): Promise<number> {
    const result = await callCpp<{ sibling: number }>(SUITE_NAME, 'GetArtPriorSibling', { art });
    return result.sibling;
}

/**
 * Retrieves the art bounds of an object.
 * @param art - Handle to the art object
 * @returns The bounding rectangle
 */
export async function GetArtBounds(art: number): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetArtBounds', { art });
    return result.bounds;
}

/**
 * Sets the art bounds of an object.
 * @param art - Handle to the art object
 */
export async function SetArtBounds(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtBounds', { art });
}

/**
 * Retrieves the art center point visible of an object.
 * @param art - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function GetArtCenterPointVisible(art: number): Promise<boolean> {
    const result = await callCpp<{ visible: boolean }>(SUITE_NAME, 'GetArtCenterPointVisible', { art });
    return result.visible;
}

/**
 * Sets the art center point visible of an object.
 * @param art - Handle to the art object
 * @param visible - The visibility flag
 */
export async function SetArtCenterPointVisible(art: number, visible: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtCenterPointVisible', { art, visible });
}

/**
 * Retrieves the art transform bounds of an object.
 * @param art - Handle to the art object
 * @param flags - The Flags value
 * @returns An object containing: transform, bounds
 */
export async function GetArtTransformBounds(art: number, flags: number): Promise<{ transform: AIRealMatrix; bounds: AIRealRect }> {
    const result = await callCpp<{ transform: AIRealMatrix; bounds: AIRealRect }>(SUITE_NAME, 'GetArtTransformBounds', { art, flags });
    return result;
}

/**
 * Updates the object state.
 * @param art - Handle to the art object
 * @param force - The Force value
 * @returns True if the condition is met, false otherwise
 */
export async function UpdateArtworkLink(art: number, force: boolean): Promise<boolean> {
    const result = await callCpp<{ updated: boolean }>(SUITE_NAME, 'UpdateArtworkLink', { art, force });
    return result.updated;
}

/**
 * Performs the valid art operation.
 * @param art - Handle to the art object
 * @param searchAllLayerLists - The Search All Layer Lists value
 */
export async function ValidArt(art: number, searchAllLayerLists: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'ValidArt', { art, searchAllLayerLists });
}

/**
 * Retrieves the art order of an object.
 * @param art1 - Handle to the art object
 * @param art2 - Handle to the art object
 * @returns The order value
 */
export async function GetArtOrder(art1: number, art2: number): Promise<number> {
    const result = await callCpp<{ order: number }>(SUITE_NAME, 'GetArtOrder', { art1, art2 });
    return result.order;
}

/**
 * Selects the object.
 * @param layer - Handle to the layer
 * @param name - The name string
 * @param matchWholeWord - The Match Whole Word value
 * @param caseSensitive - The Case Sensitive value
 */
export async function SelectNamedArtOfLayer(layer: number, name: string, matchWholeWord: boolean, caseSensitive: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SelectNamedArtOfLayer', { layer, name, matchWholeWord, caseSensitive });
}

/**
 * Retrieves the art rotated bounds of an object.
 * @param art - Handle to the art object
 * @param angle - The Angle value
 * @param flags - The Flags value
 * @returns The bounding rectangle
 */
export async function GetArtRotatedBounds(art: number, angle: number, flags: number): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetArtRotatedBounds', { art, angle, flags });
    return result.bounds;
}

/**
 * Performs the art has fill operation.
 * @param art - Handle to the art object
 */
export async function ArtHasFill(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ArtHasFill', { art });
}

/**
 * Performs the art has stroke operation.
 * @param art - Handle to the art object
 */
export async function ArtHasStroke(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ArtHasStroke', { art });
}

/**
 * Performs the arts have equal paths operation.
 * @param art1 - Handle to the art object
 * @param art2 - Handle to the art object
 */
export async function ArtsHaveEqualPaths(art1: number, art2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ArtsHaveEqualPaths', { art1, art2 });
}

/**
 * Performs the art copy fill style if equal paths operation.
 * @param dstArt - Handle to the art object
 * @param srcArt - Handle to the art object
 */
export async function ArtCopyFillStyleIfEqualPaths(dstArt: number, srcArt: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ArtCopyFillStyleIfEqualPaths', { dstArt, srcArt });
}

/**
 * Performs the art copy stroke style if equal paths operation.
 * @param dstArt - Handle to the art object
 * @param srcArt - Handle to the art object
 */
export async function ArtCopyStrokeStyleIfEqualPaths(dstArt: number, srcArt: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ArtCopyStrokeStyleIfEqualPaths', { dstArt, srcArt });
}

/**
 * Retrieves the insertion point of an object.
 * @returns An object containing: art, paintorder, editable
 */
export async function GetInsertionPoint(): Promise<{ art: number; paintorder: number; editable: boolean }> {
    const result = await callCpp<{ art: number; paintorder: number; editable: boolean }>(SUITE_NAME, 'GetInsertionPoint', {  });
    return result;
}

/**
 * Sets the insertion point of an object.
 * @param art - Handle to the art object
 */
export async function SetInsertionPoint(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetInsertionPoint', { art });
}

/**
 * Retrieves the key art of an object.
 * @returns Handle to the art object
 */
export async function GetKeyArt(): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'GetKeyArt', {  });
    return result.art;
}

/**
 * Performs the cancel key art operation.
 * @param void - The Void value
 */
export async function CancelKeyArt(void: void): Promise<void> {
    await callCpp(SUITE_NAME, 'CancelKeyArt', { void });
}

/**
 * Retrieves the dictionary of an object.
 * @param art - Handle to the art object
 * @returns The dictionary value
 */
export async function GetDictionary(art: number): Promise<any> {
    const result = await callCpp<{ dictionary: any }>(SUITE_NAME, 'GetDictionary', { art });
    return result.dictionary;
}

/**
 * Checks if has dictionary.
 * @param art - Handle to the art object
 */
export async function HasDictionary(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'HasDictionary', { art });
}

/**
 * Checks if is dictionary empty.
 * @param art - Handle to the art object
 */
export async function IsDictionaryEmpty(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'IsDictionaryEmpty', { art });
}

/**
 * Sets the art name of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 */
export async function SetArtName(art: number, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtName', { art, name });
}

/**
 * Retrieves the art name of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 * @returns The is default name value
 */
export async function GetArtName(art: number, name: string): Promise<any> {
    const result = await callCpp<{ isDefaultName: any }>(SUITE_NAME, 'GetArtName', { art, name });
    return result.isDefaultName;
}

/**
 * Checks if is art layer group.
 * @param art - Handle to the art object
 * @returns The is layer group value
 */
export async function IsArtLayerGroup(art: number): Promise<any> {
    const result = await callCpp<{ isLayerGroup: any }>(SUITE_NAME, 'IsArtLayerGroup', { art });
    return result.isLayerGroup;
}

/**
 * Performs the release to layers operation.
 * @param art - Handle to the art object
 * @param build - The Build value
 */
export async function ReleaseToLayers(art: number, build: any): Promise<void> {
    await callCpp(SUITE_NAME, 'ReleaseToLayers', { art, build });
}

/**
 * Performs the modify targeted art set operation.
 * @param count - The count value
 * @param action - The Action value
 * @returns The list value
 */
export async function ModifyTargetedArtSet(count: number, action: number): Promise<number> {
    const result = await callCpp<{ list: number }>(SUITE_NAME, 'ModifyTargetedArtSet', { count, action });
    return result.list;
}

/**
 * Checks if is art styled art.
 * @param art - Handle to the art object
 */
export async function IsArtStyledArt(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'IsArtStyledArt', { art });
}

/**
 * Checks if is art clipping.
 * @param art - Handle to the art object
 */
export async function IsArtClipping(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'IsArtClipping', { art });
}

/**
 * Performs the transfer attributes operation.
 * @param srcart - Handle to the art object
 * @param dstart - Handle to the art object
 * @param which - The Which value
 */
export async function TransferAttributes(srcart: number, dstart: number, which: any): Promise<void> {
    await callCpp(SUITE_NAME, 'TransferAttributes', { srcart, dstart, which });
}

/**
 * Retrieves the art last child of an object.
 * @param art - Handle to the art object
 * @returns The child value
 */
export async function GetArtLastChild(art: number): Promise<number> {
    const result = await callCpp<{ child: number }>(SUITE_NAME, 'GetArtLastChild', { art });
    return result.child;
}

/**
 * Sets the art text wrap property of an object.
 * @param art - Handle to the art object
 * @param offset - The Offset value
 * @param invert - The Invert value
 */
export async function SetArtTextWrapProperty(art: number, offset: number, invert: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtTextWrapProperty', { art, offset, invert });
}

/**
 * Retrieves the art text wrap property of an object.
 * @param art - Handle to the art object
 * @returns An object containing: offset, invert
 */
export async function GetArtTextWrapProperty(art: number): Promise<{ offset: number; invert: boolean }> {
    const result = await callCpp<{ offset: number; invert: boolean }>(SUITE_NAME, 'GetArtTextWrapProperty', { art });
    return result;
}

/**
 * Creates a new copy scope.
 * @param kind - The Kind value
 * @returns The scope value
 */
export async function CreateCopyScope(kind: any): Promise<any> {
    const result = await callCpp<{ scope: any }>(SUITE_NAME, 'CreateCopyScope', { kind });
    return result.scope;
}

/**
 * Performs the destroy copy scope operation.
 * @param scope - The Scope value
 */
export async function DestroyCopyScope(scope: any): Promise<void> {
    await callCpp(SUITE_NAME, 'DestroyCopyScope', { scope });
}

/**
 * Adds a new item.
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 * @param artType - Handle to the art object
 */
export async function InsertionPointBadForArtType(paintOrder: number, prep: number, artType: number): Promise<void> {
    await callCpp(SUITE_NAME, 'InsertionPointBadForArtType', { paintOrder, prep, artType });
}

/**
 * Performs the preinsertion flight check operation.
 * @param candidateArt - Handle to the art object
 * @param paintOrder - The order position
 * @param prep - The prepositional object reference
 */
export async function PreinsertionFlightCheck(candidateArt: number, paintOrder: number, prep: number): Promise<void> {
    await callCpp(SUITE_NAME, 'PreinsertionFlightCheck', { candidateArt, paintOrder, prep });
}

/**
 * Sets the note of an object.
 * @param art - Handle to the art object
 * @param inNote - The In Note value
 */
export async function SetNote(art: number, inNote: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetNote', { art, inNote });
}

/**
 * Retrieves the note of an object.
 * @param art - Handle to the art object
 * @param outNote - The Out Note value
 */
export async function GetNote(art: number, outNote: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetNote', { art, outNote });
}

/**
 * Checks if has note.
 * @param art - Handle to the art object
 */
export async function HasNote(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'HasNote', { art });
}

/**
 * Removes the note.
 * @param art - Handle to the art object
 */
export async function DeleteNote(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'DeleteNote', { art });
}

/**
 * Retrieves the art x m p size of an object.
 * @param art - Handle to the art object
 * @returns The size value
 */
export async function GetArtXMPSize(art: number): Promise<number> {
    const result = await callCpp<{ size: number }>(SUITE_NAME, 'GetArtXMPSize', { art });
    return result.size;
}

/**
 * Retrieves the art x m p of an object.
 * @param art - Handle to the art object
 * @param size - The Size value
 * @returns The xmp value
 */
export async function GetArtXMP(art: number, size: number): Promise<any> {
    const result = await callCpp<{ xmp: any }>(SUITE_NAME, 'GetArtXMP', { art, size });
    return result.xmp;
}

/**
 * Sets the art x m p of an object.
 * @param art - Handle to the art object
 * @param xmp - The Xmp value
 */
export async function SetArtXMP(art: number, xmp: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtXMP', { art, xmp });
}

/**
 * Retrieves the precise art transform bounds of an object.
 * @param art - Handle to the art object
 * @param flags - The Flags value
 * @returns An object containing: transform, bounds
 */
export async function GetPreciseArtTransformBounds(art: number, flags: number): Promise<{ transform: AIRealMatrix; bounds: any }> {
    const result = await callCpp<{ transform: AIRealMatrix; bounds: any }>(SUITE_NAME, 'GetPreciseArtTransformBounds', { art, flags });
    return result;
}

/**
 * Performs the unchecked dispose art operation.
 * @param art - Handle to the art object
 */
export async function UncheckedDisposeArt(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'UncheckedDisposeArt', { art });
}

/**
 * Performs the art is graph operation.
 * @param art - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function ArtIsGraph(art: number): Promise<boolean> {
    const result = await callCpp<{ artisgraph: boolean }>(SUITE_NAME, 'ArtIsGraph', { art });
    return result.artisgraph;
}

/**
 * Sets the key art of an object.
 * @param art - Handle to the art object
 */
export async function SetKeyArt(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetKeyArt', { art });
}

/**
 * Retrieves the drawing mode of an object.
 * @returns The mode value
 */
export async function GetDrawingMode(): Promise<number> {
    const result = await callCpp<{ mode: number }>(SUITE_NAME, 'GetDrawingMode', {  });
    return result.mode;
}

/**
 * Sets the drawing mode of an object.
 * @param mode - The Mode value
 */
export async function SetDrawingMode(mode: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDrawingMode', { mode });
}

/**
 * Retrieves the insertion point for drawing mode of an object.
 * @param mode - The Mode value
 * @returns An object containing: art, paintorder, editable
 */
export async function GetInsertionPointForDrawingMode(mode: number): Promise<{ art: number; paintorder: number; editable: boolean }> {
    const result = await callCpp<{ art: number; paintorder: number; editable: boolean }>(SUITE_NAME, 'GetInsertionPointForDrawingMode', { mode });
    return result;
}

/**
 * Retrieves the insertion point for current drawing mode of an object.
 * @returns An object containing: art, paintorder, editable
 */
export async function GetInsertionPointForCurrentDrawingMode(): Promise<{ art: number; paintorder: number; editable: boolean }> {
    const result = await callCpp<{ art: number; paintorder: number; editable: boolean }>(SUITE_NAME, 'GetInsertionPointForCurrentDrawingMode', {  });
    return result;
}

/**
 * Retrieves the path polarity of an object.
 * @param art - Handle to the art object
 * @returns The polarity value
 */
export async function GetPathPolarity(art: number): Promise<number> {
    const result = await callCpp<{ polarity: number }>(SUITE_NAME, 'GetPathPolarity', { art });
    return result.polarity;
}

/**
 * Checks if is pixel perfect.
 * @param art - Handle to the art object
 */
export async function IsPixelPerfect(art: number): Promise<void> {
    await callCpp(SUITE_NAME, 'IsPixelPerfect', { art });
}

/**
 * Sets the pixel perfect of an object.
 * @param art - Handle to the art object
 * @param isPixelPerfect - The Is Pixel Perfect value
 */
export async function SetPixelPerfect(art: number, isPixelPerfect: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetPixelPerfect', { art, isPixelPerfect });
}

/**
 * Performs the objects are equivalent operation.
 * @param art1 - Handle to the art object
 * @param art2 - Handle to the art object
 */
export async function ObjectsAreEquivalent(art1: number, art2: number): Promise<void> {
    await callCpp(SUITE_NAME, 'ObjectsAreEquivalent', { art1, art2 });
}

/**
 * Checks if is art a layer in symbol.
 * @param art - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function IsArtALayerInSymbol(art: number): Promise<boolean> {
    const result = await callCpp<{ isLayerInSymbol: boolean }>(SUITE_NAME, 'IsArtALayerInSymbol', { art });
    return result.isLayerInSymbol;
}

/**
 * Retrieves the art time stamp of an object.
 * @param art - Handle to the art object
 * @param option - The point coordinates
 * @returns The time stamp value
 */
export async function GetArtTimeStamp(art: number, option: any): Promise<number> {
    const result = await callCpp<{ timeStamp: number }>(SUITE_NAME, 'GetArtTimeStamp', { art, option });
    return result.timeStamp;
}

/**
 * Retrieves the global time stamp of an object.
 */
export async function GetGlobalTimeStamp(): Promise<void> {
    await callCpp(SUITE_NAME, 'GetGlobalTimeStamp', {  });
}

/**
 * Performs the convert point type to area type operation.
 * @param art - Handle to the art object
 * @returns Handle to the art object
 */
export async function ConvertPointTypeToAreaType(art: number): Promise<number> {
    const result = await callCpp<{ newArtHandle: number }>(SUITE_NAME, 'ConvertPointTypeToAreaType', { art });
    return result.newArtHandle;
}

/**
 * Performs the convert area type to point type operation.
 * @param art - Handle to the art object
 * @returns Handle to the art object
 */
export async function ConvertAreaTypeToPointType(art: number): Promise<number> {
    const result = await callCpp<{ newArtHandle: number }>(SUITE_NAME, 'ConvertAreaTypeToPointType', { art });
    return result.newArtHandle;
}

/**
 * Performs the mark dirty operation.
 * @param art - Handle to the art object
 * @param markStyleDirty - The Mark Style Dirty value
 */
export async function MarkDirty(art: number, markStyleDirty: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'MarkDirty', { art, markStyleDirty });
}

/**
 * Retrieves the safe art handle of an object.
 * @param art - Handle to the art object
 * @returns The safe art value
 */
export async function GetSafeArtHandle(art: number): Promise<any> {
    const result = await callCpp<{ safeArt: any }>(SUITE_NAME, 'GetSafeArtHandle', { art });
    return result.safeArt;
}

/**
 * Retrieves the art handle of an object.
 * @param safeArt - The Safe Art value
 * @returns Handle to the art object
 */
export async function GetArtHandle(safeArt: any): Promise<number> {
    const result = await callCpp<{ art: number }>(SUITE_NAME, 'GetArtHandle', { safeArt });
    return result.art;
}

/**
 * Retrieves the art default name of an object.
 * @param art - Handle to the art object
 * @param name - The name string
 */
export async function GetArtDefaultName(art: number, name: string): Promise<void> {
    await callCpp(SUITE_NAME, 'GetArtDefaultName', { art, name });
}

/**
 * Retrieves the document of art of an object.
 * @param art - Handle to the art object
 * @returns The document value
 */
export async function GetDocumentOfArt(art: number): Promise<number> {
    const result = await callCpp<{ document: number }>(SUITE_NAME, 'GetDocumentOfArt', { art });
    return result.document;
}
