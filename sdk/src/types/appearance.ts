/**
 * Appearance Configuration Types
 *
 * Type definitions for the unified appearance/theming system.
 * These types describe color schemes, fonts, backgrounds,
 * and preset themes for UXP panel UI.
 */

/**
 * Named color slots used throughout the UI.
 */
export interface AppearanceColors {
  textPrimary: string
  textSecondary: string
  textMuted: string
  activeTabText: string
  accentPrimary: string
  accentBlue: string
  accentGreen: string
  accentWarm: string
  borderDark: string
  textOnAccent: string
}

/**
 * Font family assignments for different UI contexts.
 */
export interface AppearanceFonts {
  /** Header / display font family */
  header: string
  /** Body text font family */
  body: string
  /** Monospace / code font family */
  mono: string
  /** General UI font family */
  ui: string
}

/**
 * Physics simulation mode for animated backgrounds.
 */
export type PhysicsMode = 'bounce' | 'damped' | 'gravity' | 'hybrid'

/**
 * Rhythm animation mode for pulsing/breathing effects.
 */
export type RhythmMode = 'none' | 'breathing' | 'heartbeat' | 'tidal'

/**
 * Configuration for rhythmic animation effects.
 */
export interface RhythmSettings {
  mode: RhythmMode
  /** Beats/breaths per minute (heartbeat: ~60-80, breathing: ~8-16) */
  rate: number
  /** Scale amplitude -- how much the element grows/shrinks per pulse (0-0.5) */
  depth: number
}

/**
 * Configuration for wandering/floating background animation.
 */
export interface WanderingSettings {
  /** Image URLs for the floating elements */
  images: string[]
  /** Number of floating images to display */
  imageCount: number
  /** Movement speed multiplier */
  speed: number
  /** Opacity of floating images (0-1) */
  imageOpacity: number
  /** Whether wandering animation is active */
  enabled: boolean
  /** Physics mode for movement simulation */
  physicsMode: PhysicsMode
  /** Glass tint color overlay */
  glassTint: string
  /** Rhythm/pulse settings */
  rhythm: RhythmSettings
}

/**
 * Background configuration for the panel UI.
 */
export interface AppearanceBackground {
  type: 'solid' | 'image' | 'gradient' | 'wandering'
  /** Solid or gradient base color */
  color?: string
  /** Background image URL */
  imageUrl?: string
  /** Wandering animation settings */
  wandering?: WanderingSettings
}

/**
 * Opacity settings for UI surfaces.
 */
export interface AppearanceOpacity {
  /** Card/tile opacity (0-1) */
  cards: number
  /** Panel/sidebar opacity (0-1) */
  panels: number
}

/**
 * A complete appearance preset (theme).
 */
export interface AppearancePreset {
  /** Unique preset identifier */
  id: string
  /** Human-readable preset name */
  name: string
  /** Preset category for grouping in the UI */
  category: 'minimal' | 'dark' | 'light' | 'colorful' | 'professional' | string
  /** Optional description */
  description?: string
  /** Color assignments */
  colors: AppearanceColors
  /** Font assignments */
  fonts: AppearanceFonts
  /** Background configuration */
  background: AppearanceBackground
  /** Surface opacity settings */
  opacity: AppearanceOpacity
}

/**
 * User overrides stored in localStorage.
 * Only includes fields the user has explicitly customized.
 */
export interface AppearanceOverrides {
  colors?: Partial<AppearanceColors>
  fonts?: Partial<AppearanceFonts>
  background?: Partial<AppearanceBackground>
  opacity?: Partial<AppearanceOpacity>
  wandering?: Partial<WanderingSettings>
  /** UI blur radius in pixels */
  uiBlur?: number
}
