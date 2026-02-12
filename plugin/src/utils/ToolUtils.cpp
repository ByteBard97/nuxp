/**
 * NUXP Tool Utilities Implementation
 */

#include "ToolUtils.hpp"
#include <string>
#include <cstring>

// Suite accessor - defined in SuitePointers.cpp
extern AIToolSuite* sTool;

namespace ToolUtils {

std::string GetActiveToolName() {
    if (!sTool) {
        return "";
    }

    AIToolHandle activeTool = nullptr;
    AIErr err = sTool->GetSelectedTool(&activeTool);
    if (err != kNoErr || !activeTool) {
        return "";
    }

    char* name = nullptr;
    err = sTool->GetToolName(activeTool, &name);
    if (err != kNoErr || !name) {
        return "";
    }

    return std::string(name);
}

bool ActivateToolByName(const std::string& toolName) {
    if (!sTool || toolName.empty()) {
        return false;
    }

    // Try to get tool number by name first (most efficient)
    AIToolType toolNum = 0;
    AIErr err = sTool->GetToolNumberFromName(toolName.c_str(), &toolNum);
    if (err == kNoErr) {
        AIToolHandle tool = nullptr;
        err = sTool->GetToolHandleFromNumber(toolNum, &tool);
        if (err == kNoErr && tool) {
            err = sTool->SetSelectedTool(tool);
            return (err == kNoErr);
        }
    }

    // Fallback: iterate through tools to find matching name
    ai::int32 toolCount = 0;
    err = sTool->CountTools(&toolCount);
    if (err != kNoErr || toolCount <= 0) {
        return false;
    }

    for (ai::int32 i = 0; i < toolCount; i++) {
        AIToolHandle tool = nullptr;
        err = sTool->GetToolHandleFromNumber(i, &tool);
        if (err != kNoErr || !tool) {
            continue;
        }

        char* name = nullptr;
        err = sTool->GetToolName(tool, &name);
        if (err != kNoErr || !name) {
            continue;
        }

        if (toolName == name) {
            // Found the tool, activate it
            err = sTool->SetSelectedTool(tool);
            return (err == kNoErr);
        }
    }

    return false;  // Tool not found
}

json ListTools() {
    json result = json::array();

    if (!sTool) {
        return result;
    }

    // Get active tool for comparison
    AIToolHandle activeTool = nullptr;
    sTool->GetSelectedTool(&activeTool);

    // Count total tools
    ai::int32 toolCount = 0;
    AIErr err = sTool->CountTools(&toolCount);
    if (err != kNoErr || toolCount <= 0) {
        return result;
    }

    // Iterate through all tools
    for (ai::int32 i = 0; i < toolCount; i++) {
        AIToolHandle tool = nullptr;
        err = sTool->GetToolHandleFromNumber(i, &tool);
        if (err != kNoErr || !tool) {
            continue;
        }

        char* name = nullptr;
        err = sTool->GetToolName(tool, &name);
        if (err != kNoErr || !name) {
            continue;
        }

        json toolInfo;
        toolInfo["name"] = std::string(name);
        toolInfo["number"] = i;
        toolInfo["isActive"] = (tool == activeTool);

        result.push_back(toolInfo);
    }

    return result;
}

} // namespace ToolUtils
