#include "DS/SortFHQTreap.h"
#include "IO/FastIO.h"

void test_normal_tree() {
    // 基础的排序平衡树可以实现打乱元素，然后查询每个位置的元素
    // 先给出一个长度为 10 的排列
    int A[10] = {11, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    // 这棵树 value_type = void, sum_type = void
    OY::SortFHQTreap<int> S(10, [&](int i) {
        return A[i];
    });
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    S.sort<false>(3, 6);
    cout << S << endl;

    // 访问每个下标的元素
    for (int i = 0; i < 10; i++) cout << S.get_node(i)->m_key << " \n"[i == 9];
    cout << endl;
}

void test_prod_tree() {
    // 区间排序平衡树可以轻易地维护区间乘积
    // 先给出一个长度为 10 的排列
    int A[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    // 不妨就令每个键 v 对应的值恰为键的十倍
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i] * 10; };
    // SumType 比 ValueType 高一级，但是可以互相转换
    auto S = OY::make_SortFHQ<uint32_t, int, int64_t, 1, std::less<uint32_t>, std::plus<int64_t>>(10, {}, key_mapping, info_mapping);
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl
         << endl;
}

void test_sum_tree() {
    // 区间排序平衡树可以轻易地维护区间和
    // 先给出一个长度为 10 的排列
    std::string A[10] = {"b", "a", "c", "d", "c", "a", "b", "d", "c", "a"};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    struct Monoid {
        using value_type = std::string;
        static value_type identity() { return ""; }
        static value_type op(value_type x, value_type y) { return x + y; }
    };
    // 平衡树支持非数字做元素
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i]; };
    OY::SORTFHQ::Tree<std::string, Monoid, std::less<std::string>> S(10, key_mapping, info_mapping);
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "sum(S[3~8]) = " << S.query(3, 8) << endl;

    cout << endl;
}

int main() {
    test_normal_tree();
    test_prod_tree();
    test_sum_tree();
}
/*
#输出如下
11 5 9 1 8 4 6 7 3 2
{{11}, {5}, {9}, {1}, {8}, {4}, {6}, {7}, {3}, {2}}
{{11}, {9, 8, 5, 1}, {4}, {6}, {7}, {3}, {2}}
{{11}, {9, 8}, {1, 4, 5, 6}, {7}, {3}, {2}}
11 9 8 1 4 5 6 7 3 2

10 5 9 1 8 4 6 7 3 2
{{10:100}, {5:50}, {9:90}, {1:10}, {8:80}, {4:40}, {6:60}, {7:70}, {3:30}, {2:20}}
prod(S[2~4]) = 181
{{10:100}, {9:90, 8:80, 5:50, 1:10}, {4:40}, {6:60}, {7:70}, {3:30}, {2:20}}
prod(S[2~4]) = 143
{{10:100}, {9:90}, {8:80}, {1:10, 4:40, 5:50, 6:60}, {7:70}, {3:30}, {2:20}}
prod(S[2~4]) = 132

b a c d c a b d c a
{{b:b}, {a:a}, {c:c}, {d:d}, {c:c}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = dcabdc
{{b:b}, {d:d, c:c, c:c, a:a}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = caabdc
{{b:b}, {d:d, c:c}, {a:a, a:a, b:b, c:c}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = aabcdc

*/