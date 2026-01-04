# Deterministic Isomorphic Validation of Composite Card Structures

This repository contains the formal mathematical framework and high-performance C++ implementation for validating complex structural plays in multi-deck trick-taking games. 

## Inspiration
The core of this research was inspired by the [**SPOJ TRACTOR**](https://www.spoj.com/problems/TRACTOR/) competitive programming problem. The challenge involves resolving structural ambiguity in card plays where simple greedy matching often fails. This project formalizes a robust solution by treating the validation of "Throws" (composite plays) as a problem of isomorphic structural consistency under non-monotonic rank constraints. The provided source code serves as a verified solution to the SPOJ challenge.

## Contents
* **src/solution.cpp**: A high-performance C++ implementation that serves as a direct solution to the SPOJ TRACTOR problem. It utilizes the greedy decomposition and permutative matching algorithms described in the paper.
* **latex/main.tex**: The source LaTeX for the academic paper titled *"Deterministic Isomorphic Validation of Composite Card Structures in Non-Monotonic Rank Hierarchies"* which takes inspiration from the implementation of the C++ implementation.
* **latex/main.pdf**: The academic paper titled *"Deterministic Isomorphic Validation of Composite Card Structures in Non-Monotonic Rank Hierarchies"*.

## Theoretical Overview
The project addresses two primary computational hurdles:

1.  **Non-Monotonic Rank Mapping**: The "Current Rank" and Trump system create discontinuities in standard card sequences. We implement a transformation function $\Phi(c)$ that projects these cards into a normalized integer domain, enabling $O(1)$ adjacency verification.
2.  **Structural Ambiguity**: A single hand of cards can often be partitioned into multiple valid combinations of Tractors, Pairs, and Singles. To ensure 100% accuracy, the system explores the Symmetric Group $S_n$ to find a valid structural mapping between the lead and follow plays.



## Algorithmic Implementation
The provided source code implements the following logic:
* **Maximal Greedy Extraction**: Pre-processing the hand to identify the densest possible structural blocks, effectively reducing the cardinality of the search space.
* **Permutative Isomorphism Matching**: A recursive backtracking search that utilizes symmetry breaking and pruning to maintain real-time performance.
* **Resource Fragmentation**: A Best-Fit heuristic for degrading high-order structures (Tractors) to satisfy lower-order requirements (Pairs/Singles).



## Requirements
* **C++ Compiler**: A compiler supporting C++11 or higher (e.g., `g++` or `clang++`).
* **LaTeX Distribution**: `TeX Live` or `MiKTeX` with the `algorithm2e` and `amsmath` packages for compiling the paper documentation.

## Compilation and Usage
To compile the SPOJ solution:
```bash
g++ src/solution.cpp -o tractor_solver
./tractor_solver < input.txt
```

To compile the academic paper:
```bash
pdflatex latex/main.tex
```
