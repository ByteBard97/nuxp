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
GET /events
```

This returns an SSE stream. Events include:
- Document changes
- Selection changes
- Layer modifications
- Heartbeat (every 30 seconds)

**Example event:**
```
event: selectionChanged
data: {"count": 3}

event: heartbeat
data: {"timestamp": 1234567890}
```

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

The frontend shell includes a TypeScript SDK that wraps these endpoints:

```typescript
import { callCpp } from '@/sdk/bridge'

// Generic call
const result = await callCpp('AIDocument', 'GetDocument', {})

// Or use generated suite clients
import { AIDocumentSuite } from '@/sdk/generated'
const doc = await AIDocumentSuite.GetDocument()
```

---

## CORS

The server allows cross-origin requests from any origin, enabling browser-based frontends to connect directly.

---

## Rate Limiting

There is no built-in rate limiting. The server handles requests sequentially on Illustrator's main thread, so rapid requests will queue naturally.
