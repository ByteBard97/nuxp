/**
 * NUXP Geometry Utilities
 *
 * Helper functions for creating and manipulating geometric shapes
 * in Adobe Illustrator. All functions use the AIPathSuite and AIArtSuite
 * accessed through SuitePointers.
 *
 * Usage:
 *   AIArtHandle circle = GeometryUtils::CreateCircle(100.0, 200.0, 50.0);
 *   AIArtHandle rect = GeometryUtils::CreateRectangle(0.0, 100.0, 200.0, 0.0);
 */

#ifndef NUXP_GEOMETRY_UTILS_HPP
#define NUXP_GEOMETRY_UTILS_HPP

#include "IllustratorSDK.h"

namespace GeometryUtils {

/**
 * Result of a path area calculation.
 *
 * The signed area indicates winding direction:
 * - Positive: counter-clockwise (CCW) winding
 * - Negative: clockwise (CW) winding
 * The absolute area is always positive regardless of winding.
 */
struct PathAreaResult {
  double area;        ///< Absolute area (always positive)
  double signed_area; ///< Signed area (positive=CCW, negative=CW)
};

/**
 * Create a circle path at the specified center with given radius.
 *
 * Uses 4 bezier curves with kappa = 0.5522847498 for smooth circle
 * approximation.
 *
 * @param centerX X coordinate of the circle center
 * @param centerY Y coordinate of the circle center
 * @param radius Radius of the circle
 * @return Handle to the new path art, or nullptr on failure
 */
AIArtHandle CreateCircle(AIReal centerX, AIReal centerY, AIReal radius);

/**
 * Create a rectangle path from the specified bounds.
 *
 * Creates a closed path with 4 corner points. In Illustrator's coordinate
 * system, Y increases upward, so top > bottom for typical rectangles.
 *
 * @param left Left edge X coordinate
 * @param top Top edge Y coordinate
 * @param right Right edge X coordinate
 * @param bottom Bottom edge Y coordinate
 * @return Handle to the new path art, or nullptr on failure
 */
AIArtHandle CreateRectangle(AIReal left, AIReal top, AIReal right,
                            AIReal bottom);

/**
 * Create a line path between two points.
 *
 * Creates an open path with 2 anchor points.
 *
 * @param x1 Starting point X coordinate
 * @param y1 Starting point Y coordinate
 * @param x2 Ending point X coordinate
 * @param y2 Ending point Y coordinate
 * @return Handle to the new path art, or nullptr on failure
 */
AIArtHandle CreateLine(AIReal x1, AIReal y1, AIReal x2, AIReal y2);

/**
 * Get the bounding rectangle of an art item.
 *
 * Returns the geometric bounds (not including stroke width or effects).
 *
 * @param art Handle to the art item
 * @return Bounding rectangle with left, top, right, bottom coordinates
 */
AIRealRect GetArtBounds(AIArtHandle art);

/**
 * Move an art item by the specified offset.
 *
 * Applies a translation transform to the art.
 *
 * @param art Handle to the art item to move
 * @param dx Horizontal offset (positive = right)
 * @param dy Vertical offset (positive = up)
 */
void MoveArt(AIArtHandle art, AIReal dx, AIReal dy);

/**
 * Scale an art item uniformly from its center.
 *
 * @param art Handle to the art item to scale
 * @param scaleFactor Scale factor (1.0 = no change, 2.0 = double size)
 */
void ScaleArt(AIArtHandle art, AIReal scaleFactor);

/**
 * Calculate the area of a closed path using the shoelace formula.
 *
 * Bezier curve segments are linearized by sampling points along each
 * segment, then the shoelace formula is applied to the resulting polygon.
 * This provides an approximation that is accurate for paths with many
 * sample points per bezier segment.
 *
 * @param art Handle to a path art item (must be kPathArt)
 * @return PathAreaResult with absolute and signed area.
 *         Returns {0.0, 0.0} on failure (null handle, non-path art, etc.)
 */
PathAreaResult CalculatePathArea(AIArtHandle art);

} // namespace GeometryUtils

#endif // NUXP_GEOMETRY_UTILS_HPP
