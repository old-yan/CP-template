#include "DS/MonoAVL.h"
#include "DS/MonoSplay.h"
#include "IO/FastIO.h"

/*
[翻转排序](https://ac.nowcoder.com/acm/problem/275173)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/85333/K
 * 纯模拟
 */

using Splay = OY::MonoSplaySequence<uint32_t, true>;
void solve_splay() {
    uint32_t n;
    cin >> n;
    Splay::_reserve(n + 1);
    auto read = [&](uint32_t i) {
        uint32_t x;
        cin >> x;
        return x - 1;
    };
    auto S = Splay::from_mapping(n, read);

    std::vector<Splay::node *> ps(n);
    S.enumerate([&](auto p) {
        ps[p->m_val] = p;
    });

    cout << n << endl;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t pos = Splay::get_root_and_rank(ps[i]).second;
        S.reverse(i, pos);
        cout << i + 1 << ' ' << pos + 1 << endl;
    }
}

using AVL = OY::VectorMonoMinAVL<uint32_t, 0x3f3f3f3f, true>;
void solve_avl() {
    uint32_t n;
    cin >> n;
    AVL::_reserve(n + 1);
    auto read = [&](uint32_t i) {
        uint32_t x;
        cin >> x;
        return x - 1;
    };
    auto S = AVL::from_mapping(n, read);
    cout << n << endl;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t pos = S.max_right(i, [&](auto v) {
            return v > i;
        }) + 1;
        S.reverse(i, pos);
        cout << i + 1 << ' ' << pos + 1 << endl;
    }
}

int main(){
    solve_splay();
    // solve_avl();
}