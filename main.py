from utils.random_number_generators import *

sequence_number: int = 30
seed: int = 13
multiplier: int = 17
increment: int = 43
module: int = 100

print(f"0 - {seed}, {seed / module}")
for i in range(1, sequence_number + 1):
    num, random = multiplicative_lcg(sequence_number=i,
                                     seed=seed,
                                     multiplier=multiplier,
                                     module=module,
                                     increment=increment)

    print(f"{i} - {num}, {random}")

table: List[Tuple[int, float]] = iterative_multiplicative_lcg(sequence_number=sequence_number,
                                                              seed=seed,
                                                              multiplier=multiplier,
                                                              module=module,
                                                              increment=increment)

print(*table)
print(search_period_lcg(table))
