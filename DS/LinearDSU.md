### 一、模板类别

​	数据结构：线性并查集

​	练习题目：

1. [1488. 避免洪水泛滥](https://leetcode.cn/problems/avoid-flood-in-the-city/description/)
2. [P4145 上帝造题的七分钟 2 / 花神游历各国](https://www.luogu.com.cn/problem/P4145)


### 二、模板功能

#### 1.建立并查集

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。
   
   类型设定 `mask_type = uint64_t` ，表示用来实现的掩膜类型。

   构造参数 `size_type n`​ ，表示并查集大小。默认值为 `0` 。

2. 时间复杂度

   $O(\frac n w)$ 。

3. 备注

   并查集处理的问题为分组合并问题。初始化的时候，给每个数分配一个单独的分组，所以每个数所在分组的首领都是它自己，且分组大小都是 `1`。
   
   当处理一般问题时，并查集的复杂度往往并非线性，而是额外带有一个对数或者阿克曼函数；当然，运行速度并不慢。
   
   在只有相邻元素才会发生合并的场景下，并查集的复杂度可以优化到线性。
   
   本模板与普通并查集的一大不同是，本模板的每个连通块一定是一个连续不间断的区间，而且本模板将每个连通块的“首领”视为区间内的最后一个元素。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type n` ，表示并查集要处理的元素个数。

2. 时间复杂度

   同上。
   
3. 备注

   调用本函数会将并查集大小改变，并将之前的合并信息重置。

#### 3.查询分组首领(find_tail)

1. 数据类型

   参数 `size_type i` ，表示要寻找元素 `i` 所在分组的首领（尾元素）。

2. 时间复杂度

   均摊 $O(1)$  。

#### 4.查询下一个不同分组的元素(find_next)

1. 数据类型

   参数 `size_type i` ，表示要寻找元素 `i` 所在分组的下一个分组的首个元素。

2. 时间复杂度

   均摊 $O(1)$  。


#### 5.将前者合并到后者(unite_after)

1. 数据类型

   输入参数 `size_type i` ，表示要把 `i` 和 `i+1` 合并。

2. 时间复杂度

   $O(1)$ 。


#### 6.查询二者是否在同一分组(in_same_group)

1. 数据类型：

   输入参数 `size_type a`  和 `size_type b` 。表示查询元素 `a` 和元素 `b` 是否位于同一个分组。

2. 时间复杂度

   均摊 $O(1)$  。

#### 7.查询某元素是否为分组首领(is_tail)

1. 数据类型

   参数 `size_type i` ，表示查询元素 `i` 是否是自己分组的首领（尾元素）。

2. 时间复杂度

    $O(1)$ 。

#### 8.查询分组数量(count)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 9.获取所有分组首领名单(tails)

1. 数据类型

   返回值 `vector<size_type>`，表示所有的分组首领（尾元素）。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数按照 `ID` 升序返回所有的分组首领（尾元素）。

### 三、模板示例

```c++
#include "DS/LinearDSU.h"
#include "IO/FastIO.h"

int main() {
    // 建立大小为 10 的并查集
    OY::LDSU::Table u(10);

    // 查询 3 和 6 的关系
    cout << "3 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    u.unite_after(3);
    u.unite_after(5);
    u.unite_after(8);
    u.unite_after(4);
    u.unite_after(6);
    // 查询 6 所在的分组首领
    cout << "6 is now in whose group:" << u.find_tail(6) << endl;
    // 查询 3 所在的分组首领
    cout << "3 is now in whose group:" << u.find_tail(3) << endl;
    // 查询 5 和 6 的关系
    cout << "3 and 6 in same group?  " << (u.in_same_group(3, 6) ? "true" : "false") << endl;

    auto tails = u.tails();
    for (uint32_t a : tails)
        cout << a << " is a tail\n";
}
```

```
#输出如下
3 and 6 in same group?  false
6 is now in whose group:7
3 is now in whose group:7
3 and 6 in same group?  true
0 is a tail
1 is a tail
2 is a tail
7 is a tail
9 is a tail

```

