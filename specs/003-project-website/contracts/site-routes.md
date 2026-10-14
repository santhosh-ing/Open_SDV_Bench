# Contract: public routes of the website

Base: `https://santhosh-ing.github.io/Open_SDV_Bench`. These URLs are the website's external interface; once published they should stay stable (links in CVs, posts).

| Route | Source |
|---|---|
| `/` | `site/src/content/docs/index.mdx` (landing) |
| `/docs/` | `docs/README.md` |
| `/architecture/` | `docs/architecture/README.md` |
| `/architecture/{logical,software,network,deployment}/` | `docs/architecture/<view>/README.md` (+ `*.mmd` embedded) |
| `/adr/` | `docs/adr/README.md` if present, else generated index |
| `/adr/<file-stem>/` | `docs/adr/NNNN-*.md` |
| `/hardware/bom/` | `docs/hardware/bom.md` |
| `/hardware/wiring/` | `docs/hardware/wiring/README.md` |
| `/guides/`, `/guides/<file-stem>/` | `docs/guides/*.md` |
| `/requirements-management/` | `docs/requirements/README.md` (renamed: `/requirements/` is the report) |
| `/devlog/` | `docs/devlog.md` |
| `/requirements/` | StrictDoc HTML export (static) |
| `/requirements/traceability.html` | traceability matrix |
| `/requirements/reqif/output.reqif` | ReqIF download |

Not published: `docs/spec-inputs/` (internal working inputs; linked to GitHub instead).

Rule: `<dir>/README.md` → `<dir>/`; `<dir>/<name>.md` → `<dir>/<name>/`; route segments are lower-case.
