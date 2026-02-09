import { exists, readTextFile, writeTextFile, mkdir, remove, BaseDirectory } from '@tauri-apps/plugin-fs';

const APP_DIR = 'nuxp';

export async function ensureAppDir(): Promise<void> {
  const dirExists = await exists(APP_DIR, { baseDir: BaseDirectory.AppData });
  if (!dirExists) {
    await mkdir(APP_DIR, { baseDir: BaseDirectory.AppData, recursive: true });
  }
}

export async function writeFile(filename: string, content: string): Promise<void> {
  await ensureAppDir();
  await writeTextFile(`${APP_DIR}/${filename}`, content, { baseDir: BaseDirectory.AppData });
}

export async function readFile(filename: string): Promise<string | null> {
  const path = `${APP_DIR}/${filename}`;
  const fileExists = await exists(path, { baseDir: BaseDirectory.AppData });
  if (!fileExists) return null;
  return readTextFile(path, { baseDir: BaseDirectory.AppData });
}

export async function fileExists(filename: string): Promise<boolean> {
  return exists(`${APP_DIR}/${filename}`, { baseDir: BaseDirectory.AppData });
}

export async function deleteFile(filename: string): Promise<void> {
  await remove(`${APP_DIR}/${filename}`, { baseDir: BaseDirectory.AppData });
}
