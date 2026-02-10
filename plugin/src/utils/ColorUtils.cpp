/**
 * NUXP Color Utilities Implementation
 *
 * Helper functions for working with AIColor and path styling.
 */

#include "ColorUtils.hpp"
#include "../SuitePointers.hpp"

#include <iomanip>
#include <sstream>

namespace ColorUtils {

// ----------------------------------------------------------------------------
// HexToColor
// ----------------------------------------------------------------------------
AIColor HexToColor(const std::string &hex) {
  AIColor color;
  color.Init();

  std::string hexClean = hex;

  // Remove leading '#' if present
  if (!hexClean.empty() && hexClean[0] == '#') {
    hexClean = hexClean.substr(1);
  }

  unsigned int r = 0, g = 0, b = 0;

  if (hexClean.length() == 6) {
    // Parse each pair of hex digits
    std::stringstream ss;

    ss << std::hex << hexClean.substr(0, 2);
    ss >> r;
    ss.clear();

    ss << std::hex << hexClean.substr(2, 2);
    ss >> g;
    ss.clear();

    ss << std::hex << hexClean.substr(4, 2);
    ss >> b;
  }

  color.kind = kThreeColor;
  color.c.rgb.red = static_cast<AIReal>(r) / 255.0;
  color.c.rgb.green = static_cast<AIReal>(g) / 255.0;
  color.c.rgb.blue = static_cast<AIReal>(b) / 255.0;

  return color;
}

// ----------------------------------------------------------------------------
// ColorToHex
// ----------------------------------------------------------------------------
std::string ColorToHex(const AIColor &color) {
  int r = 0, g = 0, b = 0;

  switch (color.kind) {
  case kThreeColor:
    r = static_cast<int>(color.c.rgb.red * 255.0 + 0.5);
    g = static_cast<int>(color.c.rgb.green * 255.0 + 0.5);
    b = static_cast<int>(color.c.rgb.blue * 255.0 + 0.5);
    break;

  case kGrayColor:
    // Convert gray to RGB (same value for all channels)
    r = g = b = static_cast<int>(color.c.g.gray * 255.0 + 0.5);
    break;

  case kFourColor:
    // Simple CMYK to RGB conversion
    // RGB = (1 - C) * (1 - K), etc.
    {
      AIReal c = color.c.f.cyan;
      AIReal m = color.c.f.magenta;
      AIReal y = color.c.f.yellow;
      AIReal k = color.c.f.black;

      r = static_cast<int>((1.0 - c) * (1.0 - k) * 255.0 + 0.5);
      g = static_cast<int>((1.0 - m) * (1.0 - k) * 255.0 + 0.5);
      b = static_cast<int>((1.0 - y) * (1.0 - k) * 255.0 + 0.5);
    }
    break;

  default:
    // For patterns, gradients, or none - return black
    r = g = b = 0;
    break;
  }

  // Clamp values to 0-255
  r = (r < 0) ? 0 : (r > 255) ? 255 : r;
  g = (g < 0) ? 0 : (g > 255) ? 255 : g;
  b = (b < 0) ? 0 : (b > 255) ? 255 : b;

  std::stringstream ss;
  ss << "#" << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
     << r << std::setw(2) << g << std::setw(2) << b;

  return ss.str();
}

// ----------------------------------------------------------------------------
// RGBColor (integer 0-255)
// ----------------------------------------------------------------------------
AIColor RGBColor(int r, int g, int b) {
  AIColor color;
  color.Init();

  color.kind = kThreeColor;
  color.c.rgb.red = static_cast<AIReal>(r) / 255.0;
  color.c.rgb.green = static_cast<AIReal>(g) / 255.0;
  color.c.rgb.blue = static_cast<AIReal>(b) / 255.0;

  return color;
}

// ----------------------------------------------------------------------------
// RGBColorF (float 0.0-1.0)
// ----------------------------------------------------------------------------
AIColor RGBColorF(AIReal r, AIReal g, AIReal b) {
  AIColor color;
  color.Init();

  color.kind = kThreeColor;
  color.c.rgb.red = r;
  color.c.rgb.green = g;
  color.c.rgb.blue = b;

  return color;
}

// ----------------------------------------------------------------------------
// GrayColor
// ----------------------------------------------------------------------------
AIColor GrayColor(AIReal gray) {
  AIColor color;
  color.Init();

  color.kind = kGrayColor;
  color.c.g.gray = gray;

  return color;
}

// ----------------------------------------------------------------------------
// NoColor
// ----------------------------------------------------------------------------
AIColor NoColor() {
  AIColor color;
  color.Init();

  color.kind = kNoneColor;

  return color;
}

// ----------------------------------------------------------------------------
// SetFillColor
// ----------------------------------------------------------------------------
void SetFillColor(AIArtHandle art, const AIColor &color) {
  if (art == nullptr || SuitePointers::AIPathStyle() == nullptr) {
    return;
  }

  AIPathStyle style;
  style.Init();

  AIBoolean hasAdvFill = false;

  // Get current style
  ASErr err =
      SuitePointers::AIPathStyle()->GetPathStyle(art, &style, &hasAdvFill);

  if (err != kNoErr) {
    return;
  }

  // Update fill
  if (color.kind == kNoneColor) {
    style.fillPaint = false;
  } else {
    style.fillPaint = true;
    style.fill.color = color;
    style.fill.overprint = false;
  }

  // Apply updated style
  SuitePointers::AIPathStyle()->SetPathStyle(art, &style);
}

// ----------------------------------------------------------------------------
// SetStrokeColor
// ----------------------------------------------------------------------------
void SetStrokeColor(AIArtHandle art, const AIColor &color) {
  if (art == nullptr || SuitePointers::AIPathStyle() == nullptr) {
    return;
  }

  AIPathStyle style;
  style.Init();

  AIBoolean hasAdvFill = false;

  // Get current style
  ASErr err =
      SuitePointers::AIPathStyle()->GetPathStyle(art, &style, &hasAdvFill);

  if (err != kNoErr) {
    return;
  }

  // Update stroke
  if (color.kind == kNoneColor) {
    style.strokePaint = false;
  } else {
    style.strokePaint = true;
    style.stroke.color = color;
    style.stroke.overprint = false;
  }

  // Apply updated style
  SuitePointers::AIPathStyle()->SetPathStyle(art, &style);
}

// ----------------------------------------------------------------------------
// SetStrokeWidth
// ----------------------------------------------------------------------------
void SetStrokeWidth(AIArtHandle art, AIReal width) {
  if (art == nullptr || SuitePointers::AIPathStyle() == nullptr) {
    return;
  }

  AIPathStyle style;
  style.Init();

  AIBoolean hasAdvFill = false;

  // Get current style
  ASErr err =
      SuitePointers::AIPathStyle()->GetPathStyle(art, &style, &hasAdvFill);

  if (err != kNoErr) {
    return;
  }

  // Update stroke width
  style.stroke.width = width;

  // If stroke wasn't painted before, enable it with a default color
  if (!style.strokePaint) {
    style.strokePaint = true;
    // Default to black stroke if not already set
    if (style.stroke.color.kind == kNoneColor) {
      style.stroke.color.kind = kGrayColor;
      style.stroke.color.c.g.gray = 0.0; // Black
    }
  }

  // Apply updated style
  SuitePointers::AIPathStyle()->SetPathStyle(art, &style);
}

} // namespace ColorUtils
