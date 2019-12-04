import math
import sys

n = int(input())
evaluated = list(False for i in range(n))
result = list(0 for i in range(n))
ops = []

for i in range(n):
    operation, lhs, rhs = input().split()
    ops.append((operation, lhs, rhs))

def value_of(arg):
    if arg[:1] != '$':
        return int(arg)
    else:
        return eval_cell(int(arg[1:]))
        
def eval_cell(i):
    if evaluated[i]:
        return result[i]

    op, lhs, rhs = ops[i]
    r = 0
    
    if op == "VALUE":
        r = value_of(lhs)
    elif op == "ADD":
        r = value_of(lhs) + value_of(rhs)
    elif op == "SUB":
        r = value_of(lhs) - value_of(rhs)
    elif op == "MULT":
        r = value_of(lhs) * value_of(rhs)

    result[i] = r
    evaluated[i] = True
    return r

for i in range(n):
    if not evaluated[i]:
        result[i] = eval_cell(i)

for v in result:
    print(v)
