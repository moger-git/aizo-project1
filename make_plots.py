import csv
import os
import sys
from pathlib import Path

import matplotlib.pyplot as plt


NUMERIC_COLUMNS = {
    "arrayLength",
    "tries",
    "minSortDuration",
    "maxSortDuration",
    "averageSortDuration",
    "meanSortDuration",
    "time",
}

DISTRIBUTION_ORDER = ["random", "ascending", "ascending50Per", "descending"]
STRUCTURE_ORDER = ["array", "singleList", "doubleList", "queue", "stack", "binaryTree"]
DATA_TYPE_ORDER = ["int", "double", "unsigned int", "string"]
ALGORITHM_ORDER = ["bucket", "quick", "shell"]


def ensure_dir(path: str) -> None:
    Path(path).mkdir(parents=True, exist_ok=True)


def to_float(value):
    try:
        return float(value)
    except (TypeError, ValueError):
        return None


def to_int(value):
    try:
        return int(value)
    except (TypeError, ValueError):
        return None


def load_csv(csv_path: str, delimiter: str = ";"):
    rows = []

    with open(csv_path, "r", encoding="utf-8", newline="") as f:
        reader = csv.DictReader(f, delimiter=delimiter)

        for row in reader:
            parsed = dict(row)
            for key in list(parsed.keys()):
                if key in NUMERIC_COLUMNS:
                    if key in {"arrayLength", "tries"}:
                        parsed[key] = to_int(parsed[key])
                    else:
                        parsed[key] = to_float(parsed[key])
            rows.append(parsed)

    return rows


def ordered_present_values(preferred_order, values):
    present = list(values)
    present_set = set(present)
    ordered = [v for v in preferred_order if v in present_set]
    extras = sorted(v for v in present_set if v not in preferred_order)
    return ordered + extras


def sanitize_filename(text: str) -> str:
    return (
        text.replace(" ", "_")
        .replace("/", "_")
        .replace("=", "_")
        .replace("|", "_")
    )


def bar_chart(labels, values, title, xlabel, ylabel, output_path, rotation=20):
    if not labels or not values:
        return

    plt.figure(figsize=(12, 7))
    plt.bar(labels, values)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.xticks(rotation=rotation, ha="right")
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()


def grouped_bar_chart(categories, series_dict, title, xlabel, ylabel, output_path,
                      category_rotation=15, legend_title=None):
    if not categories or not series_dict:
        return

    x = list(range(len(categories)))
    series_names = list(series_dict.keys())
    series_count = len(series_names)
    width = 0.8 / series_count if series_count > 0 else 0.8

    plt.figure(figsize=(12, 7))

    for i, series_name in enumerate(series_names):
        offset = (i - (series_count - 1) / 2.0) * width
        positions = [xi + offset for xi in x]
        plt.bar(positions, series_dict[series_name], width=width, label=series_name)

    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.xticks(x, categories, rotation=category_rotation)
    if legend_title is not None:
        plt.legend(title=legend_title)
    else:
        plt.legend()
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()


def average(values):
    if not values:
        return None
    return sum(values) / len(values)


def group_average(rows, key_columns, value_column="averageSortDuration"):
    grouped = {}

    for row in rows:
        value = row.get(value_column)
        if value is None:
            continue

        key = tuple(row.get(col) for col in key_columns)
        if any(part is None or part == "" for part in key):
            continue

        grouped.setdefault(key, []).append(value)

    result = {}
    for key, values in grouped.items():
        result[key] = average(values)

    return result


def create_alpha_plots(rows, output_dir):
    if not rows:
        return

    # Overall: every configuration as separate bar
    labels = []
    values = []

    grouped = group_average(rows, ["sortMethod", "sortParameter", "variableType"])
    for (sort_method, sort_parameter, structure), avg_value in grouped.items():
        labels.append(f"{sort_method} | {sort_parameter} | {structure}")
        values.append(avg_value)

    bar_chart(
        labels,
        values,
        "Alpha: porównanie wszystkich konfiguracji",
        "Konfiguracja",
        "Średni czas sortowania [us]",
        os.path.join(output_dir, "alpha_all_configurations.png"),
        rotation=45,
    )

    # Per structure: algorithm/parameter comparison
    structures = ordered_present_values(STRUCTURE_ORDER, [r.get("variableType") for r in rows])
    for structure in structures:
        filtered = [r for r in rows if r.get("variableType") == structure]
        structure_grouped = group_average(filtered, ["sortMethod", "sortParameter"])

        local_labels = []
        local_values = []
        for (sort_method, sort_parameter), avg_value in structure_grouped.items():
            local_labels.append(f"{sort_method} | {sort_parameter}")
            local_values.append(avg_value)

        bar_chart(
            local_labels,
            local_values,
            f"Alpha: {structure}",
            "Algorytm i parametr",
            "Średni czas sortowania [us]",
            os.path.join(output_dir, f"alpha_{sanitize_filename(structure)}.png"),
            rotation=35,
        )


def create_A_plots(rows, output_dir):
    if not rows:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, [r.get("variableType") for r in rows])

    for structure in structures:
        filtered = [r for r in rows if r.get("variableType") == structure]
        if not filtered:
            continue

        sizes = ordered_present_values([], [r.get("arrayLength") for r in filtered if r.get("arrayLength") is not None])
        sizes = sorted(sizes)

        algorithms = ordered_present_values(ALGORITHM_ORDER, [r.get("sortMethod") for r in filtered])

        series = {}
        for algorithm in algorithms:
            values = []
            for size in sizes:
                matches = [
                    r.get("averageSortDuration")
                    for r in filtered
                    if r.get("sortMethod") == algorithm and r.get("arrayLength") == size
                       and r.get("averageSortDuration") is not None
                ]
                values.append(average(matches) if matches else 0.0)

            series[algorithm] = values

        grouped_bar_chart(
            [str(size) for size in sizes],
            series,
            f"A: {structure}",
            "Rozmiar danych",
            "Średni czas sortowania [us]",
            os.path.join(output_dir, f"A_{sanitize_filename(structure)}.png"),
            legend_title="Algorytm",
        )


def create_B_plots(rows, output_dir):
    if not rows:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, [r.get("variableType") for r in rows])
    distributions = ordered_present_values(DISTRIBUTION_ORDER, [r.get("dataDistribution") for r in rows])

    # structures on X, distributions in legend
    series_structures = {}
    for distribution in distributions:
        values = []
        for structure in structures:
            matches = [
                r.get("averageSortDuration")
                for r in rows
                if r.get("variableType") == structure
                   and r.get("dataDistribution") == distribution
                   and r.get("averageSortDuration") is not None
            ]
            values.append(average(matches) if matches else 0.0)
        series_structures[distribution] = values

    grouped_bar_chart(
        structures,
        series_structures,
        "B: porównanie struktur według dataDistribution",
        "Struktura",
        "Średni czas sortowania [us]",
        os.path.join(output_dir, "B_structures_vs_distribution.png"),
        legend_title="dataDistribution",
    )

    # distributions on X, structures in legend
    series_distributions = {}
    for structure in structures:
        values = []
        for distribution in distributions:
            matches = [
                r.get("averageSortDuration")
                for r in rows
                if r.get("variableType") == structure
                   and r.get("dataDistribution") == distribution
                   and r.get("averageSortDuration") is not None
            ]
            values.append(average(matches) if matches else 0.0)
        series_distributions[structure] = values

    grouped_bar_chart(
        distributions,
        series_distributions,
        "B: porównanie dataDistribution według struktur",
        "dataDistribution",
        "Średni czas sortowania [us]",
        os.path.join(output_dir, "B_distribution_vs_structures.png"),
        legend_title="Struktura",
    )

    # one chart per structure
    for structure in structures:
        filtered = [r for r in rows if r.get("variableType") == structure]
        labels = []
        values = []

        for distribution in distributions:
            matches = [
                r.get("averageSortDuration")
                for r in filtered
                if r.get("dataDistribution") == distribution
                   and r.get("averageSortDuration") is not None
            ]
            if matches:
                labels.append(distribution)
                values.append(average(matches))

        bar_chart(
            labels,
            values,
            f"B: {structure}",
            "dataDistribution",
            "Średni czas sortowania [us]",
            os.path.join(output_dir, f"B_{sanitize_filename(structure)}.png"),
        )


def create_C_plots(rows, output_dir):
    if not rows:
        return

    data_types = ordered_present_values(DATA_TYPE_ORDER, [r.get("dataType") for r in rows])

    labels = []
    values = []

    for data_type in data_types:
        matches = [
            r.get("averageSortDuration")
            for r in rows
            if r.get("dataType") == data_type and r.get("averageSortDuration") is not None
        ]
        if matches:
            labels.append(data_type)
            values.append(average(matches))

    bar_chart(
        labels,
        values,
        "C: wpływ typu danych",
        "Typ danych",
        "Średni czas sortowania [us]",
        os.path.join(output_dir, "C_datatype.png"),
        rotation=30,
    )


def create_omega_plots(rows, output_dir):
    if not rows:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, [r.get("variableType") for r in rows])

    labels = []
    values = []

    for structure in structures:
        matches = [
            r.get("averageSortDuration")
            for r in rows
            if r.get("variableType") == structure and r.get("averageSortDuration") is not None
        ]
        if matches:
            labels.append(structure)
            values.append(average(matches))

    bar_chart(
        labels,
        values,
        "Omega: porównanie struktur",
        "Struktura",
        "Średni czas sortowania [us]",
        os.path.join(output_dir, "omega_structures.png"),
    )


def create_all_time_plots(rows, output_dir):
    if not rows:
        return

    labels = []
    values = []

    for row in rows:
        study = row.get("test")
        timing = row.get("time")

        if not study or timing is None:
            continue

        if study == "ALL":
            continue

        labels.append(study)
        values.append(timing)

    bar_chart(
        labels,
        values,
        "Czas każdego badania",
        "Badanie",
        "Czas [s]",
        os.path.join(output_dir, "all_time_per_study.png"),
    )

    total = None
    for row in rows:
        if row.get("test") == "ALL" and row.get("time") is not None:
            total = row.get("time")
            break

    if total is not None:
        bar_chart(
            ["ALL"],
            [total],
            "Łączny czas wszystkich badań",
            "Wszystkie badania",
            "Czas [s]",
            os.path.join(output_dir, "all_time_total.png"),
            rotation=0,
        )


def maybe_process(csv_path, output_dir, handler, delimiter=";"):
    if not os.path.exists(csv_path):
        print(f"Pominięto, brak pliku: {csv_path}")
        return

    rows = load_csv(csv_path, delimiter=delimiter)
    if not rows:
        print(f"Pominięto pusty plik: {csv_path}")
        return

    ensure_dir(output_dir)
    handler(rows, output_dir)
    print(f"Przetworzono: {csv_path}")


def main():
    if len(sys.argv) >= 2:
        results_dir = sys.argv[1]
    else:
        results_dir = "results"

    if not os.path.isdir(results_dir):
        print(f"Katalog nie istnieje: {results_dir}")
        return 1

    plots_root = os.path.join("plots", "all")
    ensure_dir(plots_root)

    maybe_process(
        os.path.join(results_dir, "benchmark_alpha.csv"),
        os.path.join(plots_root, "alpha"),
        create_alpha_plots,
    )

    maybe_process(
        os.path.join(results_dir, "benchmark_A.csv"),
        os.path.join(plots_root, "A"),
        create_A_plots,
    )

    maybe_process(
        os.path.join(results_dir, "benchmark_B.csv"),
        os.path.join(plots_root, "B"),
        create_B_plots,
    )

    maybe_process(
        os.path.join(results_dir, "benchmark_C.csv"),
        os.path.join(plots_root, "C"),
        create_C_plots,
    )

    maybe_process(
        os.path.join(results_dir, "benchmark_omega.csv"),
        os.path.join(plots_root, "omega"),
        create_omega_plots,
    )

    maybe_process(
        os.path.join(results_dir, "benchmark_all_time.csv"),
        os.path.join(plots_root, "time"),
        create_all_time_plots,
        delimiter=",",
    )

    print(f"Gotowe. Wykresy zapisano w: {plots_root}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())