# Spec 003 – Project website

Goal of this feature: a public project website for Open SDV Bench on GitHub Pages, modelled on project landing pages such as [rea.tools](https://rea.tools/): a short pitch, the bench architecture at a glance, a way to get started, worked examples and a devlog. The website reuses the docs that already live in this repo (docs-as-code); it does not duplicate them.

Prerequisite: none. This is project infrastructure and can run in parallel with the M1 firmware specs. It is built in two stages: the skeleton now (M0), showcases once the first vehicle feature works on the board (after M2).

---

## Part A — paste after `/speckit-specify`

Project website for open-sdv-bench, published on GitHub Pages at `https://santhosh-ing.github.io/open_sdv_bench/`, built from the repository on every push to `main`.

**Why:** The bench is a portfolio project. A visitor (recruiter, hiring manager, fellow engineer) should understand within a few seconds what the bench is and why it is built the way it is, then be able to dig into architecture, requirements and progress. The GitHub README alone does not do that well: it cannot show diagrams interactively, group guides and views, or tell the story milestone by milestone. The requirements report (StrictDoc HTML, traceability matrix, ReqIF) is already built by CI and should be part of the same site instead of a separate one.

**User stories**

1. As a visitor, I open the landing page and see the project name, a one-sentence pitch, the bench topology and links to "Get started", "Architecture" and the GitHub repository.
2. As a visitor, I see which milestones (M0–M8) are done, in progress or planned.
3. As an engineer who wants to rebuild the bench, I find the bill of materials and the setup, build and flash guides.
4. As a systems engineer, I browse the architecture views (logical, software, network, deployment) and the ADRs.
5. As a reviewer, I open the published requirements, the traceability matrix and the ReqIF export from the same site.
6. As a returning visitor, I read the devlog to see what changed recently.
7. As the maintainer, I change a Markdown file in `docs/`, merge the PR and the website updates by itself. A PR that breaks the website build or an internal link fails its check.

**Functional requirements — stage 1 (M0)**

- FR-001: The website shall be built with Astro and the Starlight documentation theme.
- FR-002: The website source (configuration, landing page, components) shall live in `site/`. Content that already exists in `docs/` shall be used from there, not copied.
- FR-003: The website shall be published on GitHub Pages at `https://santhosh-ing.github.io/open_sdv_bench/`.
- FR-004: A GitHub Actions workflow shall build and deploy the website on every push to `main` that changes `site/`, `docs/` or `README.md`.
- FR-005: The website build shall run on every pull request and fail if the build fails or an internal link is broken.
- FR-006: The landing page shall show the project name, a one-sentence pitch and links to "Get started", "Architecture" and the GitHub repository.
- FR-007: The landing page shall show the bench network topology: Raspberry Pi 5 as central HPC, CAN to the STM32 body zone controller, Ethernet to the Jetson Orin (ADAS) and the Arduino sensor zone controller, and IP to the OpenSDV.Cloud tier (ADR 0003).
- FR-008: The landing page shall list milestones M0–M8 with their status (planned, in progress, done), taken from one data file so the README and the website do not drift.
- FR-009: The website shall provide a "Get started" section built from `docs/hardware/bom.md` and `docs/guides/`.
- FR-010: The website shall provide an Architecture section built from `docs/architecture/` (logical, software, network, deployment views) and `docs/adr/`.
- FR-011: Diagrams shall stay diagrams-as-code (Mermaid rendered by the site; existing draw.io SVGs embedded as images).
- FR-012: The StrictDoc HTML export, the traceability matrix and the ReqIF file shall be published under `/requirements/` of the same site.
- FR-013: The website shall provide a Devlog page built from `docs/devlog.md`.
- FR-014: All pages shall be readable on a 380 px wide screen without horizontal scrolling.

**Functional requirements — stage 2 (after M2, not part of this spec's tasks)**

- FR-101: Each implemented vehicle feature shall get a showcase page that traces stakeholder need → system → function → component requirement → firmware module → CAN frame → test with `@verifies`.
- FR-102: The first showcase shall answer "Why does the low beam switch on when the light commands stop?" (SW-ZCS-021, fail-safe on command timeout) and include an in-browser simulation: the visitor stops the command stream and sees the timeout, the lamp states and the resulting BODY_LightStatus frame.

**Out of scope:** custom domain, blog comments, analytics, translations, search beyond Starlight's built-in search.

**Traces to:** project infrastructure, no vehicle requirement. FR numbers are spec-local. [clarify: should project/tooling requirements get their own StrictDoc file and ID scheme (e.g. `PRJ-NNN`), which would need an extension of `tools/req_check.py`?]

**Acceptance (verified by the maintainer):**
- The site is live at the URL above and opens cleanly on a phone.
- Landing page shows pitch, topology, milestone status and the three links.
- `/requirements/` shows the StrictDoc export, the traceability matrix and the ReqIF download.
- Editing `docs/devlog.md` in a PR and merging it updates the Devlog page without touching `site/`.
- A PR with a deliberately broken internal link fails the website check.

**Open questions for `/speckit-clarify`:**
- Pages conflict: `requirements.yml` can already publish the requirements report to Pages (variable `REQ_REPORT_PAGES`). A repo has one Pages site. Proposal: the new website workflow owns Pages and includes the report; `requirements.yml` keeps validating and uploading the `requirements-report` artifact but no longer deploys.
- Milestone status: one YAML data file read by the site, with the README table kept by hand, or the README table generated from it?
- Pitch wording for the hero (maintainer writes it; placeholder `TODO(Santhosh)` until then).

---

## Part B — paste after `/speckit-plan`

Technical context for the plan:

- **Stack:** Astro + `@astrojs/starlight`, Node LTS, `npm ci`. Mermaid via a Starlight/Astro Mermaid integration (e.g. `astro-mermaid`). Link check with `starlight-links-validator`.
- **Paths:** `site/astro.config.mjs` with `site: 'https://santhosh-ing.github.io'` and `base: '/open_sdv_bench'`. Landing page `site/src/content/docs/index.mdx` with `template: splash`.
- **Reusing `docs/`:** prefer pointing Starlight's content collection at `docs/` (Astro content loader with a glob over `../docs`) over copying files. If frontmatter is required, add it to the existing Markdown files rather than duplicating them. Keep relative links between docs working on GitHub too.
- **Milestones:** `site/src/data/milestones.yaml` (id, title, scope, status), rendered by `site/src/components/MilestoneTable.astro`.
- **Requirements report:** in the website workflow, install `strictdoc==0.30.2`, run the same export and `tools/req_check.py --report-html` as `requirements.yml`, and copy `build/req/html/` plus `build/req/reqif/output.reqif` into `site/public/requirements/` before `npm run build`. `site/public/requirements/` is gitignored.
- **Workflow:** `.github/workflows/site.yml` — on `pull_request`: build + link check; on `push` to `main` with path filter (`site/**`, `docs/**`, `README.md`, `tools/req_check.py`): build, `actions/upload-pages-artifact`, `actions/deploy-pages`. Remove the `publish` job and the Pages upload step from `requirements.yml` in the same PR.
- **Repo setting (maintainer, once):** Settings → Pages → Source: GitHub Actions.
- **Tasks end with:** run `npm run build` locally, open `npm run preview` on a phone-width viewport, merge, check the live URL.
- Commit messages: `spec-003: <what>`.
