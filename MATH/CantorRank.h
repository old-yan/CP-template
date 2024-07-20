/*
最后修改:
20240719
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CANTORRANK__
#define __OY_CANTORRANK__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Tp>
    struct CantorRank {
        using size_type = uint32_t;
        template <typename Iterator>
        static Tp raw_query(Iterator first, Iterator last) {
            const size_type n = last - first;
            std::vector<size_type> bit(*std::max_element(first, last) + 1);
            Tp permutation = Tp::raw(1), res{};
            auto lowbit = [&](size_type x) { return x & -x; };
            auto presum = [&](size_type x) {
                size_type res = 0;
                for (size_type i = x; ~i; i -= lowbit(i + 1)) res += bit[i];
                return res;
            };
            auto add = [&](size_type x) {
                while (x < bit.size()) bit[x]++, x += lowbit(x + 1);
            };
            Tp z{};
            for (size_type index = n - 1; ~index; index--) {
                size_type x = *(first + index), s = x ? presum(x - 1) : 0;
                res += permutation * Tp::raw(s), permutation *= ++z, add(x);
            }
            return res;
        }
        template <typename Iterator, typename ValueType = typename std::iterator_traits<Iterator>::value_type>
        static Tp query(Iterator first, Iterator last) {
            const size_type n = last - first;
            struct Pair {
                ValueType m_val;
                size_type m_id;
                bool operator<(const Pair &rhs) const { return m_val < rhs.m_val; }
            };
            std::vector<Pair> sorted;
            sorted.reserve(n);
            size_type cur = 0;
            for (auto it = first; it != last; ++it) sorted.push_back({*it, cur++});
            std::sort(sorted.begin(), sorted.end());
            std::vector<size_type> id(n);
            for (size_type i = 0, j = 0; i < n; i++) {
                if (i && !(sorted[i - 1].m_val < sorted[i].m_val)) j++;
                id[sorted[i].m_id] = i;
            }
            return raw_query(id.begin(), id.end());
        }
    };
}

#endif