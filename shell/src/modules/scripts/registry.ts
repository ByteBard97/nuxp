import type { Script, ScriptCategory } from './types'

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
