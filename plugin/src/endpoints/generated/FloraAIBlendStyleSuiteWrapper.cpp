#include "FloraAIBlendStyleSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIBlendStyleSuite* sBlendStyle;

namespace Flora {
namespace AIBlendStyleSuite {

nlohmann::json GetBlendingMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns primitive)
    AIBlendingMode result = sBlendStyle->GetBlendingMode(art_val);
    response["result"] = result;


    return response;
}

nlohmann::json GetOpacity(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns primitive)
    AIReal result = sBlendStyle->GetOpacity(art_val);
    response["result"] = result;


    return response;
}

nlohmann::json SetOpacity(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: opacity
    AIReal opacity = params["opacity"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetOpacity(art_val, opacity);
    if (err != kNoErr) {
        throw std::runtime_error("SetOpacity failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetIsolated(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sBlendStyle->GetIsolated(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetIsolated(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: isolated
    AIBoolean isolated = params["isolated"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetIsolated(art_val, isolated);
    if (err != kNoErr) {
        throw std::runtime_error("SetIsolated failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetKnockout(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns primitive)
    AIKnockout result = sBlendStyle->GetKnockout(art_val);
    response["result"] = result;


    return response;
}

nlohmann::json GetInheritedKnockout(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns primitive)
    AIKnockout result = sBlendStyle->GetInheritedKnockout(art_val);
    response["result"] = result;


    return response;
}

nlohmann::json GetAlphaIsShape(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sBlendStyle->GetAlphaIsShape(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetAlphaIsShape(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: alphaIsShape
    AIBoolean alphaIsShape = params["alphaIsShape"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetAlphaIsShape(art_val, alphaIsShape);
    if (err != kNoErr) {
        throw std::runtime_error("SetAlphaIsShape failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Copy(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: source
    AIArtHandle source_val = HandleManager::art.Get(params["source"].get<int32_t>());
    if (!source_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'source'");
    }
    // Input handle: destination
    AIArtHandle destination_val = HandleManager::art.Get(params["destination"].get<int32_t>());
    if (!destination_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'destination'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->Copy(source_val, destination_val);
    if (err != kNoErr) {
        throw std::runtime_error("Copy failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->GetArtAttrs(art_val, attrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtAttrs failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetArtAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetArtAttrs(art_val, attrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtAttrs failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetStyleAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: style
    AIArtStyleHandle style_val = HandleManager::artStyles.Get(params["style"].get<int32_t>());
    if (!style_val) {
        throw std::runtime_error("Invalid AIArtStyleHandle handle for parameter 'style'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->GetStyleAttrs(style_val, attrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("GetStyleAttrs failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetStyleAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: style
    AIArtStyleHandle style_val = HandleManager::artStyles.Get(params["style"].get<int32_t>());
    if (!style_val) {
        throw std::runtime_error("Invalid AIArtStyleHandle handle for parameter 'style'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }
    // Output handle: newStyle
    AIArtStyleHandle newStyle = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetStyleAttrs(style_val, attrs_val, &newStyle);
    if (err != kNoErr) {
        throw std::runtime_error("SetStyleAttrs failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newStyle
    if (newStyle) {
        response["newStyle"] = HandleManager::artStyles.Register(newStyle);
    } else {
        response["newStyle"] = -1;
    }

    return response;
}

nlohmann::json GetCurrentTransparency(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: styleAttrs
    AIDictionaryRef styleAttrs_val = HandleManager::dictionaries.Get(params["styleAttrs"].get<int32_t>());
    if (!styleAttrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'styleAttrs'");
    }
    // Input handle: fillAttrs
    AIDictionaryRef fillAttrs_val = HandleManager::dictionaries.Get(params["fillAttrs"].get<int32_t>());
    if (!fillAttrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'fillAttrs'");
    }
    // Input handle: strokeAttrs
    AIDictionaryRef strokeAttrs_val = HandleManager::dictionaries.Get(params["strokeAttrs"].get<int32_t>());
    if (!strokeAttrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'strokeAttrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->GetCurrentTransparency(styleAttrs_val, fillAttrs_val, strokeAttrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("GetCurrentTransparency failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetCurrentTransparency(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: styleAttrs
    AIDictionaryRef styleAttrs_val = HandleManager::dictionaries.Get(params["styleAttrs"].get<int32_t>());
    if (!styleAttrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'styleAttrs'");
    }
    // Input handle: fillAttrs
    AIDictionaryRef fillAttrs_val = HandleManager::dictionaries.Get(params["fillAttrs"].get<int32_t>());
    if (!fillAttrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'fillAttrs'");
    }
    // Input handle: strokeAttrs
    AIDictionaryRef strokeAttrs_val = HandleManager::dictionaries.Get(params["strokeAttrs"].get<int32_t>());
    if (!strokeAttrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'strokeAttrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetCurrentTransparency(styleAttrs_val, fillAttrs_val, strokeAttrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetCurrentTransparency failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetFocalFillAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: artStyle
    AIArtStyleHandle artStyle_val = HandleManager::artStyles.Get(params["artStyle"].get<int32_t>());
    if (!artStyle_val) {
        throw std::runtime_error("Invalid AIArtStyleHandle handle for parameter 'artStyle'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->GetFocalFillAttrs(artStyle_val, attrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("GetFocalFillAttrs failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetFocalStrokeAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: artStyle
    AIArtStyleHandle artStyle_val = HandleManager::artStyles.Get(params["artStyle"].get<int32_t>());
    if (!artStyle_val) {
        throw std::runtime_error("Invalid AIArtStyleHandle handle for parameter 'artStyle'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->GetFocalStrokeAttrs(artStyle_val, attrs_val);
    if (err != kNoErr) {
        throw std::runtime_error("GetFocalStrokeAttrs failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetFocalFillAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: artStyle
    AIArtStyleHandle artStyle_val = HandleManager::artStyles.Get(params["artStyle"].get<int32_t>());
    if (!artStyle_val) {
        throw std::runtime_error("Invalid AIArtStyleHandle handle for parameter 'artStyle'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }
    // Output handle: newStyle
    AIArtStyleHandle newStyle = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetFocalFillAttrs(artStyle_val, attrs_val, &newStyle);
    if (err != kNoErr) {
        throw std::runtime_error("SetFocalFillAttrs failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newStyle
    if (newStyle) {
        response["newStyle"] = HandleManager::artStyles.Register(newStyle);
    } else {
        response["newStyle"] = -1;
    }

    return response;
}

nlohmann::json SetFocalStrokeAttrs(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: artStyle
    AIArtStyleHandle artStyle_val = HandleManager::artStyles.Get(params["artStyle"].get<int32_t>());
    if (!artStyle_val) {
        throw std::runtime_error("Invalid AIArtStyleHandle handle for parameter 'artStyle'");
    }
    // Input handle: attrs
    AIDictionaryRef attrs_val = HandleManager::dictionaries.Get(params["attrs"].get<int32_t>());
    if (!attrs_val) {
        throw std::runtime_error("Invalid AIDictionaryRef handle for parameter 'attrs'");
    }
    // Output handle: newStyle
    AIArtStyleHandle newStyle = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetFocalStrokeAttrs(artStyle_val, attrs_val, &newStyle);
    if (err != kNoErr) {
        throw std::runtime_error("SetFocalStrokeAttrs failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newStyle
    if (newStyle) {
        response["newStyle"] = HandleManager::artStyles.Register(newStyle);
    } else {
        response["newStyle"] = -1;
    }

    return response;
}

nlohmann::json ContainsNonIsolatedBlending(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: object
    AIArtHandle object_val = HandleManager::art.Get(params["object"].get<int32_t>());
    if (!object_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'object'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sBlendStyle->ContainsNonIsolatedBlending(object_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json GetDocumentIsolated(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns boolean)
    AIBoolean result = sBlendStyle->GetDocumentIsolated();
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetDocumentIsolated(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: isolated
    AIBoolean isolated = params["isolated"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetDocumentIsolated(isolated);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentIsolated failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentKnockout(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns boolean)
    AIBoolean result = sBlendStyle->GetDocumentKnockout();
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetDocumentKnockout(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: knockout
    AIBoolean knockout = params["knockout"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sBlendStyle->SetDocumentKnockout(knockout);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentKnockout failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetBlendingMode") {
        return GetBlendingMode(params);
    } else if (method == "GetOpacity") {
        return GetOpacity(params);
    } else if (method == "SetOpacity") {
        return SetOpacity(params);
    } else if (method == "GetIsolated") {
        return GetIsolated(params);
    } else if (method == "SetIsolated") {
        return SetIsolated(params);
    } else if (method == "GetKnockout") {
        return GetKnockout(params);
    } else if (method == "GetInheritedKnockout") {
        return GetInheritedKnockout(params);
    } else if (method == "GetAlphaIsShape") {
        return GetAlphaIsShape(params);
    } else if (method == "SetAlphaIsShape") {
        return SetAlphaIsShape(params);
    } else if (method == "Copy") {
        return Copy(params);
    } else if (method == "GetArtAttrs") {
        return GetArtAttrs(params);
    } else if (method == "SetArtAttrs") {
        return SetArtAttrs(params);
    } else if (method == "GetStyleAttrs") {
        return GetStyleAttrs(params);
    } else if (method == "SetStyleAttrs") {
        return SetStyleAttrs(params);
    } else if (method == "GetCurrentTransparency") {
        return GetCurrentTransparency(params);
    } else if (method == "SetCurrentTransparency") {
        return SetCurrentTransparency(params);
    } else if (method == "GetFocalFillAttrs") {
        return GetFocalFillAttrs(params);
    } else if (method == "GetFocalStrokeAttrs") {
        return GetFocalStrokeAttrs(params);
    } else if (method == "SetFocalFillAttrs") {
        return SetFocalFillAttrs(params);
    } else if (method == "SetFocalStrokeAttrs") {
        return SetFocalStrokeAttrs(params);
    } else if (method == "ContainsNonIsolatedBlending") {
        return ContainsNonIsolatedBlending(params);
    } else if (method == "GetDocumentIsolated") {
        return GetDocumentIsolated(params);
    } else if (method == "SetDocumentIsolated") {
        return SetDocumentIsolated(params);
    } else if (method == "GetDocumentKnockout") {
        return GetDocumentKnockout(params);
    } else if (method == "SetDocumentKnockout") {
        return SetDocumentKnockout(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIBlendStyleSuite");
}

} // namespace AIBlendStyleSuite
} // namespace Flora
