#pragma once

/**
 * NUXP Plugin Header
 *
 * This file declares the main plugin entry points and lifecycle functions
 * for the Adobe Illustrator NUXP plugin.
 *
 * The plugin provides an HTTP/JSON bridge between Illustrator and external
 * applications (specifically a Tauri frontend).
 */

#include "AITypes.h"
#include "AIPlugin.h"
#include "AITimer.h"
#include "AINotifier.h"

// Plugin identification
#define NUXP_PLUGIN_NAME        "NUXP"
#define NUXP_PLUGIN_VERSION     "1.0.0"
#define NUXP_TIMER_NAME         "NUXP Timer"
#define NUXP_NOTIFIER_NAME      "NUXP Notifier"

// Default HTTP server port
#define NUXP_DEFAULT_PORT       8080

// Timer period in ticks (60 ticks per second, 1 tick ~= 16.67ms)
// We use 1 tick for responsive main thread dispatch
#define NUXP_TIMER_PERIOD       1

/**
 * Forward declarations for plugin message types.
 * These are defined in the Adobe SDK headers.
 */
struct SPInterfaceMessage;
struct AITimerMessage;
struct AINotifierMessage;

/**
 * Plugin startup handler.
 * Called when Illustrator loads the plugin.
 *
 * Responsibilities:
 * - Acquire SDK suites (Timer, Notifier, etc.)
 * - Create main thread dispatch timer
 * - Register document/art change notifiers
 * - Start HTTP server on background thread
 *
 * @param message The startup message from Illustrator
 * @return kNoErr on success, error code otherwise
 */
ASErr StartupPlugin(SPInterfaceMessage* message);

/**
 * Plugin shutdown handler.
 * Called when Illustrator unloads the plugin or quits.
 *
 * Responsibilities:
 * - Stop HTTP server
 * - Remove timers and notifiers
 * - Release SDK suites
 * - Clean up handle registries
 *
 * @param message The shutdown message from Illustrator
 * @return kNoErr on success, error code otherwise
 */
ASErr ShutdownPlugin(SPInterfaceMessage* message);

/**
 * Timer callback handler.
 * Called periodically by Illustrator's timer system.
 *
 * Responsibilities:
 * - Process the main thread dispatch work queue
 *
 * @param message The timer message from Illustrator
 * @return kNoErr on success, error code otherwise
 */
ASErr HandleTimer(AITimerMessage* message);

/**
 * Notifier callback handler.
 * Called when registered events occur (art selection, document changes, etc.)
 *
 * Responsibilities:
 * - Invalidate handle registries when document state changes
 *
 * @param message The notifier message from Illustrator
 * @return kNoErr on success, error code otherwise
 */
ASErr HandleNotifier(AINotifierMessage* message);

/**
 * Reload plugin handler.
 * Called when the plugin needs to reload (e.g., after preferences change).
 *
 * @param message The interface message from Illustrator
 * @return kNoErr on success, error code otherwise
 */
ASErr ReloadPlugin(SPInterfaceMessage* message);

/**
 * Unload plugin handler.
 * Called just before the plugin is unloaded from memory.
 *
 * @param message The interface message from Illustrator
 * @return kNoErr on success, error code otherwise
 */
ASErr UnloadPlugin(SPInterfaceMessage* message);
