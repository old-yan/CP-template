#include "DS/LinearDSU.h"
#include "IO/FastIO.h"

/*
[小苯的蓄水池（hard）](https://ac.nowcoder.com/acm/problem/281338)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/93847/E
 * 显然，水池的合并只会发生在邻项
 * 非常适合线性并查集
 */

void solve_dsu() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<uint64_t> a(n);
    for (uint32_t i = 0; i != n; i++) cin >> a[i];
    OY::LDSU::Table<true> u(n);
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r, l--, r--;
            uint32_t head = u.find_head(l);
            uint32_t tail = u.find_tail(l);
            while (tail < r) {
                a[head] += a[tail + 1];
                u.unite_after(tail);
                tail = u.find_tail(tail);
            }
        } else {
            uint32_t pos;
            cin >> pos, pos--;
            uint32_t l = u.find_head(pos);
            uint32_t r = u.find_tail(pos);
            double ave = 1. * a[l] / (r - l + 1);
            auto write_double = [](double x) {
                x += 0.000000005;
                uint64_t x0 = x, x1 = (x - x0) * 100000000;
                cout << x0 << '.';
                auto str = std::to_string(x1);
                cout << std::string(8 - str.size(), '0') + str << endl;
            };
            write_double(ave);
        }
    }
}

int main() {
    solve_dsu();
}
