import { describe, it, expect } from 'vitest'
import { unwrapApiResult, parseHostResult } from './apiHelpers'

describe('unwrapApiResult', () => {
  it('returns the value when there is no error property', () => {
    const result = { data: 'hello', count: 42 }
    expect(unwrapApiResult(result)).toBe(result)
  })

  it('passes through primitive values', () => {
    expect(unwrapApiResult(123)).toBe(123)
    expect(unwrapApiResult('hello')).toBe('hello')
    expect(unwrapApiResult(true)).toBe(true)
  })

  it('passes through null and undefined', () => {
    expect(unwrapApiResult(null)).toBeNull()
    expect(unwrapApiResult(undefined)).toBeUndefined()
  })

  it('throws when result has { error: "message" } shape', () => {
    expect(() => unwrapApiResult({ error: 'Something went wrong' }))
      .toThrow('Something went wrong')
  })

  it('throws when result has { error: true, message: "..." } shape', () => {
    expect(() => unwrapApiResult({ error: true, message: 'Bad request' }))
      .toThrow('Bad request')
  })

  it('throws with "Unknown API error" when error is true but no message', () => {
    expect(() => unwrapApiResult({ error: true }))
      .toThrow('Unknown API error')
  })

  it('does not throw for objects without an error property', () => {
    expect(unwrapApiResult({ success: true })).toEqual({ success: true })
  })

  it('returns arrays unchanged', () => {
    const arr = [1, 2, 3]
    expect(unwrapApiResult(arr)).toBe(arr)
  })
})

describe('parseHostResult', () => {
  it('parses a valid JSON string and returns the object', () => {
    const json = JSON.stringify({ data: 'ok' })
    expect(parseHostResult(json, 'testFn')).toEqual({ data: 'ok' })
  })

  it('passes through objects directly (non-string)', () => {
    const obj = { data: 'ok' }
    expect(parseHostResult(obj, 'testFn')).toBe(obj)
  })

  it('throws on invalid JSON with source name in message', () => {
    expect(() => parseHostResult('not-valid-json', 'loadSettings'))
      .toThrow('Failed to parse result from loadSettings')
  })

  it('includes the raw string in the error message on invalid JSON', () => {
    expect(() => parseHostResult('{{broken', 'fetchData'))
      .toThrow('{{broken')
  })

  it('throws API error when parsed JSON contains an error', () => {
    const json = JSON.stringify({ error: 'Server error' })
    expect(() => parseHostResult(json, 'testFn'))
      .toThrow('Server error')
  })

  it('throws API error when passed object contains an error', () => {
    expect(() => parseHostResult({ error: true, message: 'Timeout' }, 'testFn'))
      .toThrow('Timeout')
  })

  it('handles null and undefined inputs', () => {
    expect(parseHostResult(null, 'testFn')).toBeNull()
    expect(parseHostResult(undefined, 'testFn')).toBeUndefined()
  })

  it('handles numeric inputs', () => {
    expect(parseHostResult(42, 'testFn')).toBe(42)
  })
})
