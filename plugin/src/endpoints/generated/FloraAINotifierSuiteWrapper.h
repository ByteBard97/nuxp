#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AINotifierSuite {

/**
 * Wrapper for AINotifierSuite::GetNotifierActive
 * @param params["notifier"] - AINotifierHandle (handle ID)
 * @returns ["active"] - AIBoolean
 */
nlohmann::json GetNotifierActive(const nlohmann::json& params);

/**
 * Wrapper for AINotifierSuite::SetNotifierActive
 * @param params["notifier"] - AINotifierHandle (handle ID)
 * @param params["active"] - AIBoolean
 */
nlohmann::json SetNotifierActive(const nlohmann::json& params);

/**
 * Wrapper for AINotifierSuite::GetNotifierPlugin
 * @param params["notifier"] - AINotifierHandle (handle ID)
 * @returns ["plugin"] - SPPluginRef
 */
nlohmann::json GetNotifierPlugin(const nlohmann::json& params);

/**
 * Wrapper for AINotifierSuite::CountNotifiers
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountNotifiers(const nlohmann::json& params);

/**
 * Wrapper for AINotifierSuite::GetNthNotifier
 * @param params["n"] - ai::int32
 * @returns ["notifier"] - AINotifierHandle (handle ID)
 */
nlohmann::json GetNthNotifier(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AINotifierSuite
} // namespace Flora
