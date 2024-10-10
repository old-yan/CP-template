### 一、模板类别

​	数据结构：树状数组

​	练习题目：

1. [940. 不同的子序列 II](https://leetcode.cn/problems/distinct-subsequences-ii)
2. [#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
3. [#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
4. [P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
5. [P3369 【模板】普通平衡树](https://www.luogu.com.cn/problem/P3369)
6. [P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
7. [P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)



### 二、模板功能

#### 1.建立树状数组

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树状数组中下标编号的变量类型。

   模板参数 `typename Tp` ，表示元素类型。

   模板参数 `template<typename>typename BufferType` ，表示内存分配策略。分为静态内存池分配和 `std::vector` 两种方案。

   构造参数 `size_type length` ，表示树状数组的覆盖范围为 `[0, length)`。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   树状数组处理的问题为单点增值修改，单点/区间和的查询。

   本树状数组模板与之前版本的重大区别为，不再支持乘法/除法或者其他类型的模板函数传递，转而要求树中元素类型支持加减法运算。如果确实有进行区间乘积维护的需求，使用者须自定义一个类作为元素类型。

   树状数组通过加和函数求解前缀和，再通过前缀和做差求解任意区间和。

   本构造方法要求元素支持 `+=` 运算符；区间查询要求支持 `-` 运算符；单点查询和 `kth` 要求支持 `-=` 运算符； `kth` 运算还要求支持 `<=` 运算符。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `Tp` 对象。如果不传递此参数，表示不进行初始化，比如要建立一颗空的求和树状数组，由于全局变量值本身就是零，所以无需进行初始化。


#### 2.建立树状数组

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到树状数组里。


#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示树状数组要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到树状数组里。
   
    `mapping` 要求传入一个下标，返回一个 `Tp` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。


#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到树状数组里。

   本方法要求元素支持 `+=` 运算符。

#### 5.单点增值(add)

1. 数据类型

   输入参数 `size_type i​` ，表示单点增值的下标。

   输入参数 `Tp inc​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 6.前缀和查询(presum)

1. 数据类型

   输入参数 `size_type i`，表示前缀和查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 7.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示单点查询的下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。
   
3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）

#### 9.查询全部和(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 10.查询第 k 个元素(kth)

1. 数据类型

   输入参数 `Tp k​` ，表示要查询的元素从小到大的顺次。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   仅当树状数组的元素类型 `Tp` 为数字，且每个位置的值均为非负时，本方法才有意义。

   也可以解释为，本方法在求可令 `presum(i) > k` 成立的最小的 `i` 。

   顺次 `k​` 表示第 `k + 1` 小，顺次 `0` 表示查询最小的元素。

   本函数没有进行参数检查，所以请自己确保 `k` 合法。（位于`[0，query_all())`）
   
#### 11.对所有位置的值调用回调(enumerate)

1. 数据类型

   输入参数 `Call &&call` ，表示在所有位置处要调用的回调。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   考虑将一颗树状数组的每个位置的值输出，则需要依次调用若干次 `query` 函数，时间复杂度需要带个对数。

   此时可调用本方法，时间复杂度不带对数。

### 三、模板示例

```c++
#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
 * 常规元素类型的单点修改树状数组
 */
void test1() {
    cout << "test of normal BIT(modify a single position):" << endl;
    // 当以长度进行构造的时候，树中元素均为默认初始值
    OY::VectorBIT<int> T(10);
    cout << T << endl;
    // 单点修改
    for (int i = 0; i < 10; i++) T.add(i, i * 100);
    cout << T << endl;
    // 输出前缀和
    cout << "presum(5) = " << T.presum(5) << endl;
    cout << "presum(3) = " << T.presum(3) << endl;
    // 输出单点值
    cout << "val(4) = " << T.query(4) << endl;
    cout << "val(5) = " << T.query(5) << endl;
    // 输出区间和
    cout << "sum(4 ~ 5) = " << T.query(4, 5) << endl;
    // 树中第 k 小元素查询
    cout << "kth(0) = " << T.kth(0) << endl;
    cout << "kth(99) = " << T.kth(99) << endl;
    cout << "kth(100) = " << T.kth(100) << endl;
    cout << "kth(299) = " << T.kth(299) << endl;
    cout << "kth(1499) = " << T.kth(1499) << endl;
    cout << "kth(1500) = " << T.kth(1500) << endl;
    cout << endl;
}

struct elem {
    // 假设我们这里的元素的加法重载为成员 val 的乘法
    // 那么减法就是 val 的除法
    double val;
    // 默认元素的值必须为 1
    elem(double _val = 1) : val(_val) {}
    elem operator-(const elem &rhs) const { return elem(val / rhs.val); }
    elem &operator+=(const elem &rhs) {
        val *= rhs.val;
        return *this;
    }
    elem &operator-=(const elem &rhs) {
        val /= rhs.val;
        return *this;
    }
    bool operator<=(const elem &rhs) const { return val <= rhs.val; }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const elem &x) {
        if (x.val < 1)
            return out << "1/" << int64_t(std::round(1 / x.val));
        else
            return out << int64_t(std::round(x.val));
    }
};
/*
 * 自定义类型的单点修改树状数组
 */
void custom_test1() {
    cout << "test of custom Tree(modify a single position):" << endl;
    elem arr[] = {1, 2, 1, 2, 1};
    // 从区间构造时，必须支持 += 运算符
    OY::VectorBIT<elem> T(arr, arr + 5);
    // 要想支持打印，必须支持 -= 运算符和流式输出
    cout << T << endl;
    // 要想支持单点修改，必须支持 += 运算符
    for (int i = 0; i < 5; i++) T.add(i, elem((i + 2) * 10));
    cout << T << endl;
    // 要想支持“前缀和”查询，必须支持 += 运算符
    cout << "presum(0, 4) = " << T.presum(4) << endl;
    // 要想支持单点查询，必须支持 -= 运算符
    cout << "val(3) = " << T.query(3) << endl;
    cout << "val(4) = " << T.query(4) << endl;
    // 要想支持区间查询，必须支持 += 和 -(减法) 运算符
    cout << "sum(3 ~ 4) = " << T.query(3, 4) << endl;
    // 对于本元素，kth 没有实际意义。硬要说的话，算是一个二分，即“至少到哪儿，才可以让前缀和超过一定的程度”
    // 要想支持 kth，必须支持 <= 和 += 运算符
    cout << "kth(599) = " << T.kth(599) << endl;
    cout << "kth(600) = " << T.kth(600) << endl;
    cout << "kth(23999) = " << T.kth(23999) << endl;
    cout << "kth(24000) = " << T.kth(24000) << endl;
    cout << endl;
}

void tricks() {
    // 在 oj 做题时，往往要把一个连续数组构建成树状数组
    // 如果先存到 vector 再存树状数组，未免有脱裤子放屁之嫌
    // 按我这么写即可在树状数组内逐个输入数据，不需要外来的 vector
    OY::VectorBIT<int64_t> tree_by_cin(0, [](uint32_t) {
        int64_t num;
        cin >> num;
        return num;
    });
}

int main() {
    test1();
    custom_test1();
    tricks();
}
```

```
#输出如下
test of normal BIT(modify a single position):
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 100, 200, 300, 400, 500, 600, 700, 800, 900]
presum(5) = 1500
presum(3) = 600
val(4) = 400
val(5) = 500
sum(4 ~ 5) = 900
kth(0) = 1
kth(99) = 1
kth(100) = 2
kth(299) = 2
kth(1499) = 5
kth(1500) = 6

test of custom Tree(modify a single position):
[1, 2, 1, 2, 1]
[20, 60, 40, 100, 60]
presum(0, 4) = 288000000
val(3) = 100
val(4) = 60
sum(3 ~ 4) = 6000
kth(599) = 1
kth(600) = 1
kth(23999) = 2
kth(24000) = 2

```

