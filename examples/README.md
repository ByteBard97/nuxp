# NUXP Examples

This directory contains example code showing how to interact with the NUXP plugin.

## Prerequisites

- NUXP plugin installed and Illustrator running
- Node.js 18+ (for JavaScript examples)

## Examples

### basic-api.js

Demonstrates basic HTTP API calls to the plugin:
- Health check
- Get document info
- List layers
- Create a rectangle

```bash
node examples/basic-api.js
```

### batch-operations.js

Shows how to perform multiple operations:
- Get current selection
- Modify selected objects
- Handle errors gracefully

```bash
node examples/batch-operations.js
```

## Using from Other Languages

The NUXP plugin exposes a standard HTTP/JSON API, so you can use it from any language:

### Python
```python
import requests

response = requests.get('http://localhost:8080/health')
print(response.json())
```

### curl
```bash
curl http://localhost:8080/health
curl -X POST http://localhost:8080/api/call \
  -H "Content-Type: application/json" \
  -d '{"suite":"demo","method":"getDocumentInfo","args":{}}'
```

### Any HTTP Client
The API uses standard REST conventions - GET for reads, POST for operations.
