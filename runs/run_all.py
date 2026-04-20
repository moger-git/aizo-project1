from bench_common import *
import time
import csv

RESULTS_FILE = "results/benchmark_all.csv"
TIME_RESULTS_FILE = "results/benchmark_all_time.csv"

# alpha
ALPHA_MEDIUM_SIZE = 10000
ALPHA_ITERATIONS = 100
ALPHA_DATA_TYPE = TYPE_INT

# A
A_SIZES = [1000, 5000, 10000, 20000]
A_ITERATIONS = 100
A_DATA_TYPE = TYPE_INT
A_CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
A_CHOSEN_SHELL_PARAMETER = SHELL_OPTION2

# B
B_SIZE = 10000
B_ITERATIONS = 100
B_DATA_TYPE = TYPE_INT
B_CHOSEN_ALGORITHM = ALG_QUICK
B_CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
B_CHOSEN_SHELL_PARAMETER = SHELL_OPTION2
B_DISTRIBUTIONS = [
    DIST_RANDOM,
    DIST_DESCENDING,
    DIST_ASCENDING,
    DIST_ASCENDING_50,
]

# C
C_SIZE = 10000
C_ITERATIONS = 100
C_CHOSEN_ALGORITHM = ALG_QUICK
C_CHOSEN_STRUCTURE = STRUCT_ARRAY
C_CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
C_CHOSEN_SHELL_PARAMETER = SHELL_OPTION2
C_DATA_TYPES = [TYPE_INT, TYPE_DOUBLE, TYPE_UINT, TYPE_STRING]

# omega
OMEGA_SIZE = 10000
OMEGA_ITERATIONS = 100
OMEGA_DATA_TYPE = TYPE_INT
OMEGA_CHOSEN_ALGORITHM = ALG_QUICK
OMEGA_CHOSEN_QUICK_PIVOT = PIVOT_MIDDLE
OMEGA_CHOSEN_SHELL_PARAMETER = SHELL_OPTION2


def seconds_to_text(seconds: float) -> str:
    return f"{seconds:.2f} s"


def prepare_time_csv(path: str) -> None:
    Path(path).parent.mkdir(parents=True, exist_ok=True)

    with open(path, "w", encoding="utf-8", newline="") as f:
        writer = csv.writer(f, delimiter=';')
        writer.writerow(["test", "arrayLength", "tries", "time"])


def append_time_row(path: str, test: str, array_length, tries, time: float) -> None:
    with open(path, "a", encoding="utf-8", newline="") as f:
        writer = csv.writer(f, delimiter=';')
        writer.writerow([test, array_length, tries, f"{time:.2f}"])


def run_log(name: str, func) -> tuple[bool, float]:
    print()
    print("=" * 60)
    print(f"Start badania: {name}")
    print("=" * 60)

    start = time.perf_counter()
    ok = func()
    end = time.perf_counter()

    duration = end - start

    print("-" * 60)
    print(f"Koniec badania: {name}")
    print(f"Wynik: {'Ok' if ok else 'Error'}")
    print(f"Czas trwania badania: {seconds_to_text(duration)}")
    print("-" * 60)

    return ok, duration


def run_alpha_all() -> bool:
    all_ok = True

    # alpha / quick / 3 pivots
    for structure in LINEAR_STRUCTURES:
        for pivot in [PIVOT_RANDOM, PIVOT_MIDDLE, PIVOT_LEFT]:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_QUICK,
                structure=structure,
                data_type=ALPHA_DATA_TYPE,
                length=ALPHA_MEDIUM_SIZE,
                iterations=ALPHA_ITERATIONS,
                pivot=pivot
            )
            if not run_command(cmd):
                all_ok = False

    # alpha / shell / 2 parameters
    for structure in LINEAR_STRUCTURES:
        for shell_param in [SHELL_OPTION1, SHELL_OPTION2]:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_SHELL,
                structure=structure,
                data_type=ALPHA_DATA_TYPE,
                length=ALPHA_MEDIUM_SIZE,
                iterations=ALPHA_ITERATIONS,
                shell_param=shell_param
            )
            if not run_command(cmd):
                all_ok = False

    return all_ok


def run_A_all() -> bool:
    all_ok = True

    for size in A_SIZES:
        # A
        for structure in LINEAR_STRUCTURES:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_BUCKET,
                structure=structure,
                data_type=A_DATA_TYPE,
                length=size,
                iterations=A_ITERATIONS
            )
            if not run_command(cmd):
                all_ok = False

        for structure in LINEAR_STRUCTURES:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_QUICK,
                structure=structure,
                data_type=A_DATA_TYPE,
                length=size,
                iterations=A_ITERATIONS,
                pivot=A_CHOSEN_QUICK_PIVOT
            )
            if not run_command(cmd):
                all_ok = False

        for structure in LINEAR_STRUCTURES:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=ALG_SHELL,
                structure=structure,
                data_type=A_DATA_TYPE,
                length=size,
                iterations=A_ITERATIONS,
                shell_param=A_CHOSEN_SHELL_PARAMETER
            )
            if not run_command(cmd):
                all_ok = False

    return all_ok


def run_B_all() -> bool:
    all_ok = True

    # B
    for structure in LINEAR_STRUCTURES:
        for distribution in B_DISTRIBUTIONS:
            cmd = benchmark_cmd(
                results_file=RESULTS_FILE,
                algorithm=B_CHOSEN_ALGORITHM,
                structure=structure,
                data_type=B_DATA_TYPE,
                length=B_SIZE,
                iterations=B_ITERATIONS,
                pivot=B_CHOSEN_QUICK_PIVOT if B_CHOSEN_ALGORITHM == ALG_QUICK else None,
                shell_param=B_CHOSEN_SHELL_PARAMETER if B_CHOSEN_ALGORITHM == ALG_SHELL else None,
                distribution=distribution
            )

            if not run_command(cmd):
                all_ok = False

    return all_ok


def run_C_all() -> bool:
    all_ok = True

    # C
    for data_type in C_DATA_TYPES:
        cmd = benchmark_cmd(
            results_file=RESULTS_FILE,
            algorithm=C_CHOSEN_ALGORITHM,
            structure=C_CHOSEN_STRUCTURE,
            data_type=data_type,
            length=C_SIZE,
            iterations=C_ITERATIONS,
            pivot=C_CHOSEN_QUICK_PIVOT if C_CHOSEN_ALGORITHM == ALG_QUICK else None,
            shell_param=C_CHOSEN_SHELL_PARAMETER if C_CHOSEN_ALGORITHM == ALG_SHELL else None
        )

        if not run_command(cmd):
            all_ok = False

    return all_ok


def run_omega_all() -> bool:
    all_ok = True

    # omega
    for structure in OMEGA_STRUCTURES:
        cmd = benchmark_cmd(
            results_file=RESULTS_FILE,
            algorithm=OMEGA_CHOSEN_ALGORITHM,
            structure=structure,
            data_type=OMEGA_DATA_TYPE,
            length=OMEGA_SIZE,
            iterations=OMEGA_ITERATIONS,
            pivot=OMEGA_CHOSEN_QUICK_PIVOT if OMEGA_CHOSEN_ALGORITHM == ALG_QUICK else None,
            shell_param=OMEGA_CHOSEN_SHELL_PARAMETER if OMEGA_CHOSEN_ALGORITHM == ALG_SHELL else None
        )

        if not run_command(cmd):
            all_ok = False

    return all_ok


def main() -> int:
    prepare_csv(RESULTS_FILE)
    prepare_time_csv(TIME_RESULTS_FILE)

    total_start = time.perf_counter()

    results = {}

    results["alpha"] = run_log("alpha", run_alpha_all)
    append_time_row(
        TIME_RESULTS_FILE,
        "alpha",
        ALPHA_MEDIUM_SIZE,
        ALPHA_ITERATIONS,
        results["alpha"][1]
    )

    results["A"] = run_log("A", run_A_all)
    append_time_row(
        TIME_RESULTS_FILE,
        "A",
        ",".join(str(x) for x in A_SIZES),
        A_ITERATIONS,
        results["A"][1]
    )

    results["B"] = run_log("B", run_B_all)
    append_time_row(
        TIME_RESULTS_FILE,
        "B",
        B_SIZE,
        B_ITERATIONS,
        results["B"][1]
    )

    results["C"] = run_log("C", run_C_all)
    append_time_row(
        TIME_RESULTS_FILE,
        "C",
        C_SIZE,
        C_ITERATIONS,
        results["C"][1]
    )

    results["omega"] = run_log("omega", run_omega_all)
    append_time_row(
        TIME_RESULTS_FILE,
        "omega",
        OMEGA_SIZE,
        OMEGA_ITERATIONS,
        results["omega"][1]
    )

    total_end = time.perf_counter()
    total_duration = total_end - total_start

    append_time_row(
        TIME_RESULTS_FILE,
        "ALL",
        "-",
        "-",
        total_duration
    )

    print()
    print("=" * 60)
    print("Podsumowanie wszystkich badan")
    print("=" * 60)

    all_ok = True
    for name, (ok, duration) in results.items():
        print(f"{name:<10} | {'Ok' if ok else 'Error':<5} | czas: {seconds_to_text(duration)}")
        if not ok:
            all_ok = False

    print("-" * 60)
    print(f"Laczny czas wszystkich badan: {seconds_to_text(total_duration)}")
    print(f"Wyniki CSV: {RESULTS_FILE}")
    print(f"Wyniki czasu CSV: {TIME_RESULTS_FILE}")
    print("=" * 60)

    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())