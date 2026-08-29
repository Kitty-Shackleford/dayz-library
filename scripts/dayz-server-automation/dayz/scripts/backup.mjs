import fs from 'node:fs/promises';
import path from 'node:path';
import { nitradoJson, servicePath } from './nitrado.mjs';

function normalizeRemotePath(value) {
  if (typeof value !== 'string') return value;
  return value.replace(
    /\/ftproot\/(dayzxb|dayzps|dayzswitch)(?=\/|$)/g,
    match => match.replace('/ftproot/', '/noftp/')
  );
}
function isSafeRemotePath(value) {
  if (typeof value !== 'string' || value.length > 1024 || !value.startsWith('/')
      || value.includes('//') || value.includes('\\') || /[\u0000-\u001f\u007f]/.test(value)
      || value.split('/').some(part => part === '.' || part === '..')) return false;
  return /^\/noftp\/[^/]+(?:\/.*)?$/.test(value)
    || /^\/ftproot\/dayzstandalone(?:\/.*)?$/.test(value)
    || /^\/games\/[A-Za-z0-9._-]+\/(?:noftp\/[^/]+|ftproot\/(?:dayzstandalone|dayz(?:xb|ps|switch)_missions))(?:\/.*)?$/.test(value);
}
function canonicalRemotePath(value) {
  const normalized = normalizeRemotePath(value);
  if (!isSafeRemotePath(normalized)) throw new Error('Nitrado returned an unsafe backup path');
  return normalized.length > 1 ? normalized.replace(/\/$/, '') : normalized;
}
const configuredInput = JSON.parse(process.env.DAYZ_BACKUP_PATHS_JSON || '[]');
if (!Array.isArray(configuredInput) || !configuredInput.length || configuredInput.length > 100) {
  throw new Error('DAYZ_BACKUP_PATHS_JSON must contain 1-100 safe Nitrado file-server paths');
}
const configuredAliases = configuredInput.map(canonicalRemotePath);
const missionAliases = configuredAliases.filter(value => /^\/noftp\/dayz(?:xb|ps|switch)_missions(?:\/|$)/.test(value));
let configured = configuredAliases;
if (missionAliases.length) {
  const body = await nitradoJson(servicePath());
  const gameserver = body.data?.gameserver;
  const definitions = {
    dayzxb: { data: 'dayzxb', missions: 'dayzxb_missions' },
    dayzps: { data: 'dayzps', missions: 'dayzps_missions' },
    dayzswitch: { data: 'dayzswitch', missions: 'dayzswitch_missions' },
  };
  const definition = definitions[String(gameserver?.game || '').toLowerCase()];
  const gamePath = typeof gameserver?.game_specific?.path === 'string'
    ? gameserver.game_specific.path.replace(/\/$/, '')
    : '';
  const namespaceMatch = definition && gamePath.match(
    new RegExp(`^(/games/[A-Za-z0-9._-]+)/noftp/${definition.data}$`)
  );
  if (!namespaceMatch) throw new Error('Nitrado returned invalid game path metadata');
  const activeMission = gameserver?.settings?.config?.mission
    || gameserver?.game_specific?.mission
    || gameserver?.query?.map;
  if (typeof activeMission !== 'string' || !/^[A-Za-z0-9._-]{1,128}$/.test(activeMission)
      || activeMission === '.' || activeMission === '..') {
    throw new Error('Nitrado returned invalid active mission metadata');
  }
  configured = configuredAliases.map(value => {
    const aliasMatch = value.match(/^\/noftp\/(dayz(?:xb|ps|switch)_missions)(\/.*)?$/);
    if (!aliasMatch) return value;
    if (aliasMatch[1] !== definition.missions) {
      throw new Error('Configured backup mission path does not match the Nitrado game platform');
    }
    const missionSuffix = aliasMatch[2] || `/${activeMission}`;
    return canonicalRemotePath(`${namespaceMatch[1]}/ftproot/${definition.missions}${missionSuffix}`);
  });
}
const configuredRoots = [...new Set(configured)];
const configuredMaxBytes = Number(process.env.DAYZ_BACKUP_MAX_BYTES || 524288000);
if (!Number.isFinite(configuredMaxBytes) || configuredMaxBytes < 1) {
  throw new Error('DAYZ_BACKUP_MAX_BYTES must be a positive number');
}
const maxBytes = Math.min(2 * 1024 * 1024 * 1024, Math.floor(configuredMaxBytes));
let total = 0;
const outputRoot = path.resolve('dayz-backup');
const visited = new Set();
let entryCount = 0;
const MAX_ENTRIES = 10000;
const MAX_DEPTH = 32;

function localPath(remotePath) {
  const parts = remotePath.split('/').filter(Boolean);
  if (!parts.length || parts.some(part => part === '.' || part === '..')) throw new Error('Unsafe backup path');
  return path.join(outputRoot, ...parts);
}

function allowedRemotePath(value) {
  const remotePath = canonicalRemotePath(value);
  const allowed = configuredRoots.some(root => remotePath === root || remotePath.startsWith(`${root}/`));
  if (!allowed) throw new Error('Nitrado returned a backup path outside the configured roots');
  return remotePath;
}

async function downloadFile(remotePath) {
  remotePath = allowedRemotePath(remotePath);
  const token = await nitradoJson(`${servicePath('/file_server/download')}?file=${encodeURIComponent(remotePath)}`);
  const url = new URL(token.data?.token?.url);
  if (url.protocol !== 'https:') throw new Error('Nitrado returned an unsafe download URL');
  const response = await fetch(url, { redirect: 'error', signal: AbortSignal.timeout(300000) });
  if (!response.ok) throw new Error(`Backup download failed (${response.status})`);
  const remaining = maxBytes - total;
  const declaredLength = Number(response.headers.get('content-length'));
  if (Number.isFinite(declaredLength) && declaredLength > remaining) {
    throw new Error('Backup exceeded DAYZ_BACKUP_MAX_BYTES');
  }
  if (!response.body?.getReader) throw new Error('Backup download did not return a readable body');
  const reader = response.body.getReader();
  const chunks = [];
  let fileBytes = 0;
  while (true) {
    const { done, value } = await reader.read();
    if (done) break;
    fileBytes += value.byteLength;
    if (fileBytes > remaining) {
      await reader.cancel();
      throw new Error('Backup exceeded DAYZ_BACKUP_MAX_BYTES');
    }
    chunks.push(Buffer.from(value));
  }
  const bytes = Buffer.concat(chunks, fileBytes);
  total += bytes.length;
  const destination = localPath(remotePath);
  await fs.mkdir(path.dirname(destination), { recursive: true });
  await fs.writeFile(destination, bytes);
}

async function collect(remotePath, depth = 0) {
  remotePath = allowedRemotePath(remotePath);
  if (depth > MAX_DEPTH) throw new Error('Backup directory nesting exceeded the safety limit');
  if (visited.has(remotePath)) return;
  visited.add(remotePath);
  entryCount += 1;
  if (entryCount > MAX_ENTRIES) throw new Error('Backup entry count exceeded the safety limit');
  const listing = await nitradoJson(`${servicePath('/file_server/list')}?dir=${encodeURIComponent(remotePath)}`);
  const entries = listing.data?.entries;
  if (!Array.isArray(entries)) return downloadFile(remotePath);
  for (const entry of entries) {
    if (!entry || typeof entry !== 'object' || (typeof entry.path !== 'string' && typeof entry.name !== 'string')) {
      throw new Error('Nitrado returned an invalid backup entry');
    }
    const child = allowedRemotePath(entry.path || `${remotePath}/${entry.name}`);
    if (entry.type === 'dir' || entry.type === 'directory') await collect(child, depth + 1);
    else await downloadFile(child);
  }
}

await fs.mkdir(outputRoot, { recursive: true });
for (const remotePath of configured) await collect(remotePath);
await fs.writeFile(path.join(outputRoot, 'backup-metadata.json'), `${JSON.stringify({
  schema_version: 1,
  generated_at: new Date().toISOString(),
  source_paths: configured,
  bytes: total,
}, null, 2)}\n`);
console.log(`Backed up ${total} bytes`);
