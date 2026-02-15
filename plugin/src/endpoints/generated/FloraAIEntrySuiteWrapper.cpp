#include "FloraAIEntrySuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIEntrySuite* sEntry;

namespace Flora {
namespace AIEntrySuite {

nlohmann::json ToBoolean(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output primitive: value
    ASBoolean value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToBoolean(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToBoolean failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json ToInteger(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output primitive: value
    ai::int32 value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToInteger(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToInteger failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json ToReal(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output primitive: value
    AIReal value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToReal(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToReal failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json ToRealPoint(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output struct: value
    AIRealPoint value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToRealPoint(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToRealPoint failed with error: " + std::to_string(err));
    }

    // Marshal output struct: value
    response["value"] = {
        {"h", value.h},
        {"v", value.v}
    };

    return response;
}

nlohmann::json ToRealMatrix(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output struct: value
    AIRealMatrix value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToRealMatrix(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToRealMatrix failed with error: " + std::to_string(err));
    }

    // Marshal output struct: value
    response["value"] = {
        {"a", value.a},
        {"b", value.b},
        {"c", value.c},
        {"d", value.d},
        {"tx", value.tx},
        {"ty", value.ty}
    };

    return response;
}

nlohmann::json ToDict(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output handle: value
    AIDictionaryRef value = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToDict(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToDict failed with error: " + std::to_string(err));
    }

    // Marshal output handle: value
    if (value) {
        response["value"] = HandleManager::dictionaries.Register(value);
    } else {
        response["value"] = -1;
    }

    return response;
}

nlohmann::json ToArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToArt(entry_val, &art);
    if (err != kNoErr) {
        throw std::runtime_error("ToArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json ToArray(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output handle: value
    AIArrayRef value = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToArray(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToArray failed with error: " + std::to_string(err));
    }

    // Marshal output handle: value
    if (value) {
        response["value"] = HandleManager::arrays.Register(value);
    } else {
        response["value"] = -1;
    }

    return response;
}

nlohmann::json ToCustomColor(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output handle: value
    AICustomColorHandle value = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToCustomColor(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToCustomColor failed with error: " + std::to_string(err));
    }

    // Marshal output handle: value
    if (value) {
        response["value"] = HandleManager::customColors.Register(value);
    } else {
        response["value"] = -1;
    }

    return response;
}

nlohmann::json ToPluginObject(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    AIObjectHandle value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToPluginObject(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToPluginObject failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToFillStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    struct AIFillStyle value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToFillStyle(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToFillStyle failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToStrokeStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    struct AIStrokeStyle value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToStrokeStyle(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToStrokeStyle failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToUID(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    AIUIDRef value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToUID(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToUID failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToUIDREF(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    AIUIDREFRef value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToUIDREF(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToUIDREF failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToXMLNode(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    AIXMLNodeRef value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToXMLNode(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToXMLNode failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToSVGFilterHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    AISVGFilterHandle value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToSVGFilterHandle(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToSVGFilterHandle failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json AsBoolean(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output primitive: value
    ASBoolean value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->AsBoolean(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("AsBoolean failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json AsInteger(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output primitive: value
    ai::int32 value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->AsInteger(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("AsInteger failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json AsReal(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output primitive: value
    AIReal value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->AsReal(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("AsReal failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: value
    response["value"] = value;

    return response;
}

nlohmann::json AsUIDREF(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output unknown: value
    AIUIDREFRef value{};

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->AsUIDREF(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("AsUIDREF failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: value

    return response;
}

nlohmann::json ToArtStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output handle: value
    AIArtStyleHandle value = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToArtStyle(entry_val, &value);
    if (err != kNoErr) {
        throw std::runtime_error("ToArtStyle failed with error: " + std::to_string(err));
    }

    // Marshal output handle: value
    if (value) {
        response["value"] = HandleManager::artStyles.Register(value);
    } else {
        response["value"] = -1;
    }

    return response;
}

nlohmann::json ToUnicodeString(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output string: value
    ai::UnicodeString value;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->ToUnicodeString(entry_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("ToUnicodeString failed with error: " + std::to_string(err));
    }

    // Marshal output string: value
    response["value"] = value.as_UTF8();

    return response;
}

nlohmann::json AsUnicodeString(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: entry
    AIEntryRef entry_val = HandleManager::entries.Get(params["entry"].get<int32_t>());
    if (!entry_val) {
        throw std::runtime_error("Invalid AIEntryRef handle for parameter 'entry'");
    }
    // Output string: value
    ai::UnicodeString value;

    // Call SDK function (returns AIErr)
    AIErr err = sEntry->AsUnicodeString(entry_val, value);
    if (err != kNoErr) {
        throw std::runtime_error("AsUnicodeString failed with error: " + std::to_string(err));
    }

    // Marshal output string: value
    response["value"] = value.as_UTF8();

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "ToBoolean") {
        return ToBoolean(params);
    } else if (method == "ToInteger") {
        return ToInteger(params);
    } else if (method == "ToReal") {
        return ToReal(params);
    } else if (method == "ToRealPoint") {
        return ToRealPoint(params);
    } else if (method == "ToRealMatrix") {
        return ToRealMatrix(params);
    } else if (method == "ToDict") {
        return ToDict(params);
    } else if (method == "ToArt") {
        return ToArt(params);
    } else if (method == "ToArray") {
        return ToArray(params);
    } else if (method == "ToCustomColor") {
        return ToCustomColor(params);
    } else if (method == "ToPluginObject") {
        return ToPluginObject(params);
    } else if (method == "ToFillStyle") {
        return ToFillStyle(params);
    } else if (method == "ToStrokeStyle") {
        return ToStrokeStyle(params);
    } else if (method == "ToUID") {
        return ToUID(params);
    } else if (method == "ToUIDREF") {
        return ToUIDREF(params);
    } else if (method == "ToXMLNode") {
        return ToXMLNode(params);
    } else if (method == "ToSVGFilterHandle") {
        return ToSVGFilterHandle(params);
    } else if (method == "AsBoolean") {
        return AsBoolean(params);
    } else if (method == "AsInteger") {
        return AsInteger(params);
    } else if (method == "AsReal") {
        return AsReal(params);
    } else if (method == "AsUIDREF") {
        return AsUIDREF(params);
    } else if (method == "ToArtStyle") {
        return ToArtStyle(params);
    } else if (method == "ToUnicodeString") {
        return ToUnicodeString(params);
    } else if (method == "AsUnicodeString") {
        return AsUnicodeString(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIEntrySuite");
}

} // namespace AIEntrySuite
} // namespace Flora
