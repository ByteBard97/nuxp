/**
 * IllustratorSDK.h - Common Illustrator SDK Header
 *
 * This header provides a convenient way to include all commonly-used
 * Illustrator SDK headers. It should be included by plugin source files
 * that need access to Illustrator types and functions.
 */

#ifndef __IllustratorSDK__
#define __IllustratorSDK__

// PICA Suite Pea headers (required by Illustrator SDK)
#include "SPBasic.h"
#include "SPPlugs.h"
#include "SPFiles.h"
#include "SPInterf.h"
#include "SPAccess.h"
#include "SPProps.h"

// Core Illustrator types
#include "AITypes.h"
#include "AIBasicTypes.h"

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
#include "AIHitTest.h"

// Text
#include "AITextFrame.h"
#include "AIFont.h"

// UI and tools
#include "AITool.h"
#include "AIMenu.h"
#include "AIPanel.h"

// Symbols
#include "AISymbol.h"

// Matching and selection
#include "AIMatchingArt.h"

// Dictionary and data
#include "AIDictionary.h"
#include "AIArray.h"
#include "AIEntry.h"

// File I/O
#include "AIFileFormat.h"
#include "AIFilePath.h"

// Undo
#include "AIUndo.h"

// Context
#include "AIContext.h"

// User interaction
#include "AIUser.h"

// Error handling
#include "AIErrorCodes.h"

#endif /* __IllustratorSDK__ */
