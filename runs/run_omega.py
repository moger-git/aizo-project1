from bench_common import *

RESULTS_FILE = "results/benchmark_omega.csv"
SIZE = 10000
ITERATIONS = 100
DATA_TYPE = TYPE_INT

CHOSEN_ALGORITHM = ALG_QUICK
CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
CHOSEN_SHELL_PARAMETER = SHELL_OPTION2

def main() -> int:
    prepare_csv(RESULTS_FILE)
    all_ok = True

    for structure in OMEGA_STRUCTURES:
        cmd = benchmark_cmd(
            results_file=RESULTS_FILE,
            algorithm=CHOSEN_ALGORITHM,
            structure=structure,
            data_type=DATA_TYPE,
            length=SIZE,
            iterations=ITERATIONS,
            pivot=CHOSEN_QUICK_PIVOT if CHOSEN_ALGORITHM == ALG_QUICK else None,
            shell_param=CHOSEN_SHELL_PARAMETER if CHOSEN_ALGORITHM == ALG_SHELL else None
        )

        if not run_command(cmd):
            all_ok = False

    print(f"Results: {RESULTS_FILE}")
    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())