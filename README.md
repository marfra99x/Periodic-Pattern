# Pure Consecutive Maximal Patterns with (k, Δ)-Errors — Reference Implementation

This repository contains a small C++ reference implementation of the online algorithm used in the paper to detect **pure consecutive maximal periodic patterns** under the **(k, Δ)-error** model.

The program:
- reads a strictly increasing integer sequence `L` from a text file,
- runs the detection algorithm,
- prints the running time and the number of detected patterns.

## Definitions (as used in the code)

A candidate window corresponds to a **consecutive** subsequence `L[s..e]` whose gaps are compatible with a reference period `dP`:
- a gap `g` is **compatible** if `|g - dP| ≤ Δ`,
- at most `k` compatible gaps may be **deviations**, i.e., `g != dP`.

### Purity check (paper definition)
A pattern `P = {L_i, ..., L_j}` with reference period `dP` is **pure** iff it cannot be extended (even non-consecutively) on either side:
- `L_i - dP ∉ L` and `L_j + dP ∉ L`.

In the code this is implemented with two `std::binary_search` calls on the sorted array `L` (time `O(log |L|)` per membership test).

## Repository inputs

The repo includes three input files (one integer per line, strictly increasing):
- `random.txt` — random gaps (no strong periodic structure)
- `periodic_sparse.txt` — periodic structure with sparse deviations (typical “near-linear” regime)
- `adversarial_k1.txt` — adversarial-ish construction designed to trigger frequent restarts (stress test)
