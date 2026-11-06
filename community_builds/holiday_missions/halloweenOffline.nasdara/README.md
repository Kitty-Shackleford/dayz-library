# halloweenOffline.nasdara — DayZ CE Halloween mission pack (nasdara)

Self-contained nasdara mission folder: base mission event files + Halloween
delta. Install as its own mission folder — no edits to the base mission.

Base: `dayzOffline.nasdara` live mission, 2026-10-23 (38 spawn events, 26 event
groups). Rebuild against a newer base by re-running the generator if the base
mission's `cfgeventspawns.xml` / `cfgeventgroups.xml` / `db/events.xml` change.

## What it changes (vs base)

| File | Delta |
|---|---|
| `db/events.xml` | `InfectedSpooky` enabled (spooky-variant infected roam: old milkmaid + handyman); `InfectedReligious` children swapped to `Zmbm_Mummy`; new `StaticSpookyCamp` event (fixed-position, group-based, active) |
| `cfgeventspawns.xml` | + `StaticSpookyCamp` — 6 positions at Pesh Kuh, Bazar-e Kohna, Karizak, Gard Abad, Kotal-e Khushk, Marz-e Lut (each spawns the `SpookyCamp` group); + `StaticBonfire` — 4 positions (Ak Hisar, Zulfiqar missile base, Qalat-e Bala, Dar-e Ardis); + bare `InfectedSpooky` line (player-relative roaming, no positions) |
| `cfgeventgroups.xml` | + `SpookyCamp` group: `Bonfire`, 2× `Land_Misc_WoodenCrate_Spawn_DE`, 2× `Haybale`, `Land_Wreck_Pickup01_Aban_Blue`, `Land_Wreck_Ikarus_DE` — every classname verified present in nasdara `mapgroupproto.xml` |

No `cfgrandompresets.xml` / economy files touched. No existing base event
modified except the `InfectedReligious` child swap (imams → mummy).

## Install

Copy the whole `halloweenOffline.nasdara/` folder into the server mission
directory (next to `dayzOffline.nasdara`, e.g. `steamapps/common/DayZServer/mpmissions/`)
or into the CE custom-folder layout you already use for holiday packs. Start the
server with mission `halloweenOffline.nasdara`.

## Remove

Delete the folder and restart the server. Nothing else to roll back.

## Dashboard / holiday windows

Folder name matches the `missions: ["halloweenOffline"]` prefix in
`dayz-dashboard/public/data/holiday-windows.json` (case-insensitive), so the
dashboard's `getEventSpawnLocations` picks this pack's markers up automatically
when it's in the server download dir — no config change needed.

## Notes

- Positions come from `dayz-library/community_builds/map-data/nasdara-locations.json`
  (named settlements, all within the 0–16384 map).
- `StaticSpookyCamp` positions carry `group="SpookyCamp"` (house pattern:
  `StaticMilitaryConvoy`); no `y` attribute — server snaps to terrain, same as
  every other non-convoy event position.
