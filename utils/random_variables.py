from math import exp, sqrt, log, cos, sin, pi
from typing import Tuple

from .MultiplicativeLCG import MultiplicativeLCG


def poisson(lamda_: float, generator: MultiplicativeLCG) -> int:
    n: int = 0
    p: float = 1
    exponential_: float = exp(-lamda_)

    while True:
        r: float = generator.get_random_number(n + 1)
        p *= r

        if p < exponential_:
            return n

        n += 1


def uniform(a: float, b: float, random_number: float) -> float:
    return (a + (b - a) * random_number)


def triangular(a: float, b: float, c: float, random_number: float) -> float:
    ba: float = b - a
    ca: float = c - a
    if random_number <= ba / ca:
        return (a + sqrt(random_number * ba * ca))

    cb: float = c - b
    return (c - sqrt((1 - random_number) * cb * ca))


def exponential(lamda_: float, random_number: float) -> float:
    return (-lamda_ * log(1 - random_number))


def normal(average: float,
           deviation: float,
           random_number1: float,
           random_number2: float
           ) -> Tuple[float, float]:
    z1: float = sqrt(-2 * log(random_number1)) * cos(2 * pi * random_number2)
    z2: float = sqrt(-2 * log(random_number1)) * sin(2 * pi * random_number2)
    print(z1, z2)

    x1: float = average + deviation * z1
    x2: float = average + deviation * z2

    return (x1, x2)



