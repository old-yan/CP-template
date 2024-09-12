### 一、模板类别

​	数据结构：可持久化压缩线段树。

​	练习题目：

1. [P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)

### 二、模板功能


​		本模板为 `PersistentSegTree` 在只有单点修改、没有区间修改情况下的特化。

​		本模板要求提供模板参数 `typename Monoid` 幺半群类型。幺半群中规定了元素类型、幺元的值、元素结合操作。除此外，仍需和 `SegTree` 一样提供 `typename SizeType` 参数和 `template <typename> typename BufferType` 参数。除此外，仍需和 `PersistentSegTree` 一样提供 `bool Lock` 参数， `typename SizeType` 参数和 `template <typename> typename BufferType` 参数。

​		为了方便使用，本模板提供了 `MinTree` ， `MaxTree` ， `GcdTree` ， `LcmTree` ， `BitAndTree` ， `BitOrTree` ， `SumTree` 七种特化形式，需要填写的参数更少。

#### 1.拷贝线段树(copy)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。

   **注意：**本函数名“拷贝”并不等价于上文中的数据拷贝。本函数只涉及根结点的拷贝；数据拷贝指的是从根到叶结点的链的拷贝。
   

### 三、模板示例

```c++
#include "DS/PersistentCompressedTree.h"
#include "IO/FastIO.h"

void test() {
    // PerCompressed 可以解决各种各样的可持久化问题
    OY::VectorPerCompressedMaxTree<int> S;
    S.modify(2, 100);
    cout << S << endl;

    auto S2 = S.copy();
    S2.modify(5, 60);
    S2.modify(1, 80);
    cout << S2 << endl;

    auto S3 = S2.copy();
    S3.modify(2, 90);
    cout << S3 << endl;
}

int main() {
    test();
}
```

```
#输出如下
[2:100]
[1:80,2:100,5:60]
[1:80,2:90,5:60]

```

