// astro.config.mjs — spec 003 (project website). Content is generated from ../docs
// by scripts/sync-docs.mjs; see specs/003-project-website/plan.md.
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';
import mermaid from 'astro-mermaid';
import starlightLinksValidator from 'starlight-links-validator';

const REPO = 'https://github.com/santhosh-ing/Open_SDV_Bench';

export default defineConfig({
  site: 'https://santhosh-ing.github.io',
  base: '/Open_SDV_Bench',
  trailingSlash: 'always',
  integrations: [
    mermaid({ theme: 'default', autoTheme: true }),
    starlight({
      title: 'Open SDV Bench',
      description: 'A software-defined vehicle on your desk, built with the Eclipse SDV stack.',
      social: [{ icon: 'github', label: 'GitHub', href: REPO }],
      editLink: { baseUrl: `${REPO}/edit/main/` },
      lastUpdated: false,
      customCss: ['./src/styles/custom.css'],
      plugins: [
        starlightLinksValidator({
          // The requirements report is copied in as static files (CI), not Starlight pages.
          exclude: ['/Open_SDV_Bench/requirements/', '/Open_SDV_Bench/requirements/**'],
        }),
      ],
      sidebar: [
        {
          label: 'Get started',
          items: [
            { label: 'Bill of materials', link: '/hardware/bom/' },
            { label: 'Wiring', link: '/hardware/wiring/' },
            { label: 'Guides', collapsed: false, items: [{ autogenerate: { directory: 'guides' } }] },
          ],
        },
        {
          label: 'Architecture',
          items: [
            { label: 'Overview (arc42)', link: '/architecture/' },
            { label: 'Logical view', link: '/architecture/logical/' },
            { label: 'Software view', link: '/architecture/software/' },
            { label: 'Network view', link: '/architecture/network/' },
            { label: 'Deployment view', link: '/architecture/deployment/' },
            { label: 'Decision records', collapsed: true, items: [{ autogenerate: { directory: 'adr' } }] },
          ],
        },
        {
          label: 'Requirements',
          items: [
            { label: 'How requirements are managed', link: '/requirements-management/' },
            { label: 'Requirements report', link: '/requirements/', attrs: { target: '_self' } },
            { label: 'Traceability matrix', link: '/requirements/traceability.html', attrs: { target: '_self' } },
          ],
        },
        { label: 'Devlog', link: '/devlog/' },
        { label: 'Docs index', link: '/docs/' },
      ],
    }),
  ],
});
