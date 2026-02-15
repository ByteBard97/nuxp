#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIArtSetSuite {

/**
 * Wrapper for AIArtSetSuite::NewArtSet
 * @returns ["artSet"] - AIArtSet
 */
nlohmann::json NewArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::DisposeArtSet
 * @returns ["artSet"] - AIArtSet
 */
nlohmann::json DisposeArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::CountArtSet
 * @param params["artSet"] - AIArtSet
 * @returns ["count"] - size_t
 */
nlohmann::json CountArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::IndexArtSet
 * @param params["artSet"] - AIArtSet
 * @param params["index"] - size_t
 * @returns ["art"] - AIArtHandle (handle ID)
 */
nlohmann::json IndexArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::ArrayArtSet
 * @param params["artSet"] - AIArtSet
 * @param params["count"] - size_t
 * @returns ["artArray"] - AIArtHandle (handle ID)
 */
nlohmann::json ArrayArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::SelectedArtSet
 * @param params["artSet"] - AIArtSet
 */
nlohmann::json SelectedArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::MatchingArtSet
 * @param params["numSpecs"] - ai::int16
 * @param params["artSet"] - AIArtSet
 * @returns ["specs"] - AIArtSpec
 */
nlohmann::json MatchingArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::LayerArtSet
 * @param params["layer"] - AILayerHandle (handle ID)
 * @param params["artSet"] - AIArtSet
 */
nlohmann::json LayerArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::NotArtSet
 * @param params["src"] - AIArtSet
 * @param params["dst"] - AIArtSet
 */
nlohmann::json NotArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::UnionArtSet
 * @param params["src0"] - AIArtSet
 * @param params["src1"] - AIArtSet
 * @param params["dst"] - AIArtSet
 */
nlohmann::json UnionArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::IntersectArtSet
 * @param params["src0"] - AIArtSet
 * @param params["src1"] - AIArtSet
 * @param params["dst"] - AIArtSet
 */
nlohmann::json IntersectArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::NextInArtSet
 * @param params["artSet"] - AIArtSet
 * @param params["prevArt"] - AIArtHandle (handle ID)
 * @returns ["nextArt"] - AIArtHandle (handle ID)
 */
nlohmann::json NextInArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::AddArtToArtSet
 * @param params["artSet"] - AIArtSet
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json AddArtToArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::RemoveArtFromArtSet
 * @param params["artSet"] - AIArtSet
 * @param params["art"] - AIArtHandle (handle ID)
 */
nlohmann::json RemoveArtFromArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::ReplaceArtInArtSet
 * @param params["artSet"] - AIArtSet
 * @param params["oldArt"] - AIArtHandle (handle ID)
 * @param params["newArt"] - AIArtHandle (handle ID)
 */
nlohmann::json ReplaceArtInArtSet(const nlohmann::json& params);

/**
 * Wrapper for AIArtSetSuite::ClearArtSet
 * @param params["artSet"] - AIArtSet
 */
nlohmann::json ClearArtSet(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIArtSetSuite
} // namespace Flora
