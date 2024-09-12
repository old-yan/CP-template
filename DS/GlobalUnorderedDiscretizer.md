### 一、模板类别

​	数据结构：全局无序离散化

​	练习题目：

1. [#2774. 「BalticOI 2018」多角恋](https://loj.ac/p/2774)
2. [P1955 [NOI2015] 程序自动分析](https://www.luogu.com.cn/problem/P1955)
3. [U428478 祖孙询问](https://www.luogu.com.cn/problem/U428478)

### 二、模板功能

​		本模板建立于 `GlobalHashmap` 的基础上。当调用 `rank` 函数返回一个元素的离散化标号。

​		本模板与 `Discretizer` 模板的差别是，本模板为无序映射，不需要保证小元素对应小标号、大元素对应大标号。因此，并不需要等待全部元素插入完毕之后才能获取标号。

​		当调用 `rank` 函数时，若模板参数 `Check` 为 `true` ，会在获取标号时做检查；而当 `Check` 为 `false` ，不做检查。如果确信元素已经在表中，推荐将 `Check` 设为 `false` 。两个版本的 `rank` 函数可以被 `insert` 或者 `find` 代替。


### 三、模板示例

```c++
#include "DS/GlobalUnorderedDiscretizer.h"
#include "IO/FastIO.h"

OY::GlobalUnorderedDiscretizer<int, 1 << 10> D;
int main() {
    cout << "22 -> " << D.insert(22) << endl;
    cout << "33 -> " << D.insert(33) << endl;
    cout << "11 -> " << D.insert(11) << endl;

    // 输出一下存在的元素
    cout << "elems: ";
    D.do_for_each([](int x) {
        cout << x << ' ';
    });
    cout << endl;
}
```

```
#输出如下
22 -> 0
33 -> 1
11 -> 2
elems: 22 33 11 

```

