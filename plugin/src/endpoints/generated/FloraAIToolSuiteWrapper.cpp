#include "FloraAIToolSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIToolSuite* sTool;

namespace Flora {
namespace AIToolSuite {

nlohmann::json GetToolName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output string pointer (SDK-managed): name
    char* name = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolName(tool_val, &name);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolName failed with error: " + std::to_string(err));
    }

    // Marshal output string pointer: name
    response["name"] = name ? std::string(name) : "";

    return response;
}

nlohmann::json GetToolOptions(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output primitive: options
    ai::int32 options{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolOptions(tool_val, &options);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolOptions failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: options
    response["options"] = options;

    return response;
}

nlohmann::json SetToolOptions(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Input primitive: options
    ai::int32 options = params["options"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetToolOptions(tool_val, options);
    if (err != kNoErr) {
        throw std::runtime_error("SetToolOptions failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetToolPlugin(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output unknown: plugin
    SPPluginRef plugin{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolPlugin(tool_val, &plugin);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolPlugin failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: plugin

    return response;
}

nlohmann::json GetSelectedTool(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: tool
    AIToolHandle tool = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetSelectedTool(&tool);
    if (err != kNoErr) {
        throw std::runtime_error("GetSelectedTool failed with error: " + std::to_string(err));
    }

    // Marshal output handle: tool
    if (tool) {
        response["tool"] = HandleManager::tools.Register(tool);
    } else {
        response["tool"] = -1;
    }

    return response;
}

nlohmann::json SetSelectedTool(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetSelectedTool(tool_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetSelectedTool failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json CountTools(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->CountTools(&count);
    if (err != kNoErr) {
        throw std::runtime_error("CountTools failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetNthTool(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Output handle: tool
    AIToolHandle tool = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetNthTool(n, &tool);
    if (err != kNoErr) {
        throw std::runtime_error("GetNthTool failed with error: " + std::to_string(err));
    }

    // Marshal output handle: tool
    if (tool) {
        response["tool"] = HandleManager::tools.Register(tool);
    } else {
        response["tool"] = -1;
    }

    return response;
}

nlohmann::json GetToolHandleFromNumber(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: toolNum
    AIToolType toolNum = params["toolNum"].get<int32_t>();
    // Output handle: tool
    AIToolHandle tool = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolHandleFromNumber(toolNum, &tool);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolHandleFromNumber failed with error: " + std::to_string(err));
    }

    // Marshal output handle: tool
    if (tool) {
        response["tool"] = HandleManager::tools.Register(tool);
    } else {
        response["tool"] = -1;
    }

    return response;
}

nlohmann::json GetToolNumberFromName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: name
    std::string name_str = params["name"].get<std::string>();
    const char* name = name_str.c_str();
    // Output primitive: toolNum
    AIToolType toolNum{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolNumberFromName(name, &toolNum);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolNumberFromName failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: toolNum
    response["toolNum"] = toolNum;

    return response;
}

nlohmann::json GetToolNumberFromHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output primitive: toolNum
    AIToolType toolNum{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolNumberFromHandle(tool_val, &toolNum);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolNumberFromHandle failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: toolNum
    response["toolNum"] = toolNum;

    return response;
}

nlohmann::json GetToolNameFromNumber(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: toolNum
    AIToolType toolNum = params["toolNum"].get<int32_t>();
    // Output string pointer (SDK-managed): name
    char* name = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolNameFromNumber(toolNum, &name);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolNameFromNumber failed with error: " + std::to_string(err));
    }

    // Marshal output string pointer: name
    response["name"] = name ? std::string(name) : "";

    return response;
}

nlohmann::json GetToolTitle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output string: title
    ai::UnicodeString title;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolTitle(tool_val, title);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolTitle failed with error: " + std::to_string(err));
    }

    // Marshal output string: title
    response["title"] = title.as_UTF8();

    return response;
}

nlohmann::json SetToolTitle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Input string: title
    ai::UnicodeString title(params["title"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetToolTitle(tool_val, title);
    if (err != kNoErr) {
        throw std::runtime_error("SetToolTitle failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetTooltip(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output string: tooltip
    ai::UnicodeString tooltip;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetTooltip(tool_val, tooltip);
    if (err != kNoErr) {
        throw std::runtime_error("GetTooltip failed with error: " + std::to_string(err));
    }

    // Marshal output string: tooltip
    response["tooltip"] = tooltip.as_UTF8();

    return response;
}

nlohmann::json SetTooltip(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Input string: tooltip
    ai::UnicodeString tooltip(params["tooltip"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetTooltip(tool_val, tooltip);
    if (err != kNoErr) {
        throw std::runtime_error("SetTooltip failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SystemHasPressure(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: hasPressure
    AIBoolean hasPressure{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SystemHasPressure(&hasPressure);
    if (err != kNoErr) {
        throw std::runtime_error("SystemHasPressure failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasPressure
    response["hasPressure"] = static_cast<bool>(hasPressure);

    return response;
}

nlohmann::json GetToolNullEventInterval(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Output primitive: outTime
    AIToolTime outTime{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolNullEventInterval(tool_val, &outTime);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolNullEventInterval failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: outTime
    response["outTime"] = outTime;

    return response;
}

nlohmann::json SetToolNullEventInterval(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Input primitive: inTime
    AIToolTime inTime = params["inTime"].get<uint32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetToolNullEventInterval(tool_val, inTime);
    if (err != kNoErr) {
        throw std::runtime_error("SetToolNullEventInterval failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetSoftSelectedTool(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetSoftSelectedTool(tool_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetSoftSelectedTool failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IsSoftModeSelection(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: isSoftMode
    AIBoolean isSoftMode{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->IsSoftModeSelection(&isSoftMode);
    if (err != kNoErr) {
        throw std::runtime_error("IsSoftModeSelection failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isSoftMode
    response["isSoftMode"] = static_cast<bool>(isSoftMode);

    return response;
}

nlohmann::json SetAlternateSelectionToolName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Input string: alternateTool
    std::string alternateTool_str = params["alternateTool"].get<std::string>();
    const char* alternateTool = alternateTool_str.c_str();

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetAlternateSelectionToolName(tool_val, alternateTool);
    if (err != kNoErr) {
        throw std::runtime_error("SetAlternateSelectionToolName failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetCurrentToolNumber(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: toolNum
    AIToolType toolNum{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetCurrentToolNumber(&toolNum);
    if (err != kNoErr) {
        throw std::runtime_error("GetCurrentToolNumber failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: toolNum
    response["toolNum"] = toolNum;

    return response;
}

nlohmann::json GetCurrentEffectiveTool(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: tool
    AIToolHandle tool = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetCurrentEffectiveTool(&tool);
    if (err != kNoErr) {
        throw std::runtime_error("GetCurrentEffectiveTool failed with error: " + std::to_string(err));
    }

    // Marshal output handle: tool
    if (tool) {
        response["tool"] = HandleManager::tools.Register(tool);
    } else {
        response["tool"] = -1;
    }

    return response;
}

nlohmann::json GetCurrentEffectiveToolNumber(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: toolNum
    AIToolType toolNum{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetCurrentEffectiveToolNumber(&toolNum);
    if (err != kNoErr) {
        throw std::runtime_error("GetCurrentEffectiveToolNumber failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: toolNum
    response["toolNum"] = toolNum;

    return response;
}

nlohmann::json SetSelectedToolByName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: name
    std::string name_str = params["name"].get<std::string>();
    const char* name = name_str.c_str();

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetSelectedToolByName(name);
    if (err != kNoErr) {
        throw std::runtime_error("SetSelectedToolByName failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetSelectedToolByNumber(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: toolNum
    AIToolType toolNum = params["toolNum"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetSelectedToolByNumber(toolNum);
    if (err != kNoErr) {
        throw std::runtime_error("SetSelectedToolByNumber failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetTabletHardwareCapabilities(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: hardwareCapabilities
    ai::int32 hardwareCapabilities{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetTabletHardwareCapabilities(&hardwareCapabilities);
    if (err != kNoErr) {
        throw std::runtime_error("GetTabletHardwareCapabilities failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hardwareCapabilities
    response["hardwareCapabilities"] = hardwareCapabilities;

    return response;
}

nlohmann::json SetToolIcons(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Input string: normalIconResourceName
    std::string normalIconResourceName_str = params["normalIconResourceName"].get<std::string>();
    const char* normalIconResourceName = normalIconResourceName_str.c_str();
    // Input string: darkNormalIconResourceName
    std::string darkNormalIconResourceName_str = params["darkNormalIconResourceName"].get<std::string>();
    const char* darkNormalIconResourceName = darkNormalIconResourceName_str.c_str();

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetToolIcons(tool_val, normalIconResourceName, darkNormalIconResourceName);
    if (err != kNoErr) {
        throw std::runtime_error("SetToolIcons failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetDocumentInkParams(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: tool
    AIToolHandle tool_val = HandleManager::tools.Get(params["tool"].get<int32_t>());
    if (!tool_val) {
        throw std::runtime_error("Invalid AIToolHandle handle for parameter 'tool'");
    }
    // Unknown type: inDocInkParams (const AIDocumentInkParams)
    // WARNING: Using default initialization
    const AIDocumentInkParams inDocInkParams{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->SetDocumentInkParams(tool_val, inDocInkParams);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentInkParams failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetToolOptionsFromNumber(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: toolNum
    AIToolType toolNum = params["toolNum"].get<int32_t>();
    // Output primitive: options
    ai::int32 options{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolOptionsFromNumber(toolNum, &options);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolOptionsFromNumber failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: options
    response["options"] = options;

    return response;
}

nlohmann::json GetToolOptionsFromName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: toolName
    std::string toolName_str = params["toolName"].get<std::string>();
    const char* toolName = toolName_str.c_str();
    // Output primitive: options
    ai::int32 options{};

    // Call SDK function (returns AIErr)
    AIErr err = sTool->GetToolOptionsFromName(toolName, &options);
    if (err != kNoErr) {
        throw std::runtime_error("GetToolOptionsFromName failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: options
    response["options"] = options;

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetToolName") {
        return GetToolName(params);
    } else if (method == "GetToolOptions") {
        return GetToolOptions(params);
    } else if (method == "SetToolOptions") {
        return SetToolOptions(params);
    } else if (method == "GetToolPlugin") {
        return GetToolPlugin(params);
    } else if (method == "GetSelectedTool") {
        return GetSelectedTool(params);
    } else if (method == "SetSelectedTool") {
        return SetSelectedTool(params);
    } else if (method == "CountTools") {
        return CountTools(params);
    } else if (method == "GetNthTool") {
        return GetNthTool(params);
    } else if (method == "GetToolHandleFromNumber") {
        return GetToolHandleFromNumber(params);
    } else if (method == "GetToolNumberFromName") {
        return GetToolNumberFromName(params);
    } else if (method == "GetToolNumberFromHandle") {
        return GetToolNumberFromHandle(params);
    } else if (method == "GetToolNameFromNumber") {
        return GetToolNameFromNumber(params);
    } else if (method == "GetToolTitle") {
        return GetToolTitle(params);
    } else if (method == "SetToolTitle") {
        return SetToolTitle(params);
    } else if (method == "GetTooltip") {
        return GetTooltip(params);
    } else if (method == "SetTooltip") {
        return SetTooltip(params);
    } else if (method == "SystemHasPressure") {
        return SystemHasPressure(params);
    } else if (method == "GetToolNullEventInterval") {
        return GetToolNullEventInterval(params);
    } else if (method == "SetToolNullEventInterval") {
        return SetToolNullEventInterval(params);
    } else if (method == "SetSoftSelectedTool") {
        return SetSoftSelectedTool(params);
    } else if (method == "IsSoftModeSelection") {
        return IsSoftModeSelection(params);
    } else if (method == "SetAlternateSelectionToolName") {
        return SetAlternateSelectionToolName(params);
    } else if (method == "GetCurrentToolNumber") {
        return GetCurrentToolNumber(params);
    } else if (method == "GetCurrentEffectiveTool") {
        return GetCurrentEffectiveTool(params);
    } else if (method == "GetCurrentEffectiveToolNumber") {
        return GetCurrentEffectiveToolNumber(params);
    } else if (method == "SetSelectedToolByName") {
        return SetSelectedToolByName(params);
    } else if (method == "SetSelectedToolByNumber") {
        return SetSelectedToolByNumber(params);
    } else if (method == "GetTabletHardwareCapabilities") {
        return GetTabletHardwareCapabilities(params);
    } else if (method == "SetToolIcons") {
        return SetToolIcons(params);
    } else if (method == "SetDocumentInkParams") {
        return SetDocumentInkParams(params);
    } else if (method == "GetToolOptionsFromNumber") {
        return GetToolOptionsFromNumber(params);
    } else if (method == "GetToolOptionsFromName") {
        return GetToolOptionsFromName(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIToolSuite");
}

} // namespace AIToolSuite
} // namespace Flora
