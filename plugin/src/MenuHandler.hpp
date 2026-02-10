#pragma once

/**
 * MenuHandler - Plugin menu items
 *
 * Provides menu item in Illustrator for plugin configuration,
 * specifically for setting the HTTP server port.
 */

#include "IllustratorSDK.h"
#include "AIMenu.h"

/**
 * Menu handler for NUXP plugin menus
 */
namespace MenuHandler {

/**
 * Initialize menu system.
 * Call from StartupPlugin after acquiring suites.
 *
 * @param pluginRef The plugin reference
 * @return Error code (kNoErr on success)
 */
ASErr Initialize(SPPluginRef pluginRef);

/**
 * Handle menu item selection.
 * Call from PluginMain when receiving kCallerAIMenu.
 *
 * @param message The menu message
 * @return Error code (kNoErr on success)
 */
ASErr HandleMenu(AIMenuMessage* message);

/**
 * Cleanup menu resources.
 * Call from ShutdownPlugin.
 */
void Shutdown();

/**
 * Show the port configuration dialog.
 * Uses native platform dialog.
 *
 * @return true if user changed the port, false if cancelled
 */
bool ShowPortConfigDialog();

} // namespace MenuHandler
