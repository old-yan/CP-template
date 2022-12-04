#ifndef __OY_SPLITTER__
#define __OY_SPLITTER__

#include "../DS/UnionFind.h"

namespace OY {
    template <typename _Tp>
    struct _SplitterEdgeWrap {
        struct _SplitterEdge {
            uint32_t from, to;
            _Tp distance;
            _SplitterEdge(uint32_t __from, uint32_t __to, _Tp __distance = _Tp()) : from(__from), to(__to), distance(__distance) {}
        };
    };
    template <>
    struct _SplitterEdgeWrap<bool> {
        struct _SplitterEdge {
            uint32_t from, to;
            _SplitterEdge(uint32_t __from, uint32_t __to) : from(__from), to(__to) {}
        };
    };
    template <typename _Tp>
    struct Splitter {
        uint32_t m_vertexNum;
        UnionFind m_union;
        std::vector<typename _SplitterEdgeWrap<_Tp>::_SplitterEdge> m_edges;
        std::vector<std::vector<uint32_t>> m_groups;
        std::vector<uint32_t> m_belong, m_idInGroup;
        std::vector<std::vector<typename _SplitterEdgeWrap<_Tp>::_SplitterEdge>> m_edgesOfGroup;
        Splitter(uint32_t __vertexNum, uint32_t __edgeNum) : m_vertexNum(__vertexNum), m_union(__vertexNum) { m_edges.reserve(__edgeNum); }
        void addEdge(uint32_t __from, uint32_t __to) {
            m_edges.push_back({__from, __to});
            m_union.uniteBySize(__from, __to);
        }
        void addEdge(uint32_t __from, uint32_t __to, _Tp __info) {
            m_edges.push_back({__from, __to, __info});
            m_union.uniteBySize(__from, __to);
        }
        void prepare() {
            m_groups = m_union.groups();
            m_belong.resize(m_vertexNum);
            for (uint32_t i = 0; i < m_groups.size(); i++)
                for (uint32_t a : m_groups[i]) m_belong[a] = i;
            uint32_t size[m_groups.size()];
            std::fill_n(size, m_groups.size(), 0);
            m_idInGroup.resize(m_vertexNum);
            for (uint32_t i = 0; i < m_vertexNum; i++) m_idInGroup[i] = size[m_belong[i]]++;
            std::fill_n(size, m_groups.size(), 0);
            for (auto &e : m_edges) size[m_belong[e.from]]++;
            m_edgesOfGroup.resize(m_groups.size());
            for (uint32_t i = 0; i < m_groups.size(); i++) m_edgesOfGroup[i].reserve(size[i]);
            for (auto e : m_edges) {
                uint32_t i = m_belong[e.from];
                e.from = m_idInGroup[e.from], e.to = m_idInGroup[e.to];
                m_edgesOfGroup[i].emplace_back(std::move(e));
            }
        }
    };
}

#endif