### 一、模板类别

​	数据结构： `01` 字典树。

### 二、模板功能


#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标类型。
   
   模板参数 `typename Tp` ，表示字典树中的数字类型，默认为 `uint32_t` 。

   模板参数 `size_type L` ，表示对每个数字的 `[0, L)` 位，从高位到低位进行考虑。默认为 `30` 。
   
   模板参数 `typename Info` ，表示结点维护的信息的类。默认为 `DefaultInfo` 。 

   模板参数 `size_type MAX_NODE` ，表示节点内存池大小。默认为 `1<<20` 。 

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `01` 字典树本质和线段树一样，无非是在树中下降时，走左路还是走右路的选择比较自由。基于此，本数据结构中设计了迭代器 `iterator` 这一树中的类，通过迭代器可以较为方便地在树中上下移动、读写结点值、进行各种自由操作。
   

#### 2.插入元素

1. 数据类型

   输入参数 `Tp number` ，表示要插入的数字。

   返回类型 `iterator` ，表示插入元素后的结果点。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。。

#### 3.查询最大相同值

1. 数据类型

   输入参数 `Tp number` ，表示要查询的数字。

   返回类型 `std::pair<iterator, Tp>` ，前者为适配的叶结点，后者为适配情况下的逐位判定相同与否的 `mask` 。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。

#### 4.查询最大按位异或

1. 数据类型

   输入参数 `Tp number` ，表示要查询的数字。

   返回类型 `std::pair<iterator, _Tp>` ，前者为适配的叶结点，后者为适配情况下的按位异或值。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。

#### 5.查找某个叶结点信息

1. 数据类型

   输入类型 `iterator leaf` ，表示要查询的叶结点。

   返回类型 `Tp` ，表示该叶结点对应的数字。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

### 三、模板示例

```c++
#include "DS/BiTrie.h"
#include "IO/FastIO.h"

int main() {
    OY::BiTrie<uint32_t, 5> S;

    // 插入数字
    S.insert(15);
    S.insert(1);
    S.insert(9);

    // 插入数字的返回值是一个迭代器，可以拿来做操作
    auto insert_leaf = S.insert(14);
    insert_leaf->m_isEnd = true;

    // 查询和 5 有最大相同值的数
    auto res1 = S.query_max_same(5);
    auto leaf1 = res1.first;
    auto mask1 = res1.second;
    cout << "5 and " << S.query_leaf_value(leaf1) << " 's common bitmask :" << mask1 << endl;
    // 查询最大异或
    auto res2 = S.query_max_bitxor(5);
    auto leaf2 = res2.first;
    auto mask2 = res2.second;
    cout << "5 and " << S.query_leaf_value(leaf2) << " 's bitxor result :" << mask2 << endl;
}
```

```
#输出如下
5 and 1 's common bitmask :27
5 and 9 's bitxor result :12

```

