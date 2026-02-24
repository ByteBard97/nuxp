/**
 * @nuxp/sdk - Framework-agnostic TypeScript SDK for Adobe Illustrator plugin communication
 *
 * Public API barrel file. All consumer-facing exports are gathered here.
 */

// ─── Bridge ─────────────────────────────────────────────────────────────────────

export { Bridge, BridgeError, createBridge } from './bridge/Bridge'
export { AutoQueue } from './bridge/AutoQueue'
export type {
  BridgeConfig,
  EndpointDef,
  HttpMethod,
  CppRequest,
  CppResponse,
  QueueStatus,
  BridgeEnvironmentInfo,
} from './bridge/types'

// ─── Adapters ───────────────────────────────────────────────────────────────────

export { HttpAdapter, createHttpAdapter } from './adapters/HttpAdapter'
export type { HttpAdapterConfig } from './adapters/HttpAdapter'

export {
  SSEAdapter,
  createSSEClient,
  connectSSE,
  disconnectSSE,
} from './adapters/SSEAdapter'
export type {
  SSEAdapterConfig,
  SelectionEvent,
  DocumentEvent,
  LayersEvent,
  ArtChangedEvent,
  VersionEvent,
  EventName,
  EventPayloadMap,
  EventCallback,
  WildcardCallback,
} from './adapters/SSEAdapter'

export {
  PluginAdapter,
  createPluginAdapter,
} from './adapters/PluginAdapter'
export type {
  PluginEnvironment,
  PluginAdapterConfig,
  PluginAdapterInstances,
} from './adapters/PluginAdapter'

// ─── Services ───────────────────────────────────────────────────────────────────

export { LoggerService, logger, createLogger } from './services/LoggerService'
export type {
  LogEntry,
  LogLevel,
  ModuleLogger,
  ModuleConfig,
} from './services/LoggerService'

// ─── Tauri ──────────────────────────────────────────────────────────────────────

export {
  tauriFs,
  tauriPath,
  isTauriFsAvailable,
  getAppDataDir,
  exists,
  mkdir,
  readTextFile,
  writeTextFile,
  remove,
} from './tauri/TauriFsWrapper'

export { saveTextFile, saveBinaryFile } from './tauri/TauriDialogService'
export type { SaveDialogResult, FileFilter } from './tauri/TauriDialogService'

// ─── Geometry ───────────────────────────────────────────────────────────────────

export { CoordinateSystemManager } from './geometry/CoordinateSystemManager'
export type { Bounds, Dimensions, EllipseParams } from './geometry/CoordinateSystemManager'

export { ArtboardBoundsManager } from './geometry/ArtboardBoundsManager'

// ─── Primitives ─────────────────────────────────────────────────────────────────

export { ArtType } from './primitives/types'
export type { BridgeCallFn, ArtBounds, ArtChild, PathSegment, ArtboardInfo } from './primitives/types'
export type { LayerInfo } from './primitives/layer'

export * from './primitives/art'
export * from './primitives/text'
export * from './primitives/group'
export * from './primitives/layer'
export * from './primitives/duplication'

// ─── Generated Suites ───────────────────────────────────────────────────────────

export { setBridgeInstance, getBridgeInstance } from './generated/_bridge'

// ─── Types ──────────────────────────────────────────────────────────────────────

export type { Point, Polygon, TintStyle, FillPattern, FillParams, HealResult } from './types/core'

export type {
  ClickEvent,
  SelectionEvent as SSESelectionEvent,
  DeletedEvent,
  PasteEvent,
  DocumentEvent as SSEDocumentEvent,
  VersionEvent as SSEVersionEvent,
  SSEEventType,
  SSEEventPayload,
  SSEEventMap,
} from './types/events'

export type {
  AppearanceColors,
  AppearanceFonts,
  AppearancePreset,
  AppearanceOverrides,
  WanderingSettings,
  RhythmSettings,
  PhysicsMode,
  RhythmMode,
  AppearanceBackground,
  AppearanceOpacity,
} from './types/appearance'

// ─── Utils ──────────────────────────────────────────────────────────────────────

export { unwrapApiResult, parseHostResult } from './utils/apiHelpers'

export { isTauri, isBrowser, getServerUrl, buildServerUrl, HTTP_SERVER_URL } from './utils/environment'
export type { Environment } from './utils/environment'

export {
  calculateSize,
  validateSize,
  calculateStrokeWidth,
  calculateLabelFontSize,
  inchesToPoints,
  pointsToInches,
  mmToPoints,
  pointsToMM,
  feetToInches,
  inchesToFeet,
} from './utils/sizeCalculator'
export type { SizeCalculationInput, SizeCalculation, ValidationResult } from './utils/sizeCalculator'

export { contrastRatio, meetsAA, meetsAALarge, scanForContrastIssues, highlightContrastIssues } from './utils/contrastChecker'
export type { ContrastIssue } from './utils/contrastChecker'

export {
  showError,
  showWarning,
  showConfirm,
  safeSetInterval,
  safeSetTimeout,
  wrapAsyncHandler,
  safeExecute,
  createDebouncedAsync,
  FailureTracker,
} from './utils/asyncErrorHandling'

// ─── Adapters (wave 2) ──────────────────────────────────────────────────────────

export { getDocumentInfo, getUnits, setUnits, getAvailableFonts, getArtboardInfo } from './adapters/DocumentAdapter'
export type { DocumentInfo, DocumentUnits, FontInfo, AvailableFontsResult, UnitName } from './adapters/DocumentAdapter'

export {
  startPlacementPreview,
  updatePlacementPreview,
  finishPlacement,
  cancelPlacement,
  placeSymbolInstance,
  setItemMetadata,
  importAsset,
} from './adapters/PlacementAdapter'
export type {
  PlacementPoint,
  PlacementResult,
  PlaceSymbolOptions,
  ImportAssetOptions as PlacementImportOptions,
} from './adapters/PlacementAdapter'

// ─── Services (wave 2) ──────────────────────────────────────────────────────────

export { SettingsService } from './services/SettingsService'

export { FontConfigService } from './services/FontConfigService'
export type { FontConfig } from './services/FontConfigService'

export { AppearanceConfigService } from './services/AppearanceConfigService'
export type { AppearanceConfigOptions } from './services/AppearanceConfigService'

export { AssetCache, createAssetCache } from './services/SvgCacheService'
export type { AssetCacheOptions, CacheStats } from './services/SvgCacheService'

export { createSvgLoader } from './services/SvgLoader'
export type { SvgLoader, SvgLoadResult, CachedSvgEntry, SvgFetchFn } from './services/SvgLoader'

export { SvgPlacementService } from './services/SvgPlacementService'
export type {
  SvgPlacementOptions,
  SvgValidationResult,
  SvgMetadata,
  PlacementResult as SvgPlacementResult,
} from './services/SvgPlacementService'

export { SymbolManagementService } from './services/SymbolManagementService'
export type { SymbolDefinition, SymbolInfo, ImportAssetOptions as SymbolImportOptions } from './services/SymbolManagementService'

export { DocumentIndexService } from './services/DocumentIndexService'
export type { Identifiable, DocumentIndex, DocumentIndexOptions } from './services/DocumentIndexService'

// ─── Schemas ────────────────────────────────────────────────────────────────────

export {
  InitializeDocumentResponseSchema,
  ArtboardInfoSchema,
  CreateArtboardResponseSchema,
  SuccessResponseSchema,
  ViewZoomResponseSchema,
  DocumentUnitsSchema,
  SetDocumentUnitsResponseSchema,
  ErrorResponseSchema,
  parseDocumentResponse,
} from './schemas/DocumentSchemas'
