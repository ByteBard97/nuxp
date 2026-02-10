/**
 * DocumentUtils Implementation
 *
 * Provides utility functions for retrieving document information from
 * Adobe Illustrator. These functions directly call SDK APIs and must
 * be executed on the main thread.
 */

#include "DocumentUtils.hpp"
#include "IllustratorSDK.h"

// External suite pointers (acquired by generated wrappers or Plugin.cpp)
extern "C" SPBasicSuite* sSPBasic;

// Local suite pointers for document utilities
static AIDocumentSuite* sAIDocument = nullptr;
static AIDocumentListSuite* sAIDocumentList = nullptr;
static AIArtboardSuite* sAIArtboard = nullptr;
// static AIFontSuite* sAIFont = nullptr;  // Disabled - ATE header conflicts

// Track if suites are acquired
static bool sSuitesAcquired = false;

namespace {

/**
 * Helper macro for suite acquisition
 */
#define ACQUIRE_LOCAL_SUITE(name, type, version)                               \
  do {                                                                         \
    const void* suite = nullptr;                                               \
    ASErr err = sSPBasic->AcquireSuite(name, version, &suite);                 \
    if (err == kNoErr && suite != nullptr) {                                   \
      s##type = const_cast<type##Suite*>(                                      \
          static_cast<const type##Suite*>(suite));                             \
    }                                                                          \
  } while (0)

/**
 * Ensure all required suites are acquired.
 * Called lazily on first use of any DocumentUtils function.
 */
bool EnsureSuites() {
  if (sSuitesAcquired) {
    return sAIDocument != nullptr && sAIDocumentList != nullptr;
  }

  if (sSPBasic == nullptr) {
    return false;
  }

  ACQUIRE_LOCAL_SUITE(kAIDocumentSuite, AIDocument, kAIDocumentSuiteVersion);
  ACQUIRE_LOCAL_SUITE(kAIDocumentListSuite, AIDocumentList, kAIDocumentListSuiteVersion);
  ACQUIRE_LOCAL_SUITE(kAIArtboardSuite, AIArtboard, kAIArtboardSuiteVersion);
  // ACQUIRE_LOCAL_SUITE(kAIFontSuite, AIFont, kAIFontSuiteVersion);  // Disabled - ATE conflicts

  sSuitesAcquired = true;
  return sAIDocument != nullptr && sAIDocumentList != nullptr;
}

#undef ACQUIRE_LOCAL_SUITE

/**
 * Convert ruler units code to string representation.
 */
const char* RulerUnitsToString(ai::int16 units) {
  switch (units) {
    case kInchesUnits:
      return "inches";
    case kPointsUnits:
      return "points";
    case kCentimetersUnits:
      return "centimeters";
    case kMillimetersUnits:
      return "millimeters";
    case kPicasUnits:
      return "picas";
    case kPixelsUnits:
      return "pixels";
    default:
      return "unknown";
  }
}

/**
 * Escape special characters in a string for JSON.
 */
std::string EscapeJsonString(const std::string& input) {
  std::string output;
  output.reserve(input.size() + 16);

  for (char c : input) {
    switch (c) {
      case '"':
        output += "\\\"";
        break;
      case '\\':
        output += "\\\\";
        break;
      case '\b':
        output += "\\b";
        break;
      case '\f':
        output += "\\f";
        break;
      case '\n':
        output += "\\n";
        break;
      case '\r':
        output += "\\r";
        break;
      case '\t':
        output += "\\t";
        break;
      default:
        if (static_cast<unsigned char>(c) < 0x20) {
          // Control character - escape as \uXXXX
          char buf[8];
          snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
          output += buf;
        } else {
          output += c;
        }
        break;
    }
  }
  return output;
}

} // anonymous namespace

namespace DocumentUtils {

bool HasAIDocument() {
  if (!EnsureSuites() || sAIDocumentList == nullptr) {
    return false;
  }

  ai::int32 docCount = 0;
  ASErr error = sAIDocumentList->Count(&docCount);

  return (error == kNoErr && docCount > 0);
}

json GetDocumentInfo() {
  json response;

  if (!EnsureSuites()) {
    response["error"] = "suites_not_available";
    return response;
  }

  // Check if a document is open
  ai::int32 docCount = 0;
  ASErr error = sAIDocumentList->Count(&docCount);

  if (error != kNoErr || docCount == 0) {
    response["error"] = "no_document";
    return response;
  }

  // Get document file specification
  std::string fileName;
  std::string fullPath;

  ai::FilePath filePath;
  error = sAIDocument->GetDocumentFileSpecification(filePath);

  if (error == kNoErr) {
    // Get file name
    ai::UnicodeString fileNameUni;
    error = sAIDocument->GetDocumentFileName(fileNameUni);
    if (error == kNoErr) {
      fileName = fileNameUni.as_UTF8();
    }

    // Get full path
    ai::UnicodeString fullPathUni = filePath.GetFullPath();
    fullPath = fullPathUni.as_UTF8();
  }

  // Get document dimensions from active artboard
  AIReal width = 612.0;  // Default letter size
  AIReal height = 792.0;

  if (sAIArtboard != nullptr) {
    ai::ArtboardList artboardList;
    error = sAIArtboard->GetArtboardList(artboardList);

    if (error == kNoErr) {
      ai::ArtboardID active = 0;
      sAIArtboard->GetActive(artboardList, active);

      ai::ArtboardProperties props;
      sAIArtboard->Init(props);
      error = sAIArtboard->GetArtboardProperties(artboardList, active, props);

      if (error == kNoErr) {
        AIRealRect bounds;
        sAIArtboard->GetPosition(props, bounds);
        width = bounds.right - bounds.left;
        height = bounds.top - bounds.bottom;
      }

      sAIArtboard->Dispose(props);
      sAIArtboard->ReleaseArtboardList(artboardList);
    }
  }

  // Get ruler units
  std::string units = GetRulerUnits();

  // Build response
  response["name"] = fileName;
  response["path"] = fullPath;
  response["width"] = width;
  response["height"] = height;
  response["units"] = units;

  return response;
}

json GetArtboards() {
  json response;

  if (!EnsureSuites()) {
    response["error"] = "suites_not_available";
    return response;
  }

  // Check if a document is open
  ai::int32 docCount = 0;
  ASErr error = sAIDocumentList->Count(&docCount);

  if (error != kNoErr || docCount == 0) {
    response["error"] = "no_document";
    return response;
  }

  if (sAIArtboard == nullptr) {
    response["error"] = "artboard_suite_not_available";
    return response;
  }

  json artboardsArray = json::array();

  ai::ArtboardList artboardList;
  error = sAIArtboard->GetArtboardList(artboardList);

  if (error != kNoErr) {
    response["error"] = "could_not_get_artboard_list";
    return response;
  }

  ai::ArtboardID count = 0;
  sAIArtboard->GetCount(artboardList, count);

  ai::ArtboardID active = 0;
  sAIArtboard->GetActive(artboardList, active);

  for (ai::ArtboardID i = 0; i < count; i++) {
    ai::ArtboardProperties props;
    sAIArtboard->Init(props);
    error = sAIArtboard->GetArtboardProperties(artboardList, i, props);

    if (error == kNoErr) {
      AIRealRect bounds;
      sAIArtboard->GetPosition(props, bounds);

      ai::UnicodeString nameUni;
      sAIArtboard->GetName(props, nameUni);
      std::string name = nameUni.as_UTF8();

      json artboard;
      artboard["index"] = static_cast<int>(i);
      artboard["name"] = name;
      artboard["bounds"]["left"] = bounds.left;
      artboard["bounds"]["top"] = bounds.top;
      artboard["bounds"]["right"] = bounds.right;
      artboard["bounds"]["bottom"] = bounds.bottom;
      artboard["active"] = (i == active);

      artboardsArray.push_back(artboard);
    }

    sAIArtboard->Dispose(props);
  }

  sAIArtboard->ReleaseArtboardList(artboardList);

  response["artboards"] = artboardsArray;
  response["count"] = static_cast<int>(count);
  response["activeIndex"] = static_cast<int>(active);

  return response;
}

std::string GetRulerUnits() {
  if (!EnsureSuites() || sAIDocument == nullptr) {
    return "unknown";
  }

  // Check if a document is open
  ai::int32 docCount = 0;
  ASErr error = sAIDocumentList->Count(&docCount);

  if (error != kNoErr || docCount == 0) {
    return "unknown";
  }

  ai::int16 units = 0;
  error = sAIDocument->GetDocumentRulerUnits(&units);

  if (error != kNoErr) {
    return "unknown";
  }

  return RulerUnitsToString(units);
}

json GetFonts() {
  json response;
  // AIFontSuite is disabled due to ATE header conflicts
  // Font enumeration requires special handling to avoid typedef conflicts
  response["error"] = "font_suite_disabled";
  response["message"] = "AIFontSuite disabled due to ATE header conflicts";
  response["fonts"] = json::array();
  response["count"] = 0;
  return response;
}

} // namespace DocumentUtils
