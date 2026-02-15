#include "FloraAIUserSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIUserSuite* sUser;

namespace Flora {
namespace AIUserSuite {

nlohmann::json IUAIRealToStringUnits(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: value
    AIReal value = params["value"].get<double>();
    // Input primitive: precision
    ai::int32 precision = params["precision"].get<int32_t>();
    // Output string: string
    ai::UnicodeString string;

    // Call SDK function (returns AIErr)
    AIErr err = sUser->IUAIRealToStringUnits(value, precision, string);
    if (err != kNoErr) {
        throw std::runtime_error("IUAIRealToStringUnits failed with error: " + std::to_string(err));
    }

    // Marshal output string: string
    response["string"] = string.as_UTF8();

    return response;
}

nlohmann::json IUAIRealToStringUnitsWithoutScale(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: value
    AIReal value = params["value"].get<double>();
    // Input primitive: precision
    ai::int32 precision = params["precision"].get<int32_t>();
    // Output string: string
    ai::UnicodeString string;

    // Call SDK function (returns AIErr)
    AIErr err = sUser->IUAIRealToStringUnitsWithoutScale(value, precision, string);
    if (err != kNoErr) {
        throw std::runtime_error("IUAIRealToStringUnitsWithoutScale failed with error: " + std::to_string(err));
    }

    // Marshal output string: string
    response["string"] = string.as_UTF8();

    return response;
}

nlohmann::json GetUnitsString(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: format
    ai::int16 format = params["format"].get<int16_t>();
    // Output string: string
    ai::UnicodeString string;

    // Call SDK function (returns AIErr)
    AIErr err = sUser->GetUnitsString(format, string);
    if (err != kNoErr) {
        throw std::runtime_error("GetUnitsString failed with error: " + std::to_string(err));
    }

    // Marshal output string: string
    response["string"] = string.as_UTF8();

    return response;
}

nlohmann::json GetGlobalObjectDisplayName(const nlohmann::json& params) {
    nlohmann::json response;

    // Output string: name
    ai::UnicodeString name;

    // Call SDK function (returns AIErr)
    AIErr err = sUser->GetGlobalObjectDisplayName(name);
    if (err != kNoErr) {
        throw std::runtime_error("GetGlobalObjectDisplayName failed with error: " + std::to_string(err));
    }

    // Marshal output string: name
    response["name"] = name.as_UTF8();

    return response;
}

nlohmann::json EditInOriginalApp(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sUser->EditInOriginalApp(art_val);
    if (err != kNoErr) {
        throw std::runtime_error("EditInOriginalApp failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json BuildDirectoryMenu(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: menu (AIPlatformMenuHandle)
    // WARNING: Using default initialization
    AIPlatformMenuHandle menu{};
    // Input file path: fileSpec
    ai::FilePath fileSpec(ai::UnicodeString(params["fileSpec"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sUser->BuildDirectoryMenu(menu, fileSpec);
    if (err != kNoErr) {
        throw std::runtime_error("BuildDirectoryMenu failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetIndexedDirectorySpec(const nlohmann::json& params) {
    nlohmann::json response;

    // Output file path: fileSpec
    ai::FilePath fileSpec;
    // Input primitive: index
    ai::int32 index = params["index"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sUser->GetIndexedDirectorySpec(fileSpec, index);
    if (err != kNoErr) {
        throw std::runtime_error("GetIndexedDirectorySpec failed with error: " + std::to_string(err));
    }

    // Marshal output file path: fileSpec
    response["fileSpec"] = fileSpec.GetFullPath().as_UTF8();

    return response;
}

nlohmann::json RevealTheFile(const nlohmann::json& params) {
    nlohmann::json response;

    // Input file path: fileSpec
    ai::FilePath fileSpec(ai::UnicodeString(params["fileSpec"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sUser->RevealTheFile(fileSpec);
    if (err != kNoErr) {
        throw std::runtime_error("RevealTheFile failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDateAndTime(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: outValue
    AIUserDateTime outValue{};

    // Call SDK function (returns AIErr)
    AIErr err = sUser->GetDateAndTime(&outValue);
    if (err != kNoErr) {
        throw std::runtime_error("GetDateAndTime failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: outValue

    return response;
}

nlohmann::json LaunchApp(const nlohmann::json& params) {
    nlohmann::json response;

    // Input file path: spFileSpec
    ai::FilePath spFileSpec(ai::UnicodeString(params["spFileSpec"].get<std::string>()));
    // Input primitive: openDoc
    ASBoolean openDoc = params["openDoc"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sUser->LaunchApp(spFileSpec, openDoc);
    if (err != kNoErr) {
        throw std::runtime_error("LaunchApp failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SameName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: name1
    ai::UnicodeString name1(params["name1"].get<std::string>());
    // Input string: name2
    ai::UnicodeString name2(params["name2"].get<std::string>());
    // Output primitive: same
    AIBoolean same{};

    // Call SDK function (returns AIErr)
    AIErr err = sUser->SameName(name1, name2, same);
    if (err != kNoErr) {
        throw std::runtime_error("SameName failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: same
    response["same"] = static_cast<bool>(same);

    return response;
}

nlohmann::json GetDirectoryDialog(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: title
    ai::UnicodeString title(params["title"].get<std::string>());
    // Output file path: ioFilePath
    ai::FilePath ioFilePath;

    // Call SDK function (returns AIErr)
    AIErr err = sUser->GetDirectoryDialog(title, ioFilePath);
    if (err != kNoErr) {
        throw std::runtime_error("GetDirectoryDialog failed with error: " + std::to_string(err));
    }

    // Marshal output file path: ioFilePath
    response["ioFilePath"] = ioFilePath.GetFullPath().as_UTF8();

    return response;
}

nlohmann::json EvaluateExpression(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: expr
    ai::UnicodeString expr(params["expr"].get<std::string>());
    // Unknown type: options (const AIExpressionOptions)
    // WARNING: Using default initialization
    const AIExpressionOptions options{};
    // Output string: evaluatedExpr
    ai::UnicodeString evaluatedExpr;
    // Output primitive: isChanged
    AIBoolean isChanged{};
    // Output unknown: numericValue
    AIDouble numericValue{};

    // Call SDK function (returns AIErr)
    AIErr err = sUser->EvaluateExpression(expr, options, evaluatedExpr, isChanged, numericValue);
    if (err != kNoErr) {
        throw std::runtime_error("EvaluateExpression failed with error: " + std::to_string(err));
    }

    // Marshal output string: evaluatedExpr
    response["evaluatedExpr"] = evaluatedExpr.as_UTF8();
    // Marshal output primitive: isChanged
    response["isChanged"] = static_cast<bool>(isChanged);
    // Unable to marshal unknown type: numericValue

    return response;
}

nlohmann::json EvaluateExpressionWithoutScale(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: expr
    ai::UnicodeString expr(params["expr"].get<std::string>());
    // Unknown type: options (const AIExpressionOptions)
    // WARNING: Using default initialization
    const AIExpressionOptions options{};
    // Output string: evaluatedExpr
    ai::UnicodeString evaluatedExpr;
    // Output primitive: isChanged
    AIBoolean isChanged{};
    // Output unknown: numericValue
    AIDouble numericValue{};

    // Call SDK function (returns AIErr)
    AIErr err = sUser->EvaluateExpressionWithoutScale(expr, options, evaluatedExpr, isChanged, numericValue);
    if (err != kNoErr) {
        throw std::runtime_error("EvaluateExpressionWithoutScale failed with error: " + std::to_string(err));
    }

    // Marshal output string: evaluatedExpr
    response["evaluatedExpr"] = evaluatedExpr.as_UTF8();
    // Marshal output primitive: isChanged
    response["isChanged"] = static_cast<bool>(isChanged);
    // Unable to marshal unknown type: numericValue

    return response;
}

nlohmann::json SetCursor(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: cursorID
    ai::int32 cursorID = params["cursorID"].get<int32_t>();
    // Unknown type: inRscMgr (AIResourceManagerHandle)
    // WARNING: Using default initialization
    AIResourceManagerHandle inRscMgr{};

    // Call SDK function (returns AIErr)
    AIErr err = sUser->SetCursor(cursorID, inRscMgr);
    if (err != kNoErr) {
        throw std::runtime_error("SetCursor failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetSVGCursor(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: cursorID
    ai::int32 cursorID = params["cursorID"].get<int32_t>();
    // Unknown type: inRscMgr (AIResourceManagerHandle)
    // WARNING: Using default initialization
    AIResourceManagerHandle inRscMgr{};

    // Call SDK function (returns AIErr)
    AIErr err = sUser->SetSVGCursor(cursorID, inRscMgr);
    if (err != kNoErr) {
        throw std::runtime_error("SetSVGCursor failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetAILanguageCode(const nlohmann::json& params) {
    nlohmann::json response;

    // Output string: lang
    ai::UnicodeString lang;

    // Call SDK function (returns AIErr)
    AIErr err = sUser->GetAILanguageCode(lang);
    if (err != kNoErr) {
        throw std::runtime_error("GetAILanguageCode failed with error: " + std::to_string(err));
    }

    // Marshal output string: lang
    response["lang"] = lang.as_UTF8();

    return response;
}

nlohmann::json LaunchFolder(const nlohmann::json& params) {
    nlohmann::json response;

    // Input file path: folderPath
    ai::FilePath folderPath(ai::UnicodeString(params["folderPath"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sUser->LaunchFolder(folderPath);
    if (err != kNoErr) {
        throw std::runtime_error("LaunchFolder failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json LaunchCustomAppForCustomUri(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: customUri
    ai::UnicodeString customUri(params["customUri"].get<std::string>());
    // Input file path: appPath
    ai::FilePath appPath(ai::UnicodeString(params["appPath"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sUser->LaunchCustomAppForCustomUri(customUri, appPath);
    if (err != kNoErr) {
        throw std::runtime_error("LaunchCustomAppForCustomUri failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json EditInCustomApp(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input file path: appPath
    ai::FilePath appPath(ai::UnicodeString(params["appPath"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sUser->EditInCustomApp(art_val, appPath);
    if (err != kNoErr) {
        throw std::runtime_error("EditInCustomApp failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "IUAIRealToStringUnits") {
        return IUAIRealToStringUnits(params);
    } else if (method == "IUAIRealToStringUnitsWithoutScale") {
        return IUAIRealToStringUnitsWithoutScale(params);
    } else if (method == "GetUnitsString") {
        return GetUnitsString(params);
    } else if (method == "GetGlobalObjectDisplayName") {
        return GetGlobalObjectDisplayName(params);
    } else if (method == "EditInOriginalApp") {
        return EditInOriginalApp(params);
    } else if (method == "BuildDirectoryMenu") {
        return BuildDirectoryMenu(params);
    } else if (method == "GetIndexedDirectorySpec") {
        return GetIndexedDirectorySpec(params);
    } else if (method == "RevealTheFile") {
        return RevealTheFile(params);
    } else if (method == "GetDateAndTime") {
        return GetDateAndTime(params);
    } else if (method == "LaunchApp") {
        return LaunchApp(params);
    } else if (method == "SameName") {
        return SameName(params);
    } else if (method == "GetDirectoryDialog") {
        return GetDirectoryDialog(params);
    } else if (method == "EvaluateExpression") {
        return EvaluateExpression(params);
    } else if (method == "EvaluateExpressionWithoutScale") {
        return EvaluateExpressionWithoutScale(params);
    } else if (method == "SetCursor") {
        return SetCursor(params);
    } else if (method == "SetSVGCursor") {
        return SetSVGCursor(params);
    } else if (method == "GetAILanguageCode") {
        return GetAILanguageCode(params);
    } else if (method == "LaunchFolder") {
        return LaunchFolder(params);
    } else if (method == "LaunchCustomAppForCustomUri") {
        return LaunchCustomAppForCustomUri(params);
    } else if (method == "EditInCustomApp") {
        return EditInCustomApp(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIUserSuite");
}

} // namespace AIUserSuite
} // namespace Flora
