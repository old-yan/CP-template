/*
最后修改:
20240912
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_POTENTIALIZEDDSU__
#define __OY_POTENTIALIZEDDSU__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_gcd_lcm.h"

namespace OY {
    namespace PDSU {
        using size_type = uint32_t;
        template <typename ValueType>
        struct AddGroup {
            using value_type = ValueType;
            static value_type identity() { return value_type{}; }
            static value_type op(const value_type &x, const value_type &y) { return x + y; }
            static value_type inverse(const value_type &x) { return -x; }
        };
        template <typename ValueType, ValueType Mod>
        struct ModAddGroup {
            using value_type = ValueType;
            static value_type identity() { return value_type{}; }
            static value_type op(const value_type &x, const value_type &y) { return x + y - (x + y >= Mod ? Mod : 0); }
            static value_type inverse(const value_type &x) { return x ? Mod - x : 0; }
        };
        template <typename Group, bool MaintainGroupSize>
        class Table {
        public:
            using group = Group;
            using value_type = typename group::value_type;
            struct info {
                size_type m_head;
                value_type m_val;
            };
        private:
            mutable std::vector<size_type> m_parent, m_group_size;
            mutable std::vector<value_type> m_dis;
            size_type m_size, m_group_cnt;
        public:
            Table(size_type n = 0) { resize(n); }
            void resize(size_type n) {
                if (!(m_size = m_group_cnt = n)) return;
                m_parent.resize(m_size);
                if constexpr (MaintainGroupSize) m_group_size.resize(m_size, 1);
                std::iota(m_parent.begin(), m_parent.end(), 0);
                m_dis.assign(m_size, group::identity());
            }
            info find(size_type i) const {
                if (m_parent[i] == i) return {i, m_dis[i]};
                auto res = find(m_parent[i]);
                m_parent[i] = res.m_head;
                m_dis[i] = group::op(m_dis[i], res.m_val);
                return {m_parent[i], m_dis[i]};
            }
            template <bool IsHead = false>
            size_type size(size_type i) const {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                if constexpr (IsHead)
                    return m_group_size[i];
                else
                    return m_group_size[find(i)];
            }
            void unite_to(size_type head_a, size_type head_b, value_type dis) {
                m_parent[head_a] = head_b;
                if constexpr (MaintainGroupSize) m_group_size[head_b] += m_group_size[head_a];
                m_dis[head_a] = dis;
                m_group_cnt--;
            }
            bool unite_by_size(size_type a, size_type b, value_type dis) {
                static_assert(MaintainGroupSize, "MaintainGroupSize Must Be True");
                info ia = find(a), ib = find(b);
                if (ia.m_head == ib.m_head) return false;
                dis = group::op(group::op(group::inverse(ia.m_val), dis), ib.m_val);
                if (m_group_size[ia.m_head] > m_group_size[ib.m_head]) std::swap(ia, ib), dis = group::inverse(dis);
                unite_to(ia.m_head, ib.m_head, dis);
                return true;
            }
            bool unite_by_ID(size_type a, size_type b, value_type dis) {
                info ia = find(a), ib = find(b);
                if (ia.m_head == ib.m_head) return false;
                dis = group::op(group::op(group::inverse(ia.m_val), dis), ib.m_val);
                if (a < b) std::swap(a, b), dis = group::inverse(dis);
                unite_to(a, b, dis);
                return true;
            }
            std::pair<bool, value_type> calc(size_type a, size_type b) const {
                info ia = find(a), ib = find(b);
                if (ia.m_head != ib.m_head) return std::make_pair(false, group::identity());
                return std::make_pair(true, group::op(ia.m_val, group::inverse(ib.m_val)));
            }
            bool is_head(size_type i) const { return i == m_parent[i]; }
            size_type count() const { return m_group_cnt; }
            std::vector<size_type> heads() const {
                std::vector<size_type> ret;
                ret.reserve(m_group_cnt);
                for (size_type i = 0; i != m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() const {
                if constexpr (MaintainGroupSize) {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) ret[j].reserve(m_group_size[i]), index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i).m_head]].push_back(i);
                    return ret;
                } else {
                    std::vector<std::vector<size_type>> ret(m_group_cnt);
                    std::vector<size_type> index(m_size), cnt(m_group_cnt);
                    for (size_type i = 0, j = 0; i != m_size; i++)
                        if (is_head(i)) index[i] = j++;
                    for (size_type i = 0; i != m_size; i++) cnt[index[find(i).m_head]]++;
                    for (size_type i = 0; i != m_group_cnt; i++) ret[i].reserve(cnt[i]);
                    for (size_type i = 0; i != m_size; i++) ret[index[find(i).m_head]].push_back(i);
                    return ret;
                }
            }
        };
    }
    template <typename Tp, bool MaintainGroupSize>
    using AddPDSUTable = PDSU::Table<PDSU::AddGroup<Tp>, MaintainGroupSize>;
    template <typename Tp, Tp Mod, bool MaintainGroupSize>
    using ModAddPDSUTable = PDSU::Table<PDSU::ModAddGroup<Tp, Mod>, MaintainGroupSize>;
}

#endif