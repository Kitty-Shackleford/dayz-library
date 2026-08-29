# DayZ Server Automation Kit v1

This folder is a self-contained template for a DayZ server owner's own GitHub repository. Scheduled workflows call Nitrado directly; the DayZ Dashboard and Discord bot are optional consumers, not runtime dependencies.

## Install

1. Create a private repository from this template directory (copy all files, including `.github`).
2. Replace `SET_NITRADO_SERVER_ID` in `dayz-integration.json` with the Nitrado service ID.
3. In Repository settings → Secrets and variables → Actions:
   - Secret `NITRADO_API_TOKEN`: the server owner's Nitrado API token.
   - Variable `NITRADO_SERVER_ID`: the exact Nitrado service ID.
   - Variable `DAYZ_BACKUP_PATHS_JSON`: required only when manually running Backup; use a JSON array such as `["/noftp/dayzxb/mpmissions/dayzOffline.chernarusplus"]`.
   - Variable `DAYZ_RETENTION_DAYS`: optional status-history retention, default 31.
   - Variable `DAYZ_BACKUP_MAX_BYTES`: optional backup size ceiling, default 524288000.
   - Under **Actions → General → Workflow permissions**, allow read and write so Monitor and Validator can commit their small generated outputs.
4. Run **DayZ Server Monitor** manually once and verify `dayz/data/current/server-status.json` appears.
5. Put XML/JSON files to validate under `dayz/config/`.
6. Optionally enable GitHub Pages with **GitHub Actions** as the source, then run **DayZ GitHub Pages**.
7. In the DayZ Dashboard, connect GitHub and link this repository to the same server. The dashboard verifies that the manifest's `server_id` matches the selected server.

## Workflows and permissions

| Workflow | Purpose | Permission |
|---|---|---|
| `dayz-monitor.yml` | Six-hour status snapshots and bounded history | `contents: write` |
| `dayz-validate.yml` | XML/JSON validation reports | `contents: write` |
| `dayz-backup.yml` | Manual, expiring repository artifact | `contents: read` |
| `dayz-pages.yml` | Static reports | `contents: read`, `pages: write`, `id-token: write` |

No workflow requests `write-all`. Backups are artifacts rather than commits to avoid repository growth and accidental publication.

## Security and privacy

- Never put Nitrado, FTP, GitHub, or dashboard credentials in repository files, manifests, logs, or Pages data.
- Keep the repository private unless all committed status/history/configuration is safe to publish. Treat backup artifacts from a public repository as exposed to that repository's readers; use a private repository for backups.
- GitHub Pages on a public repository is public. This kit publishes only generated status and validation summaries, but server owners must review their data.
- Raw logs, player identifiers, IP addresses, and admin activity are not collected by v1.
- Limit token access in Nitrado and rotate a token if a workflow log or fork could have exposed it.

## Retention and recovery

Status history is retained by date for `DAYZ_RETENTION_DAYS`. Backup artifacts expire after 14 days. If GitHub is unavailable, Nitrado and the DayZ server continue normally; the last Pages site remains available when GitHub serves it. If the dashboard is unavailable, these workflows continue. Re-run any workflow with `workflow_dispatch` after recovery.

## Versioning

The copied kit is version `1.0.0`. Keep production repositories on a reviewed release. Patch/minor updates preserve schema v1. Breaking manifest or output changes require a new schema version and migration notes. GitHub's official actions are pinned to reviewed immutable commit SHAs with release comments; review Dependabot updates before changing them.

## Standalone operation

Nothing calls the DayZ Dashboard API. Removing a dashboard repository link does not disable workflows, delete data, or affect Pages. The repository remains the server owner's archive.

See `docs/GITHUB_ACTIONS_AUTOMATION_KIT.md` in the DayZ Dashboard project for architecture, schemas, integration behavior, roadmap, and troubleshooting.
