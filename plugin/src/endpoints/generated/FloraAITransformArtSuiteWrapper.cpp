#include "FloraAITransformArtSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AITransformArtSuite* sTransformArt;

namespace Flora {
namespace AITransformArtSuite {

nlohmann::json TransformArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Output struct: matrix
    AIRealMatrix matrix{};
    // Input primitive: lineScale
    AIReal lineScale = params["lineScale"].get<double>();
    // Input primitive: flags
    ai::int32 flags = params["flags"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sTransformArt->TransformArt(art_val, &matrix, lineScale, flags);
    if (err != kNoErr) {
        throw std::runtime_error("TransformArt failed with error: " + std::to_string(err));
    }

    // Marshal output struct: matrix
    response["matrix"] = {
        {"a", matrix.a},
        {"b", matrix.b},
        {"c", matrix.c},
        {"d", matrix.d},
        {"tx", matrix.tx},
        {"ty", matrix.ty}
    };

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "TransformArt") {
        return TransformArt(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AITransformArtSuite");
}

} // namespace AITransformArtSuite
} // namespace Flora
