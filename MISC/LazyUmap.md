### 一、模板类别

​	数据结构：全局懒修改的 `unordered_map` 。

​	练习题目：

1. [k - 路径（hard vension）](https://ac.nowcoder.com/acm/problem/279411)
2. [小Z的树迁移](https://ac.nowcoder.com/acm/problem/281601)


### 二、模板功能

​		本模板对 `std::unordered_map` 进行一个封装，基于差分，对 `key` 和 `mapped` 实现了全局懒修改。

​		本模板对 `Key` 和 `Mapped` ，都可以通过 `typename CommutativeGroup` 封装一个支持懒修改的交换群。交换群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

4. 定义静态函数 `inverse` ，输入参数一个 `value_type` ，返回其逆元。

​		本模板要求运算符满足**结合律**和**交换律**。常见的交换群为加法群和异或群。

**注意：**

​		不推荐通过迭代器访问 `first` 获取键，因为还需要算上全局懒标记的影响，才是真正的键；

​		通过 `find` 以及插入所获取的迭代器，访问 `second` 获取值是合理的，因为懒标记的影响已更新；

​		通过迭代器前移后移获取到的迭代器，访问 `second` 获取值是不合理的，因为懒标记的影响未更新。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MISC/LazyUmap.h"

void test_key_add() {
    // key_type 可以全局加
    OY::LazyKeyAddUmap<int, std::string> mp;
    mp.insert_or_assign(100, "A");
    mp.insert_or_assign(300, "C");
    mp.globally_add(10);
    mp.insert_or_assign(200, "B");
    mp.globally_add(2);

    cout << mp << "\n\n";
}

void test_mapped_add() {
    // mapped_type 可以全局加
    OY::LazyMappedAddUmap<std::string, int> mp;
    mp.insert_or_assign("A", 100);
    mp.insert_or_assign("C", 300);
    mp.globally_add(10);
    mp.insert_or_assign("B", 200);
    mp.globally_add(2);

    cout << mp << "\n\n";
}

void test_key_add_mapped_add() {
    // key_type 和 mapped_type 都可以全局加
    OY::LazyKeyAddMappedAddUmap<int, int> mp1;
    mp1.insert_or_assign(100, 1000);
    mp1.globally_add({20, 0});
    mp1.insert_or_assign(200, 2000);
    mp1.globally_add({0, 3});

    OY::LazyKeyAddMappedAddUmap<int, int> mp2;
    mp2.insert_or_assign(400, 4000);
    mp2.globally_add({0, 5});
    mp2.insert_or_assign(300, 3000);
    mp2.globally_add({40, 0});
    mp2.insert_or_assign(200, 70000);

    // map 合并
    mp2.enumerate([&](int k, int v) {
        mp1.insert_or_modify(k, [&](int &old, bool flag) {
            if (flag)
                old = v;
            else
                old += v;
        });
    });
    mp2.clear();

    // 看看结果
    cout << mp1 << "\n\n";
}

void test_key_xor_mapped_mul() {
    // 设计一个 key 可以全局异或， mapped 可以全局乘 的 map<int, double>

    // key 支持异或很简单，有现成的
    using Key = OY::LAZYUMAP::LazyBitXor<int>;

    // mapped 支持乘法需要写一个交换群
    struct monoid {
        using value_type = double;
        static double identity() { return 1; }
        static double op(double x, double y) { return x * y; }
        static double inverse(double x) { return 1 / x; }
    };
    using Mapped = OY::LAZYUMAP::LazyType<monoid>;

    // 组合成一个 map
    using T = OY::LAZYUMAP::Table<Key, Mapped>;

    // 试试效果
    T mp;
    mp.insert_or_assign(18, 10.1);
    mp.insert_or_assign(32, 30.3);
    mp.globally_add({4, 1});
    mp.insert_or_assign(25, 0.5);
    mp.globally_add({0, 0.1});
    mp.globally_add({12, 1});

    cout << mp << "\n\n";
}

void test_map_map() {
    // 二维 map
    // key_type 和 mapped_type 都可以全局加
    using Map1 = OY::LazyKeyAddMappedAddUmap<int, int>;
    using Map2 = OY::LAZYUMAP::Table<OY::LAZYUMAP::LazyAdd<int>, Map1>;
    Map2 mp;
    mp.insert_or_assign(10, {});
    mp.insert_or_assign(20, {});
    mp.globally_add({1});
    mp.get(20).insert_or_assign(200, 2000);
    mp.insert_or_assign(30, {});
    mp.globally_add({2, {10, 0}});
    mp.get(10).insert_or_assign(100, 1000);

    cout << mp << "\n\n";
}

int main() {
    test_key_add();
    test_mapped_add();
    test_key_add_mapped_add();
    test_key_xor_mapped_mul();
    test_map_map();
}
```

```
#输出如下
{202: B, 312: C, 112: A}

{B: 202, C: 312, A: 112}

{440: 4005, 340: 3000, 200: 72003, 120: 1003}

{21: 0.050000, 40: 3.030000, 26: 1.010000}

{32: {}, 22: {210: 2000}, 10: {100: 1000}, 23: {}, 13: {}}


```

