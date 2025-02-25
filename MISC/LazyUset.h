/*
最后修改:
20241026
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_LAZYUSET__
#define __OY_LAZYUSET__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <unordered_set>

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
    namespace LAZYUSET {
        using namespace LAZYIMP;
        template <typename Key>
        struct TableBase;
        template <typename CommutativeGroup>
        struct TableBase<LazyType<CommutativeGroup>> {
            using key_type = typename CommutativeGroup::value_type;
            using group = CommutativeGroup;
            key_type m_lazy;
            void clear() { m_lazy = group::identity(); }
            void globally_add(key_type inc) { m_lazy = group::op(m_lazy, inc); }
            key_type _ki2o(const key_type &k) const { return group::op(k, m_lazy); }
            key_type _ko2i(const key_type &k) const { return group::op(k, group::inverse(m_lazy)); }
        };
        template <typename Key, typename Hash = std::hash<typename Trait<Key>::type>, typename Pred = std::equal_to<typename Trait<Key>::type>>
        struct Table : TableBase<Key> {
            using base = TableBase<Key>;
            using key_type = typename base::key_type;
            using inner_table = std::unordered_set<key_type, Hash, Pred>;
            using iterator = typename inner_table::iterator;
            mutable inner_table m_set;
            Table() { base::clear(); }
            iterator begin() const { return m_set.begin(); }
            iterator end() const { return m_set.end(); }
            iterator find(key_type k) const { return m_set.find(base::_ko2i(k)); }
            bool contains(key_type k) const { return find(k) != m_set.end(); }
            bool empty() const { return m_set.empty(); }
            auto size() const -> decltype(m_set.size()) { return m_set.size(); }
            void clear() { inner_table().swap(m_set), base::clear(); }
            auto insert(key_type k) -> decltype(m_set.emplace(k)) { return m_set.insert(base::_ko2i(k)); }
            bool erase(key_type k) { return m_set.erase(base::_ko2i(k)); }
            template <typename Callback>
            void enumerate(Callback &&call) const {
                for (auto it = m_set.begin(); it != m_set.end(); ++it) call(base::_ki2o(*it));
            }
        };
        template <typename Ostream, typename Key, typename Hash, typename Pred>
        Ostream &operator<<(Ostream &out, const Table<Key, Hash, Pred> &x) {
            out << '{';
            bool started = false;
            auto call = [&](const typename LAZYIMP::Trait<Key>::type &k) {
                if (started)
                    out << ", ";
                else
                    started = true;
                out << k;
            };
            x.enumerate(call);
            return out << '}';
        }
    }
    namespace LAZYIMP {
        template <typename Key, typename Hash, typename Pred>
        struct Trait<LAZYUSET::Table<Key, Hash, Pred>> {
            struct pair {
                LAZYUSET::Table<Key, Hash, Pred> m_val;
                typename LAZYUSET::Table<Key, Hash, Pred>::group::value_type m_inv;
            };
            static constexpr bool is_table = true;
        };
    }
    template <typename Key, typename Hash = std::hash<typename LAZYIMP::Trait<Key>::type>, typename Pred = std::equal_to<typename LAZYIMP::Trait<Key>::type>>
    using LazyKeyAddUset = LAZYUSET::Table<LAZYUSET::LazyAdd<Key>, Hash, Pred>;
}

#endif