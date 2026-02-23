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
  document: '\u{1F4C4}',
  layers: '\u{1F4DA}',
  selection: '\u{270F}\u{FE0F}',
  objects: '\u{1F537}',
  text: '\u{1F4DD}',
  view: '\u{1F441}\u{FE0F}',
}
