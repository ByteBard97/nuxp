/*******************************************************************************
* ADOBE CONFIDENTIAL 
*
* Copyright 2023 Adobe
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
#include "IAILiteralString.h"

#include "AIHeaderBegin.h"

/*******************************************************************************
 **
 **	Types
 **
 **/

/** Type constants for WebP anti aliasing.
**/
enum class AIWebPEncodeAntiAlias : ai::uint8
{
	kNone,

	kArtOptimized,

	kTypeOptimized,
};

/*******************************************************************************
**
**	Constants
**
**/

/** Indicates whether to have lossless or lossy compression.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIWebPEncodeOptions::mLosslessCompression .
 **/
constexpr ai::LiteralString kAIWebPEncodeLosslessCompression{ "AIWebPEncodeLosslessCompression" };
/** The default value for \c #kAIWebPEncodeLosslessCompression. 
**/
constexpr AIBool8 kAIWebPEncodeLosslessCompressionDefault = true;


/** In lossy compression, indicates the quality of the image to maintain.
 ** Valid range: 0 - 100.
 ** Higher value would try to retain more details in the image at the
 ** cost of larger file size.
 **	Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIWebPEncodeOptions::mImageQuality .
 **/
constexpr ai::LiteralString kAIWebPEncodeImageQuality{ "AIWebPEncodeImageQuality" };
/** The default value for \c #kAIWebPEncodeImageQuality. 
**/
constexpr ai::uint8 kAIWebPEncodeImageQualityDefault = 100;


/** Indicates whether or not to include alpha channel.
 ** If false, alpha channel is omitted and \c #kAIWebPEncodeMatteColorRed ,
 ** \c #kAIWebPEncodeMatteColorGreen and \c #kAIWebPEncodeMatteColorBlue
 ** is used as background of image.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIWebPEncodeOptions::mIsTransparent .
 **/
constexpr ai::LiteralString kAIWebPEncodeIsTransparent{ "AIWebPEncodeIsTransparent" };
/** The default value for \c #kAIWebPEncodeIsTransparent. 
**/
constexpr AIBool8 kAIWebPEncodeIsTransparentDefault = true; 


/** If true, causes ICC color profile to be embedded in the file.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIWebPEncodeOptions::mEmbedICCProfile .
 **/
constexpr ai::LiteralString kAIWebPEncodeEmbedICCProfile{ "AIWebPEncodeEmbedICCProfile" };
/** The default value for \c #kAIWebPEncodeEmbedICCProfile. 
**/
constexpr AIBool8 kAIWebPEncodeEmbedICCProfileDefault = true;


/** If true, causes XMP Metadata to be included in the file.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIWebPEncodeOptions::mIncludeXMPMetadata .
 **/
constexpr ai::LiteralString kAIWebPEncodeIncludeXMPMetadata{ "AIWebPEncodeIncludeXMPMetadata" };
/** The default value for \c #kAIWebPEncodeIncludeXMPMetadata. 
**/
constexpr AIBool8 kAIWebPEncodeIncludeXMPMetadataDefault = false;


/** Anti-aliasing technique to use in rasterizing.
 ** Valid values: 0 (none), 1 (art-optimized) and 2 (type-optimized).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIWebPEncodeAntiAlias and \c #AIWebPEncodeOptions::mAntiAlias .
 **/
constexpr ai::LiteralString kAIWebPEncodeAntiAlias{ "AIWebPEncodeAntiAlias" };
/** The default value for \c #kAIWebPEncodeAntiAlias. 
**/
constexpr AIWebPEncodeAntiAlias kAIWebPEncodeAntiAliasDefault = AIWebPEncodeAntiAlias::kTypeOptimized;


/** Image resolution in pixels-per-inch.
 ** Use with \c #AIDicionarySuite::SetRealEntry .
 ** @see	\c #AIWebPEncodeOptions::mPPI .
 **/
constexpr ai::LiteralString kAIWebPEncodePPI{ "AIWebPEncodePPI" };
/** The default value for \c #kAIWebPEncodePPI. 
**/
constexpr AIReal kAIWebPEncodePPIDefault = 72.0;


/** Matte color (red component) to be used for background if image is not transparent.
 ** Ignored if image is transparent.
 ** Valid range: 0 - 65535 (0xffff).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIWebPEncodeOptions::mMatteColor , \c #kAIWebPEncodeMatteColorGreen and
 ** 		\c #kAIWebPEncodeMatteColorBlue .
 **/
constexpr ai::LiteralString kAIWebPEncodeMatteColorRed{ "AIWebPEncodeMatteColorRed" };
/** The default value for \c #kAIWebPEncodeMatteColorRed. 
**/
constexpr ai::uint16 kAIWebPEncodeMatteColorRedDefault = 0xffff;


/** Matte color (green component) to be used for background if image is not transparent.
 ** Ignored if image is transparent.
 ** Valid range: 0 - 65535 (0xffff).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIWebPEncodeOptions::mMatteColor , \c #kAIWebPEncodeMatteColorRed and
 ** 		\c #kAIWebPEncodeMatteColorBlue .
 **/
constexpr ai::LiteralString kAIWebPEncodeMatteColorGreen{ "AIWebPEncodeMatteColorGreen" };
/** The default value for \c #kAIWebPEncodeMatteColorGreen. 
**/
constexpr ai::uint16 kAIWebPEncodeMatteColorGreenDefault = 0xffff;


/** Matte color (blue component) to be used for background if image is not transparent.
 ** Ignored if image is transparent.
 ** Valid range: 0 - 65535 (0xffff).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIWebPEncodeOptions::mMatteColor , \c #kAIWebPEncodeMatteColorRed and
 ** 		\c #kAIWebPEncodeMatteColorGreen .
 **/
constexpr ai::LiteralString kAIWebPEncodeMatteColorBlue{ "AIWebPEncodeMatteColorBlue" };
/** The default value for \c #kAIWebPEncodeMatteColorBlue. 
**/
constexpr ai::uint16 kAIWebPEncodeMatteColorBlueDefault = 0xffff;

struct AIWebPEncodeOptions
{

	/** Indicates whether to have lossless or lossy compression.
	 ** @see	\c #kAIWebPEncodeLosslessCompression .
	 **/
	AIBool8 mLosslessCompression{ kAIWebPEncodeLosslessCompressionDefault };

	/** In lossy compression, indicates the quality of the image to maintain.
	 ** Valid range: 0 - 100.
	 ** Higher value would try to retain more details in the image at the
	 ** cost of larger file size.
	 ** @see	\c #kAIWebPEncodeImageQuality .
	 **/
	ai::uint8 mImageQuality{ kAIWebPEncodeImageQualityDefault };

	/** Indicates whether or not to include alpha channel.
	 ** If false, alpha channel is omitted and \c #AIWebPEncodeOptions::mMatteColor
	 ** is used as background of image.
	 ** @see	\c #kAIWebPEncodeIsTransparent .
	 **/
	AIBool8 mIsTransparent{ kAIWebPEncodeIsTransparentDefault };

	/** If true, causes ICC color profile to be embedded in the file.
	 ** @see	\c #kAIWebPEncodeEmbedICCProfile .
	 **/
	AIBool8 mEmbedICCProfile{ kAIWebPEncodeEmbedICCProfileDefault };

	/** If true, causes XMP Metadata to be included in the file.
	 ** @see	\c #kAIWebPEncodeIncludeXMPMetadata .
	 **/
	AIBool8 mIncludeXMPMetadata{ kAIWebPEncodeIncludeXMPMetadataDefault };

	/** Anti-aliasing technique to use in rasterizing.
	 ** @see	\c #AIWebPEncodeAntiAlias and \c #kAIWebPEncodeAntiAlias .
	 **/
	AIWebPEncodeAntiAlias mAntiAlias{ kAIWebPEncodeAntiAliasDefault };

	/** Image resolution in pixels-per-inch.
	 ** @see	\c #kAIWebPEncodePPI .
	 **/
	AIReal mPPI{ kAIWebPEncodePPIDefault };

	/** Matte color to be used for background if image is not transparent.
	 ** Ignored if image is transparent.
	 ** @see	\c #AIWebPEncodeOptions::mIsTransparent , \c #kAIWebPEncodeMatteColorRed ,
	 ** 		\c #kAIWebPEncodeMatteColorGreen and \c #kAIWebPEncodeMatteColorBlue .
	 **/
	AIRGBColor mMatteColor{ kAIWebPEncodeMatteColorRedDefault, kAIWebPEncodeMatteColorGreenDefault, kAIWebPEncodeMatteColorBlueDefault };

};

#include "AIHeaderEnd.h"
