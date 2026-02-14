/**
 * NUXP XMP Utilities Implementation
 *
 * Provides XMP metadata access for Illustrator documents and art objects.
 *
 * Two tiers of functionality:
 *
 * 1. ALWAYS AVAILABLE (uses Illustrator SDK's AIDocumentSuite/AIArtSuite):
 *    - GetDocumentXMP / SetDocumentXMP: Full XMP packet as XML string
 *    These work through AIDocumentSuite::GetDocumentXAP/SetDocumentXAP
 *    which are part of the standard Illustrator SDK.
 *
 * 2. REQUIRES XMP TOOLKIT SDK (behind #ifdef NUXP_HAS_XMP):
 *    - GetProperty / SetProperty: Individual property access
 *    - RegisterNamespace: Custom namespace registration
 *    These require the Adobe XMP Toolkit SDK to parse/modify XMP packets.
 *
 * XMP Toolkit SDK source: https://github.com/adobe/XMP-Toolkit-SDK
 * Programmer's guide: https://github.com/adobe/XMP-Toolkit-SDK/blob/main/docs/XMPProgrammersGuide.pdf
 *
 * To enable property-level XMP access:
 * 1. Clone: git clone https://github.com/adobe/XMP-Toolkit-SDK.git
 * 2. Build per repo instructions (CMake-based)
 * 3. Set XMP_SDK_PATH in CMake configuration
 * 4. NUXP_HAS_XMP will be defined automatically
 */

#include "XMPUtils.hpp"
#include "SuitePointers.hpp"

#ifdef NUXP_HAS_XMP
// XMP Toolkit SDK headers for property-level access
#define TXMP_STRING_TYPE std::string
#include "XMP.hpp"

static bool sXMPInitialized = false;
#endif // NUXP_HAS_XMP

namespace XMPUtils {

// =========================================================================
// Tier 1: Always available via Illustrator SDK
// =========================================================================

bool IsAvailable() {
    // Document-level XMP is available if the AIDocument suite is acquired
    return SuitePointers::AIDocument() != nullptr;
}

bool HasPropertyAccess() {
#ifdef NUXP_HAS_XMP
    return sXMPInitialized;
#else
    return false;
#endif
}

std::string GetDocumentXMP() {
    if (!SuitePointers::AIDocument()) {
        return "";
    }

    const char* xap = nullptr;
    AIErr err = SuitePointers::AIDocument()->GetDocumentXAP(&xap);
    if (err != kNoErr || xap == nullptr) {
        return "";
    }

    return std::string(xap);
}

bool SetDocumentXMP(const std::string& xmpString) {
    if (!SuitePointers::AIDocument()) {
        return false;
    }

    if (xmpString.empty()) {
        // Pass nullptr to clear metadata
        AIErr err = SuitePointers::AIDocument()->SetDocumentXAP(nullptr);
        return (err == kNoErr);
    }

    AIErr err = SuitePointers::AIDocument()->SetDocumentXAP(xmpString.c_str());
    return (err == kNoErr);
}

// =========================================================================
// Tier 2: Requires XMP Toolkit SDK for property-level access
// =========================================================================

#ifdef NUXP_HAS_XMP

void Initialize() {
    if (!sXMPInitialized) {
        if (SXMPMeta::Initialize()) {
            sXMPInitialized = true;
        }
    }
}

void Terminate() {
    if (sXMPInitialized) {
        SXMPMeta::Terminate();
        sXMPInitialized = false;
    }
}

std::string GetProperty(const std::string& namespaceURI, const std::string& propertyName) {
    if (!sXMPInitialized || namespaceURI.empty() || propertyName.empty()) {
        return "";
    }

    // Get the full XMP packet from the document
    std::string xmpPacket = GetDocumentXMP();
    if (xmpPacket.empty()) {
        return "";
    }

    try {
        SXMPMeta meta;
        meta.ParseFromBuffer(xmpPacket.c_str(),
                             static_cast<XMP_StringLen>(xmpPacket.size()));

        std::string value;
        if (meta.GetProperty(namespaceURI.c_str(), propertyName.c_str(),
                             &value, nullptr)) {
            return value;
        }
    } catch (const XMP_Error& e) {
        // XMP parsing or property access failed
        (void)e;
    }

    return "";
}

bool SetProperty(const std::string& namespaceURI, const std::string& propertyName,
                 const std::string& value) {
    if (!sXMPInitialized || namespaceURI.empty() || propertyName.empty()) {
        return false;
    }

    // Get the current XMP packet
    std::string xmpPacket = GetDocumentXMP();

    try {
        SXMPMeta meta;
        if (!xmpPacket.empty()) {
            meta.ParseFromBuffer(xmpPacket.c_str(),
                                 static_cast<XMP_StringLen>(xmpPacket.size()));
        }

        // Set the property value
        meta.SetProperty(namespaceURI.c_str(), propertyName.c_str(),
                         value.c_str());

        // Serialize back to string
        std::string updatedPacket;
        meta.SerializeToBuffer(&updatedPacket);

        // Write back to the document
        return SetDocumentXMP(updatedPacket);
    } catch (const XMP_Error& e) {
        (void)e;
        return false;
    }
}

std::string RegisterNamespace(const std::string& namespaceURI, const std::string& suggestedPrefix) {
    if (!sXMPInitialized || namespaceURI.empty()) {
        return "";
    }

    try {
        std::string actualPrefix;
        SXMPMeta::RegisterNamespace(namespaceURI.c_str(),
                                    suggestedPrefix.c_str(),
                                    &actualPrefix);
        return actualPrefix;
    } catch (const XMP_Error& e) {
        (void)e;
        return "";
    }
}

#else
// NUXP_HAS_XMP not defined - provide stub implementations for
// property-level functions. Document-level functions (above) always work.

void Initialize() {
    // No-op without XMP Toolkit SDK
}

void Terminate() {
    // No-op without XMP Toolkit SDK
}

std::string GetProperty(const std::string& /*namespaceURI*/, const std::string& /*propertyName*/) {
    return "";
}

bool SetProperty(const std::string& /*namespaceURI*/, const std::string& /*propertyName*/,
                 const std::string& /*value*/) {
    return false;
}

std::string RegisterNamespace(const std::string& /*namespaceURI*/, const std::string& /*suggestedPrefix*/) {
    return "";
}

#endif // NUXP_HAS_XMP

} // namespace XMPUtils
