### 一、模板类别

​	数据结构：种类并查集。

### 二、模板功能


#### 1.构造

1. 数据类型

   构造参数 `uint32_t __n` ，表示并查集中元素的数量，默认为 `0` 。

   构造参数 `uint32_t __kind` ，表示并查集中每个元素可能的种类数，默认为 `0` 。

2. 时间复杂度

   $O(n\cdot kind)$ 。

#### 2.重置

1. 数据类型

   输入参数 `uint32_t __n` ，表示并查集中元素的数量，默认为 `0` 。

   输入参数 `uint32_t __kind` ，表示并查集中每个元素可能的种类数，默认为 `0` 。

2. 时间复杂度

   $O(n\cdot kind)$ 。

#### 3.按照大小合并分组

1. 数据类型

   输入参数 `uint32_t __a`  和 `uint32_t __b` ，分别表示要合并的两个元素。

   输入参数 `uint32_t __aKind`  和 `uint32_t __bKind` ，分别表示要合并的两个元素的种类。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   $O(\alpha(n))$ 。
   
3. 备注

   表示将元素 `__a` 为 `__aKind` 时的分组和元素 `__b` 为 `__bKind` 时的分组按照组的大小合并，将小组合并到大组上。

   如果元素 `__a` 为 `__aKind` 时的分组和元素 `__b` 为 `__bKind` 时的分组本来就是同一分组，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 4.按照ID合并分组

1. 数据类型

   输入参数 `uint32_t __a`  和 `uint32_t __b` ，分别表示要合并的两个元素。

   输入参数 `uint32_t __aKind`  和 `uint32_t __bKind` ，分别表示要合并的两个元素的种类。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   $O(\alpha(n))$ 。
   
3. 备注

   表示将元素 `__a` 为 `__aKind` 时的分组和元素 `__b` 为 `__bKind` 时的分组按照组长的 `ID` 大小合并，将 `ID` 较大的组合并到 ` ID` 较小的组上。

   如果元素 `__a` 为 `__aKind` 时的分组和元素 `__b` 为 `__bKind` 时的分组本来就是同一分组，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 5.查询二者是否在同一分组

1. 数据类型：

   输入参数 `uint32_t __a`  和 `uint32_t __b` ，分别表示要查询的两个元素。

   输入参数 `uint32_t __aKind`  和 `uint32_t __bKind` ，分别表示要查询的两个元素的种类。

2. 时间复杂度

   $O(\alpha(n))$ 。

#### 6.获取所有分组名单

1. 数据类型

   返回值 `vector<vector<uint32_t>>`，表示所有的分组。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数以二维数组的形式返回所有的分组情况。


### 三、模板示例

```c++
#include "DS/KindUnionFind.h"
#include "IO/FastIO.h"

int main() {
    //假设有十个元素，每个元素可能为两个种类之一
    OY::KindUnionFind u(5,2);

    //给出一些条件
    //元素 0 和元素 1 的种类绑定相同
    u.uniteBySize(0,0,1,0);
    u.uniteBySize(0,1,1,1);
    //元素 0 和元素 2 的种类绑定相反
    u.uniteBySize(0,0,2,1);
    u.uniteBySize(0,1,2,0);
    //元素 3 和元素 4 的种类绑定相反
    u.uniteBySize(3,0,4,1);
    u.uniteBySize(3,1,4,0);

    //询问元素 1 和元素 2 的种类关系
    if(u.isSame(1,0,2,0) and u.isSame(1,1,2,1)){
        cout<<"1 and 2 is in the same kind\n";
    }
    else if(u.isSame(1,0,2,1) and u.isSame(1,1,2,0)){
        cout<<"1 and 2 is in different kind\n";
    }
    else{
        cout<<"1 and 2 has no relationship\n";
    }

    //询问元素 1 和元素 4 的种类关系
    if(u.isSame(1,0,4,0) and u.isSame(1,1,4,1)){
        cout<<"1 and 4 is in the same kind\n";
    }
    else if(u.isSame(1,0,4,1) and u.isSame(1,1,4,0)){
        cout<<"1 and 4 is in different kind\n";
    }
    else{
        cout<<"1 and 4 has no relationship\n";
    }
}
```

```
#输出如下
1 and 2 is in different kind
1 and 4 has no relationship

```

