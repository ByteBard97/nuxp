# Configurable Port Specification

## Overview

NUXP should support runtime-configurable HTTP server port, allowing users to change the port without restarting Illustrator or the frontend application.

## Goals

1. **No restart required** - Port changes apply immediately
2. **Persistent configuration** - Port setting survives app restarts
3. **Generic infrastructure** - Downstream apps (Flora, etc.) inherit this capability
4. **Graceful reconnection** - Frontend handles port change seamlessly

---

## Architecture

```
┌─────────────────┐                           ┌──────────────────┐
│  Frontend App   │                           │   C++ Plugin     │
│  (Tauri/Web)    │                           │  (Illustrator)   │
├─────────────────┤      HTTP/SSE             ├──────────────────┤
│  TypeScript     │ ←───────────────────────→ │  HttpServer      │
│  - SDK client   │     localhost:PORT        │  - ConfigManager │
│  - SSE client   │                           │  - SSE           │
└─────────────────┘                           └──────────────────┘
```

---

## C++ Implementation

### 1. Config File Location

```cpp
// Platform-specific paths
#ifdef _WIN32
    // Windows: %APPDATA%/NUXP/config.json
    std::string GetConfigDir() {
        char* appdata = getenv("APPDATA");
        return std::string(appdata) + "/NUXP";
    }
#else
    // macOS: ~/Library/Application Support/NUXP/config.json
    std::string GetConfigDir() {
        char* home = getenv("HOME");
        return std::string(home) + "/Library/Application Support/NUXP";
    }
#endif
```

### 2. Config File Format

```json
{
  "version": 1,
  "server": {
    "port": 8080
  }
}
```

### 3. ConfigManager Class

**File: `src/ConfigManager.hpp`**

```cpp
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <mutex>

class ConfigManager {
public:
    static constexpr int DEFAULT_PORT = 8080;

    /**
     * Get singleton instance
     */
    static ConfigManager& Instance();

    /**
     * Load configuration from disk.
     * Creates default config if file doesn't exist.
     */
    void Load();

    /**
     * Save current configuration to disk.
     */
    void Save();

    /**
     * Get the configured server port.
     * @return Port number (default: 8080)
     */
    int GetPort() const;

    /**
     * Set the server port.
     * Does NOT automatically restart the server - caller must do that.
     * @param port New port number
     */
    void SetPort(int port);

    /**
     * Get the full config as JSON (for /config endpoint)
     */
    nlohmann::json GetConfig() const;

    /**
     * Get the config file path
     */
    static std::string GetConfigPath();

private:
    ConfigManager() = default;

    mutable std::mutex mutex_;
    nlohmann::json config_;
};
```

### 4. HTTP Endpoints

**GET /config**
Returns current configuration.

```json
{
  "success": true,
  "config": {
    "version": 1,
    "server": {
      "port": 8080
    }
  }
}
```

**POST /config/port**
Changes the server port. Server restarts on new port immediately.

Request:
```json
{
  "port": 8082
}
```

Response:
```json
{
  "success": true,
  "previousPort": 8080,
  "newPort": 8082,
  "message": "Server restarted on port 8082"
}
```

Error Response (invalid port):
```json
{
  "success": false,
  "error": "Port must be between 1024 and 65535"
}
```

### 5. Port Change Handler

```cpp
std::string HandleSetPort(const std::string& body) {
    auto json = nlohmann::json::parse(body);
    int newPort = json["port"].get<int>();

    // Validate port range
    if (newPort < 1024 || newPort > 65535) {
        return R"({"success": false, "error": "Port must be between 1024 and 65535"})";
    }

    int oldPort = ConfigManager::Instance().GetPort();

    // Save new port to config
    ConfigManager::Instance().SetPort(newPort);
    ConfigManager::Instance().Save();

    // Schedule server restart (must happen after response is sent)
    // Use a short delay so the HTTP response can complete
    std::thread([newPort]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        HttpServer::Stop();
        HttpServer::Start(newPort);
    }).detach();

    nlohmann::json response;
    response["success"] = true;
    response["previousPort"] = oldPort;
    response["newPort"] = newPort;
    response["message"] = "Server restarting on port " + std::to_string(newPort);

    return response.dump();
}
```

### 6. Startup Flow

```cpp
// In plugin startup:
void StartPlugin() {
    // Load config (creates default if missing)
    ConfigManager::Instance().Load();

    // Start server on configured port
    int port = ConfigManager::Instance().GetPort();
    HttpServer::Start(port);
}
```

---

## TypeScript Implementation

### 1. SDK Config Extension

**File: `sdk/config.ts`**

```typescript
export interface ServerConfig {
  port: number;
}

// In-memory current config
let currentPort: number = DEFAULT_PORT;

/**
 * Get the current server URL
 */
export function getApiUrl(path: string): string {
  return `http://localhost:${currentPort}${path}`;
}

/**
 * Update the port used for API calls
 */
export function setPort(port: number): void {
  currentPort = port;
  // Persist to localStorage for page reloads
  localStorage.setItem('nuxp_port', String(port));
}

/**
 * Load port from localStorage on init
 */
export function initConfig(): void {
  const saved = localStorage.getItem('nuxp_port');
  if (saved) {
    currentPort = parseInt(saved, 10);
  }
}
```

### 2. Config API Functions

**File: `sdk/configApi.ts`**

```typescript
import { getApiUrl, setPort } from './config';
import { sseClient } from './sse';

export interface PortChangeResult {
  success: boolean;
  previousPort: number;
  newPort: number;
  message: string;
  error?: string;
}

/**
 * Get current server configuration
 */
export async function getServerConfig(): Promise<{ port: number }> {
  const response = await fetch(getApiUrl('/config'));
  const data = await response.json();
  return data.config.server;
}

/**
 * Change the server port
 * Handles reconnection automatically
 */
export async function changePort(newPort: number): Promise<PortChangeResult> {
  // Validate locally first
  if (newPort < 1024 || newPort > 65535) {
    return {
      success: false,
      previousPort: 0,
      newPort,
      message: '',
      error: 'Port must be between 1024 and 65535'
    };
  }

  try {
    // Send request to current port
    const response = await fetch(getApiUrl('/config/port'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ port: newPort })
    });

    const result: PortChangeResult = await response.json();

    if (result.success) {
      // Update local config
      setPort(newPort);

      // Reconnect SSE to new port after brief delay
      // (server needs time to restart)
      setTimeout(() => {
        sseClient.disconnect();
        sseClient.connect();
      }, 500);
    }

    return result;
  } catch (error) {
    return {
      success: false,
      previousPort: 0,
      newPort,
      message: '',
      error: `Failed to change port: ${error}`
    };
  }
}
```

### 3. SSE Reconnection

The SSE client should use `getApiUrl()` so it automatically uses the updated port:

```typescript
// In SSE client connect():
connect(): void {
  const url = getApiUrl('/events/stream');  // Uses current port
  this.eventSource = new EventSource(url);
  // ...
}
```

---

## UI Integration (Optional for NUXP, Required for Downstream Apps)

Downstream apps like Flora can add a settings UI:

```vue
<template>
  <div class="port-settings">
    <label>HTTP Server Port</label>
    <input v-model.number="port" type="number" min="1024" max="65535" />
    <button @click="savePort" :disabled="saving">
      {{ saving ? 'Saving...' : 'Save' }}
    </button>
    <span v-if="message" :class="{ error: isError }">{{ message }}</span>
  </div>
</template>

<script setup>
import { ref } from 'vue';
import { changePort, getServerConfig } from '@/sdk/configApi';

const port = ref(8080);
const saving = ref(false);
const message = ref('');
const isError = ref(false);

onMounted(async () => {
  const config = await getServerConfig();
  port.value = config.port;
});

async function savePort() {
  saving.value = true;
  message.value = '';

  const result = await changePort(port.value);

  if (result.success) {
    message.value = `Port changed to ${result.newPort}`;
    isError.value = false;
  } else {
    message.value = result.error || 'Failed to change port';
    isError.value = true;
  }

  saving.value = false;
}
</script>
```

---

## Port Discovery (Advanced)

For cases where the frontend doesn't know the port (fresh install), implement port scanning:

```typescript
const CANDIDATE_PORTS = [8080, 8081, 8082, 8083];

/**
 * Find the port where NUXP plugin is running
 */
export async function discoverPort(): Promise<number | null> {
  for (const port of CANDIDATE_PORTS) {
    try {
      const response = await fetch(`http://localhost:${port}/health`, {
        signal: AbortSignal.timeout(500)
      });
      if (response.ok) {
        const data = await response.json();
        // Verify it's NUXP, not some other service
        if (data.plugin === 'NUXP') {
          return port;
        }
      }
    } catch {
      // Port not responding, try next
    }
  }
  return null;
}
```

Update `/health` endpoint to include plugin identifier:

```json
{
  "success": true,
  "plugin": "NUXP",
  "version": "1.0.0"
}
```

---

## Testing Requirements

1. **Unit tests** for ConfigManager (load, save, get, set)
2. **Integration test**: Change port via HTTP, verify server restarts
3. **E2E test**: UI changes port, SSE reconnects, events flow
4. **Edge cases**:
   - Port already in use
   - Invalid port numbers
   - Config file permissions error
   - Config file corrupted

---

## Security Considerations

1. **Localhost only** - Server binds to `127.0.0.1`, not `0.0.0.0`
2. **Port range** - Restrict to unprivileged ports (1024-65535)
3. **No auth required** - Localhost assumption (revisit if remote access needed)

---

## Summary

| Layer | Change |
|-------|--------|
| C++ | Add `ConfigManager`, `/config` endpoints, server restart logic |
| TypeScript | Add `configApi.ts`, update SSE to use dynamic port |
| Config File | `~/Library/Application Support/NUXP/config.json` |
| Default Port | 8080 |
