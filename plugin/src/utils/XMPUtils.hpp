/**
 * NUXP XMP Utilities
 *
 * Manual wrappers for Adobe XMP (Extensible Metadata Platform) operations.
 *
 * IMPORTANT: This module requires the Adobe XMP SDK which is separate from
 * the Illustrator SDK. The XMP SDK provides types like AXE_PluginRef and
 * XMP namespace handling that cannot be code-generated.
 *
 * To enable XMP functionality:
 * 1. Download Adobe XMP SDK from https://www.adobe.com/devnet/xmp.html
 * 2. Add XMP SDK headers to include path
 * 3. Link against XMP SDK libraries
 * 4. Define NUXP_HAS_XMP in your CMake configuration
 *
 * Usage (when XMP SDK is available):
 *   #include "utils/XMPUtils.hpp"
 *
 *   std::string xmp = XMPUtils::GetDocumentXMP();
 *   bool success = XMPUtils::SetDocumentXMP(xmpString);
 *   std::string value = XMPUtils::GetProperty(ns, propName);
 *   bool success = XMPUtils::SetProperty(ns, propName, value);
 */

#ifndef NUXP_XMP_UTILS_HPP
#define NUXP_XMP_UTILS_HPP

#ifdef NUXP_HAS_XMP

#include <nlohmann/json.hpp>
#include <string>

namespace XMPUtils {

using json = nlohmann::json;

/**
 * Check if XMP functionality is available.
 *
 * @return true if XMP SDK is linked and available, false otherwise
 */
bool IsAvailable();

/**
 * Get the XMP metadata for the current document.
 *
 * @return XMP string in XML format, or empty string if unavailable/error
 */
std::string GetDocumentXMP();

/**
 * Set the XMP metadata for the current document.
 *
 * @param xmpString XMP metadata in XML format
 * @return true if successful, false otherwise
 */
bool SetDocumentXMP(const std::string& xmpString);

/**
 * Get a specific XMP property value.
 *
 * @param namespaceURI The XMP namespace URI (e.g., "http://ns.adobe.com/xap/1.0/")
 * @param propertyName The property name within the namespace
 * @return Property value as string, or empty string if not found
 */
std::string GetProperty(const std::string& namespaceURI, const std::string& propertyName);

/**
 * Set a specific XMP property value.
 *
 * @param namespaceURI The XMP namespace URI
 * @param propertyName The property name within the namespace
 * @param value The value to set
 * @return true if successful, false otherwise
 */
bool SetProperty(const std::string& namespaceURI, const std::string& propertyName, const std::string& value);

/**
 * Register a custom XMP namespace.
 *
 * @param namespaceURI The namespace URI to register
 * @param suggestedPrefix The suggested prefix for the namespace
 * @return The actual registered prefix (may differ from suggested)
 */
std::string RegisterNamespace(const std::string& namespaceURI, const std::string& suggestedPrefix);

} // namespace XMPUtils

#else
// XMP SDK not available - provide inline no-op stubs
#include <string>
namespace XMPUtils {
    inline bool IsAvailable() { return false; }
    inline std::string GetDocumentXMP() { return ""; }
    inline bool SetDocumentXMP(const std::string&) { return false; }
    inline std::string GetProperty(const std::string&, const std::string&) { return ""; }
    inline bool SetProperty(const std::string&, const std::string&, const std::string&) { return false; }
    inline std::string RegisterNamespace(const std::string&, const std::string&) { return ""; }
} // namespace XMPUtils
#endif // NUXP_HAS_XMP

#endif // NUXP_XMP_UTILS_HPP
