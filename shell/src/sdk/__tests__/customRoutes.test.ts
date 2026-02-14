import { describe, it, expect, beforeEach, vi, type Mock } from 'vitest'

// Mock fetch globally before importing the module under test
const mockFetch = vi.fn() as Mock
vi.stubGlobal('fetch', mockFetch)

import {
  GetSelection,
  GetMatchingArt,
  GetPathStyle,
  SetPathStyle,
  GetPathSegments,
  SetPathSegments,
  CheckBounds,
  DeselectAll,
  SelectByHandles,
  QueryTextFrames,
  QueryLayers,
  FindArtByName,
  CalculatePathArea,
  CreateTextFrame,
  GetTextContent,
  SetTextContent,
  GetDocumentXmp,
  SetDocumentXmp,
  GetXmpStatus,
  GetXmpProperty,
  SetXmpProperty,
  RegisterXmpNamespace,
  GetDocumentInfo,
  GetViewZoom,
  SetViewZoom,
  SetViewCenter,
  FitArtboardInView,
  FitSelectionInView,
  QueryPathItems,
  CountItemsOnLayer,
} from '../generated/customRoutes'

function mockResponse(data: unknown, status = 200) {
  return Promise.resolve({
    ok: status >= 200 && status < 300,
    status,
    text: () => Promise.resolve(JSON.stringify(data)),
  })
}

function mockTextResponse(text: string, status = 200) {
  return Promise.resolve({
    ok: status >= 200 && status < 300,
    status,
    text: () => Promise.resolve(text),
  })
}

/** Extract the fetch call args from the most recent mockFetch call */
function lastFetchArgs() {
  const [url, init] = mockFetch.mock.calls[mockFetch.mock.calls.length - 1]
  return { url: url as string, init: init as RequestInit }
}

beforeEach(() => {
  mockFetch.mockReset()
})

// ---------------------------------------------------------------------------
// fetchRoute helper behavior (tested indirectly through route functions)
// ---------------------------------------------------------------------------
describe('fetchRoute helper', () => {
  it('throws on HTTP error responses', async () => {
    mockFetch.mockReturnValue(mockTextResponse('Not Found', 404))
    await expect(GetSelection()).rejects.toThrow('HTTP 404: Not Found')
  })

  it('returns empty object for empty response body', async () => {
    mockFetch.mockReturnValue(mockTextResponse('', 200))
    const result = await GetSelection()
    expect(result).toEqual({})
  })

  it('returns empty object for whitespace-only response', async () => {
    mockFetch.mockReturnValue(mockTextResponse('   ', 200))
    const result = await GetSelection()
    expect(result).toEqual({})
  })

  it('throws on invalid JSON response', async () => {
    mockFetch.mockReturnValue(mockTextResponse('not-json{', 200))
    await expect(GetSelection()).rejects.toThrow()
  })

  it('propagates network errors', async () => {
    mockFetch.mockRejectedValue(new TypeError('Failed to fetch'))
    await expect(GetSelection()).rejects.toThrow('Failed to fetch')
  })
})

// ---------------------------------------------------------------------------
// Selection endpoints
// ---------------------------------------------------------------------------
describe('Selection endpoints', () => {
  it('GetSelection calls GET /api/selection', async () => {
    const data = { handles: [1, 2, 3], count: 3 }
    mockFetch.mockReturnValue(mockResponse(data))

    const result = await GetSelection()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/selection')
    expect(init.body).toBeUndefined()
    expect(result).toEqual(data)
  })

  it('GetMatchingArt calls POST /api/selection/match with body', async () => {
    const data = { handles: [10], count: 1 }
    mockFetch.mockReturnValue(mockResponse(data))

    const params = { type: 2, whichAttr: 1, attr: 0 }
    const result = await GetMatchingArt(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/selection/match')
    expect(init.body).toBe(JSON.stringify(params))
    expect(result).toEqual(data)
  })

  it('DeselectAll calls POST /api/selection/deselect-all', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true }))

    const result = await DeselectAll()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/selection/deselect-all')
    expect(init.body).toBeUndefined()
    expect(result.success).toBe(true)
  })

  it('SelectByHandles calls POST /api/selection/select with body', async () => {
    mockFetch.mockReturnValue(mockResponse({ selected: 2 }))

    const params = { handles: [1, 2] }
    await SelectByHandles(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/selection/select')
    expect(init.body).toBe(JSON.stringify(params))
  })
})

// ---------------------------------------------------------------------------
// Art style and segment endpoints (path-parameterized)
// ---------------------------------------------------------------------------
describe('Art style/segment endpoints', () => {
  it('GetPathStyle encodes id in URL', async () => {
    mockFetch.mockReturnValue(mockResponse({
      fillPaint: true, strokePaint: false, fill: {}, stroke: {},
      evenodd: false, resolution: 800,
    }))

    await GetPathStyle('42')
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/art/42/style')
    expect(init.body).toBeUndefined()
  })

  it('GetPathStyle encodes special characters in id', async () => {
    mockFetch.mockReturnValue(mockResponse({
      fillPaint: true, strokePaint: false, fill: {}, stroke: {},
      evenodd: false, resolution: 800,
    }))

    await GetPathStyle('foo/bar')
    const { url } = lastFetchArgs()
    expect(url).toContain('/api/art/foo%2Fbar/style')
  })

  it('SetPathStyle calls POST with id and body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true }))

    const params = { fillPaint: true, fill: { color: { kind: 'rgb', r: 1, g: 0, b: 0 } } }
    await SetPathStyle('7', params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/art/7/style')
    expect(init.body).toBe(JSON.stringify(params))
  })

  it('GetPathSegments encodes id in URL', async () => {
    mockFetch.mockReturnValue(mockResponse({ count: 4, closed: true, segments: [] }))

    await GetPathSegments('99')
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/art/99/segments')
  })

  it('SetPathSegments calls POST with id and body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true }))

    const params = { segments: [{ p: { h: 0, v: 0 }, in: { h: 0, v: 0 }, out: { h: 0, v: 0 }, corner: true }] }
    await SetPathSegments('5', params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/art/5/segments')
    expect(init.body).toBe(JSON.stringify(params))
  })

  it('CalculatePathArea encodes id in URL', async () => {
    mockFetch.mockReturnValue(mockResponse({ area: 500.5, signed_area: -500.5 }))

    const result = await CalculatePathArea('12')
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/art/12/area')
    expect(result.area).toBe(500.5)
    expect(result.signed_area).toBe(-500.5)
  })
})

// ---------------------------------------------------------------------------
// Artboard endpoint
// ---------------------------------------------------------------------------
describe('Artboard endpoint', () => {
  it('CheckBounds calls POST /api/artboard/check-bounds with body', async () => {
    const data = { fits: true, clampedX: 10, clampedY: 20 }
    mockFetch.mockReturnValue(mockResponse(data))

    const params = { x: 10, y: 20, width: 100, height: 50 }
    const result = await CheckBounds(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/artboard/check-bounds')
    expect(init.body).toBe(JSON.stringify(params))
    expect(result).toEqual(data)
  })
})

// ---------------------------------------------------------------------------
// Query endpoints
// ---------------------------------------------------------------------------
describe('Query endpoints', () => {
  it('QueryTextFrames calls GET /api/query/text-frames', async () => {
    mockFetch.mockReturnValue(mockResponse({ frames: [], count: 0 }))

    await QueryTextFrames()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/query/text-frames')
  })

  it('QueryLayers calls GET /api/query/layers', async () => {
    mockFetch.mockReturnValue(mockResponse({ layers: [], count: 0 }))

    await QueryLayers()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/query/layers')
  })

  it('FindArtByName calls POST /api/query/find with body', async () => {
    mockFetch.mockReturnValue(mockResponse({ items: [], count: 0 }))

    const params = { name: 'Rectangle 1' }
    await FindArtByName(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/query/find')
    expect(init.body).toBe(JSON.stringify(params))
  })

  it('QueryPathItems calls GET /api/query/path-items', async () => {
    mockFetch.mockReturnValue(mockResponse({ items: [], count: 0 }))

    await QueryPathItems()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/query/path-items')
  })

  it('CountItemsOnLayer calls POST /api/query/count with body', async () => {
    const data = { count: 5, layer: 'Layer 1' }
    mockFetch.mockReturnValue(mockResponse(data))

    const params = { layer: 'Layer 1' }
    const result = await CountItemsOnLayer(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/query/count')
    expect(init.body).toBe(JSON.stringify(params))
    expect(result).toEqual(data)
  })
})

// ---------------------------------------------------------------------------
// Text endpoints
// ---------------------------------------------------------------------------
describe('Text endpoints', () => {
  it('CreateTextFrame calls POST /api/text/create with body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true, artId: 42 }))

    const params = { x: 100, y: 200, contents: 'Hello' }
    const result = await CreateTextFrame(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/text/create')
    expect(init.body).toBe(JSON.stringify(params))
    expect(result.artId).toBe(42)
  })

  it('GetTextContent encodes id in URL', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true, artId: 5, contents: 'Hello World' }))

    const result = await GetTextContent('5')
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/text/5/content')
    expect(result.contents).toBe('Hello World')
  })

  it('SetTextContent calls POST with id and body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true, artId: 5 }))

    const params = { contents: 'New text' }
    await SetTextContent('5', params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/text/5/content')
    expect(init.body).toBe(JSON.stringify(params))
  })
})

// ---------------------------------------------------------------------------
// XMP endpoints
// ---------------------------------------------------------------------------
describe('XMP endpoints', () => {
  it('GetDocumentXmp calls GET /api/xmp', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true, xmp: '<xmp/>' }))

    const result = await GetDocumentXmp()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/xmp')
    expect(result.xmp).toBe('<xmp/>')
  })

  it('SetDocumentXmp calls POST /api/xmp with body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true, message: 'ok' }))

    const params = { xmp: '<xmp>test</xmp>' }
    await SetDocumentXmp(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/xmp')
    expect(url).not.toContain('/api/xmp/')
    expect(init.body).toBe(JSON.stringify(params))
  })

  it('GetXmpStatus calls GET /api/xmp/status', async () => {
    mockFetch.mockReturnValue(mockResponse({
      success: true, available: true, propertyAccess: true,
      capabilities: { getDocumentXMP: true },
    }))

    const result = await GetXmpStatus()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/xmp/status')
    expect(result.available).toBe(true)
  })

  it('GetXmpProperty calls POST /api/xmp/property with body', async () => {
    mockFetch.mockReturnValue(mockResponse({
      success: true, namespace: 'http://ns.example.com/', name: 'key', value: 'val', found: true,
    }))

    const params = { namespace: 'http://ns.example.com/', name: 'key' }
    const result = await GetXmpProperty(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/xmp/property')
    expect(url).not.toContain('/api/xmp/property/')
    expect(init.body).toBe(JSON.stringify(params))
    expect(result.found).toBe(true)
  })

  it('SetXmpProperty calls POST /api/xmp/property/set with body', async () => {
    mockFetch.mockReturnValue(mockResponse({
      success: true, namespace: 'http://ns.example.com/', name: 'key', value: 'newval',
    }))

    const params = { namespace: 'http://ns.example.com/', name: 'key', value: 'newval' }
    await SetXmpProperty(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/xmp/property/set')
    expect(init.body).toBe(JSON.stringify(params))
  })

  it('RegisterXmpNamespace calls POST /api/xmp/namespace with body', async () => {
    mockFetch.mockReturnValue(mockResponse({
      success: true, uri: 'http://ns.example.com/', requestedPrefix: 'ex', actualPrefix: 'ex',
    }))

    const params = { uri: 'http://ns.example.com/', prefix: 'ex' }
    await RegisterXmpNamespace(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/xmp/namespace')
    expect(init.body).toBe(JSON.stringify(params))
  })
})

// ---------------------------------------------------------------------------
// Document info endpoint
// ---------------------------------------------------------------------------
describe('Document info endpoint', () => {
  it('GetDocumentInfo calls GET /api/doc/info', async () => {
    const data = {
      name: 'Untitled', path: '/tmp', fullPath: '/tmp/Untitled.ai',
      saved: false, artboards: [],
    }
    mockFetch.mockReturnValue(mockResponse(data))

    const result = await GetDocumentInfo()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/doc/info')
    expect(result.name).toBe('Untitled')
    expect(result.saved).toBe(false)
  })
})

// ---------------------------------------------------------------------------
// View control endpoints
// ---------------------------------------------------------------------------
describe('View control endpoints', () => {
  it('GetViewZoom calls GET /api/view/zoom', async () => {
    mockFetch.mockReturnValue(mockResponse({ zoom: 150 }))

    const result = await GetViewZoom()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('GET')
    expect(url).toContain('/api/view/zoom')
    expect(result.zoom).toBe(150)
  })

  it('SetViewZoom calls POST /api/view/zoom with body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true, zoom: 200 }))

    const params = { zoom: 200 }
    const result = await SetViewZoom(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/view/zoom')
    expect(init.body).toBe(JSON.stringify(params))
    expect(result.zoom).toBe(200)
  })

  it('SetViewCenter calls POST /api/view/center with body', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true }))

    const params = { x: 300, y: 400 }
    await SetViewCenter(params)
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/view/center')
    expect(init.body).toBe(JSON.stringify(params))
  })

  it('FitArtboardInView calls POST /api/view/fit-artboard', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true }))

    await FitArtboardInView()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/view/fit-artboard')
    expect(init.body).toBeUndefined()
  })

  it('FitSelectionInView calls POST /api/view/fit-selection', async () => {
    mockFetch.mockReturnValue(mockResponse({ success: true }))

    await FitSelectionInView()
    const { url, init } = lastFetchArgs()

    expect(init.method).toBe('POST')
    expect(url).toContain('/api/view/fit-selection')
    expect(init.body).toBeUndefined()
  })
})

// ---------------------------------------------------------------------------
// Request headers
// ---------------------------------------------------------------------------
describe('Request headers', () => {
  it('all requests include Content-Type: application/json', async () => {
    mockFetch.mockReturnValue(mockResponse({}))
    await GetSelection()
    const { init } = lastFetchArgs()
    expect((init.headers as Record<string, string>)['Content-Type']).toBe('application/json')
  })
})
