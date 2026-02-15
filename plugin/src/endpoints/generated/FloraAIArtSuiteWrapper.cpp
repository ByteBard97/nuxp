#include "FloraAIArtSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIArtSuite* sArt;

namespace Flora {
namespace AIArtSuite {

nlohmann::json NewArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: type
    ai::int16 type = params["type"].get<int16_t>();
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }
    // Output handle: newArt
    AIArtHandle newArt = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->NewArt(type, paintOrder, prep_val, &newArt);
    if (err != kNoErr) {
        throw std::runtime_error("NewArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newArt
    if (newArt) {
        response["newArt"] = HandleManager::art.Register(newArt);
    } else {
        response["newArt"] = -1;
    }

    return response;
}

nlohmann::json DisposeArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->DisposeArt(art_val);
    if (err != kNoErr) {
        throw std::runtime_error("DisposeArt failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ReorderArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: thisArt
    AIArtHandle thisArt_val = HandleManager::art.Get(params["thisArt"].get<int32_t>());
    if (!thisArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'thisArt'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ReorderArt(thisArt_val, paintOrder, prep_val);
    if (err != kNoErr) {
        throw std::runtime_error("ReorderArt failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json DuplicateArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: thisArt
    AIArtHandle thisArt_val = HandleManager::art.Get(params["thisArt"].get<int32_t>());
    if (!thisArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'thisArt'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }
    // Output handle: newArt
    AIArtHandle newArt = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->DuplicateArt(thisArt_val, paintOrder, prep_val, &newArt);
    if (err != kNoErr) {
        throw std::runtime_error("DuplicateArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newArt
    if (newArt) {
        response["newArt"] = HandleManager::art.Register(newArt);
    } else {
        response["newArt"] = -1;
    }

    return response;
}

nlohmann::json GetFirstArtOfLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetFirstArtOfLayer(layer_val, &art);
    if (err != kNoErr) {
        throw std::runtime_error("GetFirstArtOfLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json GetLayerOfArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetLayerOfArt(art_val, &layer);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerOfArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json GetArtType(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: type
    short type{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtType(art_val, &type);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtType failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: type
    response["type"] = type;

    return response;
}

nlohmann::json GetArtUserAttr(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: whichAttr
    ai::int32 whichAttr = params["whichAttr"].get<int32_t>();
    // Output primitive: attr
    ai::int32 attr{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtUserAttr(art_val, whichAttr, &attr);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtUserAttr failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: attr
    response["attr"] = attr;

    return response;
}

nlohmann::json SetArtUserAttr(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: whichAttr
    ai::int32 whichAttr = params["whichAttr"].get<int32_t>();
    // Input primitive: attr
    ai::int32 attr = params["attr"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetArtUserAttr(art_val, whichAttr, attr);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtUserAttr failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtParent(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: parent
    AIArtHandle parent = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtParent(art_val, &parent);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtParent failed with error: " + std::to_string(err));
    }

    // Marshal output handle: parent
    if (parent) {
        response["parent"] = HandleManager::art.Register(parent);
    } else {
        response["parent"] = -1;
    }

    return response;
}

nlohmann::json GetArtFirstChild(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: child
    AIArtHandle child = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtFirstChild(art_val, &child);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtFirstChild failed with error: " + std::to_string(err));
    }

    // Marshal output handle: child
    if (child) {
        response["child"] = HandleManager::art.Register(child);
    } else {
        response["child"] = -1;
    }

    return response;
}

nlohmann::json GetArtSibling(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: sibling
    AIArtHandle sibling = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtSibling(art_val, &sibling);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtSibling failed with error: " + std::to_string(err));
    }

    // Marshal output handle: sibling
    if (sibling) {
        response["sibling"] = HandleManager::art.Register(sibling);
    } else {
        response["sibling"] = -1;
    }

    return response;
}

nlohmann::json GetArtPriorSibling(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: sibling
    AIArtHandle sibling = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtPriorSibling(art_val, &sibling);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtPriorSibling failed with error: " + std::to_string(err));
    }

    // Marshal output handle: sibling
    if (sibling) {
        response["sibling"] = HandleManager::art.Register(sibling);
    } else {
        response["sibling"] = -1;
    }

    return response;
}

nlohmann::json GetArtBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtBounds(art_val, &bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtBounds failed with error: " + std::to_string(err));
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

nlohmann::json SetArtBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetArtBounds(art_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtBounds failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtCenterPointVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: visible
    AIBoolean visible{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtCenterPointVisible(art_val, &visible);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtCenterPointVisible failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: visible
    response["visible"] = static_cast<bool>(visible);

    return response;
}

nlohmann::json SetArtCenterPointVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: visible
    AIBoolean visible = params["visible"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetArtCenterPointVisible(art_val, visible);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtCenterPointVisible failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtTransformBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output struct: transform
    AIRealMatrix transform{};
    // Input primitive: flags
    ai::int32 flags = params["flags"].get<int32_t>();
    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtTransformBounds(art_val, &transform, flags, &bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtTransformBounds failed with error: " + std::to_string(err));
    }

    // Marshal output struct: transform
    response["transform"] = {
        {"a", transform.a},
        {"b", transform.b},
        {"c", transform.c},
        {"d", transform.d},
        {"tx", transform.tx},
        {"ty", transform.ty}
    };
    // Marshal output struct: bounds
    response["bounds"] = {
        {"left", bounds.left},
        {"top", bounds.top},
        {"right", bounds.right},
        {"bottom", bounds.bottom}
    };

    return response;
}

nlohmann::json UpdateArtworkLink(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: force
    AIBoolean force = params["force"].get<bool>();
    // Output primitive: updated
    AIBoolean updated{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->UpdateArtworkLink(art_val, force, &updated);
    if (err != kNoErr) {
        throw std::runtime_error("UpdateArtworkLink failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: updated
    response["updated"] = static_cast<bool>(updated);

    return response;
}

nlohmann::json ValidArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: searchAllLayerLists
    AIBoolean searchAllLayerLists = params["searchAllLayerLists"].get<bool>();

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->ValidArt(art_val, searchAllLayerLists);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json GetArtOrder(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art1
    AIArtHandle art1_val = HandleManager::art.Get(params["art1"].get<int32_t>());
    if (!art1_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art1'");
    }
    // Input handle: art2
    AIArtHandle art2_val = HandleManager::art.Get(params["art2"].get<int32_t>());
    if (!art2_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art2'");
    }
    // Output primitive: order
    short order{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtOrder(art1_val, art2_val, &order);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtOrder failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: order
    response["order"] = order;

    return response;
}

nlohmann::json SelectNamedArtOfLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input string: name
    ai::UnicodeString name(params["name"].get<std::string>());
    // Input primitive: matchWholeWord
    AIBoolean matchWholeWord = params["matchWholeWord"].get<bool>();
    // Input primitive: caseSensitive
    AIBoolean caseSensitive = params["caseSensitive"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SelectNamedArtOfLayer(layer_val, name, matchWholeWord, caseSensitive);
    if (err != kNoErr) {
        throw std::runtime_error("SelectNamedArtOfLayer failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtRotatedBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: angle
    AIReal angle = params["angle"].get<double>();
    // Input primitive: flags
    ai::int32 flags = params["flags"].get<int32_t>();
    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtRotatedBounds(art_val, angle, flags, &bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtRotatedBounds failed with error: " + std::to_string(err));
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

nlohmann::json ArtHasFill(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->ArtHasFill(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json ArtHasStroke(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->ArtHasStroke(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json ArtCopyFillStyleIfEqualPaths(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dstArt
    AIArtHandle dstArt_val = HandleManager::art.Get(params["dstArt"].get<int32_t>());
    if (!dstArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'dstArt'");
    }
    // Input handle: srcArt
    AIArtHandle srcArt_val = HandleManager::art.Get(params["srcArt"].get<int32_t>());
    if (!srcArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'srcArt'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ArtCopyFillStyleIfEqualPaths(dstArt_val, srcArt_val);
    if (err != kNoErr) {
        throw std::runtime_error("ArtCopyFillStyleIfEqualPaths failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ArtCopyStrokeStyleIfEqualPaths(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: dstArt
    AIArtHandle dstArt_val = HandleManager::art.Get(params["dstArt"].get<int32_t>());
    if (!dstArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'dstArt'");
    }
    // Input handle: srcArt
    AIArtHandle srcArt_val = HandleManager::art.Get(params["srcArt"].get<int32_t>());
    if (!srcArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'srcArt'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ArtCopyStrokeStyleIfEqualPaths(dstArt_val, srcArt_val);
    if (err != kNoErr) {
        throw std::runtime_error("ArtCopyStrokeStyleIfEqualPaths failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetInsertionPoint(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: art
    AIArtHandle art = nullptr;
    // Output primitive: paintorder
    short paintorder{};
    // Output primitive: editable
    AIBoolean editable{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetInsertionPoint(&art, &paintorder, &editable);
    if (err != kNoErr) {
        throw std::runtime_error("GetInsertionPoint failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }
    // Marshal output primitive: paintorder
    response["paintorder"] = paintorder;
    // Marshal output primitive: editable
    response["editable"] = static_cast<bool>(editable);

    return response;
}

nlohmann::json SetInsertionPoint(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetInsertionPoint(art_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetInsertionPoint failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetKeyArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetKeyArt(&art);
    if (err != kNoErr) {
        throw std::runtime_error("GetKeyArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json HasDictionary(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->HasDictionary(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json IsDictionaryEmpty(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->IsDictionaryEmpty(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetArtName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input string: name
    ai::UnicodeString name(params["name"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetArtName(art_val, name);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtName failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output string: name
    ai::UnicodeString name;
    // Output primitive: isDefaultName
    ASBoolean isDefaultName{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtName(art_val, name, &isDefaultName);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtName failed with error: " + std::to_string(err));
    }

    // Marshal output string: name
    response["name"] = name.as_UTF8();
    // Marshal output primitive: isDefaultName
    response["isDefaultName"] = isDefaultName;

    return response;
}

nlohmann::json IsArtLayerGroup(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: isLayerGroup
    ASBoolean isLayerGroup{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->IsArtLayerGroup(art_val, &isLayerGroup);
    if (err != kNoErr) {
        throw std::runtime_error("IsArtLayerGroup failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isLayerGroup
    response["isLayerGroup"] = isLayerGroup;

    return response;
}

nlohmann::json ReleaseToLayers(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: build
    ASBoolean build = params["build"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ReleaseToLayers(art_val, build);
    if (err != kNoErr) {
        throw std::runtime_error("ReleaseToLayers failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ModifyTargetedArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: list
    AIArtHandle list = nullptr;
    // Input primitive: count
    ai::int32 count = params["count"].get<int32_t>();
    // Input primitive: action
    ai::int32 action = params["action"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ModifyTargetedArtSet(&list, count, action);
    if (err != kNoErr) {
        throw std::runtime_error("ModifyTargetedArtSet failed with error: " + std::to_string(err));
    }

    // Marshal output handle: list
    if (list) {
        response["list"] = HandleManager::art.Register(list);
    } else {
        response["list"] = -1;
    }

    return response;
}

nlohmann::json IsArtStyledArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->IsArtStyledArt(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json IsArtClipping(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->IsArtClipping(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json TransferAttributes(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: srcart
    AIArtHandle srcart_val = HandleManager::art.Get(params["srcart"].get<int32_t>());
    if (!srcart_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'srcart'");
    }
    // Input handle: dstart
    AIArtHandle dstart_val = HandleManager::art.Get(params["dstart"].get<int32_t>());
    if (!dstart_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'dstart'");
    }
    // Input primitive: which
    ai::uint32 which = params["which"].get<uint32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->TransferAttributes(srcart_val, dstart_val, which);
    if (err != kNoErr) {
        throw std::runtime_error("TransferAttributes failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtLastChild(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: child
    AIArtHandle child = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtLastChild(art_val, &child);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtLastChild failed with error: " + std::to_string(err));
    }

    // Marshal output handle: child
    if (child) {
        response["child"] = HandleManager::art.Register(child);
    } else {
        response["child"] = -1;
    }

    return response;
}

nlohmann::json SetArtTextWrapProperty(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: offset
    AIReal offset = params["offset"].get<double>();
    // Input primitive: invert
    AIBoolean invert = params["invert"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetArtTextWrapProperty(art_val, offset, invert);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtTextWrapProperty failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArtTextWrapProperty(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: offset
    AIReal offset{};
    // Output primitive: invert
    AIBoolean invert{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtTextWrapProperty(art_val, &offset, &invert);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtTextWrapProperty failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: offset
    response["offset"] = offset;
    // Marshal output primitive: invert
    response["invert"] = static_cast<bool>(invert);

    return response;
}

nlohmann::json CreateCopyScope(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: kind (enum AICopyScopeKind)
    // WARNING: Using default initialization
    enum AICopyScopeKind kind{};
    // Output unknown: scope
    AICopyScopeHandle scope{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->CreateCopyScope(kind, &scope);
    if (err != kNoErr) {
        throw std::runtime_error("CreateCopyScope failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: scope

    return response;
}

nlohmann::json DestroyCopyScope(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: scope (AICopyScopeHandle)
    // WARNING: Using default initialization
    AICopyScopeHandle scope{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->DestroyCopyScope(scope);
    if (err != kNoErr) {
        throw std::runtime_error("DestroyCopyScope failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json InsertionPointBadForArtType(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }
    // Input primitive: artType
    ai::int16 artType = params["artType"].get<int16_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->InsertionPointBadForArtType(paintOrder, prep_val, artType);
    if (err != kNoErr) {
        throw std::runtime_error("InsertionPointBadForArtType failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json PreinsertionFlightCheck(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: candidateArt
    AIArtHandle candidateArt_val = HandleManager::art.Get(params["candidateArt"].get<int32_t>());
    if (!candidateArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'candidateArt'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Input handle (optional): prep
    AIArtHandle prep_val = nullptr;
    if (params.contains("prep") && !params["prep"].is_null()) {
        int32_t prep_id = params["prep"].get<int32_t>();
        if (prep_id >= 0) {
            prep_val = HandleManager::art.Get(prep_id);
        }
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->PreinsertionFlightCheck(candidateArt_val, paintOrder, prep_val);
    if (err != kNoErr) {
        throw std::runtime_error("PreinsertionFlightCheck failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetNote(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input string: inNote
    ai::UnicodeString inNote(params["inNote"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetNote(art_val, inNote);
    if (err != kNoErr) {
        throw std::runtime_error("SetNote failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetNote(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output string: outNote
    ai::UnicodeString outNote;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetNote(art_val, outNote);
    if (err != kNoErr) {
        throw std::runtime_error("GetNote failed with error: " + std::to_string(err));
    }

    // Marshal output string: outNote
    response["outNote"] = outNote.as_UTF8();

    return response;
}

nlohmann::json HasNote(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->HasNote(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json GetArtXMPSize(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: size
    size_t size{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtXMPSize(art_val, &size);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtXMPSize failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: size
    response["size"] = size;

    return response;
}

nlohmann::json SetArtXMP(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input string: xmp
    std::string xmp_str = params["xmp"].get<std::string>();
    const char* xmp = xmp_str.c_str();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetArtXMP(art_val, xmp);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtXMP failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetPreciseArtTransformBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output struct: transform
    AIRealMatrix transform{};
    // Input primitive: flags
    ai::int32 flags = params["flags"].get<int32_t>();
    // Output unknown: bounds
    AIDoubleRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetPreciseArtTransformBounds(art_val, &transform, flags, &bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetPreciseArtTransformBounds failed with error: " + std::to_string(err));
    }

    // Marshal output struct: transform
    response["transform"] = {
        {"a", transform.a},
        {"b", transform.b},
        {"c", transform.c},
        {"d", transform.d},
        {"tx", transform.tx},
        {"ty", transform.ty}
    };
    // Unable to marshal unknown type: bounds

    return response;
}

nlohmann::json UncheckedDisposeArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->UncheckedDisposeArt(art_val);
    if (err != kNoErr) {
        throw std::runtime_error("UncheckedDisposeArt failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ArtIsGraph(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: artisgraph
    AIBoolean artisgraph{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ArtIsGraph(art_val, &artisgraph);
    if (err != kNoErr) {
        throw std::runtime_error("ArtIsGraph failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: artisgraph
    response["artisgraph"] = static_cast<bool>(artisgraph);

    return response;
}

nlohmann::json SetKeyArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetKeyArt(art_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetKeyArt failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDrawingMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: mode
    ai::int32 mode{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetDrawingMode(&mode);
    if (err != kNoErr) {
        throw std::runtime_error("GetDrawingMode failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: mode
    response["mode"] = mode;

    return response;
}

nlohmann::json SetDrawingMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: mode
    ai::int32 mode = params["mode"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetDrawingMode(mode);
    if (err != kNoErr) {
        throw std::runtime_error("SetDrawingMode failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetInsertionPointForDrawingMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: mode
    ai::int32 mode = params["mode"].get<int32_t>();
    // Output handle: art
    AIArtHandle art = nullptr;
    // Output primitive: paintorder
    short paintorder{};
    // Output primitive: editable
    AIBoolean editable{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetInsertionPointForDrawingMode(mode, &art, &paintorder, &editable);
    if (err != kNoErr) {
        throw std::runtime_error("GetInsertionPointForDrawingMode failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }
    // Marshal output primitive: paintorder
    response["paintorder"] = paintorder;
    // Marshal output primitive: editable
    response["editable"] = static_cast<bool>(editable);

    return response;
}

nlohmann::json GetInsertionPointForCurrentDrawingMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: art
    AIArtHandle art = nullptr;
    // Output primitive: paintorder
    short paintorder{};
    // Output primitive: editable
    AIBoolean editable{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetInsertionPointForCurrentDrawingMode(&art, &paintorder, &editable);
    if (err != kNoErr) {
        throw std::runtime_error("GetInsertionPointForCurrentDrawingMode failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }
    // Marshal output primitive: paintorder
    response["paintorder"] = paintorder;
    // Marshal output primitive: editable
    response["editable"] = static_cast<bool>(editable);

    return response;
}

nlohmann::json GetPathPolarity(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: polarity
    ai::int32 polarity{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetPathPolarity(art_val, &polarity);
    if (err != kNoErr) {
        throw std::runtime_error("GetPathPolarity failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: polarity
    response["polarity"] = polarity;

    return response;
}

nlohmann::json IsPixelPerfect(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sArt->IsPixelPerfect(art_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetPixelPerfect(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: isPixelPerfect
    AIBoolean isPixelPerfect = params["isPixelPerfect"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->SetPixelPerfect(art_val, isPixelPerfect);
    if (err != kNoErr) {
        throw std::runtime_error("SetPixelPerfect failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IsArtALayerInSymbol(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output primitive: isLayerInSymbol
    AIBoolean isLayerInSymbol{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->IsArtALayerInSymbol(art_val, &isLayerInSymbol);
    if (err != kNoErr) {
        throw std::runtime_error("IsArtALayerInSymbol failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isLayerInSymbol
    response["isLayerInSymbol"] = static_cast<bool>(isLayerInSymbol);

    return response;
}

nlohmann::json GetArtTimeStamp(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Unknown type: option (enum AIArtTimeStampOptions)
    // WARNING: Using default initialization
    enum AIArtTimeStampOptions option{};
    // Output primitive: timeStamp
    size_t timeStamp{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtTimeStamp(art_val, option, &timeStamp);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtTimeStamp failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: timeStamp
    response["timeStamp"] = timeStamp;

    return response;
}

nlohmann::json ConvertPointTypeToAreaType(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: newArtHandle
    AIArtHandle newArtHandle = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ConvertPointTypeToAreaType(art_val, &newArtHandle);
    if (err != kNoErr) {
        throw std::runtime_error("ConvertPointTypeToAreaType failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newArtHandle
    if (newArtHandle) {
        response["newArtHandle"] = HandleManager::art.Register(newArtHandle);
    } else {
        response["newArtHandle"] = -1;
    }

    return response;
}

nlohmann::json ConvertAreaTypeToPointType(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: newArtHandle
    AIArtHandle newArtHandle = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->ConvertAreaTypeToPointType(art_val, &newArtHandle);
    if (err != kNoErr) {
        throw std::runtime_error("ConvertAreaTypeToPointType failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newArtHandle
    if (newArtHandle) {
        response["newArtHandle"] = HandleManager::art.Register(newArtHandle);
    } else {
        response["newArtHandle"] = -1;
    }

    return response;
}

nlohmann::json MarkDirty(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Input primitive: markStyleDirty
    AIBoolean markStyleDirty = params["markStyleDirty"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArt->MarkDirty(art_val, markStyleDirty);
    if (err != kNoErr) {
        throw std::runtime_error("MarkDirty failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetSafeArtHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output unknown: safeArt
    AISafeArtHandle safeArt{};

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetSafeArtHandle(art_val, &safeArt);
    if (err != kNoErr) {
        throw std::runtime_error("GetSafeArtHandle failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: safeArt

    return response;
}

nlohmann::json GetArtHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: safeArt (AISafeArtHandle)
    // WARNING: Using default initialization
    AISafeArtHandle safeArt{};
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtHandle(safeArt, &art);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtHandle failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json GetArtDefaultName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output string: name
    ai::UnicodeString name;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetArtDefaultName(art_val, name);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtDefaultName failed with error: " + std::to_string(err));
    }

    // Marshal output string: name
    response["name"] = name.as_UTF8();

    return response;
}

nlohmann::json GetDocumentOfArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output handle: document
    AIDocumentHandle document = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArt->GetDocumentOfArt(art_val, &document);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentOfArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: document
    if (document) {
        response["document"] = HandleManager::documents.Register(document);
    } else {
        response["document"] = -1;
    }

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "NewArt") {
        return NewArt(params);
    } else if (method == "DisposeArt") {
        return DisposeArt(params);
    } else if (method == "ReorderArt") {
        return ReorderArt(params);
    } else if (method == "DuplicateArt") {
        return DuplicateArt(params);
    } else if (method == "GetFirstArtOfLayer") {
        return GetFirstArtOfLayer(params);
    } else if (method == "GetLayerOfArt") {
        return GetLayerOfArt(params);
    } else if (method == "GetArtType") {
        return GetArtType(params);
    } else if (method == "GetArtUserAttr") {
        return GetArtUserAttr(params);
    } else if (method == "SetArtUserAttr") {
        return SetArtUserAttr(params);
    } else if (method == "GetArtParent") {
        return GetArtParent(params);
    } else if (method == "GetArtFirstChild") {
        return GetArtFirstChild(params);
    } else if (method == "GetArtSibling") {
        return GetArtSibling(params);
    } else if (method == "GetArtPriorSibling") {
        return GetArtPriorSibling(params);
    } else if (method == "GetArtBounds") {
        return GetArtBounds(params);
    } else if (method == "SetArtBounds") {
        return SetArtBounds(params);
    } else if (method == "GetArtCenterPointVisible") {
        return GetArtCenterPointVisible(params);
    } else if (method == "SetArtCenterPointVisible") {
        return SetArtCenterPointVisible(params);
    } else if (method == "GetArtTransformBounds") {
        return GetArtTransformBounds(params);
    } else if (method == "UpdateArtworkLink") {
        return UpdateArtworkLink(params);
    } else if (method == "ValidArt") {
        return ValidArt(params);
    } else if (method == "GetArtOrder") {
        return GetArtOrder(params);
    } else if (method == "SelectNamedArtOfLayer") {
        return SelectNamedArtOfLayer(params);
    } else if (method == "GetArtRotatedBounds") {
        return GetArtRotatedBounds(params);
    } else if (method == "ArtHasFill") {
        return ArtHasFill(params);
    } else if (method == "ArtHasStroke") {
        return ArtHasStroke(params);
    } else if (method == "ArtCopyFillStyleIfEqualPaths") {
        return ArtCopyFillStyleIfEqualPaths(params);
    } else if (method == "ArtCopyStrokeStyleIfEqualPaths") {
        return ArtCopyStrokeStyleIfEqualPaths(params);
    } else if (method == "GetInsertionPoint") {
        return GetInsertionPoint(params);
    } else if (method == "SetInsertionPoint") {
        return SetInsertionPoint(params);
    } else if (method == "GetKeyArt") {
        return GetKeyArt(params);
    } else if (method == "HasDictionary") {
        return HasDictionary(params);
    } else if (method == "IsDictionaryEmpty") {
        return IsDictionaryEmpty(params);
    } else if (method == "SetArtName") {
        return SetArtName(params);
    } else if (method == "GetArtName") {
        return GetArtName(params);
    } else if (method == "IsArtLayerGroup") {
        return IsArtLayerGroup(params);
    } else if (method == "ReleaseToLayers") {
        return ReleaseToLayers(params);
    } else if (method == "ModifyTargetedArtSet") {
        return ModifyTargetedArtSet(params);
    } else if (method == "IsArtStyledArt") {
        return IsArtStyledArt(params);
    } else if (method == "IsArtClipping") {
        return IsArtClipping(params);
    } else if (method == "TransferAttributes") {
        return TransferAttributes(params);
    } else if (method == "GetArtLastChild") {
        return GetArtLastChild(params);
    } else if (method == "SetArtTextWrapProperty") {
        return SetArtTextWrapProperty(params);
    } else if (method == "GetArtTextWrapProperty") {
        return GetArtTextWrapProperty(params);
    } else if (method == "CreateCopyScope") {
        return CreateCopyScope(params);
    } else if (method == "DestroyCopyScope") {
        return DestroyCopyScope(params);
    } else if (method == "InsertionPointBadForArtType") {
        return InsertionPointBadForArtType(params);
    } else if (method == "PreinsertionFlightCheck") {
        return PreinsertionFlightCheck(params);
    } else if (method == "SetNote") {
        return SetNote(params);
    } else if (method == "GetNote") {
        return GetNote(params);
    } else if (method == "HasNote") {
        return HasNote(params);
    } else if (method == "GetArtXMPSize") {
        return GetArtXMPSize(params);
    } else if (method == "SetArtXMP") {
        return SetArtXMP(params);
    } else if (method == "GetPreciseArtTransformBounds") {
        return GetPreciseArtTransformBounds(params);
    } else if (method == "UncheckedDisposeArt") {
        return UncheckedDisposeArt(params);
    } else if (method == "ArtIsGraph") {
        return ArtIsGraph(params);
    } else if (method == "SetKeyArt") {
        return SetKeyArt(params);
    } else if (method == "GetDrawingMode") {
        return GetDrawingMode(params);
    } else if (method == "SetDrawingMode") {
        return SetDrawingMode(params);
    } else if (method == "GetInsertionPointForDrawingMode") {
        return GetInsertionPointForDrawingMode(params);
    } else if (method == "GetInsertionPointForCurrentDrawingMode") {
        return GetInsertionPointForCurrentDrawingMode(params);
    } else if (method == "GetPathPolarity") {
        return GetPathPolarity(params);
    } else if (method == "IsPixelPerfect") {
        return IsPixelPerfect(params);
    } else if (method == "SetPixelPerfect") {
        return SetPixelPerfect(params);
    } else if (method == "IsArtALayerInSymbol") {
        return IsArtALayerInSymbol(params);
    } else if (method == "GetArtTimeStamp") {
        return GetArtTimeStamp(params);
    } else if (method == "ConvertPointTypeToAreaType") {
        return ConvertPointTypeToAreaType(params);
    } else if (method == "ConvertAreaTypeToPointType") {
        return ConvertAreaTypeToPointType(params);
    } else if (method == "MarkDirty") {
        return MarkDirty(params);
    } else if (method == "GetSafeArtHandle") {
        return GetSafeArtHandle(params);
    } else if (method == "GetArtHandle") {
        return GetArtHandle(params);
    } else if (method == "GetArtDefaultName") {
        return GetArtDefaultName(params);
    } else if (method == "GetDocumentOfArt") {
        return GetDocumentOfArt(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIArtSuite");
}

} // namespace AIArtSuite
} // namespace Flora
