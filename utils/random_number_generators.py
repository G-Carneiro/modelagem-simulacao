from typing import Tuple, List


def _recursive_multiplicative_lcg(sequence_number: int,
                                  seed: int,
                                  multiplier: int,
                                  module: int,
                                  increment: int = 0
                                  ) -> int:
    if not sequence_number:
        return seed

    last_number: int = _recursive_multiplicative_lcg(sequence_number - 1,
                                                     seed, multiplier, module, increment)

    return (multiplier * last_number + increment) % module


def multiplicative_lcg(sequence_number: int,
                       seed: int,
                       multiplier: int,
                       module: int,
                       increment: int = 0
                       ) -> Tuple[int, float]:
    last_number: int = _recursive_multiplicative_lcg(sequence_number - 1,
                                                     seed, multiplier, module, increment)

    number: int = (multiplier * last_number + increment) % module
    random_number: float = number / module

    return (number, random_number)


def iterative_multiplicative_lcg(sequence_number: int,
                                 seed: int,
                                 multiplier: int,
                                 module: int,
                                 increment: int = 0
                                 ) -> List[Tuple[int, float]]:

    random_table: List[Tuple[int, float]] = [(seed, seed / module)]

    for _ in range(sequence_number):
        last_number: int = random_table[-1][0]

        number: int = (multiplier * last_number + increment) % module
        random_number: float = number / module

        random_table.append((number, random_number))

    return random_table


def search_period_lcg(random_table: List[Tuple[int, float]]) -> int:
    aux_table: List[int] = []
    for number, _ in random_table:
        aux_table.append(number)

    for i in range(len(aux_table)):
        if aux_table[i] in aux_table[:i]:
            return i

    return len(aux_table)


# TODO: confirmar se está correto
def schrage(seed: int,
            multiplier: int,
            module: int) -> float:

    q: int = module // multiplier
    r: int = module % multiplier
    g: int = (multiplier * seed) % q - r * (seed // q)
    h: int = (seed // q) - (multiplier * seed // module)

    return g + module * h
