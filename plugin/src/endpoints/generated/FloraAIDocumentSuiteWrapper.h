#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIDocumentSuite {

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileSpecification
 * @returns ["file"] - ai::FilePath
 */
nlohmann::json GetDocumentFileSpecification(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileSpecificationFromHandle
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["file"] - ai::FilePath
 */
nlohmann::json GetDocumentFileSpecificationFromHandle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentPageOrigin
 * @returns ["origin"] - AIRealPoint
 */
nlohmann::json GetDocumentPageOrigin(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentPageOrigin
 * @returns ["origin"] - AIRealPoint
 */
nlohmann::json SetDocumentPageOrigin(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentRulerOrigin
 * @returns ["origin"] - AIRealPoint
 */
nlohmann::json SetDocumentRulerOrigin(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentRulerUnits
 * @returns ["units"] - ai::int16
 */
nlohmann::json GetDocumentRulerUnits(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentRulerUnits
 * @param params["units"] - ai::int16
 */
nlohmann::json SetDocumentRulerUnits(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentCropStyle
 * @returns ["cropStyle"] - AICropMarkStyle
 */
nlohmann::json GetDocumentCropStyle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentCropStyle
 * @param params["cropStyle"] - AICropMarkStyle
 */
nlohmann::json SetDocumentCropStyle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentPrintRecord
 * @returns ["print"] - AIDocumentPlatformPrintRecord
 */
nlohmann::json GetDocumentPrintRecord(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentPrintRecord
 * @param params["print"] - AIDocumentPlatformPrintRecord
 */
nlohmann::json SetDocumentPrintRecord(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentSetup
 * @returns ["setup"] - AIDocumentSetup
 */
nlohmann::json GetDocumentSetup(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentSetup
 * @returns ["setup"] - AIDocumentSetup
 */
nlohmann::json SetDocumentSetup(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentModified
 * @returns ["modified"] - AIBoolean
 */
nlohmann::json GetDocumentModified(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentModified
 * @param params["modified"] - AIBoolean
 */
nlohmann::json SetDocumentModified(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileFormat
 * @returns ["fileFormat"] - AIFileFormatHandle
 */
nlohmann::json GetDocumentFileFormat(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentFileFormat
 * @param params["fileFormat"] - AIFileFormatHandle
 */
nlohmann::json SetDocumentFileFormat(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentFileFormatParameters
 * @param params["parameters"] - AIDocumentFileFormatParameters
 */
nlohmann::json SetDocumentFileFormatParameters(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocument
 * @returns ["document"] - AIDocumentHandle (handle ID)
 */
nlohmann::json GetDocument(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::WriteDocument
 * @param params["file"] - const ai::FilePath
 * @param params["fileFormatName"] - const char
 * @param params["askForParms"] - AIBoolean
 */
nlohmann::json WriteDocument(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentMiPrintRecord
 * @param params["print"] - AIDocumentMiPrintRecordPtr
 */
nlohmann::json GetDocumentMiPrintRecord(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentMiPrintRecord
 * @param params["print"] - AIDocumentMiPrintRecordPtr
 */
nlohmann::json SetDocumentMiPrintRecord(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentRulerOrigin
 * @returns ["origin"] - AIRealPoint
 */
nlohmann::json GetDocumentRulerOrigin(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::UpdateLinks
 * @returns ["updatedSomething"] - AIBoolean
 */
nlohmann::json UpdateLinks(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentZoomLimit
 * @returns ["min"] - AIReal
 * @returns ["max"] - AIReal
 */
nlohmann::json GetDocumentZoomLimit(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentMaxArtboardBounds
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetDocumentMaxArtboardBounds(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DocumentExists
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["exists"] - AIBoolean
 */
nlohmann::json DocumentExists(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentColorModel
 * @returns ["colorModel"] - ai::int16
 */
nlohmann::json GetDocumentColorModel(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentColorModel
 * @param params["colorModel"] - ai::int16
 */
nlohmann::json SetDocumentColorModel(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::Copy
 */
nlohmann::json Copy(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::Cut
 */
nlohmann::json Cut(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::Paste
 */
nlohmann::json Paste(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentTargeting
 * @returns ["paintTarget"] - short
 * @returns ["transparencyTarget"] - short
 * @returns ["effectsTarget"] - short
 */
nlohmann::json GetDocumentTargeting(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentTargeting
 * @param params["paintTarget"] - short
 * @param params["transparencyTarget"] - short
 * @param params["effectsTarget"] - short
 */
nlohmann::json SetDocumentTargeting(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DocumentHasTransparency
 * @param params["detectOverprint"] - AIBoolean
 * @returns ["hasTransparency"] - AIBoolean
 */
nlohmann::json DocumentHasTransparency(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DocumentHasSpotColorArt
 * @returns ["hasSpotColorArt"] - AIBoolean
 */
nlohmann::json DocumentHasSpotColorArt(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentAssetMgmtInfo
 * @param params["URL"] - const char
 * @returns ["managed"] - AIBoolean
 * @returns ["checkedOut"] - AIBoolean
 * @returns ["canEdit"] - AIBoolean
 */
nlohmann::json SetDocumentAssetMgmtInfo(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentXAP
 * @param params["xap"] - const char
 */
nlohmann::json SetDocumentXAP(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SuspendTextReflow
 */
nlohmann::json SuspendTextReflow(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::ResumeTextReflow
 */
nlohmann::json ResumeTextReflow(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::HasTextFocus
 * @returns ["focus"] - AIBoolean
 */
nlohmann::json HasTextFocus(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::HasTextCaret
 * @returns ["caret"] - AIBoolean
 */
nlohmann::json HasTextCaret(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetTextFocus
 * @returns ["pStory"] - StoryRef
 */
nlohmann::json GetTextFocus(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetTextFocus
 * @param params["story"] - StoryRef
 */
nlohmann::json SetTextFocus(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::LoseTextFocus
 */
nlohmann::json LoseTextFocus(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentTextResources
 * @returns ["pDocResources"] - DocumentTextResourcesRef
 */
nlohmann::json GetDocumentTextResources(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::WriteDocumentMacInformationResource
 * @param params["file"] - const ai::FilePath
 */
nlohmann::json WriteDocumentMacInformationResource(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::WriteDocumentAsLibrary
 * @param params["file"] - const ai::FilePath
 * @param params["libraryType"] - AILibraryType
 */
nlohmann::json WriteDocumentAsLibrary(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DocumentHasOverprint
 * @returns ["hasOverprint"] - AIBoolean
 */
nlohmann::json DocumentHasOverprint(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DocumentHasManagedLinks
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["hasManagedLinks"] - AIBoolean
 */
nlohmann::json DocumentHasManagedLinks(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentSpotColorMode
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["mode"] - AISpotColorMode
 */
nlohmann::json GetDocumentSpotColorMode(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::Undo
 */
nlohmann::json Undo(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::Redo
 */
nlohmann::json Redo(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DocumentRasterAttributes
 * @returns ["hasDeviceNRasters"] - AIBoolean
 * @returns ["hasOverprint"] - AIBoolean
 */
nlohmann::json DocumentRasterAttributes(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentStartupProfile
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["startupProfile"] - AIDocumentStartupProfile
 */
nlohmann::json GetDocumentStartupProfile(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentBleeds
 * @returns ["bleedOffset"] - AIRealRect
 */
nlohmann::json GetDocumentBleeds(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentBleeds
 * @param params["bleedOffset"] - const AIRealRect
 */
nlohmann::json SetDocumentBleeds(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetDocumentPixelPerfectStatus
 * @param params["isPixelPerfect"] - AIBoolean
 */
nlohmann::json SetDocumentPixelPerfectStatus(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::DeleteSelection
 */
nlohmann::json DeleteSelection(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetAutoAssignUIDOnArtCreation
 * @param params["artType"] - ai::int16
 * @param params["autoAssignUIDOnArtCreation"] - AIBoolean
 */
nlohmann::json SetAutoAssignUIDOnArtCreation(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetAutoAssignUIDOnArtCreation
 * @param params["artType"] - ai::int16
 * @returns ["outAutoAssignUIDOnArtCreation"] - AIBoolean
 */
nlohmann::json GetAutoAssignUIDOnArtCreation(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentScale
 * @returns ["docScale"] - AIReal
 */
nlohmann::json GetDocumentScale(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileName
 * @returns ["fileName"] - ai::UnicodeString
 */
nlohmann::json GetDocumentFileName(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileNameNoExt
 * @returns ["fileName"] - ai::UnicodeString
 */
nlohmann::json GetDocumentFileNameNoExt(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileNameFromHandle
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["fileName"] - ai::UnicodeString
 */
nlohmann::json GetDocumentFileNameFromHandle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetDocumentFileNameNoExtFromHandle
 * @param params["document"] - AIDocumentHandle (handle ID)
 * @returns ["fileName"] - ai::UnicodeString
 */
nlohmann::json GetDocumentFileNameNoExtFromHandle(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::GetLastExportedFilePath
 * @param params["option"] - const AIExportTriggeredFrom
 * @returns ["file"] - ai::FilePath
 */
nlohmann::json GetLastExportedFilePath(const nlohmann::json& params);

/**
 * Wrapper for AIDocumentSuite::SetLastExportedFilePath
 * @param params["option"] - const AIExportTriggeredFrom
 * @param params["file"] - ai::FilePath
 */
nlohmann::json SetLastExportedFilePath(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIDocumentSuite
} // namespace Flora
