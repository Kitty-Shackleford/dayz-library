/* global document */

const byId = id => document.getElementById(id);

async function optionalJson(url) {
  const response = await fetch(url, { cache: 'no-store' });
  if (!response.ok) return null;
  return response.json();
}

function relativeTime(value) {
  if (!value) return '—';
  const seconds = Math.max(0, Math.floor((Date.now() - new Date(value).getTime()) / 1000));
  if (seconds < 60) return `${seconds}s ago`;
  if (seconds < 3600) return `${Math.floor(seconds / 60)}m ago`;
  if (seconds < 86400) return `${Math.floor(seconds / 3600)}h ago`;
  return `${Math.floor(seconds / 86400)}d ago`;
}

function setServerStatus(status) {
  const normalized = String(status || 'unknown').toLowerCase();
  const online = ['started', 'online'].includes(normalized);
  byId('statusBadge').textContent = online ? 'ONLINE' : normalized.toUpperCase();
  byId('statusBadge').className = `badge ${online ? 'online' : 'offline'}`;
}

async function load() {
  const [manifest, status, validation] = await Promise.all([
    optionalJson('dayz-integration.json'),
    optionalJson('data/server-status.json'),
    optionalJson('reports/validation.json'),
  ]);
  const monitorInterval = Number((manifest?.actions || []).find(action =>
    (action.capabilities || []).includes('server_status'))?.expected_interval_minutes) || 360;
  const staleAfterMs = monitorInterval * 3 * 60000;
  if (manifest) {
    byId('serverId').textContent = `Server ${manifest.server_id}`;
    byId('kitVersion').textContent = `v${manifest.integration_version}`;
    byId('actions').replaceChildren(...(manifest.actions || []).map(action => {
      const row = document.createElement('div');
      row.className = 'health-row';
      const name = document.createElement('span');
      name.textContent = action.name;
      const state = document.createElement('b');
      state.textContent = `v${action.version}`;
      row.append(name, state);
      return row;
    }));
  }
  if (status) {
    setServerStatus(status.status);
    byId('players').textContent = `${status.players?.current ?? 0} / ${status.players?.maximum ?? 0}`;
    byId('lastUpdated').textContent = relativeTime(status.generated_at);
    byId('dataHealth').textContent = Date.now() - new Date(status.generated_at).getTime() < staleAfterMs ? 'Fresh' : 'Stale';
  } else {
    setServerStatus('no data');
    byId('dataHealth').textContent = 'Unavailable';
  }
  if (validation) {
    const configValid = validation.status === 'valid';
    byId('configHealth').textContent = validation.status === 'no_files' ? 'No files' : configValid ? 'Valid' : 'Needs attention';
    byId('configHealth').className = configValid ? 'good' : validation.status === 'no_files' ? '' : 'bad';
    byId('validationTime').textContent = relativeTime(validation.generated_at);
    byId('validationSummary').textContent = `${validation.files_checked} file(s) checked.`;
    byId('validationFiles').replaceChildren(...(validation.results || []).map(item => {
      const row = document.createElement('div');
      row.className = 'health-row';
      const file = document.createElement('span');
      file.textContent = item.path;
      const result = document.createElement('b');
      result.textContent = item.valid ? 'Valid' : 'Invalid';
      result.className = item.valid ? 'good' : 'bad';
      row.append(file, result);
      return row;
    }));
  }
}

load().catch(() => {
  byId('dataHealth').textContent = 'Unable to load published data';
});
