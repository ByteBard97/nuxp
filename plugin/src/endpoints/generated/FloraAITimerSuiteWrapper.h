#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AITimerSuite {

/**
 * Wrapper for AITimerSuite::GetTimerName
 * @param params["timer"] - AITimerHandle (handle ID)
 * @returns ["name"] - char
 */
nlohmann::json GetTimerName(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::GetTimerActive
 * @param params["timer"] - AITimerHandle (handle ID)
 * @returns ["active"] - AIBoolean
 */
nlohmann::json GetTimerActive(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::SetTimerActive
 * @param params["timer"] - AITimerHandle (handle ID)
 * @param params["active"] - AIBoolean
 */
nlohmann::json SetTimerActive(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::GetTimerPeriod
 * @param params["timer"] - AITimerHandle (handle ID)
 * @returns ["period"] - ai::int32
 */
nlohmann::json GetTimerPeriod(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::SetTimerPeriod
 * @param params["timer"] - AITimerHandle (handle ID)
 * @param params["period"] - ai::int32
 */
nlohmann::json SetTimerPeriod(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::GetTimerPlugin
 * @param params["timer"] - AITimerHandle (handle ID)
 * @returns ["plugin"] - SPPluginRef
 */
nlohmann::json GetTimerPlugin(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::CountTimers
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountTimers(const nlohmann::json& params);

/**
 * Wrapper for AITimerSuite::GetNthTimer
 * @param params["n"] - ai::int32
 * @returns ["timer"] - AITimerHandle (handle ID)
 */
nlohmann::json GetNthTimer(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AITimerSuite
} // namespace Flora
