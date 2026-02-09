/**
 * SPAccess.h - PICA Access Suite Header (Stub)
 *
 * This is a minimal stub for the Adobe PICA Suite Pea framework.
 * It provides access-related types needed for Illustrator plugin development.
 */

#ifndef __SPAccess__
#define __SPAccess__

#include "SPBasic.h"
#include "SPFiles.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constants
 ******************************************************************************/

#define kSPAccessSuite          "SP Access Suite"
#define kSPAccessSuiteVersion   2

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Access path reference */
typedef SPAccessRef SPAccessPathRef;

/*******************************************************************************
 * SPAccessSuite
 ******************************************************************************/

typedef struct SPAccessSuite {
    /** Get plugin file specification */
    ai::int32 (*GetPluginFileSpecification)(SPPluginRef plugin, SPPlatformFileSpecification* fileSpec);

    /** Get plugin's resource access */
    ai::int32 (*GetPluginResourceAccess)(SPPluginRef plugin, SPAccessRef* access);

    /** Acquire resource access */
    ai::int32 (*AcquireResourceAccess)(SPPluginRef plugin);

    /** Release resource access */
    ai::int32 (*ReleaseResourceAccess)(SPPluginRef plugin);

} SPAccessSuite;

#ifdef __cplusplus
}
#endif

#endif /* __SPAccess__ */
