/**
 * Connection state store
 *
 * Manages the connection status between the shell and the
 * NUXP plugin running in Adobe Illustrator.
 */

import { defineStore } from 'pinia';
import { ref, computed } from 'vue';
import { getHealthInfo } from '@/services/api';

/** Interval between automatic health checks (ms) */
const HEALTH_CHECK_INTERVAL = 5000;

export const useConnectionStore = defineStore('connection', () => {
  // State
  const connected = ref(false);
  const lastCheck = ref<Date | null>(null);
  const pluginVersion = ref<string | null>(null);
  const error = ref<string | null>(null);
  const checking = ref(false);

  // Internal state for interval management
  let healthCheckInterval: ReturnType<typeof setInterval> | null = null;

  // Getters
  const statusText = computed(() => {
    if (checking.value) return 'Checking...';
    if (connected.value) return 'Connected';
    if (error.value) return `Disconnected: ${error.value}`;
    return 'Disconnected';
  });

  const lastCheckFormatted = computed(() => {
    if (!lastCheck.value) return 'Never';
    return lastCheck.value.toLocaleTimeString();
  });

  // Actions
  async function checkConnection(): Promise<boolean> {
    if (checking.value) return connected.value;

    checking.value = true;
    error.value = null;

    try {
      const healthInfo = await getHealthInfo();

      if (healthInfo && healthInfo.status === 'ok') {
        connected.value = true;
        pluginVersion.value = healthInfo.version;
        error.value = null;
      } else {
        connected.value = false;
        pluginVersion.value = null;
        error.value = healthInfo?.message || 'Plugin not responding';
      }
    } catch (e) {
      connected.value = false;
      pluginVersion.value = null;
      error.value = e instanceof Error ? e.message : 'Connection failed';
    } finally {
      lastCheck.value = new Date();
      checking.value = false;
    }

    return connected.value;
  }

  function setConnected(status: boolean): void {
    connected.value = status;
    if (!status) {
      pluginVersion.value = null;
    }
    lastCheck.value = new Date();
  }

  function startAutoCheck(): void {
    if (healthCheckInterval) return;

    // Perform initial check
    checkConnection();

    // Set up periodic checks
    healthCheckInterval = setInterval(() => {
      checkConnection();
    }, HEALTH_CHECK_INTERVAL);
  }

  function stopAutoCheck(): void {
    if (healthCheckInterval) {
      clearInterval(healthCheckInterval);
      healthCheckInterval = null;
    }
  }

  function reset(): void {
    connected.value = false;
    lastCheck.value = null;
    pluginVersion.value = null;
    error.value = null;
    checking.value = false;
    stopAutoCheck();
  }

  return {
    // State
    connected,
    lastCheck,
    pluginVersion,
    error,
    checking,

    // Getters
    statusText,
    lastCheckFormatted,

    // Actions
    checkConnection,
    setConnected,
    startAutoCheck,
    stopAutoCheck,
    reset,
  };
});
