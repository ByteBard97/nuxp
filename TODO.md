# NUXP — Remaining Work

Consolidated from previous planning docs (Feb 2026). Only items that are actually incomplete are listed.

---

## P1 — Document Info Endpoint

**Status:** Route defined, handler not yet implemented

Aggregation endpoint that returns document name (no extension), directory path, saved state, and artboard dimensions in both points and inches.

- Route: `GET /api/doc/info`
- SDK calls: AIDocumentSuite (filename, path, modified), AIArtboardSuite (bounds)
- Handler: `HandleGetDocumentInfo` in NUXPHandlers.cpp

## P2 — View Control Endpoints

**Status:** Routes defined, handlers not yet implemented

Programmatic viewport control via AIDocumentViewSuite.

| Route | Description |
|-------|-------------|
| `GET /api/view/zoom` | Get current zoom level |
| `POST /api/view/zoom` | Set zoom level |
| `POST /api/view/center` | Center view on a point |
| `POST /api/view/fit-artboard` | Zoom to fit active artboard |
| `POST /api/view/fit-selection` | Zoom to fit current selection |

## Deferred — AIArtStyleHandle (Tier 3 Codegen)

**Status:** Low priority, ~8 BlendStyle focal functions blocked

Only ~8 functions depend on this. The codegen change requires adding an `artStyles` registry to HandleManager and extending TypeClassifier. Most affected functions also need AIDictionaryRef (which is already working). Defer unless specifically needed.

---

## Completed (reference)

- Text Frame Creation (`POST /api/text/create`)
- XMP Metadata Read/Write (GET/POST/dump/property endpoints)
- Artboard Bounds Checking (`POST /api/artboard/check-bounds`)
- Document Query Endpoints (text-frames, layers, find-by-name)
- Path Area Calculation (`GET /api/art/{id}/area`)
- Art Deselection (`POST /api/selection/deselect-all`, `POST /api/selection/select`)
- AIDictionaryRef/AIEntryRef handles (37 dict + 24 entry functions generating)
- CustomRouteGenerator (fully implemented)
- All codegen Tier 1-2 type fixes
