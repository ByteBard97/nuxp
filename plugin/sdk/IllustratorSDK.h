/**
 * IllustratorSDK.h - Common Illustrator SDK Header
 *
 * This header provides a convenient way to include all commonly-used
 * Illustrator SDK headers. It should be included by plugin source files
 * that need access to Illustrator types and functions.
 *
 * NOTE: This is a NUXP convenience header, not part of Adobe's SDK.
 */

#ifndef __IllustratorSDK__
#define __IllustratorSDK__

// Core Illustrator types - MUST be included before PICA headers
// to avoid typedef conflicts with macOS CoreServices
#include "AITypes.h"
#include "AIBasicTypes.h"

// PICA Suite Pea headers (required by Illustrator SDK)
// Note: SPAccess.h imports CoreServices which can conflict with AITypes.h
// if AITypes.h is included after
#include "SPBasic.h"
#include "SPPlugs.h"
#include "SPFiles.h"
#include "SPInterf.h"
#include "SPAccess.h"
#include "SPProps.h"

// Plugin infrastructure
#include "AIPlugin.h"
#include "AITimer.h"
#include "AINotifier.h"

// Document and layer management
#include "AIDocument.h"
#include "AILayer.h"
#include "AILayerList.h"

// Art objects
#include "AIArt.h"
#include "AIArtSet.h"
#include "AIPath.h"
#include "AIGroup.h"
#include "AIRaster.h"

// Colors and styles
#include "AIColor.h"
#include "AIPathStyle.h"
#include "AIGradient.h"
#include "AIPattern.h"
#include "AISwatchList.h"

// Transform and geometry
#include "AIRealMath.h"
#include "AITransformArt.h"

// Selection and matching
#include "AIMatchingArt.h"

// Application context
#include "AIContext.h"

// User interaction
#include "AIUser.h"

// Undo management
#include "AIUndo.h"

// Unicode strings
#include "IAIUnicodeString.h"

// Additional suites for Flora compatibility
#include "AIMdMemory.h"
#include "AIDictionary.h"
#include "AIEntry.h"
#include "AIArtboard.h"
#include "AIDocumentList.h"
#include "AIMask.h"  // Contains AIBlendStyleSuite
#include "AITool.h"
// #include "AIFont.h"  // DISABLED: causes typedef conflicts with ATE types

#endif // __IllustratorSDK__
