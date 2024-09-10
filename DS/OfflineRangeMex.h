/*
最后修改:
20240826
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_OFFLINERANGEMEX__
#define __OY_OFFLINERANGEMEX__

#include "LinkBucket.h"
#include "MonoZkwTree.h"

namespace OY {
    class OfflineRangeMexSolver {
        using size_type = uint32_t;
        struct query {
            size_type m_left, m_id;
        };
        size_type m_qid;
        LBC::LinkBucket<query> m_queries;
    public:
        OfflineRangeMexSolver(size_type length, size_type query_cnt) { resize(length, query_cnt); }
        void resize(size_type length, size_type query_cnt) {
            m_qid = 0;
            m_queries.resize(length, query_cnt);
        }
        void add_query(size_type left, size_type right) { m_queries[right].push_front(query{left, m_qid++}); }
        template <typename Mapping>
        std::vector<size_type> solve(size_type length, Mapping mapping) const {
            std::vector<size_type> ans(length);
            MonoMinTree<size_type> last(length, [&](size_type i) { return 0; });
            for (size_type r = 0; r != length; r++) {
                size_type val = mapping(r);
                if (val < length) last.modify(val, r + 1);
                for (auto &q : m_queries[r]) {
                    auto check = [left = q.m_left](size_type v) { return v > left; };
                    ans[q.m_id] = std::min(length, last.max_right(0, check) + 1);
                }
            }
            return ans;
        }
        template <typename Iterator>
        std::vector<size_type> solve(Iterator first, Iterator last) const {
            return solve(last - first, [&](size_type i) { return *(first + i); });
        }
    };
}

#endif