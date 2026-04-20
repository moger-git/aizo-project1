import csv
import os
import sys
from pathlib import Path
import matplotlib.pyplot as plt


def ensure_output_dir(path: str) -> None:
    Path(path).mkdir(parents=True, exist_ok=True)


def to_float(value: str):
    try:
        return float(value)
    except (ValueError, TypeError):
        return None


def load_csv(csv_path: str):
    rows = []

    with open(csv_path, "r", encoding="utf-8", newline="") as f:
        reader = csv.DictReader(f, delimiter=";")

        for row in reader:
            parsed = dict(row)

            if "czasSekundy" in parsed:
                parsed["czasSekundy"] = to_float(parsed["czasSekundy"])

            rows.append(parsed)

    return rows


def save_bar_chart(data_dict, title, xlabel, ylabel, output_path):
    if not data_dict:
        return

    labels = list(data_dict.keys())
    values = list(data_dict.values())

    plt.figure(figsize=(10, 6))
    plt.bar(labels, values)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.xticks(rotation=45, ha="right")
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()


def create_time_charts(rows, output_dir):
    if not rows:
        return

    first_row_keys = set(rows[0].keys())
    required_cols = {"badanie", "czasSekundy"}

    if not required_cols.issubset(first_row_keys):
        print("Brak wymaganych kolumn: badanie, czasSekundy")
        return

    # Czas kazdego badania
    per_study = {}
    for row in rows:
        badanie = row.get("badanie")
        czas = row.get("czasSekundy")

        if badanie in (None, "", "ALL"):
            continue
        if czas is None:
            continue

        per_study[badanie] = czas

    save_bar_chart(
        per_study,
        title="Czas każdego badania",
        xlabel="Badanie",
        ylabel="Czas [s]",
        output_path=os.path.join(output_dir, "czas_kazdego_badania.png")
    )

    # Laczny czas wszystkich badan
    total_time = None
    for row in rows:
        if row.get("badanie") == "ALL" and row.get("czasSekundy") is not None:
            total_time = row.get("czasSekundy")
            break

    if total_time is not None:
        save_bar_chart(
            {"ALL": total_time},
            title="Łączny czas wszystkich badań",
            xlabel="Wszystkie badania",
            ylabel="Czas [s]",
            output_path=os.path.join(output_dir, "laczny_czas_wszystkich_badan.png")
        )
    else:
        print("Nie znaleziono wiersza ALL w pliku CSV.")


def main():
    if len(sys.argv) < 2:
        print("Użycie:")
        print("  python3 make_plots.py results/benchmark_all_time.csv")
        return 1

    csv_path = sys.argv[1]

    if not os.path.exists(csv_path):
        print(f"Plik nie istnieje: {csv_path}")
        return 1

    csv_name = Path(csv_path).stem
    output_dir = f"plots/{csv_name}"
    ensure_output_dir(output_dir)

    rows = load_csv(csv_path)

    print(f"Wczytano plik: {csv_path}")
    print(f"Liczba wierszy: {len(rows)}")
    print(f"Wykresy zapiszą się do: {output_dir}")

    create_time_charts(rows, output_dir)

    print("Gotowe.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())