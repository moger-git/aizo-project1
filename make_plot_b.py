import csv
import os
import sys
from pathlib import Path

import matplotlib.pyplot as plt


# Kolejność wyświetlania na wykresach
DISTRIBUTION_ORDER = ["random", "ascending", "ascending50Per", "descending"]
STRUCTURE_ORDER = ["array", "singleList", "doubleList", "queue", "stack", "binaryTree"]

# Metryka używana do głównego porównania w badaniu B
DEFAULT_METRIC = "averageSortDuration"


def ensure_output_dir(path: str) -> None:
    Path(path).mkdir(parents=True, exist_ok=True)


def to_float(value):
    try:
        return float(value)
    except (TypeError, ValueError):
        return None


def load_csv(csv_path: str):
    rows = []

    with open(csv_path, "r", encoding="utf-8", newline="") as f:
        reader = csv.DictReader(f, delimiter=";")

        for row in reader:
            parsed = dict(row)

            for key in [
                "arrayLength",
                "tries",
                "minSortDuration",
                "maxSortDuration",
                "averageSortDuration",
                "meanSortDuration",
            ]:
                if key in parsed:
                    parsed[key] = to_float(parsed[key])

            rows.append(parsed)

    return rows


def ordered_present_values(preferred_order, values):
    present = set(values)
    ordered = [v for v in preferred_order if v in present]
    extras = sorted(v for v in present if v not in preferred_order)
    return ordered + extras


def aggregate_metric(rows, metric_name):
    """
    Zwraca słownik:
    {
        "array": {
            "random": 123.0,
            "ascending": 110.0,
            ...
        },
        "singleList": {
            ...
        }
    }

    Jeżeli w CSV jest kilka wierszy dla tej samej pary
    (structure, distribution), liczona jest średnia.
    """
    grouped = {}

    for row in rows:
        structure = row.get("variableType")
        distribution = row.get("dataDistribution")
        metric_value = row.get(metric_name)

        if not structure or not distribution:
            continue
        if metric_value is None:
            continue

        grouped.setdefault(structure, {}).setdefault(distribution, []).append(metric_value)

    result = {}
    for structure, distributions in grouped.items():
        result[structure] = {}
        for distribution, values in distributions.items():
            result[structure][distribution] = sum(values) / len(values)

    return result


def save_chart_structures_with_distributions(data, metric_name, output_path):
    """
    Główny wykres:
    - oś X: struktury
    - w każdej grupie słupki dla różnych dataDistribution
    """
    if not data:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, data.keys())

    all_distributions = set()
    for structure in data:
        all_distributions.update(data[structure].keys())

    distributions = ordered_present_values(DISTRIBUTION_ORDER, all_distributions)

    if not structures or not distributions:
        return

    x = list(range(len(structures)))
    series_count = len(distributions)
    width = 0.8 / series_count if series_count > 0 else 0.8

    plt.figure(figsize=(12, 7))

    for i, distribution in enumerate(distributions):
        offset = (i - (series_count - 1) / 2.0) * width
        x_positions = [xi + offset for xi in x]

        values = []
        for structure in structures:
            values.append(data.get(structure, {}).get(distribution, 0.0))

        plt.bar(x_positions, values, width=width, label=distribution)

    plt.title("Badanie B - porównanie struktur według dataDistribution")
    plt.xlabel("Struktura")
    plt.ylabel(f"{metric_name} [us]")
    plt.xticks(x, structures, rotation=15)
    plt.legend(title="dataDistribution")
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()


def save_chart_distributions_with_structures(data, metric_name, output_path):
    """
    Drugi wykres:
    - oś X: dataDistribution
    - w każdej grupie słupki dla różnych struktur
    """
    if not data:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, data.keys())

    all_distributions = set()
    for structure in data:
        all_distributions.update(data[structure].keys())

    distributions = ordered_present_values(DISTRIBUTION_ORDER, all_distributions)

    if not structures or not distributions:
        return

    x = list(range(len(distributions)))
    series_count = len(structures)
    width = 0.8 / series_count if series_count > 0 else 0.8

    plt.figure(figsize=(12, 7))

    for i, structure in enumerate(structures):
        offset = (i - (series_count - 1) / 2.0) * width
        x_positions = [xi + offset for xi in x]

        values = []
        for distribution in distributions:
            values.append(data.get(structure, {}).get(distribution, 0.0))

        plt.bar(x_positions, values, width=width, label=structure)

    plt.title("Badanie B - porównanie dataDistribution według struktur")
    plt.xlabel("dataDistribution")
    plt.ylabel(f"{metric_name} [us]")
    plt.xticks(x, distributions, rotation=15)
    plt.legend(title="Struktura")
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()


def save_one_chart_per_distribution(data, metric_name, output_dir):
    """
    Osobny wykres dla każdego rozkładu:
    random, ascending, ascending50Per, descending
    Dzięki temu łatwo odróżnić wyniki dla konkretnego dataDistribution.
    """
    if not data:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, data.keys())

    all_distributions = set()
    for structure in data:
        all_distributions.update(data[structure].keys())

    distributions = ordered_present_values(DISTRIBUTION_ORDER, all_distributions)

    for distribution in distributions:
        labels = []
        values = []

        for structure in structures:
            value = data.get(structure, {}).get(distribution)
            if value is None:
                continue
            labels.append(structure)
            values.append(value)

        if not labels:
            continue

        plt.figure(figsize=(10, 6))
        plt.bar(labels, values)
        plt.title(f"Badanie B - {distribution}")
        plt.xlabel("Struktura")
        plt.ylabel(f"{metric_name} [us]")
        plt.xticks(rotation=15)
        plt.tight_layout()
        plt.savefig(os.path.join(output_dir, f"B_{distribution}.png"))
        plt.close()


def save_one_chart_per_structure(data, metric_name, output_dir):
    """
    Osobny wykres dla każdej struktury:
    pozwala zobaczyć, jak zmienia się czas zależnie od dataDistribution.
    """
    if not data:
        return

    structures = ordered_present_values(STRUCTURE_ORDER, data.keys())

    for structure in structures:
        dist_map = data.get(structure, {})
        if not dist_map:
            continue

        distributions = ordered_present_values(DISTRIBUTION_ORDER, dist_map.keys())
        values = [dist_map[d] for d in distributions]

        plt.figure(figsize=(10, 6))
        plt.bar(distributions, values)
        plt.title(f"Badanie B - {structure}")
        plt.xlabel("dataDistribution")
        plt.ylabel(f"{metric_name} [us]")
        plt.xticks(rotation=15)
        plt.tight_layout()
        plt.savefig(os.path.join(output_dir, f"B_{structure}.png"))
        plt.close()


def main():
    if len(sys.argv) >= 2:
        csv_path = sys.argv[1]
    else:
        csv_path = "results/benchmark_B.csv"

    metric_name = DEFAULT_METRIC
    if len(sys.argv) >= 3:
        metric_name = sys.argv[2]

    if not os.path.exists(csv_path):
        print(f"Plik nie istnieje: {csv_path}")
        return 1

    rows = load_csv(csv_path)

    if not rows:
        print("CSV jest pusty.")
        return 1

    required_cols = {"variableType", "dataDistribution", metric_name}
    if not required_cols.issubset(set(rows[0].keys())):
        print("Brak wymaganych kolumn w CSV.")
        print("Potrzebne kolumny:", ", ".join(sorted(required_cols)))
        print("Uwaga: poprawna nazwa kolumny to dataDistribution.")
        return 1

    csv_name = Path(csv_path).stem
    output_dir = os.path.join("plots", csv_name)
    ensure_output_dir(output_dir)

    data = aggregate_metric(rows, metric_name)

    save_chart_structures_with_distributions(
        data,
        metric_name,
        os.path.join(output_dir, "B_struktury_vs_dataDistribution.png"),
    )

    save_chart_distributions_with_structures(
        data,
        metric_name,
        os.path.join(output_dir, "B_dataDistribution_vs_struktury.png"),
    )

    save_one_chart_per_distribution(data, metric_name, output_dir)
    save_one_chart_per_structure(data, metric_name, output_dir)

    print(f"Wczytano plik: {csv_path}")
    print(f"Użyta metryka: {metric_name}")
    print(f"Liczba wierszy: {len(rows)}")
    print(f"Wykresy zapisano do: {output_dir}")
    print("Gotowe.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())