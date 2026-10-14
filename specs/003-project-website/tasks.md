# Tasks: Project website

**Input**: `specs/003-project-website/` (spec, plan, research, data-model, contracts, quickstart)
**Tests**: verification is the build itself (link validation) plus the quickstart scenarios; no unit-test suite for a static site.

## Phase 1: Setup

- [x] T001 Create `site/package.json` (astro, @astrojs/starlight, astro-mermaid, mermaid, starlight-links-validator, js-yaml; scripts `sync`, `dev`, `build`, `preview`) and lock file
- [x] T002 Add `site/.gitignore` entries for generated pages, `public/requirements/`, `dist/`, `node_modules/`
- [x] T003 Create `site/astro.config.mjs` with `site`, `base: '/Open_SDV_Bench'`, Starlight title, GitHub social link, edit links, sidebar, Mermaid and links validator; `site/src/content.config.ts`

## Phase 2: Foundational

- [x] T004 Write `site/scripts/sync-docs.mjs`: docs/ → `src/content/docs/` per contracts/site-routes.md (title from first heading, link rewriting per research R3, images copied, `.mmd` embedded, ADR index generated, spec-inputs excluded)

## Phase 3: User Story 1 – landing page (P1) 🎯 MVP

- [x] T005 [US1] `site/src/data/milestones.yaml` with M0–M8 (M0 in progress, M1–M8 planned, M8 proposed)
- [x] T006 [P] [US1] `site/src/components/MilestoneTable.astro`
- [x] T007 [P] [US1] `site/src/components/Topology.astro` (inline SVG of ADR 0003 topology, light/dark aware)
- [x] T008 [US1] `site/src/content/docs/index.mdx` (splash: hero with pitch + 3 actions, topology, "why" cards, milestones, traceability link)
- [x] T009 [US1] `site/src/styles/custom.css` (accent colour, narrow-screen safety for tables)

## Phase 4: User Story 2 + 3 – architecture, requirements, devlog, get started (P2/P3)

- [x] T010 [US2] Sidebar groups: Get started (BOM, wiring, guides), Architecture (overview, 4 views, ADRs), Requirements (management page + report link), Devlog
- [x] T011 [US2] Link to `/requirements/` report excluded from link validation; report link in sidebar and landing page

## Phase 5: User Story 4 – automatic publishing (P2)

- [x] T012 [US4] `.github/workflows/site.yml`: PR → requirements export + build; push to main (path filter) → build + upload-pages-artifact + deploy-pages
- [x] T013 [US4] `.github/workflows/requirements.yml`: remove Pages upload step and `publish` job, keep validation and artifact

## Phase 6: Polish

- [x] T014 Update `docs/requirements/README.md` "Published report" paragraph, `README.md` (website link + local build note), `docs/README.md` (site folder), devlog entry
- [x] T015 Run quickstart scenarios 1–6 locally (build, preview at 380 px, broken-link test)
- [ ] T016 Push branch, open PR; after merge maintainer enables Pages (Source: GitHub Actions) and checks scenario 7 on the live URL

## Dependencies

T001–T003 → T004 → US1 (T005–T009) → US2/3 (T010–T011) → US4 (T012–T013) → Polish. T006 and T007 in parallel.
