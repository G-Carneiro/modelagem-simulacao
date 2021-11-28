from math import exp

from .MultiplicativeLCG import MultiplicativeLCG


def poisson(lamda_: float, generator: MultiplicativeLCG) -> int:
    n: int = 0
    p: float = 1
    exponential: float = exp(-lamda_)

    while True:
        r: float = generator.get_random_number(n + 1)
        # r: float = [0, 0.8353, 0.9952, 0.8004][n + 1]
        p *= r

        if p < exponential:
            return n

        n += 1



