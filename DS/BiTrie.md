### 一、模板类别

​	数据结构： `01` 字典树。

### 二、模板功能

​	`BiTrie` 建立在 `Trie` 的基础上。 `BiTrie` 使用类内静态函数进行初始化、插入数字、查找树中最大异或点等操作。实际上，`BiTrie` 就是一颗动态开点线段树，只不过需要一些特定的接口。能用 `BiTrie` 解决的问题往往可以用动态开点线段树解决。


#### 1.初始化

1. 数据类型

   模板参数 `typename _Tp` ，表示字典树中的数字类型，默认为 `uint32_t` 。

   模板参数 `uint8_t _L` ，表示对每个数字的 `[0, _L)` 位，从高位到低位进行考虑。默认为 `30` 。
   
   模板参数 `typename _Info` ，表示结点维护的信息的类。默认为 `BiTrieDefaultInfo` 。 

   模板参数 `uint32_t _PoolSize` ，表示节点内存池大小。默认为 `2000000` 。 

2. 时间复杂度

   $O(1)$ 。
   

#### 2.插入元素

1. 数据类型

   输入参数 `const BiTrieIteration<_Tp,_L>& __number` ，表示要插入的数字。

   返回类型 `TrieNode` ，表示插入元素后的结果点。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。。

3. 备注

   传递的参数可以是 `_Tp` 类型。

   插入顺序为，从前往后逐个插入字符。

#### 3.查询最大相同值

1. 数据类型

   输入参数 `_Tp __number` ，表示要查询的数字是 `__number` 。

   返回类型 `std::pair<TrieNode, _Tp>` ，前者为适配的叶结点，后者为适配情况下的逐位判定相同与否的 `mask` 。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。

#### 4.查询最大按位异或

1. 数据类型

   输入参数 `_Tp __number` ，表示要查询的数字是 `__number` 。

   返回类型 `std::pair<TrieNode, _Tp>` ，前者为适配的叶结点，后者为适配情况下的按位异或值。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

3. 备注

   调用此函数时，默认树中含有元素。

#### 5.查找某个叶结点信息

1. 数据类型

   输入类型 `TrieNode __leaf` ，表示要查询的叶结点。

   返回类型 `_Tp` ，表示该叶结点对应的数字。

2. 时间复杂度

   $O(L)$ ，即关注的二进制长度。

### 三、模板示例

```c++
#include "DS/BiTrie.h"
#include "IO/FastIO.h"

int main() {
    //可以用静态方法初始化，而无需实例对象
    OY::BiTrie<uint32_t, 5>::Init();

    //可以用静态方法插入数字
    OY::BiTrie<uint32_t, 5>::Insert(15);
    OY::BiTrie<uint32_t, 5>::Insert(1);
    OY::BiTrie<uint32_t, 5>::Insert(9);
    OY::BiTrie<uint32_t, 5>::Insert(14);

    //查询和 5 有最大相同值的数
    auto [leaf, mask] = OY::BiTrie<uint32_t, 5>::QueryMaxSame(5);
    cout << "5 and " << OY::BiTrie<uint32_t, 5>::QueryLeafValue(leaf) << " 's common bitmask :" << mask << endl;
    //查询最大异或
    auto [leaf2, mask2] = OY::BiTrie<uint32_t, 5>::QueryMaxBitxor(5);
    cout << "5 and " << OY::BiTrie<uint32_t, 5>::QueryLeafValue(leaf2) << " 's bitxor result :" << mask2 << endl;
}
```

```
#输出如下
5 and 1 's common bitmask :27
5 and 9 's bitxor result :12

```

