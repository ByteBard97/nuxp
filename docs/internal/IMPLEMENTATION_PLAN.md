# Implementation Plan - Phase 6: Flora App Migration

**Goal**: Port the legacy Flora application logic and UI to the new NUXP architecture, replacing `CSInterface` calls with the typed `sdk` and `bridge`.

## User Review Required
> [!NOTE]
> We are skipping Binary Data Support (Task 17) as effectively "YAGNI" (You Ain't Gonna Need It) for now. The community can add it if needed.

## Proposed Changes

### Data Layer (Task 18)

#### [NEW] [plantStore.ts](file:///Users/ceres/Desktop/flora/nuxp/shell/src/modules/flora/stores/plantStore.ts)
- Pinia store to manage Plant state.
- Actions:
  - `createPlant(name: string)`: Calls `AIArtSuite.NewArt`, `SetArtName`.
  - `growPlant(handle: number)`: Calls `AIArtSuite.TransformArt` (scaling).
  - `syncFromSelection()`: Reads current selection, checks if Art is a "Plant" (via UserAttributes or name convention), and populates state.

#### [NEW] [Plant.ts](file:///Users/ceres/Desktop/flora/nuxp/shell/src/modules/flora/models/Plant.ts)
- Domain model wrapping an `AIArtHandle`.
- Methods: `grow()`, `wither()`, `rename()`.

### UI Layer (Task 19)

#### [NEW] [FloraPanel.vue](file:///Users/ceres/Desktop/flora/nuxp/shell/src/modules/flora/components/FloraPanel.vue)
- Main view for the Flora app.
- Lists current plants.
- "Add Plant" button.
- Controls for selected plant (Grow/Shrink).
- Uses `variables.css` for valid NUXP styling.

#### [MODIFY] [App.vue](file:///Users/ceres/Desktop/flora/nuxp/shell/src/App.vue)
- Add "Flora" to the navigation sidebar / routing.

## Verification Plan

### Manual Verification (Task 20)
1.  **Launch**: Run NUXP Shell + Illustrator.
2.  **Create**: Click "Add Plant" -> Verify rectangle appears in Illustrator.
3.  **Interact**: Click "Grow" -> Verify rectangle scales up in Illustrator.
4.  **Reflect**: Rename rectangle in Illustrator -> Verify name updates in Flora Panel (via Events).
