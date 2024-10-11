### 一、模板类别

​	数据结构：可子矩形加的历史和哈希二维树状数组


​	练习题目：

1. [P3810 【模板】三维偏序（陌上花开）](https://www.luogu.com.cn/problem/P3810)

### 二、模板功能

​		本模板为 `GlobalHashBIT2D` 模板的加强版，支持在原树基础上复制版本，并进行目前的所有版本的区间和的查询。


### 三、模板示例

```c++
#include "DS/GlobalHashHistoryBIT2D.h"
#include "IO/FastIO.h"

OY::GHBIT2D::Tree<uint32_t, int, true, false, 1000> S(2, 3);
auto print = [] {
    cout << "S:\n";
    for (int r = 0; r < S.row(); r++)
        for (int c = 0; c < S.column(); c++)
            cout << S.query(r, c) << " \n"[c + 1 == S.column()];
};
void test() {
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 3; c++)
            S.add(r, c, (r + 1) * 100 + (c + 1));
    print();

    S.copy_version();
    S.add(0, 0, 10);
    print();

    S.copy_version();
    S.add(1, 2, 70);
    print();

    cout << "history sum of each position:\n";
    for (int r = 0; r < 2; r++)
        for (int c = 0; c < 3; c++)
            cout << S.history_query(r, c) << ",\n"[c == 2];
}

int main() {
    test();
}
```

```
#输出如下
S:
101 102 103
201 202 203
S:
111 102 103
201 202 203
S:
111 102 103
201 202 273
history sum of each position:
323,306,309
603,606,679

```

