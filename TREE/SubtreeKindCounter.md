### 一、模板类别

​	数据结构：子树元素种类数查询表

​	练习题目：

1. [U41492 树上数颜色](https://www.luogu.com.cn/problem/U41492)

### 二、模板功能

​		本模板名为模板，其实只是一个函数，模板参数需要填写哈希表类型（见 `KindCounter` 模板）、最大结点数，入参需要填写有根树指针、结点下标到元素种类的映射函数。在正常调用后，返回一个 `std::vector` 表示每个结点以自身为根的子树的元素种类数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/SubtreeKindCounter.h"

void test() {
    OY::FlatTree::Tree<bool, 1000> S(5);
    S.add_edge(0, 1);
    S.add_edge(0, 2);
    S.add_edge(1, 3);
    S.add_edge(1, 4);
    S.prepare(), S.set_root(0);

    std::string elem[5] = {"apple", "banana", "banana", "orange", "orange"};

    auto res = OY::HashmapTreeKindCounter<false, 1000, 1000>::solve(&S, [&](int i) {
        return elem[i];
    });

    for (int i = 0; i < 5; i++)
        cout << "kind count of subtree[" << i << "] = " << res[i] << endl;
}

int main() {
    test();
}
```

```
#输出如下
kind count of subtree[0] = 3
kind count of subtree[1] = 2
kind count of subtree[2] = 1
kind count of subtree[3] = 1
kind count of subtree[4] = 1

```

