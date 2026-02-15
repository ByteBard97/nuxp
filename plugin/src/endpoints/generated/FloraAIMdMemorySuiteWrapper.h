#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIMdMemorySuite {

/**
 * Wrapper for AIMdMemorySuite::MdMemoryNewHandle
 * @param params["size"] - size_t
 * @returns ["hMem"] - AIMdMemoryHandle
 */
nlohmann::json MdMemoryNewHandle(const nlohmann::json& params);

/**
 * Wrapper for AIMdMemorySuite::MdMemoryDisposeHandle
 * @param params["hMem"] - AIMdMemoryHandle
 */
nlohmann::json MdMemoryDisposeHandle(const nlohmann::json& params);

/**
 * Wrapper for AIMdMemorySuite::MdMemoryGetSize
 * @param params["hMem"] - AIMdMemoryHandle
 * @returns ["size"] - size_t
 */
nlohmann::json MdMemoryGetSize(const nlohmann::json& params);

/**
 * Wrapper for AIMdMemorySuite::MdMemoryResize
 * @param params["hMem"] - AIMdMemoryHandle
 * @param params["newSize"] - size_t
 */
nlohmann::json MdMemoryResize(const nlohmann::json& params);

/**
 * Wrapper for AIMdMemorySuite::MdMemoryUnLock
 * @param params["hMem"] - AIMdMemoryHandle
 */
nlohmann::json MdMemoryUnLock(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIMdMemorySuite
} // namespace Flora
