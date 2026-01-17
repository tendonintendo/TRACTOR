# Deterministic Isomorphic Validation of Composite Card Structures

This repository contains the full, high-performance C++ implementation and formal mathematical framework for validating complex structural plays in multi-deck trick-taking games.

## Publication Status
**Current Status:** Under Review (Submitted January 2026)  
**Venue:** IAENG International Journal of Computer Science  
**Title:** Deterministic Isomorphic Validation of Composite Card Structures in Non-Monotonic Rank Hierarchies  
**Authors:** Muhammad Rengga Putra Kuncoro, Rully Soelaiman

## The Solution
The core of this repository, `src/solution.cpp`, is the **complete and verified solution** to the [**SPOJ TRACTOR**](https://www.spoj.com/problems/TRACTOR/) competitive programming problem. 

Standard greedy matching often fails in these scenarios due to structural ambiguity—for example, deciding whether a set of cards should be treated as multiple consecutive Tractors or a combination of Pairs and Singles to satisfy a "follow" requirement. This engine resolves such ambiguities by treating "Throws" (composite plays) as a problem of **isomorphic structural consistency** under non-monotonic rank constraints.

**Validation Details:**
* **Verified:** Successfully passes all test cases on SPOJ with 100% accuracy.
* **Peak Performance:** Achieved a **0.00s execution time** on SPOJ, placing the implementation among the top-tier most efficient solutions.
* **Profile:** [rengga_f_101 on SPOJ](https://www.spoj.com/users/rengga_f_101/)
* **Constraint Optimization:** Hand-tuned for $O(1)$ rank adjacency verification and highly-optimized backtracking pruning. The engine utilizes symmetry breaking to satisfy strict competitive programming limits while maintaining theoretical completeness.

## Theoretical Overview
The implementation addresses two primary computational hurdles:

1. **Non-Monotonic Rank Mapping**: Uses a transformation function $\Phi(c)$ to normalize rank discontinuities (Trump and Current Rank systems) into a continuous integer domain, enabling $O(1)$ sequence validation.
2. **Combinatorial Search**: Explores the Symmetric Group $S_n$ using a recursive backtracking engine with symmetry breaking to find a valid structural mapping between lead and follow plays.

## Contents
* **`src/solution.cpp`**: The complete production-ready engine.
* **`src/eval.ipynb`**: Performance analysis and Monte Carlo simulations demonstrating the algorithm's reliability across stochastic distributions.

## Compilation
The implementation utilizes `#define int long long` to ensure robust handling of rank-normalized integer domains.
```bash
g++ -O3 src/solution.cpp -o tractor_solver
./tractor_solver < input.txt
```
