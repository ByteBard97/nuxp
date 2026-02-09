/*******************************************************************************
* ADOBE CONFIDENTIAL 
*
* Copyright 2025 Adobe
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

/** Type constants for AVIF anti aliasing.
**/
enum class AIAVIFEncodeAntiAlias : ai::uint8
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
 ** @see	\c #AIAVIFEncodeOptions::mLosslessCompression .
 **/
constexpr ai::LiteralString kAIAVIFEncodeLosslessCompression{ "AIAVIFEncodeLosslessCompression" };
/** The default value for \c #kAIAVIFEncodeLosslessCompression. 
**/
constexpr AIBool8 kAIAVIFEncodeLosslessCompressionDefault = true;


/** In lossy compression, indicates the quality of the image to maintain.
 ** Valid range: 0 - 63.
 ** Higher value would try to retain more details in the image at the
 ** cost of larger file size.
 **	Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIAVIFEncodeOptions::mImageQuality .
 **/
constexpr ai::LiteralString kAIAVIFEncodeImageQuality{ "AIAVIFEncodeImageQuality" };
/** The default value for \c #kAIAVIFEncodeImageQuality. 
**/
constexpr ai::uint8 kAIAVIFEncodeImageQualityDefault = 63;


/** Indicates whether or not to include alpha channel.
 ** If false, alpha channel is omitted and \c #kAIAVIFEncodeMatteColorRed ,
 ** \c #kAIAVIFEncodeMatteColorGreen and \c #kAIAVIFEncodeMatteColorBlue
 ** is used as background of image.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIAVIFEncodeOptions::mIsTransparent .
 **/
constexpr ai::LiteralString kAIAVIFEncodeIsTransparent{ "AIAVIFEncodeIsTransparent" };
/** The default value for \c #kAIAVIFEncodeIsTransparent. 
**/
constexpr AIBool8 kAIAVIFEncodeIsTransparentDefault = true; 


/** If true, causes ICC color profile to be embedded in the file.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIAVIFEncodeOptions::mEmbedICCProfile .
 **/
constexpr ai::LiteralString kAIAVIFEncodeEmbedICCProfile{ "AIAVIFEncodeEmbedICCProfile" };
/** The default value for \c #kAIAVIFEncodeEmbedICCProfile. 
**/
constexpr AIBool8 kAIAVIFEncodeEmbedICCProfileDefault = true;


/** If true, causes XMP Metadata to be included in the file.
 ** Use with \c #AIDicionarySuite::SetBooleanEntry .
 ** @see	\c #AIAVIFEncodeOptions::mIncludeXMPMetadata .
 **/
constexpr ai::LiteralString kAIAVIFEncodeIncludeXMPMetadata{ "AIAVIFEncodeIncludeXMPMetadata" };
/** The default value for \c #kAIAVIFEncodeIncludeXMPMetadata. 
**/
constexpr AIBool8 kAIAVIFEncodeIncludeXMPMetadataDefault = false;


/** Anti-aliasing technique to use in rasterizing.
 ** Valid values: 0 (none), 1 (art-optimized) and 2 (type-optimized).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIAVIFEncodeAntiAlias and \c #AIAVIFEncodeOptions::mAntiAlias .
 **/
constexpr ai::LiteralString kAIAVIFEncodeAntiAlias{ "AIAVIFEncodeAntiAlias" };
/** The default value for \c #kAIAVIFEncodeAntiAlias. 
**/
constexpr AIAVIFEncodeAntiAlias kAIAVIFEncodeAntiAliasDefault = AIAVIFEncodeAntiAlias::kTypeOptimized;


/** Image resolution in pixels-per-inch.
 ** Use with \c #AIDicionarySuite::SetRealEntry .
 ** @see	\c #AIAVIFEncodeOptions::mPPI .
 **/
constexpr ai::LiteralString kAIAVIFEncodePPI{ "AIAVIFEncodePPI" };
/** The default value for \c #kAIAVIFEncodePPI. 
**/
constexpr AIReal kAIAVIFEncodePPIDefault = 72.0;


/** Matte color (red component) to be used for background if image is not transparent.
 ** Ignored if image is transparent.
 ** Valid range: 0 - 65535 (0xffff).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIAVIFEncodeOptions::mMatteColor , \c #kAIAVIFEncodeMatteColorGreen and
 ** 		\c #kAIAVIFEncodeMatteColorBlue .
 **/
constexpr ai::LiteralString kAIAVIFEncodeMatteColorRed{ "AIAVIFEncodeMatteColorRed" };
/** The default value for \c #kAIAVIFEncodeMatteColorRed. 
**/
constexpr ai::uint16 kAIAVIFEncodeMatteColorRedDefault = 0xffff;


/** Matte color (green component) to be used for background if image is not transparent.
 ** Ignored if image is transparent.
 ** Valid range: 0 - 65535 (0xffff).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIAVIFEncodeOptions::mMatteColor , \c #kAIAVIFEncodeMatteColorRed and
 ** 		\c #kAIAVIFEncodeMatteColorBlue .
 **/
constexpr ai::LiteralString kAIAVIFEncodeMatteColorGreen{ "AIAVIFEncodeMatteColorGreen" };
/** The default value for \c #kAIAVIFEncodeMatteColorGreen. 
**/
constexpr ai::uint16 kAIAVIFEncodeMatteColorGreenDefault = 0xffff;


/** Matte color (blue component) to be used for background if image is not transparent.
 ** Ignored if image is transparent.
 ** Valid range: 0 - 65535 (0xffff).
 ** Use with \c #AIDicionarySuite::SetIntegerEntry .
 ** @see	\c #AIAVIFEncodeOptions::mMatteColor , \c #kAIAVIFEncodeMatteColorRed and
 ** 		\c #kAIAVIFEncodeMatteColorGreen .
 **/
constexpr ai::LiteralString kAIAVIFEncodeMatteColorBlue{ "AIAVIFEncodeMatteColorBlue" };
/** The default value for \c #kAIAVIFEncodeMatteColorBlue. 
**/
constexpr ai::uint16 kAIAVIFEncodeMatteColorBlueDefault = 0xffff;

struct AIAVIFEncodeOptions
{

	/** Indicates whether to have lossless or lossy compression.
	 ** @see	\c #kAIAVIFEncodeLosslessCompression .
	 **/
	AIBool8 mLosslessCompression{ kAIAVIFEncodeLosslessCompressionDefault };

	/** In lossy compression, indicates the quality of the image to maintain.
	 ** Valid range: 0 - 63.
	 ** Higher value would try to retain more details in the image at the
	 ** cost of larger file size.
	 ** @see	\c #kAIAVIFEncodeImageQuality .
	 **/
	ai::uint8 mImageQuality{ kAIAVIFEncodeImageQualityDefault };

	/** Indicates whether or not to include alpha channel.
	 ** If false, alpha channel is omitted and \c #AIAVIFEncodeOptions::mMatteColor
	 ** is used as background of image.
	 ** @see	\c #kAIAVIFEncodeIsTransparent .
	 **/
	AIBool8 mIsTransparent{ kAIAVIFEncodeIsTransparentDefault };

	/** If true, causes ICC color profile to be embedded in the file.
	 ** @see	\c #kAIAVIFEncodeEmbedICCProfile .
	 **/
	AIBool8 mEmbedICCProfile{ kAIAVIFEncodeEmbedICCProfileDefault };

	/** If true, causes XMP Metadata to be included in the file.
	 ** @see	\c #kAIAVIFEncodeIncludeXMPMetadata .
	 **/
	AIBool8 mIncludeXMPMetadata{ kAIAVIFEncodeIncludeXMPMetadataDefault };

	/** Anti-aliasing technique to use in rasterizing.
	 ** @see	\c #AIAVIFEncodeAntiAlias and \c #kAIAVIFEncodeAntiAlias .
	 **/
	AIAVIFEncodeAntiAlias mAntiAlias{ kAIAVIFEncodeAntiAliasDefault };

	/** Image resolution in pixels-per-inch.
	 ** @see	\c #kAIAVIFEncodePPI .
	 **/
	AIReal mPPI{ kAIAVIFEncodePPIDefault };

	/** Matte color to be used for background if image is not transparent.
	 ** Ignored if image is transparent.
	 ** @see	\c #AIAVIFEncodeOptions::mIsTransparent , \c #kAIAVIFEncodeMatteColorRed ,
	 ** 		\c #kAIAVIFEncodeMatteColorGreen and \c #kAIAVIFEncodeMatteColorBlue .
	 **/
	AIRGBColor mMatteColor{ kAIAVIFEncodeMatteColorRedDefault, kAIAVIFEncodeMatteColorGreenDefault, kAIAVIFEncodeMatteColorBlueDefault };

};

#include "AIHeaderEnd.h"
