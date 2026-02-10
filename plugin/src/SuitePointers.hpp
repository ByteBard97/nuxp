/**
 * NUXP Suite Pointers
 *
 * Centralized management of Adobe Illustrator SDK suite pointers.
 * These suites provide access to Illustrator's functionality and
 * must be acquired during plugin startup and released during shutdown.
 *
 * Usage:
 *   1. Call SuitePointers::Acquire() in StartupPlugin()
 *   2. Use suite pointers via SuitePointers::AIArt(), etc.
 *   3. Call SuitePointers::Release() in ShutdownPlugin()
 */

#ifndef NUXP_SUITE_POINTERS_HPP
#define NUXP_SUITE_POINTERS_HPP

#include "IllustratorSDK.h"

/**
 * Centralized suite pointer management.
 *
 * All Adobe SDK suites are acquired here and made available to the
 * rest of the plugin via static accessor methods.
 */
class SuitePointers {
public:
  /**
   * Acquire all essential suites from Illustrator.
   * Must be called during plugin startup after sSPBasic is set.
   *
   * @return kNoErr on success, error code on failure
   */
  static ASErr Acquire();

  /**
   * Release all acquired suites.
   * Must be called during plugin shutdown.
   */
  static void Release();

  /**
   * Check if all essential suites were successfully acquired.
   */
  static bool IsValid();

  // -------------------------------------------------------------------------
  // Suite Accessors
  // -------------------------------------------------------------------------

  /** Art manipulation suite - create, delete, modify art objects */
  static AIArtSuite *AIArt();

  /** Document suite - document info, settings, iteration */
  static AIDocumentSuite *AIDocument();

  /** Layer suite - layer management and properties */
  static AILayerSuite *AILayer();

  /** Path suite - path geometry and segments */
  static AIPathSuite *AIPath();

  /** Real math suite - mathematical operations on AIReal values */
  static AIRealMathSuite *AIRealMath();

  /** Application suite - app-level info and preferences */
  static AIAppContextSuite *AIAppContext();

  /** Selection suite - manage selected art */
  static AIMatchingArtSuite *AIMatchingArt();

  /** User interaction suite - dialogs and alerts */
  static AIUserSuite *AIUser();

  /** Undo suite - undo transaction management */
  static AIUndoSuite *AIUndo();

  /** Transform suite - geometric transformations */
  static AITransformArtSuite *AITransformArt();

  /** Path style suite - fill and stroke management */
  static AIPathStyleSuite *AIPathStyle();

  /** Memory suite - memory management for SDK allocations */
  static AIMdMemorySuite *AIMdMemory();

private:
  // Prevent instantiation
  SuitePointers() = delete;
  ~SuitePointers() = delete;

  // Suite pointers (defined in SuitePointers.cpp)
  static AIArtSuite *sAIArt;
  static AIDocumentSuite *sAIDocument;
  static AILayerSuite *sAILayer;
  static AIPathSuite *sAIPath;
  static AIRealMathSuite *sAIRealMath;
  static AIAppContextSuite *sAIAppContext;
  static AIMatchingArtSuite *sAIMatchingArt;
  static AIUserSuite *sAIUser;
  static AIUndoSuite *sAIUndo;
  static AITransformArtSuite *sAITransformArt;
  static AIPathStyleSuite *sAIPathStyle;
  static AIMdMemorySuite *sAIMdMemory;

  // Track if suites are acquired
  static bool sAcquired;
};

#endif // NUXP_SUITE_POINTERS_HPP
