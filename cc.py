import collections
import decimal
import itertools
import math
import os
import random
import sys
from io import BytesIO, IOBase

BUFSIZE = 4096
inf = float('inf')

class FastIO(IOBase):
    newlines = 0

    def __init__(self, file):
        self._fd = file.fileno()
        self.buffer = BytesIO()
        self.writable = "x" in file.mode or "r" not in file.mode
        self.write = self.buffer.write if self.writable else None

    def read(self):
        while True:
            b = os.read(self._fd, max(os.fstat(self._fd).st_size, BUFSIZE))
            if not b:
                break
            ptr = self.buffer.tell()
            self.buffer.seek(0, 2), self.buffer.write(b), self.buffer.seek(ptr)
        self.newlines = 0
        return self.buffer.read()

    def readline(self):
        while self.newlines == 0:
            b = os.read(self._fd, max(os.fstat(self._fd).st_size, BUFSIZE))
            self.newlines = b.count(b"\n") + (not b)
            ptr = self.buffer.tell()
            self.buffer.seek(0, 2), self.buffer.write(b), self.buffer.seek(ptr)
        self.newlines -= 1
        return self.buffer.readline()

    def flush(self):
        if self.writable:
            os.write(self._fd, self.buffer.getvalue())
            self.buffer.truncate(0), self.buffer.seek(0)


class IOWrapper(IOBase):

    def __init__(self, file):
        self.buffer = FastIO(file)
        self.flush = self.buffer.flush
        self.writable = self.buffer.writable
        self.write = lambda s: self.buffer.write(s.encode("ascii"))
        self.read = lambda: self.buffer.read().decode("ascii")
        self.readline = lambda: self.buffer.readline().decode("ascii")

xx=0
oldx=sys.stdout.flush
def myflush():
    global xx
    xx+=1
    return oldx
sys.stdout.flush=myflush
input = lambda: sys.stdin.readline().rstrip("\r\n")
for i in range(100000):
    print(i,end='')
print("__________________________________________________________________________________________________________xx=",xx)

# print("hello")
# print("world")
exit(0)
# t = int(input())
# for _ in range(t):
#     n, m, k = map(int, input().split())
#     A = sorted(map(int, input().split()), reverse=True)
#     if all(A[i] <= (n + k - 1 - i) // k for i in range(m)):
#         print("Yes")
#     else:
#         print("No")


class UnionFind:

    def __init__(self, n):
        self.parent = list(range(n))
        self.count = n

    def find(self, a):
        acopy = a
        while a != self.parent[a]:
            a = self.parent[a]
        while acopy != a:
            self.parent[acopy], acopy = a, self.parent[acopy]
        return a

    def merge(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b: return False
        self.parent[self.find(b)], self.count = self.find(a), self.count - 1
        return True


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


def make_bound_fg():
    n = random.randint(3, 3)
    m = 2
    edges = []
    v = [0] * (n + 1)
    for i in range(m):
        a = random.randint(1, n)
        b = random.randint(1, n)
        # 自环
        if a == b: continue
        f = random.randint(5, 15)
        edges.append([a, b, f])
        v[a], v[b] = v[a] - f, v[b] + f
    while min(v) != 0:
        v2 = list(map(abs, v))
        i = v2.index(min(filter(lambda x: x, v2)))
        j = next(_ for _ in range(n + 1) if v[i] * v[_] < 0)
        if v[i] > 0:
            f = v[i]
            edges.append([i, j, f])
            v[i], v[j] = v[i] - f, v[j] + f
        else:
            f = -v[i]
            edges.append([j, i, f])
            v[j], v[i] = v[j] - f, v[i] + f
    for i, v in enumerate(edges):
        a, b, f = v
        low = random.randint(1, f)
        high = random.randint(low, low + 5)
        edges[i] = [a, b, low, high]
    return n, edges

    return n, edges


# for i in range(1000):
#     n = random.randint(1, 20)
#     k = random.randint(1, n)
#     A = []
#     for i in range(n):
#         a = random.randint(1, 3)
#         if a == 1:
#             A.append(random.randint(-3, -1))
#         elif a == 2:
#             A.append(0)
#         else:
#             A.append(random.randint(1, 3))
#     with open("in.txt", "wt") as f:
#         f.write("%d %d\n" % (n, k))
#         f.write(("%d " * n) % (tuple(A)))
# os.system("b.exe")
# os.system("bb.exe")
# if res := os.system("fc out.txt out2.txt"):
#     exit(0)

# import os
# import time

# path1 = "D:\\softwaredata\\vscode_file\\pyQPSLOpticalImages\\QPSLClass"
# files1 = os.listdir(path1)
# path2 = "D:\\softwaredata\\vscode_file\\pyQPSLControls\\QPSLClass"
# files2 = os.listdir(path2)

# files1.sort()
# files2.sort()
# print(set(files2)-set(files1))
# ans=[]
# for f in files1:
#     res=os.system("fc %s\\%s %s\\%s"%(path1,f, path2,f))
#     if res:ans.append(f)
# print(ans)

# print(len(files1), len(files2))
