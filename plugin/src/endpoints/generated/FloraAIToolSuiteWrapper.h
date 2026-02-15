#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIToolSuite {

/**
 * Wrapper for AIToolSuite::GetToolName
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["name"] - char
 */
nlohmann::json GetToolName(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolOptions
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["options"] - ai::int32
 */
nlohmann::json GetToolOptions(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetToolOptions
 * @param params["tool"] - AIToolHandle (handle ID)
 * @param params["options"] - ai::int32
 */
nlohmann::json SetToolOptions(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolPlugin
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["plugin"] - SPPluginRef
 */
nlohmann::json GetToolPlugin(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetSelectedTool
 * @returns ["tool"] - AIToolHandle (handle ID)
 */
nlohmann::json GetSelectedTool(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetSelectedTool
 * @param params["tool"] - AIToolHandle (handle ID)
 */
nlohmann::json SetSelectedTool(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::CountTools
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountTools(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetNthTool
 * @param params["n"] - ai::int32
 * @returns ["tool"] - AIToolHandle (handle ID)
 */
nlohmann::json GetNthTool(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolHandleFromNumber
 * @param params["toolNum"] - AIToolType
 * @returns ["tool"] - AIToolHandle (handle ID)
 */
nlohmann::json GetToolHandleFromNumber(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolNumberFromName
 * @param params["name"] - const char
 * @returns ["toolNum"] - AIToolType
 */
nlohmann::json GetToolNumberFromName(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolNumberFromHandle
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["toolNum"] - AIToolType
 */
nlohmann::json GetToolNumberFromHandle(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolNameFromNumber
 * @param params["toolNum"] - AIToolType
 * @returns ["name"] - char
 */
nlohmann::json GetToolNameFromNumber(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolTitle
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["title"] - ai::UnicodeString
 */
nlohmann::json GetToolTitle(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetToolTitle
 * @param params["tool"] - AIToolHandle (handle ID)
 * @param params["title"] - ai::UnicodeString
 */
nlohmann::json SetToolTitle(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetTooltip
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["tooltip"] - ai::UnicodeString
 */
nlohmann::json GetTooltip(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetTooltip
 * @param params["tool"] - AIToolHandle (handle ID)
 * @param params["tooltip"] - ai::UnicodeString
 */
nlohmann::json SetTooltip(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SystemHasPressure
 * @returns ["hasPressure"] - AIBoolean
 */
nlohmann::json SystemHasPressure(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolNullEventInterval
 * @param params["tool"] - AIToolHandle (handle ID)
 * @returns ["outTime"] - AIToolTime
 */
nlohmann::json GetToolNullEventInterval(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetToolNullEventInterval
 * @param params["tool"] - AIToolHandle (handle ID)
 * @param params["inTime"] - AIToolTime
 */
nlohmann::json SetToolNullEventInterval(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetSoftSelectedTool
 * @param params["tool"] - AIToolHandle (handle ID)
 */
nlohmann::json SetSoftSelectedTool(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::IsSoftModeSelection
 * @returns ["isSoftMode"] - AIBoolean
 */
nlohmann::json IsSoftModeSelection(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetAlternateSelectionToolName
 * @param params["tool"] - AIToolHandle (handle ID)
 * @param params["alternateTool"] - const char
 */
nlohmann::json SetAlternateSelectionToolName(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetCurrentToolNumber
 * @returns ["toolNum"] - AIToolType
 */
nlohmann::json GetCurrentToolNumber(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetCurrentEffectiveTool
 * @returns ["tool"] - AIToolHandle (handle ID)
 */
nlohmann::json GetCurrentEffectiveTool(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetCurrentEffectiveToolNumber
 * @returns ["toolNum"] - AIToolType
 */
nlohmann::json GetCurrentEffectiveToolNumber(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetSelectedToolByName
 * @param params["name"] - const char
 */
nlohmann::json SetSelectedToolByName(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetSelectedToolByNumber
 * @param params["toolNum"] - AIToolType
 */
nlohmann::json SetSelectedToolByNumber(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetTabletHardwareCapabilities
 * @returns ["hardwareCapabilities"] - ai::int32
 */
nlohmann::json GetTabletHardwareCapabilities(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetToolIcons
 * @param params["tool"] - AIToolHandle (handle ID)
 * @param params["normalIconResourceName"] - const char
 * @param params["darkNormalIconResourceName"] - const char
 */
nlohmann::json SetToolIcons(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::SetDocumentInkParams
 * @param params["tool"] - const AIToolHandle (handle ID)
 * @param params["inDocInkParams"] - const AIDocumentInkParams
 */
nlohmann::json SetDocumentInkParams(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolOptionsFromNumber
 * @param params["toolNum"] - AIToolType
 * @returns ["options"] - ai::int32
 */
nlohmann::json GetToolOptionsFromNumber(const nlohmann::json& params);

/**
 * Wrapper for AIToolSuite::GetToolOptionsFromName
 * @param params["toolName"] - const char
 * @returns ["options"] - ai::int32
 */
nlohmann::json GetToolOptionsFromName(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIToolSuite
} // namespace Flora
