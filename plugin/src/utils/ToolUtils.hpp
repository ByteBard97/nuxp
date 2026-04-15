/**
 * NUXP Tool Utilities
 *
 * Manual wrappers for Adobe Illustrator tool operations that cannot be
 * code-generated due to multi-step operations or complex type requirements.
 *
 * Usage:
 *   #include "utils/ToolUtils.hpp"
 *
 *   std::string name = ToolUtils::GetActiveToolName();
 *   bool success = ToolUtils::ActivateToolByName("Selection Tool");
 *   json tools = ToolUtils::ListTools();
 */

#ifndef NUXP_TOOL_UTILS_HPP
#define NUXP_TOOL_UTILS_HPP

#include "IllustratorSDK.h"
#include "NuxpThreadSafety.h"
#include <nlohmann/json.hpp>
#include <string>

namespace ToolUtils {

using json = nlohmann::json;

/**
 * Get the name of the currently active tool.
 *
 * @return Tool name string, or empty string if error
 */
std::string GetActiveToolName() REQUIRES_MAIN_THREAD;

/**
 * Activate a tool by its name.
 *
 * Iterates through available tools to find one matching the specified name,
 * then activates it. Tool names are case-sensitive.
 *
 * Common tool names:
 *   - "Selection Tool"
 *   - "Direct Selection Tool"
 *   - "Pen Tool"
 *   - "Type Tool"
 *   - "Line Segment Tool"
 *   - "Rectangle Tool"
 *   - "Ellipse Tool"
 *   - "Paintbrush Tool"
 *   - "Pencil Tool"
 *
 * @param toolName The name of the tool to activate
 * @return true if tool was found and activated, false otherwise
 */
bool ActivateToolByName(const std::string& toolName) REQUIRES_MAIN_THREAD;

/**
 * List all available tools.
 *
 * Returns an array of tool information:
 * [
 *   {
 *     "name": "Selection Tool",
 *     "number": 1,
 *     "isActive": true
 *   },
 *   ...
 * ]
 *
 * @return JSON array of tool info
 */
json ListTools() REQUIRES_MAIN_THREAD;

} // namespace ToolUtils

#endif // NUXP_TOOL_UTILS_HPP
