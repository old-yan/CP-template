### 一、模板类别

​	数据结构：可持久化平衡二叉树中的可持久化替罪羊树

### 二、模板功能

#### 1.建立平衡树

1. 数据类型

   模板参数 `_typename _Tp` ，表示树中的键类型。

   模板参数 `_typename _Fp` ，表示树中的值类型。在 `set` 和 `multiset` 里不需要这个参数，将其设置为 `bool` 类型。

   模板参数 `typename _Compare` ，表示排序函数的类，默认为 `_Tp` 的小于号比较类。

   模板参数 `_Tag`，表示树的类型:

   1. `_Tag=PersistentScapeGoatTreeSetTag` 表示树模拟 `std::set` ；
   2. `_Tag=PersistentScapeGoatTreeMultisetTag` 表示树模拟 `std::multiset` ；
   3. `_Tag=PersistentScapeGoatTreeMapTag` 表示树模拟 `std::map` ；
   
   构造参数 `_Compare  __comp` ，表示具体的排序函数，默认为`_Compare` 类的默认实例。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   可持久化平衡树可以用于对某个时间的树的状态的查询、修改。

   时间维度，常被称为“版本”，初始化后的版本号为 `0` ，对应一棵空树。以后每次有修改时自动向后生成一个版本号。

    `ScapeGoat` 常常被用来可持久化的树，而且其效率在可持久化平衡树中排位靠前。

   **注意：**以下所有方法传参时都可以用 `-1` 来表示**上一个版本**的版本号。

#### 2.设置内存池大小

1. 数据类型

   输入参数 `int __count`，表示程序运行最多用到的结点数量。

2. 时间复杂度

   $O(n)$

#### 3.清空

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   调用本函数会将平衡树置为空树，并将之前的合并信息重置。版本号重新从 `0` 开始计。


#### 4.插入键/键值对

1. 数据类型：

   参数 `int __prevVersion` ，表示以哪个版本为基础进行修改。

   参数 `_Tp __key`​ ，表示要插入的键。

   参数 `_Fp __value` ，表示要插入的值。本参数仅在 `map` 里用到。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   在 `set` 里如果键重复则不会插入；在 `map` 里如果键重复则不会插入。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 5.更新键值对

1. 数据类型：

   参数 `int __prevVersion` ，表示以哪个版本为基础进行修改。

   参数 `_Tp __key`​ ，表示要更新的键。

   参数 `_Fp __value` ，表示要更新的值。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数只适用于 `map` ；如果键不存在则会插入新的键值对。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 6.删除键

1. 数据类型：

   参数 `int __prevVersion` ，表示以哪个版本为基础进行修改。

   参数 `_Tp __key`​ ，表示要删除的键。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数适用于所有类型；如果允许键重复则只删除一个而非全部。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 7.获取排名

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

   参数 `_Tp __key`​ ，表示要获取排名的键。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数的排名以 `0` 为基准；实际等同于树中小于该键值的元素数量。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 9.查找第 k 个元素

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

   参数 `int __k` ，表示要查询的顺次。

   返回类型 `_PersistentScapeGoatNode<_Tp, _Fp, _Tag>*` ，表示元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数的排名以 `0` 为基准。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 10.查找元素

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_PersistentScapeGoatNode<_Tp, _Fp, _Tag>*` ，表示元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 11.二分下界

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_PersistentScapeGoatNode<_Tp, _Fp, _Tag>*` ，表示树中第一个大于等于 `__key` 的元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 12.二分上界

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_PersistentScapeGoatNode<_Tp, _Fp, _Tag>*` ，表示树中第一个大于 `__key` 的元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 13.二分下界的前驱

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_PersistentScapeGoatNode<_Tp, _Fp, _Tag>*` ，表示树中最后一个小于 `__key` 的元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 14.查询树的元素数量

1. 数据类型：

   参数 `int __version` ，表示查询哪个版本。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   查询树中元素数量，同元素可以计多次。

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

#### 15.查询树是否为空

1. 数据类型

   参数 `int __version` ，表示查询哪个版本。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）


#### 16.查询某元素数量

1. 数据类型

   参数 `int __version` ，表示查询哪个版本。

   参数 `_Tp __key` ，表示要查询的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保版本号合法。（位于 `[-1, versionCount())` ）

### 三、模板示例

```c++
#include "DS/PersistentScapeGoat.h"
#include "IO/FastIO.h"

int main() {
    //可持久化平衡树的用法和平衡树基本相同，除了增加了版本信息
    //此处只展示 multiset 的用法
    
    //建立一棵 int 的树
    OY::PersistentScapeGoatContainer::Multiset<int>T;
    T.insert(0,10);
    T.insert(1,20);
    T.insert(2,30);
    cout<<"version count="<<T.versionCount()<<endl;
    //输出最新版本的所有元素
    for(int i=0;i<T.size(-1);i++)
        cout<<T.kth(-1, i)->key<<" ";
    cout<<endl;

    //建立一棵 string 的树
    OY::PersistentScapeGoatContainer::Multiset<std::string>T_str;
    T_str.insert(0,std::string("apple"));
    T_str.insert(1,std::string("banana"));
    T_str.insert(1,std::string("cat"));
    T_str.insert(1,std::string("dog"));
    cout<<"version count="<<T_str.versionCount()<<endl;
    //输出最新版本的所有元素
    for(int i=0;i<T_str.size(-1);i++)
        cout<<T_str.kth(-1, i)->key<<" ";
    cout<<endl;

    //建立一颗从大到小的 int 树
    OY::PersistentScapeGoatContainer::Multiset<int,std::greater<int>>T_rev;
    T_rev.insert(0,10);
    T_rev.insert(1,20);
    T_rev.insert(2,30);
    T_rev.erase(2,20);
    T_rev.insert(4,70);
    T_rev.insert(5,40);
    cout<<"version count="<<T_rev.versionCount()<<endl;
    //输出最新版本的所有元素
    for(int i=0;i<T_rev.size(-1);i++)
        cout<<T_rev.kth(-1, i)->key<<" ";
    cout<<endl;
    cout<<"upper_bound of 40  ="<<T_rev.upper_bound(-1,40)->key<<endl;
    cout<<"lower_bound of 40  ="<<T_rev.lower_bound(-1,40)->key<<endl;
    cout<<"smaller_bound of 40="<<T_rev.smaller_bound(-1,40)->key<<endl;
}
```

```
#输出如下
version count=4
10 20 30 
version count=5
apple dog 
version count=7
70 40 10 
upper_bound of 40  =10
lower_bound of 40  =40
smaller_bound of 40=70

```

