import collections
import decimal
import functools
import itertools
import math
import os
import random
import sys
from io import BytesIO, IOBase
from math import inf
import re

sys.stdin = open("in.txt", "rt")
sys.stdout = open("out.txt", "wt")


def f(n):
    mp = dict()
    for i in range(1000):
        r = 10**i % n
        print(i, r)
        if r in mp:
            S = sorted(k for k, v in mp.items() if v >= mp[r])
            print(S)
            # assert S[0] + S[-1] == n
            return i - mp[r]
        else:
            mp[r] = i


# f(3)
# f(41)
# for i in range(10):
#     print(10**i % 6)
# print(10 % 6)
for p in range(2, 100):
    q = p
    while p % 2 == 0:
        p //= 2
    while p % 5 == 0:
        p //= 5
    # if p == 1: continue
    pp = p
    s = 1
    for a in range(2, p + 1):
        acnt = 0
        while p % a == 0:
            p //= a
            s *= a
            acnt += 1
        if a in [2, 5]: continue
        if acnt == 0: continue
        s //= a
        s *= a - 1
    print(q, s, 10**s % pp)

# f(3 * 2)
# f(3 * 2 * 2)
# f(3 * 5)
# f(3 * 5 * 5)
