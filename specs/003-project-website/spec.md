# Feature Specification: Project website

**Feature Branch**: `003-project-website`

**Created**: 2026-10-14

**Status**: Draft

**Input**: `docs/spec-inputs/03-project-website.md`, Part A — a public project website for open-sdv-bench, built from the repository's existing documentation and published on every change to `main`.

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Understand the project at a glance (Priority: P1)

A visitor (recruiter, hiring manager, fellow engineer) opens the website and, without scrolling far, learns what the bench is, sees its architecture and knows where to go next.

**Why this priority**: The website exists to make the project understandable to people who will not read the repository. Without the landing page, nothing else matters.

**Independent Test**: Open the home page on a laptop and on a phone. The project name, a one-sentence pitch, the bench topology, the milestone status and links to "Get started", "Architecture" and the source repository are visible.

**Acceptance Scenarios**:

1. **Given** the published website, **When** a visitor opens the home page, **Then** it shows the project name, a one-sentence pitch and links to Get started, Architecture and the source repository.
2. **Given** the home page, **When** the visitor scrolls once, **Then** the bench topology shows the central computer and its four connections (CAN to the body zone controller, Ethernet to the ADAS domain and the sensor zone controller, IP to the cloud tier).
3. **Given** the home page, **When** the visitor looks at the milestone list, **Then** M0–M8 are listed with a status of planned, in progress or done.

---

### User Story 2 - Dig into architecture, requirements and progress (Priority: P2)

A systems engineer or reviewer browses the architecture views and decision records, opens the published requirements with their traceability matrix, and reads the devlog.

**Why this priority**: Shows the systems-engineering discipline that differentiates the project; it builds on content that already exists.

**Independent Test**: From the home page, reach each architecture view, the decision records, the requirements report and the devlog within two clicks; each page shows the same content as its source file in the repository.

**Acceptance Scenarios**:

1. **Given** the website, **When** the visitor opens Architecture, **Then** the logical, software, network and deployment views and the list of decision records are available, with their diagrams rendered as pictures.
2. **Given** the website, **When** the visitor opens Requirements, **Then** the full requirements report, the traceability matrix and a download of the requirements exchange file (ReqIF) are available.
3. **Given** the website, **When** the visitor opens the Devlog, **Then** the dated entries appear newest first, identical to the repository's devlog.

---

### User Story 3 - Rebuild the bench (Priority: P3)

An engineer who wants to build their own bench finds the bill of materials and the setup, build and flash guides.

**Why this priority**: Valuable for the open-source audience, but secondary to presenting the project.

**Independent Test**: From the home page, "Get started" leads to the bill of materials and both existing guides.

**Acceptance Scenarios**:

1. **Given** the website, **When** the visitor follows "Get started", **Then** the bill of materials and the firmware workflow and build-and-flash guides are reachable.

---

### User Story 4 - Maintainer updates content without touching the website (Priority: P2)

The maintainer edits a document in the repository (for example the devlog), merges the change, and the website updates by itself. A change that would break the website is caught before it is merged.

**Why this priority**: Without this, the website drifts from the repository and becomes a chore.

**Independent Test**: Merge a change to the devlog only; the published Devlog page shows the change. Open a change with a broken internal link; its automated check fails.

**Acceptance Scenarios**:

1. **Given** a merged change to any document the website uses, **When** the publishing run completes, **Then** the published page reflects the change without any edit in the website folder.
2. **Given** a proposed change that breaks the website build or an internal link, **When** the automated checks run, **Then** the check fails and names the problem.

---

### Edge Cases

- A document links to a repository file that is not part of the website (for example a `.dbc` contract or a source file): the link must still lead somewhere useful (the file on the repository host), not to a missing page.
- A diagram source that the website cannot render as a picture (for example a draw.io file) is shown as its exported image.
- The requirements report fails to generate: the website build fails rather than publishing without it.
- The repository name contains capitals and underscores (`Open_SDV_Bench`): all links work under that exact path.
- Visitor uses a dark-mode browser: all pages and diagrams stay readable.

## Requirements *(mandatory)*

### Functional Requirements — stage 1 (this spec)

- **FR-001**: The website MUST be a static documentation site with a custom landing page, documentation sections and built-in search.
- **FR-002**: The website's own sources (configuration, landing page, components) MUST live in one dedicated folder; content that already exists in `docs/` MUST be used from there, not copied.
- **FR-003**: The website MUST be published at `https://santhosh-ing.github.io/Open_SDV_Bench/`.
- **FR-004**: The website MUST be rebuilt and published automatically on every change to `main` that affects the website folder, `docs/`, `README.md` or the requirements tooling.
- **FR-005**: Every proposed change MUST run the website build; the check MUST fail if the build fails or an internal link is broken.
- **FR-006**: The landing page MUST show the project name, a one-sentence pitch and links to Get started, Architecture and the source repository.
- **FR-007**: The landing page MUST show the bench network topology (ADR 0003): central HPC; CAN to the body zone controller; Ethernet to the ADAS domain and the sensor zone controller; IP to the cloud tier.
- **FR-008**: The landing page MUST list milestones M0–M8 with status planned, in progress or done, taken from a single data source.
- **FR-009**: The website MUST provide a Get started section with the bill of materials and the existing guides.
- **FR-010**: The website MUST provide an Architecture section with the four architecture views and the decision records.
- **FR-011**: Diagrams MUST remain diagrams-as-code in the repository and be shown as rendered pictures.
- **FR-012**: The requirements report, the traceability matrix and the ReqIF file MUST be published under `/requirements/` of the same website; the website MUST be the only publisher to the project's Pages site.
- **FR-013**: The website MUST provide a Devlog page built from `docs/devlog.md`.
- **FR-014**: All pages MUST be readable on a 380 px wide screen without horizontal scrolling.

### Stage 2 (after M2, out of scope for this spec's tasks)

- **FR-101**: Each implemented vehicle feature gets a showcase page tracing need → system → function → component requirement → firmware module → CAN frame → test.
- **FR-102**: First showcase: "Why does the low beam switch on when the light commands stop?" (SW-ZCS-021) with an in-browser simulation.

### Resolved decisions

- **Pages ownership**: the website workflow owns the Pages site and includes the requirements report. The requirements workflow keeps validating, building the traceability matrix and uploading its artifact, but no longer deploys.
- **Milestone source**: one data file used by the website; the README table stays hand-maintained for now (small, rarely changing).
- **Pitch**: "A production-style software-defined vehicle on a desk — zone controllers, central computer, ADAS and cloud, built with Eclipse SDV." Maintainer may reword.
- **Requirement IDs**: project infrastructure; this spec traces to no vehicle requirement. FR numbers are spec-local.

### Key Entities

- **Milestone**: identifier (M0–M8), title, scope sentence, status (planned / in progress / done).
- **Page**: a document from `docs/` or the README shown on the website, keeping its title and content.
- **Requirements report**: generated output of the requirements tooling (report pages, traceability matrix, ReqIF file).

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: A first-time visitor can state what the bench is and name its main nodes after reading the landing page for 30 seconds.
- **SC-002**: Every architecture view, decision record, guide, the devlog and the requirements report are reachable from the home page in at most two clicks.
- **SC-003**: A merged documentation change appears on the published website within 10 minutes, with zero edits in the website folder.
- **SC-004**: 100 % of internal links resolve; a deliberately broken link fails the pre-merge check.
- **SC-005**: All pages render without horizontal scrolling at 380 px width.

## Assumptions

- GitHub Pages is free for this public repository; the maintainer sets "Pages → Source: GitHub Actions" once.
- A custom domain, analytics, comments and translations are out of scope.
- Existing Markdown files may receive small, GitHub-compatible changes (for example a title line) if the website needs them; their content stays the single source.
