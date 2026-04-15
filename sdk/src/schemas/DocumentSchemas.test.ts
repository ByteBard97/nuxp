import { describe, it, expect } from 'vitest'
import {
  InitializeDocumentResponseSchema,
  ArtboardInfoSuccessSchema,
  ArtboardInfoErrorSchema,
  ArtboardInfoSchema,
  CreateArtboardResponseSchema,
  SuccessResponseSchema,
  ErrorResponseSchema,
  ViewZoomResponseSchema,
  DocumentUnitsSchema,
  SetDocumentUnitsResponseSchema,
  parseDocumentResponse,
} from './DocumentSchemas'

describe('DocumentSchemas', () => {
  // -------------------------------------------------------------------------
  // InitializeDocumentResponseSchema
  // -------------------------------------------------------------------------
  describe('InitializeDocumentResponseSchema', () => {
    it('parses a valid success response', () => {
      const data = {
        success: true,
        message: 'Initialized',
        layersCreated: ['Layer1'],
        layersExisted: ['Layer2'],
        totalLayers: 2,
        units: 'inches',
      }
      expect(InitializeDocumentResponseSchema.parse(data)).toEqual(data)
    })

    it('parses a valid error response', () => {
      const data = {
        success: false,
        error: 'No document open',
      }
      expect(InitializeDocumentResponseSchema.parse(data)).toEqual(data)
    })

    it('rejects data missing required fields', () => {
      expect(() => InitializeDocumentResponseSchema.parse({ success: true })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // ArtboardInfoSchema
  // -------------------------------------------------------------------------
  describe('ArtboardInfoSuccessSchema', () => {
    it('parses valid artboard info', () => {
      const data = {
        width: 100,
        height: 200,
        name: 'Artboard 1',
        rect: { left: 0, top: 0, right: 100, bottom: 200 },
      }
      expect(ArtboardInfoSuccessSchema.parse(data)).toEqual(data)
    })

    it('rejects when rect is missing', () => {
      expect(() =>
        ArtboardInfoSuccessSchema.parse({ width: 100, height: 200, name: 'x' }),
      ).toThrow()
    })
  })

  describe('ArtboardInfoErrorSchema', () => {
    it('parses valid error', () => {
      const data = { error: 'No artboard' }
      expect(ArtboardInfoErrorSchema.parse(data)).toEqual(data)
    })

    it('rejects when error field is missing', () => {
      expect(() => ArtboardInfoErrorSchema.parse({})).toThrow()
    })
  })

  describe('ArtboardInfoSchema (union)', () => {
    it('accepts success shape', () => {
      const data = {
        width: 10,
        height: 20,
        name: 'AB1',
        rect: { left: 0, top: 0, right: 10, bottom: 20 },
      }
      expect(ArtboardInfoSchema.parse(data)).toEqual(data)
    })

    it('accepts error shape', () => {
      expect(ArtboardInfoSchema.parse({ error: 'fail' })).toEqual({ error: 'fail' })
    })

    it('rejects completely invalid data', () => {
      expect(() => ArtboardInfoSchema.parse({ foo: 'bar' })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // CreateArtboardResponseSchema
  // -------------------------------------------------------------------------
  describe('CreateArtboardResponseSchema', () => {
    it('parses valid success response', () => {
      const data = { success: true, width: 800, height: 600, name: 'AB' }
      expect(CreateArtboardResponseSchema.parse(data)).toEqual(data)
    })

    it('parses valid error response', () => {
      const data = { error: 'Could not create' }
      expect(CreateArtboardResponseSchema.parse(data)).toEqual(data)
    })

    it('rejects invalid data', () => {
      expect(() => CreateArtboardResponseSchema.parse({ width: 'not-a-number' })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // SuccessResponseSchema
  // -------------------------------------------------------------------------
  describe('SuccessResponseSchema', () => {
    it('parses success with optional message', () => {
      expect(SuccessResponseSchema.parse({ success: true, message: 'OK' })).toEqual({
        success: true,
        message: 'OK',
      })
    })

    it('parses success without message', () => {
      expect(SuccessResponseSchema.parse({ success: true })).toEqual({ success: true })
    })

    it('parses error shape', () => {
      expect(SuccessResponseSchema.parse({ error: 'bad' })).toEqual({ error: 'bad' })
    })

    it('rejects empty object', () => {
      expect(() => SuccessResponseSchema.parse({})).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // ErrorResponseSchema
  // -------------------------------------------------------------------------
  describe('ErrorResponseSchema', () => {
    it('parses valid error', () => {
      expect(ErrorResponseSchema.parse({ error: 'oops' })).toEqual({ error: 'oops' })
    })

    it('parses error with optional message', () => {
      expect(ErrorResponseSchema.parse({ error: 'oops', message: 'detail' })).toEqual({
        error: 'oops',
        message: 'detail',
      })
    })

    it('rejects missing error field', () => {
      expect(() => ErrorResponseSchema.parse({ message: 'no error field' })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // ViewZoomResponseSchema
  // -------------------------------------------------------------------------
  describe('ViewZoomResponseSchema', () => {
    it('parses zoom value', () => {
      expect(ViewZoomResponseSchema.parse({ zoom: 1.5 })).toEqual({ zoom: 1.5 })
    })

    it('parses zoom with success flag', () => {
      expect(ViewZoomResponseSchema.parse({ zoom: 2, success: true })).toEqual({
        zoom: 2,
        success: true,
      })
    })

    it('parses error shape', () => {
      expect(ViewZoomResponseSchema.parse({ error: 'zoom failed' })).toEqual({
        error: 'zoom failed',
      })
    })

    it('rejects non-numeric zoom', () => {
      expect(() => ViewZoomResponseSchema.parse({ zoom: 'big' })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // DocumentUnitsSchema
  // -------------------------------------------------------------------------
  describe('DocumentUnitsSchema', () => {
    it('parses valid units', () => {
      expect(DocumentUnitsSchema.parse({ units: 2, unitsName: 'inches' })).toEqual({
        units: 2,
        unitsName: 'inches',
      })
    })

    it('parses error shape', () => {
      expect(DocumentUnitsSchema.parse({ error: 'no doc' })).toEqual({ error: 'no doc' })
    })

    it('rejects when units is not a number', () => {
      expect(() => DocumentUnitsSchema.parse({ units: 'two', unitsName: 'inches' })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // SetDocumentUnitsResponseSchema
  // -------------------------------------------------------------------------
  describe('SetDocumentUnitsResponseSchema', () => {
    it('parses valid response', () => {
      const data = { success: true, units: 3, unitsName: 'millimeters' }
      expect(SetDocumentUnitsResponseSchema.parse(data)).toEqual(data)
    })

    it('parses error response', () => {
      expect(SetDocumentUnitsResponseSchema.parse({ error: 'unsupported' })).toEqual({
        error: 'unsupported',
      })
    })

    it('rejects incomplete success shape', () => {
      expect(() => SetDocumentUnitsResponseSchema.parse({ success: true })).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // parseDocumentResponse
  // -------------------------------------------------------------------------
  describe('parseDocumentResponse', () => {
    it('returns typed result on valid data', () => {
      const data = { error: 'something broke' }
      const result = parseDocumentResponse(data, ErrorResponseSchema)
      expect(result).toEqual(data)
    })

    it('throws a descriptive error on invalid data', () => {
      expect(() => parseDocumentResponse({}, ErrorResponseSchema)).toThrow(
        'Invalid document response from C++ plugin',
      )
    })

    it('includes field path info in the error message', () => {
      try {
        parseDocumentResponse({ error: 123 }, ErrorResponseSchema)
        expect.fail('should have thrown')
      } catch (e: unknown) {
        const msg = (e as Error).message
        expect(msg).toContain('error')
        expect(msg).toContain('Invalid document response from C++ plugin')
      }
    })

    it('works with complex schemas (ArtboardInfoSchema)', () => {
      const validSuccess = {
        width: 50,
        height: 80,
        name: 'Test',
        rect: { left: 0, top: 0, right: 50, bottom: 80 },
      }
      expect(parseDocumentResponse(validSuccess, ArtboardInfoSchema)).toEqual(validSuccess)
    })

    it('rethrows non-ZodError exceptions unchanged', () => {
      // Passing a non-Zod schema that throws a generic error is contrived,
      // so we test via a schema whose .parse we override
      const fakeSchema = {
        parse: () => {
          throw new TypeError('something else')
        },
      }
      expect(() => parseDocumentResponse({}, fakeSchema as any)).toThrow(TypeError)
    })
  })
})
