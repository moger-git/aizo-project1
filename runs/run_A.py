from bench_common import *

RESULTS_FILE = "results/benchmark_A.csv"
SIZES = [1000, 5000, 10000, 20000]
ITERATIONS = 50
DATA_TYPE = TYPE_INT

CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
CHOSEN_SHELL_PARAMETER = SHELL_OPTION2

def main() -> int:
    prepare_csv(RESULTS_FILE)
    all_ok = True

    for size in SIZES:
        print(f"===== A / size={size} =====")

        for structure in LINEAR_STRUCTURES:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_BUCKET,
                structure=structure,
                data_type=DATA_TYPE,
                length=size,
                iterations=ITERATIONS
            )
            if not run_command(cmd):
                all_ok = False

        for structure in LINEAR_STRUCTURES:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_QUICK,
                structure=structure,
                data_type=DATA_TYPE,
                length=size,
                iterations=ITERATIONS,
                pivot=CHOSEN_QUICK_PIVOT
            )
            if not run_command(cmd):
                all_ok = False

        for structure in LINEAR_STRUCTURES:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_SHELL,
                structure=structure,
                data_type=DATA_TYPE,
                length=size,
                iterations=ITERATIONS,
                shell_param=CHOSEN_SHELL_PARAMETER
            )
            if not run_command(cmd):
                all_ok = False

    print(f"Results: {RESULTS_FILE}")
    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())