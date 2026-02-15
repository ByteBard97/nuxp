#include "FloraAIAppContextSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIAppContextSuite* sAppContext;

namespace Flora {
namespace AIAppContextSuite {

nlohmann::json GetPlatformAppWindow(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: appWindow
    AIWindowRef appWindow{};

    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->GetPlatformAppWindow(&appWindow);
    if (err != kNoErr) {
        throw std::runtime_error("GetPlatformAppWindow failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: appWindow

    return response;
}

nlohmann::json AllowAllChanges(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: allowAllChanges
    ASBoolean allowAllChanges = params["allowAllChanges"].get<bool>();
    // Output primitive: previousState
    ASBoolean previousState{};

    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->AllowAllChanges(allowAllChanges, &previousState);
    if (err != kNoErr) {
        throw std::runtime_error("AllowAllChanges failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: previousState
    response["previousState"] = previousState;

    return response;
}

nlohmann::json AllowProgress(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: showProgress
    bool showProgress = params["showProgress"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->AllowProgress(showProgress);
    if (err != kNoErr) {
        throw std::runtime_error("AllowProgress failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetPlatformAppMenu(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: appMenu
    AIAppMenuContext appMenu{};

    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->GetPlatformAppMenu(&appMenu);
    if (err != kNoErr) {
        throw std::runtime_error("GetPlatformAppMenu failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: appMenu

    return response;
}

nlohmann::json SyncAndDraw(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->SyncAndDraw();
    if (err != kNoErr) {
        throw std::runtime_error("SyncAndDraw failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetRulerWidthOffsetOnActiveDocument(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: horzRulerBounds
    AIRect horzRulerBounds{};
    // Output unknown: vertRulerBounds
    AIRect vertRulerBounds{};
    // Output unknown: cntrRulerBounds
    AIRect cntrRulerBounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->GetRulerWidthOffsetOnActiveDocument(&horzRulerBounds, &vertRulerBounds, &cntrRulerBounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetRulerWidthOffsetOnActiveDocument failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: horzRulerBounds
    // Unable to marshal unknown type: vertRulerBounds
    // Unable to marshal unknown type: cntrRulerBounds

    return response;
}

nlohmann::json IsProgressBarAllowed(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: result
    AIBoolean result{};

    // Call SDK function (returns AIErr)
    AIErr err = sAppContext->IsProgressBarAllowed(result);
    if (err != kNoErr) {
        throw std::runtime_error("IsProgressBarAllowed failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: result
    response["result"] = static_cast<bool>(result);

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetPlatformAppWindow") {
        return GetPlatformAppWindow(params);
    } else if (method == "AllowAllChanges") {
        return AllowAllChanges(params);
    } else if (method == "AllowProgress") {
        return AllowProgress(params);
    } else if (method == "GetPlatformAppMenu") {
        return GetPlatformAppMenu(params);
    } else if (method == "SyncAndDraw") {
        return SyncAndDraw(params);
    } else if (method == "GetRulerWidthOffsetOnActiveDocument") {
        return GetRulerWidthOffsetOnActiveDocument(params);
    } else if (method == "IsProgressBarAllowed") {
        return IsProgressBarAllowed(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIAppContextSuite");
}

} // namespace AIAppContextSuite
} // namespace Flora
