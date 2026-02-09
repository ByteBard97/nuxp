/**
 * SPFiles.h - PICA Files Suite Header (Stub)
 *
 * This is a minimal stub for the Adobe PICA Suite Pea framework.
 * It provides file-related types needed for Illustrator plugin development.
 */

#ifndef __SPFiles__
#define __SPFiles__

#include "SPBasic.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constants
 ******************************************************************************/

#define kSPFilesSuite           "SP Files Suite"
#define kSPFilesSuiteVersion    2

/** Maximum path length */
#define kSPMaxPathLength        1024

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Platform file specification - cross-platform file reference */
typedef struct SPPlatformFileSpecification {
    char path[kSPMaxPathLength];
} SPPlatformFileSpecification;

/** File metadata */
typedef struct SPFileMetadata {
    ai::uint32 creationDate;
    ai::uint32 modificationDate;
    ai::uint32 fileType;
    ai::uint32 creator;
} SPFileMetadata;

/*******************************************************************************
 * SPFilesSuite
 ******************************************************************************/

typedef struct SPFilesSuite {
    /** Get file specification from path */
    ai::int32 (*GetFileSpecificationFromPath)(const char* path, SPPlatformFileSpecification* fileSpec);

    /** Get path from file specification */
    ai::int32 (*GetPathFromFileSpecification)(const SPPlatformFileSpecification* fileSpec, char* path);

    /** Check if file exists */
    ai::int32 (*FileExists)(const SPPlatformFileSpecification* fileSpec, ai::int32* exists);

    /** Get file metadata */
    ai::int32 (*GetFileMetadata)(const SPPlatformFileSpecification* fileSpec, SPFileMetadata* metadata);

} SPFilesSuite;

#ifdef __cplusplus
}
#endif

#endif /* __SPFiles__ */
