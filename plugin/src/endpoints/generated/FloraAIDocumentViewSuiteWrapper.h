#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIDocumentViewSuite {

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewBounds
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetDocumentViewBounds(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewCenter
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["center"] - AIRealPoint
 */
nlohmann::json GetDocumentViewCenter(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewCenter
 * @param params["view"] - AIDocumentViewHandle
 * @param params["center"] - const AIRealPoint
 */
nlohmann::json SetDocumentViewCenter(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewUserVisibleZoom
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["zoom"] - AIReal
 */
nlohmann::json GetDocumentViewUserVisibleZoom(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewZoom
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["zoom"] - AIReal
 */
nlohmann::json GetDocumentViewZoom(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewZoom
 * @param params["view"] - AIDocumentViewHandle
 * @param params["zoom"] - AIReal
 */
nlohmann::json SetDocumentViewZoom(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewUserVisibleZoom
 * @param params["view"] - AIDocumentViewHandle
 * @param params["zoom"] - AIReal
 */
nlohmann::json SetDocumentViewUserVisibleZoom(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ArtworkPointToViewPoint
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkPoint"] - const AIRealPoint
 * @returns ["viewPoint"] - AIPoint
 */
nlohmann::json ArtworkPointToViewPoint(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::CountDocumentViews
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountDocumentViews(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetNthDocumentView
 * @param params["n"] - ai::int32
 * @returns ["view"] - AIDocumentViewHandle
 */
nlohmann::json GetNthDocumentView(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::FixedArtworkPointToViewPoint
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkPoint"] - const AIRealPoint
 * @returns ["viewPoint"] - AIRealPoint
 */
nlohmann::json FixedArtworkPointToViewPoint(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::FixedViewPointToArtworkPoint
 * @param params["view"] - AIDocumentViewHandle
 * @param params["viewPoint"] - const AIRealPoint
 * @returns ["artworkPoint"] - AIRealPoint
 */
nlohmann::json FixedViewPointToArtworkPoint(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetScreenMode
 * @param params["view"] - AIDocumentViewHandle
 * @param params["mode"] - AIScreenMode
 */
nlohmann::json SetScreenMode(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetScreenMode
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["mode"] - AIScreenMode
 */
nlohmann::json GetScreenMode(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetPageTiling
 * @returns ["pageTiling"] - AIPageTiling
 */
nlohmann::json GetPageTiling(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetTemplateVisible
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["visible"] - AIBoolean
 */
nlohmann::json GetTemplateVisible(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::DocumentViewScrollDelta
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["delta"] - AIRealPoint
 */
nlohmann::json DocumentViewScrollDelta(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewInvalidRect
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["invalidRect"] - AIRealRect
 */
nlohmann::json GetDocumentViewInvalidRect(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewInvalidRect
 * @param params["view"] - AIDocumentViewHandle
 * @param params["invalidRect"] - const AIRealRect
 */
nlohmann::json SetDocumentViewInvalidRect(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewStyle
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["style"] - ai::int16
 */
nlohmann::json GetDocumentViewStyle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewInvalidDocumentRect
 * @param params["view"] - AIDocumentViewHandle
 * @param params["invalidRect"] - const AIRealRect
 */
nlohmann::json SetDocumentViewInvalidDocumentRect(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetShowPageTiling
 * @returns ["show"] - AIBoolean
 */
nlohmann::json GetShowPageTiling(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetShowPageTiling
 * @param params["show"] - AIBoolean
 */
nlohmann::json SetShowPageTiling(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetGridOptions
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["show"] - AIBoolean
 * @returns ["snap"] - AIBoolean
 */
nlohmann::json GetGridOptions(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetGridOptions
 * @param params["view"] - AIDocumentViewHandle
 * @param params["show"] - AIBoolean
 * @param params["snap"] - AIBoolean
 */
nlohmann::json SetGridOptions(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetShowTransparencyGrid
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["show"] - AIBoolean
 */
nlohmann::json GetShowTransparencyGrid(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetShowTransparencyGrid
 * @param params["view"] - AIDocumentViewHandle
 * @param params["show"] - AIBoolean
 */
nlohmann::json SetShowTransparencyGrid(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewDocument
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["document"] - AIDocumentHandle (handle ID)
 */
nlohmann::json GetDocumentViewDocument(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ForceDocumentViewsOnScreen
 * @param params["void"] - void
 */
nlohmann::json ForceDocumentViewsOnScreen(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetShowGuides
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["show"] - AIBoolean
 */
nlohmann::json GetShowGuides(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetShowGuides
 * @param params["view"] - AIDocumentViewHandle
 * @param params["show"] - AIBoolean
 */
nlohmann::json SetShowGuides(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetShowEdges
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["show"] - AIBoolean
 */
nlohmann::json GetShowEdges(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetShowEdges
 * @param params["view"] - AIDocumentViewHandle
 * @param params["show"] - AIBoolean
 */
nlohmann::json SetShowEdges(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SaveImage
 * @param params["view"] - AIDocumentViewHandle
 * @param params["saveFilename"] - const ai::UnicodeString
 * @param params["windowSize"] - const AIPoint
 */
nlohmann::json SaveImage(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::IsArtboardRulerVisible
 * @param params["view"] - const AIDocumentViewHandle
 * @returns ["visible"] - AIBoolean
 */
nlohmann::json IsArtboardRulerVisible(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetArtboardRulerVisible
 * @param params["view"] - const AIDocumentViewHandle
 * @param params["visible"] - const AIBoolean
 */
nlohmann::json SetArtboardRulerVisible(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::CountOPPPlates
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountOPPPlates(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewStyle
 * @param params["view"] - AIDocumentViewHandle
 * @param params["style"] - ai::int16
 * @param params["mask"] - ai::int16
 */
nlohmann::json SetDocumentViewStyle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::IsRulerInArtboardCoordinates
 * @param params["view"] - const AIDocumentViewHandle
 * @returns ["isYes"] - ASBoolean
 */
nlohmann::json IsRulerInArtboardCoordinates(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::UseArtboardCoordinatesInRuler
 * @param params["view"] - const AIDocumentViewHandle
 * @param params["state"] - const ASBoolean
 */
nlohmann::json UseArtboardCoordinatesInRuler(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::IsGPUPreviewModeOn
 * @param params["view"] - const AIDocumentViewHandle
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsGPUPreviewModeOn(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::IsGPURenderingOn
 * @param params["view"] - const AIDocumentViewHandle
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsGPURenderingOn(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewVisibleArea
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetDocumentViewVisibleArea(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetClipViewToArtboards
 * @param params["view"] - AIDocumentViewHandle
 * @param params["clipToActiveArtboard"] - AIBoolean
 */
nlohmann::json SetClipViewToArtboards(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetClipViewToArtboards
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["clipToActiveArtboard"] - AIBoolean
 */
nlohmann::json GetClipViewToArtboards(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ScreenShot
 * @param params["view"] - AIDocumentViewHandle
 * @param params["saveFilePath"] - const ai::UnicodeString
 */
nlohmann::json ScreenShot(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::SetDocumentViewRotation
 * @param params["view"] - AIDocumentViewHandle
 * @param params["rotationPoint"] - const AIRealPoint
 * @param params["rotationAngle"] - AIReal
 */
nlohmann::json SetDocumentViewRotation(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::GetDocumentViewRotation
 * @param params["view"] - AIDocumentViewHandle
 * @returns ["rotationPoint"] - AIRealPoint
 * @returns ["rotationAngle"] - AIReal
 */
nlohmann::json GetDocumentViewRotation(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ResetDocumentViewRotation
 * @param params["view"] - AIDocumentViewHandle
 */
nlohmann::json ResetDocumentViewRotation(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ArtworkPointToViewPointUnrotated
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkPoint"] - const AIRealPoint
 * @returns ["viewPoint"] - AIPoint
 */
nlohmann::json ArtworkPointToViewPointUnrotated(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ArtworkRectToViewRect
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkRect"] - const AIRealRect
 * @returns ["viewRect"] - AIRect
 */
nlohmann::json ArtworkRectToViewRect(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::ArtworkRectToViewRectUnrotated
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkRect"] - const AIRealRect
 * @returns ["viewRect"] - AIRect
 */
nlohmann::json ArtworkRectToViewRectUnrotated(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::FixedArtworkPointToViewPointUnrotated
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkPoint"] - const AIRealPoint
 * @returns ["viewPoint"] - AIRealPoint
 */
nlohmann::json FixedArtworkPointToViewPointUnrotated(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::FixedViewPointToArtworkPointUnrotated
 * @param params["view"] - AIDocumentViewHandle
 * @param params["viewPoint"] - const AIRealPoint
 * @returns ["artworkPoint"] - AIRealPoint
 */
nlohmann::json FixedViewPointToArtworkPointUnrotated(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::FixedViewRectToArtworkRectUnrotated
 * @param params["view"] - AIDocumentViewHandle
 * @param params["viewRect"] - const AIRealRect
 * @returns ["artworkRect"] - AIRealRect
 */
nlohmann::json FixedViewRectToArtworkRectUnrotated(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentViewSuite::FixedArtworkRectToViewRectUnrotated
 * @param params["view"] - AIDocumentViewHandle
 * @param params["artworkRect"] - const AIRealRect
 * @returns ["viewRect"] - AIRealRect
 */
nlohmann::json FixedArtworkRectToViewRectUnrotated(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIDocumentViewSuite
} // namespace Flora
