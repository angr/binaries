#!/usr/bin/env python3
"""Build an ordinary ELF and exercise the real angr decompiler on it.

Reproducer for https://github.com/angr/angr/issues/6661 (RegionIdentifier loop
slicing ran a costly has_path() per (successor, latch) pair), contributed by
plowsec (https://github.com/plowsec). Originally named reproduce_binary.py.

The checked-in tests/aarch64/decompiler/pathological_loop binary was built from
pathological_loop.S with:

    aarch64-linux-gnu-gcc -nostdlib -no-pie -Wl,-e,region_slice_target \
        -Wl,--build-id=none -DSIDE_DIAMONDS=400 -DSPINES=150 -DLATCHES=150 \
        -o pathological_loop pathological_loop.S

(equivalent to: python pathological_loop.py --side-diamonds 400 --spines 150
--latches 150 --keep-binary --compiler aarch64-linux-gnu-gcc). These sizes keep
the fixed decompiler run at a few seconds while the pre-fix code takes minutes,
without hitting CPython's default recursion limit during structuring.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import subprocess
import sys
import time
from collections import deque
from pathlib import Path

import angr
import networkx
from angr.analyses.decompiler import region_identifier


HERE = Path(__file__).resolve().parent


def reverse_census_subgraph(graph, source, frontier, include_frontier=False):
    """Candidate replacement, inlined so this reproducer is self-contained."""
    frontier = set(frontier)
    if source not in graph or any(node not in graph for node in frontier):
        raise KeyError("Source node or frontier nodes are not in the source graph.")

    reaches_frontier = set(frontier)
    queue = deque(frontier)
    while queue:
        node = queue.popleft()
        if node == source:
            continue
        for predecessor in graph.predecessors(node):
            if predecessor not in reaches_frontier:
                reaches_frontier.add(predecessor)
                queue.append(predecessor)

    result = networkx.DiGraph()
    queue = deque([source])
    traversed = set()
    while queue:
        node = queue.popleft()
        traversed.add(node)
        for _, successor, data in graph.out_edges(node, data=True):
            if successor == source or result.has_edge(node, successor):
                continue
            result.add_edge(node, successor, **data)
            if successor not in traversed and successor not in frontier and successor in reaches_frontier:
                queue.append(successor)

    def removable(node):
        return (
            node not in frontier
            and node is not source
            and (result.out_degree[node] == 0 or result.in_degree[node] == 0)
        )

    queue = deque(node for node in result if removable(node))
    queued = set(queue)
    while queue:
        node = queue.popleft()
        queued.discard(node)
        if node not in result or not removable(node):
            continue
        neighbors = [*result.predecessors(node), *result.successors(node)]
        result.remove_node(node)
        for neighbor in neighbors:
            if neighbor in result and neighbor not in queued and removable(neighbor):
                queued.add(neighbor)
                queue.append(neighbor)

    if not include_frontier:
        result.remove_nodes_from(frontier)
    return result


def node_token(node):
    return (
        type(node).__name__,
        getattr(node, "addr", None),
        getattr(node, "idx", None),
    )


def graph_receipt(graph):
    return (
        tuple(node_token(node) for node in graph.nodes),
        tuple((node_token(source), node_token(destination)) for source, destination in graph.edges),
    )


def build(binary: Path, *, side_diamonds: int, spines: int, latches: int, compiler: str) -> None:
    subprocess.run(
        [
            compiler,
            "-nostdlib",
            "-no-pie",
            "-Wl,-e,region_slice_target",
            "-Wl,--build-id=none",
            f"-DSIDE_DIAMONDS={side_diamonds}",
            f"-DSPINES={spines}",
            f"-DLATCHES={latches}",
            "-o",
            str(binary),
            str(HERE / "pathological_loop.S"),
        ],
        check=True,
    )


def run_decompiler(binary: Path, name: str, implementation):
    project = angr.Project(binary, auto_load_libs=False)
    cfg_started = time.process_time()
    cfg = project.analyses.CFGFast(normalize=True, data_references=False)
    cfg_seconds = time.process_time() - cfg_started
    symbol = project.loader.main_object.get_symbol("region_slice_target")
    if symbol is None:
        raise RuntimeError("region_slice_target was not recovered from the ELF")
    function = cfg.functions[symbol.rebased_addr]

    stock_binding = region_identifier.subgraph_between_nodes
    stock_has_path = networkx.has_path
    has_path_calls = 0
    slice_has_path_calls = 0
    inside_slice = 0
    slice_seconds = 0.0
    slice_rows = []

    def counted_has_path(*args, **kwargs):
        nonlocal has_path_calls, slice_has_path_calls
        has_path_calls += 1
        if inside_slice:
            slice_has_path_calls += 1
        return stock_has_path(*args, **kwargs)

    def timed_slice(graph, source, frontier, include_frontier=False):
        nonlocal slice_seconds, inside_slice
        frontier = set(frontier)
        started = time.process_time()
        inside_slice += 1
        try:
            result = implementation(graph, source, frontier, include_frontier=include_frontier)
        finally:
            inside_slice -= 1
        elapsed = time.process_time() - started
        slice_seconds += elapsed
        slice_rows.append(
            {
                "input_nodes": len(graph),
                "input_edges": graph.number_of_edges(),
                "source": getattr(source, "addr", None),
                "frontier_count": len(frontier),
                "result_nodes": len(result),
                "result_edges": result.number_of_edges(),
                "seconds": elapsed,
                "receipt": graph_receipt(result),
            }
        )
        return result

    region_identifier.subgraph_between_nodes = timed_slice
    networkx.has_path = counted_has_path
    try:
        started = time.process_time()
        decompiler = project.analyses.Decompiler(function, cfg=cfg.model)
        decompiler_seconds = time.process_time() - started
    finally:
        region_identifier.subgraph_between_nodes = stock_binding
        networkx.has_path = stock_has_path

    code = decompiler.codegen.text if decompiler.codegen is not None else ""
    worst_negative_shape = max(
        slice_rows,
        key=lambda row: (row["input_nodes"] - row["result_nodes"], row["frontier_count"]),
    )
    return {
        "name": name,
        "cfg_nodes": len(function.graph),
        "cfg_edges": function.graph.number_of_edges(),
        "cfg_seconds": cfg_seconds,
        "decompiler_seconds": decompiler_seconds,
        "slice_seconds": slice_seconds,
        "slice_calls": len(slice_rows),
        "has_path_calls": slice_has_path_calls,
        "all_decompiler_has_path_calls": has_path_calls,
        "code": code,
        "code_sha256": hashlib.sha256(code.encode()).hexdigest(),
        "slice_receipts": [row["receipt"] for row in slice_rows],
        "worst_negative_shape": {key: value for key, value in worst_negative_shape.items() if key != "receipt"},
    }


def print_row(row):
    shape = row["worst_negative_shape"]
    print(
        f"{row['name']:15s} D={row['decompiler_seconds']:.3f}s "
        f"loop-slices={row['slice_seconds']:.3f}s has_path={row['has_path_calls']:,} "
        f"C={len(row['code']):,} bytes"
    )
    print(
        " " * 16
        + f"worst slice {shape['input_nodes']:,}/{shape['input_edges']:,} -> "
        + f"{shape['result_nodes']}/{shape['result_edges']} nodes/edges, "
        + f"{shape['frontier_count']} latches"
    )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--side-diamonds", type=int, default=100)
    parser.add_argument("--spines", type=int, default=450)
    parser.add_argument("--latches", type=int, default=450)
    parser.add_argument("--compiler", default=os.environ.get("CC", "gcc"))
    parser.add_argument("--binary", type=Path, default=HERE / "pathological_loop.elf")
    parser.add_argument("--keep-binary", action="store_true")
    args = parser.parse_args()
    if args.side_diamonds <= 0 or args.spines <= 0 or args.latches <= 0:
        parser.error("sizes must be positive")

    # Large real-ELF cases intentionally create hundreds of nested conditions.
    # Keep Python's unrelated default recursion ceiling from masking the graph
    # reachability timing that this reproducer is meant to isolate.
    sys.setrecursionlimit(max(10_000, 10 * (args.spines + args.latches)))

    build(
        args.binary,
        side_diamonds=args.side_diamonds,
        spines=args.spines,
        latches=args.latches,
        compiler=args.compiler,
    )
    try:
        stock = run_decompiler(args.binary, "stock", region_identifier.subgraph_between_nodes)
        candidate = run_decompiler(args.binary, "reverse-census", reverse_census_subgraph)
    finally:
        if not args.keep_binary:
            args.binary.unlink(missing_ok=True)

    print(f"real ELF CFG: {stock['cfg_nodes']:,} nodes / {stock['cfg_edges']:,} edges")
    print_row(stock)
    print_row(candidate)
    assert stock["slice_receipts"] == candidate["slice_receipts"], "loop-slice topology changed"
    assert stock["code"] == candidate["code"] and stock["code"], "generated C changed or is empty"
    assert stock["has_path_calls"] > 0 and candidate["has_path_calls"] == 0
    print(f"generated C and every ordered loop-slice topology are identical ({stock['code_sha256']})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
