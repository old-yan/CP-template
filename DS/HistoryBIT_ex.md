### 一、模板类别

​	数据结构：历史和树状数组

​	练习题目：

1. [P3246 [HNOI2016] 序列](https://www.luogu.com.cn/problem/P3246)
2. [U216697 线段树区间历史版本和](https://www.luogu.com.cn/problem/U216697)

### 二、模板功能

​		本模板与树状数组的模板接口基本一致，唯一多出来的函数是 `copy_version` ，用来增加一个新的版本，新的版本的各点点值完全等于上个版本的各点点值。在初始化时，版本数量为 `1` ，即为初始版本。如果想做一些修改并把修改后的内容作为新版本，请先 `copy_version` 再修改，因为所有的修改都是对最新的版本执行的。


### 三、模板示例

```c++
#include "DS/HistoryBIT_ex.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorHistoryBIT_ex<int> S(3, [&](int i) {
        return (i + 1) * 100;
    });
    cout << S << endl;

    S.copy_version();
    S.add(0, 1, 10);
    cout << S << endl;

    S.copy_version();
    S.add(1, 2, 70);
    cout << S << endl;

    for (int i = 0; i < 3; i++)
        cout << "history of index " << i << " = " << S.history_query(i) << endl;
}

int main() {
    test();
}
```

```
#输出如下
[100, 200, 300]
[110, 210, 300]
[110, 280, 370]
history of index 0 = 320
history of index 1 = 690
history of index 2 = 970

```

