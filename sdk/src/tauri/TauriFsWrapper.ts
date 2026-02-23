/**
 * Tauri Filesystem Wrapper
 *
 * Provides a unified interface for Tauri filesystem operations.
 * This module is only imported in Tauri builds.
 */

// Static imports - these will be bundled by Vite for Tauri builds
import * as tauriFs from '@tauri-apps/plugin-fs';
import * as tauriPath from '@tauri-apps/api/path';

export { tauriFs, tauriPath };

/**
 * Check if Tauri fs is available
 */
export function isTauriFsAvailable(): boolean {
  return typeof tauriFs !== 'undefined' && typeof tauriPath !== 'undefined';
}

/**
 * Get the app data directory
 */
export async function getAppDataDir(): Promise<string> {
  return await tauriPath.appDataDir();
}

/**
 * Check if a file/directory exists
 */
export async function exists(path: string): Promise<boolean> {
  return await tauriFs.exists(path);
}

/**
 * Create a directory (recursive)
 */
export async function mkdir(path: string): Promise<void> {
  await tauriFs.mkdir(path, { recursive: true });
}

/**
 * Read a text file
 */
export async function readTextFile(path: string): Promise<string> {
  return await tauriFs.readTextFile(path);
}

/**
 * Write a text file
 */
export async function writeTextFile(path: string, content: string): Promise<void> {
  await tauriFs.writeTextFile(path, content);
}

/**
 * Remove a file
 */
export async function remove(path: string): Promise<void> {
  await tauriFs.remove(path);
}
