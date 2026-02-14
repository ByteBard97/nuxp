import * as fs from 'fs-extra';
import * as path from 'path';
import * as Mustache from 'mustache';

/**
 * Custom Route Code Generator
 *
 * Generates C++ and TypeScript code for custom HTTP routes based on
 * a routes.json configuration file. Ensures type safety between
 * C++ handler implementations and TypeScript client functions.
 *
 * Produces three files:
 * - CustomRouteRegistration.cpp  (route wiring)
 * - CustomRouteHandlers.h        (handler declarations)
 * - customRoutes.ts              (typed TypeScript client)
 */

// ============================================================================
// Type Definitions
// ============================================================================

/**
 * Represents a field in a route request, response, or path parameter
 */
export interface RouteField {
    type: string;           // "string", "number", "boolean", "number[]", "string[]", "object"
    description?: string;
    optional?: boolean;
    enum?: string[];
    pattern?: string;       // regex for path params, default "[^/]+"
}

/**
 * Represents a single route definition
 */
export interface RouteDefinition {
    name: string;
    description?: string;
    method: string;         // GET, POST, PUT, DELETE, PATCH
    path: string;
    pathParams?: Record<string, RouteField>;
    request?: Record<string, RouteField>;
    response?: Record<string, RouteField>;
    rawBody?: boolean;
}

/**
 * Root configuration for routes.json
 */
export interface RoutesConfig {
    extends?: string;
    namespace?: string;     // default "CustomHandlers"
    routes: RouteDefinition[];
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
 * Maps JSON schema types to C++ types.
 * Note: "number" maps to "double" (not "int" like SSEGenerator) because
 * custom routes deal with real values like area_sqm.
 */
const TYPE_TO_CPP: Record<string, string> = {
    'string': 'std::string',
    'number': 'double',
    'boolean': 'bool',
    'number[]': 'std::vector<double>',
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

/** Valid HTTP methods */
const VALID_METHODS = ['GET', 'POST', 'PUT', 'DELETE', 'PATCH'];

// ============================================================================
// Mustache View Interfaces
// ============================================================================

/**
 * View for a single path parameter in C++ registration template
 */
interface CppPathParamView {
    name: string;
    index: number;
    isLast: boolean;
}

/**
 * View for a single route in C++ registration template
 */
interface CppRegistrationRouteView {
    name: string;
    method: string;                 // GET, POST, etc.
    methodLower: string;            // get, post, etc.
    methodCapitalized: string;      // Get, Post, etc.
    path: string;
    description: string;
    handlerName: string;            // HandleCreateBed, etc.
    isPattern: boolean;
    regexPattern: string;
    isGet: boolean;
    hasBody: boolean;
    hasPathParams: boolean;
    hasPathParamsAndBody: boolean;
    pathParams: CppPathParamView[];
    firstParamName: string;
    isRawBody: boolean;
}

/**
 * View for the C++ registration template
 */
interface CppRegistrationView {
    namespace: string;
    routes: CppRegistrationRouteView[];
}

/**
 * View for a single route in C++ handler declarations
 */
interface CppDeclRouteView {
    method: string;
    path: string;
    description: string;
    handlerName: string;
    signature: string;
    hasRequest: boolean;
    hasResponse: boolean;
    isRawBody: boolean;
    hasPathParams: boolean;
    pathParamDocs: { name: string; description: string }[];
    requestFieldSummary: string;
    responseFieldSummary: string;
}

/**
 * View for the C++ declarations template
 */
interface CppDeclView {
    namespace: string;
    routes: CppDeclRouteView[];
}

/**
 * View for a single field in TypeScript interfaces
 */
interface TsFieldView {
    name: string;
    tsType: string;
    description: string;
    isOptional: boolean;
    optionalMarker: string;         // "?" or ""
}

/**
 * View for a single route in TypeScript template
 */
interface TsRouteView {
    name: string;                   // camelCase function name
    method: string;
    path: string;
    description: string;
    interfacePrefix: string;        // PascalCase(name)
    hasRequest: boolean;
    hasResponse: boolean;
    isRawBody: boolean;
    hasPathParams: boolean;
    requestFields: TsFieldView[];
    responseFields: TsFieldView[];
    // Function generation helpers
    functionSignature: string;
    functionBody: string;
    responseType: string;
}

/**
 * View for the TypeScript template
 */
interface TsView {
    routes: TsRouteView[];
    hasAnyRequest: boolean;
    hasAnyResponse: boolean;
}

// ============================================================================
// CustomRouteGenerator Class
// ============================================================================

/**
 * CustomRouteGenerator transforms a routes.json configuration into C++ and
 * TypeScript code. Supports extending base configurations via the "extends" field.
 */
export class CustomRouteGenerator {
    private config: RoutesConfig;

    /**
     * Creates a new CustomRouteGenerator instance
     * @param configPath - Path to the routes.json configuration file
     */
    constructor(configPath: string) {
        this.config = this.loadConfig(configPath);
        this.validateConfig();
    }

    /**
     * Creates a CustomRouteGenerator from an in-memory configuration
     * @param config - The routes configuration object
     * @param basePath - Optional base path for resolving extends paths
     */
    static fromConfig(config: RoutesConfig, basePath?: string): CustomRouteGenerator {
        const generator = Object.create(CustomRouteGenerator.prototype);

        // If config has extends and basePath is provided, resolve it
        if (config.extends && basePath) {
            const baseConfigPath = path.resolve(path.dirname(basePath), config.extends);
            if (fs.existsSync(baseConfigPath)) {
                const baseConfig = generator.loadConfig.call(generator, baseConfigPath);
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
     * Generates the C++ route registration file
     * @returns The generated CustomRouteRegistration.cpp file
     */
    generateCppRegistration(): GeneratedFile {
        const view = this.prepareCppRegistrationView();
        const content = Mustache.render(this.getCppRegistrationTemplate(), view);
        return {
            filename: 'CustomRouteRegistration.cpp',
            content
        };
    }

    /**
     * Generates the C++ handler declarations header file
     * @returns The generated CustomRouteHandlers.h file
     */
    generateCppDeclarations(): GeneratedFile {
        const view = this.prepareCppDeclView();
        const content = Mustache.render(this.getCppDeclTemplate(), view);
        return {
            filename: 'CustomRouteHandlers.h',
            content
        };
    }

    /**
     * Generates the TypeScript client file
     * @returns The generated customRoutes.ts file
     */
    generateTypeScript(): GeneratedFile {
        const view = this.prepareTsView();
        const content = Mustache.render(this.getTsTemplate(), view);
        return {
            filename: 'customRoutes.ts',
            content
        };
    }

    /**
     * Generates all three files
     * @returns Array of generated files
     */
    generate(): GeneratedFile[] {
        return [
            this.generateCppRegistration(),
            this.generateCppDeclarations(),
            this.generateTypeScript()
        ];
    }

    // ========================================================================
    // Config Loading
    // ========================================================================

    /**
     * Loads a configuration file, recursively resolving extends
     * @param configPath - Path to the routes.json configuration file
     * @returns The merged configuration
     */
    private loadConfig(configPath: string): RoutesConfig {
        const configContent = fs.readFileSync(configPath, 'utf-8');
        const config: RoutesConfig = JSON.parse(configContent);

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
     * Merges a base config with an extension config.
     * Extension routes override base routes with the same name.
     * Extension namespace takes precedence.
     * @param base - The base configuration
     * @param extension - The extension configuration
     * @returns The merged configuration
     */
    private mergeConfigs(base: RoutesConfig, extension: RoutesConfig): RoutesConfig {
        const extensionRouteNames = new Set(extension.routes.map(r => r.name));

        // Filter out base routes that are overridden by extension
        const filteredBaseRoutes = base.routes.filter(r => !extensionRouteNames.has(r.name));

        return {
            // Extension namespace takes precedence, fall back to base
            namespace: extension.namespace || base.namespace,
            // Combined routes: base routes (minus overrides) + extension routes
            routes: [...filteredBaseRoutes, ...extension.routes]
        };
    }

    /**
     * Validates the configuration
     */
    private validateConfig(): void {
        if (!Array.isArray(this.config.routes)) {
            throw new Error('routes.json missing required "routes" array');
        }

        const seenNames = new Set<string>();
        const seenMethodPaths = new Set<string>();

        for (const route of this.config.routes) {
            // Rule 1: Each route must have name, method, path
            if (!route.name) {
                throw new Error('Route definition missing required "name" field');
            }
            if (!route.method) {
                throw new Error(`Route "${route.name}" missing required "method" field`);
            }
            if (!route.path) {
                throw new Error(`Route "${route.name}" missing required "path" field`);
            }

            // Rule 2: method must be valid
            const upperMethod = route.method.toUpperCase();
            if (!VALID_METHODS.includes(upperMethod)) {
                throw new Error(
                    `Route "${route.name}" has invalid method "${route.method}". ` +
                    `Must be one of: ${VALID_METHODS.join(', ')}`
                );
            }

            // Rule 3: Every {param} in path must have matching pathParams entry
            const pathParamMatches = route.path.match(/\{([^}]+)\}/g);
            if (pathParamMatches) {
                const pathParamNames = pathParamMatches.map(m => m.slice(1, -1));
                for (const paramName of pathParamNames) {
                    if (!route.pathParams || !(paramName in route.pathParams)) {
                        throw new Error(
                            `Route "${route.name}" has path parameter "{${paramName}}" ` +
                            `but no matching entry in pathParams`
                        );
                    }
                }
            }

            // Rule 4: rawBody cannot coexist with request
            if (route.rawBody && route.request) {
                throw new Error(
                    `Route "${route.name}" has both "rawBody: true" and "request". ` +
                    `These are mutually exclusive.`
                );
            }

            // Rule 5: No duplicate route names
            if (seenNames.has(route.name)) {
                throw new Error(`Duplicate route name: "${route.name}"`);
            }
            seenNames.add(route.name);

            // Rule 6: No duplicate method+path combinations
            const methodPath = `${upperMethod} ${route.path}`;
            if (seenMethodPaths.has(methodPath)) {
                throw new Error(`Duplicate method+path combination: "${methodPath}"`);
            }
            seenMethodPaths.add(methodPath);
        }
    }

    // ========================================================================
    // Route Analysis Helpers
    // ========================================================================

    /**
     * Checks if a route has path parameters (pattern route)
     */
    private isPatternRoute(route: RouteDefinition): boolean {
        return /\{[^}]+\}/.test(route.path);
    }

    /**
     * Builds a regex pattern string from a route's path and pathParams.
     * Replaces each {param} with a capturing group using the param's pattern
     * or the default [^/]+.
     */
    private buildRegexPattern(route: RouteDefinition): string {
        let pattern = route.path;
        const pathParamMatches = route.path.match(/\{([^}]+)\}/g);
        if (pathParamMatches) {
            for (const match of pathParamMatches) {
                const paramName = match.slice(1, -1);
                const paramDef = route.pathParams?.[paramName];
                const regex = paramDef?.pattern || '[a-zA-Z0-9_.-]+';
                pattern = pattern.replace(match, `(${regex})`);
            }
        }
        return pattern;
    }

    /**
     * Gets the handler function name for a route
     * e.g. "createBed" -> "HandleCreateBed"
     */
    private getHandlerName(route: RouteDefinition): string {
        return 'Handle' + this.toPascalCase(route.name);
    }

    /**
     * Converts a string to PascalCase
     * @param str - The input string (e.g., "createBed", "getDocInfo")
     * @returns PascalCase string (e.g., "CreateBed", "GetDocInfo")
     */
    private toPascalCase(str: string): string {
        return str.charAt(0).toUpperCase() + str.slice(1);
    }

    /**
     * Determines whether a route has a request body.
     * A route has a body if it has request fields, rawBody, or is a POST/PUT/PATCH
     * with rawBody.
     */
    private routeHasBody(route: RouteDefinition): boolean {
        // rawBody always means body
        if (route.rawBody) return true;
        // Has explicit request fields
        if (route.request && Object.keys(route.request).length > 0) return true;
        return false;
    }

    /**
     * Determines whether a route has path parameters
     */
    private routeHasPathParams(route: RouteDefinition): boolean {
        return this.isPatternRoute(route) && !!route.pathParams && Object.keys(route.pathParams).length > 0;
    }

    /**
     * Gets the ordered list of path parameter names as they appear in the path
     */
    private getOrderedPathParams(route: RouteDefinition): string[] {
        const matches = route.path.match(/\{([^}]+)\}/g);
        if (!matches) return [];
        return matches.map(m => m.slice(1, -1));
    }

    /**
     * Gets the C++ function signature for a handler
     */
    private getCppHandlerSignature(route: RouteDefinition): string {
        const handlerName = this.getHandlerName(route);
        const hasPathParams = this.routeHasPathParams(route);
        const hasBody = this.routeHasBody(route);
        const paramNames = this.getOrderedPathParams(route);

        const params: string[] = [];

        if (hasPathParams) {
            // Path params are passed as individual string parameters
            for (const paramName of paramNames) {
                params.push(`const std::string& ${paramName}`);
            }
        }

        if (hasBody) {
            params.push('const std::string& body');
        }

        return `std::string ${handlerName}(${params.join(', ')})`;
    }

    /**
     * Capitalizes first letter of a string
     */
    private capitalize(str: string): string {
        return str.charAt(0).toUpperCase() + str.slice(1).toLowerCase();
    }

    /**
     * Gets the namespace to use, defaulting to "CustomHandlers"
     */
    private getNamespace(): string {
        return this.config.namespace || 'CustomHandlers';
    }

    // ========================================================================
    // C++ Registration Generation
    // ========================================================================

    /**
     * Prepares the view data for C++ registration template rendering
     */
    private prepareCppRegistrationView(): CppRegistrationView {
        return {
            namespace: this.getNamespace(),
            routes: this.config.routes.map(route => this.prepareCppRegistrationRouteView(route))
        };
    }

    /**
     * Prepares view data for a single route in C++ registration
     */
    private prepareCppRegistrationRouteView(route: RouteDefinition): CppRegistrationRouteView {
        const isPattern = this.isPatternRoute(route);
        const hasPathParams = this.routeHasPathParams(route);
        const hasBody = this.routeHasBody(route);
        const paramNames = this.getOrderedPathParams(route);
        const method = route.method.toUpperCase();

        return {
            name: route.name,
            method,
            methodLower: method.toLowerCase(),
            methodCapitalized: this.capitalize(method),
            path: route.path,
            description: route.description || route.name,
            handlerName: this.getHandlerName(route),
            isPattern,
            regexPattern: isPattern ? this.buildRegexPattern(route) : '',
            isGet: method === 'GET',
            hasBody,
            hasPathParams,
            hasPathParamsAndBody: hasPathParams && hasBody,
            pathParams: paramNames.map((name, index) => ({
                name,
                index,
                isLast: index === paramNames.length - 1
            })),
            firstParamName: paramNames.length > 0 ? paramNames[0] : '',
            isRawBody: !!route.rawBody
        };
    }

    /**
     * Gets the C++ registration Mustache template.
     *
     * This template handles four route categories:
     * 1. Simple route, no body (GET): lambda ignores body param, calls handler with no args
     * 2. Simple route, with body: lambda passes body to handler
     * 3. Pattern route, path params only (no body): extract params, call handler with params
     * 4. Pattern route, path params + body: extract params, call handler with params + body
     */
    private getCppRegistrationTemplate(): string {
        return `// Auto-generated by NUXP Codegen - DO NOT EDIT
#include "CustomRouteHandlers.h"
#include "HttpServer.hpp"
#include <string>
#include <vector>

void RegisterCustomRoutes() {
    using namespace {{{namespace}}};
{{#routes}}

    // {{method}} {{{path}}} - {{description}}
{{#isPattern}}
    HttpServer::{{methodCapitalized}}WithPattern(
        R"({{{regexPattern}}})",
        [](const std::string& body, const std::vector<std::string>& params) {
            if (params.empty()) {
                return std::string("{\\"error\\":\\"missing_{{firstParamName}}\\"}");
            }
{{#hasPathParamsAndBody}}
            return {{handlerName}}({{#pathParams}}params[{{index}}]{{^isLast}}, {{/isLast}}{{/pathParams}}, body);
{{/hasPathParamsAndBody}}
{{^hasPathParamsAndBody}}
            return {{handlerName}}({{#pathParams}}params[{{index}}]{{^isLast}}, {{/isLast}}{{/pathParams}});
{{/hasPathParamsAndBody}}
        });
{{/isPattern}}
{{^isPattern}}
{{^hasBody}}
    HttpServer::{{methodCapitalized}}("{{{path}}}", [](const std::string&) {
        return {{handlerName}}();
    });
{{/hasBody}}
{{#hasBody}}
    HttpServer::{{methodCapitalized}}("{{{path}}}", [](const std::string& body) {
        return {{handlerName}}(body);
    });
{{/hasBody}}
{{/isPattern}}
{{/routes}}
}
`;
    }

    // ========================================================================
    // C++ Declarations Generation
    // ========================================================================

    /**
     * Prepares the view data for C++ declarations template rendering
     */
    private prepareCppDeclView(): CppDeclView {
        return {
            namespace: this.getNamespace(),
            routes: this.config.routes.map(route => this.prepareCppDeclRouteView(route))
        };
    }

    /**
     * Prepares view data for a single route in C++ declarations
     */
    private prepareCppDeclRouteView(route: RouteDefinition): CppDeclRouteView {
        const hasPathParams = this.routeHasPathParams(route);
        const hasBody = this.routeHasBody(route);
        const hasRequest = !!route.request && Object.keys(route.request).length > 0;
        const hasResponse = !!route.response && Object.keys(route.response).length > 0;
        const paramNames = this.getOrderedPathParams(route);

        // Build path param docs
        const pathParamDocs: { name: string; description: string }[] = [];
        if (hasPathParams) {
            for (const paramName of paramNames) {
                const paramDef = route.pathParams![paramName];
                pathParamDocs.push({
                    name: paramName,
                    description: paramDef.description || paramName
                });
            }
        }

        // Build request field summary for @param body doc
        // Use short C++ type names for doc (bool instead of boolean)
        let requestFieldSummary = '';
        if (route.rawBody) {
            requestFieldSummary = 'Raw JSON body (array or object, not parsed into named params)';
        } else if (hasRequest) {
            const fieldParts = Object.entries(route.request!).map(([name, field]) => {
                const optMarker = field.optional ? '?' : '';
                let typeStr = field.type === 'boolean' ? 'bool' : field.type;
                if (field.enum) {
                    typeStr = `string (${field.enum.map(v => `"${v}"`).join(' | ')})`;
                }
                return `${name}${optMarker}: ${typeStr}`;
            });
            requestFieldSummary = `{ ${fieldParts.join(', ')} }`;
        }

        // Build response field summary for @returns doc
        // Use short C++ type names for doc (bool instead of boolean)
        let responseFieldSummary = '';
        if (hasResponse) {
            const fieldParts = Object.entries(route.response!).map(([name, field]) => {
                const docType = field.type === 'boolean' ? 'bool' : field.type;
                return `${name}: ${docType}`;
            });
            responseFieldSummary = `{ ${fieldParts.join(', ')} }`;
        }

        return {
            method: route.method.toUpperCase(),
            path: route.path,
            description: route.description || route.name,
            handlerName: this.getHandlerName(route),
            signature: this.getCppHandlerSignature(route),
            hasRequest: hasRequest || !!route.rawBody,
            hasResponse,
            isRawBody: !!route.rawBody,
            hasPathParams,
            pathParamDocs,
            requestFieldSummary,
            responseFieldSummary
        };
    }

    /**
     * Gets the C++ declarations Mustache template
     */
    private getCppDeclTemplate(): string {
        return `#pragma once
// Auto-generated by NUXP Codegen - DO NOT EDIT
// Implement these functions in your own .cpp files.

#include <string>

namespace {{{namespace}}} {
{{#routes}}

/**
 * {{method}} {{{path}}} - {{description}}
{{#pathParamDocs}}
 * @param {{name}} {{description}}
{{/pathParamDocs}}
{{#hasRequest}}
{{#isRawBody}}
 * @param body {{{requestFieldSummary}}}
{{/isRawBody}}
{{^isRawBody}}
 * @param body JSON: {{{requestFieldSummary}}}
{{/isRawBody}}
{{/hasRequest}}
{{#hasResponse}}
 * @returns JSON: {{{responseFieldSummary}}}
{{/hasResponse}}
 */
{{{signature}}};
{{/routes}}

} // namespace {{{namespace}}}
`;
    }

    // ========================================================================
    // TypeScript Generation
    // ========================================================================

    /**
     * Prepares the view data for TypeScript template rendering
     */
    private prepareTsView(): TsView {
        const routes = this.config.routes.map(route => this.prepareTsRouteView(route));
        return {
            routes,
            hasAnyRequest: routes.some(r => r.hasRequest),
            hasAnyResponse: routes.some(r => r.hasResponse)
        };
    }

    /**
     * Prepares view data for a single route in TypeScript
     */
    private prepareTsRouteView(route: RouteDefinition): TsRouteView {
        const hasPathParams = this.routeHasPathParams(route);
        const hasRequest = !!route.request && Object.keys(route.request).length > 0;
        const hasResponse = !!route.response && Object.keys(route.response).length > 0;
        const interfacePrefix = this.toPascalCase(route.name);
        const paramNames = this.getOrderedPathParams(route);
        const method = route.method.toUpperCase();

        // Prepare request fields
        const requestFields: TsFieldView[] = hasRequest
            ? Object.entries(route.request!).map(([name, field]) => ({
                name,
                tsType: this.getTsType(field),
                description: field.description || '',
                isOptional: !!field.optional,
                optionalMarker: field.optional ? '?' : ''
            }))
            : [];

        // Prepare response fields
        const responseFields: TsFieldView[] = hasResponse
            ? Object.entries(route.response!).map(([name, field]) => ({
                name,
                tsType: this.getTsType(field),
                description: field.description || '',
                isOptional: !!field.optional,
                optionalMarker: field.optional ? '?' : ''
            }))
            : [];

        // Determine response type name
        const responseType = hasResponse ? `${interfacePrefix}Response` : 'void';

        // Build function signature
        const functionSignature = this.buildTsFunctionSignature(
            route.name, interfacePrefix, hasPathParams, hasRequest,
            paramNames, !!route.rawBody, responseType
        );

        // Build function body
        const functionBody = this.buildTsFunctionBody(
            method, route.path, hasPathParams, hasRequest,
            paramNames, !!route.rawBody, responseType
        );

        return {
            name: route.name,
            method,
            path: route.path,
            description: route.description
                ? `${method} ${route.path} - ${route.description}`
                : `${method} ${route.path}`,
            interfacePrefix,
            hasRequest,
            hasResponse,
            isRawBody: !!route.rawBody,
            hasPathParams,
            requestFields,
            responseFields,
            functionSignature,
            functionBody,
            responseType
        };
    }

    /**
     * Gets the TypeScript type for a field, handling enums
     */
    private getTsType(field: RouteField): string {
        // If the field has enum values, generate a union type
        if (field.enum && field.enum.length > 0) {
            return field.enum.map(v => `'${v}'`).join(' | ');
        }

        return TYPE_TO_TS[field.type] || 'unknown';
    }

    /**
     * Builds the TypeScript function signature for a route
     */
    private buildTsFunctionSignature(
        name: string,
        interfacePrefix: string,
        hasPathParams: boolean,
        hasRequest: boolean,
        pathParamNames: string[],
        isRawBody: boolean,
        responseType: string
    ): string {
        const params: string[] = [];

        // Path params come first as individual string arguments
        if (hasPathParams) {
            for (const paramName of pathParamNames) {
                params.push(`${paramName}: string`);
            }
        }

        // Then request body or raw body
        if (isRawBody) {
            params.push('body: string');
        } else if (hasRequest) {
            params.push(`params: ${interfacePrefix}Request`);
        }

        return `${name}(${params.join(', ')}): Promise<${responseType}>`;
    }

    /**
     * Builds the TypeScript function body for a route
     */
    private buildTsFunctionBody(
        method: string,
        routePath: string,
        hasPathParams: boolean,
        hasRequest: boolean,
        pathParamNames: string[],
        isRawBody: boolean,
        responseType: string
    ): string {
        // Build the URL expression
        let urlExpr: string;
        if (hasPathParams) {
            // Build a template literal with encodeURIComponent for each param
            let templatePath = routePath;
            for (const paramName of pathParamNames) {
                templatePath = templatePath.replace(
                    `{${paramName}}`,
                    `\${encodeURIComponent(${paramName})}`
                );
            }
            urlExpr = `getApiUrl(\`${templatePath}\`)`;
        } else {
            urlExpr = `getApiUrl('${routePath}')`;
        }

        // Build the body argument
        let bodyArg: string;
        if (isRawBody) {
            bodyArg = ', body';
        } else if (hasRequest) {
            bodyArg = ', JSON.stringify(params)';
        } else {
            bodyArg = '';
        }

        // Build the fetchRoute call
        const typeParam = responseType === 'void' ? '' : `<${responseType}>`;
        return `return fetchRoute${typeParam}('${method}', ${urlExpr}${bodyArg});`;
    }

    /**
     * Gets the TypeScript Mustache template
     */
    private getTsTemplate(): string {
        return `/**
 * Custom Route Client
 * Auto-generated by NUXP Codegen - DO NOT EDIT
 */

import { getApiUrl } from '../config';

// ============================================================================
// Request/Response Types
// ============================================================================
{{#routes}}
{{#hasRequest}}

/** {{{description}}} */
export interface {{interfacePrefix}}Request {
{{#requestFields}}
{{#description}}
  /** {{{description}}} */
{{/description}}
  {{name}}{{optionalMarker}}: {{{tsType}}};
{{/requestFields}}
}
{{/hasRequest}}
{{#hasResponse}}

{{^hasRequest}}
/** {{{description}}} */
{{/hasRequest}}
export interface {{interfacePrefix}}Response {
{{#responseFields}}
{{#description}}
  /** {{{description}}} */
{{/description}}
  {{name}}{{optionalMarker}}: {{{tsType}}};
{{/responseFields}}
}
{{/hasResponse}}
{{/routes}}

// ============================================================================
// HTTP Client
// ============================================================================

const DEFAULT_TIMEOUT = 10000;

async function fetchRoute<T>(
  method: string,
  url: string,
  body?: string | null,
  timeoutMs: number = DEFAULT_TIMEOUT
): Promise<T> {
  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), timeoutMs);

  try {
    const response = await fetch(url, {
      method,
      headers: { 'Content-Type': 'application/json' },
      body: body ?? undefined,
      signal: controller.signal,
    });
    clearTimeout(timeoutId);

    if (!response.ok) {
      const text = await response.text();
      throw new Error(\`HTTP \${response.status}: \${text}\`);
    }

    const text = await response.text();
    if (!text || text.trim() === '') return {} as T;
    return JSON.parse(text) as T;
  } catch (error) {
    clearTimeout(timeoutId);
    throw error;
  }
}

// ============================================================================
// Route Functions
// ============================================================================
{{#routes}}

/** {{{description}}} */
export async function {{{functionSignature}}} {
  {{{functionBody}}}
}
{{/routes}}
`;
    }

    // ========================================================================
    // Utility Methods
    // ========================================================================
}
