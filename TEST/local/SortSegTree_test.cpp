#include "DS/SortSegTree.h"
#include "IO/FastIO.h"

void test_normal_tree() {
    // 基础的排序线段树可以实现打乱元素，然后查询每个位置的元素

    // 先给出一个长度为 10 的数列
    int A[10] = {11, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序线段树
    OY::SortSegTree<int> S(10, [&](int i) { return A[i]; }, 11);
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    S.sort<false>(3, 6);
    cout << S << endl;

    // 访问每个下标的元素
    for (int i = 0; i < 10; i++)
        cout << S.get_node(i)->key() << " \n"[i == 9];
    cout << endl;
}

void test_sum_tree() {
    // 区间排序线段树可以轻易地维护区间和
    // 先给出一个长度为 10 的排列
    int A[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序线段树
    // 不妨就令每个键 v 对应的值恰为键的一百倍
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i] * 100; };
    auto S = OY::make_SortSeg<uint32_t, int, 0, std::plus<int>>(10, {}, key_mapping, info_mapping, 10);
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "sum(S[2~4]) = " << S.query(2, 4) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "sum(S[2~4]) = " << S.query(2, 4) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "sum(S[2~4]) = " << S.query(2, 4) << endl
         << endl;
}

void test_mul_tree() {
    // 加法信息具有交换律和结合律，所以维护起来比较简单
    // 如果遇到没有交换律的，比如字符串加法，可以自己写个 Monoid

    int A[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序线段树
    struct Monoid {
        using value_type = std::string;
        static value_type identity() { return ""; }
        static value_type op(value_type x, value_type y) { return x + y; }
    };
    // 不妨就令每个键 v 对应的字符恰为 'a'+v
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return std::string(1, 'a' + A[i]); };
    OY::SORTSEG::Tree<uint32_t, Monoid> S(10, key_mapping, info_mapping, 10);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl
         << endl;
}

int main() {
    test_normal_tree();
    test_sum_tree();
    test_mul_tree();
}
/*
#输出如下
11 5 9 1 8 4 6 7 3 2
{{11}, {5}, {9}, {1}, {8}, {4}, {6}, {7}, {3}, {2}}
{{11}, {9, 8, 5, 1}, {4}, {6}, {7}, {3}, {2}}
{{11}, {9, 8}, {1, 4, 5, 6}, {7}, {3}, {2}}
11 9 8 1 4 5 6 7 3 2

10 5 9 1 8 4 6 7 3 2
{{10:1000}, {5:500}, {9:900}, {1:100}, {8:800}, {4:400}, {6:600}, {7:700}, {3:300}, {2:200}}
sum(S[2~4]) = 1800
{{10:1000}, {9:900, 8:800, 5:500, 1:100}, {4:400}, {6:600}, {7:700}, {3:300}, {2:200}}
sum(S[2~4]) = 1400
{{10:1000}, {9:900}, {8:800}, {1:100, 4:400, 5:500, 6:600}, {7:700}, {3:300}, {2:200}}
sum(S[2~4]) = 1300

10 5 9 1 8 4 6 7 3 2
{{10:k}, {5:f}, {9:j}, {1:b}, {8:i}, {4:e}, {6:g}, {7:h}, {3:d}, {2:c}}
sum(S[3~8]) = bieghd
{{10:k}, {9:j, 8:i, 5:f, 1:b}, {4:e}, {6:g}, {7:h}, {3:d}, {2:c}}
sum(S[3~8]) = fbeghd
{{10:k}, {9:j, 8:i}, {1:b, 4:e, 5:f, 6:g}, {7:h}, {3:d}, {2:c}}
sum(S[3~8]) = befghd

*/