/**
 * Global Logger Service with Per-Module Configuration
 *
 * Features:
 * - Captures all console.log/warn/error messages for the Log tab
 * - Per-module logging with enable/disable
 * - Log level filtering (debug < info < warn < error)
 * - Pattern-based module matching
 * - Persistent configuration via localStorage
 * - Zero runtime overhead when modules are disabled
 * - Export as text or JSONL
 * - Subscribe pattern for reactive UI updates
 */

/** Maximum number of log entries retained in the ring buffer */
const DEFAULT_MAX_LOGS = 1000

/** Number of characters to slice from the time string (HH:MM:SS) */
const TIME_SLICE_LENGTH = 8

/**
 * A single log entry captured by the logger.
 */
export interface LogEntry {
  /** Auto-incrementing entry ID */
  id: number
  /** When the log was recorded */
  timestamp: Date
  /** Human-readable time string (HH:MM:SS) */
  time: string
  /** Log category — applications define their own type strings */
  type: string
  /** The log message body */
  message: string
  /** Status indicator character */
  status: '\u2713' | '\u2717' | '\u26A0' | ''
  /** Optional module name that produced this log */
  module?: string
  /** Optional structured data attached to the log */
  data?: any
}

/** Log severity level */
export type LogLevel = 'debug' | 'info' | 'warn' | 'error'

/**
 * A module-scoped logger with level-gated methods.
 */
export interface ModuleLogger {
  debug(message: string, data?: any): void
  info(message: string, data?: any): void
  warn(message: string, data?: any): void
  error(message: string, data?: any): void
}

/**
 * Per-module configuration controlling enabled state and minimum log level.
 */
export interface ModuleConfig {
  enabled: boolean
  level: LogLevel
}

const LOG_LEVEL_PRIORITY: Record<LogLevel, number> = {
  debug: 0,
  info: 1,
  warn: 2,
  error: 3
}

/**
 * Central logging service with ring-buffer storage, per-module filtering,
 * console interception, and a subscriber notification pattern.
 */
class LoggerService {
  private logs: LogEntry[] = []
  private maxLogs = DEFAULT_MAX_LOGS
  private nextId = 1
  private listeners: Array<() => void> = []

  // Per-module configuration
  private moduleConfigs = new Map<string, ModuleConfig>()
  private modulePatterns: Array<{ pattern: RegExp; config: ModuleConfig }> = []
  private globalLevel: LogLevel = this.getDefaultGlobalLevel()
  private registeredModules = new Set<string>()

  // Store original console methods
  private originalConsoleLog = console.log
  private originalConsoleWarn = console.warn
  private originalConsoleError = console.error
  private originalConsoleDebug = console.debug

  constructor() {
    this.loadConfiguration()
    this.interceptConsole()
  }

  /**
   * Get default global log level based on environment.
   * Development defaults to 'debug'; production defaults to 'warn'.
   */
  private getDefaultGlobalLevel(): LogLevel {
    const isDev = (import.meta as any).env?.DEV ?? true
    return isDev ? 'debug' : 'warn'
  }

  /**
   * Load configuration from localStorage.
   */
  private loadConfiguration() {
    try {
      const saved = localStorage.getItem('nuxp_logger_config')
      if (saved) {
        const config = JSON.parse(saved)

        if (config.globalLevel) {
          this.globalLevel = config.globalLevel
        }

        if (config.modules) {
          Object.entries(config.modules).forEach(([name, cfg]: [string, any]) => {
            this.moduleConfigs.set(name, cfg)
          })
        }

        if (config.patterns) {
          this.modulePatterns = config.patterns.map((p: any) => ({
            pattern: new RegExp(p.pattern),
            config: p.config
          }))
        }
      }
    } catch (error) {
      console.warn('[LoggerService] Failed to load config from localStorage:', error)
    }
  }

  /**
   * Save configuration to localStorage.
   */
  private saveConfiguration() {
    try {
      const config = {
        globalLevel: this.globalLevel,
        modules: Object.fromEntries(this.moduleConfigs),
        patterns: this.modulePatterns.map(p => ({
          pattern: p.pattern.source,
          config: p.config
        }))
      }
      localStorage.setItem('nuxp_logger_config', JSON.stringify(config))
    } catch (error) {
      console.warn('[LoggerService] Failed to save config to localStorage:', error)
    }
  }

  /**
   * Check if a module should log at a given level.
   * Returns false immediately if disabled (zero overhead).
   *
   * @param moduleName - The module identifier to check
   * @param level - The log level to test against
   * @returns Whether logging is enabled for this module at this level
   */
  isEnabled(moduleName: string, level: LogLevel): boolean {
    // Check module-specific config first
    const moduleConfig = this.moduleConfigs.get(moduleName)
    if (moduleConfig) {
      if (!moduleConfig.enabled) return false
      return LOG_LEVEL_PRIORITY[level] >= LOG_LEVEL_PRIORITY[moduleConfig.level]
    }

    // Check pattern matches
    for (const { pattern, config } of this.modulePatterns) {
      if (pattern.test(moduleName)) {
        if (!config.enabled) return false
        return LOG_LEVEL_PRIORITY[level] >= LOG_LEVEL_PRIORITY[config.level]
      }
    }

    // Fall back to global level
    return LOG_LEVEL_PRIORITY[level] >= LOG_LEVEL_PRIORITY[this.globalLevel]
  }

  /**
   * Create a module-specific logger with early-exit checks for zero overhead
   * when a module is disabled.
   *
   * @param moduleName - Unique identifier for the module (e.g. 'bridge', 'sync')
   * @returns A ModuleLogger scoped to the given module name
   */
  createLogger(moduleName: string): ModuleLogger {
    // Register module for UI display
    this.registeredModules.add(moduleName)

    return {
      debug: (message: string, data?: any) => {
        if (!this.isEnabled(moduleName, 'debug')) return
        this.addLog('DEBUG', message, '', data, moduleName)
      },

      info: (message: string, data?: any) => {
        if (!this.isEnabled(moduleName, 'info')) return
        this.addLog('INFO', message, '\u2713', data, moduleName)
      },

      warn: (message: string, data?: any) => {
        if (!this.isEnabled(moduleName, 'warn')) return
        this.addLog('WARN', message, '\u26A0', data, moduleName)
      },

      error: (message: string, data?: any) => {
        if (!this.isEnabled(moduleName, 'error')) return
        this.addLog('ERROR', message, '\u2717', data, moduleName)
      }
    }
  }

  // ── Configuration API ───────────────────────────────────────────────────

  /**
   * Enable a module at the specified log level.
   */
  enableModule(name: string, level: LogLevel = 'debug') {
    this.moduleConfigs.set(name, { enabled: true, level })
    this.saveConfiguration()
  }

  /**
   * Disable logging for a module.
   */
  disableModule(name: string) {
    const config = this.moduleConfigs.get(name) || { enabled: false, level: 'debug' as LogLevel }
    config.enabled = false
    this.moduleConfigs.set(name, config)
    this.saveConfiguration()
  }

  /**
   * Enable logging for all modules whose name matches the given pattern.
   */
  enablePattern(pattern: RegExp, level: LogLevel = 'debug') {
    this.modulePatterns.push({
      pattern,
      config: { enabled: true, level }
    })
    this.saveConfiguration()
  }

  /**
   * Set the minimum log level for a specific module.
   */
  setModuleLevel(name: string, level: LogLevel) {
    const config = this.moduleConfigs.get(name) || { enabled: true, level }
    config.level = level
    this.moduleConfigs.set(name, config)
    this.saveConfiguration()
  }

  /**
   * Set the global minimum log level (used when no module-specific config exists).
   */
  setGlobalLevel(level: LogLevel) {
    this.globalLevel = level
    this.saveConfiguration()
  }

  /** Get the current global log level. */
  getGlobalLevel(): LogLevel {
    return this.globalLevel
  }

  /** Get the configuration for a specific module, if any. */
  getModuleConfig(name: string): ModuleConfig | undefined {
    return this.moduleConfigs.get(name)
  }

  /** Get a sorted list of all registered module names. */
  getRegisteredModules(): string[] {
    return Array.from(this.registeredModules).sort()
  }

  /** Get a map of all registered modules and their current configs. */
  getModuleStates(): Record<string, ModuleConfig> {
    const states: Record<string, ModuleConfig> = {}
    this.registeredModules.forEach(name => {
      states[name] = this.getModuleConfig(name) || { enabled: true, level: this.globalLevel }
    })
    return states
  }

  /** Enable all registered modules at debug level. */
  enableAll() {
    this.registeredModules.forEach(name => {
      this.enableModule(name, 'debug')
    })
  }

  /** Disable all registered modules. */
  disableAll() {
    this.registeredModules.forEach(name => {
      this.disableModule(name)
    })
  }

  /** Reset all module configs and patterns to defaults. */
  resetToDefaults() {
    this.moduleConfigs.clear()
    this.modulePatterns = []
    this.globalLevel = this.getDefaultGlobalLevel()
    this.saveConfiguration()
  }

  // ── Console Interception ────────────────────────────────────────────────

  /**
   * Intercept console methods to capture all logs into the ring buffer.
   */
  private interceptConsole() {
    console.log = (...args: any[]) => {
      this.originalConsoleLog.apply(console, args)
      this.addLog('INFO', this.formatMessage(args), '\u2713')
    }

    console.warn = (...args: any[]) => {
      this.originalConsoleWarn.apply(console, args)
      this.addLog('WARN', this.formatMessage(args), '\u26A0')
    }

    console.error = (...args: any[]) => {
      this.originalConsoleError.apply(console, args)
      this.addLog('ERROR', this.formatMessage(args), '\u2717')
    }

    console.debug = (...args: any[]) => {
      this.originalConsoleDebug.apply(console, args)
      this.addLog('DEBUG', this.formatMessage(args), '')
    }
  }

  /**
   * Format console arguments into a single string.
   */
  private formatMessage(args: any[]): string {
    return args.map(arg => {
      if (typeof arg === 'object') {
        try {
          return JSON.stringify(arg, null, 2)
        } catch {
          return String(arg)
        }
      }
      return String(arg)
    }).join(' ')
  }

  // ── Core Logging ────────────────────────────────────────────────────────

  /**
   * Add a log entry to the ring buffer and notify subscribers.
   */
  private addLog(type: string, message: string, status: LogEntry['status'], data?: any, module?: string) {
    const now = new Date()
    const entry: LogEntry = {
      id: this.nextId++,
      timestamp: now,
      time: now.toTimeString().slice(0, TIME_SLICE_LENGTH),
      type,
      message,
      status,
      module,
      data
    }

    this.logs.push(entry)

    // Maintain ring buffer
    if (this.logs.length > this.maxLogs) {
      this.logs.shift()
    }

    this.notifyListeners()
  }

  /**
   * Add a structured log entry.
   *
   * @param type - Log category string (e.g. 'INFO', 'ERROR', or any app-specific type)
   * @param message - The log message
   * @param status - Status indicator character
   * @param data - Optional structured data
   */
  log(type: string, message: string, status: LogEntry['status'] = '\u2713', data?: any) {
    this.addLog(type, message, status, data)
  }

  /** Get a shallow copy of all log entries. */
  getLogs(): LogEntry[] {
    return [...this.logs]
  }

  /** Clear all log entries. */
  clear() {
    this.logs = []
    this.notifyListeners()
  }

  /**
   * Export all logs as a plain-text string (one line per entry).
   */
  exportAsText(): string {
    return this.logs.map(entry =>
      `[${entry.time}] ${entry.type.padEnd(6)} ${entry.status} ${entry.message}`
    ).join('\n')
  }

  /**
   * Export all logs as JSON Lines (one JSON object per line).
   */
  exportAsJSONL(): string {
    return this.logs.map(entry => JSON.stringify({
      timestamp: entry.timestamp.toISOString(),
      type: entry.type,
      message: entry.message,
      status: entry.status,
      data: entry.data
    })).join('\n')
  }

  // ── Subscriptions ───────────────────────────────────────────────────────

  /**
   * Subscribe to log updates. Returns an unsubscribe function.
   *
   * @param callback - Called whenever a new log entry is added
   * @returns A function that removes the subscription when called
   */
  subscribe(callback: () => void) {
    this.listeners.push(callback)
    return () => {
      const index = this.listeners.indexOf(callback)
      if (index >= 0) {
        this.listeners.splice(index, 1)
      }
    }
  }

  /** Notify all subscribers of a log change. */
  private notifyListeners() {
    this.listeners.forEach(callback => callback())
  }

  /**
   * Restore original console methods (for cleanup / testing).
   */
  restore() {
    console.log = this.originalConsoleLog
    console.warn = this.originalConsoleWarn
    console.error = this.originalConsoleError
  }
}

/** Singleton logger instance */
export const logger = new LoggerService()

/**
 * Create a module-scoped logger. Convenience wrapper around the singleton.
 *
 * @param moduleName - Unique identifier for the module (e.g. 'bridge', 'sync')
 * @returns A ModuleLogger scoped to the given module name
 */
export function createLogger(moduleName: string): ModuleLogger {
  return logger.createLogger(moduleName)
}

export { LoggerService }
