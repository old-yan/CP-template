#include "DS/ReversedBiTrie.h"
#include "IO/FastIO.h"

int A[] = {4, 6, 9, 2, 3, 2, 4, 4, 10, 5};
void test_normal() {
    using BiTrie = OY::REVBITRIE::Tree<uint32_t, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert(a);
        cout << S << endl;
    }

    // 查询是否包含某元素
    if (S.contains(1))
        cout << "S contains 1\n";
    else
        cout << "S doesn't contain 1\n";
    if (S.contains(7))
        cout << "S contains 7\n";
    else
        cout << "S doesn't contain 7\n";

    cout << endl;
}

void test_info() {
    // 通过自定义 info 可以维护自定义信息
    // 比如这个 info 可以维护最大出现次数
    struct info {
        int m_max_cnt;
    };
    using BiTrie = OY::REVBITRIE::Tree<uint32_t, 5, info>;

    BiTrie S;
    for (int a : A) {
        auto ptr = S.insert(a);
        int cnt = ++(ptr->m_max_cnt);
        S.trace(a, [&](BiTrie::node *p) { p->m_max_cnt = std::max(p->m_max_cnt, cnt); });
        cout << S << endl;
    }

    // 查询模 4 余 2 的元素的最大出现次数
    // 两种写法

    // 第一种
    auto S2 = S.split_by_lowbits(2, 2);
    // 先把 {2,6} 分割出来
    cout << "S2 = " << S2 << endl;
    // 但是因为这个模板是没有 pushup 动作的，所以直接查询 root() 是不对的，需要手动更新一下
    S2.trace(2, [&](BiTrie::node *p) { p->m_max_cnt = std::max(p->lchild()->m_max_cnt, p->rchild()->m_max_cnt); });
    auto res = S2.root()->m_max_cnt;
    S.merge(S2);
    cout << "max count = " << res << endl;

    // 第二种直接在原树找
    auto sub = S.find_greedy(2, 2);
    if (sub.second) {
        cout << "max count = " << sub.first->m_max_cnt << endl;
    }

    cout << endl;
}

void test_erasable() {
    // 统计某个字符串的出现次数，用次数做差来作为搜索依据
    using BiTrie = OY::REVBITRIE::CountTree<uint32_t, int, 5>;

    BiTrie S;
    for (int a : A) {
        S.insert_one(a);
        cout << S << endl;
    }

    // 把模 8 余 2 的元素分割出来
    auto S2 = S.split_by_lowbits(2, 3);
    cout << S2 << endl;
    // 将其键值全部加 2
    S2.globally_add(2);
    cout << S2 << endl;
    // 合并回 S，查询有几个 4
    S.merge(S2);
    auto it = S.find_greedy(4);
    if (it.second == 5) {
        cout << "count of 4 : " << it.first->count() << endl;
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
{4,6}
{4,6,9}
{4,2,6,9}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,10,6,9,3}
{4,2,10,6,9,5,3}
S contains 1
S contains 7

{4}
{4,6}
{4,6,9}
{4,2,6,9}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,6,9,3}
{4,2,10,6,9,3}
{4,2,10,6,9,5,3}
S2 = {2,10,6}
max count = 2
max count = 2

{4*1}
{4*1,6*1}
{4*1,6*1,9*1}
{4*1,2*1,6*1,9*1}
{4*1,2*1,6*1,9*1,3*1}
{4*1,2*2,6*1,9*1,3*1}
{4*2,2*2,6*1,9*1,3*1}
{4*3,2*2,6*1,9*1,3*1}
{4*3,2*2,10*1,6*1,9*1,3*1}
{4*3,2*2,10*1,6*1,9*1,5*1,3*1}
{2*2,10*1}
{4*2,12*1}
count of 4 : 5

*/