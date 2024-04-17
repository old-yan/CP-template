#include "DS/MonoStack.h"
#include "IO/FastIO.h"

/*
[P2698 [USACO12MAR] Flowerpot S](https://www.luogu.com.cn/problem/P2698)
*/
/**
 * 双指针找最短区间
 */

static constexpr uint32_t N = 100000;
struct {
    int pos, time;
} rain[N];
void solve_window() {
    uint32_t n, D;
    cin >> n >> D;
    for (uint32_t i = 0; i != n; i++) cin >> rain[i].pos >> rain[i].time;
    std::sort(rain, rain + n, [](auto &x, auto &y) { return x.pos < y.pos; });

    auto getter = [](uint32_t index) {
        return rain[index].time;
    };
    OY::MonoMaxStack<int, decltype(getter)> S_max(n, {}, getter, {}, 1000001, true);
    OY::MonoMinStack<int, decltype(getter)> S_min(n, {}, getter, {}, -1, true);
    int ans = 0x3f3f3f3f;
    for (uint32_t r = 0, l = 0; r < n; r++) {
        S_max.extend_right();
        S_min.extend_right();
        while (l != r && S_max.query_on_left_side(l, r)->m_value - S_min.query_on_left_side(l, r)->m_value >= D) l++;
        if (l) ans = std::min(ans, rain[r].pos - rain[l - 1].pos);
    }
    cout << (ans == 0x3f3f3f3f ? -1 : ans);
}

int main() {
    solve_window();
}