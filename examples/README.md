# NUXP Examples

## Script Toolkit (Recommended)

The best way to see NUXP in action is the built-in Script Toolkit. Run:

```bash
cd shell
npm run dev
```

Click "Scripts" in the sidebar to access 15 ready-to-run operations covering document info, layers, selection, text, and view control.

## Using the HTTP API Directly

NUXP exposes a standard HTTP/JSON API on localhost:8080. You can call it from any language:

### JavaScript / TypeScript

```javascript
const response = await fetch('http://localhost:8080/api/doc/info')
const docInfo = await response.json()
console.log(docInfo)
```

### Python

```python
import requests

response = requests.get('http://localhost:8080/api/doc/info')
print(response.json())
```

### curl

```bash
# Document info
curl http://localhost:8080/api/doc/info

# Health check
curl http://localhost:8080/health

# List layers
curl http://localhost:8080/api/query/layers

# Call a generated SDK function
curl -X POST http://localhost:8080/api/call \
  -H "Content-Type: application/json" \
  -d '{"suite":"AILayerSuite","method":"CountLayers","args":{}}'
```

See the [API Reference](../docs/api/README.md) for all available endpoints.
