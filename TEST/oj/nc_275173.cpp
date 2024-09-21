#include "DS/MonoAVL.h"
#include "IO/FastIO.h"

/*
[翻转排序](https://ac.nowcoder.com/acm/problem/275173)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/85333/K
 * 纯模拟
 */

using Tree = OY::VectorMonoMinAVL<uint32_t, 0x3f3f3f3f, true>;
int main() {
    uint32_t n;
    cin >> n;
    auto read = [&](uint32_t i) {
        uint32_t x;
        cin >> x;
        return x - 1;
    };
    Tree::_reserve(n + 1);
    auto S = Tree::from_mapping(n, read);
    cout << n << endl;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t pos = S.max_right(i, [&](auto v) {
            return v > i;
        }) + 1;
        S.reverse(i, pos);
        cout << i + 1 << ' ' << pos + 1 << endl;
    }
}