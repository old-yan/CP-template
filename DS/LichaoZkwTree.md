### 一、模板类别

​	数据结构：李超线段树

​	练习题目：

1. [P4097 【模板】李超线段树 / [HEOI2013] Segment](https://www.luogu.com.cn/problem/P4097)
2. [P4254 [JSOI2008] Blue Mary 开公司](https://www.luogu.com.cn/problem/P4254)
3. [P4655 [CEOI2017] Building Bridges](https://www.luogu.com.cn/problem/P4655)

### 二、模板功能

#### 1.建立线段树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示描述横坐标大小的类型。

   模板参数 `typename Line` ，表示线段的类型，默认值为 `OY::LCZKW::BaseLine<double>` 。

   模板参数 `typename Compare` ，表示比较函数的类，默认值为 `OY::LCZKW::BaseLess<Line>` 。

   构造参数 `size_type length` ，表示线段树的覆盖范围为 `[0, length)​`。

   构造参数 `const Line &default_line` ，表示元素默认值，默认为 `Line` 类的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   李超线段树处理的问题为，给出多个一元一次函数，查询单点的函数最值。

   元素类型限定为**线段**，但是李超线段树可以解决大部分**直线**问题，只要把自变量定义域按照问题范围进行截取即可。

   李超线段树需要提供线段数据类型，以及比较函数。在一般情况下可以使用默认的类型 `OY::LCZKW::BaseLine<double>` 及默认比较类 `OY::LCZKW::Less` 。`OY::LCZKW::BaseLine<Tp>`由两个参数组成，`Tp m_k` 表示斜率，`Tp m_b` 表示截距；`OY::LCZKW::Less` 等同于 `std::less<void>`，参数一和参数二为 `Line.calc(i)`  的结果类型，当参数一小于参数二时返回 `true`，否则返回 `false` 。

   使用自定义的线段数据类型也是可以的，具体见范例。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示将李超线段树的覆盖范围重置为 `[0, length)`。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   重置可以改变李超线段树的覆盖范围，并重置这个范围的值。

#### 3.添加竖直线段(add)

1. 数据类型

   输入参数 `size_type i` ，表示线段的下标

   输入参数 `const Line& line` ，表示要添加的线段。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   对于一条竖直的线段，输入一个横坐标，对应的纵坐标是不确定的。在此情况下，推荐把最高点作为对应的点。比如，以斜率式添加线段时，可以令斜率为 `0` ，截距设为最高点的纵坐标。

#### 4.添加非竖直线段(add)

1. 数据类型

   输入参数 `size_type left` ，表示线段的起始下标

   输入参数 `size_type right` ，表示线段的结尾下标。（闭区间）

   输入参数 `const Line& line` ，表示要添加的线段。

2. 时间复杂度

   $O(\log^2 n)$ ，特别的，如果添加的线段全部为直线（也就是覆盖整个区间），时间复杂度为 $O(\log n)$ 。

3. 备注

   本函数的时间复杂度本质上取决于插入的线段区间会不会被分成很多个 `2` 的幂长度的小区间。比如，区间 `[256, 320)` 只会被分成一块长度为 `64` 的区间；区间 `[9, 15)` 会被分割成四块：`[9, 10), [10, 12), [12，14), [14, 15)` 。（为什么 `[10, 12)` 和 `[12, 14)` 不能合并？留待思考）完整易分的区间越多，时间复杂度越接近 $O(\log n)$ 。

#### 5.单点查询最优线段(query)

1. 数据类型

   输入参数 `size_type i` ，表示单点查询的下标。

   返回类型 `Line` ，表示在当前点上，能取得最大值的线段。

2. 时间复杂度

   $O(\log n)$ 。

### 三、模板示例

```c++
#include "DS/LichaoZkwTree.h"
#include "IO/FastIO.h"

int main() {
    // 默认的李超线段树使用斜率和截距描述线段
    OY::LCZKW::Tree<> T(16);
    T.add(8, 10, {1.5, -7});
    T.add(2, 6, {0.25, 5.5});
    T.add(4, 6, {0, 7});

    cout << "x=2, max(Y)=" << T.query(2).calc(2) << endl;
    cout << "x=4, max(Y)=" << T.query(4).calc(4) << endl;
    // x=7 没有被添加的线段覆盖，所以返回的是默认值
    cout << "x=7, max(Y)=" << T.query(7).calc(7) << endl;
    cout << "x=8, max(Y)=" << T.query(8).calc(8) << endl;

    // 使用 LichaoSlopeZkwTree 创建斜率和截距为非 double 类型的树
    OY::LichaoSlopeChmaxZkwTree<long long> T2(16);
    T2.add(8, 10, {150, -700});
    T2.add(2, 6, {25, 550});
    T2.add(4, 6, {0, 700});

    cout << "x=2, max(Y)=" << T2.query(2).calc(2) << endl;
    cout << "x=4, max(Y)=" << T2.query(4).calc(4) << endl;
    cout << "x=7, max(Y)=" << T2.query(7).calc(7) << endl;
    cout << "x=8, max(Y)=" << T2.query(8).calc(8) << endl;

    //*****************************************************************************
    // 如果要添加自定义线段，需要给线段提供calc方法，才能使用 OY::LichaoZkwLess 比较方法
    struct line {
        int k;
        int b;
        std::string name;
        int calc(int i) const { return k * i + b; }
    };
    OY::LCZKW::Tree<line> T3(16);
    T3.add(8, 10, {1, -7, "apple"});
    T3.add(8, 10, {2, -10, "banana"});
    T3.add(8, 10, {-1, 10, "cat"});
    cout << "x=9, highest line'name=" << T3.query(9).name << endl;

    //*****************************************************************************
    // 如果上面的 line 的比较的内容不能满足你的需求，你可以重写 calc 的返回值
    // 然后重写比较函数
    // 比如这一次要求：首先选择函数值比较低的；在函数值并列的时候，选择名字字典序最小的线段
    struct line2 {
        int k;
        int b;
        std::string name;
        std::pair<int, std::string> calc(int i) const { return {k * i + b, name}; }
    };
    struct compare {
        bool operator()(const std::pair<int, std::string> &x, const std::pair<int, std::string> &y) const {
            if (x.first != y.first)
                return x.first > y.first;
            else
                return x.second > y.second;
        }
    };
    // 注意，如果要比较的线段都位于 x 轴上方，那么默认线段需要设得高一些
    OY::LCZKW::Tree<line2, compare> T4(16, line2{0, INT_MAX, "default"});
    T4.add(0, 15, {1, 0, "one"});
    T4.add(0, 15, {0, 4, "two"});
    T4.add(0, 15, {-1, 8, "three"});
    T4.add(0, 15, {-2, 12, "four"});
    cout << "x=4, lowest line'name=" << T4.query(4).name << endl;
}
```

```
#输出如下
x=2, max(Y)=6.000000
x=4, max(Y)=7.000000
x=7, max(Y)=0.000000
x=8, max(Y)=5.000000
x=2, max(Y)=600
x=4, max(Y)=700
x=7, max(Y)=0
x=8, max(Y)=500
x=9, highest line'name=banana
x=4, lowest line'name=four

```

