/**
 * SPInterf.h - PICA Interface Suite Header (Stub)
 *
 * This is a minimal stub for the Adobe PICA Suite Pea framework.
 * It provides interface-related types needed for Illustrator plugin development.
 */

#ifndef __SPInterf__
#define __SPInterf__

#include "SPBasic.h"
#include "SPPlugs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constants - Interface Callers and Selectors
 ******************************************************************************/

/** Interface caller identifier */
#define kSPInterfaceCaller          "SP Interface"

/** Startup selector - sent when plugin is loaded and should initialize */
#define kSPInterfaceStartupSelector     "Startup"

/** Shutdown selector - sent when plugin should clean up and prepare to unload */
#define kSPInterfaceShutdownSelector    "Shutdown"

/** About selector - sent when user requests plugin about dialog */
#define kSPInterfaceAboutSelector       "About"

/*******************************************************************************
 * Constants - Access Callers and Selectors
 ******************************************************************************/

/** Access caller identifier */
#define kSPAccessCaller             "SP Access"

/** Reload selector - sent when plugin should reload its resources */
#define kSPAccessReloadSelector     "Reload"

/** Unload selector - sent when plugin is about to be unloaded from memory */
#define kSPAccessUnloadSelector     "Unload"

/*******************************************************************************
 * Constants - Suite Versions
 ******************************************************************************/

#define kSPInterfaceSuite           "SP Interface Suite"
#define kSPInterfaceSuiteVersion    4

/*******************************************************************************
 * Types
 ******************************************************************************/

/**
 * SPMessageData - Common message data included in all plugin messages
 *
 * This structure is at the beginning of every message passed to a plugin.
 * It provides essential context about the calling environment.
 */
typedef struct SPMessageData {
    /** Version of the message structure */
    ai::int32 SPCheck;

    /** Pointer to the SPBasicSuite for acquiring other suites */
    SPBasicSuite* basic;

    /** Reference to this plugin */
    SPPluginRef self;

    /** Plugin's global data pointer */
    void* globals;

    /** Result code (output) */
    ai::int32 result;

} SPMessageData;

/**
 * SPInterfaceMessage - Message structure for interface calls
 *
 * Used for Startup, Shutdown, and About selectors.
 */
typedef struct SPInterfaceMessage {
    /** Common message data */
    SPMessageData d;

} SPInterfaceMessage;

/**
 * SPAccessMessage - Message structure for access calls
 *
 * Used for Reload and Unload selectors.
 */
typedef struct SPAccessMessage {
    /** Common message data */
    SPMessageData d;

} SPAccessMessage;

/*******************************************************************************
 * SPInterfaceSuite
 ******************************************************************************/

typedef struct SPInterfaceSuite {
    /** Send a message to a plugin */
    ai::int32 (*SendMessage)(SPPluginRef plugin, const char* caller, const char* selector, void* message, ai::int32* result);

    /** Set up a plugin data block for calling another plugin */
    ai::int32 (*SetupMessageData)(SPPluginRef plugin, SPMessageData* messageData);

    /** Clean up a plugin data block after calling another plugin */
    ai::int32 (*EmptyMessageData)(SPPluginRef plugin, SPMessageData* messageData);

} SPInterfaceSuite;

#ifdef __cplusplus
}
#endif

#endif /* __SPInterf__ */
