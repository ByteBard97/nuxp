/**
 * NUXP Color Utilities
 *
 * Helper functions for working with AIColor and path styling.
 * Provides convenient color creation and application functions.
 */

#ifndef NUXP_COLOR_UTILS_HPP
#define NUXP_COLOR_UTILS_HPP

#include "IllustratorSDK.h"
#include <string>

namespace ColorUtils {

/**
 * Convert hex string (#RRGGBB or RRGGBB) to AIColor (RGB).
 *
 * @param hex The hex color string (e.g., "#FF0000" or "FF0000")
 * @return AIColor with kind = kThreeColor and RGB values set
 */
AIColor HexToColor(const std::string &hex);

/**
 * Convert AIColor to hex string (#RRGGBB).
 *
 * @param color The color to convert (should be RGB/kThreeColor)
 * @return Hex string in format "#RRGGBB"
 */
std::string ColorToHex(const AIColor &color);

/**
 * Create RGB color from 0-255 integer values.
 *
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return AIColor with kind = kThreeColor
 */
AIColor RGBColor(int r, int g, int b);

/**
 * Create RGB color from 0.0-1.0 floating point values.
 *
 * @param r Red component (0.0-1.0)
 * @param g Green component (0.0-1.0)
 * @param b Blue component (0.0-1.0)
 * @return AIColor with kind = kThreeColor
 */
AIColor RGBColorF(AIReal r, AIReal g, AIReal b);

/**
 * Create grayscale color.
 *
 * @param gray Gray level (0.0 = black, 1.0 = white)
 * @return AIColor with kind = kGrayColor
 */
AIColor GrayColor(AIReal gray);

/**
 * Get a "no color" (transparent) for stroke or fill.
 *
 * @return AIColor with kind = kNoneColor
 */
AIColor NoColor();

/**
 * Apply fill color to art.
 *
 * @param art The art handle to modify
 * @param color The fill color to apply
 */
void SetFillColor(AIArtHandle art, const AIColor &color);

/**
 * Apply stroke color to art.
 *
 * @param art The art handle to modify
 * @param color The stroke color to apply
 */
void SetStrokeColor(AIArtHandle art, const AIColor &color);

/**
 * Set stroke width on art.
 *
 * @param art The art handle to modify
 * @param width The stroke width in points
 */
void SetStrokeWidth(AIArtHandle art, AIReal width);

} // namespace ColorUtils

#endif // NUXP_COLOR_UTILS_HPP
