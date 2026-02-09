/**
 * SPProps.h - PICA Properties Suite Header (Stub)
 *
 * This is a minimal stub for the Adobe PICA Suite Pea framework.
 * It provides property-related types needed for Illustrator plugin development.
 */

#ifndef __SPProps__
#define __SPProps__

#include "SPBasic.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constants
 ******************************************************************************/

#define kSPPropertiesSuite          "SP Properties Suite"
#define kSPPropertiesSuiteVersion   1

/** Property type codes */
#define kSPPropertyType_None        0
#define kSPPropertyType_Int32       1
#define kSPPropertyType_String      2
#define kSPPropertyType_Block       3

/*******************************************************************************
 * Types
 ******************************************************************************/

/** Property value union */
typedef union SPPropertyValue {
    ai::int32 intValue;
    const char* stringValue;
    void* blockValue;
} SPPropertyValue;

/*******************************************************************************
 * SPPropertiesSuite
 ******************************************************************************/

typedef struct SPPropertiesSuite {
    /** Get property by ID */
    ai::int32 (*AcquireProperty)(SPPluginRef plugin, ai::uint32 vendorID, ai::uint32 propertyKey, ai::int32 propertyID, void** propertyValue);

    /** Release property */
    ai::int32 (*ReleaseProperty)(SPPropertyRef property);

    /** Get property value */
    ai::int32 (*GetPropertyValue)(SPPropertyRef property, void** value);

    /** Count properties in a list */
    ai::int32 (*CountProperties)(SPPropertyListRef propertyList, ai::int32* count);

    /** Get nth property from a list */
    ai::int32 (*GetNthProperty)(SPPropertyListRef propertyList, ai::int32 n, SPPropertyRef* property);

} SPPropertiesSuite;

#ifdef __cplusplus
}
#endif

#endif /* __SPProps__ */
