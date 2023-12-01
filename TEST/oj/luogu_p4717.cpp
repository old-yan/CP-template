#include "IO/FastIO.h"
#include "MATH/FastTransform.h"
#include "MATH/StaticModInt32.h"

/*
[P4717 【模板】快速莫比乌斯/沃尔什变换 (FMT/FWT)](https://www.luogu.com.cn/problem/P4717)
*/
/**
 * 本题为 FWT 模板题
 */

static constexpr uint32_t N = 17;
using mint = OY::mint998244353;
mint arr1[1 << N], arr2[1 << N], arr3[1 << N], arr4[1 << N];

int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < 1 << n; i++) cin >> arr1[i];
    for (uint32_t i = 0; i < 1 << n; i++) cin >> arr2[i];

    // 把数组内容复制到 arr3 arr4，并进行卷积
    std::copy_n(arr1, 1 << n, arr3);
    std::copy_n(arr2, 1 << n, arr4);
    OY::FASTTRANS::fast_bitor_transform<true>(arr3, arr3 + (1 << n));
    OY::FASTTRANS::fast_bitor_transform<true>(arr4, arr4 + (1 << n));
    for (uint32_t i = 0; i < 1 << n; i++) arr3[i] *= arr4[i];
    OY::FASTTRANS::fast_bitor_transform<false>(arr3, arr3 + (1 << n));
    for (uint32_t i = 0; i < 1 << n; i++) cout << arr3[i] << " ";
    cout << endl;

    // 把数组内容复制到 arr3 arr4，并进行卷积
    std::copy_n(arr1, 1 << n, arr3);
    std::copy_n(arr2, 1 << n, arr4);
    OY::FASTTRANS::fast_bitand_transform<true>(arr3, arr3 + (1 << n));
    OY::FASTTRANS::fast_bitand_transform<true>(arr4, arr4 + (1 << n));
    for (uint32_t i = 0; i < 1 << n; i++) arr3[i] *= arr4[i];
    OY::FASTTRANS::fast_bitand_transform<false>(arr3, arr3 + (1 << n));
    for (uint32_t i = 0; i < 1 << n; i++) cout << arr3[i] << " ";
    cout << endl;

    // 最后，不用复制，直接原地卷积
    OY::FASTTRANS::fast_bitxor_transform<true>(arr1, arr1 + (1 << n), [](mint x) {
        return mint::raw((x.m_val & 1) ? ((mint::mod() + x.m_val) >> 1) : (x.m_val >> 1));
    });
    OY::FASTTRANS::fast_bitxor_transform<true>(arr2, arr2 + (1 << n), [](mint x) {
        return mint::raw((x.m_val & 1) ? ((mint::mod() + x.m_val) >> 1) : (x.m_val >> 1));
    });
    for (uint32_t i = 0; i < 1 << n; i++) arr1[i] *= arr2[i];
    OY::FASTTRANS::fast_bitxor_transform<false>(arr1, arr1 + (1 << n), [](mint x) {
        return mint::raw((x.m_val & 1) ? ((mint::mod() + x.m_val) >> 1) : (x.m_val >> 1));
    });
    for (uint32_t i = 0; i < 1 << n; i++) cout << arr1[i] << " ";
    cout << endl;
}
