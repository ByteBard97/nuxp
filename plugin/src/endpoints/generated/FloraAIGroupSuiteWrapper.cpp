#include "FloraAIGroupSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIGroupSuite* sGroup;

namespace Flora {
namespace AIGroupSuite {

nlohmann::json GetGroupClipped(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: group
    AIArtHandle group_val = HandleManager::art.Get(params["group"].get<int32_t>());
    if (!group_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'group'");
    }
    // Output primitive: clipped
    AIBoolean clipped{};

    // Call SDK function (returns AIErr)
    AIErr err = sGroup->GetGroupClipped(group_val, &clipped);
    if (err != kNoErr) {
        throw std::runtime_error("GetGroupClipped failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: clipped
    response["clipped"] = static_cast<bool>(clipped);

    return response;
}

nlohmann::json SetGroupClipped(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: group
    AIArtHandle group_val = HandleManager::art.Get(params["group"].get<int32_t>());
    if (!group_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'group'");
    }
    // Input primitive: clipped
    AIBoolean clipped = params["clipped"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sGroup->SetGroupClipped(group_val, clipped);
    if (err != kNoErr) {
        throw std::runtime_error("SetGroupClipped failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetGroupMaskLock(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: group
    AIArtHandle group_val = HandleManager::art.Get(params["group"].get<int32_t>());
    if (!group_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'group'");
    }
    // Output primitive: maskLocked
    AIBoolean maskLocked{};

    // Call SDK function (returns AIErr)
    AIErr err = sGroup->GetGroupMaskLock(group_val, &maskLocked);
    if (err != kNoErr) {
        throw std::runtime_error("GetGroupMaskLock failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: maskLocked
    response["maskLocked"] = static_cast<bool>(maskLocked);

    return response;
}

nlohmann::json SetGroupMaskLock(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: group
    AIArtHandle group_val = HandleManager::art.Get(params["group"].get<int32_t>());
    if (!group_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'group'");
    }
    // Input primitive: maskLocked
    AIBoolean maskLocked = params["maskLocked"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sGroup->SetGroupMaskLock(group_val, maskLocked);
    if (err != kNoErr) {
        throw std::runtime_error("SetGroupMaskLock failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetGroupClipped") {
        return GetGroupClipped(params);
    } else if (method == "SetGroupClipped") {
        return SetGroupClipped(params);
    } else if (method == "GetGroupMaskLock") {
        return GetGroupMaskLock(params);
    } else if (method == "SetGroupMaskLock") {
        return SetGroupMaskLock(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIGroupSuite");
}

} // namespace AIGroupSuite
} // namespace Flora
