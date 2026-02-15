#include "FloraAINotifierSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AINotifierSuite* sNotifier;

namespace Flora {
namespace AINotifierSuite {

nlohmann::json GetNotifierActive(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: notifier
    AINotifierHandle notifier_val = HandleManager::notifiers.Get(params["notifier"].get<int32_t>());
    if (!notifier_val) {
        throw std::runtime_error("Invalid AINotifierHandle handle for parameter 'notifier'");
    }
    // Output primitive: active
    AIBoolean active{};

    // Call SDK function (returns AIErr)
    AIErr err = sNotifier->GetNotifierActive(notifier_val, &active);
    if (err != kNoErr) {
        throw std::runtime_error("GetNotifierActive failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: active
    response["active"] = static_cast<bool>(active);

    return response;
}

nlohmann::json SetNotifierActive(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: notifier
    AINotifierHandle notifier_val = HandleManager::notifiers.Get(params["notifier"].get<int32_t>());
    if (!notifier_val) {
        throw std::runtime_error("Invalid AINotifierHandle handle for parameter 'notifier'");
    }
    // Input primitive: active
    AIBoolean active = params["active"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sNotifier->SetNotifierActive(notifier_val, active);
    if (err != kNoErr) {
        throw std::runtime_error("SetNotifierActive failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetNotifierPlugin(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: notifier
    AINotifierHandle notifier_val = HandleManager::notifiers.Get(params["notifier"].get<int32_t>());
    if (!notifier_val) {
        throw std::runtime_error("Invalid AINotifierHandle handle for parameter 'notifier'");
    }
    // Output unknown: plugin
    SPPluginRef plugin{};

    // Call SDK function (returns AIErr)
    AIErr err = sNotifier->GetNotifierPlugin(notifier_val, &plugin);
    if (err != kNoErr) {
        throw std::runtime_error("GetNotifierPlugin failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: plugin

    return response;
}

nlohmann::json CountNotifiers(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sNotifier->CountNotifiers(&count);
    if (err != kNoErr) {
        throw std::runtime_error("CountNotifiers failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetNthNotifier(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Output handle: notifier
    AINotifierHandle notifier = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sNotifier->GetNthNotifier(n, &notifier);
    if (err != kNoErr) {
        throw std::runtime_error("GetNthNotifier failed with error: " + std::to_string(err));
    }

    // Marshal output handle: notifier
    if (notifier) {
        response["notifier"] = HandleManager::notifiers.Register(notifier);
    } else {
        response["notifier"] = -1;
    }

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetNotifierActive") {
        return GetNotifierActive(params);
    } else if (method == "SetNotifierActive") {
        return SetNotifierActive(params);
    } else if (method == "GetNotifierPlugin") {
        return GetNotifierPlugin(params);
    } else if (method == "CountNotifiers") {
        return CountNotifiers(params);
    } else if (method == "GetNthNotifier") {
        return GetNthNotifier(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AINotifierSuite");
}

} // namespace AINotifierSuite
} // namespace Flora
