/*
最后修改:
20250209
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MAPPEDMONOZKWTREE__
#define __OY_MAPPEDMONOZKWTREE__

#include <queue>

#include "MonoZkwTree.h"

namespace OY {
    namespace MMZKW {
        using size_type = MONOZKW::size_type;
        template <typename Tp, typename Compare, Tp Identity>
        struct PqMapping {
            using container_type = std::priority_queue<Tp, std::vector<Tp>, Compare>;
            static Tp get(const container_type &bucket) {
                if (bucket.empty()) return Identity;
                return bucket.top();
            }
        };
        template <typename Monoid, typename Mapping>
        class Tree {
            using inner_type = MONOZKW::Tree<Monoid>;
            using container_type = typename Mapping::container_type;
            inner_type m_zkw;
            std::vector<container_type> m_buckets;
        public:
            Tree() = default;
            Tree(size_type n) { resize(n); }
            void resize(size_type n) {
                m_buckets.clear(), m_buckets.reserve(n);
                for(size_type i=0;i!=n;i++)m_buckets.emplace_back();
                m_zkw.resize(n, [&](size_type i) { return Mapping::get(m_buckets[i]); });
            }
            container_type &operator[](size_type i) { return m_buckets[i]; }
            const container_type &operator[](size_type i) const { return m_buckets[i]; }
            void update(size_type i) { m_zkw.modify(i, Mapping::get(m_buckets[i])); }
            auto query(size_type i) const -> decltype(m_zkw.query(i)) { return m_zkw.query(i); }
            auto query(size_type left, size_type right) const -> decltype(m_zkw.query(left, right)) { return m_zkw.query(left, right); }
            auto query_all() const -> decltype(m_zkw.query_all()) { return m_zkw.query_all(); }
            template <typename Judger>
            size_type max_right(size_type left, Judger &&judge) const { return m_zkw.max_right(left, judge); }
            template <typename Judger>
            size_type min_left(size_type right, Judger &&judge) const { return m_zkw.min_left(right, judge); }
        };
    }
    template <typename Monoid, typename Tp, Tp Minimum = std::numeric_limits<Tp>::min()>
    using MaxPqMonoZkwTree = MMZKW::Tree<Monoid, MMZKW::PqMapping<Tp, std::less<Tp>, Minimum>>;
    template <typename Monoid, typename Tp, Tp Maximum = std::numeric_limits<Tp>::max()>
    using MinPqMonoZkwTree = MMZKW::Tree<Monoid, MMZKW::PqMapping<Tp, std::greater<Tp>, Maximum>>;
}

#endif