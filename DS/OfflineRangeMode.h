/*
最后修改:
20240530
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINERANGEMODE__
#define __OY_OFFLINERANGEMODE__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace OY {
    namespace OFFLINEMODE {
        using size_type = uint32_t;
        enum ResultTag {
            FREQUENCY = 0,
            ELEM_FREQUENCY = 1,
            MINELEM_FREQUENCY = 2
        };
        template <typename Tp, ResultTag Tag>
        struct Result {
            size_type m_freq;
        };
        template <typename Tp>
        struct Result<Tp, ELEM_FREQUENCY> {
            Tp m_elem;
            size_type m_freq;
        };
        template <typename Tp>
        struct Result<Tp, MINELEM_FREQUENCY> {
            Tp m_min_elem;
            size_type m_freq;
        };
        template <typename Tp>
        class Solver {
            struct Query {
                size_type m_id, m_left, m_right;
            };
            std::vector<Tp> m_sorted;
            std::vector<size_type> m_arr;
            mutable std::vector<Query> m_queries;
        public:
            Solver() = default;
            template <typename InitMapping>
            Solver(size_type length, InitMapping mapping, size_type query_cnt = 0) { resize(length, mapping, query_cnt); }
            template <typename Iterator>
            Solver(Iterator first, Iterator last, size_type query_cnt = 0) { reset(first, last, query_cnt); }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping, size_type query_cnt = 0) {
                struct pair {
                    Tp m_val;
                    size_type m_index;
                    bool operator<(const pair &rhs) const { return m_val < rhs.m_val; }
                };
                std::vector<pair> ps(length);
                for (size_type i = 0; i != length; i++) ps[i] = {Tp(mapping(i)), i};
                std::sort(ps.begin(), ps.end());
                m_sorted.clear(), m_sorted.reserve(ps.size());
                m_arr.resize(length);
                for (size_type i = 0; i != length; i++) {
                    if (!i || ps[i - 1].m_val < ps[i].m_val) m_sorted.push_back(ps[i].m_val);
                    m_arr[ps[i].m_index] = m_sorted.size() - 1;
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last, size_type query_cnt = 0) {
                resize(last - first, [&](size_type i) { return *(first + i); }, query_cnt);
            }
            void add_query(size_type left, size_type right) { m_queries.push_back({(size_type)m_queries.size(), left, right + 1}); }
            template <ResultTag Tag = ELEM_FREQUENCY>
            std::vector<Result<Tp, Tag>> solve() const {
                if constexpr (Tag == FREQUENCY) {
                    const size_type block_size = std::sqrt(m_queries.size() * 2);
                    std::sort(m_queries.begin(), m_queries.end(), [block_size](const Query &x, const Query &y) {
                        size_type l = x.m_left / block_size, r = y.m_left / block_size;
                        if (l != r)
                            return l < r;
                        else if (l & 1)
                            return x.m_right > y.m_right;
                        else
                            return x.m_right < y.m_right;
                    });
                    std::vector<size_type> cnt_buf(m_sorted.size() + m_arr.size() + 1);
                    const size_type *arr = m_arr.data();
                    size_type *cnt1 = cnt_buf.data(), *cnt2 = cnt_buf.data() + m_sorted.size();
                    size_type l = 0, r = 0, mx = 0, c;
                    auto add = [&](size_type i) {
                        size_type &cnt = cnt1[arr[i]];
                        --cnt2[cnt];
                        ++cnt2[++cnt] > mx;
                        if (cnt > mx) mx = cnt, c = arr[i];
                    };
                    auto remove = [&](size_type i) {
                        size_type &cnt = cnt1[arr[i]];
                        if (cnt == mx && cnt2[cnt] == 1) --mx;
                        --cnt2[cnt];
                        ++cnt2[--cnt];
                    };
                    std::vector<Result<Tp, FREQUENCY>> ans(m_queries.size());
                    for (auto &q : m_queries) {
                        size_type id = q.m_id, left = q.m_left, right = q.m_right;
                        while (l > left) add(--l);
                        while (r < right) add(r++);
                        while (r > right) remove(--r);
                        while (l < left) remove(l++);
                        ans[id] = {mx};
                    }
                    return ans;
                } else {
                    const size_type block_size = std::max<size_type>(1, m_arr.size() / std::sqrt(m_queries.size()));
                    std::sort(m_queries.begin(), m_queries.end(), [block_size](const Query &x, const Query &y) {
                        size_type l = x.m_left / block_size, r = y.m_left / block_size;
                        if (l != r)
                            return l < r;
                        else
                            return x.m_right < y.m_right;
                    });
                    std::vector<size_type> cnt_buf(m_sorted.size());
                    const size_type *arr = m_arr.data();
                    size_type *cnt = cnt_buf.data();
                    size_type l = block_size - 1, r = block_size - 1, mx, c, base_c = 0, base_mx = 0;
                    auto add = [&](size_type i) {
                        auto &_cnt = ++cnt[arr[i]];
                        if (_cnt > mx)
                            mx = _cnt, c = arr[i];
                        else if constexpr (Tag == MINELEM_FREQUENCY)
                            if (_cnt == mx && arr[i] < c) c = arr[i];
                    };
                    auto remove = [&](size_type i) { --cnt[arr[i]]; };
                    std::vector<Result<Tp, Tag>> ans(m_queries.size());
                    for (size_type i = 0, j; i != m_queries.size();) {
                        size_type left = m_queries[i].m_left, start = left / block_size * block_size;
                        for (j = i + 1; j != m_queries.size() && m_queries[j].m_left < start + block_size; j++) {}
                        l = r = start + block_size - 1, base_c = base_mx = 0;
                        for (; i != j && m_queries[i].m_right <= start + block_size; i++) {
                            size_type id = m_queries[i].m_id, left = m_queries[i].m_left, right = m_queries[i].m_right;
                            c = mx = 0;
                            for (size_type i = left; i != right; i++) add(i);
                            ans[id] = {m_sorted[c], mx};
                            for (size_type i = left; i != right; i++) remove(i);
                        }
                        for (; i != j; i++) {
                            size_type id = m_queries[i].m_id, left = m_queries[i].m_left, right = m_queries[i].m_right;
                            while (l != start + block_size - 1) remove(l++);
                            c = base_c, mx = base_mx;
                            while (r != right) add(r++);
                            base_c = c, base_mx = mx;
                            while (l != left) add(--l);
                            ans[id] = {m_sorted[c], mx};
                        }
                        std::fill_n(cnt, m_sorted.size(), 0);
                    }
                    return ans;
                }
            }
        };
    }
}

#endif