import type { Script } from '../types'
import {
  contrastRatio,
  meetsAA,
  meetsAALarge,
  inchesToPoints,
  pointsToInches,
  mmToPoints,
  pointsToMM,
  feetToInches,
  inchesToFeet,
  isTauri,
  isBrowser,
  getServerUrl,
} from '@nuxp/sdk'

const sdkUtilities: Script = {
  id: 'sdk-utilities',
  name: 'SDK Utilities',
  description: 'Contrast checker, unit conversions, and environment detection',
  category: 'sdk',
  async run() {
    // ── Contrast checking (WCAG 2.1) ────────────────────────────────────
    const whiteOnBlack = contrastRatio('#ffffff', '#000000')
    const whiteOnGray = contrastRatio('#ffffff', '#767676')
    const lowContrast = contrastRatio('#cccccc', '#ffffff')

    const contrastResults = {
      'white on black': {
        ratio: Math.round(whiteOnBlack * 100) / 100,
        meetsAA: meetsAA('#ffffff', '#000000'),
        meetsAALarge: meetsAALarge('#ffffff', '#000000'),
      },
      'white on #767676': {
        ratio: Math.round(whiteOnGray * 100) / 100,
        meetsAA: meetsAA('#ffffff', '#767676'),
        meetsAALarge: meetsAALarge('#ffffff', '#767676'),
      },
      'light gray on white': {
        ratio: Math.round(lowContrast * 100) / 100,
        meetsAA: meetsAA('#cccccc', '#ffffff'),
        meetsAALarge: meetsAALarge('#cccccc', '#ffffff'),
      },
    }

    // ── Unit conversions ────────────────────────────────────────────────
    const unitConversions = {
      '1 inch \u2192 points': inchesToPoints(1),
      '72 points \u2192 inches': pointsToInches(72),
      '25.4 mm \u2192 points': Math.round(mmToPoints(25.4) * 100) / 100,
      '72 points \u2192 mm': Math.round(pointsToMM(72) * 100) / 100,
      '3 feet \u2192 inches': feetToInches(3),
      '36 inches \u2192 feet': inchesToFeet(36),
    }

    // ── Environment detection ───────────────────────────────────────────
    const environment = {
      isTauri: isTauri,
      isBrowser: isBrowser,
      serverUrl: getServerUrl(),
      runtime: isTauri ? 'Tauri desktop app' : 'Browser (dev mode)',
    }

    return {
      success: true,
      message: `Environment: ${environment.runtime}, WCAG checks: 3 color pairs tested`,
      data: {
        contrastResults,
        unitConversions,
        environment,
      },
    }
  },
}

export default sdkUtilities
