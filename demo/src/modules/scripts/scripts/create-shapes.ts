import type { Script } from '../types'
import { CreateRectangle } from '@nuxp/sdk/generated/customRoutes'

const createShapes: Script = {
  id: 'create-shapes',
  name: 'Create Shape',
  description: 'Create a rectangle at a given position and size using AIPathSuite',
  category: 'objects',
  params: [
    { name: 'x', label: 'X (pts)', type: 'number', default: 100 },
    { name: 'y', label: 'Y (pts)', type: 'number', default: -100 },
    { name: 'width', label: 'Width (pts)', type: 'number', default: 200, min: 1 },
    { name: 'height', label: 'Height (pts)', type: 'number', default: 150, min: 1 },
  ],
  async run(params) {
    const result = await CreateRectangle({
      x: params.x as number,
      y: params.y as number,
      width: params.width as number,
      height: params.height as number,
    })

    return {
      success: true,
      message: `Created rectangle (handle ${result.handle}, uuid ${result.uuid})`,
      data: {
        handle: result.handle,
        uuid: result.uuid,
        position: { x: params.x, y: params.y },
        size: { width: params.width, height: params.height },
        availableFunctions: [
          'CreateRectangle({ x, y, width, height }) — rectangle from origin + size',
          'CreateEllipse({ cx, cy, rx, ry }) — ellipse from center + radii',
          'CreateLine({ x1, y1, x2, y2 }) — line between two points',
          'CreatePath({ segments, closed? }) — arbitrary path from segments',
        ],
      },
    }
  },
}

export default createShapes
