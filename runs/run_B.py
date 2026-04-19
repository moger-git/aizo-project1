from bench_common import *

RESULTS_FILE = "results/benchmark_B.csv"
ITERATIONS = 50
DATA_TYPE = TYPE_INT
SIZE = 10000

CHOSEN_ALGORITHM = ALG_QUICK
CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
CHOSEN_SHELL_PARAMETER = SHELL_OPTION2

DISTRIBUTIONS = [
    DIST_RANDOM,
    DIST_DESCENDING,
    DIST_ASCENDING,
    DIST_ASCENDING_50,
]

def main() -> int:
    prepare_csv(RESULTS_FILE)
    all_ok = True

    print("=== B / distributions ===")

    for structure in LINEAR_STRUCTURES:
        for distribution in DISTRIBUTIONS:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=CHOSEN_ALGORITHM,
                structure=structure,
                data_type=DATA_TYPE,
                length=SIZE,
                iterations=ITERATIONS,
                pivot=CHOSEN_QUICK_PIVOT if CHOSEN_ALGORITHM == ALG_QUICK else None,
                shell_param=CHOSEN_SHELL_PARAMETER if CHOSEN_ALGORITHM == ALG_SHELL else None,
                distribution=distribution
            )

            if not run_command(cmd):
                all_ok = False

    print(f"Results: {RESULTS_FILE}")
    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())