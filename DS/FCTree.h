/*
最后修改:
20240612
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_FCTREE__
#define __OY_FCTREE__

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    template <typename Tp>
    struct FCTree {
        using size_type = uint32_t;
        struct block {
            Tp m_common_lazy;
            std::vector<Tp> m_qdata, m_data;
            std::vector<size_type> m_qcnt, m_qid;
            template <typename Mapping>
            void prepare(size_type left, size_type right, Mapping &&mapping) {
                m_qid.resize(right - left);
                m_common_lazy = {};
                m_qdata.assign(m_qcnt.size(), {});
                m_data.clear(), m_data.reserve(right - left);
                for (size_type i = left, j = 0; i != right; i++, j++) {
                    Tp val = mapping(i);
                    m_data.push_back(val), m_qdata[m_qid[j] + 1] += val;
                }
                std::partial_sum(m_qdata.begin(), m_qdata.end(), m_qdata.begin());
                std::partial_sum(m_qcnt.begin(), m_qcnt.end(), m_qcnt.begin());
            }
            void add(size_type left, size_type right, Tp inc) {
                std::adjacent_difference(m_qdata.begin(), m_qdata.end(), m_qdata.begin());
                for (size_type i = 0; i != left; i++) m_qdata[m_qid[i] + 1] += m_common_lazy, m_data[i] += m_common_lazy;
                for (size_type i = left; i != right; i++) m_qdata[m_qid[i] + 1] += inc + m_common_lazy, m_data[i] += inc + m_common_lazy;
                for (size_type i = right; i != m_qid.size(); i++) m_qdata[m_qid[i] + 1] += m_common_lazy, m_data[i] += m_common_lazy;
                m_common_lazy = {};
                std::partial_sum(m_qdata.begin(), m_qdata.end(), m_qdata.begin());
            }
            void add(Tp inc) { m_common_lazy += inc; }
            Tp query(size_type pos) const { return m_common_lazy + m_data[pos]; }
            Tp query(size_type left, size_type right) const { return m_common_lazy * (m_qcnt[right + 1] - m_qcnt[left]) + m_qdata[right + 1] - m_qdata[left]; }
        };
        size_type m_block_size, m_block_dep, m_query_block_size, m_query_dep;
        std::vector<block> m_blocks;
        std::vector<std::pair<size_type, size_type>> m_belong;
        FCTree() = default;
        template <typename InitMapping, typename PermMapping>
        FCTree(size_type length, InitMapping mapping, PermMapping perm_mapping) { resize(length, mapping, perm_mapping); }
        template <typename InitMapping, typename PermMapping>
        void resize(size_type length, InitMapping mapping, PermMapping perm_mapping) {
            m_block_size = std::bit_ceil<size_type>(std::sqrt(length));
            m_block_dep = std::countr_zero(m_block_size);
            m_query_block_size = std::bit_ceil<size_type>(std::sqrt(length));
            m_query_dep = std::countr_zero(m_query_block_size);
            size_type block_cnt = (length + m_block_size - 1) / m_block_size, query_block_cnt = (length + m_query_block_size - 1) / m_query_block_size;
            m_blocks.resize(block_cnt);
            for (auto &block : m_blocks) block.m_qid.resize(m_block_size), block.m_qcnt.assign(query_block_cnt + 1, {});
            m_belong.resize(length);
            for (size_type i = 0, j = 0, qid = 0; i != length; i++) {
                size_type index = perm_mapping(i), id = index / m_block_size, pos = index - id * m_block_size;
                m_blocks[id].m_qid[pos] = qid, m_blocks[id].m_qcnt[qid + 1]++, m_belong[i] = {id, pos};
                if (++j == m_query_block_size) j = 0, qid++;
            }
            for (size_type id = 0; id != block_cnt; id++) m_blocks[id].prepare(id * m_block_size, std::min((id + 1) * m_block_size, length), mapping);
        }
        void add(size_type left, size_type right, Tp inc) {
            size_type l = (left + m_block_size - 1) >> m_block_dep, r = (((right + 1) >> m_block_dep));
            if (l >= r && left >> m_block_dep == right >> m_block_dep)
                m_blocks[left >> m_block_dep].add(left & (m_block_size - 1), (right & (m_block_size - 1)) + 1, inc);
            else {
                if (left & (m_block_size - 1)) m_blocks[l - 1].add(left & (m_block_size - 1), m_block_size, inc);
                for (size_type i = l; i != r; i++) m_blocks[i].add(inc);
                if ((right + 1) & (m_block_size - 1)) m_blocks[r].add(0, (right & (m_block_size - 1)) + 1, inc);
            }
        }
        Tp query(size_type pos) const {
            auto &&id_pos = m_belong[pos];
            return m_blocks[id_pos.first].query(id_pos.second);
        }
        Tp query(size_type left, size_type right) const {
            size_type l = (left + m_query_block_size - 1) >> m_query_dep, r = (right + 1) >> m_query_dep;
            if (l >= r) {
                Tp res{};
                for (size_type i = left; i <= right; i++) res += query(i);
                return res;
            } else {
                Tp res{};
                size_type l0 = l * m_query_block_size, r0 = r * m_query_block_size;
                for (size_type i = left; i != l0; i++) res += query(i);
                for (auto &block : m_blocks) res += block.query(l, r - 1);
                for (size_type i = r0; i != right + 1; i++) res += query(i);
                return res;
            }
        }
    };
}

#endif