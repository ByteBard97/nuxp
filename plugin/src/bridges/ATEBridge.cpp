/**
 * ATEBridge.cpp - Isolated compilation unit for ATE headers
 *
 * This file is the ONLY place in the plugin that includes AITextFrame.h
 * and ATESuites.h. It is compiled as its own translation unit, so the ATE
 * namespace types don't conflict with anything included via IllustratorSDK.h.
 *
 * DO NOT include IllustratorSDK.h here. AITextFrame.h is self-consistent
 * and transitively includes AITypes.h, AIArt.h, AIFont.h, and ATESuites.h.
 *
 * This replaces the ~320 lines of hand-rolled vtable structs that previously
 * lived in TextEndpoints.cpp with hardcoded version numbers.
 */

#include "ATEBridge.h"

// Real Adobe SDK headers -- safe because this is an isolated TU.
// AITextFrame.h transitively includes:
//   AITypes.h, AIArt.h, AIFont.h, AIHitTest.h, ATESuites.h
#include "SPBasic.h"
#include "AITextFrame.h"

#include <vector>
#include <string>

// Access the global SPBasicSuite pointer (set in Plugin.cpp) for lazy acquisition.
extern SPBasicSuite* sSPBasic;

// Bring commonly-used ATE types into scope.
// ATEErr and kNoError live inside namespace ATE (from ATESuites.h).
using ATE::ATEErr;
using ATE::kNoError;

// ============================================================================
// Module-level Suite Pointers
// ============================================================================

namespace {

AITextFrameSuite* sTextFrame = nullptr;
bool sTextFrameAcquired = false;

// ATE TextRangeSuite -- from ATESuites.h, pulled in by AITextFrame.h
// We use the C suite interface (function pointer struct), not the C++ wrappers.
using TextRangeSuitePtr = ATE::TextRangeSuite*;
TextRangeSuitePtr sTextRange = nullptr;
bool sTextRangeAcquired = false;

// ============================================================================
// Suite Acquisition Helpers
// ============================================================================

/**
 * Acquire AITextFrameSuite using the real SDK constant names and versions.
 * Uses kAITextFrameSuite ("AI Text Frame Suite") and kAITextFrameSuiteVersion
 * (AIAPI_VERSION(4) on current SDK).
 */
AITextFrameSuite* AcquireTextFrameSuite(SPBasicSuite* spBasic) {
    if (sTextFrameAcquired) {
        return sTextFrame;
    }
    sTextFrameAcquired = true;

    if (!spBasic) {
        return nullptr;
    }

    const void* suite = nullptr;
    ASErr err = spBasic->AcquireSuite(
        kAITextFrameSuite,
        kAITextFrameSuiteVersion,
        &suite
    );
    if (err == kNoErr && suite != nullptr) {
        sTextFrame = const_cast<AITextFrameSuite*>(
            static_cast<const AITextFrameSuite*>(suite));
    }

    return sTextFrame;
}

/**
 * Acquire ATE TextRangeSuite using the real SDK constant names and versions.
 * Uses kTextRangeSuite ("ATE TextRange Suite") and kTextRangeSuiteVersion (2).
 */
TextRangeSuitePtr AcquireTextRangeSuite(SPBasicSuite* spBasic) {
    if (sTextRangeAcquired) {
        return sTextRange;
    }
    sTextRangeAcquired = true;

    if (!spBasic) {
        return nullptr;
    }

    const void* suite = nullptr;
    ASErr err = spBasic->AcquireSuite(
        kTextRangeSuite,
        kTextRangeSuiteVersion,
        &suite
    );
    if (err == kNoErr && suite != nullptr) {
        sTextRange = const_cast<TextRangeSuitePtr>(
            static_cast<const ATE::TextRangeSuite*>(suite));
    }

    return sTextRange;
}

AITextFrameSuite* GetTextFrameSuite() {
    if (!sTextFrame) {
        return AcquireTextFrameSuite(sSPBasic);
    }
    return sTextFrame;
}

TextRangeSuitePtr GetTextRangeSuite() {
    if (!sTextRange) {
        return AcquireTextRangeSuite(sSPBasic);
    }
    return sTextRange;
}

// ============================================================================
// UTF-8 <-> UTF-16 Conversion Helpers
// ============================================================================
//
// ATE uses UTF-16 (ASUnicode / unsigned short) internally.
// Our HTTP/JSON API uses UTF-8 (std::string).

/**
 * Convert a UTF-8 std::string to a vector of UTF-16 code units (ASUnicode).
 * Handles the full BMP range and supplementary planes via surrogate pairs.
 */
std::vector<ASUnicode> Utf8ToUtf16(const std::string& utf8) {
    std::vector<ASUnicode> utf16;
    utf16.reserve(utf8.size());

    size_t i = 0;
    while (i < utf8.size()) {
        uint32_t codepoint = 0;
        unsigned char c = static_cast<unsigned char>(utf8[i]);

        if (c < 0x80) {
            codepoint = c;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            if (i + 1 >= utf8.size()) break;
            codepoint = (c & 0x1F) << 6;
            codepoint |= (static_cast<unsigned char>(utf8[i + 1]) & 0x3F);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            if (i + 2 >= utf8.size()) break;
            codepoint = (c & 0x0F) << 12;
            codepoint |= (static_cast<unsigned char>(utf8[i + 1]) & 0x3F) << 6;
            codepoint |= (static_cast<unsigned char>(utf8[i + 2]) & 0x3F);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            if (i + 3 >= utf8.size()) break;
            codepoint = (c & 0x07) << 18;
            codepoint |= (static_cast<unsigned char>(utf8[i + 1]) & 0x3F) << 12;
            codepoint |= (static_cast<unsigned char>(utf8[i + 2]) & 0x3F) << 6;
            codepoint |= (static_cast<unsigned char>(utf8[i + 3]) & 0x3F);
            i += 4;
        } else {
            i += 1;
            continue;
        }

        if (codepoint <= 0xFFFF) {
            utf16.push_back(static_cast<ASUnicode>(codepoint));
        } else if (codepoint <= 0x10FFFF) {
            codepoint -= 0x10000;
            utf16.push_back(static_cast<ASUnicode>(0xD800 + (codepoint >> 10)));
            utf16.push_back(static_cast<ASUnicode>(0xDC00 + (codepoint & 0x3FF)));
        }
    }

    return utf16;
}

/**
 * Convert a buffer of UTF-16 code units (ASUnicode) to a UTF-8 std::string.
 * Handles surrogate pairs for supplementary plane characters.
 */
std::string Utf16ToUtf8(const ASUnicode* utf16, ai::int32 length) {
    std::string utf8;
    utf8.reserve(static_cast<size_t>(length) * 2);

    for (ai::int32 i = 0; i < length; ++i) {
        uint32_t codepoint = utf16[i];

        // Check for surrogate pair
        if (codepoint >= 0xD800 && codepoint <= 0xDBFF && (i + 1) < length) {
            uint32_t low = utf16[i + 1];
            if (low >= 0xDC00 && low <= 0xDFFF) {
                codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                ++i;
            }
        }

        if (codepoint < 0x80) {
            utf8.push_back(static_cast<char>(codepoint));
        } else if (codepoint < 0x800) {
            utf8.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
            utf8.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint < 0x10000) {
            utf8.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
            utf8.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        } else if (codepoint <= 0x10FFFF) {
            utf8.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
            utf8.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            utf8.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
    }

    return utf8;
}

} // anonymous namespace

// ============================================================================
// Public ATEBridge API Implementation
// ============================================================================

namespace ATEBridge {

void AcquireSuites(SPBasicSuite* spBasic) {
    AcquireTextFrameSuite(spBasic);
    AcquireTextRangeSuite(spBasic);
}

void ReleaseSuites(SPBasicSuite* spBasic) {
    if (spBasic) {
        if (sTextFrame) {
            spBasic->ReleaseSuite(kAITextFrameSuite, kAITextFrameSuiteVersion);
            sTextFrame = nullptr;
        }
        if (sTextRange) {
            spBasic->ReleaseSuite(kTextRangeSuite, kTextRangeSuiteVersion);
            sTextRange = nullptr;
        }
    }
    sTextFrameAcquired = false;
    sTextRangeAcquired = false;
}

// ---------------------------------------------------------------------------
// NewPointText
// ---------------------------------------------------------------------------

ASErr NewPointText(
    ai::int16 paintOrder,
    AIArtHandle prep,
    ai::int16 orientation,
    AIRealPoint anchor,
    AIArtHandle* outTextFrame)
{
    AITextFrameSuite* suite = GetTextFrameSuite();
    if (!suite || !suite->NewPointText) {
        return kCantHappenErr;
    }

    return suite->NewPointText(
        paintOrder,
        prep,
        static_cast<AITextOrientation>(orientation),
        anchor,
        outTextFrame
    );
}

// ---------------------------------------------------------------------------
// GetTextContent
// ---------------------------------------------------------------------------

bool GetTextContent(AIArtHandle textFrame, std::string& outUtf8, std::string& outError) {
    AITextFrameSuite* tfSuite = GetTextFrameSuite();
    if (!tfSuite || !tfSuite->GetATETextRange) {
        outError = "AITextFrameSuite or GetATETextRange not available";
        return false;
    }

    auto* trSuite = GetTextRangeSuite();
    if (!trSuite) {
        outError = "ATE TextRangeSuite not available";
        return false;
    }

    // Step 1: Get the TextRangeRef from the text frame
    TextRangeRef textRange = nullptr;
    ASErr err = tfSuite->GetATETextRange(textFrame, &textRange);
    if (err != kNoErr || textRange == nullptr) {
        outError = "GetATETextRange failed (error " + std::to_string(static_cast<int>(err)) + ")";
        return false;
    }

    // Step 2: Get the size (character count)
    ai::int32 size = 0;
    ATEErr ateErr = trSuite->GetSize(textRange, &size);
    if (ateErr != kNoError) {
        trSuite->Release(textRange);
        outError = "TextRange::GetSize failed (ATE error " + std::to_string(ateErr) + ")";
        return false;
    }

    if (size <= 0) {
        trSuite->Release(textRange);
        outUtf8 = "";
        return true;
    }

    // Step 3: Read the content as UTF-16
    std::vector<ASUnicode> utf16Buffer(static_cast<size_t>(size) + 1, 0);
    ai::int32 charsRead = 0;
    ateErr = trSuite->GetContents_AsUnicode(textRange, utf16Buffer.data(), size, &charsRead);
    if (ateErr != kNoError) {
        trSuite->Release(textRange);
        outError = "TextRange::GetContents_AsUnicode failed (ATE error " + std::to_string(ateErr) + ")";
        return false;
    }

    // Step 4: Convert UTF-16 to UTF-8
    outUtf8 = Utf16ToUtf8(utf16Buffer.data(), charsRead);

    // Step 5: Release the TextRangeRef
    trSuite->Release(textRange);

    return true;
}

// ---------------------------------------------------------------------------
// SetTextContent
// ---------------------------------------------------------------------------

bool SetTextContent(AIArtHandle textFrame, const std::string& utf8, std::string& outError) {
    AITextFrameSuite* tfSuite = GetTextFrameSuite();
    if (!tfSuite || !tfSuite->GetATETextRange) {
        outError = "AITextFrameSuite or GetATETextRange not available";
        return false;
    }

    auto* trSuite = GetTextRangeSuite();
    if (!trSuite) {
        outError = "ATE TextRangeSuite not available";
        return false;
    }

    // Step 1: Get the TextRangeRef from the text frame
    TextRangeRef textRange = nullptr;
    ASErr err = tfSuite->GetATETextRange(textFrame, &textRange);
    if (err != kNoErr || textRange == nullptr) {
        outError = "GetATETextRange failed (error " + std::to_string(static_cast<int>(err)) + ")";
        return false;
    }

    // Step 2: Check if there is existing content to remove
    ai::int32 existingSize = 0;
    ATEErr ateErr = trSuite->GetSize(textRange, &existingSize);
    if (ateErr != kNoError) {
        trSuite->Release(textRange);
        outError = "TextRange::GetSize failed (ATE error " + std::to_string(ateErr) + ")";
        return false;
    }

    if (existingSize > 0) {
        // Remove existing content
        ateErr = trSuite->Remove(textRange);
        if (ateErr != kNoError) {
            trSuite->Release(textRange);
            outError = "TextRange::Remove failed (ATE error " + std::to_string(ateErr) + ")";
            return false;
        }

        // After Remove(), re-acquire the range since Remove may invalidate it
        trSuite->Release(textRange);
        textRange = nullptr;

        err = tfSuite->GetATETextRange(textFrame, &textRange);
        if (err != kNoErr || textRange == nullptr) {
            if (textRange) trSuite->Release(textRange);
            outError = "GetATETextRange failed after Remove (error " +
                       std::to_string(static_cast<int>(err)) + ")";
            return false;
        }
    }

    // Step 3: Convert UTF-8 to UTF-16
    std::vector<ASUnicode> utf16 = Utf8ToUtf16(utf8);

    // Step 4: Insert the new content
    if (!utf16.empty()) {
        ateErr = trSuite->InsertAfter_AsUnicode(
            textRange,
            utf16.data(),
            static_cast<ai::int32>(utf16.size())
        );
        if (ateErr != kNoError) {
            trSuite->Release(textRange);
            outError = "TextRange::InsertAfter_AsUnicode failed (ATE error " +
                       std::to_string(ateErr) + ")";
            return false;
        }
    }

    // Step 5: Release the TextRangeRef
    trSuite->Release(textRange);

    return true;
}

// ---------------------------------------------------------------------------
// GetTextSize
// ---------------------------------------------------------------------------

int32_t GetTextSize(AIArtHandle textFrame) {
    AITextFrameSuite* tfSuite = GetTextFrameSuite();
    if (!tfSuite || !tfSuite->GetATETextRange) {
        return -1;
    }

    auto* trSuite = GetTextRangeSuite();
    if (!trSuite) {
        return -1;
    }

    TextRangeRef textRange = nullptr;
    ASErr err = tfSuite->GetATETextRange(textFrame, &textRange);
    if (err != kNoErr || textRange == nullptr) {
        return -1;
    }

    ai::int32 size = 0;
    ATEErr ateErr = trSuite->GetSize(textRange, &size);
    trSuite->Release(textRange);

    if (ateErr != kNoError) {
        return -1;
    }

    return size;
}

} // namespace ATEBridge
