import { describe, it, expect, vi, beforeEach } from 'vitest'
import { createGroup, addToGroup, getGroupChildren, ungroup } from './group'
import { ArtType } from './types'
import type { BridgeCallFn } from './types'

vi.mock('../generated/AIArtSuite', () => ({
  NewArt: vi.fn(),
  ReorderArt: vi.fn(),
  DisposeArt: vi.fn(),
  GetArtFirstChild: vi.fn(),
  GetArtSibling: vi.fn(),
}))

import {
  NewArt,
  ReorderArt,
  DisposeArt,
  GetArtFirstChild,
  GetArtSibling,
} from '../generated/AIArtSuite'

const bridge: BridgeCallFn = vi.fn()

beforeEach(() => {
  vi.clearAllMocks()
})

describe('createGroup', () => {
  it('calls NewArt with kGroupArt and default placement', async () => {
    vi.mocked(NewArt).mockResolvedValue(100)

    const handle = await createGroup(bridge)

    expect(NewArt).toHaveBeenCalledWith(ArtType.kGroupArt, 0, 0)
    expect(handle).toBe(100)
  })

  it('passes custom paintOrder and prep', async () => {
    vi.mocked(NewArt).mockResolvedValue(101)

    const handle = await createGroup(bridge, 2, 50)

    expect(NewArt).toHaveBeenCalledWith(ArtType.kGroupArt, 2, 50)
    expect(handle).toBe(101)
  })
})

describe('addToGroup', () => {
  it('calls ReorderArt with kPlaceInsideOnTop', async () => {
    vi.mocked(ReorderArt).mockResolvedValue(undefined)

    await addToGroup(bridge, 10, 20)

    // kPlaceInsideOnTop = 4
    expect(ReorderArt).toHaveBeenCalledWith(20, 4, 10)
  })
})

describe('getGroupChildren', () => {
  it('returns empty array for group with no children', async () => {
    vi.mocked(GetArtFirstChild).mockResolvedValue(0)

    const children = await getGroupChildren(bridge, 10)

    expect(GetArtFirstChild).toHaveBeenCalledWith(10)
    expect(children).toEqual([])
  })

  it('walks the sibling chain and collects all children', async () => {
    vi.mocked(GetArtFirstChild).mockResolvedValue(1)
    vi.mocked(GetArtSibling)
      .mockResolvedValueOnce(2)
      .mockResolvedValueOnce(3)
      .mockResolvedValueOnce(0) // end of chain

    const children = await getGroupChildren(bridge, 10)

    expect(children).toEqual([1, 2, 3])
    expect(GetArtFirstChild).toHaveBeenCalledWith(10)
    expect(GetArtSibling).toHaveBeenCalledTimes(3)
  })

  it('returns single child when group has one element', async () => {
    vi.mocked(GetArtFirstChild).mockResolvedValue(42)
    vi.mocked(GetArtSibling).mockResolvedValue(0)

    const children = await getGroupChildren(bridge, 5)

    expect(children).toEqual([42])
  })
})

describe('ungroup', () => {
  it('moves all children above group and disposes it', async () => {
    vi.mocked(GetArtFirstChild).mockResolvedValue(1)
    vi.mocked(GetArtSibling)
      .mockResolvedValueOnce(2)
      .mockResolvedValueOnce(0)
    vi.mocked(ReorderArt).mockResolvedValue(undefined)
    vi.mocked(DisposeArt).mockResolvedValue(undefined)

    const children = await ungroup(bridge, 10)

    expect(children).toEqual([1, 2])
    // Each child reordered above the group (paintOrder 0 = kPlaceAbove)
    expect(ReorderArt).toHaveBeenCalledWith(1, 0, 10)
    expect(ReorderArt).toHaveBeenCalledWith(2, 0, 10)
    expect(DisposeArt).toHaveBeenCalledWith(10)
  })

  it('disposes empty group with no children', async () => {
    vi.mocked(GetArtFirstChild).mockResolvedValue(0)
    vi.mocked(DisposeArt).mockResolvedValue(undefined)

    const children = await ungroup(bridge, 10)

    expect(children).toEqual([])
    expect(ReorderArt).not.toHaveBeenCalled()
    expect(DisposeArt).toHaveBeenCalledWith(10)
  })
})
