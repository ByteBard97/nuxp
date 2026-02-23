import type { Script, ScriptCategory } from './types'
import documentInfo from './scripts/document-info'
import objectCounter from './scripts/object-counter'
import listArtboards from './scripts/list-artboards'
import listLayers from './scripts/list-layers'
import toggleVisibility from './scripts/toggle-visibility'
import lockUnlockAll from './scripts/lock-unlock-all'
import selectionInfo from './scripts/selection-info'
import deselectAll from './scripts/deselect-all'
import renameSelected from './scripts/rename-selected'
import changeOpacity from './scripts/change-opacity'
import duplicateSelected from './scripts/duplicate-selected'
import listTextFrames from './scripts/list-text-frames'
import createTextFrame from './scripts/create-text-frame'
import fitArtboard from './scripts/fit-artboard'
import fitSelection from './scripts/fit-selection'

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

// Register document scripts
registerScript(documentInfo)
registerScript(objectCounter)
registerScript(listArtboards)

// Register layers scripts
registerScript(listLayers)
registerScript(toggleVisibility)
registerScript(lockUnlockAll)

// Register selection scripts
registerScript(selectionInfo)
registerScript(deselectAll)

// Register objects scripts
registerScript(renameSelected)
registerScript(changeOpacity)
registerScript(duplicateSelected)

// Register text scripts
registerScript(listTextFrames)
registerScript(createTextFrame)

// Register view scripts
registerScript(fitArtboard)
registerScript(fitSelection)
