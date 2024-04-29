### 一、模板类别

​	数据结构：`Prufer` 序列。

​	练习题目：

1. [P6086 【模板】Prufer 序列](https://www.luogu.com.cn/problem/P6086)

### 二、模板功能

#### 1.构造树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

    `Prufer` 序列处理的问题是，在无根树和数列之间建立一对一的对应关系。一个含有 `n` 个结点的无根树，可以唯一对应一个 `n-2` 长度的数列。此处 `n >= 2` 。

   本模板提供将一颗无根树转化为数列，以及从数列反推树形态的功能。
   
   **注意：** 本模板中，结点编号均从 `0` 开始。
   
   **注意：**本模板要求点数大于 `2` 。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新树。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

2. 时间复杂度

   $O(1)$ 。


#### 4.编码(encode)

1. 数据类型

   返回类型 `std::vector<size_type>` ，表示编码得到的序列。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   得到的编码序列长度为 `n-2` ；可能的序列有 `n^(n-2)` 种。

#### 5.解码(decode)

1. 数据类型

   输入参数 `Iterator first` ，表示 `Prufer` 序列开始位置。

   输入参数 `Iterator last` ，表示 `Prufer` 序列结束位置。（开区间）

   返回类型 `std::vector<size_type>` ，表示得到的树。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法为静态方法。

   得到的解码序列长度为 `n` 。解码得到的无根树，若以 `n` 为根，则序列的第 `i` 个元素 `a[i]`表示结点 `i` 的父结点编号； `a[n-1]` 必为 `-1` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/Prufer.h"

int main() {
    OY::PRUFER::Tree tree(5);
    tree.add_edge(0, 3);
    tree.add_edge(3, 1);
    tree.add_edge(4, 3);
    tree.add_edge(4, 2);
    // 编码
    auto code = tree.encode();
    cout << "prufer code:";
    for (auto a : code) cout << ' ' << a;
    cout << endl;

    // 解码
    auto parent_arr = OY::PRUFER::Tree::decode(code.begin(), code.end());
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

