#include "DS/SortFHQTreap.h"
#include "IO/FastIO.h"

void test_normal_tree() {
    // 基础的排序平衡树可以实现打乱元素，然后查询每个位置的元素
    // 先给出一个长度为 10 的排列
    int A[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    OY::SortFHQ::Tree<int, void, std::less<int>, OY::SortFHQ::MAINTAIN_NONE, 1000> S(10, [&](int i) {
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
    for (int i = 0; i < 10; i++) cout << S.get_node(i)->get() << " \n"[i == 9];
    cout << endl;
}

void test_sum_tree() {
    // 区间排序平衡树可以轻易地维护区间和
    // 先给出一个长度为 10 的排列
    std::string A[10] = {"b", "a", "c", "d", "c", "a", "b", "d", "c", "a"};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立区间排序平衡树
    // 平衡树支持非数字做元素
    auto key_mapping = [&](int i) { return A[i]; };
    auto info_mapping = [&](int i) { return A[i]; };
    OY::SortFHQ::Tree<std::string, std::string, std::less<std::string>, OY::SortFHQ::MAINTAIN_RANGE_REVERSE, 1000> S(10, key_mapping, info_mapping, "");
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

struct XorClass {
    int val;
    XorClass() = default;
    XorClass(int val) : val(val) {}
    XorClass operator+(const XorClass &rhs) const { return {val ^ rhs.val}; }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const XorClass &x) { return out << x.val; }
};
void test_xor_tree() {
    // 维护其他的运算，就需要创建新类
    // 比如想维护异或和，那就创建一个加号执行异或的类
    int keys[10] = {10, 5, 9, 1, 8, 4, 6, 7, 3, 2};
    XorClass infos[10] = {1000, 500, 900, 100, 800, 400, 600, 700, 300, 200};

    // 建立区间排序平衡树
    // 不妨就令每个键 v 对应的值恰为 100 * v
    auto key_mapping = [&](int i) { return keys[i]; };
    auto info_mapping = [&](int i) { return infos[i]; };
    // 由于异或满足交换律所以 MAINTAIN_RANGE 够用了
    OY::SortFHQ::Tree<int, XorClass, std::less<int>, OY::SortFHQ::MAINTAIN_RANGE, 1000> S(10, key_mapping, info_mapping, {});
    // 可以看到初始时，每个元素都是各自隔开的
    cout << S << endl;
    cout << "xor_sum(S[3~8]) = " << S.query(3, 8) << endl;
    // 进行区间排序
    S.sort<true>(1, 4);
    cout << S << endl;
    cout << "xor_sum(S[3~8]) = " << S.query(3, 8) << endl;
    S.sort<false>(3, 6);
    cout << S << endl;
    cout << "xor_sum(S[3~8]) = " << S.query(3, 8) << endl;

    cout << endl;
}

int main() {
    test_normal_tree();
    test_sum_tree();
    test_xor_tree();
}
/*
#输出如下
10 5 9 1 8 4 6 7 3 2
{{10}, {5}, {9}, {1}, {8}, {4}, {6}, {7}, {3}, {2}}
{{10}, {9, 8, 5, 1}, {4}, {6}, {7}, {3}, {2}}
{{10}, {9, 8}, {1, 4, 5, 6}, {7}, {3}, {2}}
10 9 8 1 4 5 6 7 3 2

b a c d c a b d c a
{{b:b}, {a:a}, {c:c}, {d:d}, {c:c}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = dcabdc
{{b:b}, {d:d, c:c, c:c, a:a}, {a:a}, {b:b}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = caabdc
{{b:b}, {d:d, c:c}, {a:a, a:a, b:b, c:c}, {d:d}, {c:c}, {a:a}}
sum(S[3~8]) = aabcdc

{{10:1000}, {5:500}, {9:900}, {1:100}, {8:800}, {4:400}, {6:600}, {7:700}, {3:300}, {2:200}}
xor_sum(S[3~8]) = 796
{{10:1000}, {9:900, 8:800, 5:500, 1:100}, {4:400}, {6:600}, {7:700}, {3:300}, {2:200}}
xor_sum(S[3~8]) = 456
{{10:1000}, {9:900, 8:800}, {1:100, 4:400, 5:500, 6:600}, {7:700}, {3:300}, {2:200}}
xor_sum(S[3~8]) = 456


*/