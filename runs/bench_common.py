import os
import subprocess
from pathlib import Path
from typing import List, Optional, Dict

BIN = "./cmake-build-debug/aizo-project1"
RESULTS_DIR = "results"

ALG_BUCKET = 4
ALG_QUICK = 5
ALG_SHELL = 6

STRUCT_ARRAY = 0
STRUCT_SINGLE = 1
STRUCT_DOUBLE = 2
STRUCT_QUEUE = 3
STRUCT_STACK = 4
STRUCT_TREE = 5

TYPE_INT = 0
TYPE_FLOAT = 1
TYPE_DOUBLE = 2
TYPE_CHAR = 3
TYPE_STRING = 4
TYPE_UINT = 5
TYPE_ULONG = 6
TYPE_UCHAR = 7

PIVOT_RANDOM = 0
PIVOT_LEFT = 1
PIVOT_RIGHT = 2
PIVOT_MIDDLE = 3

SHELL_OPTION1 = 0
SHELL_OPTION2 = 1
SHELL_OPTION3 = 2
SHELL_OPTION4 = 3

LINEAR_STRUCTURES = [STRUCT_ARRAY, STRUCT_SINGLE, STRUCT_DOUBLE]
OMEGA_STRUCTURES = [STRUCT_ARRAY, STRUCT_SINGLE, STRUCT_DOUBLE, STRUCT_STACK, STRUCT_TREE]

def prepare_csv(path: str) -> None:
    #Tworzenie foldera
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    #Usuwanie istniejącego pliku .csv
    if os.path.exists(path):
        os.remove(path)

#Budowanie liste argumentów
def benchmark_cmd(results_file: str,
                  algorithm: int,
                  structure: int,
                  data_type: int,
                  length: int,
                  iterations: int,
                  pivot: Optional[int] = None,
                  shell_param: Optional[int] = None) -> List[str]:
    cmd = [
        BIN,
        "--benchmark",
        "-a", str(algorithm),
        "-s", str(structure),
        "-t", str(data_type),
        "-l", str(length),
        "-n", str(iterations),
        "-r", results_file,
    ]

    if pivot is not None:
        cmd += ["-p", str(pivot)]

    if shell_param is not None:
        cmd += ["-e", str(shell_param)]

    return cmd

#Uruchonienie programu
def run_command(args: List[str], extra_env: Optional[Dict[str, str]] = None) -> bool:
    env = os.environ.copy()
    if extra_env:
        env.update(extra_env)

    print("RUN:", " ".join(args))
    if extra_env:
        print("ENV:", extra_env)

    result = subprocess.run(args, text=True, env=env)

    if result.returncode != 0:
        print(f"ERROR: command failed with code {result.returncode}")
        return False

    return True