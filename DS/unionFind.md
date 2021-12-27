### 一、模板类别

​	数据结构：并查集

### 二、模板功能

#### 1.建立并查集

1. 数据类型

   构造参数 `int n`​ ，表示并查集大小。默认值为 `1<<20` 。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   并查集处理的问题为分组合并问题。初始化的时候，给每个数分配一个单独的分组，所以每个数所在分组的首领都是它自己，且分组大小都是 `1`。


#### 2.重置

1. 数据类型

   输入参数 `int n` ，表示并查集要处理的元素个数。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   调用本函数会将并查集大小改变，并将之前的合并信息重置。

#### 3.查询分组首领

1. 数据类型

   参数 `int i` ，表示要寻找元素 `i` 所在分组的首领。

2. 时间复杂度

   $O(\alpha (n))$  。

#### 4.查询分组大小

1. 数据类型

   参数 `int i` ，表示要查询元素 `i` 所在分组的大小。

2. 时间复杂度

   $O(\alpha (n))$ 。


#### 5.将...合并到...

1. 数据类型

   输入参数 `int headA`  和 `int headB` ，分别为各自分组的首领。

2. 时间复杂度

   $O(\alpha(n))$ 。
   
3. 备注

   表示将元素 `headA`  所率领的分组合并到元素 `headB` 所率领的分组。
   
   本函数的两个参数是有向的，不能随便交换。如果想在主代码中控制合并的方向，本函数是最好的选择。

#### 6.按照大小合并分组

1. 数据类型

   输入参数 `int a`  和 `int b` 。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   $O(\alpha(n))$ 。
   
3. 备注

   表示将元素 `a` 和元素 `b` 所在的分组按照组的大小合并，将小组合并到大组上。

   如果 `a` 和 `b` 本来就在同一分组中，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 7.按照ID合并分组

1. 数据类型

   输入参数 `int a`  和 `int b` 。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   $O(\alpha(n))$ 。
   
3. 备注

   表示将元素 `a` 和元素 `b` 所在的分组按照组长的 `ID` 大小合并，将 `ID` 较大的组合并到 ` ID` 较小的组上。

   如果 `a` 和 `b` 本来就在同一分组中，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 8.查询二者是否在同一分组

1. 数据类型：

   输入参数 `int a`  和 `int b` 。表示查询元素 `a` 和元素 `b` 是否位于同一个分组。

2. 时间复杂度

   $O(\alpha(n))$ 。

#### 9.查询某元素是否为分组首领

1. 数据类型

   参数 `int i` ，表示查询元素 `i` 是否是自己分组的首领。

2. 时间复杂度

   $O(1)$ 。

#### 10.获取所有分组首领名单

1. 数据类型

   返回值 `vector<int>`，表示所有的分组首领。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数按照 `ID` 升序返回所有的分组首领。

#### 11.获取所有分组名单

1. 数据类型

   返回值 `vector<vector<int>>`，表示所有的分组。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数以二维数组的形式返回所有的分组情况。

### 三、模板示例

```c++
#include "DS/unionFind.h"
#include "IO/FastIO.h"

int main() {
    // 建立大小为 10 的并查集
    OY::unionFind u(10);

    //查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.isSame(5, 6) ? "true" : "false") << endl;

    u.uniteByID(2, 5);
    u.uniteByID(3, 6);
    u.uniteByID(1, 9);
    u.uniteByID(7, 8);
    u.uniteByID(2, 3);
    //查询 6 所在的分组首领
    cout << "6 is now in whose group:" << u.find(6) << endl;
    //查询 6 所在的分组大小
    cout << "6's group size is:      " << u.size(6) << endl;
    //查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.isSame(5, 6) ? "true" : "false") << endl;

    auto heads = u.heads();
    for (int a : heads)
        cout << a << " is a head\n";

    auto groups = u.groups();
    for (int i = 0; i < groups.size(); i++) {
        cout << "No." << i + 1 << " group's member:";
        for (int a : groups[i])
            cout << a << ' ';
        cout << endl;
    }
}
```

```
#输出如下
5 and 6 in same group?  false
6 is now in whose group:2
6's group size is:      4
5 and 6 in same group?  true
0 is a head
1 is a head
2 is a head
4 is a head
7 is a head
No.1 group's member:0 
No.2 group's member:1 9 
No.3 group's member:2 3 5 6 
No.4 group's member:4 
No.5 group's member:7 8 

```

