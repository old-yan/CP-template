import itertools
import os
import random
import time


class UnionFind:

    def __init__(self, n):
        self.parent = list(range(n))

    def find(self, a):
        acopy = a
        while a != self.parent[a]:
            a = self.parent[a]
        while acopy != a:
            self.parent[acopy], acopy = a, self.parent[acopy]
        return a

    def merge(self, a, b):
        self.parent[self.find(b)] = self.find(a)


def make_udg():
    n = random.randint(3, 10)
    u = UnionFind(n + 1)
    edges = []
    for i in range(n - 1):
        while True:
            while True:
                a = random.randint(1, n)
                b = random.randint(1, n)
                # 自环
                if a == b: continue
                if u.find(a) != u.find(b): break
                # 控制环比率
                if random.randint(1, 100) < 10: break
            edges.append([a, b, random.randint(1, 20)])
            if u.find(a) != u.find(b):
                u.merge(a, b)
                break
    return n, edges


for i in range(100):
    n, edges = make_udg()
    with open("in.txt", "wt") as f:
        f.write("%d %d\n" % (n, len(edges)))
        for a, b, c in edges:
            f.write("%d %d %d\n" % (a, b, c))
    os.system("c.exe")
    os.system("d.exe")
    if res := os.system("fc out.txt out2.txt"):
        os.system("c.exe")
        os.system("d.exe")
        exit(0)
