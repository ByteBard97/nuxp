#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

namespace Flora {
namespace AIArtboardSuite {

/**
 * Wrapper for AIArtboardSuite::Init
 * @returns ["artboard"] - ai::ArtboardProperties
 */
nlohmann::json Init(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::CloneArtboard
 * @param params["newArtboard"] - const ai::ArtboardProperties
 * @returns ["artboard"] - ai::ArtboardProperties
 */
nlohmann::json CloneArtboard(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::Dispose
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json Dispose(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetPosition
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["bounds"] - AIRealRect
 */
nlohmann::json GetPosition(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetPosition
 * @param params["bounds"] - const AIRealRect
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json SetPosition(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetPAR
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["par"] - AIReal
 */
nlohmann::json GetPAR(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetPAR
 * @param params["par"] - AIReal
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json SetPAR(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetName
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["name"] - ai::UnicodeString
 */
nlohmann::json GetName(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetName
 * @param params["name"] - const ai::UnicodeString
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json SetName(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetShowDisplayMark
 * @param params["properties"] - const ai::ArtboardProperties
 * @param params["type"] - ai::ArtboardProperties::DisplayMarkType
 * @returns ["show"] - AIBoolean
 */
nlohmann::json GetShowDisplayMark(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetShowDisplayMark
 * @param params["type"] - ai::ArtboardProperties::DisplayMarkType
 * @param params["show"] - AIBoolean
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json SetShowDisplayMark(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetArtboardList
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json GetArtboardList(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::ReleaseArtboardList
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json ReleaseArtboardList(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::AddNew
 * @returns ["artboardList"] - ai::ArtboardList
 * @returns ["newArtboard"] - ai::ArtboardProperties
 * @returns ["index"] - ai::ArtboardID
 */
nlohmann::json AddNew(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::Delete
 * @param params["index"] - ai::ArtboardID
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json Delete(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetCount
 * @param params["artboardList"] - const ai::ArtboardList
 * @returns ["count"] - ai::ArtboardID
 */
nlohmann::json GetCount(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetActive
 * @param params["artboardList"] - const ai::ArtboardList
 * @returns ["index"] - ai::ArtboardID
 */
nlohmann::json GetActive(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetActive
 * @param params["index"] - ai::ArtboardID
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json SetActive(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::Update
 * @param params["index"] - ai::ArtboardID
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json Update(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetArtboardProperties
 * @param params["index"] - ai::ArtboardID
 * @returns ["artboardList"] - ai::ArtboardList
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json GetArtboardProperties(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetRulerOrigin
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["rulerOrigin"] - AIRealPoint
 */
nlohmann::json GetRulerOrigin(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetRulerOrigin
 * @param params["rulerOrigin"] - const AIRealPoint
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json SetRulerOrigin(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::Insert
 * @returns ["artboardList"] - ai::ArtboardList
 * @returns ["artboard"] - ai::ArtboardProperties
 * @returns ["index"] - ai::ArtboardID
 */
nlohmann::json Insert(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::IsDefaultName
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["isDefault"] - AIBoolean
 */
nlohmann::json IsDefaultName(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetIsDefaultName
 * @param params["isDefault"] - const AIBoolean
 * @returns ["properties"] - ai::ArtboardProperties
 */
nlohmann::json SetIsDefaultName(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::IsSelected
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["isSelected"] - AIBoolean
 */
nlohmann::json IsSelected(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SelectArtboard
 * @param params["artboardID"] - ai::ArtboardID
 * @param params["exclusively"] - AIBoolean
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json SelectArtboard(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SelectArtboards
 * @param params["artboardIDs"] - const ai::AutoBuffer&lt;ai::ArtboardID&gt;
 * @param params["exclusively"] - AIBoolean
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json SelectArtboards(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SelectAllArtboards
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json SelectAllArtboards(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::DeleteArtboards
 * @param params["artboardIDs"] - const ai::AutoBuffer&lt;ai::ArtboardID&gt;
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json DeleteArtboards(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::DeselectArtboard
 * @param params["artboardID"] - ai::ArtboardID
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json DeselectArtboard(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::DeselectAllArtboards
 * @returns ["artboardList"] - ai::ArtboardList
 */
nlohmann::json DeselectAllArtboards(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::AreAnyArtboardsOverlapping
 * @returns ["artboardList"] - ai::ArtboardList
 * @returns ["isOverlapping"] - AIBoolean
 */
nlohmann::json AreAnyArtboardsOverlapping(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetUUID
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["uuid"] - ai::ArtboardUUID
 */
nlohmann::json GetUUID(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetUUIDAsString
 * @param params["properties"] - const ai::ArtboardProperties
 * @returns ["uuid"] - ai::UnicodeString
 */
nlohmann::json GetUUIDAsString(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::InsertUsingArtboardPropertiesUUID
 * @returns ["artboardList"] - ai::ArtboardList
 * @returns ["artboard"] - ai::ArtboardProperties
 * @returns ["index"] - ai::ArtboardID
 */
nlohmann::json InsertUsingArtboardPropertiesUUID(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetLocked
 * @param params["artboardList"] - const ai::ArtboardProperties
 * @returns ["isLocked"] - AIBoolean
 */
nlohmann::json GetLocked(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetLocked
 * @param params["isLocked"] - const AIBoolean
 * @returns ["artboardList"] - ai::ArtboardProperties
 */
nlohmann::json SetLocked(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::GetHide
 * @param params["artboardList"] - const ai::ArtboardProperties
 * @returns ["isHidden"] - AIBoolean
 */
nlohmann::json GetHide(const nlohmann::json& params);

/**
 * Wrapper for AIArtboardSuite::SetHide
 * @param params["isHidden"] - const AIBoolean
 * @returns ["artboardList"] - ai::ArtboardProperties
 */
nlohmann::json SetHide(const nlohmann::json& params);

/**
 * Dispatch a method call by name
 * @param method The method name to call
 * @param params The JSON parameters to pass to the method
 * @returns The JSON result from the method call
 * @throws std::runtime_error if method is not found
 */
nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params);

} // namespace AIArtboardSuite
} // namespace Flora
