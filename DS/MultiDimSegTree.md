### 一、模板类别

​	数据结构：任意维度线段树。

​	练习题目：

1. [Luck and Love](https://acm.hdu.edu.cn/showproblem.php?pid=1823)
2. [Check Corners](http://acm.hdu.edu.cn/showproblem.php?pid=2888)
3. [Mosaic](https://acm.hdu.edu.cn/showproblem.php?pid=4819)
4. [P3810 【模板】三维偏序（陌上花开）](https://www.luogu.com.cn/problem/P3810)

### 二、模板功能

#### 1.建立线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示各种索引下标类型。

   模板参数 `typename SizeType` ，表示高维点的坐标类型。

   模板参数 `typename CommutativeMonoid` ，表示维护的交换幺半群类型。

   模板参数 `typename BaseTable` ，表示基础表格类型。

   模板参数 `size_t DIM` ，表示维度。

   模板参数 `bool HasModify` ，表示是否有修改操作。

   构造参数 `SizeType point_cnt` ，表示预计要插入的结点数量。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   高维线段树处理的问题为单点修改，区域的查询。

   高维线段树通过小波树的封装，以及基础表格类型，建立起对高维空间区域内的信息维护。
   
   本模板通过模板参数 `typename CommutativeMonoid` 确定交换幺半群。交换幺半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

    本模板要求区间操作函数的运算符满足**结合律**和**交换律**。

   模板参数 `BaseTable` 要求支持 `resize` 和 `query` 方法。默认的，提供了 `OY::MDSEG::AdjTable` 和 `OY::MDSEG::SimpleBIT` 两种，分别作为不带修改和带修改情况下的基础表格。

   模板参数 `DIM` 可以支持任意大小，但是在维度较高时，本模板的时空效率会大幅衰减，所以不建议使用太高维度。特别的，要求 `DIM >= 2` 。

   构造参数 `point_cnt` 仅为即将插入的结点预留空间之用，即便传错或者不传，不会对模板正确性有影响。

#### 2.加入结点(add_point)

1. 数据类型

   输入参数 `value_type w` ，表示结点点权。

   输入参数 `Args... args` ，表示结点每个维度的坐标。要求传递的维度数恰好等于 `DIM` 。

2. 时间复杂度

    $O(1)$ 。


#### 3.预备(prepare)

1. 数据类型

2. 时间复杂度

   若 `BaseTable` 的 `resize` 操作为 $O(n)$ ，则为 $O(n\prod_2^D \log n)$ 。对第二维起的每个维度的坐标进行离散化，并对离散化之后的长度取对数，即为该维度对总复杂度的贡献因子。在坐标大量重复时，效率可以大幅上升。


#### 4.查询(query)

1. 数据类型

   输入参数 `Args... args` ，表示结点每个维度的下限和上限。下限和上限均为闭区间。要求传递的维度数恰好等于 `DIM` ，即参数数量为 `DIM * 2` 。

2. 时间复杂度

   若 `BaseTable` 的 `query` 操作为 $O(1)$ ，则为 $O(\prod_2^D \log n)$ 。对第二维起的每个维度的坐标进行离散化，并对离散化之后的长度取对数，即为该维度对总复杂度的贡献因子。在坐标大量重复时，效率可以大幅上升。


#### 5.单点修改(do_in_table)

1. 数据类型

   输入参数 `SizeType point_id​` ，表示要修改的结点的编号。

   输入参数 `Callback &&call` ，表示要对基础表格调用的回调函数。

2. 时间复杂度

   若调用的 `BaseTable` 的修改操作为 $O(1)$ ，则为 $O(\prod_2^D \log n)$ 。对第二维起的每个维度的坐标进行离散化，并对离散化之后的长度取对数，即为该维度对总复杂度的贡献因子。在坐标大量重复时，效率可以大幅上升。

3. 备注

   本方法仅当 `HasModify` 为 `true` 时可用。


### 三、模板示例

```c++
#include "DS/MultiDimSegTree.h"
#include "IO/FastIO.h"

void test() {
    cout << "test sum segtree(no modify):\n";
    OY::MonoSumMDST<int, int, 3> S;
    S.add_point(100, 1, 1, 1);
    S.add_point(1000, 3, 1, 2);
    S.add_point(10000, 2, -1, 3);
    S.prepare();
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl
         << endl;
}

void test_modify() {
    cout << "test sum segtree(with modify):\n";
    OY::MonoSumMDSeg<int, int, 3> S;
    S.add_point(100, 1, 1, 1);
    S.add_point(1000, 3, 1, 2);
    S.add_point(10000, 2, -1, 3);
    S.prepare();
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl;

    using bit = decltype(S)::base_table;
    // 修改第一个点的点权
    S.do_in_table(0, [](bit &tr, int pos) { tr.add(pos, 200); });
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl;

    // 修改第三个点的点权
    S.do_in_table(2, [](bit &tr, int pos) { tr.add(pos, 40000); });
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl
         << endl;
}

struct MaxTable {
    struct group {
        using value_type = int;
        static value_type op(value_type x, value_type y) { return x > y ? x : y; }
        static value_type identity() { return -999; }
    };
    std::vector<int> data;
    template <typename InitMapping>
    void resize(int len, InitMapping mapping) {
        data.resize(len);
        for (int i = 0; i < len; i++) data[i] = mapping(i);
    }
    void update(int pos, int val) {
        data[pos] = val;
    }
    int query(int left, int right) const {
        int val = -999;
        for (int i = left; i <= right; i++) val = std::max(val, data[i]);
        return val;
    }
};
void test_rangemax() {
    cout << "test max segtree(with modify):\n";
    // 此处可以看到，本模板可以把一维数据结构拿来维护高维
    OY::MonoMaxMDSeg<int, MaxTable, 3, true> S;
    S.add_point(100, 1, 1, 1);
    S.add_point(1000, 3, 1, 2);
    S.add_point(10000, 2, -1, 3);
    S.prepare();
    // 注意这里要改一下结合函数，以及初始值
    cout << "max of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl;

    // 修改第三个点的点权
    S.do_in_table(2, [](MaxTable &tr, int pos) { tr.update(pos, 6666); });
    cout << "max of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl;

    // 修改第三个点的点权
    S.do_in_table(2, [](MaxTable &tr, int pos) { tr.update(pos, 8000); });
    cout << "max of points[1~2][-1~1][1,3] = " << S.query(1, 2, -1, 1, 1, 3) << endl;
}

int main() {
    test();
    test_modify();
    test_rangemax();
}
```

```
#输出如下
test sum segtree(no modify):
sum of points[1~2][-1~1][1,3] = 10100

test sum segtree(with modify):
sum of points[1~2][-1~1][1,3] = 10100
sum of points[1~2][-1~1][1,3] = 10300
sum of points[1~2][-1~1][1,3] = 50300

test max segtree(with modify):
max of points[1~2][-1~1][1,3] = 10000
max of points[1~2][-1~1][1,3] = 6666
max of points[1~2][-1~1][1,3] = 8000

```

