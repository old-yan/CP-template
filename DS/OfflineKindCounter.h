/*
最后修改:
20240503
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINEKINDCOUNTER__
#define __OY_OFFLINEKINDCOUNTER__

#include "GlobalHashMap.h"

namespace OY {
    namespace OFFLINEKC {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        template <size_type MAX_VALUE>
        struct ArrayTag {
            template <typename Tp>
            using type = size_type[MAX_VALUE + 1];
        };
        template <bool MakeRecord, size_type BUFFER>
        struct HashmapTag {
            template <typename Tp>
            struct type {
                using hash_table = GHASH::UnorderedMap<Tp, size_type, MakeRecord, BUFFER>;
                using node = typename hash_table::node;
                static hash_table s_hashmap;
                type() {
                    if constexpr (MakeRecord) s_hashmap.do_for_each([](node *p) { p->m_mapped = 0; }), s_hashmap.clear();
                }
                size_type &operator[](const Tp &key) { return s_hashmap.insert(key).m_ptr->m_mapped; }
            };
        };
        template <bool MakeRecord, size_type BUFFER>
        template <typename Tp>
        GHASH::UnorderedMap<Tp, size_type, MakeRecord, BUFFER> HashmapTag<MakeRecord, BUFFER>::type<Tp>::s_hashmap;
        struct SimpleBIT {
            std::vector<size_type> m_sum;
            SimpleBIT(size_type length) : m_sum(length) {}
            void add(size_type i, size_type inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += lowbit(i + 1);
            }
            size_type presum(size_type i) const {
                size_type res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += m_sum[j];
                return res;
            }
        };
        template <typename Tag>
        class Solver {
            struct Query {
                size_type m_id, m_left, m_right;
                bool operator<(const Query &rhs) const { return m_right < rhs.m_right; }
            };
            std::vector<size_type> m_last;
            mutable std::vector<Query> m_queries;
        public:
            Solver() = default;
            template <typename InitMapping>
            Solver(size_type length, InitMapping mapping, size_type query_cnt = 0) { resize(length, mapping, query_cnt); }
            template <typename Iterator>
            Solver(Iterator first, Iterator last, size_type query_cnt = 0) { reset(first, last, query_cnt); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping, size_type query_cnt = 0) {
                m_last.assign(length, {});
                m_queries.clear();
                m_queries.reserve(query_cnt);
                using Tp = typename std::decay<decltype(mapping(0))>::type;
                if constexpr (std::is_void<Tag>::value) {
                    struct pair {
                        Tp m_val;
                        size_type m_index;
                        bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                    };
                    std::vector<pair> ps(length);
                    for (size_type i = 0; i != length; i++) ps[i] = {Tp(mapping(i)), i};
                    std::sort(ps.begin(), ps.end());
                    std::vector<size_type> items(length);
                    size_type id = 0;
                    for (size_type i = 0; i != length; i++) {
                        if (i && ps[i - 1].m_val < ps[i].m_val) id++;
                        items[ps[i].m_index] = id;
                    }
                    std::vector<size_type> mp(id + 1);
                    for (size_type i = 0; i != length; i++) {
                        size_type c = items[i];
                        std::swap(mp[c], m_last[i] = i + 1);
                    }
                } else {
                    typename Tag::template type<Tp> mp{};
                    for (size_type i = 0; i != length; i++) {
                        Tp c = mapping(i);
                        std::swap(mp[c], m_last[i] = i + 1);
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, size_type query_cnt = 0) {
                resize(last - first, [&](size_type i) { return *(first + i); }, query_cnt);
            }
            void add_query(size_type left, size_type right) { m_queries.push_back({(size_type)m_queries.size(), left, right}); }
            template <typename CountTree = SimpleBIT>
            std::vector<size_type> solve() const {
                size_type length = m_last.size(), query_cnt = m_queries.size();
                std::sort(m_queries.begin(), m_queries.end());
                std::vector<size_type> ans(query_cnt);
                CountTree cnt(length);
                for (size_type r = 0, cur = 0; r != length && cur != query_cnt; r++) {
                    if (m_last[r]) cnt.add(m_last[r] - 1, -1);
                    cnt.add(r, 1);
                    auto sum_r = cnt.presum(r);
                    while (cur != query_cnt && m_queries[cur].m_right == r) {
                        auto &&q = m_queries[cur++];
                        ans[q.m_id] = sum_r - (q.m_left ? cnt.presum(q.m_left - 1) : 0);
                    }
                }
                return ans;
            };
        };
    }
    template <OFFLINEKC::size_type MAX_VALUE>
    using OfflineArrayKindCounter = OFFLINEKC::Solver<OFFLINEKC::ArrayTag<MAX_VALUE>>;
    template <bool MakeRecord, OFFLINEKC::size_type BUFFER>
    using OfflineHashmapKindCounter = OFFLINEKC::Solver<OFFLINEKC::HashmapTag<MakeRecord, BUFFER>>;
}

#endif