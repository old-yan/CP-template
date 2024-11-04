/*
最后修改:
20241030
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DYNAMICFLOYD__
#define __OY_DYNAMICFLOYD__

#include <algorithm>
#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>
#include <vector>

namespace OY {
    namespace DFLOYD {
        using size_type = uint32_t;
        struct Ignore {};
        template <typename Tp, bool GetPath>
        struct DistanceNode {
            Tp m_val;
            size_type m_from;
        };
        template <typename Tp>
        struct DistanceNode<Tp, false> {
            Tp m_val;
        };
        template <typename Tp, bool IsNumeric = std::is_integral<Tp>::value || std::is_floating_point<Tp>::value>
        struct SafeInfinite {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max() / 2; }
        };
        template <typename Tp>
        struct SafeInfinite<Tp, false> {
            static constexpr Tp max() { return std::numeric_limits<Tp>::max(); }
        };
        template <typename ValueType, typename Compare = std::less<ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct AddGroup {
            using value_type = ValueType;
            using compare_type = Compare;
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type identity() { return {}; }
            static value_type infinite() { return Inf; }
        };
        template <typename ValueType, typename Compare = std::less<ValueType>, ValueType Inf = SafeInfinite<ValueType>::max()>
        struct MaxGroup {
            using value_type = ValueType;
            using compare_type = Compare;
            static value_type op(const value_type &x, const value_type &y) { return std::max(x, y); }
            static value_type identity() { return {}; }
            static value_type infinite() { return Inf; }
        };
        template <size_type MAXN>
        struct StaticContainerWrap {
            template <typename Tp>
            struct type {
                Tp m_data[MAXN][MAXN];
                size_type m_vertex_cnt;
                void resize(size_type vertex_cnt, const Tp &infinite) {
                    m_vertex_cnt = vertex_cnt;
                    for (size_type i = 0; i != m_vertex_cnt; i++) std::fill_n(m_data[i], m_vertex_cnt, infinite);
                }
                Tp &get(size_type x, size_type y) { return m_data[x][y]; }
                const Tp &get(size_type x, size_type y) const { return m_data[x][y]; }
            };
        };
        template <typename Tp>
        struct VectorContainer {
            std::vector<Tp> m_data;
            size_type m_vertex_cnt;
            void resize(size_type vertex_cnt, const Tp &infinite) {
                m_vertex_cnt = vertex_cnt;
                m_data.assign(m_vertex_cnt * m_vertex_cnt, infinite);
            }
            Tp &get(size_type x, size_type y) { return m_data[m_vertex_cnt * x + y]; }
            const Tp &get(size_type x, size_type y) const { return m_data[m_vertex_cnt * x + y]; }
        };
        template <typename Group, bool GetPath = false, template <typename> typename Container = VectorContainer>
        struct Solver : Container<DistanceNode<typename Group::value_type, GetPath>> {
            using group = Group;
            using value_type = typename group::value_type;
            using compare_type = typename group::compare_type;
            using node = DistanceNode<value_type, GetPath>;
            using base = Container<node>;
            static value_type infinite() { return group::infinite(); }
            static bool _update(value_type &val, const value_type &dis) { return compare_type()(dis, val) ? val = dis, true : false; }
            Solver() = default;
            Solver(size_type vertex_cnt) { resize(vertex_cnt); }
            void resize(size_type vertex_cnt) {
                node id;
                id.m_val = infinite();
                if constexpr (GetPath) id.m_from = -1;
                base::resize(vertex_cnt, id);
                for (size_type i = 0; i != base::m_vertex_cnt; i++) base::get(i, i).m_val = group::identity();
            }
            void add_edge(size_type from, size_type to, value_type val) {
                auto &v = base::get(from, to);
                if (_update(v.m_val, val))
                    if constexpr (GetPath) v.m_from = -1;
            }
            template <typename Callback = Ignore>
            void run(Callback &&call = Callback()) {
                size_type n = base::m_vertex_cnt;
                for (size_type k = 0; k != n; k++) update_by_vertex(k, call);
            }
            template <typename Callback = Ignore>
            void update_by_edge(size_type from, size_type to, const value_type &val, Callback &&call = Callback()) {
                if (compare_type()(val, base::get(from, to).m_val)) {
                    size_type n = base::m_vertex_cnt;
                    for (size_type i = 0; i != n; i++) {
                        auto &x = base::get(i, to);
                        if (_update(x.m_val, group::op(base::get(i, from).m_val, val))) {
                            if constexpr (GetPath) x.m_from = i != from ? from : -1;
                            if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(i, to, x.m_val);
                            for (size_type j = 0; j != n; j++) {
                                auto &x2 = base::get(i, j);
                                if (_update(x2.m_val, group::op(x.m_val, base::get(to, j).m_val))) {
                                    if constexpr (GetPath) x2.m_from = to;
                                    if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(i, j, x2.m_val);
                                }
                            }
                        }
                    }
                }
            }
            template <typename Callback = Ignore>
            void update_by_vertex(size_type k, Callback &&call = Callback()) {
                size_type n = base::m_vertex_cnt;
                for (size_type i = 0; i != n; i++) {
                    auto &x = base::get(i, k);
                    if (compare_type()(x.m_val, infinite())) {
                        for (size_type j = 0; j != n; j++) {
                            auto &x2 = base::get(i, j);
                            if (_update(x2.m_val, group::op(x.m_val, base::get(k, j).m_val))) {
                                if constexpr (GetPath) x2.m_from = k;
                                if constexpr (!std::is_same<typename std::decay<Callback>::type, Ignore>::value) call(i, j, x2.m_val);
                            }
                        }
                    }
                }
            }
            const value_type &query(size_type from, size_type to) const { return base::get(from, to).m_val; }
        };
    }
    template <typename Group, bool GetPath = false>
    using VectorDynamicFloyd = DFLOYD::Solver<Group, GetPath>;
    template <typename Group, bool GetPath = false, DFLOYD::size_type MAXN = 500>
    using StaticDynamicFloyd = DFLOYD::Solver<Group, GetPath, DFLOYD::StaticContainerWrap<MAXN>::template type>;
}

#endif