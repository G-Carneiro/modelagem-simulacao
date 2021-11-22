def multiplicative_lcg(sequence_number: int,
                       seed: int,
                       multiplier: int,
                       module: int,
                       increment: int = 0
                       ) -> int:
    if not sequence_number:
        return seed

    last_number = multiplicative_lcg(sequence_number - 1, seed, multiplier, module, increment)

    return (multiplier * last_number + increment) % module
