/**
 * NUXP String Utilities
 *
 * Helper functions for converting between std::string and Adobe Illustrator
 * SDK string types (ai::UnicodeString). Wraps the awkward SDK string handling
 * into a simple, consistent interface.
 *
 * Usage:
 *   #include "utils/StringUtils.hpp"
 *
 *   std::string name = StringUtils::GetArtName(artHandle);
 *   StringUtils::SetArtName(artHandle, "New Name");
 *   ai::UnicodeString uni = StringUtils::ToUnicode("hello");
 */

#ifndef NUXP_STRING_UTILS_HPP
#define NUXP_STRING_UTILS_HPP

#include "IllustratorSDK.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace StringUtils {

/**
 * Convert std::string (UTF-8) to ai::UnicodeString.
 *
 * @param str  UTF-8 encoded string
 * @return     Equivalent ai::UnicodeString
 */
ai::UnicodeString ToUnicode(const std::string& str);

/**
 * Convert ai::UnicodeString to std::string (UTF-8).
 *
 * @param str  ai::UnicodeString to convert
 * @return     UTF-8 encoded std::string
 */
std::string FromUnicode(const ai::UnicodeString& str);

/**
 * Get the name of an art item as a std::string.
 *
 * @param art  Handle to the art item
 * @return     Art item name, or empty string if art is null or unnamed
 */
std::string GetArtName(AIArtHandle art);

/**
 * Set the name of an art item from a std::string.
 *
 * @param art   Handle to the art item
 * @param name  Name to set (UTF-8)
 */
void SetArtName(AIArtHandle art, const std::string& name);

/**
 * Get the title/name of a layer as a std::string.
 *
 * @param layer  Handle to the layer
 * @return       Layer name, or empty string if layer is null
 */
std::string GetLayerName(AILayerHandle layer);

/**
 * Format an AIReal value to a string with specified precision.
 *
 * @param value      The AIReal value to format
 * @param precision  Number of decimal places (default: 2)
 * @return           Formatted string representation
 */
std::string FormatReal(AIReal value, int precision = 2);

} // namespace StringUtils

#endif // NUXP_STRING_UTILS_HPP
