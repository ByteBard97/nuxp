#include "FloraAIMaskSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIMaskSuite* sMask;

namespace Flora {
namespace AIMaskSuite {

nlohmann::json GetMask(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: object
    AIArtHandle object_val = HandleManager::art.Get(params["object"].get<int32_t>());
    if (!object_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'object'");
    }
    // Output handle: mask
    AIMaskRef mask = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sMask->GetMask(object_val, &mask);
    if (err != kNoErr) {
        throw std::runtime_error("GetMask failed with error: " + std::to_string(err));
    }

    // Marshal output handle: mask
    if (mask) {
        response["mask"] = HandleManager::masks.Register(mask);
    } else {
        response["mask"] = -1;
    }

    return response;
}

nlohmann::json CreateMask(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: object
    AIArtHandle object_val = HandleManager::art.Get(params["object"].get<int32_t>());
    if (!object_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'object'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sMask->CreateMask(object_val);
    if (err != kNoErr) {
        throw std::runtime_error("CreateMask failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json DeleteMask(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: object
    AIArtHandle object_val = HandleManager::art.Get(params["object"].get<int32_t>());
    if (!object_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'object'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sMask->DeleteMask(object_val);
    if (err != kNoErr) {
        throw std::runtime_error("DeleteMask failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLinked(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sMask->GetLinked(mask_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetLinked(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }
    // Input primitive: linked
    AIBoolean linked = params["linked"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sMask->SetLinked(mask_val, linked);
    if (err != kNoErr) {
        throw std::runtime_error("SetLinked failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDisabled(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sMask->GetDisabled(mask_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetDisabled(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }
    // Input primitive: disabled
    AIBoolean disabled = params["disabled"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sMask->SetDisabled(mask_val, disabled);
    if (err != kNoErr) {
        throw std::runtime_error("SetDisabled failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetInverted(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sMask->GetInverted(mask_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetInverted(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }
    // Input primitive: inverted
    AIBoolean inverted = params["inverted"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sMask->SetInverted(mask_val, inverted);
    if (err != kNoErr) {
        throw std::runtime_error("SetInverted failed with error: " + std::to_string(err));
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
    AIErr err = sMask->Copy(source_val, destination_val);
    if (err != kNoErr) {
        throw std::runtime_error("Copy failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }

    // Call SDK function (returns handle)
    AIArtHandle result = sMask->GetArt(mask_val);
    if (result) {
        response["result"] = HandleManager::art.Register(result);
    } else {
        response["result"] = -1;
    }


    return response;
}

nlohmann::json IsEditingArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sMask->IsEditingArt(mask_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetEditingArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }
    // Input primitive: isedit
    AIBoolean isedit = params["isedit"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sMask->SetEditingArt(mask_val, isedit);
    if (err != kNoErr) {
        throw std::runtime_error("SetEditingArt failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetMaskedArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIArtHandle mask_val = HandleManager::art.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'mask'");
    }
    // Output handle: masked
    AIArtHandle masked = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sMask->GetMaskedArt(mask_val, &masked);
    if (err != kNoErr) {
        throw std::runtime_error("GetMaskedArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: masked
    if (masked) {
        response["masked"] = HandleManager::art.Register(masked);
    } else {
        response["masked"] = -1;
    }

    return response;
}

nlohmann::json GetClipping(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }

    // Call SDK function (returns boolean)
    AIBoolean result = sMask->GetClipping(mask_val);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json SetClipping(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: mask
    AIMaskRef mask_val = HandleManager::masks.Get(params["mask"].get<int32_t>());
    if (!mask_val) {
        throw std::runtime_error("Invalid AIMaskRef handle for parameter 'mask'");
    }
    // Input primitive: clipping
    AIBoolean clipping = params["clipping"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sMask->SetClipping(mask_val, clipping);
    if (err != kNoErr) {
        throw std::runtime_error("SetClipping failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetMask") {
        return GetMask(params);
    } else if (method == "CreateMask") {
        return CreateMask(params);
    } else if (method == "DeleteMask") {
        return DeleteMask(params);
    } else if (method == "GetLinked") {
        return GetLinked(params);
    } else if (method == "SetLinked") {
        return SetLinked(params);
    } else if (method == "GetDisabled") {
        return GetDisabled(params);
    } else if (method == "SetDisabled") {
        return SetDisabled(params);
    } else if (method == "GetInverted") {
        return GetInverted(params);
    } else if (method == "SetInverted") {
        return SetInverted(params);
    } else if (method == "Copy") {
        return Copy(params);
    } else if (method == "GetArt") {
        return GetArt(params);
    } else if (method == "IsEditingArt") {
        return IsEditingArt(params);
    } else if (method == "SetEditingArt") {
        return SetEditingArt(params);
    } else if (method == "GetMaskedArt") {
        return GetMaskedArt(params);
    } else if (method == "GetClipping") {
        return GetClipping(params);
    } else if (method == "SetClipping") {
        return SetClipping(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIMaskSuite");
}

} // namespace AIMaskSuite
} // namespace Flora
