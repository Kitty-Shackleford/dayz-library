#!/usr/bin/env python3
import json
import pathlib
import xml.etree.ElementTree as ET
from datetime import datetime, timezone

ROOT = pathlib.Path('dayz/config')
REPORT_DIR = pathlib.Path('dayz/reports/current')
results = []
if ROOT.exists():
    for file in sorted(ROOT.rglob('*')):
        if not file.is_file() or file.suffix.lower() not in {'.json', '.xml'}:
            continue
        try:
            if file.suffix.lower() == '.json':
                with file.open(encoding='utf-8') as handle:
                    json.load(handle)
            else:
                ET.parse(file)
            results.append({'path': file.as_posix(), 'valid': True, 'error': None})
        except (ValueError, OSError, ET.ParseError) as error:
            results.append({'path': file.as_posix(), 'valid': False, 'error': str(error)[:500]})

validation_status = 'no_files' if not results else ('valid' if all(item['valid'] for item in results) else 'invalid')
output = {
    'schema_version': 1,
    'generated_at': datetime.now(timezone.utc).isoformat().replace('+00:00', 'Z'),
    'status': validation_status,
    'valid': None if not results else validation_status == 'valid',
    'files_checked': len(results),
    'results': results,
}
REPORT_DIR.mkdir(parents=True, exist_ok=True)
(REPORT_DIR / 'validation.json').write_text(json.dumps(output, indent=2) + '\n', encoding='utf-8')
overall = 'NO FILES' if validation_status == 'no_files' else ('PASS' if output['valid'] else 'FAIL')
lines = ['# DayZ Configuration Validation', '', f"Overall: {overall}", '', '| File | Result |', '|---|---|']
for item in results:
    result_text = 'Valid' if item['valid'] else 'Invalid: ' + item['error'].replace('|', '\\|')
    lines.append(f"| `{item['path']}` | {result_text} |")
(REPORT_DIR / 'validation.md').write_text('\n'.join(lines) + '\n', encoding='utf-8')
print(f"Validated {len(results)} files")
raise SystemExit(1 if validation_status == 'invalid' else 0)
