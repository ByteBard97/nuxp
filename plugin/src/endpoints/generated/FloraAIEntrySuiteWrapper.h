#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIEntrySuite {

/**
 * Wrapper for AIEntrySuite::ToBoolean
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - ASBoolean
 */
nlohmann::json ToBoolean(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToInteger
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - ai::int32
 */
nlohmann::json ToInteger(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToReal
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIReal
 */
nlohmann::json ToReal(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToRealPoint
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIRealPoint
 */
nlohmann::json ToRealPoint(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToRealMatrix
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIRealMatrix
 */
nlohmann::json ToRealMatrix(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToDict
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIDictionaryRef (handle ID)
 */
nlohmann::json ToDict(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToArt
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json ToArt(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToArray
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIArrayRef (handle ID)
 */
nlohmann::json ToArray(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToCustomColor
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AICustomColorHandle (handle ID)
 */
nlohmann::json ToCustomColor(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToPluginObject
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIObjectHandle
 */
nlohmann::json ToPluginObject(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToFillStyle
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - struct AIFillStyle
 */
nlohmann::json ToFillStyle(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToStrokeStyle
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - struct AIStrokeStyle
 */
nlohmann::json ToStrokeStyle(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToUID
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIUIDRef
 */
nlohmann::json ToUID(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToUIDREF
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIUIDREFRef
 */
nlohmann::json ToUIDREF(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToXMLNode
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIXMLNodeRef
 */
nlohmann::json ToXMLNode(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToSVGFilterHandle
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AISVGFilterHandle
 */
nlohmann::json ToSVGFilterHandle(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::AsBoolean
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - ASBoolean
 */
nlohmann::json AsBoolean(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::AsInteger
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - ai::int32
 */
nlohmann::json AsInteger(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::AsReal
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIReal
 */
nlohmann::json AsReal(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::AsUIDREF
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIUIDREFRef
 */
nlohmann::json AsUIDREF(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToArtStyle
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - AIArtStyleHandle (handle ID)
 */
nlohmann::json ToArtStyle(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::ToUnicodeString
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - ai::UnicodeString
 */
nlohmann::json ToUnicodeString(const nlohmann::json& params);

/**
 * Wrapper for AIEntrySuite::AsUnicodeString
 * @param params["entry"] - AIEntryRef (handle ID)
 * @returns ["value"] - ai::UnicodeString
 */
nlohmann::json AsUnicodeString(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIEntrySuite
} // namespace Flora
