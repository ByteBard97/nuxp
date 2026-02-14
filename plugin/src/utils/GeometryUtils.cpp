/**
 * NUXP Geometry Utilities Implementation
 *
 * Implementation of geometric shape creation and manipulation functions.
 */

#include "GeometryUtils.hpp"
#include "SuitePointers.hpp"

#include <cmath>
#include <vector>

namespace GeometryUtils {

// -------------------------------------------------------------------------
// Constants
// -------------------------------------------------------------------------

/**
 * Kappa value for approximating a circle with 4 bezier curves.
 * kappa = 4/3 * (sqrt(2) - 1) = 0.5522847498
 *
 * This provides optimal control point placement for smooth circle curves.
 */
static const AIReal kCircleKappa = 0.5522847498;

// -------------------------------------------------------------------------
// CreateCircle
// -------------------------------------------------------------------------

AIArtHandle CreateCircle(AIReal centerX, AIReal centerY, AIReal radius) {
  if (!SuitePointers::AIArt() || !SuitePointers::AIPath()) {
    return nullptr;
  }

  // Create a new path art object
  AIArtHandle path = nullptr;
  ASErr error =
      SuitePointers::AIArt()->NewArt(kPathArt, kPlaceAboveAll, nullptr, &path);
  if (error != kNoErr || path == nullptr) {
    return nullptr;
  }

  // Calculate control point distance from anchor points
  AIReal controlDist = radius * kCircleKappa;

  // Build 4 bezier segments for the circle
  // Starting from top and going clockwise: Top -> Right -> Bottom -> Left
  AIPathSegment segments[4];

  // Segment 0: Top (centerX, centerY + radius)
  segments[0].p.h = centerX;
  segments[0].p.v = centerY + radius;
  segments[0].in.h = centerX - controlDist; // Control point from Left
  segments[0].in.v = centerY + radius;
  segments[0].out.h = centerX + controlDist; // Control point to Right
  segments[0].out.v = centerY + radius;
  segments[0].corner = false;

  // Segment 1: Right (centerX + radius, centerY)
  segments[1].p.h = centerX + radius;
  segments[1].p.v = centerY;
  segments[1].in.h = centerX + radius;
  segments[1].in.v = centerY + controlDist; // From Top
  segments[1].out.h = centerX + radius;
  segments[1].out.v = centerY - controlDist; // To Bottom
  segments[1].corner = false;

  // Segment 2: Bottom (centerX, centerY - radius)
  segments[2].p.h = centerX;
  segments[2].p.v = centerY - radius;
  segments[2].in.h = centerX + controlDist; // From Right
  segments[2].in.v = centerY - radius;
  segments[2].out.h = centerX - controlDist; // To Left
  segments[2].out.v = centerY - radius;
  segments[2].corner = false;

  // Segment 3: Left (centerX - radius, centerY)
  segments[3].p.h = centerX - radius;
  segments[3].p.v = centerY;
  segments[3].in.h = centerX - radius;
  segments[3].in.v = centerY - controlDist; // From Bottom
  segments[3].out.h = centerX - radius;
  segments[3].out.v = centerY + controlDist; // To Top
  segments[3].corner = false;

  // Set the path segments
  error = SuitePointers::AIPath()->SetPathSegments(path, 0, 4, segments);
  if (error != kNoErr) {
    SuitePointers::AIArt()->DisposeArt(path);
    return nullptr;
  }

  // Close the path to complete the circle
  error = SuitePointers::AIPath()->SetPathClosed(path, true);
  if (error != kNoErr) {
    SuitePointers::AIArt()->DisposeArt(path);
    return nullptr;
  }

  return path;
}

// -------------------------------------------------------------------------
// CreateRectangle
// -------------------------------------------------------------------------

AIArtHandle CreateRectangle(AIReal left, AIReal top, AIReal right,
                            AIReal bottom) {
  if (!SuitePointers::AIArt() || !SuitePointers::AIPath()) {
    return nullptr;
  }

  // Create a new path art object
  AIArtHandle path = nullptr;
  ASErr error =
      SuitePointers::AIArt()->NewArt(kPathArt, kPlaceAboveAll, nullptr, &path);
  if (error != kNoErr || path == nullptr) {
    return nullptr;
  }

  // Build 4 corner segments for the rectangle
  // Going clockwise from top-left: TL -> TR -> BR -> BL
  AIPathSegment segments[4];

  // Segment 0: Top-Left corner
  segments[0].p.h = left;
  segments[0].p.v = top;
  segments[0].in.h = left; // Corner points have coincident control points
  segments[0].in.v = top;
  segments[0].out.h = left;
  segments[0].out.v = top;
  segments[0].corner = true;

  // Segment 1: Top-Right corner
  segments[1].p.h = right;
  segments[1].p.v = top;
  segments[1].in.h = right;
  segments[1].in.v = top;
  segments[1].out.h = right;
  segments[1].out.v = top;
  segments[1].corner = true;

  // Segment 2: Bottom-Right corner
  segments[2].p.h = right;
  segments[2].p.v = bottom;
  segments[2].in.h = right;
  segments[2].in.v = bottom;
  segments[2].out.h = right;
  segments[2].out.v = bottom;
  segments[2].corner = true;

  // Segment 3: Bottom-Left corner
  segments[3].p.h = left;
  segments[3].p.v = bottom;
  segments[3].in.h = left;
  segments[3].in.v = bottom;
  segments[3].out.h = left;
  segments[3].out.v = bottom;
  segments[3].corner = true;

  // Set the path segments
  error = SuitePointers::AIPath()->SetPathSegments(path, 0, 4, segments);
  if (error != kNoErr) {
    SuitePointers::AIArt()->DisposeArt(path);
    return nullptr;
  }

  // Close the path to complete the rectangle
  error = SuitePointers::AIPath()->SetPathClosed(path, true);
  if (error != kNoErr) {
    SuitePointers::AIArt()->DisposeArt(path);
    return nullptr;
  }

  return path;
}

// -------------------------------------------------------------------------
// CreateLine
// -------------------------------------------------------------------------

AIArtHandle CreateLine(AIReal x1, AIReal y1, AIReal x2, AIReal y2) {
  AIArtSuite *artSuite = SuitePointers::AIArt();
  AIPathSuite *pathSuite = SuitePointers::AIPath();

  if (artSuite == nullptr || pathSuite == nullptr) {
    return nullptr;
  }

  // Create a new path art object
  AIArtHandle path = nullptr;
  ASErr error = artSuite->NewArt(kPathArt, kPlaceAboveAll, nullptr, &path);
  if (error != kNoErr || path == nullptr) {
    return nullptr;
  }

  // Build 2 endpoint segments for the line
  AIPathSegment segments[2];

  // Segment 0: Start point
  segments[0].p.h = x1;
  segments[0].p.v = y1;
  segments[0].in.h = x1; // Line endpoints have coincident control points
  segments[0].in.v = y1;
  segments[0].out.h = x1;
  segments[0].out.v = y1;
  segments[0].corner = true;

  // Segment 1: End point
  segments[1].p.h = x2;
  segments[1].p.v = y2;
  segments[1].in.h = x2;
  segments[1].in.v = y2;
  segments[1].out.h = x2;
  segments[1].out.v = y2;
  segments[1].corner = true;

  // Set the path segments
  error = pathSuite->SetPathSegments(path, 0, 2, segments);
  if (error != kNoErr) {
    artSuite->DisposeArt(path);
    return nullptr;
  }

  // Keep the path open for a line
  error = pathSuite->SetPathClosed(path, false);
  if (error != kNoErr) {
    artSuite->DisposeArt(path);
    return nullptr;
  }

  return path;
}

// -------------------------------------------------------------------------
// GetArtBounds
// -------------------------------------------------------------------------

AIRealRect GetArtBounds(AIArtHandle art) {
  AIRealRect bounds = {0, 0, 0, 0};

  AIArtSuite *artSuite = SuitePointers::AIArt();
  if (artSuite == nullptr || art == nullptr) {
    return bounds;
  }

  // Get geometric bounds (not visual bounds which include stroke/effects)
  ASErr error = artSuite->GetArtBounds(art, &bounds);
  if (error != kNoErr) {
    // Return zero rect on error
    bounds.left = bounds.top = bounds.right = bounds.bottom = 0;
  }

  return bounds;
}

// -------------------------------------------------------------------------
// MoveArt
// -------------------------------------------------------------------------

void MoveArt(AIArtHandle art, AIReal dx, AIReal dy) {
  AITransformArtSuite *transformSuite = SuitePointers::AITransformArt();
  if (transformSuite == nullptr || art == nullptr) {
    return;
  }

  // Create a translation matrix
  AIRealMatrix matrix;
  matrix.a = 1.0; // Scale X
  matrix.b = 0.0; // Shear Y
  matrix.c = 0.0; // Shear X
  matrix.d = 1.0; // Scale Y
  matrix.tx = dx; // Translate X
  matrix.ty = dy; // Translate Y

  // Apply the transformation
  // Flags: 0 means transform the art itself (not patterns, etc.)
  transformSuite->TransformArt(art, &matrix, 1.0, kTransformObjects);
}

// -------------------------------------------------------------------------
// ScaleArt
// -------------------------------------------------------------------------

void ScaleArt(AIArtHandle art, AIReal scaleFactor) {
  AIArtSuite *artSuite = SuitePointers::AIArt();
  AITransformArtSuite *transformSuite = SuitePointers::AITransformArt();
  if (artSuite == nullptr || transformSuite == nullptr || art == nullptr) {
    return;
  }

  // Get the current bounds to find the center
  AIRealRect bounds;
  ASErr error = artSuite->GetArtBounds(art, &bounds);
  if (error != kNoErr) {
    return;
  }

  // Calculate center of the art
  AIReal centerX = (bounds.left + bounds.right) / 2.0;
  AIReal centerY = (bounds.top + bounds.bottom) / 2.0;

  // Create a scale-from-center transformation matrix
  // This is equivalent to: translate to origin, scale, translate back
  // Combined matrix: [s, 0, 0, s, cx*(1-s), cy*(1-s)]
  AIRealMatrix matrix;
  matrix.a = scaleFactor;                    // Scale X
  matrix.b = 0.0;                            // Shear Y
  matrix.c = 0.0;                            // Shear X
  matrix.d = scaleFactor;                    // Scale Y
  matrix.tx = centerX * (1.0 - scaleFactor); // Translate X
  matrix.ty = centerY * (1.0 - scaleFactor); // Translate Y

  // Apply the transformation
  transformSuite->TransformArt(art, &matrix, 1.0, kTransformObjects);
}

// -------------------------------------------------------------------------
// CalculatePathArea Helpers
// -------------------------------------------------------------------------

/**
 * Number of sample points per bezier curve segment for linearization.
 * Higher values give more accurate area at the cost of computation.
 * 16 provides sub-pixel accuracy for typical Illustrator paths.
 */
static const int kBezierSamples = 16;

/**
 * Evaluate a cubic bezier curve at parameter t.
 *
 * B(t) = (1-t)^3 * P0 + 3*(1-t)^2*t * P1 + 3*(1-t)*t^2 * P2 + t^3 * P3
 *
 * @param p0 Start anchor point
 * @param p1 First control point (out handle of start)
 * @param p2 Second control point (in handle of end)
 * @param p3 End anchor point
 * @param t Parameter value in [0, 1]
 * @return The point on the curve at parameter t
 */
static AIRealPoint EvalBezier(AIRealPoint p0, AIRealPoint p1, AIRealPoint p2,
                              AIRealPoint p3, double t) {
  double u = 1.0 - t;
  double u2 = u * u;
  double u3 = u2 * u;
  double t2 = t * t;
  double t3 = t2 * t;

  AIRealPoint result;
  result.h = static_cast<AIReal>(u3 * p0.h + 3.0 * u2 * t * p1.h +
                                 3.0 * u * t2 * p2.h + t3 * p3.h);
  result.v = static_cast<AIReal>(u3 * p0.v + 3.0 * u2 * t * p1.v +
                                 3.0 * u * t2 * p2.v + t3 * p3.v);
  return result;
}

/**
 * Check if a bezier segment is actually a straight line.
 * A segment is linear if both control points coincide with their
 * respective anchor points.
 */
static bool IsLinearSegment(AIRealPoint anchor, AIRealPoint outCtrl,
                            AIRealPoint inCtrl, AIRealPoint nextAnchor) {
  const AIReal kEpsilon = 0.001;
  return (std::fabs(outCtrl.h - anchor.h) < kEpsilon &&
          std::fabs(outCtrl.v - anchor.v) < kEpsilon &&
          std::fabs(inCtrl.h - nextAnchor.h) < kEpsilon &&
          std::fabs(inCtrl.v - nextAnchor.v) < kEpsilon);
}

// -------------------------------------------------------------------------
// CalculatePathArea
// -------------------------------------------------------------------------

PathAreaResult CalculatePathArea(AIArtHandle art) {
  PathAreaResult result = {0.0, 0.0};

  AIPathSuite *pathSuite = SuitePointers::AIPath();
  AIArtSuite *artSuite = SuitePointers::AIArt();
  if (pathSuite == nullptr || artSuite == nullptr || art == nullptr) {
    return result;
  }

  // Verify this is a path art object
  short artType = 0;
  ASErr error = artSuite->GetArtType(art, &artType);
  if (error != kNoErr || artType != kPathArt) {
    return result;
  }

  // Get segment count
  short segmentCount = 0;
  error = pathSuite->GetPathSegmentCount(art, &segmentCount);
  if (error != kNoErr || segmentCount < 2) {
    return result;
  }

  // Get all path segments
  std::vector<AIPathSegment> segments(segmentCount);
  error = pathSuite->GetPathSegments(art, 0, segmentCount, segments.data());
  if (error != kNoErr) {
    return result;
  }

  // Check if path is closed (area is only meaningful for closed paths)
  AIBoolean closed = false;
  error = pathSuite->GetPathClosed(art, &closed);
  if (error != kNoErr || !closed) {
    return result;  // Returns {0.0, 0.0} for open paths
  }

  // Linearize all bezier segments into a polygon
  // For each segment pair, sample points along the bezier curve
  std::vector<AIRealPoint> polygon;
  polygon.reserve(segmentCount * kBezierSamples);

  int totalSegments = closed ? segmentCount : (segmentCount - 1);

  for (int i = 0; i < totalSegments; ++i) {
    int nextIdx = (i + 1) % segmentCount;

    // Bezier control points:
    // P0 = current anchor point
    // P1 = current segment's out control point
    // P2 = next segment's in control point
    // P3 = next anchor point
    AIRealPoint p0 = segments[i].p;
    AIRealPoint p1 = segments[i].out;
    AIRealPoint p2 = segments[nextIdx].in;
    AIRealPoint p3 = segments[nextIdx].p;

    if (IsLinearSegment(p0, p1, p2, p3)) {
      // Straight line - just add the start point
      polygon.push_back(p0);
    } else {
      // Bezier curve - sample points along the curve
      // Start at t=0 (anchor point), stop before t=1 (next segment handles it)
      for (int s = 0; s < kBezierSamples; ++s) {
        double t = static_cast<double>(s) / static_cast<double>(kBezierSamples);
        polygon.push_back(EvalBezier(p0, p1, p2, p3, t));
      }
    }
  }

  // If the path is not closed, add the last anchor point
  if (!closed) {
    polygon.push_back(segments[segmentCount - 1].p);
  }

  // Apply the shoelace formula
  // signed_area = 0.5 * sum of (x_i * y_{i+1} - x_{i+1} * y_i)
  if (polygon.size() < 3) {
    return result;
  }

  double sum = 0.0;
  size_t n = polygon.size();
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    sum += static_cast<double>(polygon[i].h) *
               static_cast<double>(polygon[j].v) -
           static_cast<double>(polygon[j].h) *
               static_cast<double>(polygon[i].v);
  }

  result.signed_area = sum * 0.5;
  result.area = std::fabs(result.signed_area);

  return result;
}

} // namespace GeometryUtils
