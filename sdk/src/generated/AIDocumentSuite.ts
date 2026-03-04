/**
 * AIDocumentSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';
import { AIRealPoint, AIRealRect } from './types';

const SUITE_NAME = 'AIDocumentSuite';

/**
 * Retrieves the document file specification of an object.
 * @returns The file value
 */
export async function GetDocumentFileSpecification(): Promise<string> {
    const result = await callCpp<{ file: string }>(SUITE_NAME, 'GetDocumentFileSpecification', {  });
    return result.file;
}

/**
 * Retrieves the document file specification from handle of an object.
 * @param document - Handle to the document
 * @returns The file value
 */
export async function GetDocumentFileSpecificationFromHandle(document: number): Promise<string> {
    const result = await callCpp<{ file: string }>(SUITE_NAME, 'GetDocumentFileSpecificationFromHandle', { document });
    return result.file;
}

/**
 * Retrieves the document page origin of an object.
 * @returns The origin value
 */
export async function GetDocumentPageOrigin(): Promise<AIRealPoint> {
    const result = await callCpp<{ origin: AIRealPoint }>(SUITE_NAME, 'GetDocumentPageOrigin', {  });
    return result.origin;
}

/**
 * Sets the document page origin of an object.
 * @returns The origin value
 */
export async function SetDocumentPageOrigin(): Promise<AIRealPoint> {
    const result = await callCpp<{ origin: AIRealPoint }>(SUITE_NAME, 'SetDocumentPageOrigin', {  });
    return result.origin;
}

/**
 * Sets the document ruler origin of an object.
 * @returns The origin value
 */
export async function SetDocumentRulerOrigin(): Promise<AIRealPoint> {
    const result = await callCpp<{ origin: AIRealPoint }>(SUITE_NAME, 'SetDocumentRulerOrigin', {  });
    return result.origin;
}

/**
 * Retrieves the document ruler units of an object.
 * @returns The units value
 */
export async function GetDocumentRulerUnits(): Promise<number> {
    const result = await callCpp<{ units: number }>(SUITE_NAME, 'GetDocumentRulerUnits', {  });
    return result.units;
}

/**
 * Sets the document ruler units of an object.
 * @param units - The Units value
 */
export async function SetDocumentRulerUnits(units: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentRulerUnits', { units });
}

/**
 * Retrieves the document crop style of an object.
 * @returns The crop style value
 */
export async function GetDocumentCropStyle(): Promise<any> {
    const result = await callCpp<{ cropStyle: any }>(SUITE_NAME, 'GetDocumentCropStyle', {  });
    return result.cropStyle;
}

/**
 * Sets the document crop style of an object.
 * @param cropStyle - The Crop Style value
 */
export async function SetDocumentCropStyle(cropStyle: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentCropStyle', { cropStyle });
}

/**
 * Retrieves the document print record of an object.
 * @returns The print value
 */
export async function GetDocumentPrintRecord(): Promise<any> {
    const result = await callCpp<{ print: any }>(SUITE_NAME, 'GetDocumentPrintRecord', {  });
    return result.print;
}

/**
 * Sets the document print record of an object.
 * @param print - The Print value
 */
export async function SetDocumentPrintRecord(print: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentPrintRecord', { print });
}

/**
 * Retrieves the document setup of an object.
 * @returns The setup value
 */
export async function GetDocumentSetup(): Promise<any> {
    const result = await callCpp<{ setup: any }>(SUITE_NAME, 'GetDocumentSetup', {  });
    return result.setup;
}

/**
 * Sets the document setup of an object.
 * @returns The setup value
 */
export async function SetDocumentSetup(): Promise<any> {
    const result = await callCpp<{ setup: any }>(SUITE_NAME, 'SetDocumentSetup', {  });
    return result.setup;
}

/**
 * Retrieves the document modified of an object.
 * @returns True if the condition is met, false otherwise
 */
export async function GetDocumentModified(): Promise<boolean> {
    const result = await callCpp<{ modified: boolean }>(SUITE_NAME, 'GetDocumentModified', {  });
    return result.modified;
}

/**
 * Sets the document modified of an object.
 * @param modified - The Modified value
 */
export async function SetDocumentModified(modified: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentModified', { modified });
}

/**
 * Retrieves the document file format of an object.
 * @returns The file format value
 */
export async function GetDocumentFileFormat(): Promise<any> {
    const result = await callCpp<{ fileFormat: any }>(SUITE_NAME, 'GetDocumentFileFormat', {  });
    return result.fileFormat;
}

/**
 * Sets the document file format of an object.
 * @param fileFormat - The File Format value
 */
export async function SetDocumentFileFormat(fileFormat: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentFileFormat', { fileFormat });
}

/**
 * Sets the document file format parameters of an object.
 * @param parameters - The Parameters value
 */
export async function SetDocumentFileFormatParameters(parameters: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentFileFormatParameters', { parameters });
}

/**
 * Retrieves the document of an object.
 * @returns The document value
 */
export async function GetDocument(): Promise<number> {
    const result = await callCpp<{ document: number }>(SUITE_NAME, 'GetDocument', {  });
    return result.document;
}

/**
 * Performs the write document operation.
 * @param file - The File value
 * @param fileFormatName - The name string
 * @param askForParms - The Ask For Parms value
 */
export async function WriteDocument(file: string, fileFormatName: string, askForParms: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'WriteDocument', { file, fileFormatName, askForParms });
}

/**
 * Retrieves the document mi print record of an object.
 * @param print - The Print value
 */
export async function GetDocumentMiPrintRecord(print: any): Promise<void> {
    await callCpp(SUITE_NAME, 'GetDocumentMiPrintRecord', { print });
}

/**
 * Sets the document mi print record of an object.
 * @param print - The Print value
 */
export async function SetDocumentMiPrintRecord(print: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentMiPrintRecord', { print });
}

/**
 * Retrieves the document ruler origin of an object.
 * @returns The origin value
 */
export async function GetDocumentRulerOrigin(): Promise<AIRealPoint> {
    const result = await callCpp<{ origin: AIRealPoint }>(SUITE_NAME, 'GetDocumentRulerOrigin', {  });
    return result.origin;
}

/**
 * Updates the object state.
 * @returns True if the condition is met, false otherwise
 */
export async function UpdateLinks(): Promise<boolean> {
    const result = await callCpp<{ updatedSomething: boolean }>(SUITE_NAME, 'UpdateLinks', {  });
    return result.updatedSomething;
}

/**
 * Retrieves the document zoom limit of an object.
 * @returns An object containing: min, max
 */
export async function GetDocumentZoomLimit(): Promise<{ min: number; max: number }> {
    const result = await callCpp<{ min: number; max: number }>(SUITE_NAME, 'GetDocumentZoomLimit', {  });
    return result;
}

/**
 * Retrieves the document max artboard bounds of an object.
 * @returns The bounding rectangle
 */
export async function GetDocumentMaxArtboardBounds(): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetDocumentMaxArtboardBounds', {  });
    return result.bounds;
}

/**
 * Performs the document exists operation.
 * @param document - Handle to the document
 * @returns True if the condition is met, false otherwise
 */
export async function DocumentExists(document: number): Promise<boolean> {
    const result = await callCpp<{ exists: boolean }>(SUITE_NAME, 'DocumentExists', { document });
    return result.exists;
}

/**
 * Retrieves the document color model of an object.
 * @returns The color model value
 */
export async function GetDocumentColorModel(): Promise<number> {
    const result = await callCpp<{ colorModel: number }>(SUITE_NAME, 'GetDocumentColorModel', {  });
    return result.colorModel;
}

/**
 * Sets the document color model of an object.
 * @param colorModel - The color value
 */
export async function SetDocumentColorModel(colorModel: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentColorModel', { colorModel });
}

/**
 * Copies the object.
 */
export async function Copy(): Promise<void> {
    await callCpp(SUITE_NAME, 'Copy', {  });
}

/**
 * Performs the cut operation.
 */
export async function Cut(): Promise<void> {
    await callCpp(SUITE_NAME, 'Cut', {  });
}

/**
 * Performs the paste operation.
 */
export async function Paste(): Promise<void> {
    await callCpp(SUITE_NAME, 'Paste', {  });
}

/**
 * Retrieves the document targeting of an object.
 * @returns An object containing: paintTarget, transparencyTarget, effectsTarget
 */
export async function GetDocumentTargeting(): Promise<{ paintTarget: number; transparencyTarget: number; effectsTarget: number }> {
    const result = await callCpp<{ paintTarget: number; transparencyTarget: number; effectsTarget: number }>(SUITE_NAME, 'GetDocumentTargeting', {  });
    return result;
}

/**
 * Sets the document targeting of an object.
 * @param paintTarget - The Paint Target value
 * @param transparencyTarget - The Transparency Target value
 * @param effectsTarget - The Effects Target value
 */
export async function SetDocumentTargeting(paintTarget: number, transparencyTarget: number, effectsTarget: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentTargeting', { paintTarget, transparencyTarget, effectsTarget });
}

/**
 * Performs the document has transparency operation.
 * @param detectOverprint - The Detect Overprint value
 * @returns True if the condition is met, false otherwise
 */
export async function DocumentHasTransparency(detectOverprint: boolean): Promise<boolean> {
    const result = await callCpp<{ hasTransparency: boolean }>(SUITE_NAME, 'DocumentHasTransparency', { detectOverprint });
    return result.hasTransparency;
}

/**
 * Performs the document has spot color art operation.
 * @returns True if the condition is met, false otherwise
 */
export async function DocumentHasSpotColorArt(): Promise<boolean> {
    const result = await callCpp<{ hasSpotColorArt: boolean }>(SUITE_NAME, 'DocumentHasSpotColorArt', {  });
    return result.hasSpotColorArt;
}

/**
 * Sets the document asset mgmt info of an object.
 * @param URL - The U R L value
 * @returns An object containing: managed, checkedOut, canEdit
 */
export async function SetDocumentAssetMgmtInfo(URL: string): Promise<{ managed: boolean; checkedOut: boolean; canEdit: boolean }> {
    const result = await callCpp<{ managed: boolean; checkedOut: boolean; canEdit: boolean }>(SUITE_NAME, 'SetDocumentAssetMgmtInfo', { URL });
    return result;
}

/**
 * Sets the document x a p of an object.
 * @param xap - The Xap value
 */
export async function SetDocumentXAP(xap: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentXAP', { xap });
}

/**
 * Performs the suspend text reflow operation.
 */
export async function SuspendTextReflow(): Promise<void> {
    await callCpp(SUITE_NAME, 'SuspendTextReflow', {  });
}

/**
 * Performs the resume text reflow operation.
 */
export async function ResumeTextReflow(): Promise<void> {
    await callCpp(SUITE_NAME, 'ResumeTextReflow', {  });
}

/**
 * Checks if has text focus.
 * @returns True if the condition is met, false otherwise
 */
export async function HasTextFocus(): Promise<boolean> {
    const result = await callCpp<{ focus: boolean }>(SUITE_NAME, 'HasTextFocus', {  });
    return result.focus;
}

/**
 * Checks if has text caret.
 * @returns True if the condition is met, false otherwise
 */
export async function HasTextCaret(): Promise<boolean> {
    const result = await callCpp<{ caret: boolean }>(SUITE_NAME, 'HasTextCaret', {  });
    return result.caret;
}

/**
 * Retrieves the text focus of an object.
 * @returns The p story value
 */
export async function GetTextFocus(): Promise<any> {
    const result = await callCpp<{ pStory: any }>(SUITE_NAME, 'GetTextFocus', {  });
    return result.pStory;
}

/**
 * Sets the text focus of an object.
 * @param story - The Story value
 */
export async function SetTextFocus(story: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetTextFocus', { story });
}

/**
 * Performs the lose text focus operation.
 */
export async function LoseTextFocus(): Promise<void> {
    await callCpp(SUITE_NAME, 'LoseTextFocus', {  });
}

/**
 * Retrieves the document text resources of an object.
 * @returns The p doc resources value
 */
export async function GetDocumentTextResources(): Promise<any> {
    const result = await callCpp<{ pDocResources: any }>(SUITE_NAME, 'GetDocumentTextResources', {  });
    return result.pDocResources;
}

/**
 * Performs the write document mac information resource operation.
 * @param file - The File value
 */
export async function WriteDocumentMacInformationResource(file: string): Promise<void> {
    await callCpp(SUITE_NAME, 'WriteDocumentMacInformationResource', { file });
}

/**
 * Performs the write document as library operation.
 * @param file - The File value
 * @param libraryType - The Library Type value
 */
export async function WriteDocumentAsLibrary(file: string, libraryType: any): Promise<void> {
    await callCpp(SUITE_NAME, 'WriteDocumentAsLibrary', { file, libraryType });
}

/**
 * Performs the document has overprint operation.
 * @returns True if the condition is met, false otherwise
 */
export async function DocumentHasOverprint(): Promise<boolean> {
    const result = await callCpp<{ hasOverprint: boolean }>(SUITE_NAME, 'DocumentHasOverprint', {  });
    return result.hasOverprint;
}

/**
 * Performs the document has managed links operation.
 * @param document - Handle to the document
 * @returns True if the condition is met, false otherwise
 */
export async function DocumentHasManagedLinks(document: number): Promise<boolean> {
    const result = await callCpp<{ hasManagedLinks: boolean }>(SUITE_NAME, 'DocumentHasManagedLinks', { document });
    return result.hasManagedLinks;
}

/**
 * Retrieves the document spot color mode of an object.
 * @param document - Handle to the document
 * @returns The mode value
 */
export async function GetDocumentSpotColorMode(document: number): Promise<any> {
    const result = await callCpp<{ mode: any }>(SUITE_NAME, 'GetDocumentSpotColorMode', { document });
    return result.mode;
}

/**
 * Performs the undo operation.
 */
export async function Undo(): Promise<void> {
    await callCpp(SUITE_NAME, 'Undo', {  });
}

/**
 * Performs the redo operation.
 */
export async function Redo(): Promise<void> {
    await callCpp(SUITE_NAME, 'Redo', {  });
}

/**
 * Performs the document raster attributes operation.
 * @returns An object containing: hasDeviceNRasters, hasOverprint
 */
export async function DocumentRasterAttributes(): Promise<{ hasDeviceNRasters: boolean; hasOverprint: boolean }> {
    const result = await callCpp<{ hasDeviceNRasters: boolean; hasOverprint: boolean }>(SUITE_NAME, 'DocumentRasterAttributes', {  });
    return result;
}

/**
 * Retrieves the document startup profile of an object.
 * @param document - Handle to the document
 * @returns The startup profile value
 */
export async function GetDocumentStartupProfile(document: number): Promise<any> {
    const result = await callCpp<{ startupProfile: any }>(SUITE_NAME, 'GetDocumentStartupProfile', { document });
    return result.startupProfile;
}

/**
 * Retrieves the document bleeds of an object.
 * @returns The bleed offset value
 */
export async function GetDocumentBleeds(): Promise<AIRealRect> {
    const result = await callCpp<{ bleedOffset: AIRealRect }>(SUITE_NAME, 'GetDocumentBleeds', {  });
    return result.bleedOffset;
}

/**
 * Sets the document bleeds of an object.
 * @param bleedOffset - The Bleed Offset value
 */
export async function SetDocumentBleeds(bleedOffset: AIRealRect): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentBleeds', { bleedOffset });
}

/**
 * Sets the document pixel perfect status of an object.
 * @param isPixelPerfect - The Is Pixel Perfect value
 */
export async function SetDocumentPixelPerfectStatus(isPixelPerfect: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentPixelPerfectStatus', { isPixelPerfect });
}

/**
 * Removes the selection.
 */
export async function DeleteSelection(): Promise<void> {
    await callCpp(SUITE_NAME, 'DeleteSelection', {  });
}

/**
 * Sets the auto assign u i d on art creation of an object.
 * @param artType - Handle to the art object
 * @param autoAssignUIDOnArtCreation - The Auto Assign U I D On Art Creation value
 */
export async function SetAutoAssignUIDOnArtCreation(artType: number, autoAssignUIDOnArtCreation: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetAutoAssignUIDOnArtCreation', { artType, autoAssignUIDOnArtCreation });
}

/**
 * Retrieves the auto assign u i d on art creation of an object.
 * @param artType - Handle to the art object
 * @returns True if the condition is met, false otherwise
 */
export async function GetAutoAssignUIDOnArtCreation(artType: number): Promise<boolean> {
    const result = await callCpp<{ outAutoAssignUIDOnArtCreation: boolean }>(SUITE_NAME, 'GetAutoAssignUIDOnArtCreation', { artType });
    return result.outAutoAssignUIDOnArtCreation;
}

/**
 * Retrieves the document scale of an object.
 * @returns The doc scale value
 */
export async function GetDocumentScale(): Promise<number> {
    const result = await callCpp<{ docScale: number }>(SUITE_NAME, 'GetDocumentScale', {  });
    return result.docScale;
}

/**
 * Retrieves the document file name of an object.
 * @returns The name string
 */
export async function GetDocumentFileName(): Promise<string> {
    const result = await callCpp<{ fileName: string }>(SUITE_NAME, 'GetDocumentFileName', {  });
    return result.fileName;
}

/**
 * Retrieves the document file name no ext of an object.
 * @returns The name string
 */
export async function GetDocumentFileNameNoExt(): Promise<string> {
    const result = await callCpp<{ fileName: string }>(SUITE_NAME, 'GetDocumentFileNameNoExt', {  });
    return result.fileName;
}

/**
 * Retrieves the document file name from handle of an object.
 * @param document - Handle to the document
 * @returns The name string
 */
export async function GetDocumentFileNameFromHandle(document: number): Promise<string> {
    const result = await callCpp<{ fileName: string }>(SUITE_NAME, 'GetDocumentFileNameFromHandle', { document });
    return result.fileName;
}

/**
 * Retrieves the document file name no ext from handle of an object.
 * @param document - Handle to the document
 * @returns The name string
 */
export async function GetDocumentFileNameNoExtFromHandle(document: number): Promise<string> {
    const result = await callCpp<{ fileName: string }>(SUITE_NAME, 'GetDocumentFileNameNoExtFromHandle', { document });
    return result.fileName;
}

/**
 * Retrieves the last exported file path of an object.
 * @param option - The point coordinates
 * @returns The file value
 */
export async function GetLastExportedFilePath(option: any): Promise<string> {
    const result = await callCpp<{ file: string }>(SUITE_NAME, 'GetLastExportedFilePath', { option });
    return result.file;
}

/**
 * Sets the last exported file path of an object.
 * @param option - The point coordinates
 * @param file - The File value
 */
export async function SetLastExportedFilePath(option: any, file: string): Promise<void> {
    await callCpp(SUITE_NAME, 'SetLastExportedFilePath', { option, file });
}
