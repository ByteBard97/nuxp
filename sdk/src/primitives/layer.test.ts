import { describe, it, expect, vi, beforeEach } from 'vitest'
import { getLayerByName, createLayer, getLayerArt, getAllLayers } from './layer'
import type { BridgeCallFn } from './types'

vi.mock('../generated/AILayerSuite', () => ({
  CountLayers: vi.fn(),
  GetNthLayer: vi.fn(),
  GetLayerTitle: vi.fn(),
  InsertLayer: vi.fn(),
  SetLayerTitle: vi.fn(),
  GetLayerByTitle: vi.fn(),
}))

vi.mock('../generated/AIArtSuite', () => ({
  GetFirstArtOfLayer: vi.fn(),
  GetArtSibling: vi.fn(),
}))

import {
  CountLayers,
  GetNthLayer,
  GetLayerTitle,
  InsertLayer,
  SetLayerTitle,
  GetLayerByTitle,
} from '../generated/AILayerSuite'
import { GetFirstArtOfLayer, GetArtSibling } from '../generated/AIArtSuite'

const bridge: BridgeCallFn = vi.fn()

beforeEach(() => {
  vi.clearAllMocks()
})

describe('getLayerByName', () => {
  it('returns handle when layer exists', async () => {
    vi.mocked(GetLayerByTitle).mockResolvedValue(5)

    const result = await getLayerByName(bridge, 'Background')

    expect(GetLayerByTitle).toHaveBeenCalledWith('Background')
    expect(result).toBe(5)
  })

  it('returns null when handle is 0', async () => {
    vi.mocked(GetLayerByTitle).mockResolvedValue(0)

    const result = await getLayerByName(bridge, 'Nonexistent')

    expect(result).toBeNull()
  })

  it('returns null when GetLayerByTitle throws', async () => {
    vi.mocked(GetLayerByTitle).mockRejectedValue(new Error('Layer not found'))

    const result = await getLayerByName(bridge, 'Missing')

    expect(result).toBeNull()
  })
})

describe('createLayer', () => {
  it('inserts a layer and sets its title', async () => {
    vi.mocked(InsertLayer).mockResolvedValue(10)
    vi.mocked(SetLayerTitle).mockResolvedValue(undefined)

    const handle = await createLayer(bridge, 'New Layer')

    expect(InsertLayer).toHaveBeenCalledWith(0, 0)
    expect(SetLayerTitle).toHaveBeenCalledWith(10, 'New Layer')
    expect(handle).toBe(10)
  })

  it('passes custom position to InsertLayer', async () => {
    vi.mocked(InsertLayer).mockResolvedValue(11)
    vi.mocked(SetLayerTitle).mockResolvedValue(undefined)

    await createLayer(bridge, 'Bottom Layer', 3)

    expect(InsertLayer).toHaveBeenCalledWith(0, 3)
  })
})

describe('getLayerArt', () => {
  it('returns empty array when layer has no art', async () => {
    vi.mocked(GetFirstArtOfLayer).mockResolvedValue(0)

    const art = await getLayerArt(bridge, 5)

    expect(GetFirstArtOfLayer).toHaveBeenCalledWith(5)
    expect(art).toEqual([])
  })

  it('walks sibling chain and collects all art handles', async () => {
    vi.mocked(GetFirstArtOfLayer).mockResolvedValue(100)
    vi.mocked(GetArtSibling)
      .mockResolvedValueOnce(101)
      .mockResolvedValueOnce(102)
      .mockResolvedValueOnce(0)

    const art = await getLayerArt(bridge, 5)

    expect(art).toEqual([100, 101, 102])
    expect(GetArtSibling).toHaveBeenCalledTimes(3)
  })
})

describe('getAllLayers', () => {
  it('returns empty array when document has no layers', async () => {
    vi.mocked(CountLayers).mockResolvedValue(0)

    const layers = await getAllLayers(bridge)

    expect(layers).toEqual([])
    expect(GetNthLayer).not.toHaveBeenCalled()
  })

  it('returns all layers with handles and names', async () => {
    vi.mocked(CountLayers).mockResolvedValue(3)
    vi.mocked(GetNthLayer)
      .mockResolvedValueOnce(10)
      .mockResolvedValueOnce(20)
      .mockResolvedValueOnce(30)
    vi.mocked(GetLayerTitle)
      .mockResolvedValueOnce('Layer 1')
      .mockResolvedValueOnce('Layer 2')
      .mockResolvedValueOnce('Layer 3')

    const layers = await getAllLayers(bridge)

    expect(layers).toEqual([
      { handle: 10, name: 'Layer 1' },
      { handle: 20, name: 'Layer 2' },
      { handle: 30, name: 'Layer 3' },
    ])
    expect(GetNthLayer).toHaveBeenCalledTimes(3)
    expect(GetNthLayer).toHaveBeenCalledWith(0)
    expect(GetNthLayer).toHaveBeenCalledWith(1)
    expect(GetNthLayer).toHaveBeenCalledWith(2)
  })
})
