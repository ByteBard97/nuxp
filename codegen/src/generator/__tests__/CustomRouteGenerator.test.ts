import { CustomRouteGenerator, RoutesConfig, RouteDefinition, RouteField } from '../CustomRouteGenerator';
import * as fs from 'fs-extra';
import * as path from 'path';
import * as os from 'os';

describe('CustomRouteGenerator', () => {
    let generator: CustomRouteGenerator;
    let tempDir: string;
    let configPath: string;

    /**
     * Standard test config with common route types for testing
     */
    const standardTestConfig: RoutesConfig = {
        namespace: 'TestHandlers',
        routes: [
            {
                name: 'createBed',
                description: 'Creates a bed from the current selection',
                method: 'POST',
                path: '/beds/create',
                request: {
                    bedId: { type: 'string', description: 'Unique bed identifier' },
                    bedName: { type: 'string', description: 'Display name', optional: true }
                },
                response: {
                    success: { type: 'boolean' },
                    bedId: { type: 'string' },
                    area_sqm: { type: 'number' }
                }
            },
            {
                name: 'deleteBed',
                description: 'Deletes a bed by its ID',
                method: 'DELETE',
                path: '/beds/{id}',
                pathParams: {
                    id: { type: 'string', pattern: '[a-zA-Z0-9_-]+', description: 'Bed identifier' }
                },
                response: {
                    success: { type: 'boolean' },
                    deleted: { type: 'number' }
                }
            },
            {
                name: 'getDocInfo',
                description: 'Returns document information',
                method: 'GET',
                path: '/doc/info',
                response: {
                    name: { type: 'string' },
                    path: { type: 'string' },
                    artboardCount: { type: 'number' }
                }
            },
            {
                name: 'placeBatch',
                description: 'Places multiple plants from a JSON array',
                method: 'POST',
                path: '/plant/place-batch',
                rawBody: true,
                response: {
                    success: { type: 'boolean' },
                    placed: { type: 'number' }
                }
            },
            {
                name: 'switchDisplayMode',
                description: 'Switches display mode for a single plant',
                method: 'POST',
                path: '/plant/{uuid}/display-mode',
                pathParams: {
                    uuid: { type: 'string', pattern: '[a-f0-9-]+', description: 'Plant UUID' }
                },
                request: {
                    mode: { type: 'string', enum: ['circle', 'svg', 'symbol'], description: 'Display mode' }
                },
                response: {
                    success: { type: 'boolean' }
                }
            }
        ]
    };

    beforeEach(async () => {
        // Create temp dir with test routes.json
        tempDir = await fs.mkdtemp(path.join(os.tmpdir(), 'route-test-'));
        configPath = path.join(tempDir, 'routes.json');

        await fs.writeFile(configPath, JSON.stringify(standardTestConfig, null, 2));
        generator = new CustomRouteGenerator(configPath);
    });

    afterEach(async () => {
        await fs.remove(tempDir);
    });

    // =========================================================================
    // Config Parsing Tests
    // =========================================================================

    describe('Config parsing', () => {
        it('should parse routes.json and generate output', () => {
            const result = generator.generateCppRegistration();

            expect(result.filename).toBe('CustomRouteRegistration.cpp');
            expect(result.content).toBeTruthy();
        });

        it('should throw error for non-existent config file', () => {
            expect(() => {
                new CustomRouteGenerator('/nonexistent/path/routes.json');
            }).toThrow();
        });

        it('should throw error for invalid JSON', async () => {
            const invalidPath = path.join(tempDir, 'invalid.json');
            await fs.writeFile(invalidPath, 'not valid json {{{');

            expect(() => {
                new CustomRouteGenerator(invalidPath);
            }).toThrow();
        });

        it('should handle empty routes array', async () => {
            const emptyConfig: RoutesConfig = { routes: [] };
            await fs.writeFile(configPath, JSON.stringify(emptyConfig));

            const emptyGenerator = new CustomRouteGenerator(configPath);
            const result = emptyGenerator.generateCppRegistration();

            expect(result.filename).toBe('CustomRouteRegistration.cpp');
        });

        it('should throw error for missing routes array', async () => {
            const invalidConfig = { namespace: 'Test' };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/routes/);
        });

        it('should throw error for route missing name', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [{ name: '', method: 'GET', path: '/test' } as any]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/name/);
        });

        it('should throw error for route missing method', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [{ name: 'test', path: '/test' } as any]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/method/);
        });

        it('should throw error for route missing path', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [{ name: 'test', method: 'GET' } as any]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/path/);
        });

        it('should throw error for invalid method', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [{ name: 'test', method: 'CONNECT', path: '/test' }]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/method/i);
        });

        it('should support creating generator from in-memory config', () => {
            const memGenerator = CustomRouteGenerator.fromConfig(standardTestConfig);
            const result = memGenerator.generateCppRegistration();

            expect(result.filename).toBe('CustomRouteRegistration.cpp');
            expect(result.content).toContain('HandleCreateBed');
        });
    });

    // =========================================================================
    // Config Validation Tests
    // =========================================================================

    describe('Config validation', () => {
        it('should throw when path param has no matching pathParams entry', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [{
                    name: 'test',
                    method: 'DELETE',
                    path: '/items/{id}',
                    // No pathParams defined
                }]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/param/i);
        });

        it('should throw when rawBody is true and request is also provided', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [{
                    name: 'test',
                    method: 'POST',
                    path: '/test',
                    rawBody: true,
                    request: {
                        field: { type: 'string' }
                    }
                }]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/rawBody/i);
        });

        it('should throw for duplicate route names', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [
                    { name: 'duplicate', method: 'GET', path: '/a' },
                    { name: 'duplicate', method: 'POST', path: '/b' }
                ]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/duplicate/i);
        });

        it('should throw for duplicate method+path combinations', async () => {
            const invalidConfig: RoutesConfig = {
                routes: [
                    { name: 'routeA', method: 'GET', path: '/same' },
                    { name: 'routeB', method: 'GET', path: '/same' }
                ]
            };
            await fs.writeFile(configPath, JSON.stringify(invalidConfig));

            expect(() => {
                new CustomRouteGenerator(configPath);
            }).toThrow(/duplicate/i);
        });

        it('should accept valid path params with matching pathParams', () => {
            const validConfig: RoutesConfig = {
                routes: [{
                    name: 'getItem',
                    method: 'GET',
                    path: '/items/{id}',
                    pathParams: {
                        id: { type: 'string', description: 'Item ID' }
                    },
                    response: {
                        name: { type: 'string' }
                    }
                }]
            };

            const gen = CustomRouteGenerator.fromConfig(validConfig);
            const result = gen.generateCppRegistration();

            expect(result.content).toBeTruthy();
        });

        it('should accept rawBody without request', () => {
            const validConfig: RoutesConfig = {
                routes: [{
                    name: 'rawRoute',
                    method: 'POST',
                    path: '/raw',
                    rawBody: true,
                    response: {
                        success: { type: 'boolean' }
                    }
                }]
            };

            const gen = CustomRouteGenerator.fromConfig(validConfig);
            const result = gen.generateCppRegistration();

            expect(result.content).toBeTruthy();
        });
    });

    // =========================================================================
    // C++ Registration Generation Tests
    // =========================================================================

    describe('C++ registration generation', () => {
        describe('File structure', () => {
            it('should generate file with filename CustomRouteRegistration.cpp', () => {
                const result = generator.generateCppRegistration();

                expect(result.filename).toBe('CustomRouteRegistration.cpp');
            });

            it('should include auto-generated header comment with DO NOT EDIT', () => {
                const result = generator.generateCppRegistration();

                expect(result.content).toContain('Auto-generated');
                expect(result.content).toContain('DO NOT EDIT');
            });

            it('should include necessary headers', () => {
                const result = generator.generateCppRegistration();

                expect(result.content).toContain('CustomRouteHandlers.h');
                expect(result.content).toContain('HttpServer.hpp');
            });

            it('should use correct namespace from config', () => {
                const result = generator.generateCppRegistration();

                expect(result.content).toContain('TestHandlers');
            });

            it('should use default namespace when config does not provide one', async () => {
                const noNsConfig: RoutesConfig = {
                    routes: [{
                        name: 'test',
                        method: 'GET',
                        path: '/test',
                        response: { ok: { type: 'boolean' } }
                    }]
                };
                await fs.writeFile(configPath, JSON.stringify(noNsConfig));
                const gen = new CustomRouteGenerator(configPath);
                const result = gen.generateCppRegistration();

                expect(result.content).toContain('CustomHandlers');
            });
        });

        describe('Route registration by method', () => {
            it('should generate HttpServer::Post() for POST routes without path params', () => {
                const result = generator.generateCppRegistration();

                // createBed is POST /beds/create with no path params
                expect(result.content).toMatch(/HttpServer::Post\(\s*"\/beds\/create"/);
            });

            it('should generate HttpServer::Get() for GET routes without path params', () => {
                const result = generator.generateCppRegistration();

                // getDocInfo is GET /doc/info
                expect(result.content).toMatch(/HttpServer::Get\(\s*"\/doc\/info"/);
            });

            it('should generate HttpServer::Delete() for DELETE routes without path params', async () => {
                const config: RoutesConfig = {
                    routes: [{
                        name: 'clearAll',
                        method: 'DELETE',
                        path: '/items/clear',
                        response: { success: { type: 'boolean' } }
                    }]
                };
                await fs.writeFile(configPath, JSON.stringify(config));
                const gen = new CustomRouteGenerator(configPath);
                const result = gen.generateCppRegistration();

                expect(result.content).toMatch(/HttpServer::Delete\(\s*"\/items\/clear"/);
            });

            it('should generate HttpServer::PostWithPattern() for POST routes with path params', () => {
                const result = generator.generateCppRegistration();

                // switchDisplayMode is POST /plant/{uuid}/display-mode with pathParams
                expect(result.content).toContain('HttpServer::PostWithPattern');
            });

            it('should generate HttpServer::DeleteWithPattern() for DELETE routes with path params', () => {
                const result = generator.generateCppRegistration();

                // deleteBed is DELETE /beds/{id} with pathParams
                expect(result.content).toContain('HttpServer::DeleteWithPattern');
            });
        });

        describe('Path param handling', () => {
            it('should generate correct regex from path params with pattern', () => {
                const result = generator.generateCppRegistration();

                // deleteBed: /beds/{id} with pattern [a-zA-Z0-9_-]+
                expect(result.content).toContain('/beds/([a-zA-Z0-9_-]+)');
            });

            it('should use default pattern [^/]+ when no pattern specified', async () => {
                const config: RoutesConfig = {
                    routes: [{
                        name: 'getItem',
                        method: 'GET',
                        path: '/items/{id}',
                        pathParams: {
                            id: { type: 'string', description: 'Item ID' }
                            // No pattern specified
                        },
                        response: { name: { type: 'string' } }
                    }]
                };
                await fs.writeFile(configPath, JSON.stringify(config));
                const gen = new CustomRouteGenerator(configPath);
                const result = gen.generateCppRegistration();

                expect(result.content).toContain('/items/([^/]+)');
            });

            it('should include params empty guard for pattern routes', () => {
                const result = generator.generateCppRegistration();

                // Pattern routes should check that params are not empty
                expect(result.content).toContain('params.empty()');
            });
        });

        describe('Handler invocation', () => {
            it('should pass body to handler for POST routes', () => {
                const result = generator.generateCppRegistration();

                // createBed: POST with request body
                expect(result.content).toMatch(/HandleCreateBed\(.*body/);
            });

            it('should pass params[0] to handler for single path param routes', () => {
                const result = generator.generateCppRegistration();

                // deleteBed: DELETE /beds/{id}
                expect(result.content).toMatch(/HandleDeleteBed\(.*params\[0\]/);
            });

            it('should pass both params[0] and body for routes with path params AND body', () => {
                const result = generator.generateCppRegistration();

                // switchDisplayMode: POST /plant/{uuid}/display-mode with request body
                expect(result.content).toMatch(/HandleSwitchDisplayMode\(.*params\[0\].*body/);
            });

            it('should call handler with no args for GET routes without path params', () => {
                const result = generator.generateCppRegistration();

                // getDocInfo: GET /doc/info
                expect(result.content).toContain('HandleGetDocInfo()');
            });
        });

        describe('Documentation', () => {
            it('should include route description as comment', () => {
                const result = generator.generateCppRegistration();

                expect(result.content).toContain('Creates a bed from the current selection');
                expect(result.content).toContain('Deletes a bed by its ID');
            });
        });
    });

    // =========================================================================
    // C++ Declarations Generation Tests
    // =========================================================================

    describe('C++ declarations generation', () => {
        describe('File structure', () => {
            it('should generate file with filename CustomRouteHandlers.h', () => {
                const result = generator.generateCppDeclarations();

                expect(result.filename).toBe('CustomRouteHandlers.h');
            });

            it('should include #pragma once', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('#pragma once');
            });

            it('should include auto-generated comment', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('Auto-generated');
                expect(result.content).toContain('DO NOT EDIT');
            });

            it('should include <string> header', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('#include <string>');
            });

            it('should wrap declarations in namespace', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('namespace TestHandlers');
            });
        });

        describe('Handler declarations', () => {
            it('should generate HandleCreateBed(const std::string& body) for POST with body', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toMatch(/HandleCreateBed\(const std::string& body\)/);
            });

            it('should generate HandleDeleteBed(const std::string& id) for DELETE with path param', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toMatch(/HandleDeleteBed\(const std::string& id\)/);
            });

            it('should generate HandleGetDocInfo() for GET with no params', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('HandleGetDocInfo()');
            });

            it('should generate HandlePlaceBatch(const std::string& body) for rawBody POST', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toMatch(/HandlePlaceBatch\(const std::string& body\)/);
            });

            it('should generate HandleSwitchDisplayMode(const std::string& uuid, const std::string& body) for path param + body', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toMatch(/HandleSwitchDisplayMode\(const std::string& uuid, const std::string& body\)/);
            });
        });

        describe('Documentation', () => {
            it('should include JSDoc with route info', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('/**');
                expect(result.content).toContain(' */');
                expect(result.content).toContain('Creates a bed from the current selection');
            });

            it('should include @param descriptions', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('@param');
            });

            it('should include @returns with response fields', () => {
                const result = generator.generateCppDeclarations();

                expect(result.content).toContain('@returns');
                // Should describe response fields
                expect(result.content).toContain('success');
            });

            it('should show optional fields with ? in JSDoc request descriptions', () => {
                const result = generator.generateCppDeclarations();

                // createBed has optional bedName field
                expect(result.content).toContain('bedName?');
            });
        });
    });

    // =========================================================================
    // TypeScript Generation Tests
    // =========================================================================

    describe('TypeScript generation', () => {
        describe('File structure', () => {
            it('should generate file with filename customRoutes.ts', () => {
                const result = generator.generateTypeScript();

                expect(result.filename).toBe('customRoutes.ts');
            });

            it('should include auto-generated comment', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('Auto-generated');
                expect(result.content).toContain('DO NOT EDIT');
            });

            it('should include import from config for getApiUrl', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain("from '../config'");
                expect(result.content).toContain('getApiUrl');
            });
        });

        describe('Request interfaces', () => {
            it('should generate Request interface for routes with request fields', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('export interface CreateBedRequest');
                expect(result.content).toContain('export interface SwitchDisplayModeRequest');
            });

            it('should not generate Request interface for routes without request', () => {
                const result = generator.generateTypeScript();

                // getDocInfo and deleteBed have no request fields
                expect(result.content).not.toContain('GetDocInfoRequest');
                expect(result.content).not.toContain('DeleteBedRequest');
            });

            it('should not generate Request interface for rawBody routes', () => {
                const result = generator.generateTypeScript();

                // placeBatch is rawBody, no structured request
                expect(result.content).not.toContain('PlaceBatchRequest');
            });
        });

        describe('Response interfaces', () => {
            it('should generate Response interface for all routes with response fields', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('export interface CreateBedResponse');
                expect(result.content).toContain('export interface DeleteBedResponse');
                expect(result.content).toContain('export interface GetDocInfoResponse');
                expect(result.content).toContain('export interface PlaceBatchResponse');
                expect(result.content).toContain('export interface SwitchDisplayModeResponse');
            });
        });

        describe('Field types', () => {
            it('should generate correct string field types', () => {
                const result = generator.generateTypeScript();

                // CreateBedRequest should have bedId: string
                expect(result.content).toContain('bedId: string');
            });

            it('should generate correct number field types', () => {
                const result = generator.generateTypeScript();

                // CreateBedResponse should have area_sqm: number
                expect(result.content).toContain('area_sqm: number');
            });

            it('should generate correct boolean field types', () => {
                const result = generator.generateTypeScript();

                // Response success fields
                expect(result.content).toContain('success: boolean');
            });

            it('should generate number[] field types', async () => {
                const config: RoutesConfig = {
                    routes: [{
                        name: 'test',
                        method: 'POST',
                        path: '/test',
                        request: { ids: { type: 'number[]', description: 'IDs' } },
                        response: { success: { type: 'boolean' } }
                    }]
                };
                const gen = CustomRouteGenerator.fromConfig(config);
                const result = gen.generateTypeScript();

                expect(result.content).toContain('ids: number[]');
            });

            it('should generate string[] field types', async () => {
                const config: RoutesConfig = {
                    routes: [{
                        name: 'test',
                        method: 'POST',
                        path: '/test',
                        request: { names: { type: 'string[]', description: 'Names' } },
                        response: { success: { type: 'boolean' } }
                    }]
                };
                const gen = CustomRouteGenerator.fromConfig(config);
                const result = gen.generateTypeScript();

                expect(result.content).toContain('names: string[]');
            });

            it('should generate Record<string, unknown> for object type', async () => {
                const config: RoutesConfig = {
                    routes: [{
                        name: 'test',
                        method: 'POST',
                        path: '/test',
                        request: { data: { type: 'object', description: 'Data' } },
                        response: { success: { type: 'boolean' } }
                    }]
                };
                const gen = CustomRouteGenerator.fromConfig(config);
                const result = gen.generateTypeScript();

                expect(result.content).toContain('data:');
                expect(result.content).toContain('Record');
                expect(result.content).toContain('string');
                expect(result.content).toContain('unknown');
            });

            it('should generate enum fields as union types', () => {
                const result = generator.generateTypeScript();

                // switchDisplayMode has mode enum: ['circle', 'svg', 'symbol']
                expect(result.content).toContain('circle');
                expect(result.content).toContain('svg');
                expect(result.content).toContain('symbol');
                expect(result.content).toMatch(/mode:\s*/);
            });

            it('should generate optional fields with ?', () => {
                const result = generator.generateTypeScript();

                // createBed has optional bedName
                expect(result.content).toContain('bedName?');
            });
        });

        describe('Function signatures', () => {
            it('should generate createBed(params: CreateBedRequest): Promise<CreateBedResponse> for POST with request', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toMatch(/createBed\(params: CreateBedRequest\):\s*Promise<CreateBedResponse>/);
            });

            it('should generate deleteBed(id: string): Promise<DeleteBedResponse> for DELETE with path param', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toMatch(/deleteBed\(id: string\):\s*Promise<DeleteBedResponse>/);
            });

            it('should generate getDocInfo(): Promise<GetDocInfoResponse> for GET no params', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toMatch(/getDocInfo\(\):\s*Promise<GetDocInfoResponse>/);
            });

            it('should generate placeBatch(body: string): Promise<PlaceBatchResponse> for rawBody POST', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toMatch(/placeBatch\(body: string\):\s*Promise<PlaceBatchResponse>/);
            });

            it('should generate switchDisplayMode(uuid: string, params: SwitchDisplayModeRequest): Promise<SwitchDisplayModeResponse> for path param + request', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toMatch(/switchDisplayMode\(uuid: string, params: SwitchDisplayModeRequest\):\s*Promise<SwitchDisplayModeResponse>/);
            });
        });

        describe('Fetch helper', () => {
            it('should include fetchRoute helper function with timeout', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('fetchRoute');
                expect(result.content).toContain('timeout');
            });
        });

        describe('URL construction', () => {
            it('should use encodeURIComponent for path params in URLs', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('encodeURIComponent');
            });

            it('should use JSON.stringify(params) for request body', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('JSON.stringify(params)');
            });

            it('should pass body as-is for rawBody routes', () => {
                const result = generator.generateTypeScript();

                // For placeBatch (rawBody), the body should be passed directly, not JSON.stringify'd
                // The function signature takes body: string and it should be used directly as the fetch body
                expect(result.content).toMatch(/body:\s*body(?!\s*\))/);
            });
        });

        describe('Documentation', () => {
            it('should include JSDoc descriptions for interfaces', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('Creates a bed from the current selection');
            });

            it('should include JSDoc descriptions for functions', () => {
                const result = generator.generateTypeScript();

                expect(result.content).toContain('/**');
                expect(result.content).toContain('Returns document information');
            });
        });
    });

    // =========================================================================
    // Extends Feature Tests
    // =========================================================================

    describe('Extends feature', () => {
        let baseConfigPath: string;
        let extensionConfigPath: string;

        const baseConfig: RoutesConfig = {
            namespace: 'BaseHandlers',
            routes: [
                {
                    name: 'getStatus',
                    method: 'GET',
                    path: '/status',
                    response: {
                        ok: { type: 'boolean' },
                        version: { type: 'string' }
                    }
                },
                {
                    name: 'createItem',
                    method: 'POST',
                    path: '/items/create',
                    request: {
                        name: { type: 'string', description: 'Item name' }
                    },
                    response: {
                        success: { type: 'boolean' },
                        id: { type: 'string' }
                    }
                }
            ]
        };

        beforeEach(async () => {
            baseConfigPath = path.join(tempDir, 'base-routes.json');
            extensionConfigPath = path.join(tempDir, 'extension-routes.json');
            await fs.writeFile(baseConfigPath, JSON.stringify(baseConfig, null, 2));
        });

        it('should merge base and extension routes', async () => {
            const extensionConfig = {
                extends: './base-routes.json',
                routes: [
                    {
                        name: 'deleteItem',
                        method: 'DELETE',
                        path: '/items/{id}',
                        pathParams: {
                            id: { type: 'string', description: 'Item ID' }
                        },
                        response: {
                            success: { type: 'boolean' }
                        }
                    }
                ]
            };

            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));
            const extGenerator = new CustomRouteGenerator(extensionConfigPath);
            const result = extGenerator.generateTypeScript();

            // Should have base routes
            expect(result.content).toContain('GetStatusResponse');
            expect(result.content).toContain('CreateItemRequest');

            // Should have extension route
            expect(result.content).toContain('DeleteItemResponse');
        });

        it('should allow extension to override base routes by name', async () => {
            const extensionConfig = {
                extends: './base-routes.json',
                routes: [
                    {
                        name: 'createItem',  // Override base createItem
                        method: 'POST',
                        path: '/items/create',
                        request: {
                            name: { type: 'string', description: 'Item name' },
                            category: { type: 'string', description: 'Item category' }
                        },
                        response: {
                            success: { type: 'boolean' },
                            id: { type: 'string' }
                        }
                    }
                ]
            };

            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));
            const extGenerator = new CustomRouteGenerator(extensionConfigPath);
            const result = extGenerator.generateTypeScript();

            // Should have the extended createItem with category field
            expect(result.content).toContain('category: string');

            // Should still have getStatus from base
            expect(result.content).toContain('GetStatusResponse');

            // Should only have one CreateItemRequest (not duplicated)
            const matches = result.content.match(/interface CreateItemRequest/g) || [];
            expect(matches.length).toBe(1);
        });

        it('should use extension namespace if provided', async () => {
            const extensionConfig = {
                extends: './base-routes.json',
                namespace: 'ExtendedHandlers',
                routes: []
            };

            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));
            const extGenerator = new CustomRouteGenerator(extensionConfigPath);
            const result = extGenerator.generateCppRegistration();

            expect(result.content).toContain('ExtendedHandlers');
        });

        it('should fall back to base namespace if extension does not provide one', async () => {
            const extensionConfig = {
                extends: './base-routes.json',
                routes: []
            };

            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));
            const extGenerator = new CustomRouteGenerator(extensionConfigPath);
            const result = extGenerator.generateCppRegistration();

            expect(result.content).toContain('BaseHandlers');
        });

        it('should throw error for non-existent base config', async () => {
            const extensionConfig = {
                extends: './nonexistent.json',
                routes: []
            };

            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));

            expect(() => {
                new CustomRouteGenerator(extensionConfigPath);
            }).toThrow(/not found/i);
        });

        it('should support chained extends (A extends B extends C)', async () => {
            const middleConfigPath = path.join(tempDir, 'middle-routes.json');

            const middleConfig = {
                extends: './base-routes.json',
                routes: [
                    {
                        name: 'updateItem',
                        method: 'PUT',
                        path: '/items/{id}',
                        pathParams: {
                            id: { type: 'string', description: 'Item ID' }
                        },
                        request: {
                            name: { type: 'string', description: 'New name' }
                        },
                        response: {
                            success: { type: 'boolean' }
                        }
                    }
                ]
            };

            const extensionConfig = {
                extends: './middle-routes.json',
                routes: [
                    {
                        name: 'deleteItem',
                        method: 'DELETE',
                        path: '/items/{id}',
                        pathParams: {
                            id: { type: 'string', description: 'Item ID' }
                        },
                        response: {
                            success: { type: 'boolean' }
                        }
                    }
                ]
            };

            await fs.writeFile(middleConfigPath, JSON.stringify(middleConfig, null, 2));
            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));

            const extGenerator = new CustomRouteGenerator(extensionConfigPath);
            const result = extGenerator.generateTypeScript();

            // Should have routes from all levels
            expect(result.content).toContain('GetStatusResponse');     // from base
            expect(result.content).toContain('CreateItemRequest');     // from base
            expect(result.content).toContain('UpdateItemRequest');     // from middle
            expect(result.content).toContain('DeleteItemResponse');    // from extension
        });

        it('should generate C++ and TS with merged routes', async () => {
            const extensionConfig = {
                extends: './base-routes.json',
                routes: [
                    {
                        name: 'deleteItem',
                        method: 'DELETE',
                        path: '/items/{id}',
                        pathParams: {
                            id: { type: 'string', description: 'Item ID' }
                        },
                        response: {
                            success: { type: 'boolean' }
                        }
                    }
                ]
            };

            await fs.writeFile(extensionConfigPath, JSON.stringify(extensionConfig, null, 2));
            const extGenerator = new CustomRouteGenerator(extensionConfigPath);

            const cppReg = extGenerator.generateCppRegistration();
            const cppDecl = extGenerator.generateCppDeclarations();
            const ts = extGenerator.generateTypeScript();

            // C++ registration should have all routes
            expect(cppReg.content).toContain('HandleGetStatus');
            expect(cppReg.content).toContain('HandleCreateItem');
            expect(cppReg.content).toContain('HandleDeleteItem');

            // C++ declarations should have all handlers
            expect(cppDecl.content).toContain('HandleGetStatus');
            expect(cppDecl.content).toContain('HandleCreateItem');
            expect(cppDecl.content).toContain('HandleDeleteItem');

            // TypeScript should have all functions
            expect(ts.content).toContain('getStatus');
            expect(ts.content).toContain('createItem');
            expect(ts.content).toContain('deleteItem');
        });
    });

    // =========================================================================
    // generate() Method Tests
    // =========================================================================

    describe('generate() method', () => {
        it('should return all 3 files', () => {
            const files = generator.generate();

            expect(files).toHaveLength(3);
            expect(files.map(f => f.filename)).toContain('CustomRouteRegistration.cpp');
            expect(files.map(f => f.filename)).toContain('CustomRouteHandlers.h');
            expect(files.map(f => f.filename)).toContain('customRoutes.ts');
        });

        it('should return files with non-empty content', () => {
            const files = generator.generate();

            for (const file of files) {
                expect(file.content.length).toBeGreaterThan(0);
            }
        });
    });

    // =========================================================================
    // Cross-language Consistency Tests
    // =========================================================================

    describe('Cross-language consistency', () => {
        it('should generate matching route paths between C++ registration and TypeScript functions', () => {
            const cppResult = generator.generateCppRegistration();
            const tsResult = generator.generateTypeScript();

            // Both should reference the same route paths
            expect(cppResult.content).toContain('/beds/create');
            expect(tsResult.content).toContain('/beds/create');

            expect(cppResult.content).toContain('/doc/info');
            expect(tsResult.content).toContain('/doc/info');

            expect(cppResult.content).toContain('/plant/place-batch');
            expect(tsResult.content).toContain('/plant/place-batch');
        });

        it('should generate matching handler names between C++ registration and declarations', () => {
            const cppReg = generator.generateCppRegistration();
            const cppDecl = generator.generateCppDeclarations();

            // All handler names should appear in both files
            const handlerNames = [
                'HandleCreateBed',
                'HandleDeleteBed',
                'HandleGetDocInfo',
                'HandlePlaceBatch',
                'HandleSwitchDisplayMode'
            ];

            for (const name of handlerNames) {
                expect(cppReg.content).toContain(name);
                expect(cppDecl.content).toContain(name);
            }
        });

        it('should generate matching field names between C++ JSDoc and TypeScript interfaces', () => {
            const cppDecl = generator.generateCppDeclarations();
            const tsResult = generator.generateTypeScript();

            // Response field names should match
            expect(cppDecl.content).toContain('success');
            expect(tsResult.content).toContain('success');

            expect(cppDecl.content).toContain('bedId');
            expect(tsResult.content).toContain('bedId');

            expect(cppDecl.content).toContain('area_sqm');
            expect(tsResult.content).toContain('area_sqm');

            // Request field names should match
            expect(cppDecl.content).toContain('bedName');
            expect(tsResult.content).toContain('bedName');
        });
    });

    // =========================================================================
    // Edge Cases
    // =========================================================================

    describe('Edge cases', () => {
        it('should handle routes with multiple path params', async () => {
            const config: RoutesConfig = {
                routes: [{
                    name: 'getPlantInBed',
                    method: 'GET',
                    path: '/beds/{bedId}/plants/{plantId}',
                    pathParams: {
                        bedId: { type: 'string', description: 'Bed ID' },
                        plantId: { type: 'string', description: 'Plant ID' }
                    },
                    response: {
                        name: { type: 'string' }
                    }
                }]
            };
            const gen = CustomRouteGenerator.fromConfig(config);

            const cppReg = gen.generateCppRegistration();
            const cppDecl = gen.generateCppDeclarations();
            const ts = gen.generateTypeScript();

            // C++ should handle multiple params
            expect(cppReg.content).toContain('bedId');
            expect(cppReg.content).toContain('plantId');

            // Declarations should have both params
            expect(cppDecl.content).toContain('bedId');
            expect(cppDecl.content).toContain('plantId');

            // TypeScript function should accept both params
            expect(ts.content).toMatch(/getPlantInBed\(bedId: string, plantId: string\)/);
        });

        it('should handle route with no response defined', async () => {
            const config: RoutesConfig = {
                routes: [{
                    name: 'fireAndForget',
                    method: 'POST',
                    path: '/fire',
                    request: {
                        action: { type: 'string' }
                    }
                    // No response
                }]
            };
            const gen = CustomRouteGenerator.fromConfig(config);

            const ts = gen.generateTypeScript();
            const cppReg = gen.generateCppRegistration();

            // Should still generate valid output
            expect(cppReg.content).toContain('HandleFireAndForget');
            expect(ts.content).toContain('fireAndForget');

            // Should not generate a response interface
            expect(ts.content).not.toContain('FireAndForgetResponse');
        });

        it('should handle route with no description', async () => {
            const config: RoutesConfig = {
                routes: [{
                    name: 'noDesc',
                    method: 'GET',
                    path: '/no-desc',
                    response: { ok: { type: 'boolean' } }
                    // No description
                }]
            };
            const gen = CustomRouteGenerator.fromConfig(config);

            const cppReg = gen.generateCppRegistration();
            const cppDecl = gen.generateCppDeclarations();
            const ts = gen.generateTypeScript();

            // Should generate valid output without crashing
            expect(cppReg.content).toContain('HandleNoDesc');
            expect(cppDecl.content).toContain('HandleNoDesc');
            expect(ts.content).toContain('noDesc');
        });

        it('should handle single route config', async () => {
            const config: RoutesConfig = {
                routes: [{
                    name: 'onlyRoute',
                    method: 'GET',
                    path: '/only',
                    response: { value: { type: 'number' } }
                }]
            };
            const gen = CustomRouteGenerator.fromConfig(config);
            const files = gen.generate();

            expect(files).toHaveLength(3);
            expect(files[0].content).toContain('HandleOnlyRoute');
        });

        it('should handle many routes (10+)', async () => {
            const config: RoutesConfig = {
                routes: Array.from({ length: 12 }, (_, i) => ({
                    name: `route${i}`,
                    method: 'GET' as const,
                    path: `/route/${i}`,
                    response: { index: { type: 'number' } }
                }))
            };
            const gen = CustomRouteGenerator.fromConfig(config);

            const cppReg = gen.generateCppRegistration();
            const cppDecl = gen.generateCppDeclarations();
            const ts = gen.generateTypeScript();

            // All 12 routes should be present
            for (let i = 0; i < 12; i++) {
                expect(cppReg.content).toContain(`HandleRoute${i}`);
                expect(cppDecl.content).toContain(`HandleRoute${i}`);
                expect(ts.content).toContain(`route${i}`);
            }
        });
    });

    // =========================================================================
    // Output Quality Tests
    // =========================================================================

    describe('Output quality', () => {
        it('should generate valid C++ syntax (balanced braces)', () => {
            const regResult = generator.generateCppRegistration();

            const openBraces = (regResult.content.match(/{/g) || []).length;
            const closeBraces = (regResult.content.match(/}/g) || []).length;

            expect(openBraces).toBe(closeBraces);
        });

        it('should generate valid TypeScript syntax (balanced braces)', () => {
            const result = generator.generateTypeScript();

            const openBraces = (result.content.match(/{/g) || []).length;
            const closeBraces = (result.content.match(/}/g) || []).length;

            expect(openBraces).toBe(closeBraces);
        });

        it('should generate valid C++ declarations syntax (balanced braces)', () => {
            const result = generator.generateCppDeclarations();

            const openBraces = (result.content.match(/{/g) || []).length;
            const closeBraces = (result.content.match(/}/g) || []).length;

            expect(openBraces).toBe(closeBraces);
        });
    });
});
