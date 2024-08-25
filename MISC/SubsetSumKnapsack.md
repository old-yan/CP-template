### 一、模板类别

​	数据结构： `01` 背包获取可能的子集和。

​	练习题目：

1. [416. 分割等和子集](https://leetcode.cn/problems/partition-equal-subset-sum/)
2. [自爆机器人](https://ac.nowcoder.com/acm/problem/275719)
3. [图上计数(Hard)](https://ac.nowcoder.com/acm/problem/276004)


### 二、模板功能

​		本模板需要配合 `DynamicBitset` 或者 `StaticBitset` 使用。本模板提供两个函数，传入一个 `DynamicBitset` 或者 `StaticBitset` 对象的引用，并传入一个可以用于背包的材料集合；返回通过这些材料可以获取的背包答案。

​		本模板进行背包转移的范围，由 `DynamicBitset` 或者 `StaticBitset` 的 `size` 属性指定，所以如果使用 `DynamicBitset` ，需要在传参之前先设定其大小。

​		本模板的 `solve_01` 函数对应 `01` 背包； `solve_unbounded` 函数对应完全背包。
      

### 三、模板示例

```c++
#include "DS/DynamicBitset.h"
#include "IO/FastIO.h"
#include "MISC/SubsetSumKnapsack.h"

int main() {
    constexpr int n = 100000;
    OY::DynamicBitset table(n + 1);
    std::vector<int> materials;
    for (int v = 40000; v <= 49999; v++) materials.push_back(v);
    for (int v = 60001; v <= 99999; v++) materials.push_back(v);
    OY::SSK::solve_01(table, materials);

    if (table[99999]) {
        cout << "99999 can be made\n";
    }
    if (!table[100000]) {
        cout << "100000 can not be made\n";
    }
}
```

```
#输出如下
99999 can be made
100000 can not be made

```

