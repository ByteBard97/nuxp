import type { Script, ScriptCategory } from './types'
import documentInfo from './scripts/document-info'
import objectCounter from './scripts/object-counter'
import listArtboards from './scripts/list-artboards'
import listLayers from './scripts/list-layers'
import toggleVisibility from './scripts/toggle-visibility'
import lockUnlockAll from './scripts/lock-unlock-all'

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
