### 一、模板类别

​	数据结构：随机数。

### 二、模板功能

​	本模板可以构造随机的树。

#### 1.构造树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `typename Tree` ，表示想要构造的树的类型。

   输入参数 `size_type vertex_cnt` ，表示希望构造的树有几个结点。

   输入参数 `Generator &&gen` ，表示随机数生成器，默认为 `std::mt19937()` 。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   任何一个结点数大于 `2` 的树，都和一个 `n^(n-2)` 范围内的数字一一对应，或者说和一个元素取值范围在 `[0, n)` ，长度为 `n-2` 的可重序列一一对应。所以，本方法只是先产生一个这样的序列，再把这个序列转化为对应的树结构。

   本方法一般不需要贴到 `oj` ，只是为了本地测试方便。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/RandTree.h"
#include "TREE/VectorTree.h"

int main() {
    using Tree = OY::VectorTree::Tree<bool>;

    auto T = OY::RandTree::make_tree<Tree>(10);

    T.set_root(4);

    cout << T << endl;
}
```

```
# gcc/clang输出如下
[4[9[1[3][6]][7][2[8[0][5]]]]]

```

```
# msvc输出如下
[4[2[0][3]][7][1[9[5[6][8]]]]]

```

