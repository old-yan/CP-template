/*
最后修改:
20241024
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LAZYMAP__
#define __OY_LAZYMAP__

#include <algorithm>
#include <cstdint>
#include <map>
#include <numeric>

namespace OY {
#ifndef __OY_LAZYIMP__
#define __OY_LAZYIMP__
    namespace LAZYIMP {
        template <typename Tp>
        struct AddCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x + y; }
            static Tp inverse(const Tp &x) { return -x; }
        };
        template <typename Tp>
        struct BitxorCommutativeGroup {
            using value_type = Tp;
            static Tp identity() { return Tp{}; }
            static Tp op(const Tp &x, const Tp &y) { return x ^ y; }
            static Tp inverse(const Tp &x) { return x; }
        };
        template <typename CommutativeGroup>
        struct LazyType {};
        template <typename Tp>
        using LazyAdd = LazyType<AddCommutativeGroup<Tp>>;
        template <typename Tp>
        using LazyBitXor = LazyType<BitxorCommutativeGroup<Tp>>;
        template <typename Tp>
        struct Trait {
            using type = Tp;
            static constexpr bool is_table = false;
        };
        template <typename CommutativeGroup>
        struct Trait<LazyType<CommutativeGroup>> {
            using type = typename CommutativeGroup::value_type;
            struct pair {
                type m_val, m_inv;
            };
            static constexpr bool is_table = false;
        };
    }
#endif
    namespace LAZYMAP {
        using namespace LAZYIMP;
        template <typename Key, typename Mapped, bool IsTable = Trait<Mapped>::is_table>
        struct TableBase;
        template <typename Key, typename CommutativeGroup>
        struct TableBase<Key, LazyType<CommutativeGroup>, false> {
            using key_type = Key;
            using mapped_type = typename CommutativeGroup::value_type;
            using inner_mapped_type = typename Trait<LazyType<CommutativeGroup>>::pair;
            using group = CommutativeGroup;
            mapped_type m_lazy;
            void clear() { m_lazy = group::identity(); }
            void globally_add(mapped_type inc) { m_lazy = group::op(m_lazy, inc); }
            const Key &_ki2o(const Key &k) const { return k; }
            const Key &_ko2i(const Key &k) const { return k; }
            void _pushdown(inner_mapped_type &p) const { p.m_val = group::op(p.m_val, group::op(p.m_inv, m_lazy)), p.m_inv = group::inverse(m_lazy); }
            template <typename It>
            It _reduced(It it) const { return _pushdown(it->second), it; }
            inner_mapped_type _make_mapped(const mapped_type &m) const { return {m, group::inverse(m_lazy)}; }
            static mapped_type &_get(inner_mapped_type &p) { return p.m_val; }
        };
        template <typename CommutativeGroup, typename Mapped>
        struct TableBase<LazyType<CommutativeGroup>, Mapped, false> {
            using key_type = typename CommutativeGroup::value_type;
            using mapped_type = Mapped;
            using inner_mapped_type = Mapped;
            using group = CommutativeGroup;
            key_type m_lazy;
            void clear() { m_lazy = group::identity(); }
            void globally_add(key_type inc) { m_lazy = group::op(m_lazy, inc); }
            key_type _ki2o(const key_type &k) const { return group::op(k, m_lazy); }
            key_type _ko2i(const key_type &k) const { return group::op(k, group::inverse(m_lazy)); }
            static void _pushdown(inner_mapped_type &) {}
            template <typename It>
            static It _reduced(It it) { return it; }
            static const Mapped &_make_mapped(const Mapped &m) { return m; }
            static Mapped &_get(Mapped &m) { return m; }
        };
        template <typename CommutativeGroup1, typename CommutativeGroup2>
        struct TableBase<LazyType<CommutativeGroup1>, LazyType<CommutativeGroup2>, false> {
            using key_type = typename CommutativeGroup1::value_type;
            using mapped_type = typename CommutativeGroup2::value_type;
            using inner_mapped_type = typename Trait<LazyType<CommutativeGroup2>>::pair;
            struct group {
                struct value_type {
                    key_type m_key_lazy;
                    mapped_type m_mapped_lazy;
                };
                static value_type identity() { return {CommutativeGroup1::identity(), CommutativeGroup2::identity()}; }
                static value_type op(const value_type &x, const value_type &y) { return {CommutativeGroup1::op(x.m_key_lazy, y.m_key_lazy), CommutativeGroup2::op(x.m_mapped_lazy, y.m_mapped_lazy)}; }
                static value_type inverse(const value_type &x) { return {CommutativeGroup1::inverse(x.m_key_lazy), CommutativeGroup2::inverse(x.m_mapped_lazy)}; }
            };
            typename group::value_type m_lazy;
            void clear() { m_lazy = group::identity(); }
            void globally_add(typename group::value_type inc) { m_lazy = group::op(m_lazy, inc); }
            key_type _ki2o(const key_type &k) const { return CommutativeGroup1::op(k, m_lazy.m_key_lazy); }
            key_type _ko2i(const key_type &k) const { return CommutativeGroup1::op(k, CommutativeGroup1::inverse(m_lazy.m_key_lazy)); }
            void _pushdown(inner_mapped_type &p) const { p.m_val = CommutativeGroup2::op(p.m_val, CommutativeGroup2::op(p.m_inv, m_lazy.m_mapped_lazy)), p.m_inv = CommutativeGroup2::inverse(m_lazy.m_mapped_lazy); }
            template <typename It>
            It _reduced(It it) const { return _pushdown(it->second), it; }
            inner_mapped_type _make_mapped(const mapped_type &m) const { return {m, CommutativeGroup2::inverse(m_lazy.m_mapped_lazy)}; }
            static mapped_type &_get(inner_mapped_type &p) { return p.m_val; }
        };
        template <typename Key, typename Table>
        struct TableBase<Key, Table, true> {
            using key_type = Key;
            using mapped_type = Table;
            using inner_mapped_type = typename Trait<mapped_type>::pair;
            using group = typename mapped_type::group;
            typename group::value_type m_lazy;
            void clear() { m_lazy = group::identity(); }
            void globally_add(typename group::value_type inc) { m_lazy = group::op(m_lazy, inc); }
            const Key &_ki2o(const Key &k) const { return k; }
            const Key &_ko2i(const Key &k) const { return k; }
            void _pushdown(inner_mapped_type &p) const { p.m_val.globally_add(group::op(p.m_inv, m_lazy)), p.m_inv = group::inverse(m_lazy); }
            template <typename It>
            It _reduced(It it) const { return _pushdown(it->second), it; }
            inner_mapped_type _make_mapped(mapped_type &&m) const { return {std::move(m), group::inverse(m_lazy)}; }
            static mapped_type &_get(inner_mapped_type &p) { return p.m_val; }
        };
        template <typename CommutativeGroup, typename Table>
        struct TableBase<LazyType<CommutativeGroup>, Table, true> {
            using key_type = typename CommutativeGroup::value_type;
            using mapped_type = Table;
            using inner_mapped_type = typename Trait<mapped_type>::pair;
            using sub_group = typename mapped_type::group;
            struct group {
                struct value_type {
                    key_type m_key_lazy;
                    typename sub_group::value_type m_mapped_lazy;
                };
                static value_type identity() { return {CommutativeGroup::identity(), sub_group::identity()}; }
                static value_type op(const value_type &x, const value_type &y) { return {CommutativeGroup::op(x.m_key_lazy, y.m_key_lazy), sub_group::op(x.m_mapped_lazy, y.m_mapped_lazy)}; }
                static value_type inverse(const value_type &x) { return {CommutativeGroup::inverse(x.m_key_lazy), sub_group::inverse(x.m_mapped_lazy)}; }
            };
            typename group::value_type m_lazy;
            void clear() { m_lazy = group::identity(); }
            void globally_add(typename group::value_type inc) { m_lazy = group::op(m_lazy, inc); }
            key_type _ki2o(const key_type &k) const { return CommutativeGroup::op(k, m_lazy.m_key_lazy); }
            key_type _ko2i(const key_type &k) const { return CommutativeGroup::op(k, CommutativeGroup::inverse(m_lazy.m_key_lazy)); }
            void _pushdown(inner_mapped_type &p) const { p.m_val.globally_add(sub_group::op(p.m_inv, m_lazy.m_mapped_lazy)), p.m_inv = sub_group::inverse(m_lazy.m_mapped_lazy); }
            template <typename It>
            It _reduced(It it) const { return _pushdown(it->second), it; }
            inner_mapped_type _make_mapped(mapped_type &&m) const { return {std::move(m), sub_group::inverse(m_lazy.m_mapped_lazy)}; }
            static mapped_type &_get(inner_mapped_type &p) { return p.m_val; }
        };
        template <typename Key, typename Mapped, typename Compare = std::less<typename Trait<Key>::type>>
        struct Table : TableBase<Key, Mapped> {
            using base = TableBase<Key, Mapped>;
            using key_type = typename base::key_type;
            using mapped_type = typename base::mapped_type;
            using inner_mapped_type = typename base::inner_mapped_type;
            using inner_table = std::map<key_type, inner_mapped_type, Compare>;
            using iterator = typename inner_table::iterator;
            mutable inner_table m_map;
            Table() { base::clear(); }
            iterator begin() const {
                auto it = m_map.begin();
                return it == m_map.end() ? it : base::_reduced(it);
            }
            iterator end() const { return m_map.end(); }
            iterator find(key_type k) const {
                auto it = m_map.find(base::_ko2i(k));
                return it == m_map.end() ? it : base::_reduced(it);
            }
            iterator lower_bound(key_type k) const {
                auto it = m_map.lower_bound(base::_ko2i(k));
                return it == m_map.end() ? it : base::_reduced(it);
            }
            iterator upper_bound(key_type k) const {
                auto it = m_map.upper_bound(base::_ko2i(k));
                return it == m_map.end() ? it : base::_reduced(it);
            }
            bool contains(key_type k) const { return find(k) != m_map.end(); }
            bool empty() const { return m_map.empty(); }
            auto size() const -> decltype(m_map.size()) { return m_map.size(); }
            void clear() { inner_table().swap(m_map), base::clear(); }
            auto insert_or_assign(key_type k, mapped_type m) -> decltype(m_map.emplace(k, inner_mapped_type())) {
                auto res = m_map.emplace(base::_ko2i(k), base::_make_mapped(std::move(m)));
                if (!res.second) base::_pushdown(res.first->second), base::_get(res.first->second) = m;
                return res;
            }
            auto insert_or_ignore(key_type k, mapped_type m) -> decltype(m_map.emplace(k, inner_mapped_type())) { return m_map.emplace(base::_ko2i(k), base::_make_mapped(std::move(m))); }
            template <typename Callback>
            auto insert_or_modify(key_type k, Callback &&call) -> decltype(m_map.emplace(k, inner_mapped_type())) {
                auto res = m_map.emplace(base::_ko2i(k), base::_make_mapped({}));
                base::_pushdown(res.first->second);
                call(base::_get(res.first->second), res.second);
                return res;
            }
            bool erase(key_type k) { return m_map.erase(base::_ko2i(k)); }
            const mapped_type &get(key_type k, const mapped_type &_default) const {
                auto res = find(k);
                return res != m_map.end() ? base::_get(res->second) : _default;
            }
            const mapped_type &get(key_type k) const { return base::_get(find(k)->second); }
            mapped_type &get(key_type k) {
                auto res = m_map.emplace(base::_ko2i(k), base::_make_mapped({}));
                return base::_get(base::_reduced(res.first)->second);
            }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                for (auto it = m_map.begin(); it != m_map.end(); ++it) {
                    base::_pushdown(it->second);
                    call(base::_ki2o(it->first), base::_get(it->second));
                }
            }
        };
        template <typename Ostream, typename Key, typename Mapped, typename Compare>
        Ostream &operator<<(Ostream &out, const Table<Key, Mapped, Compare> &x) {
            out << '{';
            bool started = false;
            auto call = [&](const typename LAZYIMP::Trait<Key>::type &k, const typename Table<Key, Mapped, Compare>::mapped_type &v) {
                if (started)
                    out << ", ";
                else
                    started = true;
                out << k << ": " << v;
            };
            x.enumerate(call);
            return out << '}';
        }
    }
    namespace LAZYIMP {
        template <typename Key, typename Mapped, typename Compare>
        struct Trait<LAZYMAP::Table<Key, Mapped, Compare>> {
            struct pair {
                LAZYMAP::Table<Key, Mapped, Compare> m_val;
                typename LAZYMAP::Table<Key, Mapped, Compare>::group::value_type m_inv;
            };
            static constexpr bool is_table = true;
        };
    }
    template <typename Key, typename Mapped, typename Compare = std::less<typename LAZYIMP::Trait<Key>::type>>
    using LazyKeyAddMap = LAZYMAP::Table<LAZYMAP::LazyAdd<Key>, Mapped, Compare>;
    template <typename Key, typename Mapped, typename Compare = std::less<typename LAZYIMP::Trait<Key>::type>>
    using LazyMappedAddMap = LAZYMAP::Table<Key, LAZYMAP::LazyAdd<Mapped>, Compare>;
    template <typename Key, typename Mapped, typename Compare = std::less<typename LAZYIMP::Trait<Key>::type>>
    using LazyKeyAddMappedAddMap = LAZYMAP::Table<LAZYMAP::LazyAdd<Key>, LAZYMAP::LazyAdd<Mapped>, Compare>;
}

#endif