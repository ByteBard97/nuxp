/*************************************************************************
*
* ADOBE CONFIDENTIAL
*
* Copyright 2025 Adobe
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

enum AIErrorCode : ai::int32
{
    kAICoreErrorRangeStart                          = 0x00000001, // = 1.    Start of Error Range used by Illustrator core (app) only
    kAICoreCanceledError                            = 129,
    kAICoreErrorRangeEnd                            = 0x00000800, // = 2048. End of Error Range used by Illustrator core (app) only

    //----------------------------------------------------------------------------------------------------------
    kAICommonErrorRangeStart                        = kAICoreErrorRangeEnd + 1, // = 2049. Start of Error Range used by Illustrator plugins and core (app)

    kAIAlreadyExportingSameDocument                 = kAICommonErrorRangeStart + 1, // = 2050 //Returned if there is already a request being processed to export the same document.
    kAIOriginalNetworkPathDoesNotExist              = kAICommonErrorRangeStart + 2, //Original network path is not found, may be it is called when optimized open/save is not being performed
    kAIScratchFolderNotAvailable                    = kAICommonErrorRangeStart + 3, //Scratch folder is not accessible
    kAISufficientScratchDiskSpaceNotAvailable       = kAICommonErrorRangeStart + 4, //Scratch folder does not have enough space
    kAIMultipleArtboardInASingleArtboardWorkflow    = kAICommonErrorRangeStart + 5, //Work flow does not support Multiple artboard documents
    kAIPreConditionNotMet                           = kAICommonErrorRangeStart + 6, //Pre-condition for an work-flow is not met
    kAIFeatureNotOptedIn                            = kAICommonErrorRangeStart + 7, //Feature not opted-in
    kAIFeatureNotEnabled                            = kAICommonErrorRangeStart + 8, //Feature not enabled
    kAIFileCopyToNetworkLocationFailed              = kAICommonErrorRangeStart + 9,  //File could not be copied to a network location, may be due to network failure
    kAIJSONParsingFailed                            = kAICommonErrorRangeStart + 10, //Error When Parsing A JSON value
    kAIVerificationFailedErr                        = kAICommonErrorRangeStart + 11, // = 2060 //Error when Document Corruption Verification is failed
    kAIFileReadError                                = kAICommonErrorRangeStart + 12, //Error when reading a file
    kAIFileRenameError                              = kAICommonErrorRangeStart + 13, //Error while renaming file
    kAICorruptLinkErr                               = kAICommonErrorRangeStart + 14, //Error while opening a linked file due to Corruption
    kAICantCutAllArtboardsErr                       = kAICommonErrorRangeStart + 15, //Artboard Copy-Paste Error 
    kAINoSpaceToPasteArtboardErr                    = kAICommonErrorRangeStart + 16, //Artboard Copy-Paste Error - No Space left after copy
    kAIHTTPErr                                      = kAICommonErrorRangeStart + 17,
    kAIDXFDWGSDKErr                                 = kAICommonErrorRangeStart + 18, // Error when calling RealDWG api
    kAILiveEditTimeExceededErr                      = kAICommonErrorRangeStart + 19, // Error in case of live edit fallback occured during operation.
    kAIJsonValueNotAMapErr                          = kAICommonErrorRangeStart + 20,
    kAIFileNotFoundErr                              = kAICommonErrorRangeStart + 21, // = 2070
    kAIFileGeneratedByForegroundSaveNotFoundErr     = kAICommonErrorRangeStart + 22,
    kAINotACloudDocumentErr                         = kAICommonErrorRangeStart + 23,
    kAINotALocalDocumentErr                         = kAICommonErrorRangeStart + 24,
    kCloudDocumentAlreadyOpeningErr                 = kAICommonErrorRangeStart + 25, //The cloud document is already in open 
    kAINotOnMainThreadError                         = kAICommonErrorRangeStart + 26,
    kAINoFeatureFound                               = kAICommonErrorRangeStart + 27,
    kAISignatureNotValid                            = kAICommonErrorRangeStart + 28,
    kAIUserNotRegisteredWithCloudErr                = kAICommonErrorRangeStart + 29,
    kAIInvalidToolBoxDrawerTileViewErr              = kAICommonErrorRangeStart + 30,
    kAIRepeatArtIsolationModeNotActiveErr           = kAICommonErrorRangeStart + 31, // = 2080
    kAIFileDeleteError                              = kAICommonErrorRangeStart + 32, 
    kAIDirectoryDeleteError                         = kAICommonErrorRangeStart + 33,
    kAIFastExportFailedErr                          = kAICommonErrorRangeStart + 34,
    kAICantHibernateInvalidDocType                  = kAICommonErrorRangeStart + 35,
    kAICantHibernatePreferenceOff                   = kAICommonErrorRangeStart + 36,
    kAIInvalidHibernateInfoFile                     = kAICommonErrorRangeStart + 37,
    kAIFileFormatNotFoundErr                        = kAICommonErrorRangeStart + 38,
    kAITaskCantBeScheduledErr                       = kAICommonErrorRangeStart + 39,
    kAILiveEffectNotFoundErr                        = kAICommonErrorRangeStart + 40,
    kAIUserNotLoggedInToCCErr                       = kAICommonErrorRangeStart + 41, // = 2090
    kAILiveEffectParamsNotFoundErr                  = kAICommonErrorRangeStart + 42,
    kAILiveEffectVisibilityHiddenErr                = kAICommonErrorRangeStart + 43,
    kAIFileCopyErr                                  = kAICommonErrorRangeStart + 44,
    kUITaskGroupExistsError                         = kAICommonErrorRangeStart + 45,
    kUITaskGroupDoesNotExistError                   = kAICommonErrorRangeStart + 46,
    kUITaskGroupInvalidHandle                       = kAICommonErrorRangeStart + 47,
    kAIUnsupportedSkiaFeatureErr                    = kAICommonErrorRangeStart + 48,
    kAIUnsupportedAGMGPUFeatureErr                  = kAICommonErrorRangeStart + 49,
    kAISkipEffectExecutionErr                       = kAICommonErrorRangeStart + 50,
    kAICodecNotAvailableErr                         = kAICommonErrorRangeStart + 51, // = 2100
    kAIPDFFormatUnknownErr                          = kAICommonErrorRangeStart + 52,
    kAICantErr                                      = kAICommonErrorRangeStart + 53,
    kAINotEnoughRAMErr                              = kAICommonErrorRangeStart + 54,
    kAIBrokenJPEGErr                                = kAICommonErrorRangeStart + 55,
    kAIPluginLoadingErr                             = kAICommonErrorRangeStart + 56,
    kAIFileReadWriteErr                             = kAICommonErrorRangeStart + 57,
    kAIInvalidObjectsIgnoredErr                     = kAICommonErrorRangeStart + 58,
    kAISufficientDiskSpaceNotAvailable              = kAICommonErrorRangeStart + 59, /* more generic error which indicates insufficient disk space*/
    kAITextResourceLoadingFailed                    = kAICommonErrorRangeStart + 60, /* error if text resources are not loaded during document read*/
    kAINotEntitledErr                               = kAICommonErrorRangeStart + 61, // = 2110
    kAIInvalidVMStreamErr                           = kAICommonErrorRangeStart + 62,
    kAIImageSizeTooSmallForMockupErr                = kAICommonErrorRangeStart + 63,
    kUnSupportedLanguageError                       = kAICommonErrorRangeStart + 64,
    kBlockedContentError                            = kAICommonErrorRangeStart + 65,
    kInternetDisconnectedError                      = kAICommonErrorRangeStart + 66,
    kAIFFQuotaExhaustedError                        = kAICommonErrorRangeStart + 67,
    kAIFFBlockByAdminError                          = kAICommonErrorRangeStart + 68,
    kAIFFBlockDueToViolationError                   = kAICommonErrorRangeStart + 69,
    kAIFFNotEntitledError                           = kAICommonErrorRangeStart + 70,
    kAIFFProfileDeniedError                         = kAICommonErrorRangeStart + 71, // = 2120
    kAIFFInvalidScopeError                          = kAICommonErrorRangeStart + 72,
    kAIFFThrottledError                             = kAICommonErrorRangeStart + 73,
    kAIFFUnauthorizedError                          = kAICommonErrorRangeStart + 74,
    kAIFFGeoBlockError                              = kAICommonErrorRangeStart + 75,
    kNotEnoughSpaceAvailableErr                     = kAICommonErrorRangeStart + 76,
    kAINoVectorToFormMockupErr                      = kAICommonErrorRangeStart + 77,
    kAINoGPUToFormMockupErr                         = kAICommonErrorRangeStart + 78,
    kAIOnDemandModuleNotFoundErr                    = kAICommonErrorRangeStart + 79,
    kBlockedArtistContentError                      = kAICommonErrorRangeStart + 80,
    kAIFFBlockDueToInvalidSubscription              = kAICommonErrorRangeStart + 81, // = 2130
    kAIFFValidationError                            = kAICommonErrorRangeStart + 82,
    kAIImageContainsPSConstructsErr                 = kAICommonErrorRangeStart + 83,
    kAIUnsupportedColorSpaceErr                     = kAICommonErrorRangeStart + 84,
    kAIPreloadPluginNotFoundErr                     = kAICommonErrorRangeStart + 85,
    kAINotFoundError                                = kAICommonErrorRangeStart + 86,
    kEmptyArtBoundsErr                              = kAICommonErrorRangeStart + 87,
    kAIPSDMissingCompositeErr                       = kAICommonErrorRangeStart + 88,
    kAINeedUserInputErr                             = kAICommonErrorRangeStart + 89,
    kPreloadNotReadyErr                             = kAICommonErrorRangeStart + 90,
    kSnippetDocTextUnsupportedErr                   = kAICommonErrorRangeStart + 91, // = 2140
    kInvalidSelectionErr                            = kAICommonErrorRangeStart + 92,
    kAIUnsupportedTypeErr                           = kAICommonErrorRangeStart + 93,
    kAIInvalidComponentCountErr						= kAICommonErrorRangeStart + 94, 
	kAITextSaveFailedErr							= kAICommonErrorRangeStart + 95,
	kAIPDFLSaveFailedErr		       				= kAICommonErrorRangeStart + 96,
	kAITextError 									= kAICommonErrorRangeStart + 97,

    //Sentinel Specific Errors
    kSentinelErrorCodeStarted                       = kAICommonErrorRangeStart + 150, // = 2199
    kAIInvalidSHMStreamErr                          = kSentinelErrorCodeStarted + 1, // = 2200
    kAIInvalidRecoveryInfo                          = kSentinelErrorCodeStarted + 2,
    kAIMonitorNotLaunched                           = kSentinelErrorCodeStarted + 3,
    kAISHMLoadError                                 = kSentinelErrorCodeStarted + 4,
    kAISentinelDirectoryErr                         = kSentinelErrorCodeStarted + 5,
    kAISentinelTempDirErr                           = kSentinelErrorCodeStarted + 6,
    kAISentinelSerializeIPCCacheMapErr              = kSentinelErrorCodeStarted + 7,
    kAISentinelSerializeIPCStreamErr                = kSentinelErrorCodeStarted + 8,
    kAISentinelRenametoOldErr                       = kSentinelErrorCodeStarted + 9,
    kAISentinelRenameTemptoSentinelErr              = kSentinelErrorCodeStarted + 10,
    kAISentinelSerializeImageArrayErr               = kSentinelErrorCodeStarted + 11, // = 2210
    kAIElixirDocViewSaveError                       = kSentinelErrorCodeStarted + 12,
    kAIObjectStreamSizeTooLargeErr                  = kSentinelErrorCodeStarted + 13,

    //Robin Specific errors
    kAIRobinErrorCodeStarted                        = kAICommonErrorRangeStart + 1100, // = 3149
    kAIRobinClientNotRegisteredWithVulcanErr        = kAIRobinErrorCodeStarted + 1, // = 3150
    kAIRobinServiceIsDownErr                        = kAIRobinErrorCodeStarted + 2,
    kAIRobinCommunicationBrokenErr                  = kAIRobinErrorCodeStarted + 3,
    kAIRobinServiceFailedToOpenFileErr              = kAIRobinErrorCodeStarted + 4,
    kAIRobinJobFailedUptoMaxTimesErr                = kAIRobinErrorCodeStarted + 5,
    kAIRobinNoErrorCodeInFailedResponseMsgErr       = kAIRobinErrorCodeStarted + 6,
    kAIRobinAckOfMessageNotReceivedErr              = kAIRobinErrorCodeStarted + 7,
    kAIRobinFallbackAllAsServiceInactiveErr         = kAIRobinErrorCodeStarted + 8,
    kAIRobinFallbackAllAsServiceNotRunningErr       = kAIRobinErrorCodeStarted + 9,
    kAIRobinCommandFallbackNotImplementedErr        = kAIRobinErrorCodeStarted + 10,
    kAIRobinCrashedWhileExecutingJobErr             = kAIRobinErrorCodeStarted + 11, // = 3160
    kAIRobinRealErrorConvertedToCancelErr           = kAIRobinErrorCodeStarted + 12,
    kAIRobinServiceStartTimedOutErr                 = kAIRobinErrorCodeStarted + 13,
    kAIRobinServiceFailedFullShadowDOMErr           = kAIRobinErrorCodeStarted + 14,
    kAIRobinServiceFailedIncrementalUpdateErr       = kAIRobinErrorCodeStarted + 15,
    kAIRobinServiceFailedPGFErr                     = kAIRobinErrorCodeStarted + 16,
    kAIRobinServiceFailedPDFErr                     = kAIRobinErrorCodeStarted + 17,
    kAIRobinServiceFailedPGForPDFErr                = kAIRobinErrorCodeStarted + 18,
    kAIRobinServiceFailedDefragErr                  = kAIRobinErrorCodeStarted + 19,
    kAIRobinServiceFailedSaveAlreadyFailedErr       = kAIRobinErrorCodeStarted + 20,
    kAIRobinServiceFailedSharedMemContextMergeErr   = kAIRobinErrorCodeStarted + 21,
    kAIRobinServiceFailedInvalidDocumentNameSpace   = kAIRobinErrorCodeStarted + 22,
    kAIRobinServiceFailedInvalidFFState             = kAIRobinErrorCodeStarted + 23,
	kAIRobinServiceExportFailedErr					= kAIRobinErrorCodeStarted + 24,
    kAIRobinServiceRaidenNotInitialized             = kAIRobinErrorCodeStarted + 25,

    kAIRobinErrorCodeEnded                          = kAICommonErrorRangeStart + 1200,

    // IllustratorDiagnosys specific errors
    kIllustratorDiagnosysErrorCodeStarted                    = kAICommonErrorRangeStart + 1300,
    kIllustratorDiagnosysNotLaunched                         = kIllustratorDiagnosysErrorCodeStarted + 1,
    kIllustratorDiagnosysErrorCodeEnded                      = kAICommonErrorRangeStart + 1400,
    
    
    // TransformFallback specific error codes
    kAITransformFallbackErrorCodeStarted                     = kAICommonErrorRangeStart + 1500,
    kAITransformFallbackBibError                             = kAITransformFallbackErrorCodeStarted + 1,
    kAITransformFallbackImageGenerationFailure               = kAITransformFallbackErrorCodeStarted + 2,
    kAITransformFallbackClusterSizeExceeded                  = kAITransformFallbackErrorCodeStarted + 3,
    kAITransformFallbackInvalidReferenceGraphic              = kAITransformFallbackErrorCodeStarted + 4,
    kAITransformFallbackBatchChildGraphic                    = kAITransformFallbackErrorCodeStarted + 5,
    kAITransformFallbackReferenceGraphicNotInTree            = kAITransformFallbackErrorCodeStarted + 6,
    kAITransformFallbackInvalidGraphics                      = kAITransformFallbackErrorCodeStarted + 7,
    kAITransformFallbackInvalidClusterGraphics               = kAITransformFallbackErrorCodeStarted + 8,
    kAITransformFallbackClusterSizeExceededDuringTranslate   = kAITransformFallbackErrorCodeStarted + 9,
    kAITransformFallbackClusterSizeExceededDuringClone       = kAITransformFallbackErrorCodeStarted + 10,
    kAITransformFallbackBibErrorDuringTranslate              = kAITransformFallbackErrorCodeStarted + 11,
    kAITransformFallbackBibErrorDuringClone                  = kAITransformFallbackErrorCodeStarted + 12,
    kAITransformFallbackImageGenerationFailureDuringTranslate = kAITransformFallbackErrorCodeStarted + 13,
    kAITransformFallbackImageGenerationFailureDuringClone     = kAITransformFallbackErrorCodeStarted + 14,
    kAITransformFallbackInvalidReferenceGraphicDuringTranslate = kAITransformFallbackErrorCodeStarted + 15,
    kAITransformFallbackInvalidReferenceGraphicDuringClone     = kAITransformFallbackErrorCodeStarted + 16,
    kAITransformFallbackBatchChildGraphicDuringTranslate        = kAITransformFallbackErrorCodeStarted + 17,
    kAITransformFallbackBatchChildGraphicDuringClone            = kAITransformFallbackErrorCodeStarted + 18,
    kAITransformFallbackReferenceGraphicNotInTreeDuringTranslate = kAITransformFallbackErrorCodeStarted + 19,
    kAITransformFallbackReferenceGraphicNotInTreeDuringClone     = kAITransformFallbackErrorCodeStarted + 20,
    kAITransformFallbackInvalidGraphicsDuringTranslate          = kAITransformFallbackErrorCodeStarted + 21,
    kAITransformFallbackInvalidGraphicsDuringClone               = kAITransformFallbackErrorCodeStarted + 22,
    kAITransformFallbackInvalidClusterGraphicsDuringTranslate   = kAITransformFallbackErrorCodeStarted + 23,
    kAITransformFallbackInvalidClusterGraphicsDuringClone       = kAITransformFallbackErrorCodeStarted + 24,
    kAITransformFallbackLiveEditTimeExceededDuringTranslate     = kAITransformFallbackErrorCodeStarted + 25,
    kAITransformFallbackLiveEditTimeExceededDuringClone          = kAITransformFallbackErrorCodeStarted + 26,
    kAITransformFallbackErrorCodeEnded                       = kAICommonErrorRangeStart + 1600,

    kAICommonErrorRangeEnd                          = 0x0000FFFF, //End of Error Range used by Illustrator plugins and core (app)

    //----------------------------------------------------------------------------------------------------------

    kAIThirdPartySDKErrorRangeStart                 = 0x00010000, //Start of Error range reserved for use by SDK
    kAIThirdPartySDKErrorRangeEnd                   = 0x0001FFFF, //End   of Error range reserved for use by SDK
};
