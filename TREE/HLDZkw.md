### 一、模板类别

​	数据结构：树上线段树。

​	练习题目：

1. [P2486 [SDOI2011] 染色](https://www.luogu.com.cn/problem/P2486)
2. [P3703 [SDOI2017] 树点涂色](https://www.luogu.com.cn/problem/P3703)


### 二、模板功能

​	本模板依托于树链剖分和 `zkw` 线段树，只要会用树链剖分和 `zkw` 线段树，就会用本模板，故文档内不再赘述。

​	本模板名为**树上线段树**，而不像数据结构里一样，对 `Seg` 和 `Zkw` 进行区分，这是因为只要是树问题，不存在区间过大必须要离散化的可能，那么 `zkw` 就是最优解，不需要考虑 `Seg` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDZkw.h"

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
    auto op = std::plus<int>();
#if CPP_STANDARD >= 202002L
    auto map = [](int x, int y, int size) { return y + x * size; };
#else
    struct {
        int operator()(int x, int y, int size) const { return y + x * size; };
    } map;
#endif
    auto com = std::plus<int>();
    auto mapping = [](int i) { return i * 10000; };
    auto zkw = OY::make_lazy_HLDZkw<int, int, false>(&T, mapping, op, map, com);
    cout << zkw << endl;

    // 令 5~8 之间的路径增值
    zkw.add_path<true>(5, 8, 100);
    cout << zkw << endl;

    // 令以 7 为根的子树增值
    zkw.add_subtree(7, 1000);
    cout << zkw << endl;
}
```

```
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
[30000[10000[0][20000[80000]]][40000][70000[50000][60000]]]
[30100[10100[0][20100[80100]]][40000][70100[50100][60000]]]
[30100[10100[0][20100[80100]]][40000][71100[51100][61000]]]

```

