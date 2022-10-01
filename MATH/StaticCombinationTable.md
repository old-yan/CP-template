### 一、模板类别

​	数学：排列组合打表（静态模数）。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename _ModType` ，可以为 `uint32_t` 或者 `uint64_t` ，表示模数的类型。

   模板参数 `_ModType _P` ，表示模数。

   构造参数 `_ModType __n` ，表示打表范围。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   由于在排列组合问题中大量用到阶乘和阶乘的乘法逆元，所以在构造函数中对阶乘和阶乘的乘法逆元进行打表。

#### 2.查询组合数

1. 数据类型

   输入参数 `_ModType __n` ，表示从 `__n` 个元素中进行选取。

   输入参数 `_ModType __m` ，表示选取出 `__m` 个元素。

2. 时间复杂度

   $O(1)$ 。

#### 3.查询全排列

1. 数据类型

   输入参数 `_ModType __n` ，表示对 `__n` 个元素进行排列。

2. 时间复杂度

   $O(1)$ 。

#### 4.查询排列数

1. 数据类型

   输入参数 `_ModType __n` ，表示从 `__n` 个元素中进行选取、排列。

   输入参数 `_ModType __m` ，表示选取出 `__m` 个元素并排列。

2. 时间复杂度

   $O(1)$ 。

#### 5.查询多重集的全排列

1. 数据类型

   输入参数 `std::vector<_Tp> __ns` ，表示每种元素的数量。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法的含义为：如果输入 `[2,3,5]` ，表示查询有两个苹果，三个梨，五个橘子，总共有多少种排列方式。

#### 6.查询不相邻的组合数

1. 数据类型

   输入参数 `_ModType __n` ，表示从 `__n` 个元素中进行选取。

   输入参数 `_ModType __m` ，表示选取出 `__m` 个不相邻的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示从 `__n` 个元素中选取出 `__m` 个互不相邻的元素的方案数。

#### 7.全错排列

1. 数据类型

   输入参数 `_ModType __n` ，表示对 `__n` 个元素进行排列。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示对 `__n` 个元素进行排列后，每个元素都和自己之前的旧位置不同的方案数。

#### 8.圆上全排列

1. 数据类型

   输入参数 `_ModType __n` ，表示对 `__n` 个元素进行排列。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示对 `__n` 个圆上的相同元素进行排列的方案数。

#### 9.圆排列

1. 数据类型

   输入参数 `_ModType __n` ，表示从 `__n` 个元素中进行选取。

   输入参数 `_ModType __m` ，表示选取出 `__m` 个不相邻的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法表示从 `__n` 个圆上的相同元素中选取 `__m` 个进行排列的方案数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticCombinationTable.h"

int main() {
    OY::StaticCombinationTable32<97> table(20);
    int people = 10;
    int need_people = 4;
    //在 10 个人里抽出 4 个人，一起去参加某一场会议的方案数
    cout << "comb(" << people << "," << need_people << ") = " << table.comb(people, need_people) << endl;
    // 10 个人按序参加 10 场会议（每人一场）的方案数
    cout << "perm(" << people << "," << people << ") = " << table.perm(people) << endl;
    //在 10 个人里抽出 4 个人，按序参加 4 场会议（每人一场）的方案数
    cout << "perm(" << people << "," << need_people << ") = " << table.perm(people, need_people) << endl;
    //在 10 个人里抽出 4 个不相邻的人，一起去参加某一场会议的方案数
    cout << "nonadjacent perm(" << people << "," << need_people << ") = " << table.nonadjacentPerm(people, need_people) << endl;
    // 10 个人每个人有一个不想去参加的会议，则他们参加 10 场会议（每人一场）且皆大欢喜的总方案数
    cout << "staggered perm(" << people << "," << people << ") = " << table.staggeredPerm(people) << endl;
    // 10 个人在圆桌上吃饭的排列总方案数
    cout << "circular perm(" << people << "," << people << ") = " << table.circularPerm(people) << endl;
    // 10 个人派 4 个人去圆桌上吃饭的排列总方案数
    cout << "circular perm(" << people << "," << need_people << ") = " << table.circularPerm(people, need_people) << endl;

    //把 2 个苹果，3 个梨，5 个橘子排列的方案数
    int apple = 2;
    int pear = 3;
    int orange = 5;
    cout << "perm(apple * " << apple << ", pear * " << pear << ", orange * " << orange << ") = " << table.perm(std::vector{apple, pear, orange}) << endl;
}
```

```
#输出如下
comb(10,4) = 16
perm(10,10) = 30
perm(10,4) = 93
nonadjacent comb(10,4) = 35
staggered comb(10,10) = 47
circular perm(10,10) = 3
circular perm(10,4) = 96
perm(apple * 2, pear * 3, orange * 5) = 9

```

