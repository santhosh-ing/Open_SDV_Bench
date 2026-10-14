# Research: Project website

## R1 – Site generator

- **Decision**: Astro + Starlight.
- **Rationale**: custom landing page (`template: splash`) and docs sections in one site, built-in search (Pagefind), dark mode, mobile layout, Mermaid via integration, room for interactive widgets later (FR-102).
- **Alternatives**: MkDocs Material (Python, close to StrictDoc, but interactive demos are clumsy); plain GitHub README (no structure, no diagrams beyond Mermaid).

## R2 – Using `docs/` without copying it

- **Decision**: a dependency-free Node script `site/scripts/sync-docs.mjs` runs before `dev`/`build` and writes generated pages into `site/src/content/docs/` (gitignored). For each Markdown file it takes the first `# Heading` as the Starlight `title`, removes that heading, rewrites relative links, and embeds `.mmd` diagram files of the same folder as Mermaid blocks.
- **Rationale**: Starlight requires a `title` in frontmatter; the repo's Markdown has none and must stay GitHub-friendly. Pointing the content loader straight at `../docs` would fail schema validation and leave relative links (`../adr/0003-….md`, `bom.md`) broken on the site.
- **Alternatives**: add frontmatter to every doc (clutters GitHub view, easy to forget); symlink `docs/` (same title and link problems).

## R3 – Link rewriting

- **Decision**: link to another published `.md` → its site route (`/Open_SDV_Bench/<route>/`, anchors kept); link to anything else in the repo (`AGENTS.md`, `.dbc`, source files, folders) → `https://github.com/santhosh-ing/Open_SDV_Bench/blob/main/<path>` (`tree/` for folders); images (`.svg`, `.png`) → copied next to the generated page so they render.
- **Rationale**: covers the spec's edge case "link to a non-website file"; `starlight-links-validator` then catches anything still broken (FR-005).

## R4 – Requirements report and Pages ownership

- **Decision**: `site.yml` runs the same StrictDoc export (`strictdoc==0.30.2`) and `tools/req_check.py --report-html` as `requirements.yml`, copies `build/req/html/` and `output.reqif` into `site/public/requirements/`, then builds. `requirements.yml` keeps validation and its artifact; its Pages upload step and `publish` job are removed.
- **Rationale**: one Pages publisher per repo (FR-012). Duplicating two commands is cheaper than cross-workflow artifact passing.
- **Note**: the links validator only checks Starlight pages; links into `/requirements/` are excluded from validation because the report is copied in as static files.

## R5 – Diagrams

- **Decision**: `astro-mermaid` renders ```` ```mermaid ```` blocks (client-side, follows light/dark theme). draw.io diagrams are already exported as `.drawio.svg` and are shown as images.

## R6 – Milestones single source

- **Decision**: `site/src/data/milestones.yaml`, read at build time by `MilestoneTable.astro`. README table stays hand-maintained (decided in spec).
