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
/*
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

*/