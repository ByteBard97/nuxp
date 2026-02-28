/**
 * Tauri Filesystem Wrapper
 *
 * Provides a unified interface for Tauri filesystem operations.
 * Uses dynamic imports so the module resolves even when @tauri-apps
 * packages are not installed (they are optional peer dependencies).
 */

let _tauriFs: typeof import('@tauri-apps/plugin-fs') | undefined
let _tauriPath: typeof import('@tauri-apps/api/path') | undefined

async function loadTauriFs() {
  if (!_tauriFs) _tauriFs = await import('@tauri-apps/plugin-fs')
  return _tauriFs
}

async function loadTauriPath() {
  if (!_tauriPath) _tauriPath = await import('@tauri-apps/api/path')
  return _tauriPath
}

/**
 * Lazy accessors for Tauri modules (for consumers that need the raw APIs).
 */
export const tauriFs = new Proxy({} as typeof import('@tauri-apps/plugin-fs'), {
  get(_target, prop) {
    return async (...args: unknown[]) => {
      const mod = await loadTauriFs()
      return (mod as any)[prop](...args)
    }
  }
})

export const tauriPath = new Proxy({} as typeof import('@tauri-apps/api/path'), {
  get(_target, prop) {
    return async (...args: unknown[]) => {
      const mod = await loadTauriPath()
      return (mod as any)[prop](...args)
    }
  }
})

/**
 * Check if Tauri fs is available
 */
export function isTauriFsAvailable(): boolean {
  try {
    return typeof (window as any).__TAURI__ !== 'undefined'
  } catch {
    return false
  }
}

/**
 * Get the app data directory
 */
export async function getAppDataDir(): Promise<string> {
  const path = await loadTauriPath()
  return await path.appDataDir()
}

/**
 * Check if a file/directory exists
 */
export async function exists(path: string): Promise<boolean> {
  const fs = await loadTauriFs()
  return await fs.exists(path)
}

/**
 * Create a directory (recursive)
 */
export async function mkdir(path: string): Promise<void> {
  const fs = await loadTauriFs()
  await fs.mkdir(path, { recursive: true })
}

/**
 * Read a text file
 */
export async function readTextFile(path: string): Promise<string> {
  const fs = await loadTauriFs()
  return await fs.readTextFile(path)
}

/**
 * Write a text file
 */
export async function writeTextFile(path: string, content: string): Promise<void> {
  const fs = await loadTauriFs()
  await fs.writeTextFile(path, content)
}

/**
 * Remove a file
 */
export async function remove(path: string): Promise<void> {
  const fs = await loadTauriFs()
  await fs.remove(path)
}
