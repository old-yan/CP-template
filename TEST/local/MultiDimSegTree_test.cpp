#include "DS/MultiDimSegTree.h"
#include "IO/FastIO.h"

void test() {
    cout << "test sum segtree(no modify):\n";
    OY::Segtree3D<int, int, OY::MDSEG::AdjTable<int>, false> S;
    S.add_point(100, 1, 1, 1);
    S.add_point(1000, 3, 1, 2);
    S.add_point(10000, 2, -1, 3);
    S.prepare();
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(0, 1, 2, -1, 1, 1, 3) << endl;

    // 点权为 0 1 的可以把点权设为 bool
    OY::Segtree3D<int, bool, OY::MDSEG::AdjTable<int>, false> S2;
    S2.add_point(1, 1, 1);
    S2.add_point(3, 1, 2);
    S2.add_point(2, -1, 3);
    S2.prepare();
    cout << "cnt of points[1~2][-1~1][1,3] = " << S2.query(0, 1, 2, -1, 1, 1, 3) << endl
         << endl;
}

void test_modify() {
    cout << "test sum segtree(with modify):\n";
    using bit = OY::MDSEG::SimpleBIT<int>;
    OY::Segtree3D<int, int, bit, true> S;
    S.add_point(100, 1, 1, 1);
    S.add_point(1000, 3, 1, 2);
    S.add_point(10000, 2, -1, 3);
    S.prepare();
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(0, 1, 2, -1, 1, 1, 3) << endl;

    // 修改第一个点的点权
    S.do_in_table(0, [](bit &tr, int pos) { tr.add(pos, 200); });
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(0, 1, 2, -1, 1, 1, 3) << endl;

    // 修改第三个点的点权
    S.do_in_table(2, [](bit &tr, int pos) { tr.add(pos, 40000); });
    cout << "sum of points[1~2][-1~1][1,3] = " << S.query(0, 1, 2, -1, 1, 1, 3) << endl
         << endl;
}

struct MaxTable {
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
    OY::Segtree3D<int, int, MaxTable, true> S;
    S.add_point(100, 1, 1, 1);
    S.add_point(1000, 3, 1, 2);
    S.add_point(10000, 2, -1, 3);
    S.prepare();
    struct GetMax {
        int operator()(int x, int y) const { return x > y ? x : y; }
    };
    // 注意这里要改一下结合函数，以及初始值
    cout << "max of points[1~2][-1~1][1,3] = " << S.query<GetMax>(-999, 1, 2, -1, 1, 1, 3) << endl;

    // 修改第三个点的点权
    S.do_in_table(2, [](MaxTable &tr, int pos) { tr.update(pos, 6666); });
    cout << "max of points[1~2][-1~1][1,3] = " << S.query<GetMax>(-999, 1, 2, -1, 1, 1, 3) << endl;

    // 修改第三个点的点权
    S.do_in_table(2, [](MaxTable &tr, int pos) { tr.update(pos, 8000); });
    cout << "max of points[1~2][-1~1][1,3] = " << S.query<GetMax>(-999, 1, 2, -1, 1, 1, 3) << endl;
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
cnt of points[1~2][-1~1][1,3] = 2

test sum segtree(with modify):
sum of points[1~2][-1~1][1,3] = 10100
sum of points[1~2][-1~1][1,3] = 10300
sum of points[1~2][-1~1][1,3] = 50300

test max segtree(with modify):
max of points[1~2][-1~1][1,3] = 10000
max of points[1~2][-1~1][1,3] = 6666
max of points[1~2][-1~1][1,3] = 8000

*/