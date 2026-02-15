#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIUndoSuite {

/**
 * Wrapper for AIUndoSuite::SetUndoTextUS
 * @param params["undoText"] - const ai::UnicodeString
 * @param params["redoText"] - const ai::UnicodeString
 */
nlohmann::json SetUndoTextUS(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::SetUndoRedoCmdTextUS
 * @param params["undoText"] - const ai::UnicodeString
 * @param params["redoText"] - const ai::UnicodeString
 * @param params["cmdText"] - const ai::UnicodeString
 */
nlohmann::json SetUndoRedoCmdTextUS(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::MultiUndoTransaction
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @param params["n"] - ai::int32
 */
nlohmann::json MultiUndoTransaction(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::MultiRedoTransaction
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @param params["n"] - ai::int32
 */
nlohmann::json MultiRedoTransaction(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::ForgetRedos
 * @param params["document"] - AIDocumentHandle (handle ID)
 */
nlohmann::json ForgetRedos(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::ClearHistory
 * @param params["document"] - AIDocumentHandle (handle ID)
 */
nlohmann::json ClearHistory(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::SetSilent
 * @param params["silent"] - AIBoolean
 */
nlohmann::json SetSilent(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::SetKind
 * @param params["kind"] - ai::int32
 */
nlohmann::json SetKind(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::CountTransactions
 * @returns ["past"] - ai::int32
 * @returns ["future"] - ai::int32
 */
nlohmann::json CountTransactions(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::IsSilent
 * @returns ["silent"] - AIBoolean
 */
nlohmann::json IsSilent(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::SetTagUS
 * @param params["tagString"] - const ai::UnicodeString
 * @param params["tagInteger"] - ai::int32
 */
nlohmann::json SetTagUS(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::GetTagUS
 * @returns ["tagString"] - ai::UnicodeString
 * @returns ["tagInteger"] - ai::int32
 */
nlohmann::json GetTagUS(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::SetNthTransactionTagUS
 * @param params["n"] - ai::int32
 * @param params["tagString"] - const ai::UnicodeString
 * @param params["tagInteger"] - ai::int32
 */
nlohmann::json SetNthTransactionTagUS(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::GetNthTransactionTagUS
 * @param params["n"] - ai::int32
 * @returns ["tagString"] - ai::UnicodeString
 * @returns ["tagInteger"] - ai::int32
 */
nlohmann::json GetNthTransactionTagUS(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::SetRecordingSuspended
 * @param params["inSuspend"] - AIBoolean
 */
nlohmann::json SetRecordingSuspended(const nlohmann::json& params);

/**
 * Wrapper for AIUndoSuite::IsRecordingSuspended
 * @returns ["outIsSuspended"] - AIBoolean
 */
nlohmann::json IsRecordingSuspended(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIUndoSuite
} // namespace Flora
