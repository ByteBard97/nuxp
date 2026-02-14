# NUXP — Remaining Work

Consolidated from previous planning docs (Feb 2026). Only items that are actually incomplete are listed.

---

## Deferred — AIArtStyleHandle (Tier 3 Codegen)

**Status:** Low priority, ~8 BlendStyle focal functions blocked

Only ~8 functions depend on this. The codegen change requires adding an `artStyles` registry to HandleManager and extending TypeClassifier. Most affected functions also need AIDictionaryRef (which is already working). Defer unless specifically needed.

---

## Completed (reference)

- Document Info Endpoint (`GET /api/doc/info`)
- View Control Endpoints (zoom, center, fit-artboard, fit-selection)
- Query Endpoints (path-items, count-items-on-layer, text-frames, layers, find-by-name)
- Text Frame Creation (`POST /api/text/create`)
- XMP Metadata Read/Write (GET/POST/dump/property endpoints)
- Artboard Bounds Checking (`POST /api/artboard/check-bounds`)
- Path Area Calculation (`GET /api/art/{id}/area`)
- Art Deselection (`POST /api/selection/deselect-all`, `POST /api/selection/select`)
- AIDictionaryRef/AIEntryRef handles (37 dict + 24 entry functions generating)
- CustomRouteGenerator (fully implemented)
- All codegen Tier 1-2 type fixes
- TypeScript client tests (37 tests for all 30 custom route functions)
- Codegen: void parameter filtering, shared struct types (no duplicate exports)
