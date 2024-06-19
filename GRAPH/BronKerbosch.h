/*
最后修改:
20231106
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BRONKERBOSCH__
#define __OY_BRONKERBOSCH__

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace BK {
        using size_type = uint32_t;
        template <typename MaskType, size_type MAX_VERTEX>
        struct Graph {
            static constexpr size_type mask_size = sizeof(MaskType) << 3, mask_count = MAX_VERTEX / mask_size + 1;
            struct node {
                MaskType m_mask[mask_count];
                void set_true(size_type i) { m_mask[i / mask_size] |= MaskType(1) << (i & (mask_size - 1)); }
                void set_false(size_type i) { m_mask[i / mask_size] &= -(MaskType(1) << (i & (mask_size - 1))) - 1; }
                size_type count_one(size_type range) const {
                    if constexpr (mask_count == 1)
                        return std::popcount(m_mask[0] & ((MaskType(1) << range) - 1));
                    else if (range < mask_size)
                        return std::popcount(m_mask[0] & ((MaskType(1) << range) - 1));
                    else {
                        size_type res = 0, index = 0;
                        while (mask_size * (index + 1) <= range) res += std::popcount(m_mask[index++]);
                        return res + std::popcount(m_mask[index] & ((MaskType(1) << (range & (mask_size - 1))) - 1));
                    }
                }
                size_type count_zero(size_type first, size_type range) const {
                    if constexpr (mask_count == 1)
                        return mask_size - std::popcount(m_mask[0] | ~((MaskType(1) << range) - (MaskType(1) << first)));
                    else if (range < mask_size)
                        return mask_size - std::popcount(m_mask[0] | ~((MaskType(1) << range) - (MaskType(1) << first)));
                    else
                        return count_zero(range) - count_zero(first - 1);
                }
                size_type count_zero(size_type range) const {
                    if constexpr (mask_count == 1)
                        return mask_size - std::popcount(m_mask[0] | -(MaskType(1) << range));
                    else if (range < mask_size)
                        return mask_size - std::popcount(m_mask[0] | -(MaskType(1) << range));
                    else {
                        size_type res = 0, index = 0;
                        while (mask_size * (index + 1) <= range) res += mask_size - std::popcount(m_mask[index++]);
                        return res + mask_size - std::popcount(m_mask[index] | -(MaskType(1) << (range & (mask_size - 1))));
                    }
                }
                size_type find_one(size_type cur) const {
                    size_type i = cur / mask_size;
                    MaskType x = m_mask[i] & -(MaskType(1) << (cur & (mask_size - 1)));
                    if (x) return i * mask_size | std::countr_zero(x);
                    while (!m_mask[++i]) {}
                    return i * mask_size | std::countr_zero(m_mask[i]);
                }
                size_type find_zero(size_type cur) const {
                    size_type i = cur / mask_size;
                    MaskType x = m_mask[i] | ((MaskType(1) << (cur & (mask_size - 1))) - 1);
                    if (~x) return i * mask_size | std::countr_zero(~x);
                    while (!~m_mask[++i]) {}
                    return i * mask_size | std::countr_zero(~m_mask[i]);
                }
                node operator&(const node &rhs) const {
                    node res;
                    for (size_type i = 0; i != mask_count; i++) res.m_mask[i] = m_mask[i] & rhs.m_mask[i];
                    return res;
                }
                node operator|(const node &rhs) const {
                    node res;
                    for (size_type i = 0; i != mask_count; i++) res.m_mask[i] = m_mask[i] | rhs.m_mask[i];
                    return res;
                }
            };
            size_type m_vertex_cnt, m_max_clique, m_max_independant_set;
            node m_adj[MAX_VERTEX];
            void _dfs_clique(size_type cur, size_type cnt, node &&next, const std::vector<std::pair<size_type, size_type>> &suf, size_type thresh) {
                if (cnt + next.count_one(m_vertex_cnt) > m_max_clique) {
                    while (true) {
                        size_type nx = next.find_one(cur + 1);
                        if (nx == m_vertex_cnt || cnt + suf[nx].second <= m_max_clique) break;
                        if (cnt + suf[nx].first > m_max_clique) {
                            _dfs_clique(nx, cnt + 1, next & m_adj[nx], suf, thresh);
                            if (m_max_clique == thresh) return;
                        }
                        cur = nx;
                    }
                    if (cnt > m_max_clique) m_max_clique = cnt;
                }
            }
            void _dfs_independant_set(size_type cur, size_type cnt, node &&next, const std::vector<std::pair<size_type, size_type>> &suf, size_type thresh) {
                if (cnt + next.count_zero(cur + 1, m_vertex_cnt) > m_max_independant_set) {
                    while (true) {
                        size_type nx = next.find_zero(cur + 1);
                        if (nx == m_vertex_cnt || cnt + suf[nx].second <= m_max_independant_set) break;
                        if (cnt + suf[nx].first > m_max_independant_set) {
                            _dfs_independant_set(nx, cnt + 1, next | m_adj[nx], suf, thresh);
                            if (m_max_independant_set == thresh) return;
                        }
                        cur = nx;
                    }
                    if (cnt > m_max_independant_set) m_max_independant_set = cnt;
                }
            }
            bool _find_clique(size_type cur, size_type cnt, node &&next, const std::vector<std::pair<size_type, size_type>> &suf, std::vector<size_type> &res) {
                if (cnt == m_max_clique) return res.push_back(cur), true;
                if (cnt + next.count_one(m_vertex_cnt) >= m_max_clique)
                    for (size_type x = cur, nx;; x = nx) {
                        nx = next.find_one(x + 1);
                        if (nx == m_vertex_cnt || cnt + suf[nx].second < m_max_clique) break;
                        if (cnt + suf[nx].first >= m_max_clique && _find_clique(nx, cnt + 1, next & m_adj[nx], suf, res)) return res.push_back(cur), true;
                    }
                return false;
            }
            bool _find_independant_set(size_type cur, size_type cnt, node &&next, const std::vector<std::pair<size_type, size_type>> &suf, std::vector<size_type> &res) {
                if (cnt == m_max_independant_set) return res.push_back(cur), true;
                if (cnt + next.count_zero(cur + 1, m_vertex_cnt) >= m_max_independant_set)
                    for (size_type x = cur, nx;; x = nx) {
                        nx = next.find_zero(x + 1);
                        if (nx == m_vertex_cnt || cnt + suf[nx].second < m_max_independant_set) break;
                        if (cnt + suf[nx].first >= m_max_independant_set && _find_independant_set(nx, cnt + 1, next | m_adj[nx], suf, res)) return res.push_back(cur), true;
                    }
                return false;
            }
            template <typename Callback>
            void _dfs_clique(size_type cur, size_type cnt, node &&next, std::vector<size_type> &clique, Callback &&call) const {
                clique.push_back(cur), call(clique);
                size_type x = cur;
                while (true) {
                    size_type nx = next.find_one(x + 1);
                    if (nx == m_vertex_cnt) break;
                    _dfs_clique(nx, cnt + 1, next & m_adj[nx], clique, call), x = nx;
                }
                clique.pop_back();
            }
            template <typename Callback>
            void _dfs_independant_set(size_type cur, size_type cnt, node &&next, std::vector<size_type> &independant_set, Callback &&call) const {
                independant_set.push_back(cur), call(independant_set);
                size_type x = cur;
                while (true) {
                    size_type nx = next.find_zero(x + 1);
                    if (nx == m_vertex_cnt) break;
                    _dfs_independant_set(nx, cnt + 1, next | m_adj[nx], independant_set, call), x = nx;
                }
                independant_set.pop_back();
            }
            Graph(size_type vertex_cnt) : m_vertex_cnt(vertex_cnt) {
                for (size_type i = 0; i != m_vertex_cnt; i++)
                    for (size_type j = 0; j != mask_count; j++) m_adj[i].m_mask[j] = 0;
            }
            void add_edge(size_type i, size_type j) {
                if (i > j) std::swap(i, j);
                m_adj[i].set_true(j);
            }
            size_type max_clique() {
                std::vector<std::pair<size_type, size_type>> suf(m_vertex_cnt);
                size_type his_max{};
                for (size_type i = 0; i != m_vertex_cnt; i++) m_adj[i].set_true(m_vertex_cnt);
                for (size_type first = m_vertex_cnt - 1; ~first; first--) {
                    m_max_clique = 0;
                    _dfs_clique(first, 1, std::move(m_adj[first]), suf, his_max + 1);
                    suf[first].first = m_max_clique;
                    if (m_max_clique > his_max) his_max = m_max_clique;
                    suf[first].second = his_max;
                }
                return m_max_clique = his_max;
            }
            size_type max_independant_set() {
                std::vector<std::pair<size_type, size_type>> suf(m_vertex_cnt);
                size_type his_max{};
                for (size_type i = 0; i != m_vertex_cnt; i++) m_adj[i].set_false(m_vertex_cnt);
                for (size_type first = m_vertex_cnt - 1; ~first; first--) {
                    m_max_independant_set = 0;
                    _dfs_independant_set(first, 1, std::move(m_adj[first]), suf, his_max + 1);
                    suf[first].first = m_max_independant_set;
                    if (m_max_independant_set > his_max) his_max = m_max_independant_set;
                    suf[first].second = his_max;
                }
                return m_max_independant_set = his_max;
            }
            std::vector<size_type> get_max_clique() {
                std::vector<std::pair<size_type, size_type>> suf(m_vertex_cnt);
                size_type his_max{};
                for (size_type i = 0; i != m_vertex_cnt; i++) m_adj[i].set_true(m_vertex_cnt);
                for (size_type first = m_vertex_cnt - 1; ~first; first--) {
                    m_max_clique = 0;
                    _dfs_clique(first, 1, std::move(m_adj[first]), suf, his_max + 1);
                    suf[first].first = m_max_clique;
                    if (m_max_clique > his_max) his_max = m_max_clique;
                    suf[first].second = his_max;
                }
                std::vector<size_type> res;
                res.reserve(m_max_clique = his_max);
                size_type first = 0;
                while (suf[first].first != his_max) first++;
                _find_clique(first, 1, std::move(m_adj[first]), suf, res);
                std::reverse(res.begin(), res.end());
                return res;
            }
            std::vector<size_type> get_max_independant_set() {
                std::vector<std::pair<size_type, size_type>> suf(m_vertex_cnt);
                size_type his_max{};
                for (size_type i = 0; i != m_vertex_cnt; i++) m_adj[i].set_false(m_vertex_cnt);
                for (size_type first = m_vertex_cnt - 1; ~first; first--) {
                    m_max_independant_set = 0;
                    _dfs_independant_set(first, 1, std::move(m_adj[first]), suf, his_max + 1);
                    suf[first].first = m_max_independant_set;
                    if (m_max_independant_set > his_max) his_max = m_max_independant_set;
                    suf[first].second = his_max;
                }
                std::vector<size_type> res;
                res.reserve(m_max_independant_set = his_max);
                size_type first = 0;
                while (suf[first].first != his_max) first++;
                _find_independant_set(first, 1, std::move(m_adj[first]), suf, res);
                std::reverse(res.begin(), res.end());
                return res;
            }
            template <typename Callback>
            void enumerate_clique(Callback &&call) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_adj[i].set_true(m_vertex_cnt);
                std::vector<size_type> clique;
                for (size_type first = 0; first != m_vertex_cnt; first++) _dfs_clique(first, 1, std::move(m_adj[first]), clique, call);
            }
            template <typename Callback>
            void enumerate_independant_set(Callback &&call) {
                for (size_type i = 0; i != m_vertex_cnt; i++) m_adj[i].set_false(m_vertex_cnt);
                std::vector<size_type> independant_set;
                for (size_type first = 0; first != m_vertex_cnt; first++) _dfs_independant_set(first, 1, std::move(m_adj[first]), independant_set, call);
            }
        };
    }
}

#endif