### 一、模板类别

​	数据结构：可区间加的历史和二维树状数组


### 二、模板功能

​		本模板为 `BIT2D` 模板的加强版，支持在原树基础上复制版本，并进行目前的所有版本的区间和的查询。


### 三、模板示例

```c++
#include "DS/HistoryBIT2D.h"
#include "IO/FastIO.h"

void test() {
    OY::HBIT2D::Tree<int> S(2, 3, [&](int r, int c) {
        return (r + 1) * 100 + (c + 1);
    });
    cout << S << endl;

    S.copy_version();
    S.add(0, 0, 10);
    cout << S << endl;

    S.copy_version();
    S.add(1, 2, 70);
    cout << S << endl;

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
[[101, 102, 103], [201, 202, 203]]
[[111, 102, 103], [201, 202, 203]]
[[111, 102, 103], [201, 202, 273]]
history sum of each position:
323,306,309
603,606,679

```

