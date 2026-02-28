#!/usr/bin/env node
/**
 * Generates endpointMap.ts from routes.json.
 * This provides a Record<string, { method, path }> that HttpApiAdapter uses
 * instead of a hand-written ENDPOINT_MAP.
 *
 * Supports:
 * - Canonical endpoint names from routes.json
 * - Aliases: additional names that map to the same endpoint
 * - RAW_BODY_FUNCTIONS: functions that send raw string body (from rawBody flag)
 *
 * Usage: node codegen/generate-endpoint-map.cjs [path/to/routes.json]
 */
const path = require('path');
const fs = require('fs');

// Config path: CLI arg or default to routes.json in this directory
const configPath = process.argv[2]
    ? path.resolve(process.argv[2])
    : path.resolve(__dirname, 'routes.json');

const outDir = path.resolve(__dirname, 'output');
const outFile = path.join(outDir, 'endpointMap.ts');

const config = JSON.parse(fs.readFileSync(configPath, 'utf-8'));

if (!fs.existsSync(outDir)) {
  fs.mkdirSync(outDir, { recursive: true });
}

// Collect alias entries (emitted AFTER canonical entries so they override on collision)
const aliasEntries = [];
// Collect rawBody function names (canonical + aliases)
const rawBodyNames = [];

const lines = [
  '/**',
  ' * Generated endpoint map from routes.json',
  ' * Auto-generated — DO NOT EDIT',
  ' *',
  ' * Usage: node codegen/generate-endpoint-map.cjs',
  ' */',
  '',
  'export interface EndpointDef {',
  "  method: 'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH'",
  '  path: string',
  '}',
  '',
  'export const GENERATED_ENDPOINTS: Record<string, EndpointDef> = {',
  '  // --- Canonical endpoints ---',
];

let aliasCount = 0;

for (const route of config.routes) {
  const name = route.name;
  const method = route.method;
  const routePath = route.path;
  lines.push(`  /** ${method} ${routePath} - ${route.description || ''} */`);
  lines.push(`  ${name}: { method: '${method}', path: '${routePath}' },`);

  // Track rawBody for canonical name
  if (route.rawBody) {
    rawBodyNames.push(name);
  }

  // Collect aliases for this route
  if (route.aliases && Array.isArray(route.aliases)) {
    for (const alias of route.aliases) {
      aliasEntries.push({ alias, method, routePath, canonicalName: name });
      aliasCount++;

      // Aliases inherit rawBody from their canonical route
      if (route.rawBody) {
        rawBodyNames.push(alias);
      }
    }
  }
}

// Emit alias entries after canonical entries (overrides on collision)
if (aliasEntries.length > 0) {
  lines.push('');
  lines.push('  // --- Aliases (backward-compatible names) ---');
  for (const { alias, method, routePath, canonicalName } of aliasEntries) {
    lines.push(`  /** Alias for ${canonicalName} → ${method} ${routePath} */`);
    lines.push(`  ${alias}: { method: '${method}', path: '${routePath}' },`);
  }
}

lines.push('}');
lines.push('');

// Generate RAW_BODY_FUNCTIONS set
lines.push('/**');
lines.push(' * Functions that expect raw string body (no JSON wrapping).');
lines.push(' * Generated from routes with "rawBody": true in routes.json.');
lines.push(' */');
lines.push('export const RAW_BODY_FUNCTIONS = new Set([');
for (const name of rawBodyNames) {
  lines.push(`  '${name}',`);
}
lines.push('])');
lines.push('');

fs.writeFileSync(outFile, lines.join('\n'), 'utf-8');
const totalEndpoints = config.routes.length + aliasCount;
console.log(`Wrote endpointMap.ts (${config.routes.length} canonical + ${aliasCount} aliases = ${totalEndpoints} endpoints, ${rawBodyNames.length} rawBody)`);
