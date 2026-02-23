<script setup lang="ts">
/**
 * Tests View
 *
 * Wrapper view for the Test Runner with sample test suites.
 * Navigate to /tests to view this page.
 */

import { TestRunner } from '@/modules/tests';
import type { TestSuite } from '@/modules/tests';

// Sample test suites for demonstration
// These will be replaced with actual tests as the QA app develops
const testSuites: TestSuite[] = [
  {
    id: 'system',
    name: 'System',
    icon: '⚙️',
    tests: [
      {
        id: 'system-health',
        name: 'Plugin Health Check',
        description: 'Verify plugin server is running and responding',
        run: async () => {
          // Placeholder - will connect to actual health check
          await new Promise((r) => setTimeout(r, 500));
          return { success: true, message: 'Plugin server is healthy', data: { status: 'ok' } };
        },
      },
      {
        id: 'system-version',
        name: 'Version Info',
        description: 'Get plugin and Illustrator version information',
        run: async () => {
          await new Promise((r) => setTimeout(r, 300));
          return {
            success: true,
            message: 'Version retrieved',
            data: { plugin: '1.0.0', illustrator: '28.0', sdk: '2024' },
          };
        },
      },
    ],
  },
  {
    id: 'filesystem',
    name: 'Filesystem',
    icon: '📁',
    tests: [
      {
        id: 'fs-write',
        name: 'Write File',
        description: 'Test writing a file to app data directory',
        run: async () => {
          await new Promise((r) => setTimeout(r, 400));
          return { success: true, message: 'File written successfully' };
        },
      },
      {
        id: 'fs-read',
        name: 'Read File',
        description: 'Test reading a file from app data directory',
        run: async () => {
          await new Promise((r) => setTimeout(r, 350));
          return { success: true, message: 'File read successfully', data: { size: 1024 } };
        },
      },
      {
        id: 'fs-exists',
        name: 'File Exists',
        description: 'Test checking if a file exists',
        run: async () => {
          await new Promise((r) => setTimeout(r, 200));
          return { success: true, message: 'File existence check passed', data: { exists: true } };
        },
      },
    ],
  },
  {
    id: 'art',
    name: 'Art',
    icon: '🎨',
    tests: [
      {
        id: 'art-list',
        name: 'List Art Objects',
        description: 'Get all art objects in the current document',
        run: async () => {
          await new Promise((r) => setTimeout(r, 600));
          return {
            success: true,
            message: 'Retrieved 5 art objects',
            data: { count: 5, types: ['path', 'text', 'group'] },
          };
        },
      },
      {
        id: 'art-bounds',
        name: 'Get Art Bounds',
        description: 'Get bounding box of selected art',
        run: async () => {
          await new Promise((r) => setTimeout(r, 450));
          return {
            success: true,
            message: 'Bounds retrieved',
            data: { top: 0, left: 0, bottom: 100, right: 200 },
          };
        },
      },
    ],
  },
  {
    id: 'hierarchy',
    name: 'Hierarchy',
    icon: '🏗️',
    tests: [
      {
        id: 'hierarchy-layers',
        name: 'List Layers',
        description: 'Get all layers in the current document',
        run: async () => {
          await new Promise((r) => setTimeout(r, 500));
          return {
            success: true,
            message: 'Found 3 layers',
            data: { layers: ['Background', 'Artwork', 'Text'] },
          };
        },
      },
      {
        id: 'hierarchy-parent',
        name: 'Get Parent Art',
        description: 'Get parent of selected art object',
        run: async () => {
          await new Promise((r) => setTimeout(r, 350));
          return { success: true, message: 'Parent found', data: { parentType: 'group' } };
        },
      },
      {
        id: 'hierarchy-children',
        name: 'Get Children',
        description: 'Get children of selected group',
        run: async () => {
          await new Promise((r) => setTimeout(r, 400));
          return {
            success: true,
            message: 'Found 4 children',
            data: { count: 4, types: ['path', 'path', 'text', 'path'] },
          };
        },
      },
    ],
  },
];
</script>

<template>
  <div class="tests-view">
    <TestRunner :suites="testSuites" />
  </div>
</template>

<style scoped>
.tests-view {
  height: 100vh;
  width: 100vw;
  overflow: hidden;
}
</style>
