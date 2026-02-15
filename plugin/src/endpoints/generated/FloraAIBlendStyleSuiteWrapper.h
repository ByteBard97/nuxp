#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIBlendStyleSuite {

/**
 * Wrapper for AIBlendStyleSuite::GetBlendingMode
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - AIBlendingMode value
 */
nlohmann::json GetBlendingMode(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetOpacity
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - AIReal value
 */
nlohmann::json GetOpacity(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetOpacity
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["opacity"] - AIReal
 */
nlohmann::json SetOpacity(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetIsolated
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetIsolated(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetIsolated
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["isolated"] - AIBoolean
 */
nlohmann::json SetIsolated(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetKnockout
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - AIKnockout value
 */
nlohmann::json GetKnockout(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetInheritedKnockout
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - AIKnockout value
 */
nlohmann::json GetInheritedKnockout(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetAlphaIsShape
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetAlphaIsShape(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetAlphaIsShape
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["alphaIsShape"] - AIBoolean
 */
nlohmann::json SetAlphaIsShape(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::Copy
 * @param params["source"] - const AIArtHandle (handle ID)
 * @param params["destination"] - AIArtHandle (handle ID)
 */
nlohmann::json Copy(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetArtAttrs
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json GetArtAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetArtAttrs
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json SetArtAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetStyleAttrs
 * @param params["style"] - AIArtStyleHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json GetStyleAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetStyleAttrs
 * @param params["style"] - AIArtStyleHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 * @returns ["newStyle"] - AIArtStyleHandle (handle ID)
 */
nlohmann::json SetStyleAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetCurrentTransparency
 * @param params["styleAttrs"] - AIDictionaryRef (handle ID)
 * @param params["fillAttrs"] - AIDictionaryRef (handle ID)
 * @param params["strokeAttrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json GetCurrentTransparency(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetCurrentTransparency
 * @param params["styleAttrs"] - AIDictionaryRef (handle ID)
 * @param params["fillAttrs"] - AIDictionaryRef (handle ID)
 * @param params["strokeAttrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json SetCurrentTransparency(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetFocalFillAttrs
 * @param params["artStyle"] - AIArtStyleHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json GetFocalFillAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetFocalStrokeAttrs
 * @param params["artStyle"] - AIArtStyleHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 */
nlohmann::json GetFocalStrokeAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetFocalFillAttrs
 * @param params["artStyle"] - AIArtStyleHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 * @returns ["newStyle"] - AIArtStyleHandle (handle ID)
 */
nlohmann::json SetFocalFillAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetFocalStrokeAttrs
 * @param params["artStyle"] - AIArtStyleHandle (handle ID)
 * @param params["attrs"] - AIDictionaryRef (handle ID)
 * @returns ["newStyle"] - AIArtStyleHandle (handle ID)
 */
nlohmann::json SetFocalStrokeAttrs(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::ContainsNonIsolatedBlending
 * @param params["object"] - AIArtHandle (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json ContainsNonIsolatedBlending(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetDocumentIsolated
 * @param params["void"] - void
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetDocumentIsolated(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetDocumentIsolated
 * @param params["isolated"] - AIBoolean
 */
nlohmann::json SetDocumentIsolated(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::GetDocumentKnockout
 * @param params["void"] - void
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetDocumentKnockout(const nlohmann::json& params);

/**
 * Wrapper for AIBlendStyleSuite::SetDocumentKnockout
 * @param params["knockout"] - AIBoolean
 */
nlohmann::json SetDocumentKnockout(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIBlendStyleSuite
} // namespace Flora
