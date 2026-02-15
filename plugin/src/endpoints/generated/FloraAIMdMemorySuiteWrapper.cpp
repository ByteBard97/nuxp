#include "FloraAIMdMemorySuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIMdMemorySuite* sMdMemory;

namespace Flora {
namespace AIMdMemorySuite {

nlohmann::json MdMemoryNewHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: size
    size_t size = params["size"].get<uint32_t>();
    // Output unknown: hMem
    AIMdMemoryHandle hMem{};

    // Call SDK function (returns AIErr)
    AIErr err = sMdMemory->MdMemoryNewHandle(size, &hMem);
    if (err != kNoErr) {
        throw std::runtime_error("MdMemoryNewHandle failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: hMem

    return response;
}

nlohmann::json MdMemoryDisposeHandle(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: hMem (AIMdMemoryHandle)
    // WARNING: Using default initialization
    AIMdMemoryHandle hMem{};

    // Call SDK function (returns AIErr)
    AIErr err = sMdMemory->MdMemoryDisposeHandle(hMem);
    if (err != kNoErr) {
        throw std::runtime_error("MdMemoryDisposeHandle failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MdMemoryGetSize(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: hMem (AIMdMemoryHandle)
    // WARNING: Using default initialization
    AIMdMemoryHandle hMem{};
    // Output primitive: size
    size_t size{};

    // Call SDK function (returns AIErr)
    AIErr err = sMdMemory->MdMemoryGetSize(hMem, &size);
    if (err != kNoErr) {
        throw std::runtime_error("MdMemoryGetSize failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: size
    response["size"] = size;

    return response;
}

nlohmann::json MdMemoryResize(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: hMem (AIMdMemoryHandle)
    // WARNING: Using default initialization
    AIMdMemoryHandle hMem{};
    // Input primitive: newSize
    size_t newSize = params["newSize"].get<uint32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sMdMemory->MdMemoryResize(hMem, newSize);
    if (err != kNoErr) {
        throw std::runtime_error("MdMemoryResize failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json MdMemoryUnLock(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: hMem (AIMdMemoryHandle)
    // WARNING: Using default initialization
    AIMdMemoryHandle hMem{};

    // Call SDK function (returns AIErr)
    AIErr err = sMdMemory->MdMemoryUnLock(hMem);
    if (err != kNoErr) {
        throw std::runtime_error("MdMemoryUnLock failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "MdMemoryNewHandle") {
        return MdMemoryNewHandle(params);
    } else if (method == "MdMemoryDisposeHandle") {
        return MdMemoryDisposeHandle(params);
    } else if (method == "MdMemoryGetSize") {
        return MdMemoryGetSize(params);
    } else if (method == "MdMemoryResize") {
        return MdMemoryResize(params);
    } else if (method == "MdMemoryUnLock") {
        return MdMemoryUnLock(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIMdMemorySuite");
}

} // namespace AIMdMemorySuite
} // namespace Flora
