#include "FloraAITimerSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AITimerSuite* sTimer;

namespace Flora {
namespace AITimerSuite {

nlohmann::json GetTimerName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: timer
    AITimerHandle timer_val = HandleManager::timers.Get(params["timer"].get<int32_t>());
    if (!timer_val) {
        throw std::runtime_error("Invalid AITimerHandle handle for parameter 'timer'");
    }
    // Output string pointer (SDK-managed): name
    char* name = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->GetTimerName(timer_val, &name);
    if (err != kNoErr) {
        throw std::runtime_error("GetTimerName failed with error: " + std::to_string(err));
    }

    // Marshal output string pointer: name
    response["name"] = name ? std::string(name) : "";

    return response;
}

nlohmann::json GetTimerActive(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: timer
    AITimerHandle timer_val = HandleManager::timers.Get(params["timer"].get<int32_t>());
    if (!timer_val) {
        throw std::runtime_error("Invalid AITimerHandle handle for parameter 'timer'");
    }
    // Output primitive: active
    AIBoolean active{};

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->GetTimerActive(timer_val, &active);
    if (err != kNoErr) {
        throw std::runtime_error("GetTimerActive failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: active
    response["active"] = static_cast<bool>(active);

    return response;
}

nlohmann::json SetTimerActive(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: timer
    AITimerHandle timer_val = HandleManager::timers.Get(params["timer"].get<int32_t>());
    if (!timer_val) {
        throw std::runtime_error("Invalid AITimerHandle handle for parameter 'timer'");
    }
    // Input primitive: active
    AIBoolean active = params["active"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->SetTimerActive(timer_val, active);
    if (err != kNoErr) {
        throw std::runtime_error("SetTimerActive failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetTimerPeriod(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: timer
    AITimerHandle timer_val = HandleManager::timers.Get(params["timer"].get<int32_t>());
    if (!timer_val) {
        throw std::runtime_error("Invalid AITimerHandle handle for parameter 'timer'");
    }
    // Output primitive: period
    ai::int32 period{};

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->GetTimerPeriod(timer_val, &period);
    if (err != kNoErr) {
        throw std::runtime_error("GetTimerPeriod failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: period
    response["period"] = period;

    return response;
}

nlohmann::json SetTimerPeriod(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: timer
    AITimerHandle timer_val = HandleManager::timers.Get(params["timer"].get<int32_t>());
    if (!timer_val) {
        throw std::runtime_error("Invalid AITimerHandle handle for parameter 'timer'");
    }
    // Input primitive: period
    ai::int32 period = params["period"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->SetTimerPeriod(timer_val, period);
    if (err != kNoErr) {
        throw std::runtime_error("SetTimerPeriod failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetTimerPlugin(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: timer
    AITimerHandle timer_val = HandleManager::timers.Get(params["timer"].get<int32_t>());
    if (!timer_val) {
        throw std::runtime_error("Invalid AITimerHandle handle for parameter 'timer'");
    }
    // Output unknown: plugin
    SPPluginRef plugin{};

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->GetTimerPlugin(timer_val, &plugin);
    if (err != kNoErr) {
        throw std::runtime_error("GetTimerPlugin failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: plugin

    return response;
}

nlohmann::json CountTimers(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->CountTimers(&count);
    if (err != kNoErr) {
        throw std::runtime_error("CountTimers failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetNthTimer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Output handle: timer
    AITimerHandle timer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sTimer->GetNthTimer(n, &timer);
    if (err != kNoErr) {
        throw std::runtime_error("GetNthTimer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: timer
    if (timer) {
        response["timer"] = HandleManager::timers.Register(timer);
    } else {
        response["timer"] = -1;
    }

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetTimerName") {
        return GetTimerName(params);
    } else if (method == "GetTimerActive") {
        return GetTimerActive(params);
    } else if (method == "SetTimerActive") {
        return SetTimerActive(params);
    } else if (method == "GetTimerPeriod") {
        return GetTimerPeriod(params);
    } else if (method == "SetTimerPeriod") {
        return SetTimerPeriod(params);
    } else if (method == "GetTimerPlugin") {
        return GetTimerPlugin(params);
    } else if (method == "CountTimers") {
        return CountTimers(params);
    } else if (method == "GetNthTimer") {
        return GetNthTimer(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AITimerSuite");
}

} // namespace AITimerSuite
} // namespace Flora
