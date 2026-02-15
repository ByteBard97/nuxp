#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AILayerListSuite {

/**
 * Wrapper for AILayerListSuite::GetLayerOfArt
 * @param params["art"] - AIArtHandle (handle ID)
 * @returns ["list"] - AILayerList
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetLayerOfArt(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::Count
 * @returns ["count"] - ai::int32
 */
nlohmann::json Count(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetFirst
 * @returns ["list"] - AILayerList
 */
nlohmann::json GetFirst(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetLast
 * @returns ["list"] - AILayerList
 */
nlohmann::json GetLast(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetNext
 * @param params["list"] - AILayerList
 * @returns ["next"] - AILayerList
 */
nlohmann::json GetNext(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::CountLayers
 * @param params["list"] - AILayerList
 * @returns ["count"] - ai::int32
 */
nlohmann::json CountLayers(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetFirstLayer
 * @param params["list"] - AILayerList
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetFirstLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetLastLayer
 * @param params["list"] - AILayerList
 * @returns ["layer"] - AILayerHandle (handle ID)
 */
nlohmann::json GetLastLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetNextLayer
 * @param params["list"] - AILayerList
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["next"] - AILayerHandle (handle ID)
 */
nlohmann::json GetNextLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::GetPrevLayer
 * @param params["list"] - AILayerList
 * @param params["layer"] - AILayerHandle (handle ID)
 * @returns ["prev"] - AILayerHandle (handle ID)
 */
nlohmann::json GetPrevLayer(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::SetDisplayMode
 * @param params["list"] - AILayerList
 * @param params["mode"] - AILayerListMode
 */
nlohmann::json SetDisplayMode(const nlohmann::json& params);

/**
 * Wrapper for AILayerListSuite::SetEditabilityMode
 * @param params["list"] - AILayerList
 * @param params["mode"] - AILayerListEditabilityMode
 */
nlohmann::json SetEditabilityMode(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AILayerListSuite
} // namespace Flora
