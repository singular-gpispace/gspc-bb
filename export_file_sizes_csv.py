#!/usr/bin/env python3

import argparse
import csv
import re
from itertools import zip_longest
from pathlib import Path


def collect_sizes(directory: Path, prefix: str) -> list[int]:
    pattern = re.compile(rf"^{re.escape(prefix)}(\d+)$")
    indexed_sizes: list[tuple[int, int]] = []

    if not directory.is_dir():
        return []

    for entry in directory.iterdir():
        if not entry.is_file():
            continue

        match = pattern.match(entry.name)
        if match is None:
            continue

        indexed_sizes.append((int(match.group(1)), entry.stat().st_size))

    indexed_sizes.sort(key=lambda item: item[0])
    return [size for _, size in indexed_sizes]


def write_csv(base_dir: Path, output_path: Path) -> None:
    intermediate_dir = base_dir / "intermediate_files"
    result_dir = base_dir / "result"

    columns = {
        "f": collect_sizes(intermediate_dir, "f"),
        "s": collect_sizes(result_dir, "s"),
        "g": collect_sizes(result_dir, "g"),
        "l": collect_sizes(result_dir, "l"),
    }

    output_path.parent.mkdir(parents=True, exist_ok=True)
    with output_path.open("w", newline="", encoding="ascii") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow(["f", "s", "g", "l"])
        writer.writerows(
            zip_longest(
                columns["f"],
                columns["s"],
                columns["g"],
                columns["l"],
                fillvalue="",
            )
        )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Write a CSV with the byte sizes of f-files from intermediate_files "
            "and s/g/l-files from result."
        )
    )
    parser.add_argument(
        "base_dir",
        nargs="?",
        default=".",
        help="Directory containing intermediate_files/ and result/ (default: current directory).",
    )
    parser.add_argument(
        "-o",
        "--output",
        default="file_sizes.csv",
        help="Path to the output CSV file (default: file_sizes.csv).",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    base_dir = Path(args.base_dir).resolve()
    output_path = Path(args.output)
    if not output_path.is_absolute():
        output_path = (Path.cwd() / output_path).resolve()

    write_csv(base_dir, output_path)


if __name__ == "__main__":
    main()