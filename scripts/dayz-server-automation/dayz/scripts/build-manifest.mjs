import fs from 'node:fs/promises';

const file = 'dayz-integration.json';
const manifest = JSON.parse(await fs.readFile(file, 'utf8'));
manifest.schema_version = 1;
manifest.platform = 'dayz';
manifest.server_id = process.env.NITRADO_SERVER_ID || manifest.server_id;
if (!manifest.server_id || manifest.server_id === 'SET_NITRADO_SERVER_ID') throw new Error('Set NITRADO_SERVER_ID');
manifest.generated_at = new Date().toISOString();
const actions = Array.isArray(manifest.actions) ? manifest.actions : [];
manifest.capabilities = [...new Set(actions.flatMap(action => action.capabilities || []))].sort();
await fs.writeFile(file, `${JSON.stringify(manifest, null, 2)}\n`);
console.log(`Updated integration manifest for ${manifest.server_id}`);
