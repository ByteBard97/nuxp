/**
 * NUXP Selection Utilities Implementation
 *
 * Provides helper functions for working with Adobe Illustrator art selection.
 */

#include "SelectionUtils.hpp"
#include "../SuitePointers.hpp"
#include "IllustratorSDK.h"

namespace SelectionUtils {

// -------------------------------------------------------------------------
// Helper: Convert AIArtType to string
// -------------------------------------------------------------------------

static const char* ArtTypeToString(short type) {
  switch (type) {
    case kGroupArt:           return "group";
    case kPathArt:            return "path";
    case kCompoundPathArt:    return "compoundPath";
    case kPlacedArt:          return "placed";
    case kRasterArt:          return "raster";
    case kPluginArt:          return "plugin";
    case kMeshArt:            return "mesh";
    case kTextFrameArt:       return "textFrame";
    case kSymbolArt:          return "symbol";
    case kForeignArt:         return "foreign";
    case kLegacyTextArt:      return "legacyText";
    case kChartArt:           return "chart";
    case kRadialRepeatArt:    return "radialRepeat";
    case kGridRepeatArt:      return "gridRepeat";
    case kSymmetryArt:        return "symmetry";
    case kConcentricRepeatArt: return "concentricRepeat";
    default:                  return "unknown";
  }
}

// -------------------------------------------------------------------------
// Helper: Dispose memory handle from AIMatchingArt calls
// -------------------------------------------------------------------------

static void DisposeMatchesHandle(AIArtHandle** matches) {
  if (matches && SuitePointers::AIMdMemory()) {
    SuitePointers::AIMdMemory()->MdMemoryDisposeHandle(
        reinterpret_cast<AIMdMemoryHandle>(matches));
  }
}

// -------------------------------------------------------------------------
// GetSelection
// -------------------------------------------------------------------------

json GetSelection() {
  json result = json::array();

  if (!SuitePointers::AIMatchingArt() || !SuitePointers::AIArt()) {
    return result;
  }

  AIArtHandle** matches = nullptr;
  ai::int32 numMatches = 0;

  ASErr err = SuitePointers::AIMatchingArt()->GetSelectedArt(&matches, &numMatches);
  if (err != kNoErr) {
    return result;
  }

  if (matches && numMatches > 0) {
    for (ai::int32 i = 0; i < numMatches; ++i) {
      AIArtHandle art = (*matches)[i];
      if (!art) continue;

      json item;

      // Get art type
      short artType = 0;
      err = SuitePointers::AIArt()->GetArtType(art, &artType);
      if (err == kNoErr) {
        item["type"] = ArtTypeToString(artType);
      } else {
        item["type"] = "unknown";
      }

      // Get art name
      ai::UnicodeString nameUni;
      err = SuitePointers::AIArt()->GetArtName(art, nameUni, nullptr);
      if (err == kNoErr && !nameUni.empty()) {
        item["name"] = nameUni.as_UTF8();
      } else {
        item["name"] = "";
      }

      // Get art bounds
      AIRealRect bounds;
      err = SuitePointers::AIArt()->GetArtBounds(art, &bounds);
      if (err == kNoErr) {
        item["bounds"] = {
          {"left", bounds.left},
          {"top", bounds.top},
          {"right", bounds.right},
          {"bottom", bounds.bottom},
          {"width", bounds.right - bounds.left},
          {"height", bounds.top - bounds.bottom}
        };
      }

      result.push_back(item);
    }

    // Properly dispose of the memory allocated by GetSelectedArt
    DisposeMatchesHandle(matches);
  }

  return result;
}

// -------------------------------------------------------------------------
// GetSelectionCount
// -------------------------------------------------------------------------

int GetSelectionCount() {
  if (!SuitePointers::AIMatchingArt()) {
    return 0;
  }

  AIArtHandle** matches = nullptr;
  ai::int32 numMatches = 0;

  ASErr err = SuitePointers::AIMatchingArt()->GetSelectedArt(&matches, &numMatches);
  if (err != kNoErr) {
    return 0;
  }

  // Properly dispose of the memory allocated by GetSelectedArt
  DisposeMatchesHandle(matches);

  return static_cast<int>(numMatches);
}

// -------------------------------------------------------------------------
// ClearSelection
// -------------------------------------------------------------------------

void ClearSelection() {
  if (!SuitePointers::AIMatchingArt()) {
    return;
  }

  SuitePointers::AIMatchingArt()->DeselectAll();
}

// -------------------------------------------------------------------------
// DeleteSelection
// -------------------------------------------------------------------------

int DeleteSelection() {
  if (!SuitePointers::AIMatchingArt() || !SuitePointers::AIArt() ||
      !SuitePointers::AILayer()) {
    return 0;
  }

  AIArtHandle** matches = nullptr;
  ai::int32 numMatches = 0;

  ASErr err = SuitePointers::AIMatchingArt()->GetSelectedArt(&matches, &numMatches);
  if (err != kNoErr) {
    return 0;
  }

  int deleted = 0;

  if (matches && numMatches > 0) {
    for (ai::int32 i = 0; i < numMatches; ++i) {
      AIArtHandle art = (*matches)[i];
      if (!art) continue;

      // Check if the layer is editable (not locked)
      AILayerHandle layer = nullptr;
      err = SuitePointers::AIArt()->GetLayerOfArt(art, &layer);
      if (err != kNoErr || !layer) continue;

      AIBoolean editable = false;
      SuitePointers::AILayer()->GetLayerEditable(layer, &editable);

      if (editable) {
        err = SuitePointers::AIArt()->DisposeArt(art);
        if (err == kNoErr) {
          ++deleted;
        }
      }
    }

    // Properly dispose of the memory allocated by GetSelectedArt
    DisposeMatchesHandle(matches);
  }

  return deleted;
}

// -------------------------------------------------------------------------
// SelectByLayerName
// -------------------------------------------------------------------------

int SelectByLayerName(const std::string& layerName) {
  if (!SuitePointers::AIMatchingArt() || !SuitePointers::AIArt() ||
      !SuitePointers::AILayer()) {
    return 0;
  }

  // Clear current selection first
  SuitePointers::AIMatchingArt()->DeselectAll();

  // Find the layer by name
  ai::UnicodeString nameUni(layerName);
  AILayerHandle targetLayer = nullptr;
  ASErr err = SuitePointers::AILayer()->GetLayerByTitle(&targetLayer, nameUni);
  if (err != kNoErr || !targetLayer) {
    return 0;
  }

  // Get all art in the document (kAnyArt)
  AIMatchingArtSpec spec;
  spec.type = kAnyArt;
  spec.whichAttr = 0;
  spec.attr = 0;

  AIArtHandle** matches = nullptr;
  ai::int32 numMatches = 0;

  err = SuitePointers::AIMatchingArt()->GetMatchingArt(&spec, 1, &matches, &numMatches);
  if (err != kNoErr) {
    return 0;
  }

  int selectedCount = 0;

  if (matches && numMatches > 0) {
    for (ai::int32 i = 0; i < numMatches; ++i) {
      AIArtHandle art = (*matches)[i];
      if (!art) continue;

      // Get the layer this art belongs to
      AILayerHandle artLayer = nullptr;
      err = SuitePointers::AIArt()->GetLayerOfArt(art, &artLayer);
      if (err != kNoErr || !artLayer) continue;

      // Check if it's on our target layer
      if (artLayer == targetLayer) {
        // Select this art item
        err = SuitePointers::AIArt()->SetArtUserAttr(art, kArtSelected, kArtSelected);
        if (err == kNoErr) {
          ++selectedCount;
        }
      }
    }

    // Properly dispose of the memory allocated by GetMatchingArt
    DisposeMatchesHandle(matches);
  }

  return selectedCount;
}

// -------------------------------------------------------------------------
// SelectArt
// -------------------------------------------------------------------------

void SelectArt(AIArtHandle art) {
  if (!SuitePointers::AIMatchingArt() || !SuitePointers::AIArt() || !art) {
    return;
  }

  // Clear current selection first
  SuitePointers::AIMatchingArt()->DeselectAll();

  // Select the specified art
  SuitePointers::AIArt()->SetArtUserAttr(art, kArtSelected, kArtSelected);
}

} // namespace SelectionUtils
