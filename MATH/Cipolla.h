/*
最后修改:
20240318
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CIPOLLA__
#define __OY_CIPOLLA__

#include <algorithm>
#include <cstdint>

namespace OY {
    template <typename Tp, typename ModType = typename Tp::mod_type>
    ModType Cipolla(Tp a) {
        const ModType P = Tp::mod();
        auto one = Tp::raw(1);
        if (a.pow((P - 1) / 2) != one) return 0;
        auto b = one;
        while (true) {
            Tp c = b * b - a;
            if (c.pow((P - 1) / 2) != one) break;
            b += one;
        }
        Tp neg = b * b - a;
        struct node {
            Tp a, b;
        };
        auto mul = [&](const node &x, const node &y) {
            return node{x.a * y.a + x.b * y.b * neg, x.b * y.a + x.a * y.b};
        };
        auto pow = [&](node x, ModType n) {
            node res{one, Tp{}};
            while (n) {
                if (n & 1) res = mul(res, x);
                x = mul(x, x), n >>= 1;
            }
            return res;
        };
        auto ans = pow({b, one}, (P + 1) / 2).a.val();
        return ans * 2 > P ? P - ans : ans;
    }
}

#endif