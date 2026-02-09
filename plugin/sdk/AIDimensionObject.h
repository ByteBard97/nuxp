/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2023 Adobe
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains
* the property of Adobe and its suppliers, if any. The intellectual
* and technical concepts contained herein are proprietary to Adobe
* and its suppliers and are protected by all applicable intellectual
* property laws, including trade secret and copyright laws.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe.
**************************************************************************/

#pragma once

#include "AITypes.h"

#include "AIHeaderBegin.h"

#define kAIDimensionObjectSuite                  "AI Dimension Object Suite"
#define kAIDimensionObjectSuiteVersion1          AIAPI_VERSION(1)
#define kAIDimensionObjectSuiteVersion           kAIDimensionObjectSuiteVersion1
#define kAIDimensionObjectVersion                kAIDimensionObjectSuiteVersion

struct AIDimensionObjectSuite
{
	/** Checks if the given art object is a dimension Art
		@param inGroup The art object to check.
		@return True if the art object passed is of type Dimension, False otherwise.
	 */
	AIAPI AIBoolean (*IsDimension) (AIArtHandle inGroup);
	
	/** Expand dimension Art
		@param inGroup The dimension art object to be expanded
		@return Art handle of the expanded group
	 */
	AIAPI AIArtHandle (*Expand) (AIArtHandle inGroup);
};

#include "AIHeaderEnd.h"
