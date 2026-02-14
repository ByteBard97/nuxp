/**
 * NUXP Text Endpoint Implementations
 *
 * Hand-written endpoints for ATE text frame operations.
 *
 * ARCHITECTURE NOTE:
 * The AITextFrame.h header normally #includes ATESuites.h, which pulls in
 * ~375KB of ATE type definitions that conflict with AITypes.h typedefs
 * (e.g., ASInt16, ASFixed, etc.). This is the same conflict that prevents
 * AIFontSuite from being acquired in SuitePointers.
 *
 * Our strategy:
 *   1. For text frame CREATION: We acquire AITextFrameSuite manually at
 *      runtime via SPBasicSuite::AcquireSuite() using the suite name string
 *      and a locally-defined struct that mirrors the vtable layout. This
 *      avoids #including AITextFrame.h entirely.
 *
 *   2. For text CONTENT get/set: We use the same forward-declared vtable
 *      approach for the ATE TextRangeSuite. The flow is:
 *        a. AITextFrameSuite::GetATETextRange() to get a TextRangeRef
 *        b. TextRangeSuite::GetSize() / GetContents_AsUnicode() to read
 *        c. TextRangeSuite::Remove() + InsertAfter_AsUnicode() to write
 *        d. TextRangeSuite::Release() to free the ref-counted object
 *      This avoids #including ATESuites.h or any ATE C++ wrapper headers.
 *
 * See NUXPHandlers.cpp for the established handler patterns.
 */

#include "TextEndpoints.hpp"
#include "HandleManager.hpp"
#include "HttpServer.hpp"
#include "MainThreadDispatch.hpp"
#include "SuitePointers.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

// Access SPBasicSuite global set in Plugin.cpp
extern SPBasicSuite* sSPBasic;

// ============================================================================
// AITextFrameSuite - Local Forward Declaration
// ============================================================================
//
// We cannot #include "AITextFrame.h" because it transitively includes
// ATESuites.h which conflicts with AITypes.h. Instead, we forward-declare
// only the suite struct with the function pointers we need.
//
// The struct layout MUST match the real AITextFrameSuite vtable exactly.
// We include ALL function pointer slots (even ones we don't call) so that
// the offsets are correct.
//
// Source of truth: plugin/sdk/AITextFrame.h, AITextFrameSuite struct
// ============================================================================

// Forward-declare ATE ref types as opaque pointers (we don't use them here)
// These are needed to complete the AITextFrameSuite struct layout.
struct _t_TextRange;
typedef _t_TextRange* TextRangeRef_Local;
struct _t_TextRanges;
typedef _t_TextRanges* TextRangesRef_Local;
struct _t_TextFrame;
typedef _t_TextFrame* TextFrameRef_Local;

// Text orientation enum (matches AIFont.h AITextOrientation)
// 0 = horizontal, 1 = vertical
enum AITextOrientation_Local {
    kHorizontalTextOrientation_Local = 0,
    kVerticalTextOrientation_Local = 1
};

// Text frame type enum (matches AITextFrame.h AITextType)
enum AITextType_Local {
    kUnknownTextType_Local = -1,
    kPointTextType_Local = 0,
    kInPathTextType_Local = 1,
    kOnPathTextType_Local = 2
};

// TextAntialiasingAvailableTypes enum (matches AITextFrame.h)
enum TextAntialiasingAvailableTypes_Local {
    kTextAntialiasingInvalid_Local = 0,
    kTextAntialiasingNone_Local,
    kTextAntialiasingSharp_Local,
    kTextAntialiasingCrisp_Local,
    kTextAntialiasingStrong_Local
};

/**
 * Mirror of AITextFrameSuite from AITextFrame.h.
 * Every function pointer slot must be present to preserve vtable offsets.
 * We only call NewPointText; the rest are typed as void* placeholders where
 * we don't need the exact signature.
 */
typedef struct {
    // 1. NewPointText - the one we actually call
    ASErr (ASAPI *NewPointText)(
        ai::int16 paintOrder,
        AIArtHandle prep,
        ai::int16 orient,  // AITextOrientation is an enum (int16-sized)
        AIRealPoint anchor,
        AIArtHandle* newTextFrame
    );

    // 2. NewInPathText
    ASErr (ASAPI *NewInPathText)(
        ai::int16 paintOrder, AIArtHandle prep, ai::int16 orient,
        AIArtHandle path, AIArtHandle baseFrame, AIBool8 append,
        AIArtHandle* newTextFrame
    );

    // 3. NewOnPathText
    ASErr (ASAPI *NewOnPathText)(
        ai::int16 paintOrder, AIArtHandle prep, ai::int16 orient,
        AIArtHandle path, AIReal startT, AIReal endT,
        AIArtHandle baseFrame, AIBool8 append,
        AIArtHandle* newTextFrame
    );

    // 4. NewOnPathText2
    ASErr (ASAPI *NewOnPathText2)(
        ai::int16 paintOrder, AIArtHandle prep, ai::int16 orient,
        AIArtHandle path, AIRealPoint anchor,
        AIArtHandle baseFrame, AIBool8 append,
        AIArtHandle* newTextFrame
    );

    // 5. GetType
    ASErr (ASAPI *GetType)(AIArtHandle textFrame, ai::int32* type);

    // 6. GetOrientation
    ASErr (ASAPI *GetOrientation)(AIArtHandle textFrame, ai::int16* orient);

    // 7. SetOrientation
    ASErr (ASAPI *SetOrientation)(AIArtHandle textFrame, ai::int16 orient);

    // 8. GetTextAntialias
    ASErr (ASAPI *GetTextAntialias)(AIArtHandle textFrame, ai::int32* tAntialias);

    // 9. SetTextAntialias
    ASErr (ASAPI *SetTextAntialias)(AIArtHandle textFrame, ai::int32 tAntialias);

    // 10. GetPointTextAnchor
    ASErr (ASAPI *GetPointTextAnchor)(AIArtHandle textFrame, AIRealPoint* anchor);

    // 11. GetPathObject
    ASErr (ASAPI *GetPathObject)(AIArtHandle textFrame, AIArtHandle* pathObject);

    // 12. GetOnPathTextTRange
    ASErr (ASAPI *GetOnPathTextTRange)(AIArtHandle textFrame, AIReal* startT, AIReal* endT);

    // 13. SetOnPathTextTRange
    ASErr (ASAPI *SetOnPathTextTRange)(AIArtHandle textFrame, AIReal startT, AIReal endT);

    // 14. GetATETextRange
    ASErr (ASAPI *GetATETextRange)(AIArtHandle textFrame, TextRangeRef_Local* textRange);

    // 15. GetATETextFrame
    ASErr (ASAPI *GetATETextFrame)(AIArtHandle textFrame, TextFrameRef_Local* ateFrame);

    // 16. GetAITextFrame
    ASErr (ASAPI *GetAITextFrame)(TextFrameRef_Local ateFrame, AIArtHandle* textFrame);

    // 17. GetATETextSelection
    ASErr (ASAPI *GetATETextSelection)(AIArtHandle textFrame, TextRangesRef_Local* selection);

    // 18. DoTextFrameHit - returns int32 not ASErr
    ai::int32 (ASAPI *DoTextFrameHit)(void* hitRef, TextRangeRef_Local* textRange);

    // 19. CreateOutline
    ASErr (ASAPI *CreateOutline)(AIArtHandle textFrame, AIArtHandle* outline);

    // 20. Link
    ASErr (ASAPI *Link)(AIArtHandle baseFrame, AIArtHandle nextFrame);

    // 21. Unlink
    ASErr (ASAPI *Unlink)(AIArtHandle frame, AIBool8 before, AIBool8 after);

    // 22. CreateATEV2Text
    ASErr (ASAPI *CreateATEV2Text)(
        ai::int16 paintOrder, AIArtHandle prep, void* data,
        size_t size, ai::int32 index, AIArtHandle* newTextFrame
    );

    // 23. GetATEV2Data
    ASErr (ASAPI *GetATEV2Data)(void** data, size_t* size);

    // 24. GetStoryIndex
    ASErr (ASAPI *GetStoryIndex)(AIArtHandle frame, ai::int32* index);

    // 25. GetFrameIndex
    ASErr (ASAPI *GetFrameIndex)(AIArtHandle frame, ai::int32* index);

    // 26. PartOfLinkedText
    ASErr (ASAPI *PartOfLinkedText)(AIArtHandle frame, AIBool8* linked);
} AITextFrameSuite_Local;

// Suite name and version constants (from AITextFrame.h)
static const char* kAITextFrameSuiteLocal = "AI Text Frame Suite";
static const ai::int32 kAITextFrameSuiteVersionLocal = 4; // AIAPI_VERSION(4)

// ============================================================================
// ATE TextRangeSuite - Local Forward Declaration
// ============================================================================
//
// The ATE TextRangeSuite provides functions to read and modify text content
// within a TextRangeRef obtained from AITextFrameSuite::GetATETextRange().
//
// We forward-declare the entire suite struct so that function pointer offsets
// are correct. Only a handful of slots are actually called (GetSize,
// GetContents_AsUnicode, Remove, InsertAfter_AsUnicode, Release).
//
// Source of truth: plugin/sdk/ATESuites.h, TextRangeSuite struct (version 2)
//
// ATE types we need locally:
//   ATEErr        = enum int (0 = kNoError)
//   ATEBool8      = unsigned char
//   ATETextDOM::Int32    = ai::int32 (from SloTextdomTypes.h)
//   ATETextDOM::Unicode  = ASUnicode = unsigned short (UTF-16)
//   TextRangeRef  = opaque pointer (same as our TextRangeRef_Local)
// ============================================================================

// ATE error type (matches ATETypes.h ATEErr enum)
typedef int ATEErr_Local;
static const ATEErr_Local kATENoError_Local = 0;

// ATE bool type (matches ATETypes.h ATEBool8)
typedef unsigned char ATEBool8_Local;

// Forward-declare additional opaque ATE ref types used in TextRangeSuite
// slots we don't call. These ensure correct struct padding.
struct _t_Story;
typedef _t_Story* StoryRef_Local;
struct _t_TextFramesIterator;
typedef _t_TextFramesIterator* TextFramesIteratorRef_Local;
struct _t_ParagraphsIterator;
typedef _t_ParagraphsIterator* ParagraphsIteratorRef_Local;
struct _t_WordsIterator;
typedef _t_WordsIterator* WordsIteratorRef_Local;
struct _t_TextRunsIterator;
typedef _t_TextRunsIterator* TextRunsIteratorRef_Local;
struct _t_CharInspector;
typedef _t_CharInspector* CharInspectorRef_Local;
struct _t_ParaInspector;
typedef _t_ParaInspector* ParaInspectorRef_Local;
struct _t_CharStyles;
typedef _t_CharStyles* CharStylesRef_Local;
struct _t_ParaStyles;
typedef _t_ParaStyles* ParaStylesRef_Local;
struct _t_CharFeatures;
typedef _t_CharFeatures* CharFeaturesRef_Local;
struct _t_ParaFeatures;
typedef _t_ParaFeatures* ParaFeaturesRef_Local;

// ASUnicode = unsigned short (UTF-16 code unit), from ASTypes.h
// Already available via the SDK headers included through SuitePointers.hpp,
// but we alias it here for clarity in the ATE suite signatures.
typedef ASUnicode ATEUnicode_Local;

/**
 * Mirror of ATE::TextRangeSuite (version 2) from ATESuites.h.
 * Every function pointer slot is present to preserve offsets.
 *
 * The ATE C suites do NOT use ASAPI calling convention - they use the
 * default C calling convention. Note: AddRef/Release/IsNull return
 * void/void/ATEBool8 (not ATEErr).
 */
typedef struct {
    // --- Reference count maintenance ---
    // 1. AddRef
    void (*AddRef)(TextRangeRef_Local textrange);
    // 2. Release
    void (*Release)(TextRangeRef_Local textrange);
    // 3. IsNull
    ATEBool8_Local (*IsNull)(TextRangeRef_Local textrange);

    // --- PROPERTIES ---
    // 4. GetStart
    ATEErr_Local (*GetStart)(TextRangeRef_Local textrange, ai::int32* ret);
    // 5. SetStart
    ATEErr_Local (*SetStart)(TextRangeRef_Local textrange, ai::int32 start);
    // 6. GetEnd
    ATEErr_Local (*GetEnd)(TextRangeRef_Local textrange, ai::int32* ret);
    // 7. SetEnd
    ATEErr_Local (*SetEnd)(TextRangeRef_Local textrange, ai::int32 end);
    // 8. GetSize
    ATEErr_Local (*GetSize)(TextRangeRef_Local textrange, ai::int32* ret);

    // --- NAVIGATION OBJECTS ---
    // 9. GetStory
    ATEErr_Local (*GetStory)(TextRangeRef_Local textrange, StoryRef_Local* ret);
    // 10. GetTextSelection
    ATEErr_Local (*GetTextSelection)(TextRangeRef_Local textrange, TextRangesRef_Local* ret);
    // 11. GetTextFramesIterator
    ATEErr_Local (*GetTextFramesIterator)(TextRangeRef_Local textrange, TextFramesIteratorRef_Local* ret);
    // 12. GetParagraphsIterator
    ATEErr_Local (*GetParagraphsIterator)(TextRangeRef_Local textrange, ParagraphsIteratorRef_Local* ret);
    // 13. GetWordsIterator
    ATEErr_Local (*GetWordsIterator)(TextRangeRef_Local textrange, WordsIteratorRef_Local* ret);
    // 14. GetTextRunsIterator
    ATEErr_Local (*GetTextRunsIterator)(TextRangeRef_Local textrange, TextRunsIteratorRef_Local* ret);

    // --- ATTRIBUTE INSPECTION AND MODIFICATION ---
    // 15. GetCharInspector
    ATEErr_Local (*GetCharInspector)(TextRangeRef_Local textrange, CharInspectorRef_Local* ret);
    // 16. GetParaInspector
    ATEErr_Local (*GetParaInspector)(TextRangeRef_Local textrange, ParaInspectorRef_Local* ret);
    // 17. GetNamedCharStyles
    ATEErr_Local (*GetNamedCharStyles)(TextRangeRef_Local textrange, CharStylesRef_Local* ret);
    // 18. GetNamedParaStyles
    ATEErr_Local (*GetNamedParaStyles)(TextRangeRef_Local textrange, ParaStylesRef_Local* ret);
    // 19. SetNamedCharStyle
    ATEErr_Local (*SetNamedCharStyle)(TextRangeRef_Local textrange, const ATEUnicode_Local* pName, ATEBool8_Local* ret);
    // 20. SetNamedParaStyle
    ATEErr_Local (*SetNamedParaStyle)(TextRangeRef_Local textrange, const ATEUnicode_Local* pName, ATEBool8_Local* ret);
    // 21. ClearNamedCharStyle
    ATEErr_Local (*ClearNamedCharStyle)(TextRangeRef_Local textrange);
    // 22. ClearNamedParaStyle
    ATEErr_Local (*ClearNamedParaStyle)(TextRangeRef_Local textrange);
    // 23. GetUniqueCharFeatures
    ATEErr_Local (*GetUniqueCharFeatures)(TextRangeRef_Local textrange, CharFeaturesRef_Local* ret);
    // 24. GetUniqueParaFeatures
    ATEErr_Local (*GetUniqueParaFeatures)(TextRangeRef_Local textrange, ParaFeaturesRef_Local* ret);
    // 25. HasLocalCharFeatures
    ATEErr_Local (*HasLocalCharFeatures)(TextRangeRef_Local textrange, ATEBool8_Local* ret);
    // 26. HasLocalParaFeatures
    ATEErr_Local (*HasLocalParaFeatures)(TextRangeRef_Local textrange, ATEBool8_Local* ret);
    // 27. GetUniqueLocalCharFeatures
    ATEErr_Local (*GetUniqueLocalCharFeatures)(TextRangeRef_Local textrange, CharFeaturesRef_Local* ret);
    // 28. GetUniqueLocalParaFeatures
    ATEErr_Local (*GetUniqueLocalParaFeatures)(TextRangeRef_Local textrange, ParaFeaturesRef_Local* ret);
    // 29. SetLocalCharFeatures
    ATEErr_Local (*SetLocalCharFeatures)(TextRangeRef_Local textrange, const CharFeaturesRef_Local pFeatures);
    // 30. ReplaceOrAddLocalCharFeatures
    ATEErr_Local (*ReplaceOrAddLocalCharFeatures)(TextRangeRef_Local textrange, const CharFeaturesRef_Local pFeatures);
    // 31. ClearLocalCharFeatures
    ATEErr_Local (*ClearLocalCharFeatures)(TextRangeRef_Local textrange);
    // 32. SetLocalParaFeatures
    ATEErr_Local (*SetLocalParaFeatures)(TextRangeRef_Local textrange, const ParaFeaturesRef_Local pFeatures);
    // 33. ReplaceOrAddLocalParaFeatures
    ATEErr_Local (*ReplaceOrAddLocalParaFeatures)(TextRangeRef_Local textrange, const ParaFeaturesRef_Local pFeatures);
    // 34. ClearLocalParaFeatures
    ATEErr_Local (*ClearLocalParaFeatures)(TextRangeRef_Local textrange);

    // --- METHODS ---
    // 35. SetStory
    ATEErr_Local (*SetStory)(TextRangeRef_Local textrange, const StoryRef_Local story);
    // 36. SetRange
    ATEErr_Local (*SetRange)(TextRangeRef_Local textrange, ai::int32 start, ai::int32 end);
    // 37. Collapse (CollapseDirection is an enum: CollapseEnd=0, CollapseStart=1)
    ATEErr_Local (*Collapse)(TextRangeRef_Local textrange, ai::int32 direction);
    // 38. Move
    ATEErr_Local (*Move)(TextRangeRef_Local textrange, ai::int32 unit, ai::int32* ret);
    // 39. Clone
    ATEErr_Local (*Clone)(TextRangeRef_Local textrange, TextRangeRef_Local* ret);
    // 40. InsertBefore_AsUnicode
    ATEErr_Local (*InsertBefore_AsUnicode)(TextRangeRef_Local textrange, const ATEUnicode_Local* text, ai::int32 length);
    // 41. InsertAfter_AsUnicode
    ATEErr_Local (*InsertAfter_AsUnicode)(TextRangeRef_Local textrange, const ATEUnicode_Local* text, ai::int32 length);
    // 42. InsertBefore_AsTextRange
    ATEErr_Local (*InsertBefore_AsTextRange)(TextRangeRef_Local textrange, const TextRangeRef_Local anotherRange);
    // 43. InsertAfter_AsTextRange
    ATEErr_Local (*InsertAfter_AsTextRange)(TextRangeRef_Local textrange, const TextRangeRef_Local anotherRange);
    // 44. GetContents_AsUnicode
    ATEErr_Local (*GetContents_AsUnicode)(TextRangeRef_Local textrange, ATEUnicode_Local* text, ai::int32 maxLength, ai::int32* ret);
    // 45. GetContents_AsChar
    ATEErr_Local (*GetContents_AsChar)(TextRangeRef_Local textrange, char* text, ai::int32 maxLength, ai::int32* ret);
    // 46. GetSingleGlyphInRange
    ATEErr_Local (*GetSingleGlyphInRange)(TextRangeRef_Local textrange, int* pSingleGlyph, ATEBool8_Local* ret);
    // 47. Select
    ATEErr_Local (*Select)(TextRangeRef_Local textrange, ATEBool8_Local addToSelection);
    // 48. DeSelect
    ATEErr_Local (*DeSelect)(TextRangeRef_Local textrange);
    // 49. ChangeCase
    ATEErr_Local (*ChangeCase)(TextRangeRef_Local textrange, int caseChangeType);
    // 50. ConvertListStyleToText (version 2 addition)
    ATEErr_Local (*ConvertListStyleToText)(TextRangeRef_Local textrange);
    // 51. FitHeadlines
    ATEErr_Local (*FitHeadlines)(TextRangeRef_Local textrange);
    // 52. Remove
    ATEErr_Local (*Remove)(TextRangeRef_Local textrange);
    // 53. IsEqual
    ATEErr_Local (*IsEqual)(TextRangeRef_Local textrange, const TextRangeRef_Local anotherRange, ATEBool8_Local* ret);
    // 54. GetCharacterType
    ATEErr_Local (*GetCharacterType)(TextRangeRef_Local textrange, int* ret);
} TextRangeSuite_Local;

// Suite name and version constants (from ATESuites.h)
static const char* kTextRangeSuiteLocal = "ATE TextRange Suite";
static const ai::int32 kTextRangeSuiteVersionLocal = 2;

// ============================================================================
// UTF-8 <-> UTF-16 Conversion Helpers
// ============================================================================
//
// ATE uses UTF-16 (ASUnicode / unsigned short) internally.
// Our HTTP/JSON API uses UTF-8 (std::string).
// These helpers convert between the two encodings.
// ============================================================================

namespace {

/**
 * Convert a UTF-8 std::string to a vector of UTF-16 code units (ASUnicode).
 * Handles the full BMP range and supplementary planes via surrogate pairs.
 */
std::vector<ATEUnicode_Local> Utf8ToUtf16(const std::string& utf8) {
    std::vector<ATEUnicode_Local> utf16;
    utf16.reserve(utf8.size()); // Rough estimate

    size_t i = 0;
    while (i < utf8.size()) {
        uint32_t codepoint = 0;
        unsigned char c = static_cast<unsigned char>(utf8[i]);

        if (c < 0x80) {
            // 1-byte sequence (ASCII)
            codepoint = c;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            // 2-byte sequence
            if (i + 1 >= utf8.size()) break;
            codepoint = (c & 0x1F) << 6;
            codepoint |= (static_cast<unsigned char>(utf8[i + 1]) & 0x3F);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            // 3-byte sequence
            if (i + 2 >= utf8.size()) break;
            codepoint = (c & 0x0F) << 12;
            codepoint |= (static_cast<unsigned char>(utf8[i + 1]) & 0x3F) << 6;
            codepoint |= (static_cast<unsigned char>(utf8[i + 2]) & 0x3F);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4-byte sequence (supplementary plane)
            if (i + 3 >= utf8.size()) break;
            codepoint = (c & 0x07) << 18;
            codepoint |= (static_cast<unsigned char>(utf8[i + 1]) & 0x3F) << 12;
            codepoint |= (static_cast<unsigned char>(utf8[i + 2]) & 0x3F) << 6;
            codepoint |= (static_cast<unsigned char>(utf8[i + 3]) & 0x3F);
            i += 4;
        } else {
            // Invalid UTF-8 byte, skip
            i += 1;
            continue;
        }

        if (codepoint <= 0xFFFF) {
            // BMP character - single UTF-16 code unit
            utf16.push_back(static_cast<ATEUnicode_Local>(codepoint));
        } else if (codepoint <= 0x10FFFF) {
            // Supplementary plane - encode as surrogate pair
            codepoint -= 0x10000;
            utf16.push_back(static_cast<ATEUnicode_Local>(0xD800 + (codepoint >> 10)));
            utf16.push_back(static_cast<ATEUnicode_Local>(0xDC00 + (codepoint & 0x3FF)));
        }
    }

    return utf16;
}

/**
 * Convert a buffer of UTF-16 code units (ASUnicode) to a UTF-8 std::string.
 * Handles surrogate pairs for supplementary plane characters.
 */
std::string Utf16ToUtf8(const ATEUnicode_Local* utf16, ai::int32 length) {
    std::string utf8;
    utf8.reserve(static_cast<size_t>(length) * 2); // Rough estimate

    for (ai::int32 i = 0; i < length; ++i) {
        uint32_t codepoint = utf16[i];

        // Check for surrogate pair
        if (codepoint >= 0xD800 && codepoint <= 0xDBFF && (i + 1) < length) {
            uint32_t low = utf16[i + 1];
            if (low >= 0xDC00 && low <= 0xDFFF) {
                codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                ++i; // Consume the low surrogate
            }
        }

        // Encode as UTF-8
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

} // anonymous namespace (UTF conversion helpers)

// ============================================================================
// Module-level Suite Pointers
// ============================================================================

namespace {

// --- AITextFrameSuite ---
AITextFrameSuite_Local* sTextFrame = nullptr;
bool sTextFrameAcquireAttempted = false;

/**
 * Acquire AITextFrameSuite at runtime via SPBasicSuite.
 * This avoids including AITextFrame.h (which pulls in ATESuites.h).
 *
 * @return Pointer to the suite, or nullptr on failure.
 */
AITextFrameSuite_Local* AcquireTextFrameSuite() {
    if (sTextFrameAcquireAttempted) {
        return sTextFrame;
    }
    sTextFrameAcquireAttempted = true;

    if (!sSPBasic) {
        return nullptr;
    }

    const void* suite = nullptr;
    ASErr err = sSPBasic->AcquireSuite(
        kAITextFrameSuiteLocal,
        kAITextFrameSuiteVersionLocal,
        &suite
    );
    if (err == kNoErr && suite != nullptr) {
        sTextFrame = const_cast<AITextFrameSuite_Local*>(
            static_cast<const AITextFrameSuite_Local*>(suite));
    }

    return sTextFrame;
}

/**
 * Get the cached AITextFrameSuite pointer.
 * Thread-safe because acquisition happens inside MainThreadDispatch::Run().
 */
AITextFrameSuite_Local* GetTextFrameSuite() {
    if (!sTextFrame) {
        return AcquireTextFrameSuite();
    }
    return sTextFrame;
}

// --- ATE TextRangeSuite ---
TextRangeSuite_Local* sTextRange = nullptr;
bool sTextRangeAcquireAttempted = false;

/**
 * Acquire ATE TextRangeSuite at runtime via SPBasicSuite.
 * This avoids including ATESuites.h which conflicts with AITypes.h.
 *
 * @return Pointer to the suite, or nullptr on failure.
 */
TextRangeSuite_Local* AcquireTextRangeSuite() {
    if (sTextRangeAcquireAttempted) {
        return sTextRange;
    }
    sTextRangeAcquireAttempted = true;

    if (!sSPBasic) {
        return nullptr;
    }

    const void* suite = nullptr;
    ASErr err = sSPBasic->AcquireSuite(
        kTextRangeSuiteLocal,
        kTextRangeSuiteVersionLocal,
        &suite
    );
    if (err == kNoErr && suite != nullptr) {
        sTextRange = const_cast<TextRangeSuite_Local*>(
            static_cast<const TextRangeSuite_Local*>(suite));
    }

    return sTextRange;
}

/**
 * Get the cached ATE TextRangeSuite pointer.
 * Thread-safe because acquisition happens inside MainThreadDispatch::Run().
 */
TextRangeSuite_Local* GetTextRangeSuite() {
    if (!sTextRange) {
        return AcquireTextRangeSuite();
    }
    return sTextRange;
}

// ============================================================================
// Internal Helpers -- Text Content via ATE TextRangeSuite
// ============================================================================

/**
 * Get the text content of an AIArtHandle text frame as a UTF-8 string.
 *
 * Flow:
 *   1. AITextFrameSuite::GetATETextRange() -> TextRangeRef
 *   2. TextRangeSuite::GetSize() -> character count
 *   3. TextRangeSuite::GetContents_AsUnicode() -> UTF-16 buffer
 *   4. Convert UTF-16 -> UTF-8
 *   5. TextRangeSuite::Release() the TextRangeRef
 *
 * @param art       A valid AIArtHandle of type kTextFrameArt
 * @param outText   [out] The text content as UTF-8
 * @param outError  [out] Error description if return is false
 * @return true on success, false on failure (outError is set)
 */
bool GetTextContentFromFrame(AIArtHandle art, std::string& outText, std::string& outError) {
    AITextFrameSuite_Local* tfSuite = GetTextFrameSuite();
    if (!tfSuite || !tfSuite->GetATETextRange) {
        outError = "AITextFrameSuite or GetATETextRange not available";
        return false;
    }

    TextRangeSuite_Local* trSuite = GetTextRangeSuite();
    if (!trSuite) {
        outError = "ATE TextRangeSuite not available";
        return false;
    }

    // Step 1: Get the TextRangeRef from the text frame
    TextRangeRef_Local textRange = nullptr;
    ASErr err = tfSuite->GetATETextRange(art, &textRange);
    if (err != kNoErr || textRange == nullptr) {
        outError = "GetATETextRange failed (error " + std::to_string(static_cast<int>(err)) + ")";
        return false;
    }

    // Step 2: Get the size (character count) of the text range
    ai::int32 size = 0;
    ATEErr_Local ateErr = trSuite->GetSize(textRange, &size);
    if (ateErr != kATENoError_Local) {
        trSuite->Release(textRange);
        outError = "TextRange::GetSize failed (ATE error " + std::to_string(ateErr) + ")";
        return false;
    }

    if (size <= 0) {
        // Empty text frame -- valid result
        trSuite->Release(textRange);
        outText = "";
        return true;
    }

    // Step 3: Read the content as UTF-16
    // Allocate buffer with +1 for safety
    std::vector<ATEUnicode_Local> utf16Buffer(static_cast<size_t>(size) + 1, 0);
    ai::int32 charsRead = 0;
    ateErr = trSuite->GetContents_AsUnicode(textRange, utf16Buffer.data(), size, &charsRead);
    if (ateErr != kATENoError_Local) {
        trSuite->Release(textRange);
        outError = "TextRange::GetContents_AsUnicode failed (ATE error " + std::to_string(ateErr) + ")";
        return false;
    }

    // Step 4: Convert UTF-16 to UTF-8
    outText = Utf16ToUtf8(utf16Buffer.data(), charsRead);

    // Step 5: Release the TextRangeRef
    trSuite->Release(textRange);

    return true;
}

/**
 * Set the text content of an AIArtHandle text frame from a UTF-8 string.
 * Replaces all existing content.
 *
 * Flow:
 *   1. AITextFrameSuite::GetATETextRange() -> TextRangeRef
 *   2. TextRangeSuite::Remove() to clear existing content (if any)
 *   3. Convert UTF-8 -> UTF-16
 *   4. Re-acquire TextRangeRef (Remove may invalidate it)
 *   5. TextRangeSuite::InsertAfter_AsUnicode() to insert new text
 *   6. TextRangeSuite::Release() the TextRangeRef
 *
 * @param art       A valid AIArtHandle of type kTextFrameArt
 * @param text      The new text content as UTF-8
 * @param outError  [out] Error description if return is false
 * @return true on success, false on failure (outError is set)
 */
bool SetTextContentOnFrame(AIArtHandle art, const std::string& text, std::string& outError) {
    AITextFrameSuite_Local* tfSuite = GetTextFrameSuite();
    if (!tfSuite || !tfSuite->GetATETextRange) {
        outError = "AITextFrameSuite or GetATETextRange not available";
        return false;
    }

    TextRangeSuite_Local* trSuite = GetTextRangeSuite();
    if (!trSuite) {
        outError = "ATE TextRangeSuite not available";
        return false;
    }

    // Step 1: Get the TextRangeRef from the text frame
    TextRangeRef_Local textRange = nullptr;
    ASErr err = tfSuite->GetATETextRange(art, &textRange);
    if (err != kNoErr || textRange == nullptr) {
        outError = "GetATETextRange failed (error " + std::to_string(static_cast<int>(err)) + ")";
        return false;
    }

    // Step 2: Check if there is existing content to remove
    ai::int32 existingSize = 0;
    ATEErr_Local ateErr = trSuite->GetSize(textRange, &existingSize);
    if (ateErr != kATENoError_Local) {
        trSuite->Release(textRange);
        outError = "TextRange::GetSize failed (ATE error " + std::to_string(ateErr) + ")";
        return false;
    }

    if (existingSize > 0) {
        // Remove existing content
        ateErr = trSuite->Remove(textRange);
        if (ateErr != kATENoError_Local) {
            trSuite->Release(textRange);
            outError = "TextRange::Remove failed (ATE error " + std::to_string(ateErr) + ")";
            return false;
        }

        // After Remove(), re-acquire the range since Remove may invalidate it
        trSuite->Release(textRange);
        textRange = nullptr;

        err = tfSuite->GetATETextRange(art, &textRange);
        if (err != kNoErr || textRange == nullptr) {
            if (textRange) trSuite->Release(textRange);
            outError = "GetATETextRange failed after Remove (error " +
                       std::to_string(static_cast<int>(err)) + ")";
            return false;
        }
    }

    // Step 3: Convert UTF-8 to UTF-16
    std::vector<ATEUnicode_Local> utf16 = Utf8ToUtf16(text);

    // Step 4: Insert the new content
    if (!utf16.empty()) {
        ateErr = trSuite->InsertAfter_AsUnicode(
            textRange,
            utf16.data(),
            static_cast<ai::int32>(utf16.size())
        );
        if (ateErr != kATENoError_Local) {
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

} // anonymous namespace

// ============================================================================
// Handler Implementations
// ============================================================================
// These handlers implement the declarations in generated/CustomRouteHandlers.h
// which places them in the NUXP namespace. Route registration is handled by
// generated/CustomRouteRegistration.cpp.

namespace NUXP {

// ---------------------------------------------------------------------------
// POST /api/text/create — Create a new point text frame
// ---------------------------------------------------------------------------
std::string HandleCreateTextFrame(const std::string& body) {
    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    // x and y are required for meaningful text placement
    if (!params.contains("x") || !params.contains("y")) {
        return json{{"success", false},
                    {"error", "Missing required fields: x, y"}}.dump();
    }

    json result = MainThreadDispatch::Run([&params]() -> json {
        AITextFrameSuite_Local* textFrameSuite = GetTextFrameSuite();
        if (!textFrameSuite) {
            return {{"success", false},
                    {"error", "AITextFrameSuite not available"}};
        }
        if (!textFrameSuite->NewPointText) {
            return {{"success", false},
                    {"error", "NewPointText function not available in suite"}};
        }

        // Parse anchor point
        AIRealPoint anchor;
        anchor.h = params["x"].get<AIReal>();
        anchor.v = params["y"].get<AIReal>();

        // Parse orientation: 0 = horizontal (default), 1 = vertical
        ai::int16 orientation = static_cast<ai::int16>(
            params.value("orientation", 0));

        // Create the point text frame at the top of the paint order
        AIArtHandle newTextFrame = nullptr;
        ASErr err = textFrameSuite->NewPointText(
            kPlaceAboveAll,     // paint order
            nullptr,            // prep object (ignored with kPlaceAboveAll)
            orientation,        // text orientation
            anchor,             // anchor point
            &newTextFrame       // output handle
        );

        if (err != kNoErr || newTextFrame == nullptr) {
            return {{"success", false},
                    {"error", "NewPointText failed"},
                    {"errorCode", static_cast<int>(err)}};
        }

        // Register the handle for cross-thread access
        int32_t artId = HandleManager::art.Register(newTextFrame);

        // Set initial text content if provided
        if (params.contains("contents") && params["contents"].is_string()) {
            std::string contents = params["contents"].get<std::string>();
            if (!contents.empty()) {
                std::string setError;
                if (!SetTextContentOnFrame(newTextFrame, contents, setError)) {
                    // Frame was created but content could not be set
                    return {{"success", true},
                            {"artId", artId},
                            {"warning", "Text frame created but content "
                                        "could not be set: " + setError}};
                }
            }
        }

        return {{"success", true},
                {"artId", artId}};
    });

    return result.dump();
}

// ---------------------------------------------------------------------------
// GET /api/text/{id}/content — Get text content from a text frame
// ---------------------------------------------------------------------------
std::string HandleGetTextContent(const std::string& id) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

    json result = MainThreadDispatch::Run([artId]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }

        // Verify this is a text frame art object
        if (!SuitePointers::AIArt()) {
            return {{"success", false},
                    {"error", "AIArt suite not available"}};
        }
        short artType = kUnknownArt;
        SuitePointers::AIArt()->GetArtType(art, &artType);
        if (artType != kTextFrameArt) {
            return {{"success", false},
                    {"error", "Art object is not a text frame"},
                    {"artType", static_cast<int>(artType)}};
        }

        // Extract text content via ATE TextRangeSuite (forward-declared)
        std::string contents;
        std::string getError;
        if (!GetTextContentFromFrame(art, contents, getError)) {
            return {{"success", false},
                    {"error", getError},
                    {"artId", artId}};
        }

        return {{"success", true},
                {"artId", artId},
                {"contents", contents}};
    });

    return result.dump();
}

// ---------------------------------------------------------------------------
// POST /api/text/{id}/content — Set text content on a text frame
// ---------------------------------------------------------------------------
std::string HandleSetTextContent(const std::string& id, const std::string& body) {
    int artId;
    try {
        artId = std::stoi(id);
    } catch (...) {
        return json{{"success", false},
                    {"error", "Invalid art handle ID"}}.dump();
    }

    json params;
    try {
        params = json::parse(body);
    } catch (const json::parse_error& e) {
        return json{{"success", false},
                    {"error", std::string("Invalid JSON: ") + e.what()}}.dump();
    }

    if (!params.contains("contents") || !params["contents"].is_string()) {
        return json{{"success", false},
                    {"error", "Missing required field: contents (string)"}}.dump();
    }

    json result = MainThreadDispatch::Run([artId, &params]() -> json {
        AIArtHandle art = HandleManager::art.Get(artId);
        if (!art) {
            return {{"success", false},
                    {"error", "Invalid or stale art handle"}};
        }

        // Verify this is a text frame art object
        if (!SuitePointers::AIArt()) {
            return {{"success", false},
                    {"error", "AIArt suite not available"}};
        }
        short artType = kUnknownArt;
        SuitePointers::AIArt()->GetArtType(art, &artType);
        if (artType != kTextFrameArt) {
            return {{"success", false},
                    {"error", "Art object is not a text frame"},
                    {"artType", static_cast<int>(artType)}};
        }

        // Set text content via ATE TextRangeSuite (forward-declared)
        std::string contents = params["contents"].get<std::string>();
        std::string setError;
        if (!SetTextContentOnFrame(art, contents, setError)) {
            return {{"success", false},
                    {"error", setError},
                    {"artId", artId}};
        }

        return {{"success", true},
                {"artId", artId}};
    });

    return result.dump();
}

} // namespace NUXP

// ============================================================================
// Legacy Route Registration
// ============================================================================
// DEPRECATED: These routes are now registered via generated
// CustomRouteRegistration.cpp from routes.json. This function is retained
// temporarily for backward compatibility but should be removed once the
// generated registration is fully active.

namespace TextEndpoints {

void RegisterRoutes() {
    // NOTE: These routes are now also registered by CustomRouteRegistration.cpp.
    // This function should be removed once the generated registration is
    // confirmed working. Do NOT call both RegisterRoutes() and
    // RegisterCustomRoutes() for the same routes — it will cause duplicate
    // route registration.

    // POST /api/text/create - Create a new point text frame
    HttpServer::Post("/api/text/create", [](const std::string& body) {
        return NUXP::HandleCreateTextFrame(body);
    });

    // GET /api/text/{id}/content - Get text content from a text frame
    HttpServer::GetWithPattern(
        R"(/api/text/([^/]+)/content)",
        [](const std::string& /* body */,
           const std::vector<std::string>& params) {
            if (params.empty()) {
                return std::string(
                    R"({"success":false,"error":"missing text frame ID"})");
            }
            return NUXP::HandleGetTextContent(params[0]);
        });

    // POST /api/text/{id}/content - Set text content on a text frame
    HttpServer::PostWithPattern(
        R"(/api/text/([^/]+)/content)",
        [](const std::string& body,
           const std::vector<std::string>& params) {
            if (params.empty()) {
                return std::string(
                    R"({"success":false,"error":"missing text frame ID"})");
            }
            return NUXP::HandleSetTextContent(params[0], body);
        });
}

} // namespace TextEndpoints
