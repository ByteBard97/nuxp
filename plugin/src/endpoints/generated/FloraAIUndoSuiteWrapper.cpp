#include "FloraAIUndoSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIUndoSuite* sUndo;

namespace Flora {
namespace AIUndoSuite {

nlohmann::json SetUndoTextUS(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: undoText
    ai::UnicodeString undoText(params["undoText"].get<std::string>());
    // Input string: redoText
    ai::UnicodeString redoText(params["redoText"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetUndoTextUS(undoText, redoText);
    if (err != kNoErr) {
        throw std::runtime_error("SetUndoTextUS failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetUndoRedoCmdTextUS(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: undoText
    ai::UnicodeString undoText(params["undoText"].get<std::string>());
    // Input string: redoText
    ai::UnicodeString redoText(params["redoText"].get<std::string>());
    // Input string: cmdText
    ai::UnicodeString cmdText(params["cmdText"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetUndoRedoCmdTextUS(undoText, redoText, cmdText);
    if (err != kNoErr) {
        throw std::runtime_error("SetUndoRedoCmdTextUS failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MultiUndoTransaction(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->MultiUndoTransaction(document_val, n);
    if (err != kNoErr) {
        throw std::runtime_error("MultiUndoTransaction failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MultiRedoTransaction(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->MultiRedoTransaction(document_val, n);
    if (err != kNoErr) {
        throw std::runtime_error("MultiRedoTransaction failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ForgetRedos(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->ForgetRedos(document_val);
    if (err != kNoErr) {
        throw std::runtime_error("ForgetRedos failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ClearHistory(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->ClearHistory(document_val);
    if (err != kNoErr) {
        throw std::runtime_error("ClearHistory failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetSilent(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: silent
    AIBoolean silent = params["silent"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetSilent(silent);
    if (err != kNoErr) {
        throw std::runtime_error("SetSilent failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetKind(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: kind
    ai::int32 kind = params["kind"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetKind(kind);
    if (err != kNoErr) {
        throw std::runtime_error("SetKind failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json CountTransactions(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: past
    ai::int32 past{};
    // Output primitive: future
    ai::int32 future{};

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->CountTransactions(&past, &future);
    if (err != kNoErr) {
        throw std::runtime_error("CountTransactions failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: past
    response["past"] = past;
    // Marshal output primitive: future
    response["future"] = future;

    return response;
}

nlohmann::json IsSilent(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: silent
    AIBoolean silent{};

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->IsSilent(&silent);
    if (err != kNoErr) {
        throw std::runtime_error("IsSilent failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: silent
    response["silent"] = static_cast<bool>(silent);

    return response;
}

nlohmann::json SetTagUS(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: tagString
    ai::UnicodeString tagString(params["tagString"].get<std::string>());
    // Input primitive: tagInteger
    ai::int32 tagInteger = params["tagInteger"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetTagUS(tagString, tagInteger);
    if (err != kNoErr) {
        throw std::runtime_error("SetTagUS failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetTagUS(const nlohmann::json& params) {
    nlohmann::json response;

    // Output string: tagString
    ai::UnicodeString tagString;
    // Output primitive: tagInteger
    ai::int32 tagInteger{};

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->GetTagUS(tagString, &tagInteger);
    if (err != kNoErr) {
        throw std::runtime_error("GetTagUS failed with error: " + std::to_string(err));
    }

    // Marshal output string: tagString
    response["tagString"] = tagString.as_UTF8();
    // Marshal output primitive: tagInteger
    response["tagInteger"] = tagInteger;

    return response;
}

nlohmann::json SetNthTransactionTagUS(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Input string: tagString
    ai::UnicodeString tagString(params["tagString"].get<std::string>());
    // Input primitive: tagInteger
    ai::int32 tagInteger = params["tagInteger"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetNthTransactionTagUS(n, tagString, tagInteger);
    if (err != kNoErr) {
        throw std::runtime_error("SetNthTransactionTagUS failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetNthTransactionTagUS(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Output string: tagString
    ai::UnicodeString tagString;
    // Output primitive: tagInteger
    ai::int32 tagInteger{};

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->GetNthTransactionTagUS(n, tagString, &tagInteger);
    if (err != kNoErr) {
        throw std::runtime_error("GetNthTransactionTagUS failed with error: " + std::to_string(err));
    }

    // Marshal output string: tagString
    response["tagString"] = tagString.as_UTF8();
    // Marshal output primitive: tagInteger
    response["tagInteger"] = tagInteger;

    return response;
}

nlohmann::json SetRecordingSuspended(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: inSuspend
    AIBoolean inSuspend = params["inSuspend"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->SetRecordingSuspended(inSuspend);
    if (err != kNoErr) {
        throw std::runtime_error("SetRecordingSuspended failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IsRecordingSuspended(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: outIsSuspended
    AIBoolean outIsSuspended{};

    // Call SDK function (returns AIErr)
    AIErr err = sUndo->IsRecordingSuspended(&outIsSuspended);
    if (err != kNoErr) {
        throw std::runtime_error("IsRecordingSuspended failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: outIsSuspended
    response["outIsSuspended"] = static_cast<bool>(outIsSuspended);

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "SetUndoTextUS") {
        return SetUndoTextUS(params);
    } else if (method == "SetUndoRedoCmdTextUS") {
        return SetUndoRedoCmdTextUS(params);
    } else if (method == "MultiUndoTransaction") {
        return MultiUndoTransaction(params);
    } else if (method == "MultiRedoTransaction") {
        return MultiRedoTransaction(params);
    } else if (method == "ForgetRedos") {
        return ForgetRedos(params);
    } else if (method == "ClearHistory") {
        return ClearHistory(params);
    } else if (method == "SetSilent") {
        return SetSilent(params);
    } else if (method == "SetKind") {
        return SetKind(params);
    } else if (method == "CountTransactions") {
        return CountTransactions(params);
    } else if (method == "IsSilent") {
        return IsSilent(params);
    } else if (method == "SetTagUS") {
        return SetTagUS(params);
    } else if (method == "GetTagUS") {
        return GetTagUS(params);
    } else if (method == "SetNthTransactionTagUS") {
        return SetNthTransactionTagUS(params);
    } else if (method == "GetNthTransactionTagUS") {
        return GetNthTransactionTagUS(params);
    } else if (method == "SetRecordingSuspended") {
        return SetRecordingSuspended(params);
    } else if (method == "IsRecordingSuspended") {
        return IsRecordingSuspended(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIUndoSuite");
}

} // namespace AIUndoSuite
} // namespace Flora
