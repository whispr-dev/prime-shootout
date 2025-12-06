#!/usr/bin/env python3
import re, sys, subprocess, statistics, csv, argparse
from pathlib import Path

LINE_RE = re.compile(
    r"^(?P<prog>\S+)\s+"
    r"(?P<avg>\d+(?:\.\d+)?)\s+"
    r"(?P<min>\d+(?:\.\d+)?)\s+"
    r"(?P<max>\d+(?:\.\d+)?)\s+"
    r"(?P<std>\d+(?:\.\d+)?)\s*$"
)

def parse_table(text: str):
    rows = {}
    for line in text.splitlines():
        m = LINE_RE.match(line.strip())
        if not m:
            continue
        prog = m.group("prog")
        rows[prog] = {
            "avg": float(m.group("avg")),
            "min": float(m.group("min")),
            "max": float(m.group("max")),
            "std": float(m.group("std")),
        }
    return rows

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe", help="D:/code/prime-shootout/build/dist/round-Aux/cs_exe_bm_mkviii.exe")
    ap.add_argument("-n", "--runs", type=int, default=10)
    ap.add_argument("-o", "--out", default="", help="Optional CSV output path")
    ap.add_argument("--filter", nargs="*", default=[], help="Only include these program names")
    args = ap.parse_args()

    exe = Path(args.exe)
    if not exe.exists():
        print(f"Exe not found: {exe}", file=sys.stderr)
        sys.exit(1)

    per_prog_avgs = {}
    per_run_rows = []

    for i in range(args.runs):
        proc = subprocess.run([str(exe)], capture_output=True, text=True, check=True)
        rows = parse_table(proc.stdout)

        if args.filter:
            rows = {k: v for k, v in rows.items() if k in set(args.filter)}

        per_run_rows.append(rows)
        for prog, v in rows.items():
            per_prog_avgs.setdefault(prog, []).append(v["avg"])

        print(f"Run {i+1}/{args.runs} parsed: {len(rows)} programs")

    # Summary
    progs = sorted(per_prog_avgs.keys(), key=lambda p: statistics.mean(per_prog_avgs[p]))
    print("\n10-run mean of per-run Avg (ms):\n")
    print("| Program | Mean Avg (ms) | Std of Avgs | Runs |")
    print("|---|---:|---:|---:|")
    for p in progs:
        vals = per_prog_avgs[p]
        mean = statistics.mean(vals)
        sd = statistics.pstdev(vals) if len(vals) > 1 else 0.0
        print(f"| {p} | {mean:.3f} | {sd:.3f} | {len(vals)} |")

    # Optional CSV with per-run Avgs
    if args.out:
        all_progs = sorted({p for r in per_run_rows for p in r.keys()})
        with open(args.out, "w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["run"] + all_progs)
            for i, r in enumerate(per_run_rows, 1):
                w.writerow([i] + [r.get(p, {}).get("avg", "") for p in all_progs])
        print(f"\nWrote CSV: {args.out}")

if __name__ == "__main__":
    main()
