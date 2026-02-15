#include "FloraAIDocumentViewSuiteWrapper.h"
#include "HandleManager.hpp"
#include "IllustratorSDK.h"

// Suite accessor - provided by plugin infrastructure
extern "C" AIDocumentViewSuite* sDocumentView;

namespace Flora {
namespace AIDocumentViewSuite {

nlohmann::json GetDocumentViewBounds(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewBounds(view, &bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewBounds failed with error: " + std::to_string(err));
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

nlohmann::json GetDocumentViewCenter(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output struct: center
    AIRealPoint center{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewCenter(view, &center);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewCenter failed with error: " + std::to_string(err));
    }

    // Marshal output struct: center
    response["center"] = {
        {"h", center.h},
        {"v", center.v}
    };

    return response;
}

nlohmann::json SetDocumentViewCenter(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: center
    AIRealPoint center;
    center.h = params["center"]["h"].get<double>();
    center.v = params["center"]["v"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewCenter(view, &center);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewCenter failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentViewUserVisibleZoom(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: zoom
    AIReal zoom{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewUserVisibleZoom(view, &zoom);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewUserVisibleZoom failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: zoom
    response["zoom"] = zoom;

    return response;
}

nlohmann::json GetDocumentViewZoom(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: zoom
    AIReal zoom{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewZoom(view, &zoom);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewZoom failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: zoom
    response["zoom"] = zoom;

    return response;
}

nlohmann::json SetDocumentViewZoom(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: zoom
    AIReal zoom = params["zoom"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewZoom(view, zoom);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewZoom failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetDocumentViewUserVisibleZoom(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: zoom
    AIReal zoom = params["zoom"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewUserVisibleZoom(view, zoom);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewUserVisibleZoom failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ArtworkPointToViewPoint(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkPoint
    AIRealPoint artworkPoint;
    artworkPoint.h = params["artworkPoint"]["h"].get<double>();
    artworkPoint.v = params["artworkPoint"]["v"].get<double>();
    // Output unknown: viewPoint
    AIPoint viewPoint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ArtworkPointToViewPoint(view, &artworkPoint, &viewPoint);
    if (err != kNoErr) {
        throw std::runtime_error("ArtworkPointToViewPoint failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: viewPoint

    return response;
}

nlohmann::json CountDocumentViews(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->CountDocumentViews(&count);
    if (err != kNoErr) {
        throw std::runtime_error("CountDocumentViews failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json GetNthDocumentView(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: n
    ai::int32 n = params["n"].get<int32_t>();
    // Output unknown: view
    AIDocumentViewHandle view{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetNthDocumentView(n, &view);
    if (err != kNoErr) {
        throw std::runtime_error("GetNthDocumentView failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: view

    return response;
}

nlohmann::json FixedArtworkPointToViewPoint(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkPoint
    AIRealPoint artworkPoint;
    artworkPoint.h = params["artworkPoint"]["h"].get<double>();
    artworkPoint.v = params["artworkPoint"]["v"].get<double>();
    // Output struct: viewPoint
    AIRealPoint viewPoint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->FixedArtworkPointToViewPoint(view, &artworkPoint, &viewPoint);
    if (err != kNoErr) {
        throw std::runtime_error("FixedArtworkPointToViewPoint failed with error: " + std::to_string(err));
    }

    // Marshal output struct: viewPoint
    response["viewPoint"] = {
        {"h", viewPoint.h},
        {"v", viewPoint.v}
    };

    return response;
}

nlohmann::json FixedViewPointToArtworkPoint(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: viewPoint
    AIRealPoint viewPoint;
    viewPoint.h = params["viewPoint"]["h"].get<double>();
    viewPoint.v = params["viewPoint"]["v"].get<double>();
    // Output struct: artworkPoint
    AIRealPoint artworkPoint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->FixedViewPointToArtworkPoint(view, &viewPoint, &artworkPoint);
    if (err != kNoErr) {
        throw std::runtime_error("FixedViewPointToArtworkPoint failed with error: " + std::to_string(err));
    }

    // Marshal output struct: artworkPoint
    response["artworkPoint"] = {
        {"h", artworkPoint.h},
        {"v", artworkPoint.v}
    };

    return response;
}

nlohmann::json SetScreenMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Unknown type: mode (AIScreenMode)
    // WARNING: Using default initialization
    AIScreenMode mode{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetScreenMode(view, mode);
    if (err != kNoErr) {
        throw std::runtime_error("SetScreenMode failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetScreenMode(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output unknown: mode
    AIScreenMode mode{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetScreenMode(view, &mode);
    if (err != kNoErr) {
        throw std::runtime_error("GetScreenMode failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: mode

    return response;
}

nlohmann::json GetPageTiling(const nlohmann::json& params) {
    nlohmann::json response;

    // Output unknown: pageTiling
    AIPageTiling pageTiling{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetPageTiling(&pageTiling);
    if (err != kNoErr) {
        throw std::runtime_error("GetPageTiling failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: pageTiling

    return response;
}

nlohmann::json GetTemplateVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: visible
    AIBoolean visible{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetTemplateVisible(view, &visible);
    if (err != kNoErr) {
        throw std::runtime_error("GetTemplateVisible failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: visible
    response["visible"] = static_cast<bool>(visible);

    return response;
}

nlohmann::json DocumentViewScrollDelta(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output struct: delta
    AIRealPoint delta{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->DocumentViewScrollDelta(view, &delta);
    if (err != kNoErr) {
        throw std::runtime_error("DocumentViewScrollDelta failed with error: " + std::to_string(err));
    }

    // Marshal output struct: delta
    response["delta"] = {
        {"h", delta.h},
        {"v", delta.v}
    };

    return response;
}

nlohmann::json GetDocumentViewInvalidRect(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output struct: invalidRect
    AIRealRect invalidRect{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewInvalidRect(view, &invalidRect);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewInvalidRect failed with error: " + std::to_string(err));
    }

    // Marshal output struct: invalidRect
    response["invalidRect"] = {
        {"left", invalidRect.left},
        {"top", invalidRect.top},
        {"right", invalidRect.right},
        {"bottom", invalidRect.bottom}
    };

    return response;
}

nlohmann::json SetDocumentViewInvalidRect(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: invalidRect
    AIRealRect invalidRect;
    invalidRect.left = params["invalidRect"]["left"].get<double>();
    invalidRect.top = params["invalidRect"]["top"].get<double>();
    invalidRect.right = params["invalidRect"]["right"].get<double>();
    invalidRect.bottom = params["invalidRect"]["bottom"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewInvalidRect(view, &invalidRect);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewInvalidRect failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentViewStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: style
    ai::int16 style{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewStyle(view, &style);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewStyle failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: style
    response["style"] = style;

    return response;
}

nlohmann::json SetDocumentViewInvalidDocumentRect(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: invalidRect
    AIRealRect invalidRect;
    invalidRect.left = params["invalidRect"]["left"].get<double>();
    invalidRect.top = params["invalidRect"]["top"].get<double>();
    invalidRect.right = params["invalidRect"]["right"].get<double>();
    invalidRect.bottom = params["invalidRect"]["bottom"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewInvalidDocumentRect(view, &invalidRect);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewInvalidDocumentRect failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetShowPageTiling(const nlohmann::json& params) {
    nlohmann::json response;

    // Output primitive: show
    AIBoolean show{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetShowPageTiling(&show);
    if (err != kNoErr) {
        throw std::runtime_error("GetShowPageTiling failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: show
    response["show"] = static_cast<bool>(show);

    return response;
}

nlohmann::json SetShowPageTiling(const nlohmann::json& params) {
    nlohmann::json response;

    // Input primitive: show
    AIBoolean show = params["show"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetShowPageTiling(show);
    if (err != kNoErr) {
        throw std::runtime_error("SetShowPageTiling failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetGridOptions(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: show
    AIBoolean show{};
    // Output primitive: snap
    AIBoolean snap{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetGridOptions(view, &show, &snap);
    if (err != kNoErr) {
        throw std::runtime_error("GetGridOptions failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: show
    response["show"] = static_cast<bool>(show);
    // Marshal output primitive: snap
    response["snap"] = static_cast<bool>(snap);

    return response;
}

nlohmann::json SetGridOptions(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: show
    AIBoolean show = params["show"].get<bool>();
    // Input primitive: snap
    AIBoolean snap = params["snap"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetGridOptions(view, show, snap);
    if (err != kNoErr) {
        throw std::runtime_error("SetGridOptions failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetShowTransparencyGrid(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: show
    AIBoolean show{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetShowTransparencyGrid(view, &show);
    if (err != kNoErr) {
        throw std::runtime_error("GetShowTransparencyGrid failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: show
    response["show"] = static_cast<bool>(show);

    return response;
}

nlohmann::json SetShowTransparencyGrid(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: show
    AIBoolean show = params["show"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetShowTransparencyGrid(view, show);
    if (err != kNoErr) {
        throw std::runtime_error("SetShowTransparencyGrid failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentViewDocument(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output handle: document
    AIDocumentHandle document = nullptr;

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewDocument(view, &document);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewDocument failed with error: " + std::to_string(err));
    }

    // Marshal output handle: document
    if (document) {
        response["document"] = HandleManager::documents.Register(document);
    } else {
        response["document"] = -1;
    }

    return response;
}

nlohmann::json ForceDocumentViewsOnScreen(const nlohmann::json& params) {
    nlohmann::json response;


    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ForceDocumentViewsOnScreen();
    if (err != kNoErr) {
        throw std::runtime_error("ForceDocumentViewsOnScreen failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetShowGuides(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: show
    AIBoolean show{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetShowGuides(view, &show);
    if (err != kNoErr) {
        throw std::runtime_error("GetShowGuides failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: show
    response["show"] = static_cast<bool>(show);

    return response;
}

nlohmann::json SetShowGuides(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: show
    AIBoolean show = params["show"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetShowGuides(view, show);
    if (err != kNoErr) {
        throw std::runtime_error("SetShowGuides failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetShowEdges(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: show
    AIBoolean show{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetShowEdges(view, &show);
    if (err != kNoErr) {
        throw std::runtime_error("GetShowEdges failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: show
    response["show"] = static_cast<bool>(show);

    return response;
}

nlohmann::json SetShowEdges(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: show
    AIBoolean show = params["show"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetShowEdges(view, show);
    if (err != kNoErr) {
        throw std::runtime_error("SetShowEdges failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SaveImage(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input string: saveFilename
    ai::UnicodeString saveFilename(params["saveFilename"].get<std::string>());
    // Unknown type: windowSize (const AIPoint)
    // WARNING: Using default initialization
    const AIPoint windowSize{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SaveImage(view, saveFilename, windowSize);
    if (err != kNoErr) {
        throw std::runtime_error("SaveImage failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IsArtboardRulerVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (const AIDocumentViewHandle)
    // WARNING: Using default initialization
    const AIDocumentViewHandle view{};
    // Output primitive: visible
    AIBoolean visible{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->IsArtboardRulerVisible(view, &visible);
    if (err != kNoErr) {
        throw std::runtime_error("IsArtboardRulerVisible failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: visible
    response["visible"] = static_cast<bool>(visible);

    return response;
}

nlohmann::json SetArtboardRulerVisible(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (const AIDocumentViewHandle)
    // WARNING: Using default initialization
    const AIDocumentViewHandle view{};
    // Input primitive: visible
    AIBoolean visible = params["visible"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetArtboardRulerVisible(view, visible);
    if (err != kNoErr) {
        throw std::runtime_error("SetArtboardRulerVisible failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json CountOPPPlates(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: count
    ai::int32 count{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->CountOPPPlates(view, count);
    if (err != kNoErr) {
        throw std::runtime_error("CountOPPPlates failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: count
    response["count"] = count;

    return response;
}

nlohmann::json SetDocumentViewStyle(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: style
    ai::int16 style = params["style"].get<int16_t>();
    // Input primitive: mask
    ai::int16 mask = params["mask"].get<int16_t>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewStyle(view, style, mask);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewStyle failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IsRulerInArtboardCoordinates(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (const AIDocumentViewHandle)
    // WARNING: Using default initialization
    const AIDocumentViewHandle view{};
    // Output primitive: isYes
    ASBoolean isYes{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->IsRulerInArtboardCoordinates(view, &isYes);
    if (err != kNoErr) {
        throw std::runtime_error("IsRulerInArtboardCoordinates failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: isYes
    response["isYes"] = isYes;

    return response;
}

nlohmann::json UseArtboardCoordinatesInRuler(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (const AIDocumentViewHandle)
    // WARNING: Using default initialization
    const AIDocumentViewHandle view{};
    // Input primitive: state
    ASBoolean state = params["state"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->UseArtboardCoordinatesInRuler(view, state);
    if (err != kNoErr) {
        throw std::runtime_error("UseArtboardCoordinatesInRuler failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json IsGPUPreviewModeOn(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (const AIDocumentViewHandle)
    // WARNING: Using default initialization
    const AIDocumentViewHandle view{};

    // Call SDK function (returns boolean)
    AIBoolean result = sDocumentView->IsGPUPreviewModeOn(view);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json IsGPURenderingOn(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (const AIDocumentViewHandle)
    // WARNING: Using default initialization
    const AIDocumentViewHandle view{};

    // Call SDK function (returns boolean)
    AIBoolean result = sDocumentView->IsGPURenderingOn(view);
    response["result"] = static_cast<bool>(result);


    return response;
}

nlohmann::json GetDocumentViewVisibleArea(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output struct: bounds
    AIRealRect bounds{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewVisibleArea(view, &bounds);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewVisibleArea failed with error: " + std::to_string(err));
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

nlohmann::json SetClipViewToArtboards(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input primitive: clipToActiveArtboard
    AIBoolean clipToActiveArtboard = params["clipToActiveArtboard"].get<bool>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetClipViewToArtboards(view, clipToActiveArtboard);
    if (err != kNoErr) {
        throw std::runtime_error("SetClipViewToArtboards failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetClipViewToArtboards(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output primitive: clipToActiveArtboard
    AIBoolean clipToActiveArtboard{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetClipViewToArtboards(view, clipToActiveArtboard);
    if (err != kNoErr) {
        throw std::runtime_error("GetClipViewToArtboards failed with error: " + std::to_string(err));
    }

    // Marshal output primitive: clipToActiveArtboard
    response["clipToActiveArtboard"] = static_cast<bool>(clipToActiveArtboard);

    return response;
}

nlohmann::json ScreenShot(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input string: saveFilePath
    ai::UnicodeString saveFilePath(params["saveFilePath"].get<std::string>());

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ScreenShot(view, saveFilePath);
    if (err != kNoErr) {
        throw std::runtime_error("ScreenShot failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json SetDocumentViewRotation(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: rotationPoint
    AIRealPoint rotationPoint;
    rotationPoint.h = params["rotationPoint"]["h"].get<double>();
    rotationPoint.v = params["rotationPoint"]["v"].get<double>();
    // Input primitive: rotationAngle
    AIReal rotationAngle = params["rotationAngle"].get<double>();

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->SetDocumentViewRotation(view, rotationPoint, rotationAngle);
    if (err != kNoErr) {
        throw std::runtime_error("SetDocumentViewRotation failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json GetDocumentViewRotation(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Output struct: rotationPoint
    AIRealPoint rotationPoint{};
    // Output primitive: rotationAngle
    AIReal rotationAngle{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->GetDocumentViewRotation(view, rotationPoint, rotationAngle);
    if (err != kNoErr) {
        throw std::runtime_error("GetDocumentViewRotation failed with error: " + std::to_string(err));
    }

    // Marshal output struct: rotationPoint
    response["rotationPoint"] = {
        {"h", rotationPoint.h},
        {"v", rotationPoint.v}
    };
    // Marshal output primitive: rotationAngle
    response["rotationAngle"] = rotationAngle;

    return response;
}

nlohmann::json ResetDocumentViewRotation(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ResetDocumentViewRotation(view);
    if (err != kNoErr) {
        throw std::runtime_error("ResetDocumentViewRotation failed with error: " + std::to_string(err));
    }


    return response;
}

nlohmann::json ArtworkPointToViewPointUnrotated(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkPoint
    AIRealPoint artworkPoint;
    artworkPoint.h = params["artworkPoint"]["h"].get<double>();
    artworkPoint.v = params["artworkPoint"]["v"].get<double>();
    // Output unknown: viewPoint
    AIPoint viewPoint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ArtworkPointToViewPointUnrotated(view, &artworkPoint, &viewPoint);
    if (err != kNoErr) {
        throw std::runtime_error("ArtworkPointToViewPointUnrotated failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: viewPoint

    return response;
}

nlohmann::json ArtworkRectToViewRect(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkRect
    AIRealRect artworkRect;
    artworkRect.left = params["artworkRect"]["left"].get<double>();
    artworkRect.top = params["artworkRect"]["top"].get<double>();
    artworkRect.right = params["artworkRect"]["right"].get<double>();
    artworkRect.bottom = params["artworkRect"]["bottom"].get<double>();
    // Output unknown: viewRect
    AIRect viewRect{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ArtworkRectToViewRect(view, &artworkRect, &viewRect);
    if (err != kNoErr) {
        throw std::runtime_error("ArtworkRectToViewRect failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: viewRect

    return response;
}

nlohmann::json ArtworkRectToViewRectUnrotated(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkRect
    AIRealRect artworkRect;
    artworkRect.left = params["artworkRect"]["left"].get<double>();
    artworkRect.top = params["artworkRect"]["top"].get<double>();
    artworkRect.right = params["artworkRect"]["right"].get<double>();
    artworkRect.bottom = params["artworkRect"]["bottom"].get<double>();
    // Output unknown: viewRect
    AIRect viewRect{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->ArtworkRectToViewRectUnrotated(view, &artworkRect, &viewRect);
    if (err != kNoErr) {
        throw std::runtime_error("ArtworkRectToViewRectUnrotated failed with error: " + std::to_string(err));
    }

    // Unable to marshal unknown type: viewRect

    return response;
}

nlohmann::json FixedArtworkPointToViewPointUnrotated(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkPoint
    AIRealPoint artworkPoint;
    artworkPoint.h = params["artworkPoint"]["h"].get<double>();
    artworkPoint.v = params["artworkPoint"]["v"].get<double>();
    // Output struct: viewPoint
    AIRealPoint viewPoint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->FixedArtworkPointToViewPointUnrotated(view, &artworkPoint, &viewPoint);
    if (err != kNoErr) {
        throw std::runtime_error("FixedArtworkPointToViewPointUnrotated failed with error: " + std::to_string(err));
    }

    // Marshal output struct: viewPoint
    response["viewPoint"] = {
        {"h", viewPoint.h},
        {"v", viewPoint.v}
    };

    return response;
}

nlohmann::json FixedViewPointToArtworkPointUnrotated(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: viewPoint
    AIRealPoint viewPoint;
    viewPoint.h = params["viewPoint"]["h"].get<double>();
    viewPoint.v = params["viewPoint"]["v"].get<double>();
    // Output struct: artworkPoint
    AIRealPoint artworkPoint{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->FixedViewPointToArtworkPointUnrotated(view, &viewPoint, &artworkPoint);
    if (err != kNoErr) {
        throw std::runtime_error("FixedViewPointToArtworkPointUnrotated failed with error: " + std::to_string(err));
    }

    // Marshal output struct: artworkPoint
    response["artworkPoint"] = {
        {"h", artworkPoint.h},
        {"v", artworkPoint.v}
    };

    return response;
}

nlohmann::json FixedViewRectToArtworkRectUnrotated(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: viewRect
    AIRealRect viewRect;
    viewRect.left = params["viewRect"]["left"].get<double>();
    viewRect.top = params["viewRect"]["top"].get<double>();
    viewRect.right = params["viewRect"]["right"].get<double>();
    viewRect.bottom = params["viewRect"]["bottom"].get<double>();
    // Output struct: artworkRect
    AIRealRect artworkRect{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->FixedViewRectToArtworkRectUnrotated(view, &viewRect, &artworkRect);
    if (err != kNoErr) {
        throw std::runtime_error("FixedViewRectToArtworkRectUnrotated failed with error: " + std::to_string(err));
    }

    // Marshal output struct: artworkRect
    response["artworkRect"] = {
        {"left", artworkRect.left},
        {"top", artworkRect.top},
        {"right", artworkRect.right},
        {"bottom", artworkRect.bottom}
    };

    return response;
}

nlohmann::json FixedArtworkRectToViewRectUnrotated(const nlohmann::json& params) {
    nlohmann::json response;

    // Unknown type: view (AIDocumentViewHandle)
    // WARNING: Using default initialization
    AIDocumentViewHandle view{};
    // Input struct: artworkRect
    AIRealRect artworkRect;
    artworkRect.left = params["artworkRect"]["left"].get<double>();
    artworkRect.top = params["artworkRect"]["top"].get<double>();
    artworkRect.right = params["artworkRect"]["right"].get<double>();
    artworkRect.bottom = params["artworkRect"]["bottom"].get<double>();
    // Output struct: viewRect
    AIRealRect viewRect{};

    // Call SDK function (returns AIErr)
    AIErr err = sDocumentView->FixedArtworkRectToViewRectUnrotated(view, &artworkRect, &viewRect);
    if (err != kNoErr) {
        throw std::runtime_error("FixedArtworkRectToViewRectUnrotated failed with error: " + std::to_string(err));
    }

    // Marshal output struct: viewRect
    response["viewRect"] = {
        {"left", viewRect.left},
        {"top", viewRect.top},
        {"right", viewRect.right},
        {"bottom", viewRect.bottom}
    };

    return response;
}

nlohmann::json Dispatch(const std::string& method, const nlohmann::json& params) {
    if (method == "GetDocumentViewBounds") {
        return GetDocumentViewBounds(params);
    } else if (method == "GetDocumentViewCenter") {
        return GetDocumentViewCenter(params);
    } else if (method == "SetDocumentViewCenter") {
        return SetDocumentViewCenter(params);
    } else if (method == "GetDocumentViewUserVisibleZoom") {
        return GetDocumentViewUserVisibleZoom(params);
    } else if (method == "GetDocumentViewZoom") {
        return GetDocumentViewZoom(params);
    } else if (method == "SetDocumentViewZoom") {
        return SetDocumentViewZoom(params);
    } else if (method == "SetDocumentViewUserVisibleZoom") {
        return SetDocumentViewUserVisibleZoom(params);
    } else if (method == "ArtworkPointToViewPoint") {
        return ArtworkPointToViewPoint(params);
    } else if (method == "CountDocumentViews") {
        return CountDocumentViews(params);
    } else if (method == "GetNthDocumentView") {
        return GetNthDocumentView(params);
    } else if (method == "FixedArtworkPointToViewPoint") {
        return FixedArtworkPointToViewPoint(params);
    } else if (method == "FixedViewPointToArtworkPoint") {
        return FixedViewPointToArtworkPoint(params);
    } else if (method == "SetScreenMode") {
        return SetScreenMode(params);
    } else if (method == "GetScreenMode") {
        return GetScreenMode(params);
    } else if (method == "GetPageTiling") {
        return GetPageTiling(params);
    } else if (method == "GetTemplateVisible") {
        return GetTemplateVisible(params);
    } else if (method == "DocumentViewScrollDelta") {
        return DocumentViewScrollDelta(params);
    } else if (method == "GetDocumentViewInvalidRect") {
        return GetDocumentViewInvalidRect(params);
    } else if (method == "SetDocumentViewInvalidRect") {
        return SetDocumentViewInvalidRect(params);
    } else if (method == "GetDocumentViewStyle") {
        return GetDocumentViewStyle(params);
    } else if (method == "SetDocumentViewInvalidDocumentRect") {
        return SetDocumentViewInvalidDocumentRect(params);
    } else if (method == "GetShowPageTiling") {
        return GetShowPageTiling(params);
    } else if (method == "SetShowPageTiling") {
        return SetShowPageTiling(params);
    } else if (method == "GetGridOptions") {
        return GetGridOptions(params);
    } else if (method == "SetGridOptions") {
        return SetGridOptions(params);
    } else if (method == "GetShowTransparencyGrid") {
        return GetShowTransparencyGrid(params);
    } else if (method == "SetShowTransparencyGrid") {
        return SetShowTransparencyGrid(params);
    } else if (method == "GetDocumentViewDocument") {
        return GetDocumentViewDocument(params);
    } else if (method == "ForceDocumentViewsOnScreen") {
        return ForceDocumentViewsOnScreen(params);
    } else if (method == "GetShowGuides") {
        return GetShowGuides(params);
    } else if (method == "SetShowGuides") {
        return SetShowGuides(params);
    } else if (method == "GetShowEdges") {
        return GetShowEdges(params);
    } else if (method == "SetShowEdges") {
        return SetShowEdges(params);
    } else if (method == "SaveImage") {
        return SaveImage(params);
    } else if (method == "IsArtboardRulerVisible") {
        return IsArtboardRulerVisible(params);
    } else if (method == "SetArtboardRulerVisible") {
        return SetArtboardRulerVisible(params);
    } else if (method == "CountOPPPlates") {
        return CountOPPPlates(params);
    } else if (method == "SetDocumentViewStyle") {
        return SetDocumentViewStyle(params);
    } else if (method == "IsRulerInArtboardCoordinates") {
        return IsRulerInArtboardCoordinates(params);
    } else if (method == "UseArtboardCoordinatesInRuler") {
        return UseArtboardCoordinatesInRuler(params);
    } else if (method == "IsGPUPreviewModeOn") {
        return IsGPUPreviewModeOn(params);
    } else if (method == "IsGPURenderingOn") {
        return IsGPURenderingOn(params);
    } else if (method == "GetDocumentViewVisibleArea") {
        return GetDocumentViewVisibleArea(params);
    } else if (method == "SetClipViewToArtboards") {
        return SetClipViewToArtboards(params);
    } else if (method == "GetClipViewToArtboards") {
        return GetClipViewToArtboards(params);
    } else if (method == "ScreenShot") {
        return ScreenShot(params);
    } else if (method == "SetDocumentViewRotation") {
        return SetDocumentViewRotation(params);
    } else if (method == "GetDocumentViewRotation") {
        return GetDocumentViewRotation(params);
    } else if (method == "ResetDocumentViewRotation") {
        return ResetDocumentViewRotation(params);
    } else if (method == "ArtworkPointToViewPointUnrotated") {
        return ArtworkPointToViewPointUnrotated(params);
    } else if (method == "ArtworkRectToViewRect") {
        return ArtworkRectToViewRect(params);
    } else if (method == "ArtworkRectToViewRectUnrotated") {
        return ArtworkRectToViewRectUnrotated(params);
    } else if (method == "FixedArtworkPointToViewPointUnrotated") {
        return FixedArtworkPointToViewPointUnrotated(params);
    } else if (method == "FixedViewPointToArtworkPointUnrotated") {
        return FixedViewPointToArtworkPointUnrotated(params);
    } else if (method == "FixedViewRectToArtworkRectUnrotated") {
        return FixedViewRectToArtworkRectUnrotated(params);
    } else if (method == "FixedArtworkRectToViewRectUnrotated") {
        return FixedArtworkRectToViewRectUnrotated(params);
    }
    throw std::runtime_error("Unknown method: " + method + " in AIDocumentViewSuite");
}

} // namespace AIDocumentViewSuite
} // namespace Flora
