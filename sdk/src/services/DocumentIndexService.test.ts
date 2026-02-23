import { describe, it, expect, beforeEach, vi } from 'vitest'
import { DocumentIndexService } from './DocumentIndexService'

interface TestItem {
  id: string
  name: string
  x: number
  y: number
}

describe('DocumentIndexService', () => {
  let mockBridge: ReturnType<typeof vi.fn>
  let svc: DocumentIndexService<TestItem>

  beforeEach(() => {
    mockBridge = vi.fn()
    svc = new DocumentIndexService<TestItem>(mockBridge)
  })

  // -------------------------------------------------------------------------
  // CRUD
  // -------------------------------------------------------------------------

  describe('CRUD operations', () => {
    it('addItem and getItem', () => {
      const item: TestItem = { id: 'w1', name: 'Button', x: 10, y: 20 }
      svc.addItem(item)
      expect(svc.getItem('w1')).toEqual(item)
    })

    it('getItem returns undefined for missing id', () => {
      expect(svc.getItem('missing')).toBeUndefined()
    })

    it('getAllItems returns all items as array', () => {
      svc.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      svc.addItem({ id: 'b', name: 'B', x: 1, y: 1 })
      const all = svc.getAllItems()
      expect(all).toHaveLength(2)
      expect(all.map(i => i.id).sort()).toEqual(['a', 'b'])
    })

    it('getItemCount returns correct count', () => {
      expect(svc.getItemCount()).toBe(0)
      svc.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      expect(svc.getItemCount()).toBe(1)
      svc.addItem({ id: 'b', name: 'B', x: 1, y: 1 })
      expect(svc.getItemCount()).toBe(2)
    })

    it('updateItem merges partial fields', () => {
      svc.addItem({ id: 'w1', name: 'Button', x: 10, y: 20 })
      svc.updateItem('w1', { name: 'Updated', x: 99 })
      const item = svc.getItem('w1')!
      expect(item.name).toBe('Updated')
      expect(item.x).toBe(99)
      expect(item.y).toBe(20) // untouched
    })

    it('updateItem warns and does nothing for non-existent id', () => {
      const warnSpy = vi.spyOn(console, 'warn').mockImplementation(() => {})
      svc.updateItem('nope', { name: 'X' })
      expect(warnSpy).toHaveBeenCalled()
      warnSpy.mockRestore()
    })

    it('removeItem deletes by ID', () => {
      svc.addItem({ id: 'w1', name: 'Button', x: 10, y: 20 })
      svc.removeItem('w1')
      expect(svc.getItem('w1')).toBeUndefined()
      expect(svc.getItemCount()).toBe(0)
    })

    it('removeItem warns for non-existent id', () => {
      const warnSpy = vi.spyOn(console, 'warn').mockImplementation(() => {})
      svc.removeItem('nope')
      expect(warnSpy).toHaveBeenCalled()
      warnSpy.mockRestore()
    })
  })

  // -------------------------------------------------------------------------
  // readIndex
  // -------------------------------------------------------------------------

  describe('readIndex', () => {
    it('parses JSON string result from bridge', async () => {
      const indexData = {
        schemaVersion: 1,
        createdAt: '2024-01-01T00:00:00.000Z',
        updatedAt: '2024-01-01T00:00:00.000Z',
        items: { w1: { id: 'w1', name: 'Button', x: 10, y: 20 } },
      }
      mockBridge.mockResolvedValue(
        JSON.stringify({ success: true, data: JSON.stringify(indexData) })
      )

      const result = await svc.readIndex()

      expect(result).not.toBeNull()
      expect(result!.items.w1.name).toBe('Button')
      expect(svc.getItem('w1')?.name).toBe('Button')
    })

    it('parses object result from bridge (data as object)', async () => {
      const indexData = {
        schemaVersion: 1,
        createdAt: '2024-01-01T00:00:00.000Z',
        updatedAt: '2024-01-01T00:00:00.000Z',
        items: { w1: { id: 'w1', name: 'Widget', x: 0, y: 0 } },
      }
      mockBridge.mockResolvedValue({ success: true, data: indexData })

      const result = await svc.readIndex()

      expect(result).not.toBeNull()
      expect(result!.items.w1.name).toBe('Widget')
    })

    it('returns null for expected "no_index" error', async () => {
      mockBridge.mockResolvedValue(JSON.stringify({ error: 'no_index' }))

      const result = await svc.readIndex()
      expect(result).toBeNull()
    })

    it('returns null on bridge exception', async () => {
      const errorSpy = vi.spyOn(console, 'error').mockImplementation(() => {})
      mockBridge.mockRejectedValue(new Error('bridge down'))

      const result = await svc.readIndex()
      expect(result).toBeNull()
      errorSpy.mockRestore()
    })

    it('returns null when bridge returns null', async () => {
      mockBridge.mockResolvedValue(null)

      const result = await svc.readIndex()
      expect(result).toBeNull()
    })
  })

  // -------------------------------------------------------------------------
  // writeIndex
  // -------------------------------------------------------------------------

  describe('writeIndex', () => {
    it('calls bridge with serialized JSON and returns true on success', async () => {
      mockBridge.mockResolvedValue({ success: true })
      svc.addItem({ id: 'w1', name: 'A', x: 0, y: 0 })

      const ok = await svc.writeIndex()

      expect(ok).toBe(true)
      expect(mockBridge).toHaveBeenCalledWith(
        'writeIndexToXMP',
        expect.any(String)
      )
      // Verify the serialized data contains our item
      const jsonArg = mockBridge.mock.calls[0][1] as string
      const parsed = JSON.parse(jsonArg)
      expect(parsed.items.w1.name).toBe('A')
    })

    it('returns false on bridge error', async () => {
      const errorSpy = vi.spyOn(console, 'error').mockImplementation(() => {})
      mockBridge.mockResolvedValue({ error: 'write failed' })

      const ok = await svc.writeIndex()
      expect(ok).toBe(false)
      errorSpy.mockRestore()
    })

    it('returns false when bridge throws', async () => {
      const errorSpy = vi.spyOn(console, 'error').mockImplementation(() => {})
      mockBridge.mockRejectedValue(new Error('crash'))

      const ok = await svc.writeIndex()
      expect(ok).toBe(false)
      errorSpy.mockRestore()
    })

    it('returns false when success is falsy', async () => {
      const errorSpy = vi.spyOn(console, 'error').mockImplementation(() => {})
      mockBridge.mockResolvedValue({ success: false })

      const ok = await svc.writeIndex()
      expect(ok).toBe(false)
      errorSpy.mockRestore()
    })
  })

  // -------------------------------------------------------------------------
  // needsSave / clear / metadata
  // -------------------------------------------------------------------------

  describe('state queries', () => {
    it('needsSave() is true after addItem, false after writeIndex', async () => {
      expect(svc.needsSave()).toBe(false)
      svc.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      expect(svc.needsSave()).toBe(true)

      mockBridge.mockResolvedValue({ success: true })
      await svc.writeIndex()
      expect(svc.needsSave()).toBe(false)
    })

    it('needsSave() is true after updateItem', () => {
      svc.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      // Reset dirty flag by hand via a write mock
      mockBridge.mockResolvedValue({ success: true })

      svc.updateItem('a', { x: 99 })
      expect(svc.needsSave()).toBe(true)
    })

    it('needsSave() is true after removeItem', () => {
      svc.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      svc.removeItem('a')
      expect(svc.needsSave()).toBe(true)
    })
  })

  describe('clear', () => {
    it('resets to empty state', () => {
      svc.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      svc.clear()
      expect(svc.getItemCount()).toBe(0)
      expect(svc.getAllItems()).toEqual([])
      expect(svc.needsSave()).toBe(false)
    })
  })

  describe('metadata', () => {
    it('set and get metadata values', () => {
      svc.metadata('version', '2.0')
      expect(svc.metadata('version')).toBe('2.0')
    })

    it('setting metadata marks index as dirty', () => {
      svc.metadata('key', 'val')
      expect(svc.needsSave()).toBe(true)
    })

    it('get returns undefined for unset metadata key', () => {
      expect(svc.metadata('nonexistent')).toBeUndefined()
    })
  })

  // -------------------------------------------------------------------------
  // Custom bridge function names
  // -------------------------------------------------------------------------

  describe('custom options', () => {
    it('uses custom read/write function names', async () => {
      const bridge = vi.fn().mockResolvedValue({ success: true })
      const custom = new DocumentIndexService<TestItem>(bridge, {
        readFunctionName: 'customRead',
        writeFunctionName: 'customWrite',
      })

      custom.addItem({ id: 'a', name: 'A', x: 0, y: 0 })
      await custom.writeIndex()
      expect(bridge).toHaveBeenCalledWith('customWrite', expect.any(String))

      bridge.mockResolvedValue(null)
      await custom.readIndex()
      expect(bridge).toHaveBeenCalledWith('customRead')
    })
  })
})
