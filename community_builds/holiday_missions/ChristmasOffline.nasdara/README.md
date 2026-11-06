# ChristmasOffline.nasdara — DayZ CE Christmas mission pack (nasdara)

Self-contained nasdara mission folder: base mission event files + Christmas
delta. Install as its own mission folder — no edits to the base mission.

Base: `dayzOffline.nasdara` live mission, 2026-10-23 (38 spawn events, 26 event
groups). Rebuild against a newer base by re-running the generator if the base
mission's `cfgeventspawns.xml` / `cfgeventgroups.xml` / `db/events.xml` change.

## What it changes (vs base)

| File | Delta |
|---|---|
| `db/events.xml` | `StaticSantaCrash`, `StaticBonfire`, `InfectedSanta`, `AnimalDeer`, `AnimalRoeDeer` enabled; deer children swapped to reindeer (`Animal_RangiferTarandus` / `_F` / `_Xmas` — class names reused from the CE Christmas pack); new `StaticChristmasMarket` event (fixed-position, group-based, active) |
| `cfgeventspawns.xml` | + `StaticSantaCrash` — 6 positions at Dashtabad, Khorramabad, Bustanshahr, Sabzabad, Shah Yailaq, Kamar Zir; + `StaticBonfire` — 4 positions (No Abad, Gandomabad, Bagh-e Mir, Chashme Sabz); + `AnimalDeer` — 6 positions at hills (Se Khah, Zanu Ghar, Kuh-e Shamal, Spinkay Tapa, Tak Tapa, Gharibabad); + `AnimalRoeDeer` — 4 positions (Shana Kuh, Darvazeh-e Gharb, Sabz Tapa, Loy Tapa); + `StaticChristmasMarket` — 3 positions (Nasdara capital, Kucha Dar, Bam-e Sabz, each spawning the `ChristmasMarket` group); + bare `InfectedSanta` line (player-relative roaming, no positions) |
| `cfgeventgroups.xml` | + `ChristmasMarket` group: 2× `ChristmasTree`, 2× `Land_Bazaar_Flat`, `Bonfire`, 2× `Haybale`, 2× `Land_Misc_WoodenCrate_Spawn_DE` — every classname verified present in nasdara `mapgroupproto.xml` |

No `cfgrandompresets.xml` / economy files touched. No existing base event
modified except the deer → reindeer child swap.

## Install

Copy the whole `ChristmasOffline.nasdara/` folder into the server mission
directory (next to `dayzOffline.nasdara`, e.g. `steamapps/common/DayZServer/mpmissions/`)
or into the CE custom-folder layout you already use for holiday packs. Start the
server with mission `ChristmasOffline.nasdara`.

## Remove

Delete the folder and restart the server. Nothing else to roll back.

## Dashboard / holiday windows

Folder name matches the `missions: ["ChristmasOffline"]` prefix in
`dayz-dashboard/public/data/holiday-windows.json` (case-insensitive), so the
dashboard's `getEventSpawnLocations` picks this pack's markers up automatically
when it's in the server download dir — no config change needed.

## Notes

- Positions come from `dayz-library/community_builds/map-data/nasdara-locations.json`
  (named settlements, all within the 0–16384 map).
- `StaticChristmasMarket` positions carry `group="ChristmasMarket"` (house
  pattern: `StaticMilitaryConvoy`); no `y` attribute — server snaps to terrain,
  same as every other non-convoy event position.
- Reindeer class names are the CE pack's own; if a game update renames
  `Animal_RangiferTarandus_Xmas`, delete that one child line (min=0, optional).
