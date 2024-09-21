#include "DS/LichaoSegTree.h"
#include "IO/FastIO.h"

int main() {
    // 默认的李超线段树使用斜率和截距描述线段
    OY::LCSEG::Tree<> T(16, {});
    T.add(8, 10, {1.5, -7});
    T.add(2, 6, {0.25, 5.5});
    T.add(4, 6, {0, 7});

    cout << "x=2, max(Y)=" << T.query(2).calc(2) << endl;
    cout << "x=4, max(Y)=" << T.query(4).calc(4) << endl;
    // x=7 没有被添加的线段覆盖，所以返回的是默认值
    cout << "x=7, max(Y)=" << T.query(7).calc(7) << endl;
    cout << "x=8, max(Y)=" << T.query(8).calc(8) << endl;

    // 使用 LichaoSlopeZkwTree 创建斜率和截距为非 double 类型的树
    OY::VectorLichaoSlopeChmaxSegTree<long long> T2(16, {});
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
    OY::LCSEG::Tree<line> T3(16, {});
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
    OY::LCSEG::Tree<line2, compare> T4(16, line2{0, INT_MAX, "default"});
    T4.add(0, 15, {1, 0, "one"});
    T4.add(0, 15, {0, 4, "two"});
    T4.add(0, 15, {-1, 8, "three"});
    T4.add(0, 15, {-2, 12, "four"});
    cout << "x=4, lowest line'name=" << T4.query(4).name << endl;
}
/*
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

*/