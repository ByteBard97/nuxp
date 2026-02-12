/**
 * NUXP Geometry Utilities Implementation
 *
 * Implementation of geometric shape creation and manipulation functions.
 */

#include "GeometryUtils.hpp"
#include "SuitePointers.hpp"

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

} // namespace GeometryUtils
