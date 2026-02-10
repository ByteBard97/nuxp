import * as fs from 'fs-extra';
import * as path from 'path';
import * as Mustache from 'mustache';

/**
 * SSE Code Generator
 *
 * Generates C++ and TypeScript code for Server-Sent Events based on
 * an events.json configuration file. Ensures type safety between
 * C++ event emission and TypeScript event reception.
 */

// ============================================================================
// Type Definitions
// ============================================================================

/**
 * Represents a field in an event payload
 */
export interface EventPayloadField {
    type: string;           // "string", "number", "boolean", "number[]", "string[]", "object"
    enum?: string[];        // Optional enum values for string types
    description: string;    // Description for documentation
}

/**
 * Represents a single event definition
 */
export interface EventDefinition {
    name: string;                                   // Event name (e.g., "selection")
    description: string;                            // Description for documentation
    payload: Record<string, EventPayloadField>;     // Payload fields
}

/**
 * Root configuration for events.json
 */
export interface EventsConfig {
    extends?: string;           // Optional path to base events.json to extend
    endpoint: string;           // SSE endpoint path (e.g., "/events/stream")
    events: EventDefinition[];  // Array of event definitions
}

/**
 * Represents a generated file
 */
export interface GeneratedFile {
    filename: string;
    content: string;
}

// ============================================================================
// Type Mappings
// ============================================================================

/**
 * Maps JSON schema types to C++ types
 */
const TYPE_TO_CPP: Record<string, string> = {
    'string': 'std::string',
    'number': 'int',
    'boolean': 'bool',
    'number[]': 'std::vector<int>',
    'string[]': 'std::vector<std::string>',
    'object': 'nlohmann::json'
};

/**
 * Maps JSON schema types to TypeScript types
 */
const TYPE_TO_TS: Record<string, string> = {
    'string': 'string',
    'number': 'number',
    'boolean': 'boolean',
    'number[]': 'number[]',
    'string[]': 'string[]',
    'object': 'Record<string, unknown>'
};

// ============================================================================
// Mustache View Interfaces
// ============================================================================

/**
 * View for a single payload field in C++ template
 */
interface CppFieldView {
    name: string;
    cppType: string;
    isLast: boolean;
    description: string;
}

/**
 * View for a single event in C++ template
 */
interface CppEventView {
    name: string;               // Original event name (e.g., "selection")
    functionName: string;       // PascalCase function name (e.g., "Selection")
    description: string;
    fields: CppFieldView[];
    hasFields: boolean;
}

/**
 * View for the entire C++ template
 */
interface CppView {
    events: CppEventView[];
}

/**
 * View for a single payload field in TypeScript template
 */
interface TsFieldView {
    name: string;
    tsType: string;
    description: string;
    isLast: boolean;
}

/**
 * View for a single event in TypeScript template
 */
interface TsEventView {
    name: string;               // Original event name (e.g., "selection")
    interfaceName: string;      // PascalCase interface name (e.g., "SelectionEvent")
    description: string;
    fields: TsFieldView[];
    isLast: boolean;
}

/**
 * View for the entire TypeScript template
 */
interface TsView {
    events: TsEventView[];
    eventNames: string;         // Union type of event names
    eventHandlerRegistrations: string;  // Handler registration lines
    endpoint: string;           // SSE endpoint path from config
}

// ============================================================================
// SSEGenerator Class
// ============================================================================

/**
 * SSEGenerator transforms an events.json configuration into C++ and TypeScript code.
 * Supports extending base configurations via the "extends" field.
 */
export class SSEGenerator {
    private config: EventsConfig;

    /**
     * Creates a new SSEGenerator instance
     * @param configPath - Path to the events.json configuration file
     */
    constructor(configPath: string) {
        this.config = this.loadConfig(configPath);
        this.validateConfig();
    }

    /**
     * Creates a SSEGenerator from an in-memory configuration
     * @param config - The events configuration object
     * @param basePath - Optional base path for resolving extends paths
     */
    static fromConfig(config: EventsConfig, basePath?: string): SSEGenerator {
        const generator = Object.create(SSEGenerator.prototype);

        // If config has extends and basePath is provided, resolve it
        if (config.extends && basePath) {
            const baseConfigPath = path.resolve(path.dirname(basePath), config.extends);
            if (fs.existsSync(baseConfigPath)) {
                const baseConfig = generator.loadConfig.call({ loadConfig: generator.loadConfig }, baseConfigPath);
                generator.config = generator.mergeConfigs(baseConfig, config);
            } else {
                generator.config = config;
            }
        } else {
            generator.config = config;
        }

        generator.validateConfig();
        return generator;
    }

    /**
     * Loads a configuration file, recursively resolving extends
     * @param configPath - Path to the events.json configuration file
     * @returns The merged configuration
     */
    private loadConfig(configPath: string): EventsConfig {
        const configContent = fs.readFileSync(configPath, 'utf-8');
        const config: EventsConfig = JSON.parse(configContent);

        // If this config extends another, load and merge
        if (config.extends) {
            const basePath = path.resolve(path.dirname(configPath), config.extends);

            if (!fs.existsSync(basePath)) {
                throw new Error(`Extended config not found: ${basePath} (referenced from ${configPath})`);
            }

            const baseConfig = this.loadConfig(basePath);
            return this.mergeConfigs(baseConfig, config);
        }

        return config;
    }

    /**
     * Merges a base config with an extension config
     * @param base - The base configuration
     * @param extension - The extension configuration
     * @returns The merged configuration
     */
    private mergeConfigs(base: EventsConfig, extension: EventsConfig): EventsConfig {
        // Create a map of existing events by name for conflict detection
        const baseEventNames = new Set(base.events.map(e => e.name));
        const extensionEventNames = new Set(extension.events.map(e => e.name));

        // Check for duplicate event names (extension overrides base)
        const overriddenEvents: string[] = [];
        for (const name of extensionEventNames) {
            if (baseEventNames.has(name)) {
                overriddenEvents.push(name);
            }
        }

        // Filter out base events that are overridden
        const filteredBaseEvents = base.events.filter(e => !extensionEventNames.has(e.name));

        return {
            // Use extension's endpoint if provided, otherwise use base
            endpoint: extension.endpoint || base.endpoint,
            // Combine events: base events (filtered) + extension events
            events: [...filteredBaseEvents, ...extension.events]
        };
    }

    /**
     * Validates the configuration
     */
    private validateConfig(): void {
        if (!this.config.endpoint) {
            throw new Error('events.json missing required "endpoint" field');
        }
        if (!Array.isArray(this.config.events)) {
            throw new Error('events.json missing required "events" array');
        }
        for (const event of this.config.events) {
            if (!event.name) {
                throw new Error('Event definition missing required "name" field');
            }
            if (!event.payload || typeof event.payload !== 'object') {
                throw new Error(`Event "${event.name}" missing required "payload" object`);
            }
        }
    }

    /**
     * Generates the C++ Events.hpp file
     * @returns The generated C++ file
     */
    generateCpp(): GeneratedFile {
        const view = this.prepareCppView();
        const content = Mustache.render(this.getCppTemplate(), view);
        return {
            filename: 'Events.hpp',
            content
        };
    }

    /**
     * Generates the TypeScript events.ts file
     * @returns The generated TypeScript file
     */
    generateTypeScript(): GeneratedFile {
        const view = this.prepareTsView();
        const content = Mustache.render(this.getTsTemplate(), view);
        return {
            filename: 'events.ts',
            content
        };
    }

    /**
     * Generates both C++ and TypeScript files
     * @returns Array of generated files
     */
    generate(): GeneratedFile[] {
        return [
            this.generateCpp(),
            this.generateTypeScript()
        ];
    }

    // ========================================================================
    // C++ Generation Helpers
    // ========================================================================

    /**
     * Prepares the view data for C++ template rendering
     */
    private prepareCppView(): CppView {
        return {
            events: this.config.events.map(event => this.prepareCppEventView(event))
        };
    }

    /**
     * Prepares view data for a single C++ event
     */
    private prepareCppEventView(event: EventDefinition): CppEventView {
        const fieldEntries = Object.entries(event.payload);
        const fields: CppFieldView[] = fieldEntries.map(([name, field], index) => ({
            name,
            cppType: this.getCppParamType(field),
            description: field.description,
            isLast: index === fieldEntries.length - 1
        }));

        return {
            name: event.name,
            functionName: this.toPascalCase(event.name),
            description: event.description,
            fields,
            hasFields: fields.length > 0
        };
    }

    /**
     * Gets the C++ parameter type for a field, including const ref for complex types
     */
    private getCppParamType(field: EventPayloadField): string {
        const baseType = TYPE_TO_CPP[field.type] || 'nlohmann::json';

        // Pass complex types by const reference
        if (field.type === 'string' || field.type === 'number[]' ||
            field.type === 'string[]' || field.type === 'object') {
            return `const ${baseType}&`;
        }

        return baseType;
    }

    /**
     * Gets the C++ Mustache template
     */
    private getCppTemplate(): string {
        return `#pragma once
// Auto-generated by NUXP Codegen - DO NOT EDIT

#include "../SSE.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Events {

{{#events}}
/**
 * {{description}}
{{#fields}}
 * @param {{name}} {{description}}
{{/fields}}
 */
inline void Emit{{functionName}}({{#fields}}{{{cppType}}} {{name}}{{^isLast}}, {{/isLast}}{{/fields}}) {
    nlohmann::json data;
{{#fields}}
    data["{{name}}"] = {{name}};
{{/fields}}
    SSE::Broadcast("{{name}}", data);
}

{{/events}}
} // namespace Events
`;
    }

    // ========================================================================
    // TypeScript Generation Helpers
    // ========================================================================

    /**
     * Prepares the view data for TypeScript template rendering
     */
    private prepareTsView(): TsView {
        const events = this.config.events.map((event, index) =>
            this.prepareTsEventView(event, index === this.config.events.length - 1)
        );

        // Generate event name union type
        const eventNames = this.config.events.map(e => `'${e.name}'`).join(' | ');

        // Generate handler registration lines
        const eventHandlerRegistrations = this.config.events
            .map(e => `      this.registerHandler('${e.name}');`)
            .join('\n');

        return {
            events,
            eventNames,
            eventHandlerRegistrations,
            endpoint: this.config.endpoint
        };
    }

    /**
     * Prepares view data for a single TypeScript event
     */
    private prepareTsEventView(event: EventDefinition, isLast: boolean): TsEventView {
        const fieldEntries = Object.entries(event.payload);
        const fields: TsFieldView[] = fieldEntries.map(([name, field], index) => ({
            name,
            tsType: this.getTsType(field),
            description: field.description,
            isLast: index === fieldEntries.length - 1
        }));

        return {
            name: event.name,
            interfaceName: this.toPascalCase(event.name) + 'Event',
            description: event.description,
            fields,
            isLast
        };
    }

    /**
     * Gets the TypeScript type for a field, handling enums
     */
    private getTsType(field: EventPayloadField): string {
        // If the field has enum values, generate a union type
        if (field.enum && field.enum.length > 0) {
            return field.enum.map(v => `'${v}'`).join(' | ');
        }

        return TYPE_TO_TS[field.type] || 'unknown';
    }

    /**
     * Gets the TypeScript Mustache template
     */
    private getTsTemplate(): string {
        return `/**
 * SSE Event Types and Client
 * Auto-generated by NUXP Codegen - DO NOT EDIT
 */

import { getApiUrl, sdkConfig } from '../config';

// ============================================================================
// Event Payload Types
// ============================================================================

{{#events}}
/** {{description}} */
export interface {{interfaceName}} {
{{#fields}}
  /** {{description}} */
  {{name}}: {{{tsType}}};
{{/fields}}
}

{{/events}}
// ============================================================================
// Event Type Union and Map
// ============================================================================

/** All possible event names */
export type EventName = {{{eventNames}}};

/** Maps event names to their payload types */
export interface EventPayloadMap {
{{#events}}
  {{name}}: {{interfaceName}};
{{/events}}
}

/** Callback type for a specific event */
export type EventCallback<T extends EventName> = (data: EventPayloadMap[T]) => void;

/** Wildcard callback receives event name and data */
export type WildcardCallback = (event: { type: EventName; data: EventPayloadMap[EventName] }) => void;

// ============================================================================
// SSE Client Class
// ============================================================================

const RECONNECT_BASE_DELAY = 3000;
const MAX_RECONNECT_ATTEMPTS = 10;

/**
 * Server-Sent Events client for receiving plugin events.
 * Uses native EventSource with automatic reconnection.
 */
class SSEClient {
  private eventSource: EventSource | null = null;
  private listeners: Map<string, Set<Function>> = new Map();
  private reconnectAttempts = 0;
  private reconnectTimer: ReturnType<typeof setTimeout> | null = null;
  private manuallyDisconnected = false;

  /**
   * Connect to the SSE endpoint
   */
  connect(): void {
    if (this.eventSource || sdkConfig.useMock) {
      return;
    }

    this.manuallyDisconnected = false;
    const url = getApiUrl('{{{endpoint}}}');

    try {
      this.eventSource = new EventSource(url);

      this.eventSource.onopen = () => {
        console.log('[SSE] Connected');
        this.reconnectAttempts = 0;
      };

      this.eventSource.onerror = () => {
        console.error('[SSE] Connection error');
        this.handleDisconnect();
      };

      // Register handlers for each event type
{{{eventHandlerRegistrations}}}
    } catch (error) {
      console.error('[SSE] Failed to create EventSource:', error);
      this.handleDisconnect();
    }
  }

  /**
   * Disconnect from the SSE endpoint
   */
  disconnect(): void {
    this.manuallyDisconnected = true;

    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
      this.reconnectTimer = null;
    }

    if (this.eventSource) {
      this.eventSource.close();
      this.eventSource = null;
    }

    this.reconnectAttempts = 0;
  }

  /**
   * Subscribe to a specific event type
   */
  on<T extends EventName>(event: T, callback: EventCallback<T>): () => void {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, new Set());
    }
    this.listeners.get(event)!.add(callback);

    return () => this.off(event, callback);
  }

  /**
   * Subscribe to all events
   */
  onAll(callback: WildcardCallback): () => void {
    if (!this.listeners.has('*')) {
      this.listeners.set('*', new Set());
    }
    this.listeners.get('*')!.add(callback);

    return () => {
      this.listeners.get('*')?.delete(callback);
    };
  }

  /**
   * Unsubscribe from an event
   */
  off<T extends EventName>(event: T, callback: EventCallback<T>): void {
    this.listeners.get(event)?.delete(callback);
  }

  /**
   * Check if connected
   */
  get isConnected(): boolean {
    return this.eventSource?.readyState === EventSource.OPEN;
  }

  private registerHandler(eventType: EventName): void {
    if (!this.eventSource) return;

    this.eventSource.addEventListener(eventType, (event: MessageEvent) => {
      try {
        const data = JSON.parse(event.data);
        this.dispatch(eventType, data);
      } catch (error) {
        console.error(\`[SSE] Error parsing \${eventType} event:\`, error);
      }
    });
  }

  private dispatch(eventType: EventName, data: EventPayloadMap[EventName]): void {
    // Call specific listeners
    this.listeners.get(eventType)?.forEach((cb) => {
      try {
        cb(data);
      } catch (error) {
        console.error(\`[SSE] Error in \${eventType} listener:\`, error);
      }
    });

    // Call wildcard listeners
    this.listeners.get('*')?.forEach((cb) => {
      try {
        (cb as WildcardCallback)({ type: eventType, data });
      } catch (error) {
        console.error('[SSE] Error in wildcard listener:', error);
      }
    });
  }

  private handleDisconnect(): void {
    if (this.eventSource) {
      this.eventSource.close();
      this.eventSource = null;
    }

    if (this.manuallyDisconnected) {
      return;
    }

    if (this.reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
      this.reconnectAttempts++;
      const delay = RECONNECT_BASE_DELAY * Math.min(this.reconnectAttempts, 5);

      console.log(\`[SSE] Reconnecting in \${delay}ms (attempt \${this.reconnectAttempts}/\${MAX_RECONNECT_ATTEMPTS})\`);

      this.reconnectTimer = setTimeout(() => {
        this.reconnectTimer = null;
        this.connect();
      }, delay);
    } else {
      console.error('[SSE] Max reconnect attempts reached');
    }
  }
}

// ============================================================================
// Singleton Instance and Exports
// ============================================================================

/** Global SSE client instance */
export const sseClient = new SSEClient();

/** Connect to SSE endpoint */
export const connectSSE = () => sseClient.connect();

/** Disconnect from SSE endpoint */
export const disconnectSSE = () => sseClient.disconnect();

/** Subscribe to a specific event */
export const onEvent = <T extends EventName>(event: T, callback: EventCallback<T>) =>
  sseClient.on(event, callback);

/** Subscribe to all events */
export const onAllEvents = (callback: WildcardCallback) => sseClient.onAll(callback);
`;
    }

    // ========================================================================
    // Utility Methods
    // ========================================================================

    /**
     * Converts a string to PascalCase
     * @param str - The input string (e.g., "selection", "artChanged")
     * @returns PascalCase string (e.g., "Selection", "ArtChanged")
     */
    private toPascalCase(str: string): string {
        return str.charAt(0).toUpperCase() + str.slice(1);
    }
}
