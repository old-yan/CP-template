### 一、模板类别

​	数据结构：`Prufer` 序列。

### 二、模板功能

#### 1.构造图

1. 数据类型

   构造参数 `uint32_t __vertexNum`​ ，表示树中的点数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

    `Prufer` 序列处理的问题是，在无根树和数列之间建立一对一的对应关系。一个含有 `n` 个结点的无根树，可以唯一对应一个 `n-2` 长度的数列。此处 `n >= 2` 。

   本模板提供将一颗无根树转化为数列，以及从数列反推树形态的功能。
   
   **注意：** 本模板中，结点编号均从 `0` 开始。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示边的一端的结点编号。

   输入参数 `uint32_t __b` ，表示边的另一端的结点编号。

2. 时间复杂度

   $O(1)$ 。


#### 3.编码为 Prufer 序列

1. 数据类型

   返回类型 `std::vector<uint32_t>` ，表示编码得到的数列。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法可以将加完边的 `n` 个结点的无根树，转化为 `n - 2` 长度的数列。

#### 4.从 Prufer 序列解码

1. 数据类型

   输入参数 `_Iterator __first` ，表示 `Prufer` 序列开始位置。

   输入参数 `_Iterator __last` ，表示 `Prufer` 序列结束位置。（开区间）

   返回类型 `std::vector<uint32_t>` ，表示得到的树。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本方法可以从长度为 `n - 2` 的数列，恢复出 `n` 个结点的无根树。返回形式是，若结点 `n - 1` 为根，则返回值表示每一个结点的父结点编号（根的父结点为 `-1` ）。
   
   任意的数列都可以推出唯一的一棵树，且任意树一定能由某数列推出。所以大小为 `n` 的无根树的总可能数是 `pow(n, n-2)` 。

### 三、模板示例

```c++
#include "GRAPH/Prufer.h"
#include "IO/FastIO.h"

int main() {
    OY::Prufer tree(5);
    tree.addEdge(0, 3);
    tree.addEdge(3, 1);
    tree.addEdge(4, 3);
    tree.addEdge(4, 2);
    // 编码
    auto code = tree.encode();
    cout << "prufer code:";
    for (auto a : code) cout << ' ' << a;
    cout << endl;

    // 解码
    auto parent_arr = OY::Prufer::decode(code.begin(), code.end());
    for (int i = 0; i < parent_arr.size(); i++)
        if (parent_arr[i] != -1)
            cout << "node " << i << "'s parent: node " << parent_arr[i] << endl;
}
```

```
#输出如下
prufer code: 3 3 4
node 0's parent: node 3
node 1's parent: node 3
node 2's parent: node 4
node 3's parent: node 4

```

