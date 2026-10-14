// sync-docs.mjs — spec 003 (project website), FR-002, FR-009..FR-013
//
// Purpose: turn the repository's Markdown in ../docs into Starlight pages
// without copying them into git. Runs before `astro dev` / `astro build`.
//
// For every published .md file it:
//   - takes the first "# Heading" as the page title and removes it from the body
//   - rewrites relative links: other published docs -> site routes,
//     everything else in the repo -> the file on GitHub
//   - copies referenced images to public/docs-assets/
//   - embeds *.mmd diagram files of the same folder as Mermaid blocks
// Route rules: specs/003-project-website/contracts/site-routes.md
//
// The output folder src/content/docs/ is gitignored except index.mdx.

import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const SITE_DIR = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const REPO_DIR = path.resolve(SITE_DIR, '..');
const DOCS_DIR = path.join(REPO_DIR, 'docs');
const OUT_DIR = path.join(SITE_DIR, 'src', 'content', 'docs');
// Images are served as-is (draw.io SVGs have no fixed size, so Astro's image pipeline rejects them).
const ASSET_DIR = path.join(SITE_DIR, 'public', 'docs-assets');

export const BASE = '/Open_SDV_Bench';
export const REPO_URL = 'https://github.com/santhosh-ing/Open_SDV_Bench';
const BRANCH = 'main';

// Folders under docs/ that are not published (linked to GitHub instead).
const EXCLUDED = ['spec-inputs'];
// Route overrides: /requirements/ is reserved for the StrictDoc report.
const ROUTE_OVERRIDES = { 'requirements/README.md': 'requirements-management' };
// Relative links whose target does not exist in the repo; reported and fail the build (FR-005).
const broken = [];
const IMAGE_EXT = new Set(['.svg', '.png', '.jpg', '.jpeg', '.gif', '.webp']);

/** All .md files below docs/, as paths relative to docs/ with forward slashes. */
function listDocs(dir = DOCS_DIR, rel = '') {
  const out = [];
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const relPath = rel ? `${rel}/${entry.name}` : entry.name;
    if (entry.isDirectory()) {
      if (!rel && EXCLUDED.includes(entry.name)) continue;
      out.push(...listDocs(path.join(dir, entry.name), relPath));
    } else if (entry.name.endsWith('.md')) {
      out.push(relPath);
    }
  }
  return out.sort();
}

/** docs-relative .md path -> route without slashes, e.g. "adr/0003-bench-network-topology". */
function routeOf(rel) {
  if (ROUTE_OVERRIDES[rel]) return ROUTE_OVERRIDES[rel];
  let r = rel.replace(/\.md$/, '');
  if (r === 'README') r = 'docs';
  else if (r.endsWith('/README')) r = r.slice(0, -'/README'.length);
  return r.toLowerCase();
}

/** Where the generated file for a route is written (Starlight: <route>.md or <route>/index.md). */
function outFileOf(route, isIndex) {
  return path.join(OUT_DIR, isIndex ? `${route}/index.md` : `${route}.md`);
}

function githubUrl(repoRel, isDir) {
  return `${REPO_URL}/${isDir ? 'tree' : 'blob'}/${BRANCH}/${repoRel}`;
}

function rewriteLinks(body, srcRel, routes, outFile) {
  const srcDir = path.posix.dirname(`docs/${srcRel}`);
  // [text](target) and ![alt](target); skip absolute URLs, anchors, mailto.
  return body.replace(/(!?)\[([^\]]*)\]\(([^)\s]+)(\s+"[^"]*")?\)/g, (match, bang, text, target, title = '') => {
    if (/^([a-z]+:|#|\/)/i.test(target)) return match;
    const [rawPath, anchor = ''] = target.split('#');
    const repoRel = path.posix.normalize(path.posix.join(srcDir, decodeURIComponent(rawPath)));
    const abs = path.join(REPO_DIR, repoRel);
    const hash = anchor ? `#${anchor}` : '';
    if (rawPath && !fs.existsSync(abs)) {
      broken.push(`docs/${srcRel}: ${target}`);
      return match;
    }
    const ext = path.extname(repoRel).toLowerCase();

    // Link to another published doc (file or folder with README.md).
    let docRel = null;
    if (repoRel.startsWith('docs/') && repoRel.endsWith('.md')) docRel = repoRel.slice(5);
    else if (repoRel.startsWith('docs/') && fs.existsSync(path.join(abs, 'README.md'))) docRel = `${repoRel.slice(5)}/README.md`;
    if (docRel && routes.has(docRel)) {
      return `${bang}[${text}](${BASE}/${routes.get(docRel)}/${hash}${title})`;
    }
    // Generated ADR index for the adr/ folder.
    if (repoRel === 'docs/adr' || repoRel === 'docs/adr/') {
      return `${bang}[${text}](${BASE}/adr/${hash}${title})`;
    }
    // Images: copy to public/docs-assets/ and reference them by URL.
    if (bang && IMAGE_EXT.has(ext) && fs.existsSync(abs)) {
      const assetRel = repoRel.replace(/^docs\//, '');
      fs.mkdirSync(path.dirname(path.join(ASSET_DIR, assetRel)), { recursive: true });
      fs.copyFileSync(abs, path.join(ASSET_DIR, assetRel));
      return `![${text}](${BASE}/docs-assets/${assetRel}${title})`;
    }
    // Anything else in the repo: show it on GitHub.
    const isDir = fs.existsSync(abs) && fs.statSync(abs).isDirectory();
    return `${bang}[${text}](${githubUrl(repoRel, isDir)}${hash}${title})`;
  });
}

/** Mermaid sources (*.mmd) in the doc's folder, appended as rendered diagrams. */
function mermaidAppendix(srcRel) {
  if (!srcRel.endsWith('README.md')) return '';
  const dir = path.join(DOCS_DIR, path.dirname(srcRel));
  const files = fs.readdirSync(dir).filter((f) => f.endsWith('.mmd')).sort();
  return files
    .map((f) => {
      const src = fs.readFileSync(path.join(dir, f), 'utf8').trim();
      return `\n\n## Diagram\n\nRendered from \`${f}\`.\n\n\`\`\`mermaid\n${src}\n\`\`\`\n`;
    })
    .join('');
}

function yamlString(s) {
  return JSON.stringify(s); // JSON strings are valid YAML scalars
}

function writePage(outFile, { title, description, editPath, body }) {
  fs.mkdirSync(path.dirname(outFile), { recursive: true });
  const fm = ['---', `title: ${yamlString(title)}`];
  if (description) fm.push(`description: ${yamlString(description)}`);
  if (editPath) fm.push(`editUrl: ${yamlString(`${REPO_URL}/edit/${BRANCH}/${editPath}`)}`);
  fm.push('---', '');
  fs.writeFileSync(outFile, fm.join('\n') + body.trimStart() + '\n');
}

function main() {
  fs.rmSync(ASSET_DIR, { recursive: true, force: true });
  // Clean previously generated pages, keep the hand-written landing page.
  for (const entry of fs.readdirSync(OUT_DIR)) {
    if (entry === 'index.mdx') continue;
    fs.rmSync(path.join(OUT_DIR, entry), { recursive: true, force: true });
  }

  const docs = listDocs();
  const routes = new Map(docs.map((rel) => [rel, routeOf(rel)]));
  // A route is an index page if another route lives below it.
  const allRoutes = [...routes.values()];
  const isIndexRoute = (r) => allRoutes.some((o) => o.startsWith(`${r}/`));

  for (const rel of docs) {
    const route = routes.get(rel);
    const outFile = outFileOf(route, isIndexRoute(route) || rel.endsWith('README.md'));
    fs.mkdirSync(path.dirname(outFile), { recursive: true });
    let text = fs.readFileSync(path.join(DOCS_DIR, rel), 'utf8').replace(/\r\n/g, '\n');
    let title = path.basename(rel, '.md');
    const h1 = text.match(/^#\s+(.+?)\s*$/m);
    if (h1) {
      title = h1[1];
      text = text.replace(h1[0], '');
    }
    const body = rewriteLinks(text, rel, routes, outFile) + mermaidAppendix(rel);
    writePage(outFile, { title, editPath: `docs/${rel}`, body });
  }

  // Generated index for decision records (docs/adr has no README).
  const adrs = docs.filter((rel) => /^adr\/\d{4}-.+\.md$/.test(rel));
  if (adrs.length && !routes.has('adr/README.md')) {
    const rows = adrs.map((rel) => {
      const t = (fs.readFileSync(path.join(DOCS_DIR, rel), 'utf8').match(/^#\s+(.+?)\s*$/m) || [, rel])[1];
      return `- [${t}](${BASE}/${routes.get(rel)}/)`;
    });
    writePage(path.join(OUT_DIR, 'adr', 'index.md'), {
      title: 'Architecture decision records',
      description: 'One decision per record, newest last.',
      body: `Each record states the context, the decision and its consequences. Source: [\`docs/adr/\`](${githubUrl('docs/adr', true)}).\n\n${rows.join('\n')}\n`,
    });
  }

  if (broken.length) {
    console.error(`sync-docs: ${broken.length} broken relative link(s):\n  ${broken.join('\n  ')}`);
    process.exit(1);
  }
  console.log(`sync-docs: ${docs.length} pages from docs/ -> ${path.relative(SITE_DIR, OUT_DIR)}/`);
}

main();
