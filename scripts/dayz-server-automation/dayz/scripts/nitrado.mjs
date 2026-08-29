const API_BASE = 'https://api.nitrado.net';

export function requiredEnv(name) {
  const value = process.env[name]?.trim();
  if (!value) throw new Error(`${name} is required`);
  return value;
}

export async function nitradoJson(path, options = {}) {
  const token = requiredEnv('NITRADO_API_TOKEN');
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), 30000);
  try {
    const response = await fetch(`${API_BASE}${path}`, {
      ...options,
      headers: { Accept: 'application/json', Authorization: `Bearer ${token}`, ...(options.headers || {}) },
      signal: controller.signal,
    });
    if (!response.ok) throw new Error(`Nitrado request failed (${response.status})`);
    const body = await response.json();
    if (!body || typeof body !== 'object') throw new Error('Nitrado returned an invalid response');
    return body;
  } finally {
    clearTimeout(timeout);
  }
}

export function servicePath(suffix = '') {
  const serverId = encodeURIComponent(requiredEnv('NITRADO_SERVER_ID'));
  return `/services/${serverId}/gameservers${suffix}`;
}
