from bench_common import *

RESULTS_FILE = "results/benchmark_alpha.csv"
MEDIUM_SIZE = 10000
ITERATIONS = 100
DATA_TYPE = TYPE_INT

def main() -> int:
    prepare_csv(RESULTS_FILE)
    all_ok = True

    print("===== alpha / quick / 3 pivots =====")
    for structure in LINEAR_STRUCTURES:
        for pivot in [PIVOT_RANDOM, PIVOT_MIDDLE, PIVOT_LEFT]:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_QUICK,
                structure=structure,
                data_type=DATA_TYPE,
                length=MEDIUM_SIZE,
                iterations=ITERATIONS,
                pivot=pivot
            )
            if not run_command(cmd):
                all_ok = False

    print("===== alpha / shell / 2 parameters =====")
    for structure in LINEAR_STRUCTURES:
        for shell_param in [SHELL_OPTION1, SHELL_OPTION2]:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_SHELL,
                structure=structure,
                data_type=DATA_TYPE,
                length=MEDIUM_SIZE,
                iterations=ITERATIONS,
                shell_param=shell_param
            )
            if not run_command(cmd):
                all_ok = False

    print(f"Results: {RESULTS_FILE}")
    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())