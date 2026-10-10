#!/usr/bin/env python3
"""Requirement rules check for Open SDV Bench.

StrictDoc already validates the .sdoc syntax, duplicate UIDs and broken links
(`strictdoc export docs/requirements`). This script adds the project rules on top:

  1. IDs follow the scheme STK-NNN, SYS-<AREA>-NNN, FUN-<AREA>-NNN, SW|HW-<NODE>-NNN.
  2. Every requirement below STK has at least one parent, one level up
     (SYS -> STK, FUN -> SYS, SW/HW -> FUN, or SYS for constraints).
  3. No active requirement traces to an Obsolete parent.
  4. Requirements with status Implemented or Verified are referenced by at least one
     test (`@verifies <ID>` in a test or source file). Tests must not reference unknown IDs.

Optional outputs:
  --report-md / --report-html  traceability matrix (requirement -> children -> tests)
  --impact <git-ref>           suspect-link review: children of requirements whose
                               title or statement changed since <git-ref> (warnings only)

Usage:
  python3 tools/req_check.py
  python3 tools/req_check.py --report-html build/req/traceability.html
  python3 tools/req_check.py --impact origin/main
"""
from __future__ import annotations

import argparse
import html
import os
import re
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
REQ_DIR = REPO / "docs" / "requirements"

LEVELS = {"STK": 0, "SYS": 1, "FUN": 2, "SW": 3, "HW": 3}
ID_RE = re.compile(r"^(STK-\d{3}|(SYS|FUN)-[A-Z]+-\d{3}|(SW|HW)-[A-Z]+-\d{3})$")
VERIFIES_RE = re.compile(r"@verifies\s+((?:STK|SYS|FUN|SW|HW)(?:-[A-Z]+)?-\d{3})")
NEEDS_TEST = {"Implemented", "Verified"}
SCAN_SKIP_DIRS = {".git", ".specify", ".claude", "node_modules", "build", "docs", "__pycache__"}
SCAN_SKIP_SUFFIXES = {".md", ".sdoc", ".sgra", ".svg", ".png"}


@dataclass
class Req:
    uid: str
    file: str
    fields: dict[str, str] = field(default_factory=dict)
    parents: list[str] = field(default_factory=list)

    @property
    def level(self) -> str:
        return self.uid.split("-")[0]

    @property
    def status(self) -> str:
        return self.fields.get("STATUS", "")

    @property
    def title(self) -> str:
        return self.fields.get("TITLE", "")


def parse_sdoc(text: str, filename: str) -> list[Req]:
    """Parse the [REQUIREMENT] nodes of one .sdoc file (project grammar only)."""
    reqs: list[Req] = []
    for block in re.split(r"\n(?=\[)", text):
        if not block.startswith("[REQUIREMENT]"):
            continue
        fields: dict[str, str] = {}
        for m in re.finditer(r"^([A-Z_]+): >>>\n(.*?)\n<<<", block, re.S | re.M):
            fields[m.group(1)] = m.group(2).strip()
        for m in re.finditer(r"^([A-Z_]+): (?!>>>)(.+)$", block, re.M):
            fields.setdefault(m.group(1), m.group(2).strip())
        parents = re.findall(r"- TYPE: Parent\n\s+VALUE: (\S+)", block)
        uid = fields.get("UID", "")
        reqs.append(Req(uid=uid, file=filename, fields=fields, parents=parents))
    return reqs


def load_requirements(read=lambda p: p.read_text(encoding="utf-8")) -> dict[str, Req]:
    reqs: dict[str, Req] = {}
    for path in sorted(REQ_DIR.glob("*.sdoc")):
        for r in parse_sdoc(read(path), path.name):
            reqs[r.uid] = r
    return reqs


def load_requirements_at(ref: str) -> dict[str, Req]:
    """Requirements as they were at a git ref (files that did not exist are skipped)."""
    rel = REQ_DIR.relative_to(REPO).as_posix()
    try:
        names = subprocess.run(["git", "ls-tree", "--name-only", f"{ref}:{rel}"], cwd=REPO,
                               capture_output=True, text=True, check=True).stdout.split()
    except subprocess.CalledProcessError:
        return {}
    reqs: dict[str, Req] = {}
    for name in names:
        if not name.endswith(".sdoc"):
            continue
        text = subprocess.run(["git", "show", f"{ref}:{rel}/{name}"], cwd=REPO,
                              capture_output=True, text=True, check=True).stdout
        for r in parse_sdoc(text, name):
            reqs[r.uid] = r
    return reqs


def find_test_references() -> dict[str, list[str]]:
    """Map requirement ID -> files that contain `@verifies <ID>`."""
    refs: dict[str, list[str]] = {}
    for root, dirs, files in os.walk(REPO):
        dirs[:] = [d for d in dirs if d not in SCAN_SKIP_DIRS]
        for name in files:
            path = Path(root) / name
            if path.suffix in SCAN_SKIP_SUFFIXES or path.resolve() == Path(__file__).resolve():
                continue
            try:
                text = path.read_text(encoding="utf-8")
            except (UnicodeDecodeError, OSError):
                continue
            for uid in VERIFIES_RE.findall(text):
                refs.setdefault(uid, []).append(path.relative_to(REPO).as_posix())
    return refs


def children_of(reqs: dict[str, Req]) -> dict[str, list[str]]:
    kids: dict[str, list[str]] = {uid: [] for uid in reqs}
    for r in reqs.values():
        for p in r.parents:
            kids.setdefault(p, []).append(r.uid)
    return kids


def check(reqs: dict[str, Req], tests: dict[str, list[str]]) -> tuple[list[str], list[str]]:
    errors: list[str] = []
    warnings: list[str] = []
    kids = children_of(reqs)
    for r in reqs.values():
        where = f"{r.file}: {r.uid}"
        if not ID_RE.match(r.uid):
            errors.append(f"{where}: ID does not follow the scheme (see docs/requirements/README.md)")
            continue
        if r.level != "STK" and not r.parents:
            errors.append(f"{where}: has no parent requirement")
        for p in r.parents:
            parent = reqs.get(p)
            if parent is None:
                errors.append(f"{where}: parent {p} does not exist")
                continue
            allowed = {"SYS": {"STK"}, "FUN": {"SYS"}, "SW": {"FUN", "SYS"}, "HW": {"FUN", "SYS"}}
            if parent.level not in allowed.get(r.level, set()):
                errors.append(f"{where}: parent {p} is not one level up ({r.level} -> {parent.level})")
            if parent.status == "Obsolete" and r.status != "Obsolete":
                errors.append(f"{where}: traces to obsolete parent {p}")
        if r.status in NEEDS_TEST and r.uid not in tests:
            errors.append(f"{where}: status {r.status} but no test has '@verifies {r.uid}'")
        if r.status != "Obsolete" and r.level in ("SYS", "FUN") and not kids.get(r.uid):
            nxt = "function (FUN)" if r.level == "SYS" else "component (SW/HW)"
            warnings.append(f"{where}: not yet refined into a {nxt} requirement")
    for uid, files in tests.items():
        if uid not in reqs:
            errors.append(f"{', '.join(files)}: '@verifies {uid}' references an unknown requirement")
    return errors, warnings


def impact(reqs: dict[str, Req], ref: str) -> list[str]:
    """Suspect links: children whose parent's title or statement changed since `ref`."""
    old = load_requirements_at(ref)
    kids = children_of(reqs)
    notes: list[str] = []
    for uid, r in reqs.items():
        before = old.get(uid)
        if before is None:
            continue
        changed = [f for f in ("TITLE", "STATEMENT") if before.fields.get(f) != r.fields.get(f)]
        if r.status == "Obsolete" and before.status != "Obsolete":
            changed.append("STATUS -> Obsolete")
        if changed and kids.get(uid):
            notes.append(f"{uid} changed ({', '.join(changed)}): review children "
                         f"{', '.join(sorted(kids[uid]))}")
    return notes


def report_rows(reqs: dict[str, Req], tests: dict[str, list[str]]):
    kids = children_of(reqs)
    order = sorted(reqs.values(), key=lambda r: (LEVELS.get(r.level, 9), r.uid))
    for r in order:
        yield r, sorted(kids.get(r.uid, [])), sorted(set(tests.get(r.uid, [])))


def write_markdown(path: Path, reqs, tests) -> None:
    lines = ["# Traceability matrix", "",
             "| ID | Title | Status | Parents | Children | Tests |", "|---|---|---|---|---|---|"]
    for r, kids, files in report_rows(reqs, tests):
        lines.append(f"| {r.uid} | {r.title} | {r.status} | {', '.join(r.parents) or '-'} | "
                     f"{', '.join(kids) or '-'} | {', '.join(files) or '-'} |")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_html(path: Path, reqs, tests) -> None:
    esc = html.escape
    rows = []
    for r, kids, files in report_rows(reqs, tests):
        link = lambda u: f'<a href="#{esc(u)}">{esc(u)}</a>'  # noqa: E731
        rows.append(
            f'<tr id="{esc(r.uid)}"><td><b>{esc(r.uid)}</b></td><td>{esc(r.title)}</td>'
            f'<td class="s-{esc(r.status.lower())}">{esc(r.status)}</td>'
            f'<td>{esc(r.fields.get("MILESTONE", "-"))}</td>'
            f'<td>{", ".join(map(link, r.parents)) or "-"}</td>'
            f'<td>{", ".join(map(link, kids)) or "-"}</td>'
            f'<td>{"<br>".join(map(esc, files)) or "-"}</td></tr>')
    counts: dict[str, int] = {}
    for r in reqs.values():
        counts[r.status] = counts.get(r.status, 0) + 1
    summary = " · ".join(f"{k}: {v}" for k, v in sorted(counts.items()))
    doc = f"""<!doctype html><html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Traceability matrix</title><style>
body{{font-family:system-ui,sans-serif;margin:24px;color:#222}}
table{{border-collapse:collapse;width:100%;font-size:14px}}
th,td{{border:1px solid #ddd;padding:6px 8px;text-align:left;vertical-align:top}}
th{{background:#f4f4f4}} tr:target{{background:#fff7d6}}
.s-verified{{color:#0f6e56;font-weight:600}} .s-obsolete{{color:#999}}
</style></head><body>
<h1>Traceability matrix</h1>
<p>Requirement → children → tests, generated by <code>tools/req_check.py</code>.
Full requirement documents: <a href="index.html">StrictDoc export</a> ·
ReqIF: <a href="reqif/output.reqif">output.reqif</a></p>
<p>{len(reqs)} requirements · {esc(summary)}</p>
<table><thead><tr><th>ID</th><th>Title</th><th>Status</th><th>Milestone</th>
<th>Parents</th><th>Children</th><th>Tests</th></tr></thead><tbody>
{chr(10).join(rows)}
</tbody></table></body></html>
"""
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(doc, encoding="utf-8")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--report-md", type=Path)
    ap.add_argument("--report-html", type=Path)
    ap.add_argument("--impact", metavar="GIT_REF")
    args = ap.parse_args()

    gha = os.environ.get("GITHUB_ACTIONS") == "true"
    reqs = load_requirements()
    tests = find_test_references()
    errors, warnings = check(reqs, tests)

    for w in warnings:
        print(f"::warning title=Requirement::{w}" if gha else f"WARN  {w}")
    for e in errors:
        print(f"::error title=Requirement::{e}" if gha else f"ERROR {e}")
    if args.impact:
        for n in impact(reqs, args.impact):
            print(f"::warning title=Suspect link::{n}" if gha else f"SUSPECT {n}")
    if args.report_md:
        write_markdown(args.report_md, reqs, tests)
    if args.report_html:
        write_html(args.report_html, reqs, tests)

    print(f"{len(reqs)} requirements, {len(tests)} referenced by tests, "
          f"{len(errors)} error(s), {len(warnings)} warning(s)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
