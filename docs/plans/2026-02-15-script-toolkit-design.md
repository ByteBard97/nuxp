# Script Toolkit — Design

A new view in the NUXP shell app that presents 10-15 Illustrator operations organized by category, each runnable with one click. Inspired by the most popular ExtendScript script collections (creold/illustrator-scripts, sky-chaser-high/adobe-illustrator-scripts, alexander-ladygin/illustrator-scripts).

The goal: show that NUXP can replace common ExtendScript workflows, entirely from TypeScript.

## Location

Built into the existing shell app as a sidebar-based view (same pattern as DebugPanel). Users see it immediately when they run `npm run dev`.

- Add `"scripts"` nav item to `Sidebar.vue`
- Add `<ScriptToolkit v-if="currentView === 'scripts'" />` to `App.vue`
- Script toolkit component lives in `components/ScriptToolkit.vue`
- Individual script implementations in `modules/scripts/`

## Layout

Sidebar categories on the left (within the toolkit, not the app sidebar). Main area shows script cards for the selected category. Each card has:

- Script name and one-line description
- "Inspired by: ScriptName.jsx" attribution where applicable
- Optional parameter inputs (slider for opacity, text input for rename prefix, etc.)
- Run button
- Result area showing formatted output or success/error state

## Scripts (10-15)

### Document (3)
| Script | Description | SDK |
|--------|-------------|-----|
| Document Info | Show name, size, ruler units, modification state | `GetDocumentInfo()`, `AIDocumentSuite` |
| Object Counter | Walk art tree, count objects by type (paths, text, groups, etc.) | `AIArtSuite` (GetFirstArtOfLayer, GetArtSibling, GetArtType) |
| List Artboards | Show all artboards with dimensions | `AIArtboardSuite` |

### Layers (3)
| Script | Description | SDK |
|--------|-------------|-----|
| List Layers | Show all layers with visibility, lock, print state | `AILayerSuite`, `QueryLayers()` |
| Toggle All Visibility | Show/hide all layers at once | `AILayerSuite` (SetLayerVisible) |
| Lock/Unlock All | Lock or unlock all layers | `AILayerSuite` (SetLayerEditable) |

### Selection (2)
| Script | Description | SDK |
|--------|-------------|-----|
| Selection Info | Show what's selected — count, types, bounds | `GetSelection()`, `AIArtSuite` |
| Deselect All | Clear selection | `DeselectAll()` |

### Objects (3)
| Script | Description | SDK |
|--------|-------------|-----|
| Rename Selected | Set name on selected items with user-provided prefix | `AIArtSuite` (SetArtName) |
| Change Opacity | Set opacity on selected items (slider input) | `AIBlendStyleSuite` (SetOpacity) |
| Duplicate Selected | Duplicate selected art objects | `AIArtSuite` (DuplicateArt) |

### Text (2)
| Script | Description | SDK |
|--------|-------------|-----|
| List Text Frames | Find all text frames, show their content | `QueryTextFrames()`, `GetTextContent()` |
| Create Text Frame | Create a new text frame with user-provided text and position | `CreateTextFrame()` |

### View (2)
| Script | Description | SDK |
|--------|-------------|-----|
| Fit Artboard | Zoom to fit current artboard | `FitArtboardInView()` |
| Fit Selection | Zoom to fit current selection | `FitSelectionInView()` |

Total: 15 scripts across 6 categories.

## Script Implementation Pattern

Each script is a TypeScript module exporting a standard interface:

```typescript
interface Script {
  id: string
  name: string
  description: string
  category: 'document' | 'layers' | 'selection' | 'objects' | 'text' | 'view'
  inspiratedBy?: string  // e.g., "ObjectsCounter.jsx (creold)"
  params?: ScriptParam[]
  run: (params?: Record<string, unknown>) => Promise<ScriptResult>
}

interface ScriptParam {
  name: string
  label: string
  type: 'number' | 'string' | 'boolean'
  default?: unknown
  min?: number
  max?: number
}

interface ScriptResult {
  success: boolean
  message: string
  data?: unknown
}
```

## File Structure

```
shell/src/
  modules/scripts/
    types.ts                 # Script, ScriptParam, ScriptResult interfaces
    registry.ts              # All scripts registered here
    scripts/
      document-info.ts
      object-counter.ts
      list-artboards.ts
      list-layers.ts
      toggle-visibility.ts
      lock-unlock-all.ts
      selection-info.ts
      deselect-all.ts
      rename-selected.ts
      change-opacity.ts
      duplicate-selected.ts
      list-text-frames.ts
      create-text-frame.ts
      fit-artboard.ts
      fit-selection.ts
  components/
    ScriptToolkit.vue        # Main view component
    ScriptCard.vue           # Individual script card
```

## Mock Mode

All scripts should work in mock mode (VITE_USE_MOCK=true) by returning simulated data through the existing MockBridge. This lets users explore the toolkit UI without Illustrator running.

## What This Doesn't Include

- No new C++ code needed
- No custom routes needed (all operations use existing generated functions + custom routes)
- No new dependencies
