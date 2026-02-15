# NUXP — Remaining Work

Consolidated from previous planning docs (Feb 2026). Only items that are actually incomplete are listed.

---

## Deferred — BlendStyle Enum Functions (Low Priority)

**Status:** 14 functions blocked by enum parameter types

The `AIBlendStyleSuite` has 14 functions still on the blocklist because they use enum types (`AIBlendingMode`, `AIKnockout`) that the codegen doesn't yet map to integers. The underlying handle types (`AIArtStyleHandle`, `AIDictionaryRef`) are fully working — the remaining blocker is purely enum support.

**Blocked functions:** `SetBlendingMode`, `SetKnockout`, `Get/SetBlendStyleAttrs`, `Get/SetFocalFillBlendStyleAttrs`, `Get/SetFocalStrokeBlendStyleAttrs`, `Get/SetBlendStyleAllAttrs`, `Get/SetFocalBlendStyleAllAttrs`, `Get/SetFocalFillBlendStyleAllAttrs`

**To unblock:** Add enum type mappings to `codegen/src/config/type-map.json` (map `AIBlendingMode` and `AIKnockout` to `int32_t`), then remove these functions from `BLOCKED_FUNCTIONS` in `codegen/src/index.ts`.

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
- AIArtStyleHandle support (6 BlendStyle functions working: Get/SetStyleAttrs, focal variants)
- CustomRouteGenerator (fully implemented)
- All codegen Tier 1-2 type fixes
- TypeScript client tests (37 tests for all 30 custom route functions)
- Codegen: void parameter filtering, shared struct types (no duplicate exports)
