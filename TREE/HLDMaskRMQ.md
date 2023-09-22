### 一、模板类别

​	数据结构：树上状压 `RMQ` 。

### 二、模板功能

​	本模板依托于树链剖分和状压 `RMQ` ，只要会用树链剖分和状压 `RMQ` ，就会用本模板，故文档内不再赘述。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDMaskRMQ.h"

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
    auto rmq = OY::make_HLDMaskRMQ_ValueTable<int, uint64_t, 1000>(&T, std::less<int>(), [](int i) { return i * 10000; });
    cout << rmq << endl;

    // 输出 5~6 之间路径上的最大值
    cout << rmq.query_path<true>(5, 6) << endl;

    // 输出 5~6 之间路径上，不算最高位结点的最大值
    cout << rmq.query_path<false>(5, 6) << endl;
}
```

```
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
[30000[10000[0][20000[80000]]][40000][70000[50000][60000]]]
70000
60000

```

