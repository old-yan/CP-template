### 一、模板类别

​	数学：排列组合。

​	练习题目：

1. [1359. 有效的快递序列数目](https://leetcode.cn/problems/count-all-valid-pickup-and-delivery-options)
2. [P1595 信封问题](https://www.luogu.com.cn/problem/P1595)
3. [P4921 [MtOI2018] 情侣？给我烧了！](https://www.luogu.com.cn/problem/P4921)
4. [P4931 [MtOI2018] 情侣？给我烧了！（加强版）](https://www.luogu.com.cn/problem/P4931)
5. [P8764 [蓝桥杯 2021 国 BC] 二进制问题](https://www.luogu.com.cn/problem/P8764)
6. [U177764 【模板】错排统计/逃脱](https://www.luogu.com.cn/problem/U177764)


### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename Tp` ，可以为任意一种自取模数类。

   模板参数 `uint32_t MAX_RANGE` ，表示最大打表范围。

   构造参数 `uint32_t range` ，表示实际打表范围。默认为 `MAX_RANGE` 。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   由于在排列组合问题中大量用到阶乘和阶乘的乘法逆元，所以在构造函数中对阶乘和阶乘的乘法逆元进行打表。

#### 2.查询组合数(comb)

1. 数据类型

   输入参数 `uint32_t n` ，表示从 `n` 个元素中进行选取。

   输入参数 `uint32_t m` ，表示选取出 `m` 个元素。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法的含义为，给 `10` 个标号各不同的的球，一次性选 `3` 个球，则选择方案数为 `comb(10, 3)` 种。

#### 3.查询全排列(perm)

1. 数据类型

   输入参数 `uint32_t n` ，表示对 `n` 个元素进行排列。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法的含义为，给 `10` 个标号各不同的的球，按顺序选 `10` 个球，则选择方案数为 `perm(10)` 种。

#### 4.查询排列数(perm)

1. 数据类型

   输入参数 `uint32_t n` ，表示从 `n` 个元素中进行选取、排列。

   输入参数 `uint32_t m` ，表示选取出 `m` 个元素并排列。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法的含义为，给 `10` 个标号各不同的的球，按顺序选 `3` 个球，则选择方案数为 `perm(10, 3)` 种。

#### 5.查询多重集的全排列(perm_multi)

1. 数据类型

   输入参数 `Iterator first` ，表示元素所在区间的区间头。

   输入参数 `Iterator last` ，表示元素所在区间的区间尾。（开区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法的含义为：如果输入 `[2, 3, 5]` ，表示查询有两个红球，三个蓝球，五个绿球，总共有多少种排列方式。

#### 6.查询不相邻的组合数(comb_nonadjacent)

1. 数据类型

   输入参数 `uint32_t n` ，表示从 `n` 个元素中进行选取。

   输入参数 `uint32_t m` ，表示选取出 `m` 个不相邻的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示从 `n` 个元素中选取出 `m` 个互不相邻的元素的方案数。
   
   显然，当 `m > (n + 1) / 2` 时，必无解，返回 `0` 。

#### 7.全错排列(perm_staggered)

1. 数据类型

   输入参数 `uint32_t n` ，表示对 `n` 个元素进行排列。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示对 `n` 个元素进行排列后，每个元素都和自己之前的旧位置不同的方案数。
   
   显然，当 `n == 1` 时，必无解，返回 `0` 。
   
   特别的，当 `n == 0` ，时，返回 `1` 。

#### 8.圆上全排列(perm_circular)

1. 数据类型

   输入参数 `uint32_t n` ，表示对 `n` 个元素进行排列。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示对 `n` 个圆上的不同元素进行排列的方案数。
   
   某种排列方案经过旋转得到的方案视为重复方案。

#### 9.圆排列(perm_circular)

1. 数据类型

   输入参数 `uint32_t n` ，表示从 `n` 个元素中进行选取。

   输入参数 `uint32_t m` ，表示选取出 `m` 个不相邻的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示从 `n` 个圆上的不同元素中选取 `m` 个进行排列的方案数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt32.h"

using mint = OY::StaticModInt32<97, true>;
int main() {
    OY::CombinationTable<mint, 20> table;
    int people = 10;
    int need_people = 4;
    // 在 10 个人里抽出 4 个人，一起去参加某一场会议的方案数
    cout << "comb(" << people << "," << need_people << ") = " << table.comb(people, need_people) << endl;
    // 10 个人按序参加 10 场会议（每人一场）的方案数
    cout << "perm(" << people << "," << people << ") = " << table.perm(people) << endl;
    // 在 10 个人里抽出 4 个人，按序参加 4 场会议（每人一场）的方案数
    cout << "perm(" << people << "," << need_people << ") = " << table.perm(people, need_people) << endl;
    // 在 10 个人里抽出 4 个不相邻的人，一起去参加某一场会议的方案数
    cout << "nonadjacent perm(" << people << "," << need_people << ") = " << table.comb_nonadjacent(people, need_people) << endl;
    // 10 个人每个人有一个不想去参加的会议，则他们参加 10 场会议（每人一场）且皆大欢喜的总方案数
    cout << "staggered perm(" << people << "," << people << ") = " << table.perm_staggered(people) << endl;
    // 10 个人在圆桌上吃饭的排列总方案数
    cout << "circular perm(" << people << "," << people << ") = " << table.perm_circular(people) << endl;
    // 10 个人派 4 个人去圆桌上吃饭的排列总方案数
    cout << "circular perm(" << people << "," << need_people << ") = " << table.perm_circular(people, need_people) << endl;

    // 把 2 个苹果，3 个梨，5 个橘子排列的方案数
    int apple_cnt = 2;
    int pear_cnt = 3;
    int orange_cnt = 5;
    std::vector<int> items{apple_cnt, pear_cnt, orange_cnt};
    cout << "perm(apple * " << apple_cnt << ", pear * " << pear_cnt << ", orange * " << orange_cnt << ") = " << table.perm_multi(items.begin(), items.end()) << endl;
}
```

```
#输出如下
comb(10,4) = 16
perm(10,10) = 30
perm(10,4) = 93
nonadjacent perm(10,4) = 35
staggered perm(10,10) = 47
circular perm(10,10) = 3
circular perm(10,4) = 96
perm(apple * 2, pear * 3, orange * 5) = 95

```

