#pragma once

#include "AITypes.h"
#include "SPErrorCodes.h"

#include <string>

namespace Errors {

/**
 * Convert an AIErr error code to a human-readable string.
 * Covers both SDK errors and Illustrator-specific error codes.
 *
 * @param error The error code to convert
 * @return A descriptive string for the error
 */
std::string AIErrToString(AIErr error);

/**
 * Check if an error code indicates success.
 *
 * @param error The error code to check
 * @return true if the error code is kNoErr (success)
 */
bool IsSuccess(AIErr error);

/**
 * Get the error code as an integer for JSON serialization.
 *
 * @param error The error code
 * @return The error code as an integer
 */
inline int ErrorCode(AIErr error) {
    return static_cast<int>(error);
}

} // namespace Errors
