#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[P4301 [CQOI2013] 新Nim游戏](https://www.luogu.com.cn/problem/P4301)
*/
/**
 * 本题为线性基模板题
 */

uint32_t arr[100];
int main() {
    uint32_t n;
    cin >> n;
    OY::StaticHamelXorBase32<30> hxb{};
    for (uint32_t i = 0; i < n; i++) cin >> arr[i];
    std::sort(arr, arr + n, std::greater<uint32_t>());
    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++)
        if (!~hxb.insert(arr[i])) ans += arr[i];
    cout << ans;
}