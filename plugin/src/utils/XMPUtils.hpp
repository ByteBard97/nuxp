/**
 * NUXP XMP Utilities
 *
 * Manual wrappers for Adobe XMP (Extensible Metadata Platform) operations.
 *
 * Two tiers of functionality:
 *
 * Tier 1 - ALWAYS AVAILABLE (uses Illustrator SDK's AIDocumentSuite):
 *   - IsAvailable(): Check if document-level XMP access works
 *   - GetDocumentXMP(): Get full XMP packet as XML string
 *   - SetDocumentXMP(): Set full XMP packet from XML string
 *   These use AIDocumentSuite::GetDocumentXAP/SetDocumentXAP which are
 *   part of the standard Illustrator SDK. No additional dependencies.
 *
 * Tier 2 - REQUIRES XMP TOOLKIT SDK (optional):
 *   - HasPropertyAccess(): Check if property-level access is available
 *   - GetProperty(): Read a single XMP property by namespace + name
 *   - SetProperty(): Write a single XMP property
 *   - RegisterNamespace(): Register custom XMP namespaces
 *   These require the Adobe XMP Toolkit SDK to parse/modify XMP packets.
 *
 * XMP Toolkit SDK source: https://github.com/adobe/XMP-Toolkit-SDK
 * Programmer's guide: https://github.com/adobe/XMP-Toolkit-SDK/blob/main/docs/XMPProgrammersGuide.pdf
 *
 * To enable property-level XMP access:
 * 1. Clone the XMP Toolkit SDK: git clone https://github.com/adobe/XMP-Toolkit-SDK.git
 * 2. Build it per the repo instructions (CMake-based)
 * 3. Set XMP_SDK_PATH in your CMake configuration
 * 4. NUXP_HAS_XMP will be defined automatically when the SDK is found
 *
 * Usage:
 *   #include "utils/XMPUtils.hpp"
 *
 *   // Tier 1 (always works):
 *   std::string xmp = XMPUtils::GetDocumentXMP();
 *   bool success = XMPUtils::SetDocumentXMP(xmpString);
 *
 *   // Tier 2 (requires XMP Toolkit SDK):
 *   if (XMPUtils::HasPropertyAccess()) {
 *       std::string value = XMPUtils::GetProperty(ns, propName);
 *       bool ok = XMPUtils::SetProperty(ns, propName, value);
 *   }
 */

#ifndef NUXP_XMP_UTILS_HPP
#define NUXP_XMP_UTILS_HPP

#include <string>

namespace XMPUtils {

// =========================================================================
// Tier 1: Always available via Illustrator SDK
// =========================================================================

/**
 * Check if document-level XMP functionality is available.
 * Returns true if the AIDocument suite is acquired (always the case
 * when the plugin is running inside Illustrator).
 *
 * @return true if GetDocumentXMP/SetDocumentXMP will work
 */
bool IsAvailable();

/**
 * Check if property-level XMP access is available.
 * Returns true only if the XMP Toolkit SDK was linked and initialized.
 *
 * @return true if GetProperty/SetProperty/RegisterNamespace will work
 */
bool HasPropertyAccess();

/**
 * Get the XMP metadata for the current document.
 * Returns the full XMP packet as an XML string (UTF-8).
 *
 * @return XMP string in XML format, or empty string if unavailable/error
 */
std::string GetDocumentXMP();

/**
 * Set the XMP metadata for the current document.
 * Replaces any existing XMP metadata with the provided XML packet.
 * Pass an empty string to clear all metadata.
 *
 * @param xmpString XMP metadata in XML format (UTF-8)
 * @return true if successful, false otherwise
 */
bool SetDocumentXMP(const std::string& xmpString);

// =========================================================================
// Tier 2: Requires XMP Toolkit SDK (property-level access)
// =========================================================================

/**
 * Initialize the XMP Toolkit SDK.
 * Call this during plugin startup if XMP property-level access is needed.
 * Safe to call even when NUXP_HAS_XMP is not defined (no-op).
 */
void Initialize();

/**
 * Terminate the XMP Toolkit SDK.
 * Call this during plugin shutdown.
 * Safe to call even when NUXP_HAS_XMP is not defined (no-op).
 */
void Terminate();

/**
 * Get a specific XMP property value from the document's metadata.
 * Requires XMP Toolkit SDK (check HasPropertyAccess() first).
 *
 * @param namespaceURI The XMP namespace URI (e.g., "http://ns.adobe.com/xap/1.0/")
 * @param propertyName The property name within the namespace
 * @return Property value as string, or empty string if not found or unavailable
 */
std::string GetProperty(const std::string& namespaceURI, const std::string& propertyName);

/**
 * Set a specific XMP property value in the document's metadata.
 * Requires XMP Toolkit SDK (check HasPropertyAccess() first).
 *
 * This reads the current XMP, modifies the property, and writes it back.
 *
 * @param namespaceURI The XMP namespace URI
 * @param propertyName The property name within the namespace
 * @param value The value to set
 * @return true if successful, false otherwise
 */
bool SetProperty(const std::string& namespaceURI, const std::string& propertyName, const std::string& value);

/**
 * Register a custom XMP namespace.
 * Requires XMP Toolkit SDK (check HasPropertyAccess() first).
 *
 * @param namespaceURI The namespace URI to register
 * @param suggestedPrefix The suggested prefix for the namespace
 * @return The actual registered prefix (may differ from suggested), or empty on failure
 */
std::string RegisterNamespace(const std::string& namespaceURI, const std::string& suggestedPrefix);

} // namespace XMPUtils

#endif // NUXP_XMP_UTILS_HPP
