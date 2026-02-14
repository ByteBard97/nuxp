/**
 * NUXP Suite Pointers Implementation
 *
 * Acquires and releases Adobe Illustrator SDK suites.
 */

#include "SuitePointers.hpp"
#include "AIAssertion.h"
#include "AIFilePath.h"
#include "SPBlocks.h"

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
AIDictionarySuite *SuitePointers::sAIDictionary = nullptr;
AIEntrySuite *SuitePointers::sAIEntry = nullptr;
AIArtboardSuite *SuitePointers::sAIArtboard = nullptr;
AIBlendStyleSuite *SuitePointers::sAIBlendStyle = nullptr;
// AIFontSuite *SuitePointers::sAIFont = nullptr;  // Disabled - ATE header
// conflicts
AIDocumentListSuite *SuitePointers::sAIDocumentList = nullptr;
AIArtSetSuite *SuitePointers::sAIArtSet = nullptr;
AIToolSuite *SuitePointers::sAITool = nullptr;
AIGroupSuite *SuitePointers::sAIGroup = nullptr;
AILayerListSuite *SuitePointers::sAILayerList = nullptr;
AIMaskSuite *SuitePointers::sAIMask = nullptr;
AINotifierSuite *SuitePointers::sAINotifier = nullptr;
AITimerSuite *SuitePointers::sAITimer = nullptr;
AIDocumentViewSuite *SuitePointers::sAIDocumentView = nullptr;

bool SuitePointers::sAcquired = false;

// -------------------------------------------------------------------------
// Global Suite Pointer Aliases for Generated Wrappers
// -------------------------------------------------------------------------
// The code generator produces extern declarations like "extern AIArtSuite*
// sArt" (without the "AI" prefix). These aliases point to the class members.

AIArtSuite *sArt = nullptr;
AIDocumentSuite *sDocument = nullptr;
AILayerSuite *sLayer = nullptr;
AIAppContextSuite *sAppContext = nullptr;
AIUserSuite *sUser = nullptr;
AIUndoSuite *sUndo = nullptr;
AITransformArtSuite *sTransformArt = nullptr;
AIMdMemorySuite *sMdMemory = nullptr;
AIDictionarySuite *sDictionary = nullptr;
AIEntrySuite *sEntry = nullptr;
AIArtboardSuite *sArtboard = nullptr;
AIBlendStyleSuite *sBlendStyle = nullptr;
AIDocumentListSuite *sDocumentList = nullptr;
AIArtSetSuite *sArtSet = nullptr;
AIToolSuite *sTool = nullptr;
AIGroupSuite *sGroup = nullptr;
AILayerListSuite *sLayerList = nullptr;
AIMaskSuite *sMask = nullptr;
AINotifierSuite *sNotifier = nullptr;
AITimerSuite *sTimer = nullptr;
AIDocumentViewSuite *sDocumentView = nullptr;

// -------------------------------------------------------------------------
// Global Suite Pointers for SDK Implementation Files (IAI*.cpp)
// -------------------------------------------------------------------------
// The SDK implementation files (IAIUnicodeString.cpp, IAIFilePath.cpp, etc.)
// expect these specific global names for their suite pointers.

AIUnicodeStringSuite *sAIUnicodeString = nullptr;
AIFilePathSuite *sAIFilePath = nullptr;
SPBlocksSuite *sSPBlocks = nullptr;
// Note: sAIArtboard is also expected by IAIArtboards.cpp - we alias our class
// member
AIArtboardSuite *sAIArtboard = nullptr;
// AIAssertionSuite is used by IAIArtboards.cpp for assertions
AIAssertionSuite *sAIAssertion = nullptr;

// -------------------------------------------------------------------------
// Helper Macro for Suite Acquisition
// -------------------------------------------------------------------------

#define ACQUIRE_SUITE(name, type, version)                                     \
  do {                                                                         \
    const void *suite = nullptr;                                               \
    ASErr err = sSPBasic->AcquireSuite(name, version, &suite);                 \
    if (err == kNoErr && suite != nullptr) {                                   \
      s##type =                                                                \
          const_cast<type##Suite *>(static_cast<const type##Suite *>(suite));  \
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
  ACQUIRE_SUITE(kAIDictionarySuite, AIDictionary, kAIDictionarySuiteVersion);
  ACQUIRE_SUITE(kAIEntrySuite, AIEntry, kAIEntrySuiteVersion);
  ACQUIRE_SUITE(kAIArtboardSuite, AIArtboard, kAIArtboardSuiteVersion);
  ACQUIRE_SUITE(kAIBlendStyleSuite, AIBlendStyle, kAIBlendStyleSuiteVersion);
  // ACQUIRE_SUITE(kAIFontSuite, AIFont, kAIFontSuiteVersion);  // Disabled -
  // ATE conflicts
  ACQUIRE_SUITE(kAIDocumentListSuite, AIDocumentList,
                kAIDocumentListSuiteVersion);
  ACQUIRE_SUITE(kAIArtSetSuite, AIArtSet, kAIArtSetSuiteVersion);
  ACQUIRE_SUITE(kAIToolSuite, AITool, kAIToolSuiteVersion);
  ACQUIRE_SUITE(kAIGroupSuite, AIGroup, kAIGroupSuiteVersion);
  ACQUIRE_SUITE(kAILayerListSuite, AILayerList, kAILayerListSuiteVersion);
  ACQUIRE_SUITE(kAIMaskSuite, AIMask, kAIMaskSuiteVersion);
  ACQUIRE_SUITE(kAINotifierSuite, AINotifier, kAINotifierSuiteVersion);
  ACQUIRE_SUITE(kAITimerSuite, AITimer, kAITimerSuiteVersion);
  ACQUIRE_SUITE(kAIDocumentViewSuite, AIDocumentView,
                kAIDocumentViewSuiteVersion);

  // Acquire SDK implementation suites (for IAI*.cpp files)
  {
    const void *suite = nullptr;
    if (sSPBasic->AcquireSuite(kAIUnicodeStringSuite,
                               kAIUnicodeStringSuiteVersion,
                               &suite) == kNoErr &&
        suite) {
      sAIUnicodeString = const_cast<AIUnicodeStringSuite *>(
          static_cast<const AIUnicodeStringSuite *>(suite));
    }
  }
  {
    const void *suite = nullptr;
    if (sSPBasic->AcquireSuite(kAIFilePathSuite, kAIFilePathSuiteVersion,
                               &suite) == kNoErr &&
        suite) {
      sAIFilePath = const_cast<AIFilePathSuite *>(
          static_cast<const AIFilePathSuite *>(suite));
    }
  }
  {
    const void *suite = nullptr;
    if (sSPBasic->AcquireSuite(kSPBlocksSuite, kSPBlocksSuiteVersion, &suite) ==
            kNoErr &&
        suite) {
      sSPBlocks = const_cast<SPBlocksSuite *>(
          static_cast<const SPBlocksSuite *>(suite));
    }
  }
  {
    const void *suite = nullptr;
    if (sSPBasic->AcquireSuite(kAIAssertionSuite, kAIAssertionSuiteVersion,
                               &suite) == kNoErr &&
        suite) {
      sAIAssertion = const_cast<AIAssertionSuite *>(
          static_cast<const AIAssertionSuite *>(suite));
    }
  }

  // Set global SDK implementation pointers from class members
  // IAIArtboards.cpp expects sAIArtboard as a global variable
  // Use :: to refer to the file-scope global, not the class member
  ::sAIArtboard = SuitePointers::sAIArtboard;

  // Update global aliases for generated wrappers
  sArt = sAIArt;
  sDocument = sAIDocument;
  sLayer = sAILayer;
  sAppContext = sAIAppContext;
  sUser = sAIUser;
  sUndo = sAIUndo;
  sTransformArt = sAITransformArt;
  sMdMemory = sAIMdMemory;
  sDictionary = sAIDictionary;
  sEntry = sAIEntry;
  sArtboard = sAIArtboard;
  sBlendStyle = sAIBlendStyle;
  sDocumentList = sAIDocumentList;
  sArtSet = sAIArtSet;
  sTool = sAITool;
  sGroup = sAIGroup;
  sLayerList = sAILayerList;
  sMask = sAIMask;
  sNotifier = sAINotifier;
  sTimer = sAITimer;
  sDocumentView = sAIDocumentView;

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
  RELEASE_SUITE(kAIDictionarySuite, AIDictionary, kAIDictionarySuiteVersion);
  RELEASE_SUITE(kAIEntrySuite, AIEntry, kAIEntrySuiteVersion);
  RELEASE_SUITE(kAIArtboardSuite, AIArtboard, kAIArtboardSuiteVersion);
  RELEASE_SUITE(kAIBlendStyleSuite, AIBlendStyle, kAIBlendStyleSuiteVersion);
  // RELEASE_SUITE(kAIFontSuite, AIFont, kAIFontSuiteVersion);  // Disabled -
  // ATE conflicts
  RELEASE_SUITE(kAIDocumentListSuite, AIDocumentList,
                kAIDocumentListSuiteVersion);
  RELEASE_SUITE(kAIArtSetSuite, AIArtSet, kAIArtSetSuiteVersion);
  RELEASE_SUITE(kAIToolSuite, AITool, kAIToolSuiteVersion);
  RELEASE_SUITE(kAIGroupSuite, AIGroup, kAIGroupSuiteVersion);
  RELEASE_SUITE(kAILayerListSuite, AILayerList, kAILayerListSuiteVersion);
  RELEASE_SUITE(kAIMaskSuite, AIMask, kAIMaskSuiteVersion);
  RELEASE_SUITE(kAINotifierSuite, AINotifier, kAINotifierSuiteVersion);
  RELEASE_SUITE(kAITimerSuite, AITimer, kAITimerSuiteVersion);
  RELEASE_SUITE(kAIDocumentViewSuite, AIDocumentView,
                kAIDocumentViewSuiteVersion);

  // Release SDK implementation suites
  if (sAIUnicodeString) {
    sSPBasic->ReleaseSuite(kAIUnicodeStringSuite, kAIUnicodeStringSuiteVersion);
    sAIUnicodeString = nullptr;
  }
  if (sAIFilePath) {
    sSPBasic->ReleaseSuite(kAIFilePathSuite, kAIFilePathSuiteVersion);
    sAIFilePath = nullptr;
  }
  if (sSPBlocks) {
    sSPBasic->ReleaseSuite(kSPBlocksSuite, kSPBlocksSuiteVersion);
    sSPBlocks = nullptr;
  }
  if (sAIAssertion) {
    sSPBasic->ReleaseSuite(kAIAssertionSuite, kAIAssertionSuiteVersion);
    sAIAssertion = nullptr;
  }

  // Clear global aliases
  sArt = nullptr;
  sDocument = nullptr;
  sLayer = nullptr;
  sAppContext = nullptr;
  sUser = nullptr;
  sUndo = nullptr;
  sTransformArt = nullptr;
  sMdMemory = nullptr;
  sDictionary = nullptr;
  sEntry = nullptr;
  sArtboard = nullptr;
  sBlendStyle = nullptr;
  sDocumentList = nullptr;
  sArtSet = nullptr;
  sTool = nullptr;
  sGroup = nullptr;
  sLayerList = nullptr;
  sMask = nullptr;
  sNotifier = nullptr;
  sTimer = nullptr;
  sDocumentView = nullptr;

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

AIDictionarySuite *SuitePointers::AIDictionary() { return sAIDictionary; }

AIEntrySuite *SuitePointers::AIEntry() { return sAIEntry; }

AIArtboardSuite *SuitePointers::AIArtboard() { return sAIArtboard; }

AIBlendStyleSuite *SuitePointers::AIBlendStyle() { return sAIBlendStyle; }

// AIFontSuite *SuitePointers::AIFont() { return sAIFont; }  // Disabled - ATE
// conflicts

AIDocumentListSuite *SuitePointers::AIDocumentList() { return sAIDocumentList; }

AIArtSetSuite *SuitePointers::AIArtSet() { return sAIArtSet; }

AIToolSuite *SuitePointers::AITool() { return sAITool; }

AIGroupSuite *SuitePointers::AIGroup() { return sAIGroup; }

AILayerListSuite *SuitePointers::AILayerList() { return sAILayerList; }

AIMaskSuite *SuitePointers::AIMask() { return sAIMask; }

AINotifierSuite *SuitePointers::AINotifier() { return sAINotifier; }

AITimerSuite *SuitePointers::AITimer() { return sAITimer; }

AIDocumentViewSuite *SuitePointers::AIDocumentView() {
  return sAIDocumentView;
}

// -------------------------------------------------------------------------
// Cleanup Macros
// -------------------------------------------------------------------------

#undef ACQUIRE_SUITE
#undef RELEASE_SUITE
