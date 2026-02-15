#include "FloraAILayerListSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AILayerListSuite* sLayerList;

namespace Flora {
namespace AILayerListSuite {

nlohmann::json GetLayerOfArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output unknown: list
    AILayerList list{};
    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetLayerOfArt(art_val, &list, &layer);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerOfArt failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: list
    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json Count(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->Count(&count);
    if (err != kNoErr) {
        throw std::runtime_error("Count failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetFirst(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: list
    AILayerList list{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetFirst(&list);
    if (err != kNoErr) {
        throw std::runtime_error("GetFirst failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: list

    return response;
}

nlohmann::json GetLast(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: list
    AILayerList list{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetLast(&list);
    if (err != kNoErr) {
        throw std::runtime_error("GetLast failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: list

    return response;
}

nlohmann::json GetNext(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Output unknown: next
    AILayerList next{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetNext(list, &next);
    if (err != kNoErr) {
        throw std::runtime_error("GetNext failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: next

    return response;
}

nlohmann::json CountLayers(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->CountLayers(list, &count);
    if (err != kNoErr) {
        throw std::runtime_error("CountLayers failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetFirstLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetFirstLayer(list, &layer);
    if (err != kNoErr) {
        throw std::runtime_error("GetFirstLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json GetLastLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetLastLayer(list, &layer);
    if (err != kNoErr) {
        throw std::runtime_error("GetLastLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json GetNextLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output handle: next
    AILayerHandle next = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetNextLayer(list, layer_val, &next);
    if (err != kNoErr) {
        throw std::runtime_error("GetNextLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: next
    if (next) {
        response["next"] = HandleManager::layers.Register(next);
    } else {
        response["next"] = -1;
    }

    return response;
}

nlohmann::json GetPrevLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output handle: prev
    AILayerHandle prev = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->GetPrevLayer(list, layer_val, &prev);
    if (err != kNoErr) {
        throw std::runtime_error("GetPrevLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: prev
    if (prev) {
        response["prev"] = HandleManager::layers.Register(prev);
    } else {
        response["prev"] = -1;
    }

    return response;
}

nlohmann::json SetDisplayMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Unknown type: mode (AILayerListMode)
    // WARNING: Using default initialization
    AILayerListMode mode{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->SetDisplayMode(list, mode);
    if (err != kNoErr) {
        throw std::runtime_error("SetDisplayMode failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetEditabilityMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: list (AILayerList)
    // WARNING: Using default initialization
    AILayerList list{};
    // Unknown type: mode (AILayerListEditabilityMode)
    // WARNING: Using default initialization
    AILayerListEditabilityMode mode{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayerList->SetEditabilityMode(list, mode);
    if (err != kNoErr) {
        throw std::runtime_error("SetEditabilityMode failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetLayerOfArt") {
        return GetLayerOfArt(params);
    } else if (method == "Count") {
        return Count(params);
    } else if (method == "GetFirst") {
        return GetFirst(params);
    } else if (method == "GetLast") {
        return GetLast(params);
    } else if (method == "GetNext") {
        return GetNext(params);
    } else if (method == "CountLayers") {
        return CountLayers(params);
    } else if (method == "GetFirstLayer") {
        return GetFirstLayer(params);
    } else if (method == "GetLastLayer") {
        return GetLastLayer(params);
    } else if (method == "GetNextLayer") {
        return GetNextLayer(params);
    } else if (method == "GetPrevLayer") {
        return GetPrevLayer(params);
    } else if (method == "SetDisplayMode") {
        return SetDisplayMode(params);
    } else if (method == "SetEditabilityMode") {
        return SetEditabilityMode(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AILayerListSuite");
}

} // namespace AILayerListSuite
} // namespace Flora
