#include "Errors.hpp"
#include "AIErrorCodes.h"
#include <cstdio>

// Stub for SDK assertion function (replaces AIAssert.cpp which requires Objective-C)
extern "C" void ShowPlatformAssert(const bool success, const char* message) {
    if (!success) {
        std::fprintf(stderr, "NUXP Assert Failed: %s\n", message ? message : "(no message)");
    }
}

namespace Errors {

std::string AIErrToString(AIErr error) {
    // Check for no error first
    if (error == kNoErr) {
        return "No error";
    }

    // Standard SDK error codes (from ASTypes.h / SPErrorCodes.h)
    switch (error) {
        // Common error codes from AITypes.h
        case kCanceledErr:
            return "Operation canceled by user";
        case kNoDocumentErr:
            return "No current document";
        case kSelectorClashErr:
            return "Selector called at inappropriate time";
        case kNameNotFoundErr:
            return "Specified name not found";
        case kNameInUseErr:
            return "Specified name not unique";
        case kInvalidNameErr:
            return "Specified name not valid";
        case kNameTooLongErr:
            return "Specified name too long";
        case kUndoRedoErr:
            return "Undo/Redo cannot be done properly";
        case kAIInvalidArtBoundsErr:
            return "Art bounds are invalid";
        case kAIResourcePermissionErr:
            return "Resource permission error";
        case kImageTooBigError:
            return "Image too big";
        default:
            break;
    }

    // Standard PICA/SP error codes
    switch (error) {
        case kSPNoError:
            return "No error";
        case kSPUnimplementedError:
            return "Not implemented";
        case kSPUserCanceledError:
            return "User canceled";
        case kSPOutOfMemoryError:
            return "Out of memory";
        case kSPBadParameterError:
            return "Bad parameter";
        case kSPPluginNotFound:
            return "Plugin not found";
        case kSPPluginCachesFlushResponse:
            return "Plugin caches flush response";
        case kSPSuiteNotFoundError:
            return "Suite not found";
        default:
            break;
    }

    // Illustrator-specific error codes from AIErrorCodes.h
    if (error >= kAICommonErrorRangeStart && error <= kAICommonErrorRangeEnd) {
        switch (static_cast<AIErrorCode>(error)) {
            case kAICoreCanceledError:
                return "Core operation canceled";
            case kAIAlreadyExportingSameDocument:
                return "Already exporting same document";
            case kAIOriginalNetworkPathDoesNotExist:
                return "Original network path does not exist";
            case kAIScratchFolderNotAvailable:
                return "Scratch folder not available";
            case kAISufficientScratchDiskSpaceNotAvailable:
                return "Insufficient scratch disk space";
            case kAIMultipleArtboardInASingleArtboardWorkflow:
                return "Multiple artboards not supported in this workflow";
            case kAIPreConditionNotMet:
                return "Precondition not met";
            case kAIFeatureNotOptedIn:
                return "Feature not opted in";
            case kAIFeatureNotEnabled:
                return "Feature not enabled";
            case kAIFileCopyToNetworkLocationFailed:
                return "File copy to network location failed";
            case kAIJSONParsingFailed:
                return "JSON parsing failed";
            case kAIVerificationFailedErr:
                return "Document verification failed";
            case kAIFileReadError:
                return "File read error";
            case kAIFileRenameError:
                return "File rename error";
            case kAICorruptLinkErr:
                return "Corrupt link error";
            case kAICantCutAllArtboardsErr:
                return "Cannot cut all artboards";
            case kAINoSpaceToPasteArtboardErr:
                return "No space to paste artboard";
            case kAIHTTPErr:
                return "HTTP error";
            case kAIDXFDWGSDKErr:
                return "DXF/DWG SDK error";
            case kAILiveEditTimeExceededErr:
                return "Live edit time exceeded";
            case kAIJsonValueNotAMapErr:
                return "JSON value is not a map";
            case kAIFileNotFoundErr:
                return "File not found";
            case kAINotACloudDocumentErr:
                return "Not a cloud document";
            case kAINotALocalDocumentErr:
                return "Not a local document";
            case kAINotOnMainThreadError:
                return "Not on main thread";
            case kAINoFeatureFound:
                return "Feature not found";
            case kAISignatureNotValid:
                return "Signature not valid";
            case kAIUserNotRegisteredWithCloudErr:
                return "User not registered with cloud";
            case kAIFileDeleteError:
                return "File delete error";
            case kAIDirectoryDeleteError:
                return "Directory delete error";
            case kAIFastExportFailedErr:
                return "Fast export failed";
            case kAIFileFormatNotFoundErr:
                return "File format not found";
            case kAITaskCantBeScheduledErr:
                return "Task cannot be scheduled";
            case kAILiveEffectNotFoundErr:
                return "Live effect not found";
            case kAIUserNotLoggedInToCCErr:
                return "User not logged in to Creative Cloud";
            case kAILiveEffectParamsNotFoundErr:
                return "Live effect parameters not found";
            case kAILiveEffectVisibilityHiddenErr:
                return "Live effect visibility hidden";
            case kAIFileCopyErr:
                return "File copy error";
            case kAIUnsupportedSkiaFeatureErr:
                return "Unsupported Skia feature";
            case kAIUnsupportedAGMGPUFeatureErr:
                return "Unsupported AGM GPU feature";
            case kAISkipEffectExecutionErr:
                return "Skip effect execution";
            case kAICodecNotAvailableErr:
                return "Codec not available";
            case kAIPDFFormatUnknownErr:
                return "Unknown PDF format";
            case kAICantErr:
                return "Operation cannot be performed";
            case kAINotEnoughRAMErr:
                return "Not enough RAM";
            case kAIBrokenJPEGErr:
                return "Broken JPEG";
            case kAIPluginLoadingErr:
                return "Plugin loading error";
            case kAIFileReadWriteErr:
                return "File read/write error";
            case kAIInvalidObjectsIgnoredErr:
                return "Invalid objects ignored";
            case kAISufficientDiskSpaceNotAvailable:
                return "Insufficient disk space";
            case kAITextResourceLoadingFailed:
                return "Text resource loading failed";
            case kAINotEntitledErr:
                return "Not entitled";
            case kAIInvalidVMStreamErr:
                return "Invalid VM stream";
            case kAINotFoundError:
                return "Not found";
            case kEmptyArtBoundsErr:
                return "Empty art bounds";
            case kAINeedUserInputErr:
                return "User input needed";
            case kInvalidSelectionErr:
                return "Invalid selection";
            case kAIUnsupportedTypeErr:
                return "Unsupported type";
            default:
                break;
        }
    }

    // Return a generic error message with the code for unknown errors
    return "Unknown error: " + std::to_string(error);
}

bool IsSuccess(AIErr error) {
    return error == kNoErr;
}

} // namespace Errors
