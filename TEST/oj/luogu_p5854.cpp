#include "DS/CartesianTree.h"
#include "IO/FastIO.h"

/*
[P5854 【模板】笛卡尔树](https://www.luogu.com.cn/problem/P5854)
*/
/**
 * 本题为笛卡尔树模板题
*/
static constexpr uint32_t N = 10000000;
uint64_t len, s1, s2;

int main() {
    uint32_t n;
    cin >> n;
    auto mapping = [](auto...) {
        uint64_t x;
        cin >> x;
        return x;
    };
    auto lchild_call = [&](uint64_t i, uint32_t lc) {
        s1 ^= (i + 1) * (lc + 2);
    };
    auto rchild_call = [&](uint64_t i, uint32_t rc) {
        s2 ^= (i + 1) * (rc + 2);
    };
    OY::Cartesian::solve<uint64_t, N + 1>(n, mapping, lchild_call, rchild_call, std::greater<uint64_t>(), 0);
    cout << s1 << ' ' << s2;
}