### 一、模板类别

​	数据结构：平衡二叉树中的 `Treap` 树

### 二、模板功能

​	平衡树的实现方法有差别，但是在使用的时候完全不需要知道实现的细节，只需要知道对外的接口。

#### 1.建立平衡树

1. 数据类型

   模板参数 `_typename _Tp` ，表示树中的键类型。

   模板参数 `_typename _Fp` ，表示树中的值类型。在 `set` 和 `multiset` 里不需要这个参数，将其设置为 `bool` 类型。

   模板参数 `typename _Compare` ，表示排序函数的类，默认为 `_Tp` 的小于号比较类。

   模板参数 `_Tag`，表示树的类型:

   1. `_Tag=TreapSetTag` 表示树模拟 `std::set` ；
   2. `_Tag=TreapMultisetTag` 表示树模拟 `std::multiset` ；
   3. `_Tag=TreapMapTag` 表示树模拟 `std::map` ；

   构造参数 `_Compare  __comp` ，表示具体的排序函数，默认为`_Compare` 类的默认实例。

2. 时间复杂度

   $O(1)$ 。

#### 2.设置内存池大小

1. 数据类型

   输入参数 `int __count`，表示程序运行最多用到的结点数量。

2. 时间复杂度

   $O(n)$

#### 3.清空

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


#### 4.插入键/键值对

1. 数据类型：

   参数 `_Tp __key`​ ，表示要插入的键。

   参数 `_Fp __value` ，表示要插入的值。本参数仅在 `map` 里用到。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   在 `set` 里如果键重复则不会插入；在 `map` 里如果键重复则不会插入。

#### 5.更新键值对

1. 数据类型：

   参数 `_Tp __key`​ ，表示要更新的键。

   参数 `_Fp __value` ，表示要更新的值。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数只适用于 `map` ；如果键不存在则会插入新的键值对。

#### 6.删除键

1. 数据类型：

   参数 `_Tp __key`​ ，表示要删除的键。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数适用于所有类型；如果允许键重复则只删除一个而非全部。

#### 7.删除键

1. 数据类型：

   参数 `_Tp __key`​ ，表示要删除的键。

   参数 `int __count` ，表示要删除的数量。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数只适用于 `multiset` ；如果不够则只删除能删的。

#### 8.获取排名

1. 数据类型：

   参数 `_Tp __key`​ ，表示要获取排名的键。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数的排名以 `0` 为基准；实际等同于树中小于该键值的元素数量。

#### 9.查找第 k 个元素

1. 数据类型：

   参数 `int __k` ，表示要查询的顺次。

   返回类型 `_TreapNode<_Tp, _Fp, _Tag>*` ，表示元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数的排名以 `0` 为基准。

#### 10.查找元素

1. 数据类型：

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_TreapNode<_Tp, _Fp, _Tag>*` ，表示元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

#### 11.二分下界

1. 数据类型：

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_TreapNode<_Tp, _Fp, _Tag>*` ，表示树中第一个大于等于 `__key` 的元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

#### 12.二分上界

1. 数据类型：

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_TreapNode<_Tp, _Fp, _Tag>*` ，表示树中第一个大于 `__key` 的元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

#### 13.二分下界的前驱

1. 数据类型：

   参数 `_Tp __key` ，表示要查询的键。

   返回类型 `_TreapNode<_Tp, _Fp, _Tag>*` ，表示树中最后一个小于 `__key` 的元素所在的位置。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   如果没找到，返回 `nullptr` 。

#### 14.查询树的元素数量

1. 数据类型：

2. 时间复杂度

   $O(1)$ 。

3. 备注

   查询树中元素数量，同元素可以计多次。

#### 15.查询树是否为空

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 16.查询某元素数量

1. 数据类型

   参数 `_Tp __key` ，表示要查询的元素。

2. 时间复杂度

   $O(\log n)$ 。

### 三、模板示例

```c++
#include "DS/Treap.h"
#include "IO/FastIO.h"

int main() {
    //Treap 的接口与替罪羊树接口一模一样
    
    //展示 set
    OY::TreapContainer::Set<std::string> set;
    set.insert("apple");
    set.insert("cat");
    set.insert("banana");
    cout << "cat rank = " << set.rank("cat") << endl;
    //插入相同元素不会成功
    set.insert("banana");
    cout << "cat rank = " << set.rank("cat") << endl;
    cout << "set size = " << set.size() << endl;

    auto p = set.find("banana");
    cout << "find pointer->" << p->key << endl;
    p = set.find("dog");
    cout << "find pointer->" << (p == nullptr ? "nullptr" : "something") << endl;

    cout << "smaller_bound of banana:" << set.smaller_bound("banana")->key << endl;
    cout << "lower_bound of banana:" << set.lower_bound("banana")->key << endl;
    cout << "upper_bound of banana:" << set.upper_bound("banana")->key << endl;

    set.clear();
    cout << "set size = " << set.size() << endl;

    //*************************************************************************
    //Multiset 和 std::multiset 差不多，但是功能强得多
    OY::TreapContainer::Multiset<int> multiset;
    multiset.insert(3);
    multiset.insert(3);
    multiset.insert(3);
    multiset.insert(3);
    multiset.insert(3);
    multiset.insert(4);
    multiset.insert(4);
    multiset.insert(5);
    multiset.insert(5);
    multiset.insert(5);
    cout << "multiset size  = " << multiset.size() << endl;
    cout << "multiset no.5  = " << multiset.kth(4)->key << endl;
    cout << "multiset no.6  = " << multiset.kth(5)->key << endl;
    cout << "multiset no.8  = " << multiset.kth(7)->key << endl;
    multiset.erase(3, 4);
    cout << "multiset size  = " << multiset.size() << endl;

    //Map没有什么好说的
    OY::TreapContainer::Map<int, std::string> map;
    map.insert(1, "first");
    map.insert(2, "second");
    map.insert(3, "third");
    cout << "map[2] ->" << map.find(2)->value << endl;
    map.update(2, "two");
    cout << "map[2] ->" << map.find(2)->value << endl;
}
```

```
#输出如下
cat rank = 2
cat rank = 2
set size = 3
find pointer->banana
find pointer->nullptr
smaller_bound of banana:apple
lower_bound of banana:banana
upper_bound of banana:cat
set size = 0
multiset size  = 10
multiset no.5  = 3
multiset no.6  = 4
multiset no.8  = 5
multiset size  = 6
map[2] ->second
map[2] ->two

```

