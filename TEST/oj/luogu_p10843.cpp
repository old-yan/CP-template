#include "DS/Queue.h"
#include "DS/AdjDiff.h"
#include "IO/FastIO.h"

/*
[P10843 【MX-J2-T4】Turtle and Cycles](https://www.luogu.com.cn/problem/P10843)
*/
/**
 * 本题要做的操作，等价于差分数组上的 swap 操作
 * 当把所有的正数集合在一起时，环上只有单峰
 */

static constexpr uint32_t N = 200000;
uint32_t arr[N];
int dif[N];
void solve_adj() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n;
        cin >> n;
        for (uint32_t i = 0; i != n; i++) cin >> arr[i];
        uint32_t positive = 0;
        for (uint32_t i = 0; i != n; i++) {
            dif[i] = arr[i] - arr[i ? i - 1 : n - 1];
            positive += dif[i] > 0;
        }
        if (positive <= 1)
            cout << "0\n";
        else {
            OY::AdjDiff::Table<uint64_t, false> index_sum(positive, [cur = 0](auto...) mutable {
                while (dif[cur] <= 0) cur++;
                return cur++;
            });
            index_sum.switch_to_presum();
            auto query = [&](uint32_t l, uint32_t r) {
                if (r < positive)
                    return index_sum.query(l, r);
                else if (l >= positive)
                    return index_sum.query(l - positive, r - positive) + uint64_t(r - l + 1) * n;
                else
                    return index_sum.query(0, r - positive) + uint64_t(r - positive + 1) * n + index_sum.query(l, positive - 1);
            };
            uint64_t ans = -1;
            for (uint32_t l = 0, m = positive / 2, r = positive - 1; l != positive; l++, m++, r++) {
                uint64_t dest = m < positive ? index_sum.query(m, m) : index_sum.query(m - positive, m - positive) + n;
                // 左侧点汇集到 [dest-(m-l), dest-1]
                // 右侧点汇集到 [dest, dest+(r-m)]
                uint64_t dis_l = (dest - (m - l) + dest - 1) * (m - l) / 2 - query(l, m - 1);
                uint64_t dis_r = query(m, r) - (dest + dest + (r - m)) * (r - m + 1) / 2;
                ans = std::min(ans, dis_l + dis_r);
            }
            cout << ans << endl;
        }
    }
}

void solve_queue() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n;
        cin >> n;
        for (uint32_t i = 0; i != n; i++) cin >> arr[i];
        uint32_t positive = 0;
        for (uint32_t i = 0; i != n; i++) {
            dif[i] = arr[i] - arr[i ? i - 1 : n - 1];
            positive += dif[i] > 0;
        }
        if (positive <= 1)
            cout << "0\n";
        else {
            OY::SumQueue<uint32_t, uint64_t> ql, qr;
            uint32_t cur = 0;
            for (; ql.size() < positive / 2; cur++)
                if (dif[cur] > 0) ql.push(cur);
            for (; qr.size() < (positive + 1) / 2; cur++)
                if (dif[cur] > 0) qr.push(cur);
            uint64_t ans = -1;
            cur = 0;
            while (true) {
                while (cur != n && dif[cur] <= 0) cur++;
                if (cur == n) break;
                ql.pop();
                ql.push(qr.front());
                qr.pop();
                qr.push(n + cur++);
                uint64_t dest = qr.front();
                // 左侧点汇集到 [dest-ql.size(), dest-1]
                // 右侧点汇集到 [dest, dest+qr.size()-1]
                uint64_t dis_l = (dest - ql.size() + dest - 1) * ql.size() / 2 - ql.query_all();
                uint64_t dis_r = qr.query_all() - (dest + dest + qr.size() - 1) * qr.size() / 2;
                ans = std::min(ans, dis_l + dis_r);
            }
            cout << ans << endl;
        }
    }
}

int main() {
    solve_adj();
    // solve_queue();
}