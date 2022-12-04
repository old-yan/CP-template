import os
import random
for i in range(100):
    with open("in.txt", "wt") as f:
        L = random.randint(1, 100)
        R = random.randint(1, 100)
        if L > R: L, R = R, L
        L, R = 1, 100
        k = random.randint(1, 4)
        f.write("%d %d %d\n" % (L, R, k))
        for i in range(k):
            f.write(str(random.randint(1, 100)) + ' ')
    os.system("")
    os.system("b.exe")
    os.system("c.exe")
    if res := os.system("fc out.txt out2.txt"):
        break
