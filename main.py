from utils.random_number_generators import *


for i in range(1, 20):
    num, random = multiplicative_lcg(sequence_number=i,
                                     seed=7,
                                     multiplier=5,
                                     module=16,
                                     increment=3)

    print(num, random)

table: List[Tuple[int, float]] = iterative_multiplicative_lcg(sequence_number=19,
                                                              seed=7,
                                                              multiplier=5,
                                                              module=16,
                                                              increment=3)

print(*table)
