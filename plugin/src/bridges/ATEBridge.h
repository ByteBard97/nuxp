#pragma once

/**
 * ATEBridge - Isolated interface to Adobe Text Engine (ATE) suites
 *
 * This header exposes a clean API for ATE operations using only types from
 * AITypes.h (AIArtHandle, AIRealPoint, ASErr, etc.), which are available in
 * every translation unit via IllustratorSDK.h.
 *
 * NO ATE types leak through this header. The implementation (ATEBridge.cpp)
 * is the ONLY file that includes AITextFrame.h and ATESuites.h, compiled as
 * its own translation unit so ATE namespace types don't conflict with
 * anything else in the plugin.
 *
 * This eliminates the need for hand-rolled vtable structs that mirror Adobe's
 * suite layouts with hardcoded version numbers -- a fragile approach that is
 * prone to silent wrong-function-call bugs when versions drift.
 */

#include "AITypes.h"
#include <string>

struct SPBasicSuite;

namespace ATEBridge {

// --- Lifecycle ---
// Call from Plugin startup/shutdown. Uses SPBasicSuite for suite acquisition.
// If not called explicitly, suites are acquired lazily on first use via the
// global sSPBasic pointer.
void AcquireSuites(SPBasicSuite* spBasic);
void ReleaseSuites(SPBasicSuite* spBasic);

// --- Text Frame Creation ---
ASErr NewPointText(
    ai::int16 paintOrder,
    AIArtHandle prep,
    ai::int16 orientation,
    AIRealPoint anchor,
    AIArtHandle* outTextFrame
);

// --- Text Content Operations ---
// All operate on an AIArtHandle that must be a text frame (kTextFrameArt).
// UTF-8 in, UTF-8 out -- bridge handles UTF-8 <-> UTF-16 conversion internally.

bool GetTextContent(AIArtHandle textFrame, std::string& outUtf8, std::string& outError);
bool SetTextContent(AIArtHandle textFrame, const std::string& utf8, std::string& outError);
int32_t GetTextSize(AIArtHandle textFrame);  // Returns -1 on error

} // namespace ATEBridge
