### 一、模板类别

​	数据结构：可区间加的历史和哈希树状数组


​	练习题目：

1. [P3246 [HNOI2016] 序列](https://www.luogu.com.cn/problem/P3246)
2. [U216697 线段树区间历史版本和](https://www.luogu.com.cn/problem/U216697)


### 二、模板功能

​		本模板为 `GlobalHashBIT` 模板的加强版，支持在原树基础上复制版本，并进行目前的所有版本的区间和的查询。


### 三、模板示例

```c++
#include "DS/GlobalHashHistoryBIT.h"
#include "IO/FastIO.h"

OY::GHBIT::Tree<uint32_t, int, true, false, 1000> S(3);
void test() {
    auto print = [&] {
        cout << "S: ";
        for (int i = 0; i < S.size(); i++) cout << S.query(i) << ' ';
        cout << endl;
    };
    S.copy_version();
    S.add(0, 10);
    S.add(1, 10);
    print();

    S.copy_version();
    S.add(1, 2, 70);
    print();

    for (int i = 0; i < 3; i++)
        cout << "history of index " << i << " = " << S.history_query(i) << endl;
}

int main() {
    test();
}
```

```
#输出如下
S: 10 10 0 
S: 10 80 70 
history of index 0 = 20
history of index 1 = 90
history of index 2 = 70

```

