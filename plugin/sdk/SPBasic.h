/**
 * SPBasic.h - PICA Basic Suite Header (Stub)
 *
 * This is a minimal stub for the Adobe PICA Suite Pea framework.
 * It provides the basic types needed for Illustrator plugin development.
 */

#ifndef __SPBasic__
#define __SPBasic__

#include "AIBasicTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Error Codes
 ******************************************************************************/

#define kSPNoError                      0
#define kSPUnimplementedError           'UIMP'
#define kSPUserCanceledError            'STOP'
#define kSPOperationInterrupted         'INTR'
#define kSPOutOfMemoryError             '!MEM'
#define kSPBufferTooSmallError          'SMAL'
#define kSPBadParameterError            'PARM'
#define kSPBlockDebugError              'DBUG'
#define kSPBlockInUseError              'BUSE'
#define kSPPluginNotFound               'P!FD'
#define kSPCorruptPStringError          'PSTR'
#define kSPPluginCachesFlushResponse    'FLSH'
#define kSPPluginCouldntLoad            'LOAD'
#define kSPCantChangeProperty           'PROP'
#define kSPSuiteNotFoundError           'S!FD'

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Opaque reference to a plugin */
typedef struct SPPlugin* SPPluginRef;

/** Opaque reference to a suite */
typedef struct SPSuite* SPSuiteRef;

/** Opaque reference to a property list */
typedef struct SPPropertyList* SPPropertyListRef;

/** Opaque reference to a property */
typedef struct SPProperty* SPPropertyRef;

/** Opaque reference to an access path */
typedef struct SPAccess* SPAccessRef;

/** Opaque reference to a file */
typedef struct SPFile* SPFileRef;

/** Opaque reference to a file list */
typedef struct SPFileList* SPFileListRef;

/** Opaque reference to a platform file specification */
typedef struct SPPlatformFileSpecification* SPPlatformFileSpecificationRef;

/** Plugin list iterator */
typedef struct SPPluginListIterator* SPPluginListIteratorRef;

/*******************************************************************************
 * SPBasicSuite - The fundamental suite for acquiring/releasing other suites
 ******************************************************************************/

#define kSPBasicSuite           "SP Basic Suite"
#define kSPBasicSuiteVersion    4

typedef struct SPBasicSuite {
    /** Acquire a suite by name and version */
    ai::int32 (*AcquireSuite)(const char* name, ai::int32 version, const void** suite);

    /** Release a previously acquired suite */
    ai::int32 (*ReleaseSuite)(const char* name, ai::int32 version);

    /** Check if a suite is available */
    ai::int32 (*IsEqual)(const char* token1, const char* token2);

    /** Allocate memory */
    ai::int32 (*AllocateBlock)(size_t size, void** block);

    /** Free memory */
    ai::int32 (*FreeBlock)(void* block);

    /** Reallocate memory */
    ai::int32 (*ReallocateBlock)(void* block, size_t size, void** newBlock);

    /** Report an error to the user */
    ai::int32 (*Undefined)(void);

} SPBasicSuite;

#ifdef __cplusplus
}
#endif

#endif /* __SPBasic__ */
