#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AITransformArtSuite {

/**
 * Wrapper for AITransformArtSuite::TransformArt
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["lineScale"] - AIReal
 * @param params["flags"] - ai::int32
 * @returns ["matrix"] - AIRealMatrix
 */
nlohmann::json TransformArt(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AITransformArtSuite
} // namespace Flora
