# Script Toolkit Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add a "Script Toolkit" view to the NUXP shell app that lets users run 15 Illustrator operations from a UI, demonstrating the SDK's TypeScript-first developer experience.

**Architecture:** New sidebar view following the existing DebugPanel pattern. Scripts are TypeScript modules with a standard interface (Script/ScriptResult). A registry collects all scripts; the UI renders them as runnable cards grouped by category. No new C++ or custom routes needed.

**Tech Stack:** Vue 3 Composition API, existing UI components (Button, Panel), generated SDK functions + custom routes.

**Design Doc:** `docs/plans/2026-02-15-script-toolkit-design.md`

---

### Task 1: Clean up existing examples

The `examples/` directory has outdated code (references a nonexistent `batch-operations.js`, uses the old `callSdk('demo', ...)` pattern). Clean it up before adding the new toolkit.

**Files:**
- Delete: `examples/basic-api.js`
- Modify: `examples/README.md`

**Step 1: Replace examples/README.md**

Rewrite to point users at the Script Toolkit instead of standalone scripts:

```markdown
# NUXP Examples

## Script Toolkit (Recommended)

The best way to see NUXP in action is the built-in Script Toolkit. Run:

\`\`\`bash
cd shell
npm run dev
\`\`\`

Click "Scripts" in the sidebar to access 15 ready-to-run operations covering document info, layers, selection, text, and view control.

## Using the HTTP API Directly

NUXP exposes a standard HTTP/JSON API on localhost:8080. You can call it from any language:

### JavaScript / TypeScript

\`\`\`javascript
const response = await fetch('http://localhost:8080/api/doc/info')
const docInfo = await response.json()
console.log(docInfo)
\`\`\`

### Python

\`\`\`python
import requests

response = requests.get('http://localhost:8080/api/doc/info')
print(response.json())
\`\`\`

### curl

\`\`\`bash
# Document info
curl http://localhost:8080/api/doc/info

# Health check
curl http://localhost:8080/health

# List layers
curl http://localhost:8080/api/query/layers

# Call a generated SDK function
curl -X POST http://localhost:8080/api/call \
  -H "Content-Type: application/json" \
  -d '{"suite":"AILayerSuite","method":"CountLayers","args":{}}'
\`\`\`

See the [API Reference](../docs/api/README.md) for all available endpoints.
```

**Step 2: Delete basic-api.js**

```bash
mv examples/basic-api.js ~/.Trash/
```

**Step 3: Commit**

```bash
git add examples/
git commit -m "docs: replace examples with Script Toolkit reference and REST-style API examples"
```

---

### Task 2: Create types and registry

**Files:**
- Create: `shell/src/modules/scripts/types.ts`
- Create: `shell/src/modules/scripts/registry.ts`

**Step 1: Create types.ts**

```typescript
// shell/src/modules/scripts/types.ts

export type ScriptCategory = 'document' | 'layers' | 'selection' | 'objects' | 'text' | 'view'

export interface ScriptParam {
  name: string
  label: string
  type: 'number' | 'string' | 'boolean'
  default?: unknown
  min?: number
  max?: number
  step?: number
}

export interface ScriptResult {
  success: boolean
  message: string
  data?: unknown
}

export interface Script {
  id: string
  name: string
  description: string
  category: ScriptCategory
  inspiredBy?: string
  params?: ScriptParam[]
  run: (params: Record<string, unknown>) => Promise<ScriptResult>
}

export const CATEGORY_LABELS: Record<ScriptCategory, string> = {
  document: 'Document',
  layers: 'Layers',
  selection: 'Selection',
  objects: 'Objects',
  text: 'Text',
  view: 'View',
}

export const CATEGORY_ICONS: Record<ScriptCategory, string> = {
  document: '📄',
  layers: '📚',
  selection: '✏️',
  objects: '🔷',
  text: '📝',
  view: '👁️',
}
```

**Step 2: Create empty registry.ts** (scripts will be added in later tasks)

```typescript
// shell/src/modules/scripts/registry.ts
import type { Script, ScriptCategory } from './types'

// Scripts are registered here as they're implemented
const scripts: Script[] = []

export function getAllScripts(): Script[] {
  return scripts
}

export function getScriptsByCategory(category: ScriptCategory): Script[] {
  return scripts.filter(s => s.category === category)
}

export function getCategories(): ScriptCategory[] {
  const cats = new Set(scripts.map(s => s.category))
  return Array.from(cats)
}

export function registerScript(script: Script): void {
  scripts.push(script)
}
```

**Step 3: Verify TypeScript compiles**

```bash
cd shell && npx vue-tsc --noEmit 2>&1 | head -20
```

**Step 4: Commit**

```bash
git add shell/src/modules/scripts/
git commit -m "feat(scripts): add types and registry for Script Toolkit"
```

---

### Task 3: Create ScriptCard component

**Files:**
- Create: `shell/src/components/ScriptCard.vue`

**Step 1: Create ScriptCard.vue**

This is a card that displays one script — name, description, optional param inputs, run button, and result area. Uses existing UI components (Button).

```vue
<script setup lang="ts">
import { ref, reactive } from 'vue'
import type { Script, ScriptResult } from '@/modules/scripts/types'
import { Button } from '@/components/ui'

const props = defineProps<{
  script: Script
}>()

const running = ref(false)
const result = ref<ScriptResult | null>(null)
const paramValues = reactive<Record<string, unknown>>({})

// Initialize param defaults
if (props.script.params) {
  for (const p of props.script.params) {
    paramValues[p.name] = p.default ?? (p.type === 'number' ? 0 : p.type === 'boolean' ? false : '')
  }
}

async function runScript() {
  running.value = true
  result.value = null
  try {
    result.value = await props.script.run({ ...paramValues })
  } catch (err) {
    result.value = {
      success: false,
      message: err instanceof Error ? err.message : String(err),
    }
  } finally {
    running.value = false
  }
}
</script>

<template>
  <div class="script-card">
    <div class="script-header">
      <div class="script-info">
        <h4 class="script-name">{{ script.name }}</h4>
        <p class="script-desc">{{ script.description }}</p>
        <p v-if="script.inspiredBy" class="script-inspired">
          Inspired by: {{ script.inspiredBy }}
        </p>
      </div>
      <Button
        variant="primary"
        size="sm"
        :loading="running"
        @click="runScript"
      >
        Run
      </Button>
    </div>

    <!-- Parameters -->
    <div v-if="script.params?.length" class="script-params">
      <div v-for="param in script.params" :key="param.name" class="param-row">
        <label class="param-label">{{ param.label }}</label>
        <input
          v-if="param.type === 'string'"
          v-model="paramValues[param.name]"
          type="text"
          class="param-input"
        />
        <input
          v-else-if="param.type === 'number'"
          v-model.number="paramValues[param.name]"
          type="number"
          class="param-input param-input--number"
          :min="param.min"
          :max="param.max"
          :step="param.step ?? 1"
        />
        <label v-else-if="param.type === 'boolean'" class="param-checkbox">
          <input
            v-model="paramValues[param.name]"
            type="checkbox"
          />
          {{ param.label }}
        </label>
      </div>
    </div>

    <!-- Result -->
    <div v-if="result" class="script-result" :class="result.success ? 'result-ok' : 'result-err'">
      <div class="result-message">{{ result.message }}</div>
      <pre v-if="result.data" class="result-data">{{ JSON.stringify(result.data, null, 2) }}</pre>
    </div>
  </div>
</template>

<style scoped>
.script-card {
  padding: var(--spacing-md);
  background-color: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-lg);
  margin-bottom: var(--spacing-sm);
}

.script-header {
  display: flex;
  align-items: flex-start;
  justify-content: space-between;
  gap: var(--spacing-md);
}

.script-info { flex: 1; }

.script-name {
  margin: 0;
  font-size: var(--font-size-sm);
  font-weight: 600;
  color: var(--text-bright);
}

.script-desc {
  margin: var(--spacing-xs) 0 0;
  font-size: var(--font-size-xs);
  color: var(--text-secondary);
}

.script-inspired {
  margin: var(--spacing-xs) 0 0;
  font-size: var(--font-size-xs);
  color: var(--text-muted);
  font-style: italic;
}

.script-params {
  margin-top: var(--spacing-md);
  padding-top: var(--spacing-sm);
  border-top: 1px solid var(--border-color);
  display: flex;
  flex-direction: column;
  gap: var(--spacing-sm);
}

.param-row {
  display: flex;
  align-items: center;
  gap: var(--spacing-md);
}

.param-label {
  font-size: var(--font-size-xs);
  color: var(--text-secondary);
  min-width: 80px;
}

.param-input {
  flex: 1;
  padding: var(--spacing-xs) var(--spacing-sm);
  background-color: var(--bg-primary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
  color: var(--text-primary);
  font-size: var(--font-size-xs);
  font-family: inherit;
}

.param-input--number { max-width: 100px; }

.param-input:focus {
  outline: none;
  border-color: var(--accent-blue);
}

.script-result {
  margin-top: var(--spacing-md);
  padding: var(--spacing-sm) var(--spacing-md);
  border-radius: var(--radius-md);
  font-size: var(--font-size-xs);
}

.result-ok {
  background-color: rgba(60, 154, 95, 0.15);
  border: 1px solid var(--accent-green);
  color: var(--accent-green);
}

.result-err {
  background-color: rgba(231, 76, 60, 0.15);
  border: 1px solid var(--accent-red);
  color: var(--accent-red);
}

.result-message { font-weight: 500; }

.result-data {
  margin: var(--spacing-sm) 0 0;
  padding: var(--spacing-sm);
  background-color: var(--bg-primary);
  border-radius: var(--radius-sm);
  color: var(--text-primary);
  font-size: var(--font-size-xs);
  overflow-x: auto;
  max-height: 200px;
  overflow-y: auto;
  white-space: pre-wrap;
}
</style>
```

**Step 2: Commit**

```bash
git add shell/src/components/ScriptCard.vue
git commit -m "feat(scripts): add ScriptCard component"
```

---

### Task 4: Create ScriptToolkit view and wire into app

**Files:**
- Create: `shell/src/components/ScriptToolkit.vue`
- Modify: `shell/src/App.vue` (add import + conditional render)
- Modify: `shell/src/components/Sidebar.vue` (add nav item)

**Step 1: Create ScriptToolkit.vue**

```vue
<script setup lang="ts">
import { ref, computed } from 'vue'
import ScriptCard from '@/components/ScriptCard.vue'
import { getAllScripts, getCategories } from '@/modules/scripts/registry'
import { CATEGORY_LABELS, CATEGORY_ICONS } from '@/modules/scripts/types'
import type { ScriptCategory } from '@/modules/scripts/types'

const activeCategory = ref<ScriptCategory | null>(null)
const categories = computed(() => getCategories())
const scripts = computed(() => {
  if (!activeCategory.value) return getAllScripts()
  return getAllScripts().filter(s => s.category === activeCategory.value)
})

// Select first category on mount
if (categories.value.length > 0) {
  activeCategory.value = categories.value[0]
}
</script>

<template>
  <div class="toolkit">
    <!-- Category tabs -->
    <div class="category-bar">
      <button
        v-for="cat in categories"
        :key="cat"
        class="category-tab"
        :class="{ active: activeCategory === cat }"
        @click="activeCategory = cat"
      >
        <span class="cat-icon">{{ CATEGORY_ICONS[cat] }}</span>
        {{ CATEGORY_LABELS[cat] }}
      </button>
    </div>

    <!-- Scripts list -->
    <div class="scripts-list">
      <div v-if="scripts.length === 0" class="empty-state">
        No scripts registered yet.
      </div>
      <ScriptCard
        v-for="script in scripts"
        :key="script.id"
        :script="script"
      />
    </div>
  </div>
</template>

<style scoped>
.toolkit {
  height: 100%;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.category-bar {
  display: flex;
  gap: var(--spacing-xs);
  padding: var(--spacing-md);
  border-bottom: 1px solid var(--border-color);
  background-color: var(--bg-secondary);
  overflow-x: auto;
  flex-shrink: 0;
}

.category-tab {
  display: flex;
  align-items: center;
  gap: var(--spacing-xs);
  padding: var(--spacing-xs) var(--spacing-md);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
  background-color: transparent;
  color: var(--text-secondary);
  font-size: var(--font-size-xs);
  cursor: pointer;
  white-space: nowrap;
  transition: all 0.15s ease;
}

.category-tab:hover {
  background-color: var(--bg-hover);
  color: var(--text-primary);
}

.category-tab.active {
  background-color: var(--bg-active);
  color: var(--text-bright);
  border-color: var(--accent-blue);
}

.cat-icon { font-size: var(--font-size-sm); }

.scripts-list {
  flex: 1;
  overflow-y: auto;
  padding: var(--spacing-md);
}

.empty-state {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 200px;
  color: var(--text-muted);
  font-size: var(--font-size-sm);
}
</style>
```

**Step 2: Add nav item in Sidebar.vue**

In `shell/src/components/Sidebar.vue`, add a "Scripts" entry to `navItems` computed array, after the "debug" entry:

```typescript
  {
    id: 'scripts',
    label: 'Scripts',
    icon: '⚡',
    badge: null,
  },
```

**Step 3: Wire into App.vue**

In `shell/src/App.vue`:
- Add import: `import ScriptToolkit from '@/components/ScriptToolkit.vue';`
- Add conditional render after the `<DocumentStatus v-else-if="currentView === 'selection'" />` line:
  ```html
  <ScriptToolkit v-else-if="currentView === 'scripts'" />
  ```

**Step 4: Verify it renders**

```bash
cd shell && npm run dev
```

Open http://localhost:5173, click "Scripts" in sidebar. Should show category bar (empty) and "No scripts registered yet." message.

**Step 5: Commit**

```bash
git add shell/src/components/ScriptToolkit.vue shell/src/components/Sidebar.vue shell/src/App.vue
git commit -m "feat(scripts): add ScriptToolkit view with category tabs and sidebar nav"
```

---

### Task 5: Implement Document scripts (3 scripts)

**Files:**
- Create: `shell/src/modules/scripts/scripts/document-info.ts`
- Create: `shell/src/modules/scripts/scripts/object-counter.ts`
- Create: `shell/src/modules/scripts/scripts/list-artboards.ts`
- Modify: `shell/src/modules/scripts/registry.ts` (register scripts)

**Step 1: document-info.ts**

```typescript
import type { Script } from '../types'
import { GetDocumentInfo } from '@/sdk/generated/customRoutes'
import { GetDocumentRulerUnits, GetDocumentModified } from '@/sdk/generated/AIDocumentSuite'

const documentInfo: Script = {
  id: 'document-info',
  name: 'Document Info',
  description: 'Show document name, size, ruler units, and modification state',
  category: 'document',
  inspiredBy: 'ObjectsCounter.jsx (creold)',
  async run() {
    const info = await GetDocumentInfo()
    let rulerUnits = 'unknown'
    try {
      const units = await GetDocumentRulerUnits()
      const unitNames: Record<number, string> = { 0: 'inches', 1: 'centimeters', 2: 'points', 3: 'picas', 4: 'millimeters', 5: 'pixels' }
      rulerUnits = unitNames[units] ?? `unit ${units}`
    } catch { /* may not be available */ }

    let modified = false
    try { modified = await GetDocumentModified() } catch { /* ignore */ }

    return {
      success: true,
      message: `Document: ${info.name ?? 'Untitled'}`,
      data: { ...info, rulerUnits, modified },
    }
  },
}

export default documentInfo
```

**Step 2: object-counter.ts**

```typescript
import type { Script } from '../types'
import { QueryPathItems, QueryTextFrames, QueryLayers } from '@/sdk/generated/customRoutes'

const objectCounter: Script = {
  id: 'object-counter',
  name: 'Object Counter',
  description: 'Count objects by type — paths, text frames, layers',
  category: 'document',
  inspiredBy: 'ObjectsCounter.jsx (creold)',
  async run() {
    const [paths, textFrames, layers] = await Promise.all([
      QueryPathItems(),
      QueryTextFrames(),
      QueryLayers(),
    ])

    const counts = {
      pathItems: paths.count ?? paths.items?.length ?? 0,
      textFrames: textFrames.count ?? textFrames.items?.length ?? 0,
      layers: layers.count ?? layers.items?.length ?? 0,
    }
    const total = counts.pathItems + counts.textFrames

    return {
      success: true,
      message: `${total} objects across ${counts.layers} layers`,
      data: counts,
    }
  },
}

export default objectCounter
```

**Step 3: list-artboards.ts**

```typescript
import type { Script } from '../types'
import { GetCount, GetNthArtboardProperties, GetArtboardName } from '@/sdk/generated/AIArtboardSuite'

const listArtboards: Script = {
  id: 'list-artboards',
  name: 'List Artboards',
  description: 'Show all artboards with names and dimensions',
  category: 'document',
  inspiredBy: 'ArtboardsFinder.jsx (creold)',
  async run() {
    const count = await GetCount()
    const artboards = []

    for (let i = 0; i < count; i++) {
      let name = `Artboard ${i + 1}`
      try { name = await GetArtboardName(i) } catch { /* ignore */ }
      artboards.push({ index: i, name })
    }

    return {
      success: true,
      message: `${count} artboard(s)`,
      data: artboards,
    }
  },
}

export default listArtboards
```

**Step 4: Register in registry.ts**

Add imports and `registerScript()` calls:

```typescript
import documentInfo from './scripts/document-info'
import objectCounter from './scripts/object-counter'
import listArtboards from './scripts/list-artboards'

registerScript(documentInfo)
registerScript(objectCounter)
registerScript(listArtboards)
```

**Step 5: Verify**

```bash
cd shell && npm run dev
```

Click "Scripts" > "Document" category. Three cards should appear.

**Step 6: Commit**

```bash
git add shell/src/modules/scripts/
git commit -m "feat(scripts): add Document category — info, counter, artboards"
```

---

### Task 6: Implement Layers scripts (3 scripts)

**Files:**
- Create: `shell/src/modules/scripts/scripts/list-layers.ts`
- Create: `shell/src/modules/scripts/scripts/toggle-visibility.ts`
- Create: `shell/src/modules/scripts/scripts/lock-unlock-all.ts`
- Modify: `shell/src/modules/scripts/registry.ts`

**Step 1: list-layers.ts**

```typescript
import type { Script } from '../types'
import { CountLayers, GetNthLayer, GetLayerTitle, GetLayerVisible, GetLayerEditable } from '@/sdk/generated/AILayerSuite'

const listLayers: Script = {
  id: 'list-layers',
  name: 'List Layers',
  description: 'Show all layers with visibility and lock state',
  category: 'layers',
  async run() {
    const count = await CountLayers()
    const layers = []

    for (let i = 0; i < count; i++) {
      const handle = await GetNthLayer(i)
      const title = await GetLayerTitle(handle)
      const visible = await GetLayerVisible(handle)
      const editable = await GetLayerEditable(handle)
      layers.push({
        index: i,
        title,
        visible,
        locked: !editable,
      })
    }

    return {
      success: true,
      message: `${count} layer(s)`,
      data: layers,
    }
  },
}

export default listLayers
```

**Step 2: toggle-visibility.ts**

```typescript
import type { Script } from '../types'
import { CountLayers, GetNthLayer, GetLayerVisible, SetLayerVisible } from '@/sdk/generated/AILayerSuite'

const toggleVisibility: Script = {
  id: 'toggle-visibility',
  name: 'Toggle All Visibility',
  description: 'Show or hide all layers at once',
  category: 'layers',
  inspiredBy: 'Layer visibility scripts (sky-chaser)',
  params: [
    { name: 'visible', label: 'Visible', type: 'boolean', default: true },
  ],
  async run(params) {
    const visible = params.visible as boolean
    const count = await CountLayers()

    for (let i = 0; i < count; i++) {
      const handle = await GetNthLayer(i)
      await SetLayerVisible(handle, visible)
    }

    return {
      success: true,
      message: `Set ${count} layer(s) to ${visible ? 'visible' : 'hidden'}`,
    }
  },
}

export default toggleVisibility
```

**Step 3: lock-unlock-all.ts**

```typescript
import type { Script } from '../types'
import { CountLayers, GetNthLayer, SetLayerEditable } from '@/sdk/generated/AILayerSuite'

const lockUnlockAll: Script = {
  id: 'lock-unlock-all',
  name: 'Lock/Unlock All Layers',
  description: 'Lock or unlock all layers at once',
  category: 'layers',
  params: [
    { name: 'lock', label: 'Lock', type: 'boolean', default: false },
  ],
  async run(params) {
    const lock = params.lock as boolean
    const count = await CountLayers()

    for (let i = 0; i < count; i++) {
      const handle = await GetNthLayer(i)
      await SetLayerEditable(handle, !lock)
    }

    return {
      success: true,
      message: `${lock ? 'Locked' : 'Unlocked'} ${count} layer(s)`,
    }
  },
}

export default lockUnlockAll
```

**Step 4: Register in registry.ts**

```typescript
import listLayers from './scripts/list-layers'
import toggleVisibility from './scripts/toggle-visibility'
import lockUnlockAll from './scripts/lock-unlock-all'

registerScript(listLayers)
registerScript(toggleVisibility)
registerScript(lockUnlockAll)
```

**Step 5: Commit**

```bash
git add shell/src/modules/scripts/
git commit -m "feat(scripts): add Layers category — list, visibility toggle, lock/unlock"
```

---

### Task 7: Implement Selection + Objects + Text + View scripts (9 scripts)

**Files:**
- Create: `shell/src/modules/scripts/scripts/selection-info.ts`
- Create: `shell/src/modules/scripts/scripts/deselect-all.ts`
- Create: `shell/src/modules/scripts/scripts/rename-selected.ts`
- Create: `shell/src/modules/scripts/scripts/change-opacity.ts`
- Create: `shell/src/modules/scripts/scripts/duplicate-selected.ts`
- Create: `shell/src/modules/scripts/scripts/list-text-frames.ts`
- Create: `shell/src/modules/scripts/scripts/create-text-frame.ts`
- Create: `shell/src/modules/scripts/scripts/fit-artboard.ts`
- Create: `shell/src/modules/scripts/scripts/fit-selection.ts`
- Modify: `shell/src/modules/scripts/registry.ts`

Each script follows the same pattern. Key SDK calls:

| Script | SDK Function(s) |
|--------|----------------|
| selection-info | `GetSelection()` from customRoutes |
| deselect-all | `DeselectAll()` from customRoutes |
| rename-selected | `GetSelection()`, `SetArtName(art, name)` from AIArtSuite |
| change-opacity | `GetSelection()`, `SetOpacity(art, opacity)` from AIBlendStyleSuite |
| duplicate-selected | `GetSelection()`, `DuplicateArt(art, paintOrder, prep)` from AIArtSuite |
| list-text-frames | `QueryTextFrames()`, `GetTextContent(id)` from customRoutes |
| create-text-frame | `CreateTextFrame(params)` from customRoutes |
| fit-artboard | `FitArtboardInView()` from customRoutes |
| fit-selection | `FitSelectionInView()` from customRoutes |

Scripts with params:
- **rename-selected**: `prefix` (string, default "NUXP_")
- **change-opacity**: `opacity` (number, 0-100, default 50)
- **create-text-frame**: `text` (string, default "Hello NUXP"), `x` (number, default 100), `y` (number, default -200)

**Step 1:** Create all 9 script files following the patterns from Tasks 5-6.

**Step 2:** Register all 9 in registry.ts.

**Step 3: Verify**

```bash
cd shell && npm run dev
```

All 6 categories should appear with 15 total scripts.

**Step 4: Commit**

```bash
git add shell/src/modules/scripts/
git commit -m "feat(scripts): add Selection, Objects, Text, View categories — 15 scripts total"
```

---

### Task 8: Type-check and verify build

**Step 1: Type-check**

```bash
cd shell && npm run type-check
```

Fix any TypeScript errors. Common issues:
- SDK function signatures may differ from what's assumed — check `shell/src/sdk/generated/` for exact types
- Custom route response types may need adjustment

**Step 2: Build**

```bash
cd shell && npm run build
```

Verify production build succeeds.

**Step 3: Commit any fixes**

```bash
git add -A
git commit -m "fix(scripts): resolve type-check issues"
```

---

### Task 9: Update README and push

**Files:**
- Modify: `README.md` — add mention of Script Toolkit in Quick Start or Development Workflows
- Modify: `docs/index.md` — add link to Script Toolkit

**Step 1: Add to README**

After the "Frontend Development (Mock Mode)" section, add:

```markdown
### Script Toolkit

The shell includes a built-in Script Toolkit with 15 ready-to-run operations inspired by popular ExtendScript scripts. Click "Scripts" in the sidebar to try them:

- **Document**: Info, object counter, artboard list
- **Layers**: List, visibility toggle, lock/unlock
- **Selection**: Info, deselect all
- **Objects**: Rename, opacity, duplicate
- **Text**: List frames, create frame
- **View**: Fit artboard, fit selection
```

**Step 2: Commit and push**

```bash
git add README.md docs/index.md examples/
git commit -m "feat: Script Toolkit — 15 Illustrator operations runnable from the UI

Inspired by popular ExtendScript repos (creold, sky-chaser, ladygin).
Shows NUXP can replace common .jsx workflows from TypeScript."
git push
```
