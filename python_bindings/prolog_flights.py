"""
Вызов Prolog-решателя задачи коммивояжёра из Python через janus_swi.

Условия задачи (матрица расстояний) задаются извне (в Python/GUI),
а Prolog получает их аргументом предиката answer/3.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Any, List, Optional, Sequence, Union
import os
import shutil

Number = Union[int, float]


@dataclass(frozen=True)
class FlightsResult:
    path: List[int]  # путь, проходящий все пункты ровно 1 раз
    dist: Number


class PrologFlightsSolver:
    def __init__(self, prolog_file: Optional[Path] = None) -> None:
        if prolog_file is None:
            prolog_file = Path(__file__).resolve().parent / "prolog" / "flights.pl"
        self.prolog_file = prolog_file
        self._loaded = False

    @staticmethod
    def _to_prolog_path(p: Path) -> str:
        return str(p.resolve()).replace("\\", "/")

    def _ensure_loaded(self) -> None:
        if self._loaded:
            return

        try:
            # janus-swi ищет SWI‑Prolog через:
            # - переменную окружения SWIPL (путь к swipl.exe)
            # - swipl в PATH
            # - реестр Windows
            # Если SWI‑Prolog установлен “нестандартно”, поможем авто‑поиском.
            if os.name == "nt" and not os.environ.get("SWIPL"):
                swipl = shutil.which("swipl")
                if not swipl:
                    candidates = [
                        r"C:\Program Files\swipl\bin\swipl.exe",
                        r"C:\Program Files (x86)\swipl\bin\swipl.exe",
                    ]
                    for c in candidates:
                        if os.path.exists(c):
                            swipl = c
                            break
                if swipl:
                    os.environ["SWIPL"] = swipl

            import janus_swi as janus  # type: ignore
        except Exception as e:
            raise RuntimeError(
                "Не удалось импортировать janus_swi.\n"
                "Установка: pip install janus-swi\n"
                "Также нужен SWI‑Prolog 9.1.12+.\n"
                "Проверьте одно из:\n"
                "  - команда `swipl --version` работает в терминале (PATH)\n"
                "  - или установите переменную окружения SWIPL = полный путь к swipl.exe\n"
                f"Детали: {e}"
            ) from e

        pl_path = self._to_prolog_path(self.prolog_file)
        janus.query_once(f"consult('{pl_path}')")
        self._loaded = True

    def solve(self, matrix: Sequence[Sequence[Number]]) -> FlightsResult:
        self._ensure_loaded()

        n = len(matrix)
        if n == 0:
            raise ValueError("Матрица расстояний пуста.")
        if n > 10:
            raise ValueError("Для ЛР5 допускается N ≤ 10.")
        for i, row in enumerate(matrix):
            if len(row) != n:
                raise ValueError(f"Матрица должна быть квадратной: строка {i} длины {len(row)}, ожидалось {n}.")
            for j, v in enumerate(row):
                if float(v) < 0:
                    raise ValueError(f"Расстояния должны быть неотрицательными: [{i}][{j}] = {v}.")

        import janus_swi as janus  # type: ignore
        q = "flights:answer(Matrix, Path, Dist)"
        res: Any = janus.query_once(q, {"Matrix": [list(r) for r in matrix]})

        if not res or not res.get("truth"):
            raise RuntimeError("Prolog не нашёл решение. Проверьте матрицу расстояний.")

        path = res.get("Path")
        dist = res.get("Dist")
        if not isinstance(path, list) or not all(isinstance(x, int) for x in path):
            raise RuntimeError(f"Неожиданный формат Path от Prolog: {path!r}")

        return FlightsResult(path=path, dist=dist)


def parse_matrix(text: str) -> List[List[float]]:
    rows: List[List[float]] = []
    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line:
            continue
        parts = [p for p in line.replace(";", " ").replace(",", " ").split() if p]
        rows.append([float(p) for p in parts])
    return rows

