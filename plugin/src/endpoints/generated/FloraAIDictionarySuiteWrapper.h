#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIDictionarySuite {

/**
 * Wrapper for AIDictionarySuite::CreateDictionary
 * @returns ["dictionary"] - AIDictionaryRef (handle ID)
 */
nlohmann::json CreateDictionary(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::CreateDictionaryFromJSONFile
 * @param params["file"] - const ai::FilePath
 * @returns ["dictionary"] - AIDictionaryRef (handle ID)
 */
nlohmann::json CreateDictionaryFromJSONFile(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::Clone
 * @param params["src"] - ConstAIDictionaryRef (handle ID)
 * @returns ["dst"] - AIDictionaryRef (handle ID)
 */
nlohmann::json Clone(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::Copy
 * @param params["dict"] - AIDictionaryRef (handle ID)
 * @param params["src"] - ConstAIDictionaryRef (handle ID)
 */
nlohmann::json Copy(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::Begin
 * @param params["dict"] - ConstAIDictionaryRef (handle ID)
 * @returns ["iterator"] - AIDictionaryIterator (handle ID)
 */
nlohmann::json Begin(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::DeleteEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 */
nlohmann::json DeleteEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetEntryType
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["entryType"] - AIEntryType
 */
nlohmann::json GetEntryType(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::CopyEntry
 * @param params["dictionary1"] - ConstAIDictionaryRef (handle ID)
 * @param params["dictionary2"] - AIDictionaryRef (handle ID)
 * @param params["key1"] - AIDictKey (handle ID)
 * @param params["key2"] - AIDictKey (handle ID)
 */
nlohmann::json CopyEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::MoveEntry
 * @param params["dictionary1"] - AIDictionaryRef (handle ID)
 * @param params["dictionary2"] - AIDictionaryRef (handle ID)
 * @param params["key1"] - AIDictKey (handle ID)
 * @param params["key2"] - AIDictKey (handle ID)
 */
nlohmann::json MoveEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SwapEntries
 * @param params["dictionary1"] - AIDictionaryRef (handle ID)
 * @param params["dictionary2"] - AIDictionaryRef (handle ID)
 * @param params["key1"] - AIDictKey (handle ID)
 * @param params["key2"] - AIDictKey (handle ID)
 */
nlohmann::json SwapEntries(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetArtEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json GetArtEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::NewArtEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["type"] - ai::int16
 */
nlohmann::json NewArtEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::MoveArtToEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json MoveArtToEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::MoveEntryToArt
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json MoveEntryToArt(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::CopyArtToEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json CopyArtToEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::CopyEntryToArt
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AIArtHandle (handle ID, optional)
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json CopyEntryToArt(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetEntryToLayer
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json SetEntryToLayer(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetLayerToEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["paintOrder"] - ai::int16
 * @param params["prep"] - AILayerHandle (handle ID, optional)
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json SetLayerToEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::Set
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["entry"] - AIEntryRef (handle ID)
 */
nlohmann::json Set(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetBooleanEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - AIBoolean
 */
nlohmann::json GetBooleanEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetBooleanEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - AIBoolean
 */
nlohmann::json SetBooleanEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetIntegerEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - ai::int32
 */
nlohmann::json GetIntegerEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetIntegerEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - ai::int32
 */
nlohmann::json SetIntegerEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetPointerEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - ai::intptr
 */
nlohmann::json GetPointerEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetPointerEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - ai::intptr
 */
nlohmann::json SetPointerEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetRealEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - AIReal
 */
nlohmann::json GetRealEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetRealEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - AIReal
 */
nlohmann::json SetRealEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetStringEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - const char
 */
nlohmann::json SetStringEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetDictEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - AIDictionaryRef (handle ID)
 */
nlohmann::json GetDictEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetDictEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - AIDictionaryRef (handle ID)
 */
nlohmann::json SetDictEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetArrayEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - AIArrayRef (handle ID)
 */
nlohmann::json GetArrayEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetArrayEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - AIArrayRef (handle ID)
 */
nlohmann::json SetArrayEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::GetUnicodeStringEntry
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["value"] - ai::UnicodeString
 */
nlohmann::json GetUnicodeStringEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::SetUnicodeStringEntry
 * @param params["dictionary"] - AIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @param params["value"] - const ai::UnicodeString
 */
nlohmann::json SetUnicodeStringEntry(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::TouchArt
 * @param params["dictionary"] - ConstAIDictionaryRef (handle ID)
 */
nlohmann::json TouchArt(const nlohmann::json& params);

/**
 * Wrapper for AIDictionarySuite::Find
 * @param params["dict"] - ConstAIDictionaryRef (handle ID)
 * @param params["key"] - AIDictKey (handle ID)
 * @returns ["iterator"] - AIDictionaryIterator (handle ID)
 */
nlohmann::json Find(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIDictionarySuite
} // namespace Flora
