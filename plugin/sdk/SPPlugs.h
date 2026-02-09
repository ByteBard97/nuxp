/**
 * SPPlugs.h - PICA Plugins Suite Header (Stub)
 *
 * This is a minimal stub for the Adobe PICA Suite Pea framework.
 * It provides plugin-related types needed for Illustrator plugin development.
 */

#ifndef __SPPlugs__
#define __SPPlugs__

#include "SPBasic.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constants
 ******************************************************************************/

#define kSPPluginsSuite         "SP Plugins Suite"
#define kSPPluginsSuiteVersion  4

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Plugin entry point function prototype */
typedef ai::int32 (*SPPluginEntryFunc)(const char* caller, const char* selector, void* message);

/** Plugin host information */
typedef struct SPPluginHost {
    const char* hostName;
    ai::int32 hostVersion;
} SPPluginHost;

/*******************************************************************************
 * SPPluginsSuite
 ******************************************************************************/

typedef struct SPPluginsSuite {
    /** Get the number of plugins */
    ai::int32 (*CountPlugins)(ai::int32* count);

    /** Get a plugin by index */
    ai::int32 (*GetNthPlugin)(ai::int32 n, SPPluginRef* plugin);

    /** Get a plugin's name */
    ai::int32 (*GetPluginName)(SPPluginRef plugin, const char** name);

    /** Get a plugin's property list */
    ai::int32 (*GetPluginPropertyList)(SPPluginRef plugin, SPPropertyListRef* propertyList);

    /** Set plugin globals */
    ai::int32 (*SetPluginGlobals)(SPPluginRef plugin, void* globals);

    /** Get plugin globals */
    ai::int32 (*GetPluginGlobals)(SPPluginRef plugin, void** globals);

    /** Get plugin started flag */
    ai::int32 (*GetPluginStarted)(SPPluginRef plugin, ai::int32* started);

    /** Set plugin started flag */
    ai::int32 (*SetPluginStarted)(SPPluginRef plugin, ai::int32 started);

    /** Get plugin skiploads flag */
    ai::int32 (*GetPluginSkipShutdown)(SPPluginRef plugin, ai::int32* skipShutdown);

    /** Set plugin skiploads flag */
    ai::int32 (*SetPluginSkipShutdown)(SPPluginRef plugin, ai::int32 skipShutdown);

    /** Get plugin broken flag */
    ai::int32 (*GetPluginBroken)(SPPluginRef plugin, ai::int32* broken);

    /** Set plugin broken flag */
    ai::int32 (*SetPluginBroken)(SPPluginRef plugin, ai::int32 broken);

    /** Get the host associated with this plugin */
    ai::int32 (*GetPluginHost)(SPPluginRef plugin, SPPluginRef* host);

    /** Get the host entry point for a plugin */
    ai::int32 (*GetPluginHostEntry)(SPPluginRef plugin, SPPluginEntryFunc* hostEntry);

} SPPluginsSuite;

#ifdef __cplusplus
}
#endif

#endif /* __SPPlugs__ */
