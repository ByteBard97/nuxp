/**
 * Document Operation Validation Schemas
 *
 * Zod schemas for validating responses from document-related ExtendScript
 * bridge calls. Covers generic document operations -- artboard queries,
 * document initialization, view zoom, and units.
 *
 * Zod is a peer dependency. If your application does not use Zod, you can
 * use the exported TypeScript types directly and skip runtime validation.
 */

import { z } from 'zod'

// ---------------------------------------------------------------------------
// InitializeDocumentResponse
// ---------------------------------------------------------------------------

/**
 * Schema for the response from an `initializeDocument()` bridge call.
 *
 * Success shape includes layers created/existed and document units.
 * Error shape includes an error message with optional layer info.
 */
export const InitializeDocumentResponseSchema = z
  .object({
    success: z.boolean(),
    message: z.string(),
    layersCreated: z.array(z.string()),
    layersExisted: z.array(z.string()),
    totalLayers: z.number(),
    units: z.string(),
  })
  .or(
    z.object({
      success: z.boolean(),
      error: z.string(),
      layersCreated: z.array(z.string()).optional(),
      layersExisted: z.array(z.string()).optional(),
    }),
  )

export type InitializeDocumentResponse = z.infer<typeof InitializeDocumentResponseSchema>

// ---------------------------------------------------------------------------
// ArtboardInfo
// ---------------------------------------------------------------------------

/** Successful artboard info response. */
export const ArtboardInfoSuccessSchema = z.object({
  width: z.number(),
  height: z.number(),
  name: z.string(),
  rect: z.object({
    left: z.number(),
    top: z.number(),
    right: z.number(),
    bottom: z.number(),
  }),
})

/** Error artboard info response. */
export const ArtboardInfoErrorSchema = z.object({
  error: z.string(),
})

/** Combined artboard info schema (success | error). */
export const ArtboardInfoSchema = z.union([
  ArtboardInfoSuccessSchema,
  ArtboardInfoErrorSchema,
])

export type ArtboardInfo = z.infer<typeof ArtboardInfoSchema>
export type ArtboardInfoSuccess = z.infer<typeof ArtboardInfoSuccessSchema>
export type ArtboardInfoError = z.infer<typeof ArtboardInfoErrorSchema>

// ---------------------------------------------------------------------------
// CreateArtboardResponse
// ---------------------------------------------------------------------------

/** Schema for the response from a `createArtboard()` bridge call. */
export const CreateArtboardResponseSchema = z
  .object({
    success: z.boolean(),
    width: z.number(),
    height: z.number(),
    name: z.string(),
  })
  .or(
    z.object({
      error: z.string(),
    }),
  )

export type CreateArtboardResponse = z.infer<typeof CreateArtboardResponseSchema>

// ---------------------------------------------------------------------------
// SuccessResponse
// ---------------------------------------------------------------------------

/** Generic success/error response schema. */
export const SuccessResponseSchema = z
  .object({
    success: z.boolean(),
    message: z.string().optional(),
  })
  .or(
    z.object({
      error: z.string(),
      message: z.string().optional(),
    }),
  )

export type SuccessResponse = z.infer<typeof SuccessResponseSchema>

// ---------------------------------------------------------------------------
// ErrorResponse
// ---------------------------------------------------------------------------

/** A standalone error response schema. */
export const ErrorResponseSchema = z.object({
  error: z.string(),
  message: z.string().optional(),
})

export type ErrorResponse = z.infer<typeof ErrorResponseSchema>

// ---------------------------------------------------------------------------
// ViewZoomResponse
// ---------------------------------------------------------------------------

/** Schema for view zoom get/set responses. */
export const ViewZoomResponseSchema = z
  .object({
    zoom: z.number(),
    success: z.boolean().optional(),
  })
  .or(
    z.object({
      error: z.string(),
    }),
  )

export type ViewZoomResponse = z.infer<typeof ViewZoomResponseSchema>

// ---------------------------------------------------------------------------
// DocumentUnits
// ---------------------------------------------------------------------------

/** Schema for `getDocumentUnits()` responses. */
export const DocumentUnitsSchema = z
  .object({
    units: z.number(),
    unitsName: z.string(),
  })
  .or(
    z.object({
      error: z.string(),
    }),
  )

export type DocumentUnits = z.infer<typeof DocumentUnitsSchema>

/** Schema for `setDocumentUnits()` responses. */
export const SetDocumentUnitsResponseSchema = z
  .object({
    success: z.boolean(),
    units: z.number(),
    unitsName: z.string(),
  })
  .or(
    z.object({
      error: z.string(),
    }),
  )

export type SetDocumentUnitsResponse = z.infer<typeof SetDocumentUnitsResponseSchema>

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------

/**
 * Safely parse a raw bridge response against a Zod schema.
 *
 * @param data - The raw data returned from an ExtendScript bridge call.
 * @param schema - The Zod schema to validate against.
 * @returns The validated, typed response.
 * @throws Error with a descriptive message if validation fails.
 */
export function parseDocumentResponse<T>(data: unknown, schema: z.ZodType<T>): T {
  try {
    return schema.parse(data)
  } catch (error) {
    if (error instanceof z.ZodError) {
      const issues = error.issues
        .map((issue) => `${issue.path.join('.')}: ${issue.message}`)
        .join(', ')
      throw new Error(`Invalid document response from ExtendScript: ${issues}`)
    }
    throw error
  }
}
