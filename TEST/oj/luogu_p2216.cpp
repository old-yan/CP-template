#include "DS/ZkwTree2D.h"
#include "IO/FastIO.h"

/*
[P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)
*/
/**
 * 本题为二维线段树模板题
 */
struct Pair {
    uint32_t min, max;
};
struct MinMaxNode {
    using value_type = Pair;
    using modify_type = Pair;
    static value_type op(const value_type &x, const value_type &y) { return Pair{std::min(x.min, y.min), std::max(x.max, y.max)}; }
    Pair m_val;
    const value_type &get() const { return m_val; }
    void set(uint32_t val) { m_val.min = m_val.max = val; }
    void set(Pair val) { m_val = val; }
};
void solve_zkw() {
    using Tree = OY::Zkw2D::Tree<MinMaxNode, 1 << 22>;
    uint32_t m, n, k;
    cin >> m >> n >> k;
    Tree S(m, n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });

    uint32_t min_dif = 0x3f3f3f3f;
    for (uint32_t i = 0; i + k <= m; i++) {
        for (uint32_t j = 0; j + k <= n; j++) {
            auto [min, max] = S.query(i, i + k - 1, j, j + k - 1);
            min_dif = std::min(min_dif, max - min);
        }
    }
    cout << min_dif;
}

static constexpr uint32_t N = 1000;
uint32_t val[N][N], Mi[N][N], Mx[N][N];
struct {
    uint32_t index, value;
} deque[N];
void solve_monodeque() {
    uint32_t m, n, k;
    cin >> m >> n >> k;
    for (uint32_t i = 0; i != m; i++)
        for (uint32_t j = 0; j != n; j++) cin >> val[i][j];
    // 计算每列列内的窗口最小值
    for (uint32_t j = 0; j != n; j++) {
        uint32_t l = 0, r = 0, dl = 0, dr = 0;
        while (r < k) {
            auto x = val[r][j];
            while (dl != dr && deque[dr - 1].value >= x) dr--;
            deque[dr++] = {r++, x};
        }
        while (true) {
            Mi[l][j] = deque[dl].value;
            if (r == m) break;
            auto x = val[r][j];
            while (dl != dr && deque[dr - 1].value >= x) dr--;
            if (dl != dr && deque[dl].index == l) dl++;
            l++;
            deque[dr++] = {r++, x};
        }
    }
    // 将列内窗口最小值横向计算窗口最小值，得到正方形最小值
    for (uint32_t i = 0; i <= m - k; i++) {
        uint32_t l = 0, r = 0, dl = 0, dr = 0;
        while (r < k) {
            auto x = Mi[i][r];
            while (dl != dr && deque[dr - 1].value >= x) dr--;
            deque[dr++] = {r++, x};
        }
        while (true) {
            Mi[i][l] = deque[dl].value;
            if (r == n) break;
            auto x = Mi[i][r];
            while (dl != dr && deque[dr - 1].value >= x) dr--;
            if (dl != dr && deque[dl].index == l) dl++;
            l++;
            deque[dr++] = {r++, x};
        }
    }
    // 计算每列列内的窗口最大值
    for (uint32_t j = 0; j != n; j++) {
        uint32_t l = 0, r = 0, dl = 0, dr = 0;
        while (r < k) {
            auto x = val[r][j];
            while (dl != dr && deque[dr - 1].value <= x) dr--;
            deque[dr++] = {r++, x};
        }
        while (true) {
            Mx[l][j] = deque[dl].value;
            if (r == m) break;
            auto x = val[r][j];
            while (dl != dr && deque[dr - 1].value <= x) dr--;
            if (dl != dr && deque[dl].index == l) dl++;
            l++;
            deque[dr++] = {r++, x};
        }
    }
    // 在计算正方形最大值的同时，与正方形最小值做差
    uint32_t ans = UINT32_MAX;
    for (uint32_t i = 0; i <= m - k; i++) {
        uint32_t l = 0, r = 0, dl = 0, dr = 0;
        while (r < k) {
            auto x = Mx[i][r];
            while (dl != dr && deque[dr - 1].value <= x) dr--;
            deque[dr++] = {r++, x};
        }
        while (true) {
            ans = std::min(ans, deque[dl].value - Mi[i][l]);
            if (r == n) break;
            auto x = Mx[i][r];
            while (dl != dr && deque[dr - 1].value <= x) dr--;
            if (dl != dr && deque[dl].index == l) dl++;
            l++;
            deque[dr++] = {r++, x};
        }
    }
    cout << ans;
}

int main() {
    solve_monodeque();
    // solve_zkw();
}