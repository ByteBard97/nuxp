/**
 * API Helpers -- Centralized error handling for host API results
 *
 * Pure utility functions for unwrapping and validating results
 * returned from C++ plugin bridge calls.
 */

/**
 * Checks an API result for error properties and throws if found.
 * Handles both `{ error: "..." }` and `{ error: true, message: "..." }` shapes.
 *
 * @param result - The parsed API result object
 * @returns The result cast to T if no error was found
 * @throws Error if the result contains an error property
 */
export function unwrapApiResult<T>(result: unknown): T {
  if (result && typeof result === 'object' && 'error' in result) {
    const r = result as Record<string, unknown>
    // Some endpoints return { error: "message string" }
    // Others return { error: true, message: "message string" }
    const message = typeof r.error === 'string'
      ? r.error
      : (r.message as string) || 'Unknown API error'
    throw new Error(message)
  }
  return result as T
}

/**
 * Parses a host result that may be a JSON string or already an object,
 * then checks for error properties.
 *
 * Use this to safely handle raw results from bridge calls where the
 * transport layer may serialize to JSON strings.
 *
 * @param result - Raw result from a host bridge call
 * @param source - Name of the calling function (for error messages)
 * @returns The parsed and validated result cast to T
 * @throws Error if JSON parsing fails or the result contains an error
 */
export function parseHostResult<T>(result: unknown, source: string): T {
  if (typeof result === 'string') {
    try {
      const parsed = JSON.parse(result)
      return unwrapApiResult<T>(parsed)
    } catch (e) {
      if (e instanceof SyntaxError) {
        throw new Error(`Failed to parse result from ${source}: ${result}`)
      }
      throw e // Re-throw unwrapApiResult errors as-is
    }
  }
  return unwrapApiResult<T>(result)
}
