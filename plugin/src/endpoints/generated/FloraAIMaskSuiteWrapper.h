#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIMaskSuite {

/**
 * Wrapper for AIMaskSuite::GetMask
 * @param params["object"] - AIArtHandle (handle ID)
 * @returns ["mask"] - AIMaskRef (handle ID)
 */
nlohmann::json GetMask(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::CreateMask
 * @param params["object"] - AIArtHandle (handle ID)
 */
nlohmann::json CreateMask(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::DeleteMask
 * @param params["object"] - AIArtHandle (handle ID)
 */
nlohmann::json DeleteMask(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::GetLinked
 * @param params["mask"] - AIMaskRef (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetLinked(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::SetLinked
 * @param params["mask"] - AIMaskRef (handle ID)
 * @param params["linked"] - AIBoolean
 */
nlohmann::json SetLinked(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::GetDisabled
 * @param params["mask"] - AIMaskRef (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetDisabled(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::SetDisabled
 * @param params["mask"] - AIMaskRef (handle ID)
 * @param params["disabled"] - AIBoolean
 */
nlohmann::json SetDisabled(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::GetInverted
 * @param params["mask"] - AIMaskRef (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetInverted(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::SetInverted
 * @param params["mask"] - AIMaskRef (handle ID)
 * @param params["inverted"] - AIBoolean
 */
nlohmann::json SetInverted(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::Copy
 * @param params["source"] - const AIArtHandle (handle ID)
 * @param params["destination"] - AIArtHandle (handle ID)
 */
nlohmann::json Copy(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::GetArt
 * @param params["mask"] - AIMaskRef (handle ID)
 * @returns ["result"] - handle ID (from AIArtHandle return)
 */
nlohmann::json GetArt(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::IsEditingArt
 * @param params["mask"] - AIMaskRef (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json IsEditingArt(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::SetEditingArt
 * @param params["mask"] - AIMaskRef (handle ID)
 * @param params["isedit"] - AIBoolean
 */
nlohmann::json SetEditingArt(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::GetMaskedArt
 * @param params["mask"] - AIArtHandle (handle ID)
 * @returns ["masked"] - AIArtHandle (handle ID)
 */
nlohmann::json GetMaskedArt(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::GetClipping
 * @param params["mask"] - AIMaskRef (handle ID)
 * @returns ["result"] - bool (from AIBoolean return)
 */
nlohmann::json GetClipping(const nlohmann::json& params);

/**
 * Wrapper for AIMaskSuite::SetClipping
 * @param params["mask"] - AIMaskRef (handle ID)
 * @param params["clipping"] - AIBoolean
 */
nlohmann::json SetClipping(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIMaskSuite
} // namespace Flora
