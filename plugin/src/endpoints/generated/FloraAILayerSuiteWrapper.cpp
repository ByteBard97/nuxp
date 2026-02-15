#include "FloraAILayerSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AILayerSuite* sLayer;

namespace Flora {
namespace AILayerSuite {

nlohmann::json CountLayers(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->CountLayers(&count);
    if (err != kNoErr) {
        throw std::runtime_error("CountLayers failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetNthLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetNthLayer(n, &layer);
    if (err != kNoErr) {
        throw std::runtime_error("GetNthLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json GetCurrentLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: layer
    AILayerHandle layer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetCurrentLayer(&layer);
    if (err != kNoErr) {
        throw std::runtime_error("GetCurrentLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json SetCurrentLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetCurrentLayer(layer_val);
    if (err != kNoErr) {
        throw std::runtime_error("SetCurrentLayer failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetFirstLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: first
    AILayerHandle first = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetFirstLayer(&first);
    if (err != kNoErr) {
        throw std::runtime_error("GetFirstLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: first
    if (first) {
        response["first"] = HandleManager::layers.Register(first);
    } else {
        response["first"] = -1;
    }

    return response;
}

nlohmann::json GetNextLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: prev
    AILayerHandle prev_val = HandleManager::layers.Get(params["prev"].get<int32_t>());
    if (!prev_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'prev'");
    }
    // Output handle: next
    AILayerHandle next = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetNextLayer(prev_val, &next);
    if (err != kNoErr) {
        throw std::runtime_error("GetNextLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: next
    if (next) {
        response["next"] = HandleManager::layers.Register(next);
    } else {
        response["next"] = -1;
    }

    return response;
}

nlohmann::json InsertLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: paintOrder
    ai::int16 paintOrder = params["paintOrder"].get<int16_t>();
    // Output handle: newLayer
    AILayerHandle newLayer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->InsertLayer(layer_val, paintOrder, &newLayer);
    if (err != kNoErr) {
        throw std::runtime_error("InsertLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newLayer
    if (newLayer) {
        response["newLayer"] = HandleManager::layers.Register(newLayer);
    } else {
        response["newLayer"] = -1;
    }

    return response;
}

nlohmann::json DeleteLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->DeleteLayer(layer_val);
    if (err != kNoErr) {
        throw std::runtime_error("DeleteLayer failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerTitle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output string: title
    ai::UnicodeString title;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerTitle(layer_val, title);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerTitle failed with error: " + std::to_string(err));
    }

    // Marshal output string: title
    response["title"] = title.as_UTF8();

    return response;
}

nlohmann::json SetLayerTitle(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input string: newTitle
    ai::UnicodeString newTitle(params["newTitle"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerTitle(layer_val, newTitle);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerTitle failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerColor(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output unknown: color
    AIRGBColor color{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerColor(layer_val, &color);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerColor failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: color

    return response;
}

nlohmann::json GetLayerVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: visible
    AIBoolean visible{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerVisible(layer_val, &visible);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerVisible failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: visible
    response["visible"] = static_cast<bool>(visible);

    return response;
}

nlohmann::json SetLayerVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: visible
    AIBoolean visible = params["visible"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerVisible(layer_val, visible);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerVisible failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerPreview(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: preview
    AIBoolean preview{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerPreview(layer_val, &preview);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerPreview failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: preview
    response["preview"] = static_cast<bool>(preview);

    return response;
}

nlohmann::json SetLayerPreview(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: preview
    AIBoolean preview = params["preview"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerPreview(layer_val, preview);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerPreview failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerEditable(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: editable
    AIBoolean editable{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerEditable(layer_val, &editable);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerEditable failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: editable
    response["editable"] = static_cast<bool>(editable);

    return response;
}

nlohmann::json SetLayerEditable(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: editable
    AIBoolean editable = params["editable"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerEditable(layer_val, editable);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerEditable failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerPrinted(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: printed
    AIBoolean printed{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerPrinted(layer_val, &printed);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerPrinted failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: printed
    response["printed"] = static_cast<bool>(printed);

    return response;
}

nlohmann::json SetLayerPrinted(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: printed
    AIBoolean printed = params["printed"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerPrinted(layer_val, printed);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerPrinted failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerDimPlacedImages(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: dimmed
    AIBoolean dimmed{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerDimPlacedImages(layer_val, &dimmed);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerDimPlacedImages failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: dimmed
    response["dimmed"] = static_cast<bool>(dimmed);

    return response;
}

nlohmann::json SetLayerDimPlacedImages(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: dimmed
    AIBoolean dimmed = params["dimmed"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerDimPlacedImages(layer_val, dimmed);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerDimPlacedImages failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerSelected(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: selected
    AIBoolean selected{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerSelected(layer_val, &selected);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerSelected failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: selected
    response["selected"] = static_cast<bool>(selected);

    return response;
}

nlohmann::json SetLayerSelected(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: selected
    AIBoolean selected = params["selected"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerSelected(layer_val, selected);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerSelected failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerByTitle(const nlohmann::json& params) {
    nlohmann::json response;

    // Output handle: layer
    AILayerHandle layer = nullptr;
    // Input string: title
    ai::UnicodeString title(params["title"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerByTitle(&layer, title);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerByTitle failed with error: " + std::to_string(err));
    }

    // Marshal output handle: layer
    if (layer) {
        response["layer"] = HandleManager::layers.Register(layer);
    } else {
        response["layer"] = -1;
    }

    return response;
}

nlohmann::json LayerHasArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: hasArt
    AIBoolean hasArt{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->LayerHasArt(layer_val, &hasArt);
    if (err != kNoErr) {
        throw std::runtime_error("LayerHasArt failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasArt
    response["hasArt"] = static_cast<bool>(hasArt);

    return response;
}

nlohmann::json LayerHasSelectedArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: hasSel
    AIBoolean hasSel{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->LayerHasSelectedArt(layer_val, &hasSel);
    if (err != kNoErr) {
        throw std::runtime_error("LayerHasSelectedArt failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: hasSel
    response["hasSel"] = static_cast<bool>(hasSel);

    return response;
}

nlohmann::json DeselectArtOnLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->DeselectArtOnLayer(layer_val);
    if (err != kNoErr) {
        throw std::runtime_error("DeselectArtOnLayer failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SelectArtOnLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SelectArtOnLayer(layer_val);
    if (err != kNoErr) {
        throw std::runtime_error("SelectArtOnLayer failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerIsTemplate(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: isTemplate
    AIBoolean isTemplate{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerIsTemplate(layer_val, &isTemplate);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerIsTemplate failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isTemplate
    response["isTemplate"] = static_cast<bool>(isTemplate);

    return response;
}

nlohmann::json SetLayerIsTemplate(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: isTemplate
    AIBoolean isTemplate = params["isTemplate"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerIsTemplate(layer_val, isTemplate);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerIsTemplate failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetPrevLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: next
    AILayerHandle next_val = HandleManager::layers.Get(params["next"].get<int32_t>());
    if (!next_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'next'");
    }
    // Output handle: prev
    AILayerHandle prev = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetPrevLayer(next_val, &prev);
    if (err != kNoErr) {
        throw std::runtime_error("GetPrevLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: prev
    if (prev) {
        response["prev"] = HandleManager::layers.Register(prev);
    } else {
        response["prev"] = -1;
    }

    return response;
}

nlohmann::json GetLayerDimmedPercent(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output primitive: percent
    ai::int32 percent{};

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerDimmedPercent(layer_val, &percent);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerDimmedPercent failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: percent
    response["percent"] = percent;

    return response;
}

nlohmann::json SetLayerDimmedPercent(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input primitive: percent
    ai::int32 percent = params["percent"].get<int32_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->SetLayerDimmedPercent(layer_val, percent);
    if (err != kNoErr) {
        throw std::runtime_error("SetLayerDimmedPercent failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetLayerFirstChild(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output handle: child
    AILayerHandle child = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerFirstChild(layer_val, &child);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerFirstChild failed with error: " + std::to_string(err));
    }

    // Marshal output handle: child
    if (child) {
        response["child"] = HandleManager::layers.Register(child);
    } else {
        response["child"] = -1;
    }

    return response;
}

nlohmann::json GetLayerParent(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Output handle: parent
    AILayerHandle parent = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetLayerParent(layer_val, &parent);
    if (err != kNoErr) {
        throw std::runtime_error("GetLayerParent failed with error: " + std::to_string(err));
    }

    // Marshal output handle: parent
    if (parent) {
        response["parent"] = HandleManager::layers.Register(parent);
    } else {
        response["parent"] = -1;
    }

    return response;
}

nlohmann::json InsertLayerAtArt(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: art
    AIArtHandle art_val = HandleManager::art.Get(params["art"].get<int32_t>());
    if (!art_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'art'");
    }
    // Unknown type: paintOrder (AIPaintOrder)
    // WARNING: Using default initialization
    AIPaintOrder paintOrder{};
    // Output handle: newLayer
    AILayerHandle newLayer = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->InsertLayerAtArt(art_val, paintOrder, &newLayer);
    if (err != kNoErr) {
        throw std::runtime_error("InsertLayerAtArt failed with error: " + std::to_string(err));
    }

    // Marshal output handle: newLayer
    if (newLayer) {
        response["newLayer"] = HandleManager::layers.Register(newLayer);
    } else {
        response["newLayer"] = -1;
    }

    return response;
}

nlohmann::json ChangeLayerToGroup(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: layer
    AILayerHandle layer_val = HandleManager::layers.Get(params["layer"].get<int32_t>());
    if (!layer_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'layer'");
    }
    // Input handle: group
    AIArtHandle group_val = HandleManager::art.Get(params["group"].get<int32_t>());
    if (!group_val) {
        throw std::runtime_error("Invalid AIArtHandle handle for parameter 'group'");
    }

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->ChangeLayerToGroup(layer_val, group_val);
    if (err != kNoErr) {
        throw std::runtime_error("ChangeLayerToGroup failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetNextPreorderLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: prev
    AILayerHandle prev_val = HandleManager::layers.Get(params["prev"].get<int32_t>());
    if (!prev_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'prev'");
    }
    // Output handle: next
    AILayerHandle next = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetNextPreorderLayer(prev_val, &next);
    if (err != kNoErr) {
        throw std::runtime_error("GetNextPreorderLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: next
    if (next) {
        response["next"] = HandleManager::layers.Register(next);
    } else {
        response["next"] = -1;
    }

    return response;
}

nlohmann::json GetNextNonChildPreorderLayer(const nlohmann::json& params) {
    nlohmann::json response;

    // Input handle: prev
    AILayerHandle prev_val = HandleManager::layers.Get(params["prev"].get<int32_t>());
    if (!prev_val) {
        throw std::runtime_error("Invalid AILayerHandle handle for parameter 'prev'");
    }
    // Output handle: next
    AILayerHandle next = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sLayer->GetNextNonChildPreorderLayer(prev_val, &next);
    if (err != kNoErr) {
        throw std::runtime_error("GetNextNonChildPreorderLayer failed with error: " + std::to_string(err));
    }

    // Marshal output handle: next
    if (next) {
        response["next"] = HandleManager::layers.Register(next);
    } else {
        response["next"] = -1;
    }

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "CountLayers") {
        return CountLayers(params);
    } else if (method == "GetNthLayer") {
        return GetNthLayer(params);
    } else if (method == "GetCurrentLayer") {
        return GetCurrentLayer(params);
    } else if (method == "SetCurrentLayer") {
        return SetCurrentLayer(params);
    } else if (method == "GetFirstLayer") {
        return GetFirstLayer(params);
    } else if (method == "GetNextLayer") {
        return GetNextLayer(params);
    } else if (method == "InsertLayer") {
        return InsertLayer(params);
    } else if (method == "DeleteLayer") {
        return DeleteLayer(params);
    } else if (method == "GetLayerTitle") {
        return GetLayerTitle(params);
    } else if (method == "SetLayerTitle") {
        return SetLayerTitle(params);
    } else if (method == "GetLayerColor") {
        return GetLayerColor(params);
    } else if (method == "GetLayerVisible") {
        return GetLayerVisible(params);
    } else if (method == "SetLayerVisible") {
        return SetLayerVisible(params);
    } else if (method == "GetLayerPreview") {
        return GetLayerPreview(params);
    } else if (method == "SetLayerPreview") {
        return SetLayerPreview(params);
    } else if (method == "GetLayerEditable") {
        return GetLayerEditable(params);
    } else if (method == "SetLayerEditable") {
        return SetLayerEditable(params);
    } else if (method == "GetLayerPrinted") {
        return GetLayerPrinted(params);
    } else if (method == "SetLayerPrinted") {
        return SetLayerPrinted(params);
    } else if (method == "GetLayerDimPlacedImages") {
        return GetLayerDimPlacedImages(params);
    } else if (method == "SetLayerDimPlacedImages") {
        return SetLayerDimPlacedImages(params);
    } else if (method == "GetLayerSelected") {
        return GetLayerSelected(params);
    } else if (method == "SetLayerSelected") {
        return SetLayerSelected(params);
    } else if (method == "GetLayerByTitle") {
        return GetLayerByTitle(params);
    } else if (method == "LayerHasArt") {
        return LayerHasArt(params);
    } else if (method == "LayerHasSelectedArt") {
        return LayerHasSelectedArt(params);
    } else if (method == "DeselectArtOnLayer") {
        return DeselectArtOnLayer(params);
    } else if (method == "SelectArtOnLayer") {
        return SelectArtOnLayer(params);
    } else if (method == "GetLayerIsTemplate") {
        return GetLayerIsTemplate(params);
    } else if (method == "SetLayerIsTemplate") {
        return SetLayerIsTemplate(params);
    } else if (method == "GetPrevLayer") {
        return GetPrevLayer(params);
    } else if (method == "GetLayerDimmedPercent") {
        return GetLayerDimmedPercent(params);
    } else if (method == "SetLayerDimmedPercent") {
        return SetLayerDimmedPercent(params);
    } else if (method == "GetLayerFirstChild") {
        return GetLayerFirstChild(params);
    } else if (method == "GetLayerParent") {
        return GetLayerParent(params);
    } else if (method == "InsertLayerAtArt") {
        return InsertLayerAtArt(params);
    } else if (method == "ChangeLayerToGroup") {
        return ChangeLayerToGroup(params);
    } else if (method == "GetNextPreorderLayer") {
        return GetNextPreorderLayer(params);
    } else if (method == "GetNextNonChildPreorderLayer") {
        return GetNextNonChildPreorderLayer(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AILayerSuite");
}

} // namespace AILayerSuite
} // namespace Flora
