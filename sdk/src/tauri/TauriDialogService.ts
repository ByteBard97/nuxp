/**
 * TauriDialogService
 *
 * Provides native file save dialogs via Tauri's dialog plugin,
 * with browser fallback for development mode (npm run dev without Tauri).
 *
 * Two main functions:
 * - saveTextFile: Save text content (CSV, JSON, JSONL, etc.)
 * - saveBinaryFile: Save binary content from base64 string (XLSX, etc.)
 */

const isTauri = typeof window !== 'undefined' && '__TAURI__' in window

/**
 * Result from a save dialog operation
 */
export interface SaveDialogResult {
  /** Whether the file was saved successfully */
  success: boolean
  /** The path the file was saved to (Tauri only) */
  path?: string
  /** Whether the user cancelled the dialog */
  cancelled?: boolean
  /** Error message if save failed */
  error?: string
}

/**
 * Filter definition for save dialogs
 */
export interface FileFilter {
  /** Display name for the filter (e.g., "CSV Files") */
  name: string
  /** File extensions without dots (e.g., ["csv"]) */
  extensions: string[]
}

/**
 * Save a text file with a native save dialog (Tauri) or browser download (fallback).
 *
 * @param content - The text content to save
 * @param defaultFilename - Suggested filename for the dialog
 * @param title - Dialog window title (Tauri only)
 * @param filters - File type filters for the dialog
 * @returns Result indicating success, cancellation, or error
 */
export async function saveTextFile(
  content: string,
  defaultFilename: string,
  title: string,
  filters?: FileFilter[],
): Promise<SaveDialogResult> {
  if (isTauri) {
    return saveTextFileViaTauri(content, defaultFilename, title, filters)
  }
  return saveTextFileViaBrowser(content, defaultFilename)
}

/**
 * Save a binary file from base64-encoded content with a native save dialog (Tauri)
 * or browser download (fallback).
 *
 * @param base64Content - The base64-encoded binary content
 * @param defaultFilename - Suggested filename for the dialog
 * @param title - Dialog window title (Tauri only)
 * @param filters - File type filters for the dialog
 * @returns Result indicating success, cancellation, or error
 */
export async function saveBinaryFile(
  base64Content: string,
  defaultFilename: string,
  title: string,
  filters?: FileFilter[],
): Promise<SaveDialogResult> {
  if (isTauri) {
    return saveBinaryFileViaTauri(base64Content, defaultFilename, title, filters)
  }
  return saveBinaryFileViaBrowser(base64Content, defaultFilename)
}

// ---------------------------------------------------------------------------
// Tauri implementations
// ---------------------------------------------------------------------------

async function saveTextFileViaTauri(
  content: string,
  defaultFilename: string,
  title: string,
  filters?: FileFilter[],
): Promise<SaveDialogResult> {
  try {
    const { save } = await import('@tauri-apps/plugin-dialog')
    const { writeTextFile } = await import('@tauri-apps/plugin-fs')

    const filePath = await save({
      title,
      defaultPath: defaultFilename,
      filters: filters ?? inferFilters(defaultFilename),
    })

    if (!filePath) {
      console.log('File save cancelled by user')
      return { success: false, cancelled: true }
    }

    await writeTextFile(filePath, content)
    console.log(`File saved to: ${filePath}`)
    return { success: true, path: filePath }
  } catch (error: any) {
    const message = error?.message || String(error)
    console.error(`Failed to save file: ${message}`)
    return { success: false, error: message }
  }
}

async function saveBinaryFileViaTauri(
  base64Content: string,
  defaultFilename: string,
  title: string,
  filters?: FileFilter[],
): Promise<SaveDialogResult> {
  try {
    const { save } = await import('@tauri-apps/plugin-dialog')
    const { writeFile } = await import('@tauri-apps/plugin-fs')

    const filePath = await save({
      title,
      defaultPath: defaultFilename,
      filters: filters ?? inferFilters(defaultFilename),
    })

    if (!filePath) {
      console.log('File save cancelled by user')
      return { success: false, cancelled: true }
    }

    // Decode base64 to Uint8Array
    const binaryString = atob(base64Content)
    const bytes = new Uint8Array(binaryString.length)
    for (let i = 0; i < binaryString.length; i++) {
      bytes[i] = binaryString.charCodeAt(i)
    }

    await writeFile(filePath, bytes)
    console.log(`Binary file saved to: ${filePath}`)
    return { success: true, path: filePath }
  } catch (error: any) {
    const message = error?.message || String(error)
    console.error(`Failed to save binary file: ${message}`)
    return { success: false, error: message }
  }
}

// ---------------------------------------------------------------------------
// Browser fallback implementations (blob + anchor download)
// ---------------------------------------------------------------------------

function saveTextFileViaBrowser(
  content: string,
  defaultFilename: string,
): SaveDialogResult {
  try {
    const mimeType = guessMimeType(defaultFilename)
    const blob = new Blob([content], { type: mimeType })
    triggerBrowserDownload(blob, defaultFilename)
    return { success: true }
  } catch (error: any) {
    const message = error?.message || String(error)
    return { success: false, error: message }
  }
}

function saveBinaryFileViaBrowser(
  base64Content: string,
  defaultFilename: string,
): SaveDialogResult {
  try {
    const binaryString = atob(base64Content)
    const bytes = new Uint8Array(binaryString.length)
    for (let i = 0; i < binaryString.length; i++) {
      bytes[i] = binaryString.charCodeAt(i)
    }

    const mimeType = guessMimeType(defaultFilename)
    const blob = new Blob([bytes], { type: mimeType })
    triggerBrowserDownload(blob, defaultFilename)
    return { success: true }
  } catch (error: any) {
    const message = error?.message || String(error)
    return { success: false, error: message }
  }
}

function triggerBrowserDownload(blob: Blob, filename: string): void {
  const url = URL.createObjectURL(blob)
  const a = document.createElement('a')
  a.href = url
  a.download = filename
  a.click()
  URL.revokeObjectURL(url)
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/**
 * Infer dialog file filters from the filename extension.
 */
function inferFilters(filename: string): FileFilter[] {
  const ext = filename.split('.').pop()?.toLowerCase()
  switch (ext) {
    case 'csv':
      return [{ name: 'CSV Files', extensions: ['csv'] }]
    case 'json':
      return [{ name: 'JSON Files', extensions: ['json'] }]
    case 'jsonl':
      return [{ name: 'JSON Lines Files', extensions: ['jsonl'] }]
    case 'xlsx':
      return [{ name: 'Excel Files', extensions: ['xlsx'] }]
    case 'txt':
      return [{ name: 'Text Files', extensions: ['txt'] }]
    default:
      return [{ name: 'All Files', extensions: ['*'] }]
  }
}

/**
 * Guess MIME type from filename extension (for browser fallback).
 */
function guessMimeType(filename: string): string {
  const ext = filename.split('.').pop()?.toLowerCase()
  switch (ext) {
    case 'csv':
      return 'text/csv;charset=utf-8;'
    case 'json':
      return 'application/json;charset=utf-8;'
    case 'jsonl':
      return 'text/plain;charset=utf-8;'
    case 'xlsx':
      return 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'
    case 'txt':
      return 'text/plain;charset=utf-8;'
    default:
      return 'application/octet-stream'
  }
}
