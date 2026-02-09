/**
 * NUXP Plugin - Main Entry Point
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
#include "HttpServer.hpp"
#include "HandleManager.hpp"
#include "MainThreadDispatch.hpp"
#include "Errors.hpp"

#include "IllustratorSDK.h"

#include <cstring>

/*******************************************************************************
 * Global Suite Pointers
 *
 * These are acquired during plugin startup and used throughout the plugin.
 * They must be released during shutdown.
 ******************************************************************************/

extern "C" {
    SPBasicSuite* sSPBasic = nullptr;
}

// Timer suite for main thread dispatch
static AITimerSuite* sAITimer = nullptr;

// Notifier suite for document/art change notifications
static AINotifierSuite* sAINotifier = nullptr;

/*******************************************************************************
 * Plugin State
 ******************************************************************************/

// Timer handle for main thread dispatch polling
static AITimerHandle gTimerHandle = nullptr;

// Notifier handles for various document events
static AINotifierHandle gArtSelectionChangedNotifier = nullptr;
static AINotifierHandle gDocumentChangedNotifier = nullptr;
static AINotifierHandle gDocumentClosedNotifier = nullptr;
static AINotifierHandle gDocumentOpenedNotifier = nullptr;

// Plugin reference (from startup message)
static SPPluginRef gPluginRef = nullptr;

/*******************************************************************************
 * Plugin Entry Point
 *
 * This is the main entry point called by Illustrator for all plugin messages.
 * It dispatches to the appropriate handler based on caller and selector.
 ******************************************************************************/

extern "C" ASAPI ASErr PluginMain(char* caller, char* selector, void* message) {
    ASErr error = kNoErr;

    // Interface messages (startup, shutdown)
    if (std::strcmp(caller, kSPInterfaceCaller) == 0) {
        if (std::strcmp(selector, kSPInterfaceStartupSelector) == 0) {
            error = StartupPlugin(static_cast<SPInterfaceMessage*>(message));
        }
        else if (std::strcmp(selector, kSPInterfaceShutdownSelector) == 0) {
            error = ShutdownPlugin(static_cast<SPInterfaceMessage*>(message));
        }
    }
    // Access messages (reload, unload)
    else if (std::strcmp(caller, kSPAccessCaller) == 0) {
        if (std::strcmp(selector, kSPAccessReloadSelector) == 0) {
            error = ReloadPlugin(static_cast<SPInterfaceMessage*>(message));
        }
        else if (std::strcmp(selector, kSPAccessUnloadSelector) == 0) {
            error = UnloadPlugin(static_cast<SPInterfaceMessage*>(message));
        }
    }
    // Timer callback - process main thread work queue
    else if (std::strcmp(caller, kCallerAITimer) == 0) {
        error = HandleTimer(static_cast<AITimerMessage*>(message));
    }
    // Notifier callback - handle document/art change events
    else if (std::strcmp(caller, kCallerAINotify) == 0) {
        error = HandleNotifier(static_cast<AINotifierMessage*>(message));
    }

    return error;
}

/*******************************************************************************
 * StartupPlugin
 *
 * Called when Illustrator first loads the plugin.
 * Acquires suites, creates timer, registers notifiers, starts HTTP server.
 ******************************************************************************/

ASErr StartupPlugin(SPInterfaceMessage* message) {
    ASErr error = kNoErr;

    // Store basic suite and plugin reference
    sSPBasic = message->d.basic;
    gPluginRef = message->d.self;

    // Acquire Timer suite
    error = sSPBasic->AcquireSuite(
        kAITimerSuite,
        kAITimerSuiteVersion,
        reinterpret_cast<const void**>(&sAITimer)
    );
    if (error != kNoErr) {
        return error;
    }

    // Acquire Notifier suite
    error = sSPBasic->AcquireSuite(
        kAINotifierSuite,
        kAINotifierSuiteVersion,
        reinterpret_cast<const void**>(&sAINotifier)
    );
    if (error != kNoErr) {
        sSPBasic->ReleaseSuite(kAITimerSuite, kAITimerSuiteVersion);
        sAITimer = nullptr;
        return error;
    }

    // Create timer for main thread dispatch
    // Period of 1 tick (~16ms at 60 ticks/second) for responsive dispatch
    error = sAITimer->AddTimer(
        gPluginRef,
        NUXP_TIMER_NAME,
        NUXP_TIMER_PERIOD,
        &gTimerHandle
    );
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
        gPluginRef,
        NUXP_NOTIFIER_NAME " Art Selection",
        kAIArtSelectionChangedNotifier,
        &gArtSelectionChangedNotifier
    );
    if (error != kNoErr) {
        // Non-fatal - continue without this notifier
        gArtSelectionChangedNotifier = nullptr;
    }

    // Document changed
    error = sAINotifier->AddNotifier(
        gPluginRef,
        NUXP_NOTIFIER_NAME " Document Changed",
        kAIDocumentChangedNotifier,
        &gDocumentChangedNotifier
    );
    if (error != kNoErr) {
        gDocumentChangedNotifier = nullptr;
    }

    // Document closed
    error = sAINotifier->AddNotifier(
        gPluginRef,
        NUXP_NOTIFIER_NAME " Document Closed",
        kAIDocumentClosedNotifier,
        &gDocumentClosedNotifier
    );
    if (error != kNoErr) {
        gDocumentClosedNotifier = nullptr;
    }

    // Document opened
    error = sAINotifier->AddNotifier(
        gPluginRef,
        NUXP_NOTIFIER_NAME " Document Opened",
        kAIDocumentOpenedNotifier,
        &gDocumentOpenedNotifier
    );
    if (error != kNoErr) {
        gDocumentOpenedNotifier = nullptr;
    }

    // Start HTTP server on background thread
    HttpServer::Start(NUXP_DEFAULT_PORT);

    return kNoErr;
}

/*******************************************************************************
 * ShutdownPlugin
 *
 * Called when Illustrator unloads the plugin or quits.
 * Stops HTTP server, removes timers/notifiers, releases suites, cleans handles.
 ******************************************************************************/

ASErr ShutdownPlugin(SPInterfaceMessage* message) {
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
    }

    // Release suites
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

ASErr HandleTimer(AITimerMessage* message) {
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

ASErr HandleNotifier(AINotifierMessage* message) {
    // Invalidate all handles when document state changes
    // This ensures that any handles cached by the frontend are marked stale
    HandleManager::InvalidateAll();
    return kNoErr;
}

/*******************************************************************************
 * ReloadPlugin
 *
 * Called when the plugin needs to reload (e.g., after preferences change).
 * Re-acquires the basic suite pointer.
 ******************************************************************************/

ASErr ReloadPlugin(SPInterfaceMessage* message) {
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

ASErr UnloadPlugin(SPInterfaceMessage* message) {
    // Ensure HTTP server is stopped
    if (HttpServer::IsRunning()) {
        HttpServer::Stop();
    }

    // Final handle cleanup
    HandleManager::InvalidateAll();

    return kNoErr;
}
