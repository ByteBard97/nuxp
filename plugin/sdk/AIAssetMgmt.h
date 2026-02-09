/*******************************************************************************
* ADOBE CONFIDENTIAL 
*
* Copyright 1999 Adobe
* All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file in 
* accordance with the terms of the Adobe license agreement accompanying 
* it. If you have received this file from a source other than Adobe, 
* then your use, modification, or distribution of it requires the prior 
* written permission of Adobe. 
*******************************************************************************/

#pragma once

/*******************************************************************************
 **
 **	Imports
 **
 **/

#include "AITypes.h"
#include "IAIFilePath.hpp"
#include "IAILiteralString.h"

#include "AIHeaderBegin.h"

/** @file AIAssetMgmt.h */

/*******************************************************************************
 **
 **	Constants
 **
 **/

#define kAIAssetMgmtSuite				"AI Asset Management Suite"
#define kAIAssetMgmtSuiteVersion10		AIAPI_VERSION(10)
#define kAIAssetMgmtSuiteVersion		kAIAssetMgmtSuiteVersion10
#define kAIAssetMgmtVersion				kAIAssetMgmtSuiteVersion


/** XMP namespace
 */
constexpr ai::LiteralString kAIMetadataNamespaceXMP{ "http://ns.adobe.com/xap/1.0/" };

/** TIFF namespace
 */
constexpr ai::LiteralString kAIMetadataNamespaceTIFF{ "http://ns.adobe.com/tiff/1.0/" };


/** @ingroup Notifiers */
#define kAILaunchProjectBridgeNotifier	"AI Launch Project Bridge Notifier"

/** Metadata selectors for \c #AIAssetMgmtSuite::UpdateDynamicMetadata(),
	controls the types of metadata that are updated. */
enum AIXMPMetadataSelector {
	/** Generate the default XMP metadata */
	kAIXMPDefault				= 0x00000000,
	/** Preserve the original DocumentID (128-bit uuid), rather than creating a new one. */
	kAIXMPPreserveDocumentID	= 0x00000001,
	/** Do not generate page info metadata */
	kAIXMPNoPageInfo			= 0x00000002,
	/** Do not generate document swatches metadata */
	kAIXMPNoDocumentSwatches	= 0x00000004,
	/** Do not generate ink plate usage data */
	kAIXMPNoPlateUsages			= 0x00000008,
	/** Do not generate used-font metadata */
	kAIXMPNoUsedFonts			= 0x00000010,
	/** Do not generate XMP thumbnail metadata */
	kAIXMPNoThumbnail			= 0x00000020
};


/*******************************************************************************
 **
 **	Types
 **
 **/
/** The XMP session object, which allows you to manipulate metadata in the XMP buffer.
You must have XMP data to create a session object.
	*/
typedef struct _AIXMPSession *AIXMPSession;

/** Information for updating XMP metadata using \c #AIAssetMgmtSuite::UpdateDynamicMetadataFields()
 */
typedef struct AIXMPDataFieldMap {
	/** Name of the nameSpace where we wish to add these properties
	 * \c #kAIMetadataNamespaceXMP can be used as default nameSpace to add properties like "CreateDate", "CreatorTool", "Format"
	*/
	const char *nameSpace;
	/** Name of the XMP property, such as "CreateDate", "CreatorTool", "Format" */
	const char *name;
	/** New value of the XMP property. A null pointer value means to
		remove the XMP property from the XMP packet if the \c replace flag is true;
		for example, to remove the XMP "Thumbnails" data from the original packet. */
	const char *value;
	/** When true, reset the value of the XMP property if it already
		exists in the input XMP metadata buffer. */
	AIBoolean	replace;
} AIXMPDataFieldMap;


/*******************************************************************************
 **
 **	Suite
 **
 **/

/** @ingroup Suites
	This suite provides simple XMP metadata manipulation routines.

	\li Acquire this suite using \c #SPBasicSuite::AcquireSuite() with the constants
		\c #kAIAssetMgmtSuite and \c #kAIAssetMgmtVersion.
*/

typedef struct AIAssetMgmtSuite {

	/** Updates XMP metadata. Creates a new XMP metadata buffer.
			@param xapIn The buffer containing the XMP metadata for the current document.
			@param fields A pointer to an array of structures containing the
				new or changed metadata property name/value pairs.
			@param length The number of structures in \c fields.
			@param xapOut [out] A buffer in which to return the new XMP metadata.
				The caller is responsible for allocating and releasing the returned output buffer.
		@note For details of the XMP properties, see the Adobe "XMP Specification",
		@see \c  #AIDocumentSuite::GetDocumentXAP(), \c #AIDocumentSuite::SetDocumentXAP()
		for getting and setting the XMP metadata buffer for the current document.
	*/
	AI_THREAD_SAFE_API AIErr (*UpdateDynamicMetadataFields) (const char *xapIn,
		const AIXMPDataFieldMap *fields, const ai::int32 length, char **xapOut);

	/** Used internally to update the current document's XMP metadata.
			@param mimeType	 The MIME type of the file being updated.
				Updates the "Format" property.
			@param managedURL The URL of the file being updated, or \c NULL
				for local (unmanaged) files.Updates the "ManageTo" field of \c kXMP_NS_XMP_MM namespace.
			@param metadataSelectors A logical OR of \c #AIXMPMetadataSelector constants
				that specifies which types of metadata to update.
	*/
	AIAPI AIErr (*UpdateDynamicMetadata) (const char *mimeType, const char* managedURL, ai::uint32 metadataSelectors);

	//  Currently used by StockPhoto.
	/** Shows the FileInfo dialog for an XMP packet.
			@param xmpIn The current XMP metadata packet.
			@param xmpOUT [out] A buffer in which to return the  new XMP packet,
				allocated by \c #SPBasicSuite::AllocateBlock(). Caller must free memory when no longer needed.
			 	If \c NULL, the function shows a read-only FileInfo dialog.
			@param dialogTitle A UTF-8 title string for the dialog, typically the name of the artwork
				associated with the metadata. If a \c NULL pointer, the default title is "File Info".

		@note The function does not check whether UserInteraction is on or off, the caller must do so..
	*/
	AIAPI AIErr (*ShowMetadataDialog) (const char *xmpIn, char **xmpOut, const char *dialogTitle);

} AIAssetMgmtSuite;

#include "AIHeaderEnd.h"
