from bench_common import *

RESULTS_FILE = "results/benchmark_C.csv"
SIZE = 10000
ITERATIONS = 100

CHOSEN_ALGORITHM = ALG_QUICK
CHOSEN_STRUCTURE = STRUCT_ARRAY

CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
CHOSEN_SHELL_PARAMETER = SHELL_OPTION2

DATA_TYPES = [TYPE_INT, TYPE_DOUBLE, TYPE_UINT, TYPE_STRING]

def main() -> int:
    prepare_csv(RESULTS_FILE)
    all_ok = True

    for data_type in DATA_TYPES:
        cmd = benchmark_cmd(
            results_file=RESULTS_FILE,
            algorithm=CHOSEN_ALGORITHM,
            structure=CHOSEN_STRUCTURE,
            data_type=data_type,
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