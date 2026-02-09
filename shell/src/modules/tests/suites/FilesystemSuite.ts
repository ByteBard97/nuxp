import type { TestSuite, TestResult } from '../types';
import { writeFile, readFile, fileExists, deleteFile } from '@/services/filesystem';

const TEST_CONFIG = {
  testKey: 'nuxp-test',
  timestamp: Date.now(),
  nested: { value: 42 }
};

async function writeConfig(): Promise<TestResult> {
  try {
    const content = JSON.stringify(TEST_CONFIG, null, 2);
    await writeFile('test-config.json', content);
    return {
      success: true,
      message: 'Config written to $APPDATA/nuxp/test-config.json',
      data: TEST_CONFIG
    };
  } catch (error) {
    return {
      success: false,
      error: error instanceof Error ? error.message : 'Write failed'
    };
  }
}

async function readConfig(): Promise<TestResult> {
  try {
    const content = await readFile('test-config.json');
    if (!content) {
      return { success: false, error: 'File not found or empty' };
    }

    const parsed = JSON.parse(content) as typeof TEST_CONFIG;
    const matches = parsed.testKey === TEST_CONFIG.testKey;

    return {
      success: matches,
      message: matches ? 'Content verified' : 'Content mismatch',
      data: parsed
    };
  } catch (error) {
    return {
      success: false,
      error: error instanceof Error ? error.message : 'Read failed'
    };
  }
}

async function checkExists(): Promise<TestResult> {
  try {
    const exists = await fileExists('test-config.json');
    return {
      success: true,
      message: exists ? 'File exists' : 'File does not exist',
      data: { exists }
    };
  } catch (error) {
    return {
      success: false,
      error: error instanceof Error ? error.message : 'Check failed'
    };
  }
}

async function removeTestFile(): Promise<TestResult> {
  try {
    await deleteFile('test-config.json');

    // Verify deletion
    const stillExists = await fileExists('test-config.json');

    return {
      success: !stillExists,
      message: stillExists ? 'File still exists after delete' : 'File deleted successfully',
      data: { deleted: !stillExists }
    };
  } catch (error) {
    return {
      success: false,
      error: error instanceof Error ? error.message : 'Delete failed'
    };
  }
}

export const FilesystemSuite: TestSuite = {
  id: 'filesystem',
  name: 'Filesystem (Tauri)',
  icon: '📁',
  tests: [
    {
      id: 'fs-write',
      name: 'Write Config',
      description: 'Write a JSON object to $APPDATA/nuxp/test-config.json',
      run: writeConfig
    },
    {
      id: 'fs-read',
      name: 'Read Config',
      description: 'Read and verify the config file contents',
      run: readConfig
    },
    {
      id: 'fs-exists',
      name: 'Check Exists',
      description: 'Verify the file exists on disk',
      run: checkExists
    },
    {
      id: 'fs-delete',
      name: 'Delete File',
      description: 'Remove the test file and verify deletion',
      run: removeTestFile
    }
  ]
};

export default FilesystemSuite;
