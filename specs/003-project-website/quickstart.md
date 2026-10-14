# Quickstart: validate the project website

## Prerequisites

Node 22, Python 3.12.

## Local build

```bash
# optional: requirements report under /requirements/ (CI always does this)
pip install strictdoc==0.30.2
strictdoc export docs/requirements --formats html,reqif-sdoc --output-dir build/req
python3 tools/req_check.py --report-html build/req/html/traceability.html
mkdir -p site/public/requirements/reqif
cp -r build/req/html/. site/public/requirements/
cp build/req/reqif/output.reqif site/public/requirements/reqif/

cd site
npm ci
npm run build      # syncs docs/, builds, validates internal links
npm run preview    # http://localhost:4321/Open_SDV_Bench/
```

## Validation scenarios

| # | Do | Expect | Covers |
|---|---|---|---|
| 1 | Open `/` | Name, pitch, three buttons, topology, M0–M8 table | US1, FR-006–008 |
| 2 | Browser at 380 px width, open every sidebar section | No horizontal scroll | FR-014, SC-005 |
| 3 | Open Architecture → Software | Firmware layer diagram rendered | FR-011 |
| 4 | Open Requirements report link | StrictDoc report, traceability matrix, ReqIF download | FR-012 |
| 5 | Add a line to `docs/devlog.md`, rebuild | Devlog page shows it, nothing changed in `site/` | US4, SC-003 |
| 6 | Add a link `[x](../adr/9999-missing.md)` to a doc, rebuild | Build fails naming the link | FR-005, SC-004 |
| 7 | After merge: Actions → `site` run green, live URL opens | Site live | FR-003, FR-004 |

## One-time repo setting

Settings → Pages → Build and deployment → Source: **GitHub Actions**.
