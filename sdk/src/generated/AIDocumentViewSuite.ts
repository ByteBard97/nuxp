/**
 * AIDocumentViewSuite client functions
 * Auto-generated from Adobe Illustrator SDK
 */

import { callCpp } from '@/sdk/bridge';
import { AIRealPoint, AIRealRect } from './types';

const SUITE_NAME = 'AIDocumentViewSuite';

/**
 * Retrieves the document view bounds of an object.
 * @param view - The View value
 * @returns The bounding rectangle
 */
export async function GetDocumentViewBounds(view: any): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetDocumentViewBounds', { view });
    return result.bounds;
}

/**
 * Retrieves the document view center of an object.
 * @param view - The View value
 * @returns The center value
 */
export async function GetDocumentViewCenter(view: any): Promise<AIRealPoint> {
    const result = await callCpp<{ center: AIRealPoint }>(SUITE_NAME, 'GetDocumentViewCenter', { view });
    return result.center;
}

/**
 * Sets the document view center of an object.
 * @param view - The View value
 * @param center - The Center value
 */
export async function SetDocumentViewCenter(view: any, center: AIRealPoint): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewCenter', { view, center });
}

/**
 * Retrieves the document view user visible zoom of an object.
 * @param view - The View value
 * @returns The zoom value
 */
export async function GetDocumentViewUserVisibleZoom(view: any): Promise<number> {
    const result = await callCpp<{ zoom: number }>(SUITE_NAME, 'GetDocumentViewUserVisibleZoom', { view });
    return result.zoom;
}

/**
 * Retrieves the document view zoom of an object.
 * @param view - The View value
 * @returns The zoom value
 */
export async function GetDocumentViewZoom(view: any): Promise<number> {
    const result = await callCpp<{ zoom: number }>(SUITE_NAME, 'GetDocumentViewZoom', { view });
    return result.zoom;
}

/**
 * Sets the document view zoom of an object.
 * @param view - The View value
 * @param zoom - The Zoom value
 */
export async function SetDocumentViewZoom(view: any, zoom: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewZoom', { view, zoom });
}

/**
 * Sets the document view user visible zoom of an object.
 * @param view - The View value
 * @param zoom - The Zoom value
 */
export async function SetDocumentViewUserVisibleZoom(view: any, zoom: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewUserVisibleZoom', { view, zoom });
}

/**
 * Performs the artwork point to view point operation.
 * @param view - The View value
 * @param artworkPoint - The point coordinates
 * @returns The view point value
 */
export async function ArtworkPointToViewPoint(view: any, artworkPoint: AIRealPoint): Promise<any> {
    const result = await callCpp<{ viewPoint: any }>(SUITE_NAME, 'ArtworkPointToViewPoint', { view, artworkPoint });
    return result.viewPoint;
}

/**
 * Counts the number of document views objects.
 * @returns The count value
 */
export async function CountDocumentViews(): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountDocumentViews', {  });
    return result.count;
}

/**
 * Retrieves the nth document view of an object.
 * @param n - The N value
 * @returns The view value
 */
export async function GetNthDocumentView(n: number): Promise<any> {
    const result = await callCpp<{ view: any }>(SUITE_NAME, 'GetNthDocumentView', { n });
    return result.view;
}

/**
 * Performs the fixed artwork point to view point operation.
 * @param view - The View value
 * @param artworkPoint - The point coordinates
 * @returns The view point value
 */
export async function FixedArtworkPointToViewPoint(view: any, artworkPoint: AIRealPoint): Promise<AIRealPoint> {
    const result = await callCpp<{ viewPoint: AIRealPoint }>(SUITE_NAME, 'FixedArtworkPointToViewPoint', { view, artworkPoint });
    return result.viewPoint;
}

/**
 * Performs the fixed view point to artwork point operation.
 * @param view - The View value
 * @param viewPoint - The point coordinates
 * @returns The artwork point value
 */
export async function FixedViewPointToArtworkPoint(view: any, viewPoint: AIRealPoint): Promise<AIRealPoint> {
    const result = await callCpp<{ artworkPoint: AIRealPoint }>(SUITE_NAME, 'FixedViewPointToArtworkPoint', { view, viewPoint });
    return result.artworkPoint;
}

/**
 * Sets the screen mode of an object.
 * @param view - The View value
 * @param mode - The Mode value
 */
export async function SetScreenMode(view: any, mode: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SetScreenMode', { view, mode });
}

/**
 * Retrieves the screen mode of an object.
 * @param view - The View value
 * @returns The mode value
 */
export async function GetScreenMode(view: any): Promise<any> {
    const result = await callCpp<{ mode: any }>(SUITE_NAME, 'GetScreenMode', { view });
    return result.mode;
}

/**
 * Retrieves the page tiling of an object.
 * @returns The page tiling value
 */
export async function GetPageTiling(): Promise<any> {
    const result = await callCpp<{ pageTiling: any }>(SUITE_NAME, 'GetPageTiling', {  });
    return result.pageTiling;
}

/**
 * Retrieves the template visible of an object.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function GetTemplateVisible(view: any): Promise<boolean> {
    const result = await callCpp<{ visible: boolean }>(SUITE_NAME, 'GetTemplateVisible', { view });
    return result.visible;
}

/**
 * Performs the document view scroll delta operation.
 * @param view - The View value
 * @returns The delta value
 */
export async function DocumentViewScrollDelta(view: any): Promise<AIRealPoint> {
    const result = await callCpp<{ delta: AIRealPoint }>(SUITE_NAME, 'DocumentViewScrollDelta', { view });
    return result.delta;
}

/**
 * Retrieves the document view invalid rect of an object.
 * @param view - The View value
 * @returns The bounding rectangle
 */
export async function GetDocumentViewInvalidRect(view: any): Promise<AIRealRect> {
    const result = await callCpp<{ invalidRect: AIRealRect }>(SUITE_NAME, 'GetDocumentViewInvalidRect', { view });
    return result.invalidRect;
}

/**
 * Sets the document view invalid rect of an object.
 * @param view - The View value
 * @param invalidRect - The bounding rectangle
 */
export async function SetDocumentViewInvalidRect(view: any, invalidRect: AIRealRect): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewInvalidRect', { view, invalidRect });
}

/**
 * Retrieves the document view style of an object.
 * @param view - The View value
 * @returns The style value
 */
export async function GetDocumentViewStyle(view: any): Promise<number> {
    const result = await callCpp<{ style: number }>(SUITE_NAME, 'GetDocumentViewStyle', { view });
    return result.style;
}

/**
 * Sets the document view invalid document rect of an object.
 * @param view - The View value
 * @param invalidRect - The bounding rectangle
 */
export async function SetDocumentViewInvalidDocumentRect(view: any, invalidRect: AIRealRect): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewInvalidDocumentRect', { view, invalidRect });
}

/**
 * Retrieves the show page tiling of an object.
 * @returns True if the condition is met, false otherwise
 */
export async function GetShowPageTiling(): Promise<boolean> {
    const result = await callCpp<{ show: boolean }>(SUITE_NAME, 'GetShowPageTiling', {  });
    return result.show;
}

/**
 * Sets the show page tiling of an object.
 * @param show - The Show value
 */
export async function SetShowPageTiling(show: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetShowPageTiling', { show });
}

/**
 * Retrieves the grid options of an object.
 * @param view - The View value
 * @returns An object containing: show, snap
 */
export async function GetGridOptions(view: any): Promise<{ show: boolean; snap: boolean }> {
    const result = await callCpp<{ show: boolean; snap: boolean }>(SUITE_NAME, 'GetGridOptions', { view });
    return result;
}

/**
 * Sets the grid options of an object.
 * @param view - The View value
 * @param show - The Show value
 * @param snap - The Snap value
 */
export async function SetGridOptions(view: any, show: boolean, snap: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetGridOptions', { view, show, snap });
}

/**
 * Retrieves the show transparency grid of an object.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function GetShowTransparencyGrid(view: any): Promise<boolean> {
    const result = await callCpp<{ show: boolean }>(SUITE_NAME, 'GetShowTransparencyGrid', { view });
    return result.show;
}

/**
 * Sets the show transparency grid of an object.
 * @param view - The View value
 * @param show - The Show value
 */
export async function SetShowTransparencyGrid(view: any, show: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetShowTransparencyGrid', { view, show });
}

/**
 * Retrieves the document view document of an object.
 * @param view - The View value
 * @returns The document value
 */
export async function GetDocumentViewDocument(view: any): Promise<number> {
    const result = await callCpp<{ document: number }>(SUITE_NAME, 'GetDocumentViewDocument', { view });
    return result.document;
}

/**
 * Performs the force document views on screen operation.
 */
export async function ForceDocumentViewsOnScreen(): Promise<void> {
    await callCpp(SUITE_NAME, 'ForceDocumentViewsOnScreen', {  });
}

/**
 * Retrieves the show guides of an object.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function GetShowGuides(view: any): Promise<boolean> {
    const result = await callCpp<{ show: boolean }>(SUITE_NAME, 'GetShowGuides', { view });
    return result.show;
}

/**
 * Sets the show guides of an object.
 * @param view - The View value
 * @param show - The Show value
 */
export async function SetShowGuides(view: any, show: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetShowGuides', { view, show });
}

/**
 * Retrieves the show edges of an object.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function GetShowEdges(view: any): Promise<boolean> {
    const result = await callCpp<{ show: boolean }>(SUITE_NAME, 'GetShowEdges', { view });
    return result.show;
}

/**
 * Sets the show edges of an object.
 * @param view - The View value
 * @param show - The Show value
 */
export async function SetShowEdges(view: any, show: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetShowEdges', { view, show });
}

/**
 * Performs the save image operation.
 * @param view - The View value
 * @param saveFilename - The name string
 * @param windowSize - The Window Size value
 */
export async function SaveImage(view: any, saveFilename: string, windowSize: any): Promise<void> {
    await callCpp(SUITE_NAME, 'SaveImage', { view, saveFilename, windowSize });
}

/**
 * Checks if is artboard ruler visible.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function IsArtboardRulerVisible(view: any): Promise<boolean> {
    const result = await callCpp<{ visible: boolean }>(SUITE_NAME, 'IsArtboardRulerVisible', { view });
    return result.visible;
}

/**
 * Sets the artboard ruler visible of an object.
 * @param view - The View value
 * @param visible - The visibility flag
 */
export async function SetArtboardRulerVisible(view: any, visible: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetArtboardRulerVisible', { view, visible });
}

/**
 * Counts the number of o p p plates objects.
 * @param view - The View value
 * @returns The count value
 */
export async function CountOPPPlates(view: any): Promise<number> {
    const result = await callCpp<{ count: number }>(SUITE_NAME, 'CountOPPPlates', { view });
    return result.count;
}

/**
 * Sets the document view style of an object.
 * @param view - The View value
 * @param style - The Style value
 * @param mask - The Mask value
 */
export async function SetDocumentViewStyle(view: any, style: number, mask: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewStyle', { view, style, mask });
}

/**
 * Checks if is ruler in artboard coordinates.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function IsRulerInArtboardCoordinates(view: any): Promise<boolean> {
    const result = await callCpp<{ isYes: boolean }>(SUITE_NAME, 'IsRulerInArtboardCoordinates', { view });
    return result.isYes;
}

/**
 * Performs the use artboard coordinates in ruler operation.
 * @param view - The View value
 * @param state - The State value
 */
export async function UseArtboardCoordinatesInRuler(view: any, state: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'UseArtboardCoordinatesInRuler', { view, state });
}

/**
 * Checks if is g p u preview mode on.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function IsGPUPreviewModeOn(view: any): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'IsGPUPreviewModeOn', { view });
    return result.result;
}

/**
 * Checks if is g p u rendering on.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function IsGPURenderingOn(view: any): Promise<boolean> {
    const result = await callCpp<{ result: boolean }>(SUITE_NAME, 'IsGPURenderingOn', { view });
    return result.result;
}

/**
 * Retrieves the g p u pixel of an object.
 * @param window - The Window value
 * @param location - The Location value
 * @returns The color value
 */
export async function GetGPUPixel(window: any, location: any): Promise<any> {
    const result = await callCpp<{ color: any }>(SUITE_NAME, 'GetGPUPixel', { window, location });
    return result.color;
}

/**
 * Retrieves the document view visible area of an object.
 * @param view - The View value
 * @returns The bounding rectangle
 */
export async function GetDocumentViewVisibleArea(view: any): Promise<AIRealRect> {
    const result = await callCpp<{ bounds: AIRealRect }>(SUITE_NAME, 'GetDocumentViewVisibleArea', { view });
    return result.bounds;
}

/**
 * Sets the clip view to artboards of an object.
 * @param view - The View value
 * @param clipToActiveArtboard - The point coordinates
 */
export async function SetClipViewToArtboards(view: any, clipToActiveArtboard: boolean): Promise<void> {
    await callCpp(SUITE_NAME, 'SetClipViewToArtboards', { view, clipToActiveArtboard });
}

/**
 * Retrieves the clip view to artboards of an object.
 * @param view - The View value
 * @returns True if the condition is met, false otherwise
 */
export async function GetClipViewToArtboards(view: any): Promise<boolean> {
    const result = await callCpp<{ clipToActiveArtboard: boolean }>(SUITE_NAME, 'GetClipViewToArtboards', { view });
    return result.clipToActiveArtboard;
}

/**
 * Performs the screen shot operation.
 * @param view - The View value
 * @param saveFilePath - The Save File Path value
 */
export async function ScreenShot(view: any, saveFilePath: string): Promise<void> {
    await callCpp(SUITE_NAME, 'ScreenShot', { view, saveFilePath });
}

/**
 * Sets the document view rotation of an object.
 * @param view - The View value
 * @param rotationPoint - The point coordinates
 * @param rotationAngle - The Rotation Angle value
 */
export async function SetDocumentViewRotation(view: any, rotationPoint: AIRealPoint, rotationAngle: number): Promise<void> {
    await callCpp(SUITE_NAME, 'SetDocumentViewRotation', { view, rotationPoint, rotationAngle });
}

/**
 * Retrieves the document view rotation of an object.
 * @param view - The View value
 * @returns An object containing: rotationPoint, rotationAngle
 */
export async function GetDocumentViewRotation(view: any): Promise<{ rotationPoint: AIRealPoint; rotationAngle: number }> {
    const result = await callCpp<{ rotationPoint: AIRealPoint; rotationAngle: number }>(SUITE_NAME, 'GetDocumentViewRotation', { view });
    return result;
}

/**
 * Performs the reset document view rotation operation.
 * @param view - The View value
 */
export async function ResetDocumentViewRotation(view: any): Promise<void> {
    await callCpp(SUITE_NAME, 'ResetDocumentViewRotation', { view });
}

/**
 * Performs the artwork point to view point unrotated operation.
 * @param view - The View value
 * @param artworkPoint - The point coordinates
 * @returns The view point value
 */
export async function ArtworkPointToViewPointUnrotated(view: any, artworkPoint: AIRealPoint): Promise<any> {
    const result = await callCpp<{ viewPoint: any }>(SUITE_NAME, 'ArtworkPointToViewPointUnrotated', { view, artworkPoint });
    return result.viewPoint;
}

/**
 * Performs the artwork rect to view rect operation.
 * @param view - The View value
 * @param artworkRect - The bounding rectangle
 * @returns The bounding rectangle
 */
export async function ArtworkRectToViewRect(view: any, artworkRect: AIRealRect): Promise<any> {
    const result = await callCpp<{ viewRect: any }>(SUITE_NAME, 'ArtworkRectToViewRect', { view, artworkRect });
    return result.viewRect;
}

/**
 * Performs the artwork rect to view rect unrotated operation.
 * @param view - The View value
 * @param artworkRect - The bounding rectangle
 * @returns The bounding rectangle
 */
export async function ArtworkRectToViewRectUnrotated(view: any, artworkRect: AIRealRect): Promise<any> {
    const result = await callCpp<{ viewRect: any }>(SUITE_NAME, 'ArtworkRectToViewRectUnrotated', { view, artworkRect });
    return result.viewRect;
}

/**
 * Performs the fixed artwork point to view point unrotated operation.
 * @param view - The View value
 * @param artworkPoint - The point coordinates
 * @returns The view point value
 */
export async function FixedArtworkPointToViewPointUnrotated(view: any, artworkPoint: AIRealPoint): Promise<AIRealPoint> {
    const result = await callCpp<{ viewPoint: AIRealPoint }>(SUITE_NAME, 'FixedArtworkPointToViewPointUnrotated', { view, artworkPoint });
    return result.viewPoint;
}

/**
 * Performs the fixed view point to artwork point unrotated operation.
 * @param view - The View value
 * @param viewPoint - The point coordinates
 * @returns The artwork point value
 */
export async function FixedViewPointToArtworkPointUnrotated(view: any, viewPoint: AIRealPoint): Promise<AIRealPoint> {
    const result = await callCpp<{ artworkPoint: AIRealPoint }>(SUITE_NAME, 'FixedViewPointToArtworkPointUnrotated', { view, viewPoint });
    return result.artworkPoint;
}

/**
 * Performs the fixed view rect to artwork rect unrotated operation.
 * @param view - The View value
 * @param viewRect - The bounding rectangle
 * @returns The bounding rectangle
 */
export async function FixedViewRectToArtworkRectUnrotated(view: any, viewRect: AIRealRect): Promise<AIRealRect> {
    const result = await callCpp<{ artworkRect: AIRealRect }>(SUITE_NAME, 'FixedViewRectToArtworkRectUnrotated', { view, viewRect });
    return result.artworkRect;
}

/**
 * Performs the fixed artwork rect to view rect unrotated operation.
 * @param view - The View value
 * @param artworkRect - The bounding rectangle
 * @returns The bounding rectangle
 */
export async function FixedArtworkRectToViewRectUnrotated(view: any, artworkRect: AIRealRect): Promise<AIRealRect> {
    const result = await callCpp<{ viewRect: AIRealRect }>(SUITE_NAME, 'FixedArtworkRectToViewRectUnrotated', { view, artworkRect });
    return result.viewRect;
}
