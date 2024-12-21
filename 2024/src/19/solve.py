import sys
from functools import cache

assert(len(sys.argv) == 2)
with open(sys.argv[1]) as f:
    available,targets = f.read().split('\n\n')

available = available.split(', ')
targets = targets.splitlines()

solution = 0;
solution2 = 0;

@cache
def check(target):
    if target == '':
        return 1
    score = 0
    for start in available:
        if target.startswith(start):
            score += check(target[len(start):])
    return score

for target in targets:
    result = check(target)
    solution += (result != 0)
    solution2 += result

print(f"Part 1: {solution}\nPart 2: {solution2}")
