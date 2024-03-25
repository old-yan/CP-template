### 一、模板类别

​	数据结构：哈希树状数组

​	练习题目：

1. [940. 不同的子序列 II](https://leetcode.cn/problems/distinct-subsequences-ii)
2. [#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
3. [#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
4. [P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
5. [P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
6. [P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
7. [P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)



### 二、模板功能

#### 1.建立树状数组

1. 数据类型

   模板参数 `typename KeyType` ，表示树状数组的下标类型。

   模板参数 `typename MappedType` ，表示树状数组的值类型。

   模板参数 `size_type L`​ ，表示哈希表的结点数为 `1 << L` 。

   模板参数 `bool MakeRecord` ，表示是否对哈希表里插入过的结点进行记录。

   构造参数 `size_type length` ，表示树状数组的覆盖范围为 `[0, length)`。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本模板处理的问题为单点增值修改，单点/区间和的查询。

   本模板完全为简化版的树状数组。


#### 2.单点增值(add)

1. 数据类型

   输入参数 `KeyType i​` ，表示单点增值的下标。

   输入参数 `const MappedType &inc​` ，表示增量大小。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 3.前缀和查询(presum)

1. 数据类型

   输入参数 `KeyType i`，表示前缀和查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 4.单点查询(query)

1. 数据类型

   输入参数 `KeyType i` ，表示单点查询的下标。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 5.区间查询(query)

1. 数据类型

   输入参数 `KeyType left​` ，表示区间查询的开头下标。

   输入参数 `KeyType right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   均摊 $O(\log n)$ 。
   
3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）


#### 6.查询全部和(query_all)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ 。

#### 7.查询第 k 个元素(kth)

1. 数据类型

   输入参数 `MappedType k​` ，表示要查询的元素从小到大的顺次。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   可以解释为，本方法在求可令 `presum(i) > k` 成立的最小的 `i` 。

   顺次 `k​` 表示第 `k + 1` 小，顺次 `0` 表示查询最小的元素。

   本函数没有进行参数检查，所以请自己确保 `k` 合法。（位于`[0，query_all())`）
#### 8.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(n)$ ，此处 `n` 指此前树状数组操作涉及的结点数。

3. 备注

   本方法仅当 `MakeRecord` 为 `true` 时使用。


### 三、模板示例

```c++
#include "DS/GlobalHashBIT.h"
#include "IO/FastIO.h"

void test() {
    // 本模板完全等同于普通树状数组
    OY::GHashBIT<uint64_t, int64_t, 10, true> Gbit(100000000000000);
    Gbit.add(100, 9999);
    Gbit.add(1000000100, -99999);
    cout << Gbit.query(99, 1000000100) << endl;
    Gbit.clear();
    cout << Gbit.query_all() << endl;
}

int main() {
    test();
}
```

```
#输出如下
-90000
0


```

