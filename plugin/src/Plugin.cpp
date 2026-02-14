/**
 * NUXP Plugin - Main Entry Point
 *
 * ⚠️ DO NOT MODIFY THIS FILE FOR DOWNSTREAM-SPECIFIC CONCERNS ⚠️
 *
 * This is the STANDALONE entry point for NUXP. When downstream projects
 * (like FloraBridge) use NUXP, they link against nuxp-core (INTERFACE library)
 * which does NOT compile this file. Downstream plugins have their own
 * entry point that handles initialization in that context.
 *
 * If you see "dual initialization" issues, the fix is in CMake configuration,
 * NOT by gutting this file.
 *
 * This file implements the Adobe Illustrator plugin entry point and lifecycle
 * management for the NUXP HTTP/JSON bridge plugin.
 *
 * The plugin:
 * 1. Loads into Illustrator as a .aip plugin
 * 2. Starts an HTTP server on a background thread
 * 3. Receives JSON requests and dispatches them to the main thread
 * 4. Uses HandleManager for safe cross-thread handle management
 */

#include "Plugin.hpp"
#include "ConfigManager.hpp"
#include "Errors.hpp"
#include "EventMapper.hpp"
#include "HandleManager.hpp"
#include "HttpServer.hpp"
#include "MainThreadDispatch.hpp"
#include "MenuHandler.hpp"
#include "SuitePointers.hpp"

#include "IllustratorSDK.h"
#include "AIMenu.h"

#include <cstring>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*******************************************************************************
 * Global Suite Pointers
 *
 * These are acquired during plugin startup and used throughout the plugin.
 * They must be released during shutdown.
 ******************************************************************************/

extern "C" {
SPBasicSuite *sSPBasic = nullptr;
}

// Timer suite for main thread dispatch
static AITimerSuite *sAITimer = nullptr;

// Notifier suite for document/art change notifications
static AINotifierSuite *sAINotifier = nullptr;

/*******************************************************************************
 * Plugin State
 ******************************************************************************/

// Timer handle for main thread dispatch polling
static AITimerHandle gTimerHandle = nullptr;

// Notifier handles for various document events
static AINotifierHandle gArtSelectionChangedNotifier = nullptr;
static AINotifierHandle gArtPropertiesChangedNotifier = nullptr;
static AINotifierHandle gDocumentChangedNotifier = nullptr;
static AINotifierHandle gDocumentClosedNotifier = nullptr;
static AINotifierHandle gDocumentOpenedNotifier = nullptr;
static AINotifierHandle gDocumentNewNotifier = nullptr;
static AINotifierHandle gLayerListChangedNotifier = nullptr;

// Plugin reference (from startup message)
static SPPluginRef gPluginRef = nullptr;

/*******************************************************************************
 * Plugin Entry Point
 *
 * This is the main entry point called by Illustrator for all plugin messages.
 * It dispatches to the appropriate handler based on caller and selector.
 ******************************************************************************/

extern "C" ASAPI ASErr PluginMain(char *caller, char *selector, void *message) {
  ASErr error = kNoErr;

  // Interface messages (startup, shutdown)
  if (std::strcmp(caller, kSPInterfaceCaller) == 0) {
    if (std::strcmp(selector, kSPInterfaceStartupSelector) == 0) {
      error = StartupPlugin(static_cast<SPInterfaceMessage *>(message));
    } else if (std::strcmp(selector, kSPInterfaceShutdownSelector) == 0) {
      error = ShutdownPlugin(static_cast<SPInterfaceMessage *>(message));
    }
  }
  // Access messages (reload, unload)
  else if (std::strcmp(caller, kSPAccessCaller) == 0) {
    if (std::strcmp(selector, kSPAccessReloadSelector) == 0) {
      error = ReloadPlugin(static_cast<SPInterfaceMessage *>(message));
    } else if (std::strcmp(selector, kSPAccessUnloadSelector) == 0) {
      error = UnloadPlugin(static_cast<SPInterfaceMessage *>(message));
    }
  }
  // Timer callback - process main thread work queue
  else if (std::strcmp(caller, kCallerAITimer) == 0) {
    error = HandleTimer(static_cast<AITimerMessage *>(message));
  }
  // Notifier callback - handle document/art change events
  else if (std::strcmp(caller, kCallerAINotify) == 0) {
    error = HandleNotifier(static_cast<AINotifierMessage *>(message));
  }
  // Menu callback - handle menu item selection
  else if (std::strcmp(caller, kCallerAIMenu) == 0) {
    error = MenuHandler::HandleMenu(static_cast<AIMenuMessage *>(message));
  }

  return error;
}

/*******************************************************************************
 * StartupPlugin
 *
 * Called when Illustrator first loads the plugin.
 * Acquires suites, creates timer, registers notifiers, starts HTTP server.
 ******************************************************************************/

ASErr StartupPlugin(SPInterfaceMessage *message) {
  ASErr error = kNoErr;

  // Store basic suite and plugin reference
  sSPBasic = message->d.basic;
  gPluginRef = message->d.self;

  // Acquire Timer suite
  {
    const void *suite = nullptr;
    error = sSPBasic->AcquireSuite(kAITimerSuite, kAITimerSuiteVersion, &suite);
    sAITimer =
        const_cast<AITimerSuite *>(static_cast<const AITimerSuite *>(suite));
  }
  if (error != kNoErr) {
    return error;
  }

  // Acquire Notifier suite
  {
    const void *suite = nullptr;
    error = sSPBasic->AcquireSuite(kAINotifierSuite, kAINotifierSuiteVersion,
                                   &suite);
    sAINotifier = const_cast<AINotifierSuite *>(
        static_cast<const AINotifierSuite *>(suite));
  }
  if (error != kNoErr) {
    sSPBasic->ReleaseSuite(kAITimerSuite, kAITimerSuiteVersion);
    sAITimer = nullptr;
    return error;
  }

  // Create timer for main thread dispatch
  // Period of 1 tick (~16ms at 60 ticks/second) for responsive dispatch
  error = sAITimer->AddTimer(gPluginRef, NUXP_TIMER_NAME, NUXP_TIMER_PERIOD,
                             &gTimerHandle);
  if (error != kNoErr) {
    sSPBasic->ReleaseSuite(kAINotifierSuite, kAINotifierSuiteVersion);
    sSPBasic->ReleaseSuite(kAITimerSuite, kAITimerSuiteVersion);
    sAINotifier = nullptr;
    sAITimer = nullptr;
    return error;
  }

  // Register notifiers for document/art changes
  // These invalidate handles when the document state changes

  // Art selection changed
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Art Selection",
      kAIArtSelectionChangedNotifier, &gArtSelectionChangedNotifier);
  if (error != kNoErr) {
    // Non-fatal - continue without this notifier
    gArtSelectionChangedNotifier = nullptr;
  }

  // Art properties changed (fill, stroke, etc.)
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Art Properties",
      kAIArtPropertiesChangedNotifier, &gArtPropertiesChangedNotifier);
  if (error != kNoErr) {
    gArtPropertiesChangedNotifier = nullptr;
  }

  // Document changed
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Document Changed",
      kAIDocumentChangedNotifier, &gDocumentChangedNotifier);
  if (error != kNoErr) {
    gDocumentChangedNotifier = nullptr;
  }

  // Document closed
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Document Closed",
      kAIDocumentClosedNotifier, &gDocumentClosedNotifier);
  if (error != kNoErr) {
    gDocumentClosedNotifier = nullptr;
  }

  // Document opened
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Document Opened",
      kAIDocumentOpenedNotifier, &gDocumentOpenedNotifier);
  if (error != kNoErr) {
    gDocumentOpenedNotifier = nullptr;
  }

  // Document new (created from scratch, not opened from file)
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Document New",
      kAIDocumentNewNotifier, &gDocumentNewNotifier);
  if (error != kNoErr) {
    gDocumentNewNotifier = nullptr;
  }

  // Layer list changed
  error = sAINotifier->AddNotifier(
      gPluginRef, NUXP_NOTIFIER_NAME " Layer List",
      kAILayerListChangedNotifier, &gLayerListChangedNotifier);
  if (error != kNoErr) {
    gLayerListChangedNotifier = nullptr;
  }

  // Acquire SDK suites for use throughout the plugin
  error = SuitePointers::Acquire();
  if (error != kNoErr) {
    // Non-fatal - some suites may not be available in older Illustrator versions
    // Plugin can still function, but some features may be limited
  }

  // Initialize menu items
  error = MenuHandler::Initialize(gPluginRef);
  if (error != kNoErr) {
    // Non-fatal - plugin works without menu, just no UI for settings
  }

  // Load configuration (creates default if missing)
  ConfigManager::Instance().Load();

  // Start HTTP server on configured port
  int port = ConfigManager::Instance().GetPort();
  HttpServer::Start(port);

  return kNoErr;
}

/*******************************************************************************
 * ShutdownPlugin
 *
 * Called when Illustrator unloads the plugin or quits.
 * Stops HTTP server, removes timers/notifiers, releases suites, cleans handles.
 ******************************************************************************/

ASErr ShutdownPlugin(SPInterfaceMessage *message) {
  // Stop HTTP server first (blocks until stopped)
  HttpServer::Stop();

  // Remove timer
  if (gTimerHandle != nullptr && sAITimer != nullptr) {
    sAITimer->SetTimerActive(gTimerHandle, false);
    // Note: RemoveTimer is not available in all SDK versions
    // The timer will be automatically cleaned up when plugin unloads
    gTimerHandle = nullptr;
  }

  // Remove notifiers
  if (sAINotifier != nullptr) {
    if (gArtSelectionChangedNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gArtSelectionChangedNotifier, false);
      gArtSelectionChangedNotifier = nullptr;
    }
    if (gArtPropertiesChangedNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gArtPropertiesChangedNotifier, false);
      gArtPropertiesChangedNotifier = nullptr;
    }
    if (gDocumentChangedNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gDocumentChangedNotifier, false);
      gDocumentChangedNotifier = nullptr;
    }
    if (gDocumentClosedNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gDocumentClosedNotifier, false);
      gDocumentClosedNotifier = nullptr;
    }
    if (gDocumentOpenedNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gDocumentOpenedNotifier, false);
      gDocumentOpenedNotifier = nullptr;
    }
    if (gDocumentNewNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gDocumentNewNotifier, false);
      gDocumentNewNotifier = nullptr;
    }
    if (gLayerListChangedNotifier != nullptr) {
      sAINotifier->SetNotifierActive(gLayerListChangedNotifier, false);
      gLayerListChangedNotifier = nullptr;
    }
  }

  // Shutdown menu handler
  MenuHandler::Shutdown();

  // Release SDK suites
  SuitePointers::Release();

  // Release infrastructure suites
  if (sSPBasic != nullptr) {
    if (sAINotifier != nullptr) {
      sSPBasic->ReleaseSuite(kAINotifierSuite, kAINotifierSuiteVersion);
      sAINotifier = nullptr;
    }
    if (sAITimer != nullptr) {
      sSPBasic->ReleaseSuite(kAITimerSuite, kAITimerSuiteVersion);
      sAITimer = nullptr;
    }
  }

  // Invalidate all handles - any outstanding handles are now stale
  HandleManager::InvalidateAll();

  // Clear plugin reference
  gPluginRef = nullptr;

  return kNoErr;
}

/*******************************************************************************
 * HandleTimer
 *
 * Called periodically by Illustrator's timer system.
 * Processes the main thread dispatch work queue.
 ******************************************************************************/

ASErr HandleTimer(AITimerMessage *message) {
  // Process all pending work items from the HTTP thread
  MainThreadDispatch::ProcessQueue();
  return kNoErr;
}

/*******************************************************************************
 * HandleNotifier
 *
 * Called when registered events occur (art selection, document changes, etc.)
 * Invalidates handle registries to ensure stale handles aren't used.
 ******************************************************************************/

ASErr HandleNotifier(AINotifierMessage *message) {
  // Invalidate all handles when document state changes
  // This ensures that any handles cached by the frontend are marked stale
  HandleManager::InvalidateAll();

  // Push event to the mapper for the frontend to consume
  if (message != nullptr) {
    // Include action metadata for document lifecycle events
    json eventData = json::object();

    if (message->notifier == gDocumentNewNotifier) {
      eventData["action"] = "created";
    } else if (message->notifier == gDocumentOpenedNotifier) {
      eventData["action"] = "opened";
    } else if (message->notifier == gDocumentClosedNotifier) {
      eventData["action"] = "closed";
    }

    EventMapper::Instance().Push(message->type, eventData);
  }

  return kNoErr;
}

/*******************************************************************************
 * ReloadPlugin
 *
 * Called when the plugin needs to reload (e.g., after preferences change).
 * Re-acquires the basic suite pointer.
 ******************************************************************************/

ASErr ReloadPlugin(SPInterfaceMessage *message) {
  // Update basic suite pointer (may have changed)
  sSPBasic = message->d.basic;
  return kNoErr;
}

/*******************************************************************************
 * UnloadPlugin
 *
 * Called just before the plugin is unloaded from memory.
 * Performs final cleanup.
 ******************************************************************************/

ASErr UnloadPlugin(SPInterfaceMessage *message) {
  // Ensure HTTP server is stopped
  if (HttpServer::IsRunning()) {
    HttpServer::Stop();
  }

  // Final handle cleanup
  HandleManager::InvalidateAll();

  return kNoErr;
}
