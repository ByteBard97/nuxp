---
layout: default
title: API Reference
---

# NUXP API Reference

The NUXP plugin exposes an HTTP/JSON API on `localhost:8080` (configurable) that allows external applications to control Adobe Illustrator.

## Base URL

```
http://localhost:8080
```

The port can be changed via the `/config/port` endpoint or CMake configuration.

---

## Core Endpoints

### Health Check

Check if the plugin is running and responsive.

```http
GET /health
```

**Response:**
```json
{
  "success": true,
  "status": "ok",
  "plugin": "NUXP",
  "version": "1.0.0"
}
```

---

### Plugin Info

Get detailed plugin information and handle counts.

```http
GET /info
```

**Response:**
```json
{
  "success": true,
  "plugin": {
    "name": "NUXP",
    "version": "1.0.0",
    "description": "Illustrator HTTP/JSON Bridge"
  },
  "handles": {
    "art": 5,
    "layers": 2,
    "documents": 1
  }
}
```

---

### Configuration

Get current server configuration:

```http
GET /config
```

Change server port (server restarts automatically):

```http
POST /config/port
Content-Type: application/json

{ "port": 9000 }
```

---

## Generic API Call

All SDK methods can be called through this unified endpoint:

```http
POST /api/call
Content-Type: application/json

{
  "suite": "SuiteName",
  "method": "MethodName",
  "args": { ... }
}
```

**Example - Get document info:**
```json
{
  "suite": "AIDocument",
  "method": "GetDocument",
  "args": {}
}
```

---

## Direct Suite Endpoints

For convenience, you can also call methods directly:

```http
POST /{SuiteName}/{MethodName}
Content-Type: application/json

{ ...args }
```

**Example:**
```http
POST /AIArt/GetArtType
Content-Type: application/json

{ "art": 1 }
```

---

## Server-Sent Events (SSE)

Subscribe to real-time events from Illustrator:

```http
GET /events/stream
```

This returns an SSE stream with typed events:

| Event | Payload | Fires When |
|-------|---------|------------|
| `selection` | `{ count, selectedIds }` | Selection changes |
| `document` | `{ type, documentName }` | Document opened, closed, or switched |
| `layers` | `{ layerCount }` | Layers added, removed, or reordered |
| `artChanged` | `{ artIds, changeType }` | Art objects created, modified, or deleted |
| `version` | `{ version, build }` | Sent once on initial connection |

**Example events:**
```
event: selection
data: {"count": 3, "selectedIds": [1, 2, 3]}

event: document
data: {"type": "activated", "documentName": "my-design.ai"}
```

The TypeScript SDK provides a typed `SSEAdapter` that wraps EventSource with automatic reconnection. See [`sdk/README.md`](../../sdk/README.md) for usage.

---

## Handle Management

NUXP uses integer IDs to reference Illustrator objects safely across the HTTP boundary.

### How Handles Work

1. When you retrieve objects (layers, art, etc.), NUXP returns integer IDs
2. Use these IDs in subsequent calls to reference the same objects
3. IDs are invalidated when documents close or undo occurs

### Invalidate All Handles

Force-invalidate all handles (useful for testing):

```http
POST /handles/invalidate
```

---

## Error Handling

All responses include a `success` boolean. On failure:

```json
{
  "success": false,
  "error": "Description of what went wrong",
  "errorCode": 12345
}
```

Common error codes:
- Network errors: Plugin not running or wrong port
- `errorCode` present: SDK error code from Illustrator

---

## TypeScript SDK

The `@nuxp/sdk` package wraps these endpoints with typed Bridge calls, request serialization, and event streaming:

```typescript
import { createBridge, setBridgeInstance, connectSSE } from '@nuxp/sdk'

// Setup
const bridge = createBridge({ port: 8080 })
setBridgeInstance(bridge)

// Suite call
const result = await bridge.callSuite('AIDocument', 'GetDocument', {})

// Events
const sse = connectSSE({ serverUrl: 'http://localhost:8080' })
sse.on('selection', (data) => console.log(data.count))
```

See [`sdk/README.md`](../../sdk/README.md) for full SDK documentation.

---

## CORS

The server allows cross-origin requests from any origin, enabling browser-based frontends to connect directly.

---

## Rate Limiting

There is no built-in rate limiting. The server handles requests sequentially on Illustrator's main thread, so rapid requests will queue naturally.
