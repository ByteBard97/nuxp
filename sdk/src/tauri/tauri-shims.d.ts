/**
 * Ambient type declarations for optional Tauri peer dependencies.
 *
 * These stubs allow TypeScript to compile when the @tauri-apps packages
 * are not installed. At runtime, these modules are only imported inside
 * Tauri builds where the real packages are available.
 */

declare module '@tauri-apps/plugin-fs' {
  export enum BaseDirectory {
    Audio = 1,
    Cache,
    Config,
    Data,
    LocalData,
    Document,
    Download,
    Picture,
    Public,
    Video,
    Resource,
    Temp,
    AppConfig,
    AppData,
    AppLocalData,
    AppCache,
    AppLog,
    Desktop,
    Executable,
    Font,
    Home,
    Runtime,
    Template,
  }

  interface FsOptions {
    baseDir?: BaseDirectory
  }

  interface MkdirOptions extends FsOptions {
    recursive?: boolean
  }

  export function exists(path: string, options?: FsOptions): Promise<boolean>
  export function mkdir(path: string, options?: MkdirOptions): Promise<void>
  export function readTextFile(path: string, options?: FsOptions): Promise<string>
  export function writeTextFile(path: string, content: string, options?: FsOptions): Promise<void>
  export function writeFile(path: string, data: Uint8Array, options?: FsOptions): Promise<void>
  export function remove(path: string, options?: FsOptions): Promise<void>
}

declare module '@tauri-apps/plugin-dialog' {
  export interface SaveDialogOptions {
    title?: string
    defaultPath?: string
    filters?: Array<{ name: string; extensions: string[] }>
  }
  export function save(options?: SaveDialogOptions): Promise<string | null>
}

declare module '@tauri-apps/api/path' {
  export function appDataDir(): Promise<string>
}
