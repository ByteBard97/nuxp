/**
 * NUXP String Utilities - Implementation
 *
 * See StringUtils.hpp for documentation.
 */

#include "StringUtils.hpp"
#include "../SuitePointers.hpp"

namespace StringUtils {

//------------------------------------------------------------------------------
// String Conversion
//------------------------------------------------------------------------------

ai::UnicodeString ToUnicode(const std::string& str) {
    // ai::UnicodeString constructor accepts UTF-8 std::string directly
    return ai::UnicodeString(str);
}

std::string FromUnicode(const ai::UnicodeString& str) {
    // as_UTF8() returns std::string directly
    return str.as_UTF8();
}

//------------------------------------------------------------------------------
// Art Name Helpers
//------------------------------------------------------------------------------

std::string GetArtName(AIArtHandle art) {
    if (art == nullptr) {
        return "";
    }

    AIArtSuite* artSuite = SuitePointers::AIArt();
    if (artSuite == nullptr) {
        return "";
    }

    ai::UnicodeString name;
    ASErr error = artSuite->GetArtName(art, name, nullptr);
    if (error != kNoErr) {
        return "";
    }

    return FromUnicode(name);
}

void SetArtName(AIArtHandle art, const std::string& name) {
    if (art == nullptr) {
        return;
    }

    AIArtSuite* artSuite = SuitePointers::AIArt();
    if (artSuite == nullptr) {
        return;
    }

    ai::UnicodeString uniName = ToUnicode(name);
    artSuite->SetArtName(art, uniName);
}

//------------------------------------------------------------------------------
// Layer Name Helpers
//------------------------------------------------------------------------------

std::string GetLayerName(AILayerHandle layer) {
    if (layer == nullptr) {
        return "";
    }

    AILayerSuite* layerSuite = SuitePointers::AILayer();
    if (layerSuite == nullptr) {
        return "";
    }

    ai::UnicodeString title;
    ASErr error = layerSuite->GetLayerTitle(layer, title);
    if (error != kNoErr) {
        return "";
    }

    return FromUnicode(title);
}

//------------------------------------------------------------------------------
// Numeric Formatting
//------------------------------------------------------------------------------

std::string FormatReal(AIReal value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

} // namespace StringUtils
