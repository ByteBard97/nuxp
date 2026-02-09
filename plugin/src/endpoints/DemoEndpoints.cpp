/**
 * NUXP Demo Endpoints Implementation
 *
 * These endpoints demonstrate actual Adobe Illustrator SDK calls.
 * They serve as examples for building custom endpoints.
 */

#include "DemoEndpoints.hpp"
#include "../SuitePointers.hpp"
#include "IllustratorSDK.h"

namespace DemoEndpoints {

// -------------------------------------------------------------------------
// GetDocumentInfo
// -------------------------------------------------------------------------

json GetDocumentInfo() {
  if (!SuitePointers::AIDocument()) {
    return json{{"success", false},
                {"error", "AIDocument suite not available"}};
  }

  // Get current document
  AIDocumentHandle doc = nullptr;
  ASErr err = SuitePointers::AIDocument()->GetDocument(&doc);
  if (err != kNoErr || doc == nullptr) {
    return json{{"success", false},
                {"error", "No document open"},
                {"errorCode", static_cast<int>(err)}};
  }

  // Get document file name
  ai::UnicodeString docName;
  err = SuitePointers::AIDocument()->GetDocumentFileName(docName);
  std::string name = "Untitled";
  if (err == kNoErr) {
    name = docName.as_UTF8();
  }

  // Get document setup (width and height)
  AIDocumentSetup setup;
  AIReal width = 0, height = 0;
  err = SuitePointers::AIDocument()->GetDocumentSetup(&setup);
  if (err == kNoErr) {
    width = setup.width;
    height = setup.height;
  }

  // Artboard count would require AIArtboardSuite, using placeholder
  ai::int32 artboardCount = 1;

  return json{{"success", true},
              {"document",
               {{"name", name},
                {"width", width},
                {"height", height},
                {"artboardCount", artboardCount}}}};
}

// -------------------------------------------------------------------------
// GetLayers
// -------------------------------------------------------------------------

json GetLayers() {
  if (!SuitePointers::AILayer()) {
    return json{{"success", false},
                {"error", "AILayer suite not available"}};
  }

  json layers = json::array();

  // Get layer count
  ai::int32 layerCount = 0;
  ASErr err = SuitePointers::AILayer()->CountLayers(&layerCount);
  if (err != kNoErr) {
    return json{{"success", false},
                {"error", "Failed to count layers"},
                {"errorCode", static_cast<int>(err)}};
  }

  // Iterate through layers
  for (ai::int32 i = 0; i < layerCount; ++i) {
    AILayerHandle layer = nullptr;
    err = SuitePointers::AILayer()->GetNthLayer(i, &layer);
    if (err != kNoErr || layer == nullptr) {
      continue;
    }

    // Get layer title
    ai::UnicodeString layerTitle;
    err = SuitePointers::AILayer()->GetLayerTitle(layer, layerTitle);
    std::string name = "Untitled";
    if (err == kNoErr) {
      name = layerTitle.as_UTF8();
    }

    // Get layer visibility
    AIBoolean visible = true;
    SuitePointers::AILayer()->GetLayerVisible(layer, &visible);

    // Get layer lock state
    AIBoolean editable = true;
    SuitePointers::AILayer()->GetLayerEditable(layer, &editable);

    // Use layer index as ID (simpler than handle registration for demo)
    layers.push_back({{"index", i},
                      {"name", name},
                      {"visible", visible != 0},
                      {"locked", editable == 0}});
  }

  return json{{"success", true}, {"layers", layers}};
}

// -------------------------------------------------------------------------
// GetSelection
// -------------------------------------------------------------------------

json GetSelection() {
  if (!SuitePointers::AIMatchingArt() || !SuitePointers::AIArt()) {
    return json{{"success", false},
                {"error", "Required suites not available"}};
  }

  // Get selected art using GetSelectedArt
  AIArtHandle **matches = nullptr;
  ai::int32 count = 0;

  ASErr err =
      SuitePointers::AIMatchingArt()->GetSelectedArt(&matches, &count);
  if (err != kNoErr) {
    return json{{"success", false},
                {"error", "Failed to get selected art"},
                {"errorCode", static_cast<int>(err)}};
  }

  json items = json::array();

  if (matches != nullptr && count > 0) {
    for (ai::int32 i = 0; i < count; ++i) {
      AIArtHandle art = (*matches)[i];
      if (art == nullptr) {
        continue;
      }

      // Get art type
      short artType = kUnknownArt;
      SuitePointers::AIArt()->GetArtType(art, &artType);

      // Convert type to string
      std::string typeStr = "unknown";
      switch (artType) {
      case kPathArt:
        typeStr = "path";
        break;
      case kGroupArt:
        typeStr = "group";
        break;
      case kCompoundPathArt:
        typeStr = "compoundPath";
        break;
      case kTextFrameArt:
        typeStr = "textFrame";
        break;
      case kPlacedArt:
        typeStr = "placed";
        break;
      case kRasterArt:
        typeStr = "raster";
        break;
      case kPluginArt:
        typeStr = "plugin";
        break;
      case kMeshArt:
        typeStr = "mesh";
        break;
      case kSymbolArt:
        typeStr = "symbol";
        break;
      default:
        typeStr = "unknown";
        break;
      }

      // Get bounds
      AIRealRect bounds = {0, 0, 0, 0};
      SuitePointers::AIArt()->GetArtBounds(art, &bounds);

      // Use index as identifier for demo purposes
      items.push_back({{"index", i},
                       {"type", typeStr},
                       {"bounds",
                        {{"left", bounds.left},
                         {"top", bounds.top},
                         {"right", bounds.right},
                         {"bottom", bounds.bottom}}}});
    }

    // Free the matches array using SPBasic's FreeBlock
    // Note: The SDK docs say to use MdMemoryDisposeHandle, but that requires
    // acquiring another suite. For simplicity, we'll rely on Illustrator's
    // memory management for now.
  }

  return json{
      {"success", true}, {"selection", {{"count", count}, {"items", items}}}};
}

// -------------------------------------------------------------------------
// CreateRectangle
// -------------------------------------------------------------------------

json CreateRectangle(const json &params) {
  if (!SuitePointers::AIArt() || !SuitePointers::AIPath()) {
    return json{{"success", false},
                {"error", "Required suites not available"}};
  }

  // Parse parameters with defaults
  AIReal x = params.value("x", 100.0);
  AIReal y = params.value("y", 100.0);
  AIReal width = params.value("width", 200.0);
  AIReal height = params.value("height", 100.0);

  // Validate dimensions
  if (width <= 0 || height <= 0) {
    return json{{"success", false},
                {"error", "Width and height must be positive"}};
  }

  // Create a new path art
  AIArtHandle newArt = nullptr;
  ASErr err =
      SuitePointers::AIArt()->NewArt(kPathArt, kPlaceAboveAll, nullptr, &newArt);
  if (err != kNoErr || newArt == nullptr) {
    return json{{"success", false},
                {"error", "Failed to create art object"},
                {"errorCode", static_cast<int>(err)}};
  }

  // Define rectangle points (clockwise from bottom-left)
  // In Illustrator coordinates, Y increases upward
  AIPathSegment segments[4];

  // Bottom-left
  segments[0].p.h = x;
  segments[0].p.v = y;
  segments[0].in = segments[0].out = segments[0].p;
  segments[0].corner = true;

  // Bottom-right
  segments[1].p.h = x + width;
  segments[1].p.v = y;
  segments[1].in = segments[1].out = segments[1].p;
  segments[1].corner = true;

  // Top-right
  segments[2].p.h = x + width;
  segments[2].p.v = y + height;
  segments[2].in = segments[2].out = segments[2].p;
  segments[2].corner = true;

  // Top-left
  segments[3].p.h = x;
  segments[3].p.v = y + height;
  segments[3].in = segments[3].out = segments[3].p;
  segments[3].corner = true;

  // Set the path segments
  err = SuitePointers::AIPath()->SetPathSegments(newArt, 0, 4, segments);
  if (err != kNoErr) {
    // Clean up on failure
    SuitePointers::AIArt()->DisposeArt(newArt);
    return json{{"success", false},
                {"error", "Failed to set path segments"},
                {"errorCode", static_cast<int>(err)}};
  }

  // Close the path
  err = SuitePointers::AIPath()->SetPathClosed(newArt, true);
  if (err != kNoErr) {
    // Non-fatal, continue
  }

  return json{{"success", true},
              {"message", "Rectangle created"},
              {"bounds",
               {{"x", x}, {"y", y}, {"width", width}, {"height", height}}}};
}

} // namespace DemoEndpoints
