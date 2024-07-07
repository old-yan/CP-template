### 一、模板类别

​	数据结构：树上树状数组。

​	练习题目：

1. [P3128 [USACO15DEC] Max Flow P](https://www.luogu.com.cn/problem/P3128)
2. [P3178 [HAOI2015] 树上操作](https://www.luogu.com.cn/problem/P3178)
3. [P3258 [JLOI2014] 松鼠的新家](https://www.luogu.com.cn/problem/P3258)
4. [P3384 【模板】重链剖分/树链剖分](https://www.luogu.com.cn/problem/P3384)
5. [P4211 [LNOI2014] LCA](https://www.luogu.com.cn/problem/P4211)
6. [U335992 [提高]树上操作](https://www.luogu.com.cn/problem/U335992)

### 二、模板功能

​	本模板依托于树链剖分和树状数组，只要会用树链剖分和树状数组，就会用本模板，故文档内不再赘述。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDBIT.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(9);
    // 加边
    T.add_edge(3, 1);
    T.add_edge(1, 0);
    T.add_edge(1, 2);
    T.add_edge(2, 8);
    T.add_edge(3, 4);
    T.add_edge(3, 7);
    T.add_edge(7, 5);
    T.add_edge(7, 6);
    T.prepare();
    T.set_root(3);
    cout << T << endl;

    // 不妨让每个结点初始值为编号的一万倍
    OY::HLDBIT32<decltype(T)> bit(&T, [&](int i) {
        return i * 10000;
    });
    cout << bit << endl;

    // 令 5~8 之间的路径增值
    bit.add_path<true>(5, 8, 100);
    cout << bit << endl;

    // 令以 7 为根的子树增值
    bit.add_subtree(7, 1000);
    cout << bit << endl;
}
```

```
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
[30000[10000[0][20000[80000]]][40000][70000[50000][60000]]]
[30100[10100[0][20100[80100]]][40000][70100[50100][60000]]]
[30100[10100[0][20100[80100]]][40000][71100[51100][61000]]]

```

