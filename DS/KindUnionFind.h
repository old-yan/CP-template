#ifndef __OY_KINDUNIONFIND__
#define __OY_KINDUNIONFIND__

#include "UnionFind.h"

namespace OY {
    struct KindUnionFind {
        uint32_t m_length, m_kind;
        UnionFind m_data;
        KindUnionFind(uint32_t __n = 0, uint32_t __kind = 0) : m_length(__n), m_kind(__kind), m_data(__n * __kind) {}
        void resize(uint32_t __n, uint32_t __kind) {
            m_length = __n;
            m_kind = __kind;
            m_data.resize(__n * __kind);
        }
        bool uniteBySize(uint32_t __a, uint32_t __aKind, uint32_t __b, uint32_t __bKind) { return m_data.uniteBySize(__a * m_kind + __aKind, __b * m_kind + __bKind); }
        bool uniteByID(uint32_t __a, uint32_t __aKind, uint32_t __b, uint32_t __bKind) { return m_data.uniteByID(__a * m_kind + __aKind, __b * m_kind + __bKind); }
        bool isSame(uint32_t __a, uint32_t __aKind, uint32_t __b, uint32_t __bKind) { return m_data.isSame(__a * m_kind + __aKind, __b * m_kind + __bKind); }
        std::vector<std::vector<uint32_t>> groups() {
            std::vector<std::vector<uint32_t>> ret;
            ret.resize(m_data.m_groupCnt / m_kind);
            uint32_t index[m_length];
            for (uint32_t i = 0, j = 0; i < m_length; i++)
                if (m_data.isHead(i * m_kind)) {
                    ret[j].reserve(m_data.m_size[i]);
                    index[i] = j++;
                }
            for (uint32_t i = 0; i < m_length; i++)
                for (uint32_t j = 0; j < m_kind; j++)
                    if (uint32_t head = m_data.find(i * m_kind + j); head % m_kind == 0) {
                        ret[index[head / m_kind]].push_back(i);
                        break;
                    }
            return ret;
        }
    };
}

#endif