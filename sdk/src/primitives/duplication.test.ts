import { describe, it, expect, vi, beforeEach } from 'vitest'
import { duplicateArt, duplicateArtToPosition } from './duplication'
import type { BridgeCallFn } from './types'

vi.mock('../generated/AIArtSuite', () => ({
  DuplicateArt: vi.fn(),
  ReorderArt: vi.fn(),
}))

import { DuplicateArt, ReorderArt } from '../generated/AIArtSuite'

const bridge: BridgeCallFn = vi.fn()

beforeEach(() => {
  vi.clearAllMocks()
})

describe('duplicateArt', () => {
  it('calls DuplicateArt with defaults', async () => {
    vi.mocked(DuplicateArt).mockResolvedValue(200)

    const handle = await duplicateArt(bridge, 50)

    expect(DuplicateArt).toHaveBeenCalledWith(50, 0, 0)
    expect(handle).toBe(200)
  })

  it('passes custom paintOrder and prep', async () => {
    vi.mocked(DuplicateArt).mockResolvedValue(201)

    const handle = await duplicateArt(bridge, 50, 2, 30)

    expect(DuplicateArt).toHaveBeenCalledWith(50, 2, 30)
    expect(handle).toBe(201)
  })
})

describe('duplicateArtToPosition', () => {
  it('duplicates then reorders to target position', async () => {
    vi.mocked(DuplicateArt).mockResolvedValue(300)
    vi.mocked(ReorderArt).mockResolvedValue(undefined)

    const handle = await duplicateArtToPosition(bridge, 50, 70, 4)

    // First duplicates in place (above original)
    expect(DuplicateArt).toHaveBeenCalledWith(50, 0, 0)
    // Then reorders the copy relative to the target
    expect(ReorderArt).toHaveBeenCalledWith(300, 4, 70)
    expect(handle).toBe(300)
  })

  it('returns the new art handle', async () => {
    vi.mocked(DuplicateArt).mockResolvedValue(999)
    vi.mocked(ReorderArt).mockResolvedValue(undefined)

    const handle = await duplicateArtToPosition(bridge, 1, 2, 0)

    expect(handle).toBe(999)
  })
})
