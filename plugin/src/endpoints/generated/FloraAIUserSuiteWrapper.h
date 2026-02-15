#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIUserSuite {

/**
 * Wrapper for AIUserSuite::IUAIRealToStringUnits
 * @param params["value"] - AIReal
 * @param params["precision"] - ai::int32
 * @returns ["string"] - ai::UnicodeString
 */
nlohmann::json IUAIRealToStringUnits(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::IUAIRealToStringUnitsWithoutScale
 * @param params["value"] - AIReal
 * @param params["precision"] - ai::int32
 * @returns ["string"] - ai::UnicodeString
 */
nlohmann::json IUAIRealToStringUnitsWithoutScale(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::GetUnitsString
 * @param params["format"] - ai::int16
 * @returns ["string"] - ai::UnicodeString
 */
nlohmann::json GetUnitsString(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::GetGlobalObjectDisplayName
 * @returns ["name"] - ai::UnicodeString
 */
nlohmann::json GetGlobalObjectDisplayName(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::EditInOriginalApp
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json EditInOriginalApp(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::BuildDirectoryMenu
 * @param params["menu"] - AIPlatformMenuHandle
 * @param params["fileSpec"] - const ai::FilePath
 */
nlohmann::json BuildDirectoryMenu(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::GetIndexedDirectorySpec
 * @param params["index"] - ai::int32
 * @returns ["fileSpec"] - ai::FilePath
 */
nlohmann::json GetIndexedDirectorySpec(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::RevealTheFile
 * @param params["fileSpec"] - const ai::FilePath
 */
nlohmann::json RevealTheFile(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::GetDateAndTime
 * @returns ["outValue"] - AIUserDateTime
 */
nlohmann::json GetDateAndTime(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::LaunchApp
 * @param params["spFileSpec"] - const ai::FilePath
 * @param params["openDoc"] - ASBoolean
 */
nlohmann::json LaunchApp(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::SameName
 * @param params["name1"] - const ai::UnicodeString
 * @param params["name2"] - const ai::UnicodeString
 * @returns ["same"] - AIBoolean
 */
nlohmann::json SameName(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::GetDirectoryDialog
 * @param params["title"] - const ai::UnicodeString
 * @returns ["ioFilePath"] - ai::FilePath
 */
nlohmann::json GetDirectoryDialog(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::EvaluateExpression
 * @param params["expr"] - const ai::UnicodeString
 * @param params["options"] - const AIExpressionOptions
 * @returns ["evaluatedExpr"] - ai::UnicodeString
 * @returns ["isChanged"] - AIBoolean
 * @returns ["numericValue"] - AIDouble
 */
nlohmann::json EvaluateExpression(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::EvaluateExpressionWithoutScale
 * @param params["expr"] - const ai::UnicodeString
 * @param params["options"] - const AIExpressionOptions
 * @returns ["evaluatedExpr"] - ai::UnicodeString
 * @returns ["isChanged"] - AIBoolean
 * @returns ["numericValue"] - AIDouble
 */
nlohmann::json EvaluateExpressionWithoutScale(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::SetCursor
 * @param params["cursorID"] - ai::int32
 * @param params["inRscMgr"] - AIResourceManagerHandle
 */
nlohmann::json SetCursor(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::SetSVGCursor
 * @param params["cursorID"] - ai::int32
 * @param params["inRscMgr"] - AIResourceManagerHandle
 */
nlohmann::json SetSVGCursor(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::GetAILanguageCode
 * @returns ["lang"] - ai::UnicodeString
 */
nlohmann::json GetAILanguageCode(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::LaunchFolder
 * @param params["folderPath"] - ai::FilePath
 */
nlohmann::json LaunchFolder(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::LaunchCustomAppForCustomUri
 * @param params["customUri"] - const ai::UnicodeString
 * @param params["appPath"] - const ai::FilePath
 */
nlohmann::json LaunchCustomAppForCustomUri(const nlohmann::json& params);

/**
 * Wrapper for AIUserSuite::EditInCustomApp
 * @param params["art"] - AIArtHandle (handle ID)
 * @param params["appPath"] - const ai::FilePath
 */
nlohmann::json EditInCustomApp(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIUserSuite
} // namespace Flora
