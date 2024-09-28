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
/*
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

*/