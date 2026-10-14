# Implementation Plan: Project website

**Branch**: `003-project-website` | **Date**: 2026-10-14 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `specs/003-project-website/spec.md` and `docs/spec-inputs/03-project-website.md` Part B

## Summary

A static documentation website in `site/`, built with Astro + Starlight. A small sync script turns the existing Markdown in `docs/` into Starlight pages at build time (title from the first heading, links rewritten), so `docs/` stays the single source. A custom landing page shows pitch, topology and milestone status. A new workflow `site.yml` builds the StrictDoc report, places it under `/requirements/`, builds the site, checks links and deploys to GitHub Pages; `requirements.yml` stops deploying.

## Technical Context

**Language/Version**: Node 22 LTS, JavaScript (ES modules); Python 3.12 for the existing requirements tooling
**Primary Dependencies**: astro 7.x, @astrojs/starlight 0.42.x, astro-mermaid 2.x + mermaid 11.x, starlight-links-validator 0.27.x, js-yaml
**Storage**: files only (Markdown in `docs/`, YAML data in `site/src/data/`)
**Testing**: `npm run build` (includes link validation), `npm run preview` manual check at 380 px; CI runs the same build on every PR
**Target Platform**: GitHub Pages, `https://santhosh-ing.github.io/Open_SDV_Bench/`
**Project Type**: static website (docs-as-code)
**Performance Goals**: publish within 10 min of merge (SC-003); static pages, no runtime server
**Constraints**: base path `/Open_SDV_Bench`; no copies of `docs/` content under version control; one Pages publisher
**Scale/Scope**: ~25 pages today, growing with milestones

## Constitution Check

`.specify/memory/constitution.md` is still the unfilled template, so the gates come from `AGENTS.md`:

| Rule (AGENTS.md) | Applies? | Status |
|---|---|---|
| 1 Contracts first | No change to `interfaces/` | Pass |
| 2 Traceability | Project infrastructure, no vehicle requirement; no requirement IDs invented. `req_check.py` still runs in CI | Pass |
| 3 Firmware layering | Not touched | Pass |
| 4 Determinism | Not applicable | Pass |
| 5 Small increments | One spec, one evening; tasks end with "build, preview, check live URL" | Pass |
| 6 Twin parity | Not applicable | Pass |
| 7 Code style | Scripts get a header comment (purpose, spec 003) | Pass |
| Docs: devlog "append, never rewrite" | Devlog is read, not modified, by the site | Pass |

Post-design re-check: unchanged, all pass.

## Project Structure

### Documentation (this feature)

```text
specs/003-project-website/
├── spec.md
├── plan.md              # this file
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/site-routes.md
├── checklists/requirements.md
└── tasks.md             # /speckit-tasks
```

### Source Code (repository root)

```text
site/
├── package.json             # scripts: sync, dev, build, preview
├── astro.config.mjs         # site/base, Starlight sidebar, Mermaid, link validator
├── scripts/sync-docs.mjs    # docs/ → src/content/docs/ (generated, gitignored)
├── src/
│   ├── content.config.ts
│   ├── content/docs/index.mdx         # landing page (splash)   ← committed
│   ├── content/docs/<generated>/      # from docs/              ← gitignored
│   ├── components/MilestoneTable.astro
│   ├── components/Topology.astro
│   ├── data/milestones.yaml
│   └── styles/custom.css
└── public/requirements/     # StrictDoc export, generated in CI (gitignored)
.github/workflows/site.yml   # build on PR, build + deploy on main
.github/workflows/requirements.yml  # Pages upload + publish job removed
```

**Structure Decision**: everything website-specific in `site/`; content stays in `docs/`.

## Complexity Tracking

No violations.
