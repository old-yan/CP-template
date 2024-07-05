### 一、模板类别

​	数据结构：笛卡尔树

​	练习题目：

1. [P5854 【模板】笛卡尔树](https://www.luogu.com.cn/problem/P5854)
2. [Cartesian Tree](https://judge.yosupo.jp/problem/cartesian_tree)(https://github.com/yosupo06/library-checker-problems/issues/554)

### 二、模板功能

​		本模板实际上并非是一个数据结构，而是一个函数。

#### 1.建立笛卡尔树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示数组长度、下标的表示类型。

   模板参数 `typename Tp` ，表示输入的数组的元素类型。

   输入参数 `size_type length` ，表示数组长度。

   输入参数 `InitMapping mapping` ，表示从下标到数组的值的映射函数。

   输入参数 `LchildCallback &&lchild_call` ，表示当找到一个元素的左孩子时的回调函数。

   输入参数 `RchildCallback &&rchild_call` ，表示当找到一个元素的右孩子时的回调函数。 

   输入参数 `Compare comp` ，表示数组元素间的比较函数。
   
   输入参数 `const Tp &max` ，表示数组元素类型的无穷大。至少应当和数组内的最大元素一样大。

   返回类型 `size_type` ，表示最终生成的树的根结点的编号。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   以下讨论，假设比较函数是小于号函数，笛卡尔树维护区间最大值。

   笛卡尔树作为二叉树，满足以下两个性质：

   1. 树的中序遍历，就是原区间顺序；

   2. 相邻的元素中，较小的元素总是较大的元素的子孙。

   显然，要做到这一点，我们要首先挑出整个区间中的最大元素作为根，同时将整个区间划分成两半；在分割出的子区间里，继续挑出最大元素，作为根的左/右孩子，同时将子区间继续分割成两半...

   笛卡尔树，可以间接用来解决区间最值查询。可以是区间最大值、区间最小值。通过上述分析，可以得知，区间最值就是区间最左元素和最右元素在树中的 `LCA` 结点的值。


#### 2.建立笛卡尔树

1. 数据类型

   输入参数 `Iterator first` ，表示输入数组的区间头。

   输入参数 `Iterator last` ，表示输入数组的区间尾。（开区间）

   其余同上。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   同上。

### 三、模板示例

```c++
#include "DS/CartesianTree.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个值越大越靠近根的笛卡尔树
    auto print_lchild = [](uint32_t i, int lchild) {
        if (lchild == -1)
            cout << i << " has no lchild" << endl;
        else
            cout << "lchild of " << i << " = " << lchild << endl;
    };
    auto print_rchild = [](uint32_t i, int rchild) {
        if (rchild == -1)
            cout << i << " has no rchild" << endl;
        else
            cout << "rchild of " << i << " = " << rchild << endl;
    };
    // 通过回调函数，我们可以观测到建的树
    auto root_max = OY::Cartesian::solve<int>(10, [&](uint32_t i) { return A[i]; }, print_lchild, print_rchild);
    cout << "root_max = " << root_max << endl;

    // 如果我们要建一棵值越小越靠近根的树呢？
    // 而且我们还想把左孩子、右孩子什么的都存下来
    std::vector<int> arr_lchild(10), arr_rchild(10), arr_parent(10);
    auto get_lchild = [&](uint32_t i, int lchild) {
        arr_lchild[i] = lchild;
        if (lchild != -1) arr_parent[lchild] = i;
    };
    auto get_rchild = [&](uint32_t i, int rchild) {
        arr_rchild[i] = rchild;
        if (rchild != -1) arr_parent[rchild] = i;
    };
    auto root_min = OY::Cartesian::solve(std::begin(A), std::end(A), get_lchild, get_rchild, std::greater<int>(), 0);
    cout << "arr_lchild:";
    for (int i = 0; i < 10; i++)
        cout << arr_lchild[i] << (i == 9 ? '\n' : ' ');
    cout << "arr_rchild:";
    for (int i = 0; i < 10; i++)
        cout << arr_rchild[i] << (i == 9 ? '\n' : ' ');
    cout << "root_min = " << root_min << endl;
}
```

```
#输出如下
11 5 9 12 8 4 6 15 7 7
0 has no lchild
1 has no lchild
1 has no rchild
lchild of 2 = 1
2 has no rchild
rchild of 0 = 2
lchild of 3 = 0
4 has no lchild
5 has no lchild
5 has no rchild
lchild of 6 = 5
6 has no rchild
rchild of 4 = 6
rchild of 3 = 4
lchild of 7 = 3
8 has no lchild
9 has no lchild
9 has no rchild
rchild of 8 = 9
rchild of 7 = 8
root_max = 7
arr_lchild:-1 0 -1 -1 2 1 -1 -1 7 -1
arr_rchild:-1 4 3 -1 -1 6 8 -1 9 -1
root_min = 5

```

