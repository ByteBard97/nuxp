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
static AIDocumentSuite* sDocument = nullptr;
static AIDocumentListSuite* sDocumentList = nullptr;
static AIArtboardSuite* sArtboard = nullptr;
static AIFontSuite* sFont = nullptr;

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
    return sDocument != nullptr && sDocumentList != nullptr;
  }

  if (sSPBasic == nullptr) {
    return false;
  }

  ACQUIRE_LOCAL_SUITE(kAIDocumentSuite, Document, kAIDocumentSuiteVersion);
  ACQUIRE_LOCAL_SUITE(kAIDocumentListSuite, DocumentList, kAIDocumentListSuiteVersion);
  ACQUIRE_LOCAL_SUITE(kAIArtboardSuite, Artboard, kAIArtboardSuiteVersion);
  ACQUIRE_LOCAL_SUITE(kAIFontSuite, Font, kAIFontSuiteVersion);

  sSuitesAcquired = true;
  return sDocument != nullptr && sDocumentList != nullptr;
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

bool HasDocument() {
  if (!EnsureSuites() || sDocumentList == nullptr) {
    return false;
  }

  ai::int32 docCount = 0;
  ASErr error = sDocumentList->Count(&docCount);

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
  ASErr error = sDocumentList->Count(&docCount);

  if (error != kNoErr || docCount == 0) {
    response["error"] = "no_document";
    return response;
  }

  // Get document file specification
  std::string fileName;
  std::string fullPath;

  ai::FilePath filePath;
  error = sDocument->GetDocumentFileSpecification(filePath);

  if (error == kNoErr) {
    // Get file name
    ai::UnicodeString fileNameUni;
    error = sDocument->GetDocumentFileName(fileNameUni);
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

  if (sArtboard != nullptr) {
    ai::ArtboardList artboardList;
    error = sArtboard->GetArtboardList(artboardList);

    if (error == kNoErr) {
      ai::ArtboardID active = 0;
      sArtboard->GetActive(artboardList, active);

      ai::ArtboardProperties props;
      sArtboard->Init(props);
      error = sArtboard->GetArtboardProperties(artboardList, active, props);

      if (error == kNoErr) {
        AIRealRect bounds;
        sArtboard->GetPosition(props, bounds);
        width = bounds.right - bounds.left;
        height = bounds.top - bounds.bottom;
      }

      sArtboard->Dispose(props);
      sArtboard->ReleaseArtboardList(artboardList);
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
  ASErr error = sDocumentList->Count(&docCount);

  if (error != kNoErr || docCount == 0) {
    response["error"] = "no_document";
    return response;
  }

  if (sArtboard == nullptr) {
    response["error"] = "artboard_suite_not_available";
    return response;
  }

  json artboardsArray = json::array();

  ai::ArtboardList artboardList;
  error = sArtboard->GetArtboardList(artboardList);

  if (error != kNoErr) {
    response["error"] = "could_not_get_artboard_list";
    return response;
  }

  ai::ArtboardID count = 0;
  sArtboard->GetCount(artboardList, count);

  ai::ArtboardID active = 0;
  sArtboard->GetActive(artboardList, active);

  for (ai::ArtboardID i = 0; i < count; i++) {
    ai::ArtboardProperties props;
    sArtboard->Init(props);
    error = sArtboard->GetArtboardProperties(artboardList, i, props);

    if (error == kNoErr) {
      AIRealRect bounds;
      sArtboard->GetPosition(props, bounds);

      ai::UnicodeString nameUni;
      sArtboard->GetName(props, nameUni);
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

    sArtboard->Dispose(props);
  }

  sArtboard->ReleaseArtboardList(artboardList);

  response["artboards"] = artboardsArray;
  response["count"] = static_cast<int>(count);
  response["activeIndex"] = static_cast<int>(active);

  return response;
}

std::string GetRulerUnits() {
  if (!EnsureSuites() || sDocument == nullptr) {
    return "unknown";
  }

  // Check if a document is open
  ai::int32 docCount = 0;
  ASErr error = sDocumentList->Count(&docCount);

  if (error != kNoErr || docCount == 0) {
    return "unknown";
  }

  ai::int16 units = 0;
  error = sDocument->GetDocumentRulerUnits(&units);

  if (error != kNoErr) {
    return "unknown";
  }

  return RulerUnitsToString(units);
}

json GetFonts() {
  json response;

  if (!EnsureSuites()) {
    response["error"] = "suites_not_available";
    return response;
  }

  if (sFont == nullptr) {
    response["error"] = "font_suite_not_available";
    response["fonts"] = json::array();
    response["count"] = 0;
    return response;
  }

  json fontsArray = json::array();

  ai::int32 fontCount = 0;
  ASErr error = sFont->CountFonts(&fontCount);

  if (error != kNoErr) {
    response["error"] = "could_not_count_fonts";
    response["fonts"] = json::array();
    response["count"] = 0;
    return response;
  }

  // Limit to 500 fonts to avoid very long responses
  const ai::int32 maxFonts = 500;
  ai::int32 fontsToReturn = (fontCount < maxFonts) ? fontCount : maxFonts;
  bool truncated = (fontCount > maxFonts);

  for (ai::int32 i = 0; i < fontsToReturn; i++) {
    AIFontKey fontKey;
    error = sFont->IndexFontList(i, &fontKey);

    if (error == kNoErr) {
      char fontNameBuf[256];
      error = sFont->GetUserFontName(fontKey, fontNameBuf, sizeof(fontNameBuf));

      if (error == kNoErr) {
        json font;
        font["name"] = std::string(fontNameBuf);
        fontsArray.push_back(font);
      }
    }
  }

  response["fonts"] = fontsArray;
  response["count"] = fontCount;
  response["truncated"] = truncated;

  return response;
}

} // namespace DocumentUtils
