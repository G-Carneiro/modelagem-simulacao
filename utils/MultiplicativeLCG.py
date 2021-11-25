from typing import List, Tuple


class MultiplicativeLCG:
    def __init__(self,
                 seed: int,
                 multiplier: int,
                 module: int,
                 increment: int = 0
                 ) -> None:
        self._seed: int = seed
        self._multiplier: int = multiplier
        self._module: int = module
        self._increment: int = increment
        self._period: int = 0
        self._random_table: List[Tuple[int, float]] = [(seed, seed / module)]

    def get_random_number(self, sequence_number: int) -> float:
        while sequence_number >= len(self._random_table):
            last_number: int = self._random_table[-1][0]

            number: int = (self._multiplier * last_number + self._increment) % self._module
            random_number: float = number / self._module
            self._check_period(number)

            self._random_table.append((number, random_number))

        return self._random_table[sequence_number][1]

    def get_period(self) -> int:
        while not self._period:
            self.get_random_number(len(self._random_table))

        return self._period

    def _check_period(self, number: int) -> None:
        all_numbers: List[int] = [number for number, _ in self._random_table]
        if not self._period and (number in all_numbers):
            self._period = len(all_numbers)

        return None
