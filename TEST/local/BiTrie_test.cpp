#include "DS/BiTrie.h"
#include "IO/FastIO.h"

int A[] = {4, 6, 9, 2, 3, 5, 4, 4, 1, 5};
void test_normal() {
    using BiTrie = OY::BiTrie::Tree<uint32_t, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert(a);
        cout << S << endl;
    }

    // 查询是否包含某元素
    if (!S.contains(1)->is_null())
        cout << "S contains 1\n";
    else
        cout << "S doesn't contain 1\n";
    if (!S.contains(7)->is_null())
        cout << "S contains 7\n";
    else
        cout << "S doesn't contain 7\n";

    // 查询与 8 的最大异或：6(00110) 和 8(01000) 最大程度不同
    auto res3 = S.max_bitxor(8).second;
    cout << (res3 ^ 8) << " xor 8 = " << res3 << endl;

    // 查询与 6 的最大异或：9(01001) 和 6(00110) 最大程度不同
    auto res4 = S.max_bitxor(6).second;
    cout << (res4 ^ 6) << " xor 6 = " << res4 << endl
         << endl;
}

void test_info() {
    // 通过自定义 info 可以维护自定义信息
    // 比如这个 info 可以维护最大出现次数
    struct info {
        int m_max_cnt;
    };
    using BiTrie = OY::BiTrie::Tree<uint32_t, 5, info>;

    BiTrie S;
    for (int a : A) {
        auto ptr = S.insert(a);
        int cnt = ++(ptr->m_max_cnt);
        S.trace(a, [&](BiTrie::node *p) { p->m_max_cnt = std::max(p->m_max_cnt, cnt); });
        cout << S << endl;
    }

    // 查询出现一次或一次以上的元素与 6 的最大异或：  9(01001) 和 6(00110) 最大程度不同
    auto judge1 = [](BiTrie::node *p) { return p->m_max_cnt >= 1; };
    auto res1 = S.max_bitxor(6, judge1).second;
    cout << (res1 ^ 6) << " xor 6 = " << res1 << endl;

    // 查询出现两次或两次以上的元素与 6 的最大异或：  5(00101) 和 6(00110) 最大程度不同
    auto judge2 = [](BiTrie::node *p) { return p->m_max_cnt >= 2; };
    auto res2 = S.max_bitxor(6, judge2).second;
    cout << (res2 ^ 6) << " xor 6 = " << res2 << endl;

    // 查询出现三次或三次以上的元素与 6 的最大异或：  4(00100) 和 6(00110) 最大程度不同
    auto judge3 = [](BiTrie::node *p) { return p->m_max_cnt >= 3; };
    auto res3 = S.max_bitxor(6, judge3).second;
    cout << (res3 ^ 6) << " xor 6 = " << res3 << endl
         << endl;
}

void test_erasable() {
    // 统计某个字符串的出现次数，用次数做差来作为搜索依据
    using BiTrie = OY::BiTrie::CountTree<uint32_t, int, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert_one(a);
        cout << S << endl;
    }

    // 查询与 6 的最小异或
    auto res = S.min_bitxor(6).second;
    cout << (res ^ S._mask() ^ 6) << " xnor 6 = " << res << endl;

    // 查询与 6 的最大异或
    res = S.max_bitxor(6).second;
    cout << (res ^ 6) << " xor 6 = " << res << endl;

    // 可以求出第 k 大异或
    // 注意，此处的 k 大异或是不去重的
    cout << "kth:\n";
    for (int k = 0; k < S.root()->count(); k++) {
        auto res = S.kth_bitxor(6, k).second;
        cout << "No." << k << ": " << (res ^ 6) << " ^ 6 = " << res << endl;
    }

    // 甚至，还可以求出某个异或值是第几大异或
    // 注意，此处的 k 大异或是不去重的
    // 假如题目要求，查询与 6 的异或是排第几
    cout << "rank:\n";
    for (int res = 0; res <= S._mask(); res++)
        if (!S.contains(res ^ 6)->is_null()) {
            auto rnk = S.rank_bitxor(6, res);
            cout << "No." << rnk << ": " << (res ^ 6) << " ^ 6 = " << res << endl;
        }
}

int main() {
    test_normal();
    test_info();
    test_erasable();
}
/*
#输出如下
{4}
{4, 6}
{4, 6, 9}
{2, 4, 6, 9}
{2, 3, 4, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
S contains 1
S doesn't contain 7
6 xor 8 = 14
9 xor 6 = 15

{4}
{4, 6}
{4, 6, 9}
{2, 4, 6, 9}
{2, 3, 4, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
{1, 2, 3, 4, 5, 6, 9}
9 xor 6 = 15
5 xor 6 = 3
4 xor 6 = 2

{4*1}
{4*1, 6*1}
{4*1, 6*1, 9*1}
{2*1, 4*1, 6*1, 9*1}
{2*1, 3*1, 4*1, 6*1, 9*1}
{2*1, 3*1, 4*1, 5*1, 6*1, 9*1}
{2*1, 3*1, 4*2, 5*1, 6*1, 9*1}
{2*1, 3*1, 4*3, 5*1, 6*1, 9*1}
{1*1, 2*1, 3*1, 4*3, 5*1, 6*1, 9*1}
{1*1, 2*1, 3*1, 4*3, 5*2, 6*1, 9*1}
25 xnor 6 = 0
9 xor 6 = 15
kth:
No.0: 6 ^ 6 = 0
No.1: 4 ^ 6 = 2
No.2: 4 ^ 6 = 2
No.3: 4 ^ 6 = 2
No.4: 5 ^ 6 = 3
No.5: 5 ^ 6 = 3
No.6: 2 ^ 6 = 4
No.7: 3 ^ 6 = 5
No.8: 1 ^ 6 = 7
No.9: 9 ^ 6 = 15
rank:
No.0: 6 ^ 6 = 0
No.1: 4 ^ 6 = 2
No.4: 5 ^ 6 = 3
No.6: 2 ^ 6 = 4
No.7: 3 ^ 6 = 5
No.8: 1 ^ 6 = 7
No.9: 9 ^ 6 = 15

*/