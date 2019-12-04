import math
import sys

n = int(input())
in_eval_queue = list(False for i in range(n))
result = list(0 for i in range(n))
eval_queue = []
ops = []

for i in range(n):
    operation, lhs, rhs = input().split()
    ops.append((operation, lhs, rhs))

for i in range(n):
    if in_eval_queue[i]:
        continue
    
    queue = [i]
    
    while queue:
        cell = queue[-1]

        if in_eval_queue[cell]:
            queue.pop()
            continue
        
        op, lhs, rhs = ops[cell]
        ready = True
    
        if lhs[0] == '$':
            ref = int(lhs[1:])
            if not in_eval_queue[ref]:
                ready = False
                queue.append(ref)
            
        if rhs[0] == '$':
            ref = int(rhs[1:])
            if not in_eval_queue[ref]:
                ready = False
                queue.append(ref)

        if ready:
            queue.pop()
            eval_queue.append(cell)
            in_eval_queue[cell] = True

def value_of(arg):
    if arg[0] == '$':
        return result[int(arg[1:])]
    else:
        return int(arg)
    
for i in eval_queue:
    op, lhs, rhs = ops[i]

    if op == "VALUE":
        result[i] = value_of(lhs)
    elif op == "ADD":
        result[i] = value_of(lhs) + value_of(rhs)
    elif op == "SUB":
        result[i] = value_of(lhs) - value_of(rhs)
    elif op == "MULT":
        result[i] = value_of(lhs) * value_of(rhs)

for v in result:
    print(v)
