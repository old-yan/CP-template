### 一、模板类别

​	数据结构：可区间加的树状数组

​	练习题目：


1. [#132. 树状数组 3 ：区间修改，区间查询](https://loj.ac/p/132)
2. [P3372 【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)
3. [U187320 【模板】树状数组 3](https://www.luogu.com.cn/problem/U187320)



### 二、模板功能

​		本模板为 `BIT` 模板的加强版，支持区间加。

​		本模板对于参数的要求，与 `BIT` 的要求基本相同。

​		本模板要求元素支持 `+=` 、 `-=` 、 `-` 运算符； `kth` 运算要求支持 `<=` 和 `+` 运算符。

### 三、模板示例

```c++
#include "DS/BIT_ex.h"
#include "IO/FastIO.h"

/*
 * 常规元素类型的区间增值修改树状数组
 */
void test1() {
    cout << "test of normal BIT(modify a range):" << endl;
    int64_t arr[7] = {4, 9, 2, 3, 5, 8, 7};
    
    OY::VectorBIT_ex<int64_t> T2(arr, arr + 7);
    cout << T2 << endl;
    T2.add(1, 5, 1000);
    cout << T2 << endl;
    cout << endl;
}

struct elem {
    // 假设我们这里的元素的加法重载为成员 val 的乘法
    // 那么减法就是 val 的除法
    double val;
    // 默认元素的值必须为 1
    elem(double _val = 1) : val(_val) {}
    elem operator+(const elem &rhs) const { return elem(val * rhs.val); }
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
    elem operator-() const { return elem(1 / val); }
    elem operator*(uint32_t x) const {
        elem res;
        while (x--) res += *this;
        return res;
    }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const elem &x) {
        if (x.val < 1)
            return out << "1/" << int64_t(std::round(1 / x.val));
        else
            return out << int64_t(std::round(x.val));
    }
};
/*
 * 自定义类型的区间增值修改树状数组
 */
void custom_test1() {
    cout << "test of custom Tree(modify a range):" << endl;
    elem arr[] = {4, 9, 2, 3, 5, 7, 8};
    // 从区间构造，必须支持 -(减法)， -(负号)，+= 和 * 运算符
    OY::VectorBIT_ex<elem> T(arr, arr + 7);
    // 要想支持打印，必须支持 += 运算符和流式输出
    cout << T << endl;
    // 要想支持单点修改，必须支持 -(负号)，+= 和 * 运算符
    for (int i = 0; i < 5; i++) T.add(i, 10);
    cout << T << endl;
    // 要想支持区间修改，必须支持 -(负号)，+= 和 * 运算符
    T.add(5, 7, elem(10));
    cout << T << endl;
    // 要想支持“前缀和”查询，必须支持 -(负号)，+= 和 * 运算符
    cout << "presum(6) = " << T.presum(6) << endl;
    cout << "presum(4) = " << T.presum(4) << endl;
    // 要想支持单点查询，必须支持 += 运算符
    cout << "val(5) = " << T.query(5) << endl;
    cout << "val(6) = " << T.query(6) << endl;
    // 要想支持区间查询，必须支持 -(减号)，+= 和 * 运算符
    cout << "sum(5 ~ 6) = " << T.query(5, 6) << endl;
    // 对于本元素，kth 没有实际意义。硬要说的话，算是一个二分，即“至少到哪儿，才可以让前缀和超过一定的程度”
    // 要想支持 kth，必须支持 <= ， + ， -(减号)，+= 和 * 运算符
    cout << "kth(3599) = " << T.kth(3599) << endl;
    cout << "kth(3600) = " << T.kth(3600) << endl;
    cout << "kth(71999) = " << T.kth(71999) << endl;
    cout << "kth(72000) = " << T.kth(72000) << endl;
    cout << endl;
}

void tricks() {
    // 在 oj 做题时，往往要把一个连续数组构建成树状数组
    // 如果先存到 vector 再存树状数组，未免有脱裤子放屁之嫌
    // 按我这么写即可在树状数组内逐个输入数据，不需要外来的 vector
    OY::VectorBIT_ex<int64_t> tree_by_cin(0, [](uint32_t) {
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
test of normal BIT(modify a range):
[4, 9, 2, 3, 5, 8, 7]
[4, 1009, 1002, 1003, 1005, 1008, 7]

test of custom Tree(modify a range):
[4, 9, 2, 3, 5, 7, 8]
[40, 90, 20, 30, 50, 7, 8]
[40, 90, 20, 30, 50, 70, 80]
presum(6) = 604800000000
presum(4) = 108000000
val(5) = 70
val(6) = 80
sum(5 ~ 6) = 5600
kth(3599) = 1
kth(3600) = 2
kth(71999) = 2
kth(72000) = 3

```

