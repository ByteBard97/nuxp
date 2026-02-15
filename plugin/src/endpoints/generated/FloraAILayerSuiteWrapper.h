#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AILayerSuite {

/**
 * Wrapper for AILayerSuite::CountLayers
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountLayers(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetNthLayer
 * @param params["n"] - ai::int32
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetNthLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetCurrentLayer
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetCurrentLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetCurrentLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json SetCurrentLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetFirstLayer
 * @returns ["first"] - AILayerHandle (handle ID)
 */
nlohmann::json GetFirstLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetNextLayer
 * @param params["prev"] - AILayerHandle (handle ID)
 * @returns ["next"] - AILayerHandle (handle ID)
 */
nlohmann::json GetNextLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::InsertLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @returns ["newLayer"] - AILayerHandle (handle ID)
 */
nlohmann::json InsertLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::DeleteLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json DeleteLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerTitle
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["title"] - ai::UnicodeString
 */
nlohmann::json GetLayerTitle(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerTitle
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["newTitle"] - const ai::UnicodeString
 */
nlohmann::json SetLayerTitle(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerColor
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["color"] - AIRGBColor
 */
nlohmann::json GetLayerColor(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerVisible
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["visible"] - AIBoolean
 */
nlohmann::json GetLayerVisible(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerVisible
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["visible"] - AIBoolean
 */
nlohmann::json SetLayerVisible(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerPreview
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["preview"] - AIBoolean
 */
nlohmann::json GetLayerPreview(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerPreview
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["preview"] - AIBoolean
 */
nlohmann::json SetLayerPreview(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerEditable
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["editable"] - AIBoolean
 */
nlohmann::json GetLayerEditable(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerEditable
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["editable"] - AIBoolean
 */
nlohmann::json SetLayerEditable(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerPrinted
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["printed"] - AIBoolean
 */
nlohmann::json GetLayerPrinted(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerPrinted
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["printed"] - AIBoolean
 */
nlohmann::json SetLayerPrinted(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerDimPlacedImages
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["dimmed"] - AIBoolean
 */
nlohmann::json GetLayerDimPlacedImages(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerDimPlacedImages
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["dimmed"] - AIBoolean
 */
nlohmann::json SetLayerDimPlacedImages(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerSelected
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["selected"] - AIBoolean
 */
nlohmann::json GetLayerSelected(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerSelected
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["selected"] - AIBoolean
 */
nlohmann::json SetLayerSelected(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerByTitle
 * @param params["title"] - const ai::UnicodeString
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetLayerByTitle(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::LayerHasArt
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["hasArt"] - AIBoolean
 */
nlohmann::json LayerHasArt(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::LayerHasSelectedArt
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["hasSel"] - AIBoolean
 */
nlohmann::json LayerHasSelectedArt(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::DeselectArtOnLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json DeselectArtOnLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SelectArtOnLayer
 * @param params["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json SelectArtOnLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerIsTemplate
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["isTemplate"] - AIBoolean
 */
nlohmann::json GetLayerIsTemplate(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerIsTemplate
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["isTemplate"] - AIBoolean
 */
nlohmann::json SetLayerIsTemplate(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetPrevLayer
 * @param params["next"] - AILayerHandle (handle ID)
 * @returns ["prev"] - AILayerHandle (handle ID)
 */
nlohmann::json GetPrevLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerDimmedPercent
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["percent"] - ai::int32
 */
nlohmann::json GetLayerDimmedPercent(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::SetLayerDimmedPercent
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["percent"] - ai::int32
 */
nlohmann::json SetLayerDimmedPercent(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerFirstChild
 * @param params["layer"] - const AILayerHandle (handle ID)
 * @returns ["child"] - AILayerHandle (handle ID)
 */
nlohmann::json GetLayerFirstChild(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetLayerParent
 * @param params["layer"] - const AILayerHandle (handle ID)
 * @returns ["parent"] - AILayerHandle (handle ID)
 */
nlohmann::json GetLayerParent(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::InsertLayerAtArt
 * @param params["art"] - const AIArtHandle (handle ID)
 * @param params["paintOrder"] - AIPaintOrder
 * @returns ["newLayer"] - AILayerHandle (handle ID)
 */
nlohmann::json InsertLayerAtArt(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::ChangeLayerToGroup
 * @param params["layer"] - const AILayerHandle (handle ID)
 * @param params["group"] - const AIArtHandle (handle ID)
 */
nlohmann::json ChangeLayerToGroup(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetNextPreorderLayer
 * @param params["prev"] - AILayerHandle (handle ID)
 * @returns ["next"] - AILayerHandle (handle ID)
 */
nlohmann::json GetNextPreorderLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerSuite::GetNextNonChildPreorderLayer
 * @param params["prev"] - AILayerHandle (handle ID)
 * @returns ["next"] - AILayerHandle (handle ID)
 */
nlohmann::json GetNextNonChildPreorderLayer(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AILayerSuite
} // namespace Flora
