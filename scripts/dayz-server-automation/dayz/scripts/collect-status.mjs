import fs from 'node:fs/promises';
import path from 'node:path';
import { nitradoJson, requiredEnv, servicePath } from './nitrado.mjs';

const root = process.cwd();
const now = new Date();
const body = await nitradoJson(servicePath());
const game = body.data?.gameserver;
if (!game || typeof game !== 'object') throw new Error('Nitrado response did not include a gameserver');
const query = game.query || {};
const current = Number(query.player_current ?? query.players ?? 0);
const maximum = Number(query.player_max ?? query.maxplayers ?? game.slots ?? 0);
const output = {
  schema_version: 1,
  server_id: requiredEnv('NITRADO_SERVER_ID'),
  generated_at: now.toISOString(),
  status: game.status || 'unknown',
  players: {
    current: Number.isFinite(current) ? current : 0,
    maximum: Number.isFinite(maximum) ? maximum : 0,
  },
  uptime: { seconds: null },
  last_status_change: game.last_status_change || null,
};
const currentDir = path.join(root, 'dayz/data/current');
const historyDir = path.join(root, 'dayz/data/history', now.toISOString().slice(0, 10));
await fs.mkdir(currentDir, { recursive: true });
await fs.mkdir(historyDir, { recursive: true });
await fs.writeFile(path.join(currentDir, 'server-status.json'), `${JSON.stringify(output, null, 2)}\n`);
await fs.appendFile(path.join(historyDir, 'server-status.jsonl'), `${JSON.stringify(output)}\n`);

const configuredRetentionDays = Number(process.env.DAYZ_RETENTION_DAYS || 31);
if (!Number.isInteger(configuredRetentionDays) || configuredRetentionDays < 1 || configuredRetentionDays > 365) {
  throw new Error('DAYZ_RETENTION_DAYS must be an integer from 1 to 365');
}
const retentionDays = configuredRetentionDays;
const historyRoot = path.join(root, 'dayz/data/history');
for (const entry of await fs.readdir(historyRoot, { withFileTypes: true })) {
  if (!entry.isDirectory() || !/^\d{4}-\d{2}-\d{2}$/.test(entry.name)) continue;
  const age = now.getTime() - new Date(`${entry.name}T00:00:00.000Z`).getTime();
  if (age > retentionDays * 86400000) await fs.rm(path.join(historyRoot, entry.name), { recursive: true, force: true });
}
console.log(`Wrote status for server ${output.server_id}`);
