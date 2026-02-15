#include "FloraAIArtSetSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIArtSetSuite* sArtSet;

namespace Flora {
namespace AIArtSetSuite {

nlohmann::json NewArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: artSet
    AIArtSet artSet{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->NewArtSet(&artSet);
    if (err != kNoErr) {
        throw std::runtime_error("NewArtSet failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: artSet

    return response;
}

nlohmann::json DisposeArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: artSet
    AIArtSet artSet{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->DisposeArtSet(&artSet);
    if (err != kNoErr) {
        throw std::runtime_error("DisposeArtSet failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: artSet

    return response;
}

nlohmann::json CountArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Output primitive: count
    size_t count{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->CountArtSet(artSet, &count);
    if (err != kNoErr) {
        throw std::runtime_error("CountArtSet failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json IndexArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Input primitive: index
    size_t index = params["index"].get<uint32_t>();
    // Output handle: art
    AIArtHandle art = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->IndexArtSet(artSet, index, &art);
    if (err != kNoErr) {
        throw std::runtime_error("IndexArtSet failed with error: " + std::to_string(err));
    }

    // Marshal output handle: art
    if (art) {
        response["art"] = HandleManager::art.Register(art);
    } else {
        response["art"] = -1;
    }

    return response;
}

nlohmann::json ArrayArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Output handle: artArray
    AIArtHandle artArray = nullptr;
    // Input primitive: count
    size_t count = params["count"].get<uint32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->ArrayArtSet(artSet, &artArray, count);
    if (err != kNoErr) {
        throw std::runtime_error("ArrayArtSet failed with error: " + std::to_string(err));
    }

    // Marshal output handle: artArray
    if (artArray) {
        response["artArray"] = HandleManager::art.Register(artArray);
    } else {
        response["artArray"] = -1;
    }

    return response;
}

nlohmann::json SelectedArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->SelectedArtSet(artSet);
    if (err != kNoErr) {
        throw std::runtime_error("SelectedArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MatchingArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: specs
    AIArtSpec specs{};
    // Input primitive: numSpecs
    ai::int16 numSpecs = params["numSpecs"].get<int16_t>();
    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->MatchingArtSet(&specs, numSpecs, artSet);
    if (err != kNoErr) {
        throw std::runtime_error("MatchingArtSet failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: specs

    return response;
}

nlohmann::json LayerArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->LayerArtSet(layer_val, artSet);
    if (err != kNoErr) {
        throw std::runtime_error("LayerArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json NotArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: src (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet src{};
    // Unknown type: dst (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet dst{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->NotArtSet(src, dst);
    if (err != kNoErr) {
        throw std::runtime_error("NotArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json UnionArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: src0 (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet src0{};
    // Unknown type: src1 (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet src1{};
    // Unknown type: dst (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet dst{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->UnionArtSet(src0, src1, dst);
    if (err != kNoErr) {
        throw std::runtime_error("UnionArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IntersectArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: src0 (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet src0{};
    // Unknown type: src1 (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet src1{};
    // Unknown type: dst (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet dst{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->IntersectArtSet(src0, src1, dst);
    if (err != kNoErr) {
        throw std::runtime_error("IntersectArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json NextInArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Input handle: prevArt
    AIArtHandle prevArt_val = HandleManager::art.Get(params["prevArt"].get<int32_t>());
    if (!prevArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'prevArt'");
    }
    // Output handle: nextArt
    AIArtHandle nextArt = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->NextInArtSet(artSet, prevArt_val, &nextArt);
    if (err != kNoErr) {
        throw std::runtime_error("NextInArtSet failed with error: " + std::to_string(err));
    }

    // Marshal output handle: nextArt
    if (nextArt) {
        response["nextArt"] = HandleManager::art.Register(nextArt);
    } else {
        response["nextArt"] = -1;
    }

    return response;
}

nlohmann::json AddArtToArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->AddArtToArtSet(artSet, art_val);
    if (err != kNoErr) {
        throw std::runtime_error("AddArtToArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json RemoveArtFromArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->RemoveArtFromArtSet(artSet, art_val);
    if (err != kNoErr) {
        throw std::runtime_error("RemoveArtFromArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ReplaceArtInArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};
    // Input handle: oldArt
    AIArtHandle oldArt_val = HandleManager::art.Get(params["oldArt"].get<int32_t>());
    if (!oldArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'oldArt'");
    }
    // Input handle: newArt
    AIArtHandle newArt_val = HandleManager::art.Get(params["newArt"].get<int32_t>());
    if (!newArt_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'newArt'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->ReplaceArtInArtSet(artSet, oldArt_val, newArt_val);
    if (err != kNoErr) {
        throw std::runtime_error("ReplaceArtInArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ClearArtSet(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: artSet (AIArtSet)
    // WARNING: Using default initialization
    AIArtSet artSet{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtSet->ClearArtSet(artSet);
    if (err != kNoErr) {
        throw std::runtime_error("ClearArtSet failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "NewArtSet") {
        return NewArtSet(params);
    } else if (method == "DisposeArtSet") {
        return DisposeArtSet(params);
    } else if (method == "CountArtSet") {
        return CountArtSet(params);
    } else if (method == "IndexArtSet") {
        return IndexArtSet(params);
    } else if (method == "ArrayArtSet") {
        return ArrayArtSet(params);
    } else if (method == "SelectedArtSet") {
        return SelectedArtSet(params);
    } else if (method == "MatchingArtSet") {
        return MatchingArtSet(params);
    } else if (method == "LayerArtSet") {
        return LayerArtSet(params);
    } else if (method == "NotArtSet") {
        return NotArtSet(params);
    } else if (method == "UnionArtSet") {
        return UnionArtSet(params);
    } else if (method == "IntersectArtSet") {
        return IntersectArtSet(params);
    } else if (method == "NextInArtSet") {
        return NextInArtSet(params);
    } else if (method == "AddArtToArtSet") {
        return AddArtToArtSet(params);
    } else if (method == "RemoveArtFromArtSet") {
        return RemoveArtFromArtSet(params);
    } else if (method == "ReplaceArtInArtSet") {
        return ReplaceArtInArtSet(params);
    } else if (method == "ClearArtSet") {
        return ClearArtSet(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIArtSetSuite");
}

} // namespace AIArtSetSuite
} // namespace Flora
