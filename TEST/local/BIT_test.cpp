#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
 * 常规元素类型的单点修改树状数组
 */
void test1() {
    cout << "test of normal BIT(modify a single position):" << endl;
    // 当以长度进行构造的时候，树中元素均为默认初始值
    OY::BIT32<false, 1000> T(10);
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

/*
 * 常规元素类型的区间增值修改树状数组
 */
void test2() {
    cout << "test of normal BIT(modify a range):" << endl;
    // 可以以区间来构造
    OY::BIT64<false, 1000> T;
    int64_t arr[7] = {4, 9, 2, 3, 5, 8, 7};
    T.reset(arr, arr + 7);
    cout << T << endl;

    // 如果 RangeUpdate 为 true ，则可以进行区间增值
    OY::BIT64<true, 1000> T2(arr, arr + 7);
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
 * 自定义类型的单点修改树状数组
 */
void custom_test1() {
    cout << "test of custom Tree(modify a single position):" << endl;
    elem arr[] = {1, 2, 1, 2, 1};
    // 从区间构造时，必须支持 += 运算符
    OY::BIT::Tree<elem, false, 1000> T(arr, arr + 5);
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

/*
 * 自定义类型的区间增值修改树状数组
 */
void custom_test2() {
    cout << "test of custom Tree(modify a range):" << endl;
    elem arr2[] = {4, 9, 2, 3, 5, 7, 8};
    // 从区间构造，必须支持 -(减法)， -(负号)，+= 和 * 运算符
    OY::BIT::Tree<elem, true, 1000> T(arr2, arr2 + 7);
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
    OY::BIT64<false, 1000> tree_by_cin(0, [](uint32_t) {
        int64_t num;
        cin >> num;
        return num;
    });
}

int main() {
    test1();
    test2();
    custom_test1();
    custom_test2();
    tricks();
}
/*
#输出如下
test of normal BIT(modify a single position):
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 0, 0, 0, 0, 0, 0]
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

test of normal BIT(modify a range):
[4, 9, 2, 3, 5, 8, 7, 0]
[4, 9, 2, 3, 5, 8, 7, 0]
[4, 1009, 1002, 1003, 1005, 1008, 7, 0]

test of custom Tree(modify a single position):
[1, 2, 1, 2, 1, 1, 1, 1]
[20, 60, 40, 100, 60, 1, 1, 1]
presum(0, 4) = 288000000
val(3) = 100
val(4) = 60
sum(3 ~ 4) = 6000
kth(599) = 1
kth(600) = 1
kth(23999) = 2
kth(24000) = 2

test of custom Tree(modify a range):
[4, 9, 2, 3, 5, 7, 8, 1]
[40, 90, 20, 30, 50, 7, 8, 1]
[40, 90, 20, 30, 50, 70, 80, 10]
presum(6) = 604800000000
presum(4) = 108000000
val(5) = 70
val(6) = 80
sum(5 ~ 6) = 5600
kth(3599) = 1
kth(3600) = 2
kth(71999) = 2
kth(72000) = 3

*/