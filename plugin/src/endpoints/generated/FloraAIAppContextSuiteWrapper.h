#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIAppContextSuite {

/**
 * Wrapper for AIAppContextSuite::GetPlatformAppWindow
 * @returns ["appWindow"] - AIWindowRef
 */
nlohmann::json GetPlatformAppWindow(const nlohmann::json& params);

/**
 * Wrapper for AIAppContextSuite::AllowAllChanges
 * @param params["allowAllChanges"] - ASBoolean
 * @returns ["previousState"] - ASBoolean
 */
nlohmann::json AllowAllChanges(const nlohmann::json& params);

/**
 * Wrapper for AIAppContextSuite::AllowProgress
 * @param params["showProgress"] - bool
 */
nlohmann::json AllowProgress(const nlohmann::json& params);

/**
 * Wrapper for AIAppContextSuite::GetPlatformAppMenu
 * @returns ["appMenu"] - AIAppMenuContext
 */
nlohmann::json GetPlatformAppMenu(const nlohmann::json& params);

/**
 * Wrapper for AIAppContextSuite::SyncAndDraw
 */
nlohmann::json SyncAndDraw(const nlohmann::json& params);

/**
 * Wrapper for AIAppContextSuite::GetRulerWidthOffsetOnActiveDocument
 * @returns ["horzRulerBounds"] - AIRect
 * @returns ["vertRulerBounds"] - AIRect
 * @returns ["cntrRulerBounds"] - AIRect
 */
nlohmann::json GetRulerWidthOffsetOnActiveDocument(const nlohmann::json& params);

/**
 * Wrapper for AIAppContextSuite::IsProgressBarAllowed
 * @returns ["result"] - AIBoolean
 */
nlohmann::json IsProgressBarAllowed(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIAppContextSuite
} // namespace Flora
