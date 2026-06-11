#!/usr/bin/env python3
"""
Amalgamate the jobsys headers into a single distributable header.

Walks from --entry (relative to --root), inlining any local
#include "jobsys/..." headers recursively. #pragma once on inlined
files is stripped (the output gets a single one). #include <...>
(system/vendor) lines are collected, deduplicated, and emitted once
at the top. // IWYU pragma: lines are scrubbed.

Use --remap to emit #line directives so compiler diagnostics and
debuggers point back at the original split-file source locations
instead of the amalgamated output.
"""

import argparse
import re
from pathlib import Path

LOCAL_INCLUDE_RE = re.compile(r'^\s*#include\s*"([^"]+)"\s*$')
SYSTEM_INCLUDE_RE = re.compile(r"^\s*#include\s*<([^>]+)>\s*$")
PRAGMA_ONCE_RE = re.compile(r"^\s*#pragma\s+once\s*$")
IWYU_PRAGMA_RE = re.compile(r"^\s*//\s*IWYU pragma:")


def amalgamate(root: Path, entry: str, remap: bool):
    visited = set()
    system_includes = []  # order-preserving, deduped
    body_lines = []

    def process(rel_path: str):
        if rel_path in visited:
            return
        visited.add(rel_path)

        path = root / rel_path
        if not path.is_file():
            raise FileNotFoundError(f"Header not found: {path}")

        body_lines.append(
            f"\n// ─────────────────────────────────────────────\n"
            f"// {rel_path}\n"
            f"// ─────────────────────────────────────────────\n"
        )

        if remap:
            body_lines.append(f'#line 1 "{path.resolve()}"')

        for idx, line in enumerate(path.read_text().splitlines(), start=1):
            if PRAGMA_ONCE_RE.match(line):
                continue

            if IWYU_PRAGMA_RE.match(line):
                continue

            local_match = LOCAL_INCLUDE_RE.match(line)
            if local_match:
                inc = local_match.group(1)
                # Only treat as "local" (inline-able) if it resolves under root
                if (root / inc).is_file():
                    process(inc)
                    if remap:
                        body_lines.append(f'#line {idx + 1} "{path.resolve()}"')
                    continue
                # otherwise fall through and treat as a regular include

            system_match = SYSTEM_INCLUDE_RE.match(line)
            if system_match:
                if line not in system_includes:
                    system_includes.append(line)
                continue

            body_lines.append(line)

    process(entry)

    out = []
    out.append("// ════════════════════════════════════════════════════════════")
    out.append("// AUTO-GENERATED — do not edit directly.")
    out.append(f"// Generated from {entry} via scripts/amalgamate.py")
    if remap:
        out.append("// Built with --remap: #line directives map diagnostics")
        out.append("// back to the original split-file sources.")
    out.append("// ════════════════════════════════════════════════════════════")
    out.append("#pragma once")
    out.append("")
    out.extend(system_includes)
    out.extend(body_lines)
    out.append("")  # trailing newline

    return "\n".join(out)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--root",
        required=True,
        type=Path,
        help="Include root directory (e.g. jobsys/src)",
    )
    parser.add_argument(
        "--entry",
        required=True,
        help="Entry header, relative to --root (e.g. jobsys/job_system.hpp)",
    )
    parser.add_argument(
        "--output", required=True, type=Path, help="Output single-header file path"
    )
    parser.add_argument(
        "--remap",
        action="store_true",
        help="Emit #line directives mapping back to original sources",
    )
    args = parser.parse_args()

    result = amalgamate(args.root, args.entry, args.remap)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(result)
    print(
        f"Wrote {args.output} ({len(result.splitlines())} lines)"
        + (" [remapped]" if args.remap else "")
    )


if __name__ == "__main__":
    main()
