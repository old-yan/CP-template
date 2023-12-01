/*
最后修改:
20231201
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FASTTRANSFORM__
#define __OY_FASTTRANSFORM__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace FASTTRANS {
        using size_type = uint32_t;
        struct DivideBy2_naive {
            template <typename Tp>
            Tp operator()(Tp x) const { return x / 2; }
        };
        struct FastTranformer {
            size_type m_length;
            std::vector<std::pair<size_type, size_type>> m_transfers;
            FastTranformer(size_type length, size_type transfer_cnt) : m_length(length) { m_transfers.reserve(transfer_cnt); }
            void add_transfer(size_type prev, size_type next) { m_transfers.emplace_back(prev, next); }
            template <bool Forward, typename Sequence>
            void transform(Sequence &sequence) {
                if constexpr (Forward)
                    for (auto &[prev, next] : m_transfers) {
                        sequence[next] += sequence[prev];
                    }
                else
                    for (auto it = m_transfers.rbegin(); it != m_transfers.rend(); ++it) {
                        auto &[prev, next] = *it;
                        sequence[next] -= sequence[prev];
                    }
            }
        };
        template <bool Forward, typename Iterator, typename DivideBy2 = DivideBy2_naive>
        void fast_bitxor_transform(Iterator first, Iterator last, DivideBy2 div_by2 = DivideBy2()) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it = x + y, *it2 = x - y;
                        else
                            *it = div_by2(x + y), *it2 = div_by2(x - y);
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_bitor_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it2 = x + y;
                        else
                            *it2 = y - x;
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_bitand_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            for (size_type i = 1; i != length; i <<= 1)
                for (size_type j = 0; j != length; j += i << 1)
                    for (auto it = first + j, it2 = first + j + i, end = first + j + i; it != end; ++it, ++it2) {
                        auto x = *it, y = *it2;
                        if constexpr (Forward)
                            *it = x + y;
                        else
                            *it = x - y;
                    }
        }
        template <bool Forward, typename Iterator>
        void fast_max_transform(Iterator first, Iterator last) {
            if constexpr (Forward)
                std::partial_sum(first, last, first);
            else
                std::adjacent_difference(first, last, first);
        }
        template <bool Forward, typename Iterator>
        void fast_min_transform(Iterator first, Iterator last) {
            const size_type length = last - first;
            if constexpr (Forward)
                for (size_type i = length - 2; ~i; i--) *(first + i) += *(first + i + 1);
            else
                for (size_type i = 0; i + 1 < length; i++) *(first + i) -= *(first + i + 1);
        }
        template <bool Forward, typename Iterator, typename FindKthPrime>
        void fast_gcd_transform(Iterator first, Iterator last, FindKthPrime &&find_kth_prime) {
            const size_type length = last - first;
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p >= length) break;
                if constexpr (Forward)
                    for (size_type j = (length - 1) / p; j; j--) *(first + j) += *(first + j * p);
                else
                    for (size_type j = 0, end = (length - 1) / p; j <= end; j++) *(first + j) -= *(first + j * p);
            }
        }
        template <bool Forward, typename Iterator, typename FindKthPrime>
        void fast_lcm_transform(Iterator first, Iterator last, FindKthPrime &&find_kth_prime) {
            const size_type length = last - first;
            for (size_type i = 0;; i++) {
                const size_type p = find_kth_prime(i);
                if (p >= length) break;
                if constexpr (Forward)
                    for (size_type j = 0, end = (length - 1) / p; j <= end; j++) *(first + j * p) += *(first + j);
                else
                    for (size_type j = (length - 1) / p; j; j--) *(first + j * p) -= *(first + j);
            }
        }
    }
}

#endif