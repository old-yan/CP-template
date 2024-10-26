### 一、模板类别

​	数据结构：全局懒修改的 `set` 。

​	练习题目：

1. [P3806 【模板】点分治1](https://www.luogu.com.cn/problem/P3806)


### 二、模板功能

​		本模板对 `std::set` 进行一个封装，基于差分，对 `key` 实现了全局懒修改。

​		本模板对 `Key` ，可以通过 `typename CommutativeGroup` 封装一个支持懒修改的交换群。交换群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

4. 定义静态函数 `inverse` ，输入参数一个 `value_type` ，返回其逆元。

​		本模板要求运算符满足**结合律**和**交换律**。常见的交换群为加法群和异或群。

**注意：**

​		不推荐通过迭代器解引用获取键，因为还需要算上全局懒标记的影响，才是真正的键；

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MISC/LazySet.h"

void test_key_add() {
    // key_type 可以全局加
    OY::LazyKeyAddSet<int> mp;
    mp.insert(100);
    mp.insert(300);
    mp.globally_add(10);
    mp.insert(200);
    mp.globally_add(2);

    cout << mp << "\n\n";
}

void test_key_mul() {
    // 设计一个 key 可以全局乘 的 set<double>

    // 需要写一个交换群
    struct monoid {
        using value_type = double;
        static double identity() { return 1; }
        static double op(double x, double y) { return x * y; }
        static double inverse(double x) { return 1 / x; }
    };
    using T = OY::LAZYSET::Table<OY::LAZYIMP::LazyType<monoid>>;

    // 试试效果
    T mp;
    mp.insert(10.1);
    mp.insert(30.3);
    mp.globally_add(4);
    mp.insert(25);
    mp.globally_add(0.1);

    cout << mp << "\n\n";
}

int main() {
    test_key_add();
    test_key_mul();
}
```

```
#输出如下
{112, 202, 312}

{2.500000, 4.040000, 12.120000}

```

