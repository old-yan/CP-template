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
            static size_type _lowbit(size_type x) { return x & -x; }
            SimpleBIT(size_type length) : m_sum(length) {}
            void add(size_type i, size_type inc) {
                while (i < m_sum.size()) m_sum[i] += inc, i += _lowbit(i + 1);
            }
            size_type presum(size_type i) const {
                size_type res{};
                for (size_type j = i; ~j; j -= _lowbit(j + 1)) res += m_sum[j];
                return res;
            }
        };
        template <typename SizeType>
        struct VectorQueryProvider {
            const std::vector<std::pair<SizeType, SizeType>> &m_queries;
            template <typename QueryConsumer>
            void operator()(size_type i, QueryConsumer consumer) { consumer(m_queries[i].first, m_queries[i].second); }
        };
        template <typename CountTree = SimpleBIT, typename Tag = void, typename QueryProvider, typename InitMapping>
        std::vector<size_type> solve(size_type query_cnt, QueryProvider query_provider, size_type length, InitMapping mapping) {
            struct Query {
                size_type m_id, m_left, m_right;
                bool operator<(const Query &rhs) const { return m_right < rhs.m_right; }
            };
            std::vector<Query> qs(query_cnt);
            for (size_type i = 0; i != query_cnt; i++) {
                auto add_query = [&](size_type l, size_type r) { qs[i] = {i, l, r}; };
                query_provider(i, add_query);
            }
            std::sort(qs.begin(), qs.end());
            using Tp = typename std::decay<decltype(mapping(0))>::type;
            std::vector<size_type> ans(query_cnt);
            CountTree cnt(length);
            if constexpr (std::is_void<Tag>::value) {
                std::vector<Tp> items(length);
                for (size_type i = 0; i != length; i++) items[i] = mapping(i);
                auto sorted = items;
                std::sort(sorted.begin(), sorted.end());
                sorted.resize(std::unique(sorted.begin(), sorted.end()) - sorted.begin());
                std::vector<size_type> mp(sorted.size());
                for (size_type r = 0, cur = 0; r != length && cur != query_cnt; r++) {
                    size_type pos = r + 1;
                    std::swap(mp[std::lower_bound(sorted.begin(), sorted.end(), items[r]) - sorted.begin()], pos);
                    if (pos) cnt.add(pos - 1, -1);
                    cnt.add(r, 1);
                    auto sum_r = cnt.presum(r);
                    while (cur != query_cnt && qs[cur].m_right == r) {
                        auto &&q = qs[cur++];
                        ans[q.m_id] = sum_r - (q.m_left ? cnt.presum(q.m_left - 1) : 0);
                    }
                }
                return ans;
            } else {
                typename Tag::template type<Tp> mp{};
                for (size_type r = 0, cur = 0; r != length && cur != query_cnt; r++) {
                    size_type pos = r + 1;
                    std::swap(mp[mapping(r)], pos);
                    if (pos) cnt.add(pos - 1, -1);
                    cnt.add(r, 1);
                    auto sum_r = cnt.presum(r);
                    while (cur != query_cnt && qs[cur].m_right == r) {
                        auto &&q = qs[cur++];
                        ans[q.m_id] = sum_r - (q.m_left ? cnt.presum(q.m_left - 1) : 0);
                    }
                }
                return ans;
            }
        }
        template <typename CountTree = SimpleBIT, typename Tag = void, typename Tp, typename SizeType>
        std::vector<size_type> solve(const std::vector<Tp> &arr, const std::vector<std::pair<SizeType, SizeType>> &queries) {
            return solve<CountTree, Tag>(queries.size(), VectorQueryProvider<SizeType>{queries}, arr.size(), [&](size_type i) { return arr[i]; });
        }
    };
}

#endif