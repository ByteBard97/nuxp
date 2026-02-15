/**
 * NUXP Basic API Example
 *
 * Demonstrates how to interact with the NUXP plugin via HTTP.
 * Run with: node examples/basic-api.js
 *
 * Prerequisites:
 * - NUXP plugin installed in Illustrator
 * - Illustrator running with a document open
 */

const BASE_URL = 'http://localhost:8080'

/**
 * Make a GET request to the plugin
 */
async function get(path) {
  const response = await fetch(`${BASE_URL}${path}`)
  return response.json()
}

/**
 * Make a POST request to the plugin
 */
async function post(path, body = {}) {
  const response = await fetch(`${BASE_URL}${path}`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body)
  })
  return response.json()
}

/**
 * Call an SDK method via the generic /api/call endpoint.
 *
 * How it works:
 *   POST /api/call is handled by NUXP's CentralDispatcher, which looks up the
 *   given "suite" name in its handler registry and calls the corresponding method.
 *
 * Important distinction -- suite names used below:
 *   The suite name "demo" is NOT an Adobe Illustrator SDK suite.  It is a custom
 *   handler suite registered by DemoEndpoints.cpp (see plugin/src/endpoints/).
 *   Adobe SDK suites use names like "AIArt", "AILayer", "AIDocument", etc.
 *
 * Alternative -- REST-style routes:
 *   For custom (hand-written) endpoints, NUXP also exposes REST-style routes that
 *   are generated from routes.json.  These can be more readable than /api/call:
 *
 *     // Equivalent to callSdk('demo', 'getDocumentInfo'):
 *     const docInfo = await get('/api/doc/info')
 *
 *     // Equivalent to callSdk('demo', 'getLayers'):
 *     const layers = await get('/api/doc/layers')
 *
 *   Use whichever style you prefer.  The /api/call route is the universal fallback
 *   and works for both generated SDK wrappers and custom handlers.
 */
async function callSdk(suite, method, args = {}) {
  return post('/api/call', { suite, method, args })
}

// ============================================================================
// Examples
// ============================================================================

async function main() {
  console.log('NUXP Basic API Examples\n')
  console.log('='.repeat(50))

  // 1. Health Check
  console.log('\n1. Health Check')
  console.log('-'.repeat(30))
  try {
    const health = await get('/health')
    console.log('Status:', health.status)
    console.log('Plugin:', health.plugin)
    console.log('Version:', health.version)
  } catch (err) {
    console.error('Error: Plugin not responding. Is Illustrator running?')
    console.error('Make sure the NUXP plugin is loaded.')
    process.exit(1)
  }

  // 2. Plugin Info
  console.log('\n2. Plugin Info')
  console.log('-'.repeat(30))
  const info = await get('/info')
  console.log('Handles in use:')
  console.log('  Art objects:', info.handles?.art || 0)
  console.log('  Layers:', info.handles?.layers || 0)
  console.log('  Documents:', info.handles?.documents || 0)

  // 3. Get Document Info
  console.log('\n3. Document Info')
  console.log('-'.repeat(30))
  const docResult = await callSdk('demo', 'getDocumentInfo')
  if (docResult.success) {
    const doc = docResult.document
    console.log('Name:', doc.name)
    console.log('Size:', `${doc.width} x ${doc.height} points`)
    console.log('Artboards:', doc.artboardCount)
  } else {
    console.log('Error:', docResult.error)
    console.log('(Make sure you have a document open)')
  }

  // 4. List Layers
  console.log('\n4. Layers')
  console.log('-'.repeat(30))
  const layersResult = await callSdk('demo', 'getLayers')
  if (layersResult.success) {
    const layers = layersResult.layers
    console.log(`Found ${layers.length} layer(s):`)
    layers.forEach((layer, i) => {
      const status = []
      if (!layer.visible) status.push('hidden')
      if (layer.locked) status.push('locked')
      const statusStr = status.length ? ` (${status.join(', ')})` : ''
      console.log(`  ${i + 1}. ${layer.name}${statusStr}`)
    })
  } else {
    console.log('Error:', layersResult.error)
  }

  // 5. Get Selection
  console.log('\n5. Current Selection')
  console.log('-'.repeat(30))
  const selResult = await callSdk('demo', 'getSelection')
  if (selResult.success) {
    const sel = selResult.selection
    console.log(`${sel.count} object(s) selected`)
    if (sel.count > 0) {
      sel.items.slice(0, 5).forEach((item, i) => {
        console.log(`  ${i + 1}. ${item.type}`)
      })
      if (sel.count > 5) {
        console.log(`  ... and ${sel.count - 5} more`)
      }
    }
  } else {
    console.log('Error:', selResult.error)
  }

  // 6. Create a Rectangle
  console.log('\n6. Create Rectangle')
  console.log('-'.repeat(30))
  const rectResult = await callSdk('demo', 'createRectangle', {
    x: 100,
    y: 100,
    width: 200,
    height: 150
  })
  if (rectResult.success) {
    console.log('Created rectangle at (100, 100)')
    console.log('Size: 200 x 150 points')
  } else {
    console.log('Error:', rectResult.error)
  }

  console.log('\n' + '='.repeat(50))
  console.log('Done! Check Illustrator to see the rectangle.')
}

main().catch(console.error)
