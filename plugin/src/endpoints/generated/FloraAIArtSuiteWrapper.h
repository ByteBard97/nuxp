#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIArtSuite {

/**
 * Wrapper for AIArtSuite::NewArt
 * @param params["type"] - ai::int16
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 * @returns ["newArt"] - AIArtHandle (handle ID)
 */
nlohmann::json NewArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::DisposeArt
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json DisposeArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ReorderArt
 * @param params["thisArt"] - AIArtHandle (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 */
nlohmann::json ReorderArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::DuplicateArt
 * @param params["thisArt"] - AIArtHandle (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 * @returns ["newArt"] - AIArtHandle (handle ID)
 */
nlohmann::json DuplicateArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetFirstArtOfLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json GetFirstArtOfLayer(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetLayerOfArt
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetLayerOfArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtType
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["type"] - short
 */
nlohmann::json GetArtType(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtUserAttr
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["whichAttr"] - ai::int32
 * @returns ["attr"] - ai::int32
 */
nlohmann::json GetArtUserAttr(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetArtUserAttr
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["whichAttr"] - ai::int32
 * @param params["attr"] - ai::int32
 */
nlohmann::json SetArtUserAttr(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtParent
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["parent"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtParent(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtFirstChild
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["child"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtFirstChild(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtSibling
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["sibling"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtSibling(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtPriorSibling
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["sibling"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtPriorSibling(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtBounds
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetArtBounds(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetArtBounds
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json SetArtBounds(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtCenterPointVisible
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["visible"] - AIBoolean
 */
nlohmann::json GetArtCenterPointVisible(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetArtCenterPointVisible
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["visible"] - AIBoolean
 */
nlohmann::json SetArtCenterPointVisible(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtTransformBounds
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["flags"] - ai::int32
 * @returns ["transform"] - AIRealMatrix
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetArtTransformBounds(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::UpdateArtworkLink
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["force"] - AIBoolean
 * @returns ["updated"] - AIBoolean
 */
nlohmann::json UpdateArtworkLink(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ValidArt
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["searchAllLayerLists"] - AIBoolean
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json ValidArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtOrder
 * @param params["art1"] - AIArtHandle (handle ID)
 * @param params["art2"] - AIArtHandle (handle ID)
 * @returns ["order"] - short
 */
nlohmann::json GetArtOrder(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SelectNamedArtOfLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["name"] - const ai::UnicodeString
 * @param params["matchWholeWord"] - AIBoolean
 * @param params["caseSensitive"] - AIBoolean
 */
nlohmann::json SelectNamedArtOfLayer(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtRotatedBounds
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["angle"] - AIReal
 * @param params["flags"] - ai::int32
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetArtRotatedBounds(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ArtHasFill
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json ArtHasFill(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ArtHasStroke
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json ArtHasStroke(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ArtCopyFillStyleIfEqualPaths
 * @param params["dstArt"] - AIArtHandle (handle ID)
 * @param params["srcArt"] - AIArtHandle (handle ID)
 */
nlohmann::json ArtCopyFillStyleIfEqualPaths(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ArtCopyStrokeStyleIfEqualPaths
 * @param params["dstArt"] - AIArtHandle (handle ID)
 * @param params["srcArt"] - AIArtHandle (handle ID)
 */
nlohmann::json ArtCopyStrokeStyleIfEqualPaths(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetInsertionPoint
 * @returns ["art"] - AIArtHandle (handle ID)
 * @returns ["paintorder"] - short
 * @returns ["editable"] - AIBoolean
 */
nlohmann::json GetInsertionPoint(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetInsertionPoint
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json SetInsertionPoint(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetKeyArt
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json GetKeyArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::HasDictionary
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json HasDictionary(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::IsDictionaryEmpty
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsDictionaryEmpty(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetArtName
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["name"] - const ai::UnicodeString
 */
nlohmann::json SetArtName(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtName
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["name"] - ai::UnicodeString
 * @returns ["isDefaultName"] - ASBoolean
 */
nlohmann::json GetArtName(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::IsArtLayerGroup
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["isLayerGroup"] - ASBoolean
 */
nlohmann::json IsArtLayerGroup(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ReleaseToLayers
 * @param params["art"] - const AIArtHandle (handle ID)
 * @param params["build"] - ASBoolean
 */
nlohmann::json ReleaseToLayers(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ModifyTargetedArtSet
 * @param params["count"] - ai::int32
 * @param params["action"] - ai::int32
 * @returns ["list"] - AIArtHandle (handle ID)
 */
nlohmann::json ModifyTargetedArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::IsArtStyledArt
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsArtStyledArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::IsArtClipping
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsArtClipping(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::TransferAttributes
 * @param params["srcart"] - AIArtHandle (handle ID)
 * @param params["dstart"] - AIArtHandle (handle ID)
 * @param params["which"] - ai::uint32
 */
nlohmann::json TransferAttributes(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtLastChild
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["child"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtLastChild(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetArtTextWrapProperty
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["offset"] - AIReal
 * @param params["invert"] - AIBoolean
 */
nlohmann::json SetArtTextWrapProperty(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtTextWrapProperty
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["offset"] - AIReal
 * @returns ["invert"] - AIBoolean
 */
nlohmann::json GetArtTextWrapProperty(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::CreateCopyScope
 * @param params["kind"] - enum AICopyScopeKind
 * @returns ["scope"] - AICopyScopeHandle
 */
nlohmann::json CreateCopyScope(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::DestroyCopyScope
 * @param params["scope"] - AICopyScopeHandle
 */
nlohmann::json DestroyCopyScope(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::InsertionPointBadForArtType
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 * @param params["artType"] - ai::int16
 */
nlohmann::json InsertionPointBadForArtType(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::PreinsertionFlightCheck
 * @param params["candidateArt"] - AIArtHandle (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 */
nlohmann::json PreinsertionFlightCheck(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetNote
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["inNote"] - const ai::UnicodeString
 */
nlohmann::json SetNote(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetNote
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["outNote"] - ai::UnicodeString
 */
nlohmann::json GetNote(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::HasNote
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json HasNote(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtXMPSize
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["size"] - size_t
 */
nlohmann::json GetArtXMPSize(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetArtXMP
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["xmp"] - const char
 */
nlohmann::json SetArtXMP(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetPreciseArtTransformBounds
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["flags"] - ai::int32
 * @returns ["transform"] - AIRealMatrix
 * @returns ["bounds"] - AIDoubleRect
 */
nlohmann::json GetPreciseArtTransformBounds(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::UncheckedDisposeArt
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json UncheckedDisposeArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ArtIsGraph
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["artisgraph"] - AIBoolean
 */
nlohmann::json ArtIsGraph(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetKeyArt
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json SetKeyArt(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetDrawingMode
 * @returns ["mode"] - ai::int32
 */
nlohmann::json GetDrawingMode(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetDrawingMode
 * @param params["mode"] - ai::int32
 */
nlohmann::json SetDrawingMode(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetInsertionPointForDrawingMode
 * @param params["mode"] - ai::int32
 * @returns ["art"] - AIArtHandle (handle ID)
 * @returns ["paintorder"] - short
 * @returns ["editable"] - AIBoolean
 */
nlohmann::json GetInsertionPointForDrawingMode(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetInsertionPointForCurrentDrawingMode
 * @returns ["art"] - AIArtHandle (handle ID)
 * @returns ["paintorder"] - short
 * @returns ["editable"] - AIBoolean
 */
nlohmann::json GetInsertionPointForCurrentDrawingMode(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetPathPolarity
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["polarity"] - ai::int32
 */
nlohmann::json GetPathPolarity(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::IsPixelPerfect
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsPixelPerfect(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::SetPixelPerfect
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["isPixelPerfect"] - AIBoolean
 */
nlohmann::json SetPixelPerfect(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::IsArtALayerInSymbol
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["isLayerInSymbol"] - AIBoolean
 */
nlohmann::json IsArtALayerInSymbol(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtTimeStamp
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["option"] - enum AIArtTimeStampOptions
 * @returns ["timeStamp"] - size_t
 */
nlohmann::json GetArtTimeStamp(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ConvertPointTypeToAreaType
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["newArtHandle"] - AIArtHandle (handle ID)
 */
nlohmann::json ConvertPointTypeToAreaType(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::ConvertAreaTypeToPointType
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["newArtHandle"] - AIArtHandle (handle ID)
 */
nlohmann::json ConvertAreaTypeToPointType(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::MarkDirty
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["markStyleDirty"] - AIBoolean
 */
nlohmann::json MarkDirty(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetSafeArtHandle
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["safeArt"] - AISafeArtHandle
 */
nlohmann::json GetSafeArtHandle(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtHandle
 * @param params["safeArt"] - AISafeArtHandle
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtHandle(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetArtDefaultName
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["name"] - ai::UnicodeString
 */
nlohmann::json GetArtDefaultName(const nlohmann::json& params);

/**
 * Wrapper for AIArtSuite::GetDocumentOfArt
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["document"] - AIDocumentHandle (handle ID)
 */
nlohmann::json GetDocumentOfArt(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIArtSuite
} // namespace Flora
