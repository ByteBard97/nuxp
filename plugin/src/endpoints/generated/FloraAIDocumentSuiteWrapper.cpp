#include "FloraAIDocumentSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIDocumentSuite* sDocument;

namespace Flora {
namespace AIDocumentSuite {

nlohmann::json GetDocumentFileSpecification(const nlohmann::json& params) {
    nlohmann::json response;

    // Output file path: file
    ai::FilePath file;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileSpecification(file);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileSpecification failed with error: " + std::to_string(err));
    }

    // Marshal output file path: file
    response["file"] = file.GetFullPath().as_UTF8();

    return response;
}

nlohmann::json GetDocumentFileSpecificationFromHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output file path: file
    ai::FilePath file;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileSpecificationFromHandle(document_val, file);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileSpecificationFromHandle failed with error: " + std::to_string(err));
    }

    // Marshal output file path: file
    response["file"] = file.GetFullPath().as_UTF8();

    return response;
}

nlohmann::json GetDocumentPageOrigin(const nlohmann::json& params) {
    nlohmann::json response;

    // Output struct: origin
    AIRealPoint origin{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentPageOrigin(&origin);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentPageOrigin failed with error: " + std::to_string(err));
    }

    // Marshal output struct: origin
    response["origin"] = {
        {"h", origin.h},
        {"v", origin.v}
    };

    return response;
}

nlohmann::json SetDocumentPageOrigin(const nlohmann::json& params) {
    nlohmann::json response;

    // Output struct: origin
    AIRealPoint origin{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentPageOrigin(&origin);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentPageOrigin failed with error: " + std::to_string(err));
    }

    // Marshal output struct: origin
    response["origin"] = {
        {"h", origin.h},
        {"v", origin.v}
    };

    return response;
}

nlohmann::json SetDocumentRulerOrigin(const nlohmann::json& params) {
    nlohmann::json response;

    // Output struct: origin
    AIRealPoint origin{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentRulerOrigin(&origin);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentRulerOrigin failed with error: " + std::to_string(err));
    }

    // Marshal output struct: origin
    response["origin"] = {
        {"h", origin.h},
        {"v", origin.v}
    };

    return response;
}

nlohmann::json GetDocumentRulerUnits(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: units
    ai::int16 units{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentRulerUnits(&units);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentRulerUnits failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: units
    response["units"] = units;

    return response;
}

nlohmann::json SetDocumentRulerUnits(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: units
    ai::int16 units = params["units"].get<int16_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentRulerUnits(units);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentRulerUnits failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentCropStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: cropStyle
    AICropMarkStyle cropStyle{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentCropStyle(&cropStyle);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentCropStyle failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: cropStyle

    return response;
}

nlohmann::json SetDocumentCropStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: cropStyle (AICropMarkStyle)
    // WARNING: Using default initialization
    AICropMarkStyle cropStyle{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentCropStyle(cropStyle);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentCropStyle failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentPrintRecord(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: print
    AIDocumentPlatformPrintRecord print{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentPrintRecord(&print);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentPrintRecord failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: print

    return response;
}

nlohmann::json SetDocumentPrintRecord(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: print (AIDocumentPlatformPrintRecord)
    // WARNING: Using default initialization
    AIDocumentPlatformPrintRecord print{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentPrintRecord(print);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentPrintRecord failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentSetup(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: setup
    AIDocumentSetup setup{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentSetup(&setup);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentSetup failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: setup

    return response;
}

nlohmann::json SetDocumentSetup(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: setup
    AIDocumentSetup setup{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentSetup(&setup);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentSetup failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: setup

    return response;
}

nlohmann::json GetDocumentModified(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: modified
    AIBoolean modified{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentModified(&modified);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentModified failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: modified
    response["modified"] = static_cast<bool>(modified);

    return response;
}

nlohmann::json SetDocumentModified(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: modified
    AIBoolean modified = params["modified"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentModified(modified);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentModified failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentFileFormat(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: fileFormat
    AIFileFormatHandle fileFormat{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileFormat(&fileFormat);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileFormat failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: fileFormat

    return response;
}

nlohmann::json SetDocumentFileFormat(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: fileFormat (AIFileFormatHandle)
    // WARNING: Using default initialization
    AIFileFormatHandle fileFormat{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentFileFormat(fileFormat);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentFileFormat failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetDocumentFileFormatParameters(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: parameters (AIDocumentFileFormatParameters)
    // WARNING: Using default initialization
    AIDocumentFileFormatParameters parameters{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentFileFormatParameters(parameters);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentFileFormatParameters failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocument(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: document
    AIDocumentHandle document = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocument(&document);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocument failed with error: " + std::to_string(err));
    }

    // Marshal output handle: document
    if (document) {
        response["document"] = HandleManager::documents.Register(document);
    } else {
        response["document"] = -1;
    }

    return response;
}

nlohmann::json WriteDocument(const nlohmann::json& params) {
    nlohmann::json response;

    // Input file path: file
    ai::FilePath file(ai::UnicodeString(params["file"].get<std::string>()));
    // Input string: fileFormatName
    std::string fileFormatName_str = params["fileFormatName"].get<std::string>();
    const char* fileFormatName = fileFormatName_str.c_str();
    // Input primitive: askForParms
    AIBoolean askForParms = params["askForParms"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->WriteDocument(file, fileFormatName, askForParms);
    if (err != kNoErr) {
        throw std::runtime_error("WriteDocument failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentMiPrintRecord(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: print (AIDocumentMiPrintRecordPtr)
    // WARNING: Using default initialization
    AIDocumentMiPrintRecordPtr print{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentMiPrintRecord(print);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentMiPrintRecord failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetDocumentMiPrintRecord(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: print (AIDocumentMiPrintRecordPtr)
    // WARNING: Using default initialization
    AIDocumentMiPrintRecordPtr print{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentMiPrintRecord(print);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentMiPrintRecord failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentRulerOrigin(const nlohmann::json& params) {
    nlohmann::json response;

    // Output struct: origin
    AIRealPoint origin{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentRulerOrigin(&origin);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentRulerOrigin failed with error: " + std::to_string(err));
    }

    // Marshal output struct: origin
    response["origin"] = {
        {"h", origin.h},
        {"v", origin.v}
    };

    return response;
}

nlohmann::json UpdateLinks(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: updatedSomething
    AIBoolean updatedSomething{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->UpdateLinks(&updatedSomething);
    if (err != kNoErr) {
        throw std::runtime_error("UpdateLinks failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: updatedSomething
    response["updatedSomething"] = static_cast<bool>(updatedSomething);

    return response;
}

nlohmann::json GetDocumentZoomLimit(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: min
    AIReal min{};
    // Output primitive: max
    AIReal max{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentZoomLimit(&min, &max);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentZoomLimit failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: min
    response["min"] = min;
    // Marshal output primitive: max
    response["max"] = max;

    return response;
}

nlohmann::json GetDocumentMaxArtboardBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentMaxArtboardBounds(&bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentMaxArtboardBounds failed with error: " + std::to_string(err));
    }

    // Marshal output struct: bounds
    response["bounds"] = {
        {"left", bounds.left},
        {"top", bounds.top},
        {"right", bounds.right},
        {"bottom", bounds.bottom}
    };

    return response;
}

nlohmann::json DocumentExists(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output primitive: exists
    AIBoolean exists{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DocumentExists(document_val, &exists);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentExists failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: exists
    response["exists"] = static_cast<bool>(exists);

    return response;
}

nlohmann::json GetDocumentColorModel(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: colorModel
    ai::int16 colorModel{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentColorModel(&colorModel);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentColorModel failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: colorModel
    response["colorModel"] = colorModel;

    return response;
}

nlohmann::json SetDocumentColorModel(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: colorModel
    ai::int16 colorModel = params["colorModel"].get<int16_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentColorModel(colorModel);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentColorModel failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Copy(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->Copy();
    if (err != kNoErr) {
        throw std::runtime_error("Copy failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Cut(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->Cut();
    if (err != kNoErr) {
        throw std::runtime_error("Cut failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Paste(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->Paste();
    if (err != kNoErr) {
        throw std::runtime_error("Paste failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentTargeting(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: paintTarget
    short paintTarget{};
    // Output primitive: transparencyTarget
    short transparencyTarget{};
    // Output primitive: effectsTarget
    short effectsTarget{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentTargeting(&paintTarget, &transparencyTarget, &effectsTarget);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentTargeting failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: paintTarget
    response["paintTarget"] = paintTarget;
    // Marshal output primitive: transparencyTarget
    response["transparencyTarget"] = transparencyTarget;
    // Marshal output primitive: effectsTarget
    response["effectsTarget"] = effectsTarget;

    return response;
}

nlohmann::json SetDocumentTargeting(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: paintTarget
    short paintTarget = params["paintTarget"].get<int16_t>();
    // Input primitive: transparencyTarget
    short transparencyTarget = params["transparencyTarget"].get<int16_t>();
    // Input primitive: effectsTarget
    short effectsTarget = params["effectsTarget"].get<int16_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentTargeting(paintTarget, transparencyTarget, effectsTarget);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentTargeting failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json DocumentHasTransparency(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: hasTransparency
    AIBoolean hasTransparency{};
    // Input primitive: detectOverprint
    AIBoolean detectOverprint = params["detectOverprint"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DocumentHasTransparency(&hasTransparency, detectOverprint);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentHasTransparency failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasTransparency
    response["hasTransparency"] = static_cast<bool>(hasTransparency);

    return response;
}

nlohmann::json DocumentHasSpotColorArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: hasSpotColorArt
    AIBoolean hasSpotColorArt{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DocumentHasSpotColorArt(&hasSpotColorArt);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentHasSpotColorArt failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasSpotColorArt
    response["hasSpotColorArt"] = static_cast<bool>(hasSpotColorArt);

    return response;
}

nlohmann::json SetDocumentAssetMgmtInfo(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: managed
    AIBoolean managed{};
    // Output primitive: checkedOut
    AIBoolean checkedOut{};
    // Input string: URL
    std::string URL_str = params["URL"].get<std::string>();
    const char* URL = URL_str.c_str();
    // Output primitive: canEdit
    AIBoolean canEdit{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentAssetMgmtInfo(&managed, &checkedOut, URL, &canEdit);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentAssetMgmtInfo failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: managed
    response["managed"] = static_cast<bool>(managed);
    // Marshal output primitive: checkedOut
    response["checkedOut"] = static_cast<bool>(checkedOut);
    // Marshal output primitive: canEdit
    response["canEdit"] = static_cast<bool>(canEdit);

    return response;
}

nlohmann::json SetDocumentXAP(const nlohmann::json& params) {
    nlohmann::json response;

    // Input string: xap
    std::string xap_str = params["xap"].get<std::string>();
    const char* xap = xap_str.c_str();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentXAP(xap);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentXAP failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SuspendTextReflow(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SuspendTextReflow();
    if (err != kNoErr) {
        throw std::runtime_error("SuspendTextReflow failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ResumeTextReflow(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->ResumeTextReflow();
    if (err != kNoErr) {
        throw std::runtime_error("ResumeTextReflow failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json HasTextFocus(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: focus
    AIBoolean focus{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->HasTextFocus(&focus);
    if (err != kNoErr) {
        throw std::runtime_error("HasTextFocus failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: focus
    response["focus"] = static_cast<bool>(focus);

    return response;
}

nlohmann::json HasTextCaret(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: caret
    AIBoolean caret{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->HasTextCaret(&caret);
    if (err != kNoErr) {
        throw std::runtime_error("HasTextCaret failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: caret
    response["caret"] = static_cast<bool>(caret);

    return response;
}

nlohmann::json GetTextFocus(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: pStory
    StoryRef pStory{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetTextFocus(&pStory);
    if (err != kNoErr) {
        throw std::runtime_error("GetTextFocus failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: pStory

    return response;
}

nlohmann::json SetTextFocus(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: story (StoryRef)
    // WARNING: Using default initialization
    StoryRef story{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetTextFocus(story);
    if (err != kNoErr) {
        throw std::runtime_error("SetTextFocus failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json LoseTextFocus(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->LoseTextFocus();
    if (err != kNoErr) {
        throw std::runtime_error("LoseTextFocus failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentTextResources(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: pDocResources
    DocumentTextResourcesRef pDocResources{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentTextResources(&pDocResources);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentTextResources failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: pDocResources

    return response;
}

nlohmann::json WriteDocumentMacInformationResource(const nlohmann::json& params) {
    nlohmann::json response;

    // Input file path: file
    ai::FilePath file(ai::UnicodeString(params["file"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->WriteDocumentMacInformationResource(file);
    if (err != kNoErr) {
        throw std::runtime_error("WriteDocumentMacInformationResource failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json WriteDocumentAsLibrary(const nlohmann::json& params) {
    nlohmann::json response;

    // Input file path: file
    ai::FilePath file(ai::UnicodeString(params["file"].get<std::string>()));
    // Unknown type: libraryType (AILibraryType)
    // WARNING: Using default initialization
    AILibraryType libraryType{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->WriteDocumentAsLibrary(file, libraryType);
    if (err != kNoErr) {
        throw std::runtime_error("WriteDocumentAsLibrary failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json DocumentHasOverprint(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: hasOverprint
    AIBoolean hasOverprint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DocumentHasOverprint(&hasOverprint);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentHasOverprint failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasOverprint
    response["hasOverprint"] = static_cast<bool>(hasOverprint);

    return response;
}

nlohmann::json DocumentHasManagedLinks(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output primitive: hasManagedLinks
    AIBoolean hasManagedLinks{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DocumentHasManagedLinks(document_val, &hasManagedLinks);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentHasManagedLinks failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasManagedLinks
    response["hasManagedLinks"] = static_cast<bool>(hasManagedLinks);

    return response;
}

nlohmann::json GetDocumentSpotColorMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output unknown: mode
    AISpotColorMode mode{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentSpotColorMode(document_val, &mode);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentSpotColorMode failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: mode

    return response;
}

nlohmann::json Undo(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->Undo();
    if (err != kNoErr) {
        throw std::runtime_error("Undo failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Redo(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->Redo();
    if (err != kNoErr) {
        throw std::runtime_error("Redo failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json DocumentRasterAttributes(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: hasDeviceNRasters
    AIBoolean hasDeviceNRasters{};
    // Output primitive: hasOverprint
    AIBoolean hasOverprint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DocumentRasterAttributes(&hasDeviceNRasters, &hasOverprint);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentRasterAttributes failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasDeviceNRasters
    response["hasDeviceNRasters"] = static_cast<bool>(hasDeviceNRasters);
    // Marshal output primitive: hasOverprint
    response["hasOverprint"] = static_cast<bool>(hasOverprint);

    return response;
}

nlohmann::json GetDocumentStartupProfile(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output unknown: startupProfile
    AIDocumentStartupProfile startupProfile{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentStartupProfile(document_val, &startupProfile);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentStartupProfile failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: startupProfile

    return response;
}

nlohmann::json GetDocumentBleeds(const nlohmann::json& params) {
    nlohmann::json response;

    // Output struct: bleedOffset
    AIRealRect bleedOffset{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentBleeds(&bleedOffset);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentBleeds failed with error: " + std::to_string(err));
    }

    // Marshal output struct: bleedOffset
    response["bleedOffset"] = {
        {"left", bleedOffset.left},
        {"top", bleedOffset.top},
        {"right", bleedOffset.right},
        {"bottom", bleedOffset.bottom}
    };

    return response;
}

nlohmann::json SetDocumentBleeds(const nlohmann::json& params) {
    nlohmann::json response;

    // Input struct: bleedOffset
    AIRealRect bleedOffset;
    bleedOffset.left = params["bleedOffset"]["left"].get<double>();
    bleedOffset.top = params["bleedOffset"]["top"].get<double>();
    bleedOffset.right = params["bleedOffset"]["right"].get<double>();
    bleedOffset.bottom = params["bleedOffset"]["bottom"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentBleeds(bleedOffset);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentBleeds failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetDocumentPixelPerfectStatus(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: isPixelPerfect
    AIBoolean isPixelPerfect = params["isPixelPerfect"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetDocumentPixelPerfectStatus(isPixelPerfect);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentPixelPerfectStatus failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json DeleteSelection(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocument->DeleteSelection();
    if (err != kNoErr) {
        throw std::runtime_error("DeleteSelection failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetAutoAssignUIDOnArtCreation(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: artType
    ai::int16 artType = params["artType"].get<int16_t>();
    // Input primitive: autoAssignUIDOnArtCreation
    AIBoolean autoAssignUIDOnArtCreation = params["autoAssignUIDOnArtCreation"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetAutoAssignUIDOnArtCreation(artType, autoAssignUIDOnArtCreation);
    if (err != kNoErr) {
        throw std::runtime_error("SetAutoAssignUIDOnArtCreation failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetAutoAssignUIDOnArtCreation(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: artType
    ai::int16 artType = params["artType"].get<int16_t>();
    // Output primitive: outAutoAssignUIDOnArtCreation
    AIBoolean outAutoAssignUIDOnArtCreation{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetAutoAssignUIDOnArtCreation(artType, outAutoAssignUIDOnArtCreation);
    if (err != kNoErr) {
        throw std::runtime_error("GetAutoAssignUIDOnArtCreation failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: outAutoAssignUIDOnArtCreation
    response["outAutoAssignUIDOnArtCreation"] = static_cast<bool>(outAutoAssignUIDOnArtCreation);

    return response;
}

nlohmann::json GetDocumentScale(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: docScale
    AIReal docScale{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentScale(docScale);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentScale failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: docScale
    response["docScale"] = docScale;

    return response;
}

nlohmann::json GetDocumentFileName(const nlohmann::json& params) {
    nlohmann::json response;

    // Output string: fileName
    ai::UnicodeString fileName;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileName(fileName);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileName failed with error: " + std::to_string(err));
    }

    // Marshal output string: fileName
    response["fileName"] = fileName.as_UTF8();

    return response;
}

nlohmann::json GetDocumentFileNameNoExt(const nlohmann::json& params) {
    nlohmann::json response;

    // Output string: fileName
    ai::UnicodeString fileName;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileNameNoExt(fileName);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileNameNoExt failed with error: " + std::to_string(err));
    }

    // Marshal output string: fileName
    response["fileName"] = fileName.as_UTF8();

    return response;
}

nlohmann::json GetDocumentFileNameFromHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output string: fileName
    ai::UnicodeString fileName;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileNameFromHandle(document_val, fileName);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileNameFromHandle failed with error: " + std::to_string(err));
    }

    // Marshal output string: fileName
    response["fileName"] = fileName.as_UTF8();

    return response;
}

nlohmann::json GetDocumentFileNameNoExtFromHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: document
    AIDocumentHandle document_val = HandleManager::documents.Get(params["document"].get<int32_t>());
    if (!document_val) {
        throw std::runtime_error("Invalid AIDocumentHandle handle for parameter 'document'");
    }
    // Output string: fileName
    ai::UnicodeString fileName;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetDocumentFileNameNoExtFromHandle(document_val, fileName);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentFileNameNoExtFromHandle failed with error: " + std::to_string(err));
    }

    // Marshal output string: fileName
    response["fileName"] = fileName.as_UTF8();

    return response;
}

nlohmann::json GetLastExportedFilePath(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: option (const AIExportTriggeredFrom)
    // WARNING: Using default initialization
    const AIExportTriggeredFrom option{};
    // Output file path: file
    ai::FilePath file;

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->GetLastExportedFilePath(option, file);
    if (err != kNoErr) {
        throw std::runtime_error("GetLastExportedFilePath failed with error: " + std::to_string(err));
    }

    // Marshal output file path: file
    response["file"] = file.GetFullPath().as_UTF8();

    return response;
}

nlohmann::json SetLastExportedFilePath(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: option (const AIExportTriggeredFrom)
    // WARNING: Using default initialization
    const AIExportTriggeredFrom option{};
    // Input file path: file
    ai::FilePath file(ai::UnicodeString(params["file"].get<std::string>()));

    // Call SDK function (returns AIErr)
    AIErr err = sDocument->SetLastExportedFilePath(option, file);
    if (err != kNoErr) {
        throw std::runtime_error("SetLastExportedFilePath failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetDocumentFileSpecification") {
        return GetDocumentFileSpecification(params);
    } else if (method == "GetDocumentFileSpecificationFromHandle") {
        return GetDocumentFileSpecificationFromHandle(params);
    } else if (method == "GetDocumentPageOrigin") {
        return GetDocumentPageOrigin(params);
    } else if (method == "SetDocumentPageOrigin") {
        return SetDocumentPageOrigin(params);
    } else if (method == "SetDocumentRulerOrigin") {
        return SetDocumentRulerOrigin(params);
    } else if (method == "GetDocumentRulerUnits") {
        return GetDocumentRulerUnits(params);
    } else if (method == "SetDocumentRulerUnits") {
        return SetDocumentRulerUnits(params);
    } else if (method == "GetDocumentCropStyle") {
        return GetDocumentCropStyle(params);
    } else if (method == "SetDocumentCropStyle") {
        return SetDocumentCropStyle(params);
    } else if (method == "GetDocumentPrintRecord") {
        return GetDocumentPrintRecord(params);
    } else if (method == "SetDocumentPrintRecord") {
        return SetDocumentPrintRecord(params);
    } else if (method == "GetDocumentSetup") {
        return GetDocumentSetup(params);
    } else if (method == "SetDocumentSetup") {
        return SetDocumentSetup(params);
    } else if (method == "GetDocumentModified") {
        return GetDocumentModified(params);
    } else if (method == "SetDocumentModified") {
        return SetDocumentModified(params);
    } else if (method == "GetDocumentFileFormat") {
        return GetDocumentFileFormat(params);
    } else if (method == "SetDocumentFileFormat") {
        return SetDocumentFileFormat(params);
    } else if (method == "SetDocumentFileFormatParameters") {
        return SetDocumentFileFormatParameters(params);
    } else if (method == "GetDocument") {
        return GetDocument(params);
    } else if (method == "WriteDocument") {
        return WriteDocument(params);
    } else if (method == "GetDocumentMiPrintRecord") {
        return GetDocumentMiPrintRecord(params);
    } else if (method == "SetDocumentMiPrintRecord") {
        return SetDocumentMiPrintRecord(params);
    } else if (method == "GetDocumentRulerOrigin") {
        return GetDocumentRulerOrigin(params);
    } else if (method == "UpdateLinks") {
        return UpdateLinks(params);
    } else if (method == "GetDocumentZoomLimit") {
        return GetDocumentZoomLimit(params);
    } else if (method == "GetDocumentMaxArtboardBounds") {
        return GetDocumentMaxArtboardBounds(params);
    } else if (method == "DocumentExists") {
        return DocumentExists(params);
    } else if (method == "GetDocumentColorModel") {
        return GetDocumentColorModel(params);
    } else if (method == "SetDocumentColorModel") {
        return SetDocumentColorModel(params);
    } else if (method == "Copy") {
        return Copy(params);
    } else if (method == "Cut") {
        return Cut(params);
    } else if (method == "Paste") {
        return Paste(params);
    } else if (method == "GetDocumentTargeting") {
        return GetDocumentTargeting(params);
    } else if (method == "SetDocumentTargeting") {
        return SetDocumentTargeting(params);
    } else if (method == "DocumentHasTransparency") {
        return DocumentHasTransparency(params);
    } else if (method == "DocumentHasSpotColorArt") {
        return DocumentHasSpotColorArt(params);
    } else if (method == "SetDocumentAssetMgmtInfo") {
        return SetDocumentAssetMgmtInfo(params);
    } else if (method == "SetDocumentXAP") {
        return SetDocumentXAP(params);
    } else if (method == "SuspendTextReflow") {
        return SuspendTextReflow(params);
    } else if (method == "ResumeTextReflow") {
        return ResumeTextReflow(params);
    } else if (method == "HasTextFocus") {
        return HasTextFocus(params);
    } else if (method == "HasTextCaret") {
        return HasTextCaret(params);
    } else if (method == "GetTextFocus") {
        return GetTextFocus(params);
    } else if (method == "SetTextFocus") {
        return SetTextFocus(params);
    } else if (method == "LoseTextFocus") {
        return LoseTextFocus(params);
    } else if (method == "GetDocumentTextResources") {
        return GetDocumentTextResources(params);
    } else if (method == "WriteDocumentMacInformationResource") {
        return WriteDocumentMacInformationResource(params);
    } else if (method == "WriteDocumentAsLibrary") {
        return WriteDocumentAsLibrary(params);
    } else if (method == "DocumentHasOverprint") {
        return DocumentHasOverprint(params);
    } else if (method == "DocumentHasManagedLinks") {
        return DocumentHasManagedLinks(params);
    } else if (method == "GetDocumentSpotColorMode") {
        return GetDocumentSpotColorMode(params);
    } else if (method == "Undo") {
        return Undo(params);
    } else if (method == "Redo") {
        return Redo(params);
    } else if (method == "DocumentRasterAttributes") {
        return DocumentRasterAttributes(params);
    } else if (method == "GetDocumentStartupProfile") {
        return GetDocumentStartupProfile(params);
    } else if (method == "GetDocumentBleeds") {
        return GetDocumentBleeds(params);
    } else if (method == "SetDocumentBleeds") {
        return SetDocumentBleeds(params);
    } else if (method == "SetDocumentPixelPerfectStatus") {
        return SetDocumentPixelPerfectStatus(params);
    } else if (method == "DeleteSelection") {
        return DeleteSelection(params);
    } else if (method == "SetAutoAssignUIDOnArtCreation") {
        return SetAutoAssignUIDOnArtCreation(params);
    } else if (method == "GetAutoAssignUIDOnArtCreation") {
        return GetAutoAssignUIDOnArtCreation(params);
    } else if (method == "GetDocumentScale") {
        return GetDocumentScale(params);
    } else if (method == "GetDocumentFileName") {
        return GetDocumentFileName(params);
    } else if (method == "GetDocumentFileNameNoExt") {
        return GetDocumentFileNameNoExt(params);
    } else if (method == "GetDocumentFileNameFromHandle") {
        return GetDocumentFileNameFromHandle(params);
    } else if (method == "GetDocumentFileNameNoExtFromHandle") {
        return GetDocumentFileNameNoExtFromHandle(params);
    } else if (method == "GetLastExportedFilePath") {
        return GetLastExportedFilePath(params);
    } else if (method == "SetLastExportedFilePath") {
        return SetLastExportedFilePath(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIDocumentSuite");
}

} // namespace AIDocumentSuite
} // namespace Flora
