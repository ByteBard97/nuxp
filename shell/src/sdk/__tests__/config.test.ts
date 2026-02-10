import { describe, it, expect, beforeEach } from 'vitest'
import {
  DEFAULT_HOST,
  DEFAULT_TIMEOUT_MS,
  getApiUrl,
  sdkConfig,
  updateSdkConfig,
  setPort,
  getPort
} from '../config'

describe('SDK Config', () => {
  describe('constants', () => {
    it('should have correct default host', () => {
      expect(DEFAULT_HOST).toBe('localhost')
    })

    it('should have a reasonable default timeout', () => {
      expect(DEFAULT_TIMEOUT_MS).toBeGreaterThan(0)
      expect(DEFAULT_TIMEOUT_MS).toBeLessThanOrEqual(60000)
    })
  })

  describe('getApiUrl', () => {
    it('should prepend base URL to path', () => {
      const url = getApiUrl('/api/health')
      expect(url).toContain('/api/health')
      expect(url).toMatch(/^http/)
    })

    it('should handle paths without leading slash', () => {
      const url = getApiUrl('api/health')
      expect(url).toContain('/api/health')
    })

    it('should include the configured port', () => {
      const port = getPort()
      const url = getApiUrl('/api/test')
      expect(url).toContain(`:${port}`)
    })
  })

  describe('sdkConfig', () => {
    it('should have required properties', () => {
      expect(sdkConfig).toHaveProperty('baseUrl')
      expect(sdkConfig).toHaveProperty('port')
      expect(sdkConfig).toHaveProperty('timeoutMs')
      expect(sdkConfig).toHaveProperty('useMock')
    })

    it('should have valid port number', () => {
      expect(sdkConfig.port).toBeGreaterThanOrEqual(1024)
      expect(sdkConfig.port).toBeLessThanOrEqual(65535)
    })
  })

  describe('setPort', () => {
    const originalPort = sdkConfig.port

    beforeEach(() => {
      // Reset to original port after each test
      setPort(originalPort)
    })

    it('should update the port', () => {
      setPort(9000)
      expect(getPort()).toBe(9000)
    })

    it('should update the base URL', () => {
      setPort(9000)
      expect(sdkConfig.baseUrl).toContain(':9000')
    })
  })

  describe('updateSdkConfig', () => {
    it('should update timeout', () => {
      const original = sdkConfig.timeoutMs
      updateSdkConfig({ timeoutMs: 5000 })
      expect(sdkConfig.timeoutMs).toBe(5000)
      // Restore
      updateSdkConfig({ timeoutMs: original })
    })
  })
})
