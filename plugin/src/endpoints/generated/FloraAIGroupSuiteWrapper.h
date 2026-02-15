#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIGroupSuite {

/**
 * Wrapper for AIGroupSuite::GetGroupClipped
 * @param params["group"] - AIArtHandle (handle ID)
 * @returns ["clipped"] - AIBoolean
 */
nlohmann::json GetGroupClipped(const nlohmann::json& params);

/**
 * Wrapper for AIGroupSuite::SetGroupClipped
 * @param params["group"] - AIArtHandle (handle ID)
 * @param params["clipped"] - AIBoolean
 */
nlohmann::json SetGroupClipped(const nlohmann::json& params);

/**
 * Wrapper for AIGroupSuite::GetGroupMaskLock
 * @param params["group"] - AIArtHandle (handle ID)
 * @returns ["maskLocked"] - AIBoolean
 */
nlohmann::json GetGroupMaskLock(const nlohmann::json& params);

/**
 * Wrapper for AIGroupSuite::SetGroupMaskLock
 * @param params["group"] - AIArtHandle (handle ID)
 * @param params["maskLocked"] - AIBoolean
 */
nlohmann::json SetGroupMaskLock(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIGroupSuite
} // namespace Flora
