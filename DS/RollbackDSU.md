### 一、模板类别

​	数据结构：回滚并查集

​	练习题目：

1. [P3402 可持久化并查集](https://www.luogu.com.cn/problem/P3402)
2. [U208135 可持久化并查集 加强版](https://www.luogu.com.cn/problem/U208135)
3. [Persistent Unionfind](https://judge.yosupo.jp/problem/persistent_unionfind)(https://github.com/yosupo06/library-checker-problems/issues/405)


### 二、模板功能

#### 1.建立并查集

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   构造参数 `size_type n`​ ，表示并查集大小。默认值为 `0` 。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   并查集处理的问题为分组合并问题。初始化的时候，给每个数分配一个单独的分组，所以每个数所在分组的首领都是它自己，且分组大小都是 `1`。
   
   通过路径压缩实现的并查集可以有更优秀的时间复杂度，但是如果不做压缩，就可以实现可回滚的并查集；可以通过调用 `cancle` 方法撤销最后添加的边。


#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type n` ，表示并查集要处理的元素个数。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   调用本函数会将并查集大小改变，并将之前的合并信息重置。

#### 3.查询分组首领(find)

1. 数据类型

   参数 `size_type i` ，表示要寻找元素 `i` 所在分组的首领。

2. 时间复杂度

   均摊 $O(\log n)$  。

#### 4.查询分组大小(size)

1. 数据类型

   模板参数 `bool IsHead` ，表示是否保证输入的参数为分组首领。

   参数 `size_type i` ，表示要查询元素 `i` 所在分组的大小。

2. 时间复杂度

   均摊 $O(\log n)$  。
   
3. 备注

   当 `IsHead` 参数为 `true` 时，无需对元素 `i` 进行 `find` 操作，所以复杂度为 $O(1)$ 。


#### 5.将前者合并到后者(unite_to)

1. 数据类型

   模板参数 `bool MakeRecord` ，表示是否把本次加边操作记录。默认为 `true` 。

   输入参数 `size_type head_a`  和 `size_type head_b` ，分别为各自分组的首领。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   表示将元素 `head_a`  所率领的分组合并到元素 `head_b` 所率领的分组。
   
   本函数的两个参数是有向的，不能随便交换。如果想在主代码中控制合并的方向，本函数是最好的选择。
   
   **注意：**
   
   如果 `MakeRecord` 参数为 `true` ，本次操作会被记录，之后可以通过调用无参的 `cancle` 方法将本次操作撤销；如果 `MakeRecord` 参数为 `false` ，本次操作不会被本数据结构记录，如果还想将本次操作撤销，需要在本数据结构之外记录本次操作，调用两个参数的 `cancle` 方法将本次操作撤销。

#### 6.按照大小合并分组(unite_by_size)

1. 数据类型

   模板参数 `bool MakeRecord` ，表示是否把本次加边操作记录。默认为 `true` 。

   输入参数 `size_type a`  和 `size_type b` 。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   均摊 $O(\log n)$  。
   
3. 备注

   表示将元素 `a` 和元素 `b` 所在的分组按照组的大小合并，将小组合并到大组上。

   如果 `a` 和 `b` 本来就在同一分组中，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 7.查询二者是否在同一分组(in_same_group)

1. 数据类型：

   输入参数 `size_type a`  和 `size_type b` 。表示查询元素 `a` 和元素 `b` 是否位于同一个分组。

2. 时间复杂度

   均摊 $O(\log n)$  。

#### 8.查询某元素是否为分组首领(is_head)

1. 数据类型

   参数 `size_type i` ，表示查询元素 `i` 是否是自己分组的首领。

2. 时间复杂度

   $O(1)$ 。

#### 9.查询分组数量(count)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 10.获取所有分组首领名单(heads)

1. 数据类型

   返回值 `vector<size_type>`，表示所有的分组首领。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数按照 `ID` 升序返回所有的分组首领。

#### 11.获取所有分组名单(groups)

1. 数据类型

   返回值 `vector<vector<size_type>>`，表示所有的分组。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数以二维数组的形式返回所有的分组情况。

### 三、模板示例

```c++
#include "DS/RollbackDSU.h"
#include "IO/FastIO.h"

int main() {
    /**
     * 本模板的其余功能与普通并查集类似，所以只展示本模板特有的功能
     */
    // 建立大小为 10 的并查集
    OY::RollbackDSU::Table u(10);

    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    u.unite_by_size(2, 5);
    u.unite_by_size(3, 6);
    u.unite_by_size(6, 5);
    // 2 和 3 的连接失败，所以不做保存
    u.unite_by_size(2, 3);
    u.unite_by_size(6, 8);
    // 查询 6 所在的分组大小
    cout << "6's group size is:      " << u.size(6) << endl;
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    // 撤销 6 和 8 的连接
    u.cancle();
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    // 撤销 6 和 5 的连接
    u.cancle();
    // 查询 6 所在的分组大小
    cout << "6's group size is:      " << u.size(6) << endl;
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;
}
```

```
#输出如下
5 and 6 in same group?  false
6's group size is:      5
5 and 6 in same group?  true
5 and 6 in same group?  true
6's group size is:      2
5 and 6 in same group?  false

```

