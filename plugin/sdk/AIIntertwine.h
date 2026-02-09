/*************************************************************************
 *
 * ADOBE CONFIDENTIAL
 *
 * Copyright 2022 Adobe
 *
 * All Rights Reserved.
 *
 * NOTICE: Adobe permits you to use, modify, and distribute this file in
 * accordance with the terms of the Adobe license agreement accompanying
 * it. If you have received this file from a source other than Adobe,
 * then your use, modification, or distribution of it requires the prior
 * written permission of Adobe.
 *
 **************************************************************************/

#pragma once

#include "AITypes.h"

#include "AIHeaderBegin.h"

#define kAIIntertwineSuite                  "AI Intertwine Suite"
#define kAIIntertwineSuiteVersion1          AIAPI_VERSION(1)
#define kAIIntertwineSuiteVersion           kAIIntertwineSuiteVersion1
#define kAIIntertwineVersion                kAIIntertwineSuiteVersion

struct AIIntertwineSuite
{
	/** Checks if the given art object is of type intertwine.
		@param inGroup The art object  to check.
		@return True if the art object passed is of type intertwine, False otherwise.
	 */
	AIAPI AIBoolean( *IsIntertwined ) ( AIArtHandle inGroup );
};

#include "AIHeaderEnd.h"
