#include "FloraAIArtboardSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIArtboardSuite* sArtboard;

namespace Flora {
namespace AIArtboardSuite {

nlohmann::json Init(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboard
    ai::ArtboardProperties artboard;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->Init(artboard);
    if (err != kNoErr) {
        throw std::runtime_error("Init failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboard
    response["artboard"] = HandleManager::artboardProperties.Register(std::move(artboard));

    return response;
}

nlohmann::json CloneArtboard(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboard
    ai::ArtboardProperties artboard;
    // Input managed handle: newArtboard
    auto* newArtboard_ptr = HandleManager::artboardProperties.Get(params["newArtboard"].get<int32_t>());
    if (!newArtboard_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'newArtboard'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->CloneArtboard(artboard, *newArtboard_ptr);
    if (err != kNoErr) {
        throw std::runtime_error("CloneArtboard failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboard
    response["artboard"] = HandleManager::artboardProperties.Register(std::move(artboard));

    return response;
}

nlohmann::json Dispose(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->Dispose(properties);
    if (err != kNoErr) {
        throw std::runtime_error("Dispose failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json GetPosition(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetPosition(*properties_ptr, bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetPosition failed with error: " + std::to_string(err));
    }

    // Marshal output struct: bounds
    response["bounds"] = {
        {"left", bounds.left},
        {"top", bounds.top},
        {"right", bounds.right},
        {"bottom", bounds.bottom}
    };

    return response;
}

nlohmann::json SetPosition(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;
    // Input struct: bounds
    AIRealRect bounds;
    bounds.left = params["bounds"]["left"].get<double>();
    bounds.top = params["bounds"]["top"].get<double>();
    bounds.right = params["bounds"]["right"].get<double>();
    bounds.bottom = params["bounds"]["bottom"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetPosition(properties, bounds);
    if (err != kNoErr) {
        throw std::runtime_error("SetPosition failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json GetPAR(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output primitive: par
    AIReal par{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetPAR(*properties_ptr, par);
    if (err != kNoErr) {
        throw std::runtime_error("GetPAR failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: par
    response["par"] = par;

    return response;
}

nlohmann::json SetPAR(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;
    // Input primitive: par
    AIReal par = params["par"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetPAR(properties, par);
    if (err != kNoErr) {
        throw std::runtime_error("SetPAR failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json GetName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output string: name
    ai::UnicodeString name;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetName(*properties_ptr, name);
    if (err != kNoErr) {
        throw std::runtime_error("GetName failed with error: " + std::to_string(err));
    }

    // Marshal output string: name
    response["name"] = name.as_UTF8();

    return response;
}

nlohmann::json SetName(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;
    // Input string: name
    ai::UnicodeString name(params["name"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetName(properties, name);
    if (err != kNoErr) {
        throw std::runtime_error("SetName failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json GetShowDisplayMark(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Unknown type: type (ai::ArtboardProperties::DisplayMarkType)
    // WARNING: Using default initialization
    ai::ArtboardProperties::DisplayMarkType type{};
    // Output primitive: show
    AIBoolean show{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetShowDisplayMark(*properties_ptr, type, show);
    if (err != kNoErr) {
        throw std::runtime_error("GetShowDisplayMark failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: show
    response["show"] = static_cast<bool>(show);

    return response;
}

nlohmann::json SetShowDisplayMark(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;
    // Unknown type: type (ai::ArtboardProperties::DisplayMarkType)
    // WARNING: Using default initialization
    ai::ArtboardProperties::DisplayMarkType type{};
    // Input primitive: show
    AIBoolean show = params["show"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetShowDisplayMark(properties, type, show);
    if (err != kNoErr) {
        throw std::runtime_error("SetShowDisplayMark failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json GetArtboardList(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetArtboardList(artboardList);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtboardList failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json ReleaseArtboardList(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->ReleaseArtboardList(artboardList);
    if (err != kNoErr) {
        throw std::runtime_error("ReleaseArtboardList failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json AddNew(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Output managed handle: newArtboard
    ai::ArtboardProperties newArtboard;
    // Output primitive: index
    ai::ArtboardID index{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->AddNew(artboardList, newArtboard, index);
    if (err != kNoErr) {
        throw std::runtime_error("AddNew failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));
    // Marshal output managed handle: newArtboard
    response["newArtboard"] = HandleManager::artboardProperties.Register(std::move(newArtboard));
    // Marshal output primitive: index
    response["index"] = index;

    return response;
}

nlohmann::json Delete(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Input primitive: index
    ai::ArtboardID index = params["index"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->Delete(artboardList, index);
    if (err != kNoErr) {
        throw std::runtime_error("Delete failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json GetCount(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: artboardList
    auto* artboardList_ptr = HandleManager::artboardLists.Get(params["artboardList"].get<int32_t>());
    if (!artboardList_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'artboardList'");
    }
    // Output primitive: count
    ai::ArtboardID count{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetCount(*artboardList_ptr, count);
    if (err != kNoErr) {
        throw std::runtime_error("GetCount failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetActive(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: artboardList
    auto* artboardList_ptr = HandleManager::artboardLists.Get(params["artboardList"].get<int32_t>());
    if (!artboardList_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'artboardList'");
    }
    // Output primitive: index
    ai::ArtboardID index{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetActive(*artboardList_ptr, index);
    if (err != kNoErr) {
        throw std::runtime_error("GetActive failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: index
    response["index"] = index;

    return response;
}

nlohmann::json SetActive(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Input primitive: index
    ai::ArtboardID index = params["index"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetActive(artboardList, index);
    if (err != kNoErr) {
        throw std::runtime_error("SetActive failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json Update(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Input primitive: index
    ai::ArtboardID index = params["index"].get<int32_t>();
    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->Update(artboardList, index, *properties_ptr);
    if (err != kNoErr) {
        throw std::runtime_error("Update failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json GetArtboardProperties(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Input primitive: index
    ai::ArtboardID index = params["index"].get<int32_t>();
    // Output managed handle: properties
    ai::ArtboardProperties properties;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetArtboardProperties(artboardList, index, properties);
    if (err != kNoErr) {
        throw std::runtime_error("GetArtboardProperties failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));
    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json GetRulerOrigin(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output struct: rulerOrigin
    AIRealPoint rulerOrigin{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetRulerOrigin(*properties_ptr, rulerOrigin);
    if (err != kNoErr) {
        throw std::runtime_error("GetRulerOrigin failed with error: " + std::to_string(err));
    }

    // Marshal output struct: rulerOrigin
    response["rulerOrigin"] = {
        {"h", rulerOrigin.h},
        {"v", rulerOrigin.v}
    };

    return response;
}

nlohmann::json SetRulerOrigin(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;
    // Input struct: rulerOrigin
    AIRealPoint rulerOrigin;
    rulerOrigin.h = params["rulerOrigin"]["h"].get<double>();
    rulerOrigin.v = params["rulerOrigin"]["v"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetRulerOrigin(properties, rulerOrigin);
    if (err != kNoErr) {
        throw std::runtime_error("SetRulerOrigin failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json Insert(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Output managed handle: artboard
    ai::ArtboardProperties artboard;
    // Output primitive: index
    ai::ArtboardID index{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->Insert(artboardList, artboard, index);
    if (err != kNoErr) {
        throw std::runtime_error("Insert failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));
    // Marshal output managed handle: artboard
    response["artboard"] = HandleManager::artboardProperties.Register(std::move(artboard));
    // Marshal output primitive: index
    response["index"] = index;

    return response;
}

nlohmann::json IsDefaultName(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output primitive: isDefault
    AIBoolean isDefault{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->IsDefaultName(*properties_ptr, isDefault);
    if (err != kNoErr) {
        throw std::runtime_error("IsDefaultName failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isDefault
    response["isDefault"] = static_cast<bool>(isDefault);

    return response;
}

nlohmann::json SetIsDefaultName(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: properties
    ai::ArtboardProperties properties;
    // Input primitive: isDefault
    AIBoolean isDefault = params["isDefault"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetIsDefaultName(properties, isDefault);
    if (err != kNoErr) {
        throw std::runtime_error("SetIsDefaultName failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: properties
    response["properties"] = HandleManager::artboardProperties.Register(std::move(properties));

    return response;
}

nlohmann::json IsSelected(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output primitive: isSelected
    AIBoolean isSelected{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->IsSelected(*properties_ptr, isSelected);
    if (err != kNoErr) {
        throw std::runtime_error("IsSelected failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isSelected
    response["isSelected"] = static_cast<bool>(isSelected);

    return response;
}

nlohmann::json SelectArtboard(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Input primitive: artboardID
    ai::ArtboardID artboardID = params["artboardID"].get<int32_t>();
    // Input primitive: exclusively
    AIBoolean exclusively = params["exclusively"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SelectArtboard(artboardList, artboardID, exclusively);
    if (err != kNoErr) {
        throw std::runtime_error("SelectArtboard failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json SelectArtboards(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Unknown type: artboardIDs (const ai::AutoBuffer<ai::ArtboardID>)
    // WARNING: Using default initialization
    const ai::AutoBuffer<ai::ArtboardID> artboardIDs{};
    // Input primitive: exclusively
    AIBoolean exclusively = params["exclusively"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SelectArtboards(artboardList, artboardIDs, exclusively);
    if (err != kNoErr) {
        throw std::runtime_error("SelectArtboards failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json SelectAllArtboards(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SelectAllArtboards(artboardList);
    if (err != kNoErr) {
        throw std::runtime_error("SelectAllArtboards failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json DeleteArtboards(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Unknown type: artboardIDs (const ai::AutoBuffer<ai::ArtboardID>)
    // WARNING: Using default initialization
    const ai::AutoBuffer<ai::ArtboardID> artboardIDs{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->DeleteArtboards(artboardList, artboardIDs);
    if (err != kNoErr) {
        throw std::runtime_error("DeleteArtboards failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json DeselectArtboard(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Input primitive: artboardID
    ai::ArtboardID artboardID = params["artboardID"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->DeselectArtboard(artboardList, artboardID);
    if (err != kNoErr) {
        throw std::runtime_error("DeselectArtboard failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json DeselectAllArtboards(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->DeselectAllArtboards(artboardList);
    if (err != kNoErr) {
        throw std::runtime_error("DeselectAllArtboards failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));

    return response;
}

nlohmann::json AreAnyArtboardsOverlapping(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Output primitive: isOverlapping
    AIBoolean isOverlapping{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->AreAnyArtboardsOverlapping(artboardList, isOverlapping);
    if (err != kNoErr) {
        throw std::runtime_error("AreAnyArtboardsOverlapping failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));
    // Marshal output primitive: isOverlapping
    response["isOverlapping"] = static_cast<bool>(isOverlapping);

    return response;
}

nlohmann::json GetUUID(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output unknown: uuid
    ai::ArtboardUUID uuid{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetUUID(*properties_ptr, uuid);
    if (err != kNoErr) {
        throw std::runtime_error("GetUUID failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: uuid

    return response;
}

nlohmann::json GetUUIDAsString(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: properties
    auto* properties_ptr = HandleManager::artboardProperties.Get(params["properties"].get<int32_t>());
    if (!properties_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'properties'");
    }
    // Output string: uuid
    ai::UnicodeString uuid;

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetUUIDAsString(*properties_ptr, uuid);
    if (err != kNoErr) {
        throw std::runtime_error("GetUUIDAsString failed with error: " + std::to_string(err));
    }

    // Marshal output string: uuid
    response["uuid"] = uuid.as_UTF8();

    return response;
}

nlohmann::json InsertUsingArtboardPropertiesUUID(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardList artboardList;
    // Output managed handle: artboard
    ai::ArtboardProperties artboard;
    // Output primitive: index
    ai::ArtboardID index{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->InsertUsingArtboardPropertiesUUID(artboardList, artboard, index);
    if (err != kNoErr) {
        throw std::runtime_error("InsertUsingArtboardPropertiesUUID failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardLists.Register(std::move(artboardList));
    // Marshal output managed handle: artboard
    response["artboard"] = HandleManager::artboardProperties.Register(std::move(artboard));
    // Marshal output primitive: index
    response["index"] = index;

    return response;
}

nlohmann::json GetLocked(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: artboardList
    auto* artboardList_ptr = HandleManager::artboardProperties.Get(params["artboardList"].get<int32_t>());
    if (!artboardList_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'artboardList'");
    }
    // Output primitive: isLocked
    AIBoolean isLocked{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetLocked(*artboardList_ptr, isLocked);
    if (err != kNoErr) {
        throw std::runtime_error("GetLocked failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isLocked
    response["isLocked"] = static_cast<bool>(isLocked);

    return response;
}

nlohmann::json SetLocked(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardProperties artboardList;
    // Input primitive: isLocked
    AIBoolean isLocked = params["isLocked"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetLocked(artboardList, isLocked);
    if (err != kNoErr) {
        throw std::runtime_error("SetLocked failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardProperties.Register(std::move(artboardList));

    return response;
}

nlohmann::json GetHide(const nlohmann::json& params) {
    nlohmann::json response;

    // Input managed handle: artboardList
    auto* artboardList_ptr = HandleManager::artboardProperties.Get(params["artboardList"].get<int32_t>());
    if (!artboardList_ptr) {
        throw std::runtime_error("Invalid managed handle for parameter 'artboardList'");
    }
    // Output primitive: isHidden
    AIBoolean isHidden{};

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->GetHide(*artboardList_ptr, isHidden);
    if (err != kNoErr) {
        throw std::runtime_error("GetHide failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isHidden
    response["isHidden"] = static_cast<bool>(isHidden);

    return response;
}

nlohmann::json SetHide(const nlohmann::json& params) {
    nlohmann::json response;

    // Output managed handle: artboardList
    ai::ArtboardProperties artboardList;
    // Input primitive: isHidden
    AIBoolean isHidden = params["isHidden"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sArtboard->SetHide(artboardList, isHidden);
    if (err != kNoErr) {
        throw std::runtime_error("SetHide failed with error: " + std::to_string(err));
    }

    // Marshal output managed handle: artboardList
    response["artboardList"] = HandleManager::artboardProperties.Register(std::move(artboardList));

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "Init") {
        return Init(params);
    } else if (method == "CloneArtboard") {
        return CloneArtboard(params);
    } else if (method == "Dispose") {
        return Dispose(params);
    } else if (method == "GetPosition") {
        return GetPosition(params);
    } else if (method == "SetPosition") {
        return SetPosition(params);
    } else if (method == "GetPAR") {
        return GetPAR(params);
    } else if (method == "SetPAR") {
        return SetPAR(params);
    } else if (method == "GetName") {
        return GetName(params);
    } else if (method == "SetName") {
        return SetName(params);
    } else if (method == "GetShowDisplayMark") {
        return GetShowDisplayMark(params);
    } else if (method == "SetShowDisplayMark") {
        return SetShowDisplayMark(params);
    } else if (method == "GetArtboardList") {
        return GetArtboardList(params);
    } else if (method == "ReleaseArtboardList") {
        return ReleaseArtboardList(params);
    } else if (method == "AddNew") {
        return AddNew(params);
    } else if (method == "Delete") {
        return Delete(params);
    } else if (method == "GetCount") {
        return GetCount(params);
    } else if (method == "GetActive") {
        return GetActive(params);
    } else if (method == "SetActive") {
        return SetActive(params);
    } else if (method == "Update") {
        return Update(params);
    } else if (method == "GetArtboardProperties") {
        return GetArtboardProperties(params);
    } else if (method == "GetRulerOrigin") {
        return GetRulerOrigin(params);
    } else if (method == "SetRulerOrigin") {
        return SetRulerOrigin(params);
    } else if (method == "Insert") {
        return Insert(params);
    } else if (method == "IsDefaultName") {
        return IsDefaultName(params);
    } else if (method == "SetIsDefaultName") {
        return SetIsDefaultName(params);
    } else if (method == "IsSelected") {
        return IsSelected(params);
    } else if (method == "SelectArtboard") {
        return SelectArtboard(params);
    } else if (method == "SelectArtboards") {
        return SelectArtboards(params);
    } else if (method == "SelectAllArtboards") {
        return SelectAllArtboards(params);
    } else if (method == "DeleteArtboards") {
        return DeleteArtboards(params);
    } else if (method == "DeselectArtboard") {
        return DeselectArtboard(params);
    } else if (method == "DeselectAllArtboards") {
        return DeselectAllArtboards(params);
    } else if (method == "AreAnyArtboardsOverlapping") {
        return AreAnyArtboardsOverlapping(params);
    } else if (method == "GetUUID") {
        return GetUUID(params);
    } else if (method == "GetUUIDAsString") {
        return GetUUIDAsString(params);
    } else if (method == "InsertUsingArtboardPropertiesUUID") {
        return InsertUsingArtboardPropertiesUUID(params);
    } else if (method == "GetLocked") {
        return GetLocked(params);
    } else if (method == "SetLocked") {
        return SetLocked(params);
    } else if (method == "GetHide") {
        return GetHide(params);
    } else if (method == "SetHide") {
        return SetHide(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIArtboardSuite");
}

} // namespace AIArtboardSuite
} // namespace Flora
