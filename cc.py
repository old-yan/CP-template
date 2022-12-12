import collections
import itertools
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
sys.stdin = IOWrapper(sys.stdin)
sys.stdout = IOWrapper(sys.stdout)
input = lambda: sys.stdin.readline().rstrip("\r\n")

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

# def make_udg():
#     n = random.randint(3, 10)
#     u = UnionFind(n + 1)
#     edges = []
#     for i in range(n - 1):
#         while True:
#             while True:
#                 a = random.randint(1, n)
#                 b = random.randint(1, n)
#                 # 自环
#                 if a == b: continue
#                 if u.find(a) != u.find(b): break
#                 # 控制环比率
#                 if random.randint(1, 100) < 10: break
#             edges.append([a, b, random.randint(1, 20)])
#             if u.find(a) != u.find(b):
#                 u.merge(a, b)
#                 break
#     return n, edges

for i in range(100):
    n=random.randint(8,10)
    m=random.randint(5,n)
    k=random.randint(3,m)
    with open("in.txt", "wt") as f:
        f.write("%d %d %d\n"%(n,m,k))
        for i in range(n):
            f.write("%d "%random.randint(10,15))
    os.system("c.exe")
    os.system("d.exe")
    if res := os.system("fc out.txt out2.txt"):
        os.system("c.exe")
        os.system("d.exe")
        exit(0)

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
