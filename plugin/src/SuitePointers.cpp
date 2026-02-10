/**
 * NUXP Suite Pointers Implementation
 *
 * Acquires and releases Adobe Illustrator SDK suites.
 */

#include "SuitePointers.hpp"

// External basic suite (set in Plugin.cpp)
extern "C" {
extern SPBasicSuite *sSPBasic;
}

// -------------------------------------------------------------------------
// Static Member Definitions
// -------------------------------------------------------------------------

AIArtSuite *SuitePointers::sAIArt = nullptr;
AIDocumentSuite *SuitePointers::sAIDocument = nullptr;
AILayerSuite *SuitePointers::sAILayer = nullptr;
AIPathSuite *SuitePointers::sAIPath = nullptr;
AIRealMathSuite *SuitePointers::sAIRealMath = nullptr;
AIAppContextSuite *SuitePointers::sAIAppContext = nullptr;
AIMatchingArtSuite *SuitePointers::sAIMatchingArt = nullptr;
AIUserSuite *SuitePointers::sAIUser = nullptr;
AIUndoSuite *SuitePointers::sAIUndo = nullptr;
AITransformArtSuite *SuitePointers::sAITransformArt = nullptr;
AIPathStyleSuite *SuitePointers::sAIPathStyle = nullptr;
AIMdMemorySuite *SuitePointers::sAIMdMemory = nullptr;

bool SuitePointers::sAcquired = false;

// -------------------------------------------------------------------------
// Helper Macro for Suite Acquisition
// -------------------------------------------------------------------------

#define ACQUIRE_SUITE(name, type, version)                                     \
  do {                                                                         \
    const void *suite = nullptr;                                               \
    ASErr err = sSPBasic->AcquireSuite(name, version, &suite);                 \
    if (err == kNoErr && suite != nullptr) {                                   \
      s##type = const_cast<type##Suite *>(                                     \
          static_cast<const type##Suite *>(suite));                            \
    }                                                                          \
  } while (0)

#define RELEASE_SUITE(name, type, version)                                     \
  do {                                                                         \
    if (s##type != nullptr) {                                                  \
      sSPBasic->ReleaseSuite(name, version);                                   \
      s##type = nullptr;                                                       \
    }                                                                          \
  } while (0)

// -------------------------------------------------------------------------
// Acquire / Release
// -------------------------------------------------------------------------

ASErr SuitePointers::Acquire() {
  if (sSPBasic == nullptr) {
    return kBadParameterErr;
  }

  if (sAcquired) {
    return kNoErr; // Already acquired
  }

  // Acquire essential suites
  // Note: We don't fail on individual suite failures - some may not be
  // available in all Illustrator versions. Check IsValid() or individual
  // suite pointers before use.

  ACQUIRE_SUITE(kAIArtSuite, AIArt, kAIArtSuiteVersion);
  ACQUIRE_SUITE(kAIDocumentSuite, AIDocument, kAIDocumentSuiteVersion);
  ACQUIRE_SUITE(kAILayerSuite, AILayer, kAILayerSuiteVersion);
  ACQUIRE_SUITE(kAIPathSuite, AIPath, kAIPathSuiteVersion);
  ACQUIRE_SUITE(kAIRealMathSuite, AIRealMath, kAIRealMathSuiteVersion);
  ACQUIRE_SUITE(kAIAppContextSuite, AIAppContext, kAIAppContextSuiteVersion);
  ACQUIRE_SUITE(kAIMatchingArtSuite, AIMatchingArt, kAIMatchingArtSuiteVersion);
  ACQUIRE_SUITE(kAIUserSuite, AIUser, kAIUserSuiteVersion);
  ACQUIRE_SUITE(kAIUndoSuite, AIUndo, kAIUndoSuiteVersion);
  ACQUIRE_SUITE(kAITransformArtSuite, AITransformArt,
                kAITransformArtSuiteVersion);
  ACQUIRE_SUITE(kAIPathStyleSuite, AIPathStyle, kAIPathStyleSuiteVersion);
  ACQUIRE_SUITE(kAIMdMemorySuite, AIMdMemory, kAIMdMemorySuiteVersion);

  sAcquired = true;
  return kNoErr;
}

void SuitePointers::Release() {
  if (!sAcquired || sSPBasic == nullptr) {
    return;
  }

  RELEASE_SUITE(kAIArtSuite, AIArt, kAIArtSuiteVersion);
  RELEASE_SUITE(kAIDocumentSuite, AIDocument, kAIDocumentSuiteVersion);
  RELEASE_SUITE(kAILayerSuite, AILayer, kAILayerSuiteVersion);
  RELEASE_SUITE(kAIPathSuite, AIPath, kAIPathSuiteVersion);
  RELEASE_SUITE(kAIRealMathSuite, AIRealMath, kAIRealMathSuiteVersion);
  RELEASE_SUITE(kAIAppContextSuite, AIAppContext, kAIAppContextSuiteVersion);
  RELEASE_SUITE(kAIMatchingArtSuite, AIMatchingArt, kAIMatchingArtSuiteVersion);
  RELEASE_SUITE(kAIUserSuite, AIUser, kAIUserSuiteVersion);
  RELEASE_SUITE(kAIUndoSuite, AIUndo, kAIUndoSuiteVersion);
  RELEASE_SUITE(kAITransformArtSuite, AITransformArt,
                kAITransformArtSuiteVersion);
  RELEASE_SUITE(kAIPathStyleSuite, AIPathStyle, kAIPathStyleSuiteVersion);
  RELEASE_SUITE(kAIMdMemorySuite, AIMdMemory, kAIMdMemorySuiteVersion);

  sAcquired = false;
}

bool SuitePointers::IsValid() {
  // Check that at minimum the core suites are available
  return sAcquired && sAIArt != nullptr && sAIDocument != nullptr;
}

// -------------------------------------------------------------------------
// Suite Accessors
// -------------------------------------------------------------------------

AIArtSuite *SuitePointers::AIArt() { return sAIArt; }

AIDocumentSuite *SuitePointers::AIDocument() { return sAIDocument; }

AILayerSuite *SuitePointers::AILayer() { return sAILayer; }

AIPathSuite *SuitePointers::AIPath() { return sAIPath; }

AIRealMathSuite *SuitePointers::AIRealMath() { return sAIRealMath; }

AIAppContextSuite *SuitePointers::AIAppContext() { return sAIAppContext; }

AIMatchingArtSuite *SuitePointers::AIMatchingArt() { return sAIMatchingArt; }

AIUserSuite *SuitePointers::AIUser() { return sAIUser; }

AIUndoSuite *SuitePointers::AIUndo() { return sAIUndo; }

AITransformArtSuite *SuitePointers::AITransformArt() { return sAITransformArt; }

AIPathStyleSuite *SuitePointers::AIPathStyle() { return sAIPathStyle; }

AIMdMemorySuite *SuitePointers::AIMdMemory() { return sAIMdMemory; }

// -------------------------------------------------------------------------
// Cleanup Macros
// -------------------------------------------------------------------------

#undef ACQUIRE_SUITE
#undef RELEASE_SUITE
