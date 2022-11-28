#ifndef __OY_VECTORTREE__
#define __OY_VECTORTREE__

#include "../GRAPH/Graph.h"

namespace OY {
    template <typename _Tp>
    struct _TreeEdgeWrap {
        struct _TreeEdge {
            uint32_t to;
            _Tp distance;
            _TreeEdge(uint32_t __to, _Tp __distance = _Tp()) : to(__to), distance(__distance) {}
            const _Tp &getDis() const { return distance; }
        };
    };
    template <>
    struct _TreeEdgeWrap<bool> {
        struct _TreeEdge {
            uint32_t to;
            _TreeEdge(uint32_t __to) : to(__to) {}
            constexpr bool getDis() const { return true; }
        };
    };
    template <typename _Tp = bool>
    struct VectorTree {
        using distance_type = std::conditional_t<std::is_same_v<_Tp, bool>, uint32_t, _Tp>;
        uint32_t m_vertexNum, m_root;
        std::vector<uint32_t> m_parent;
        std::vector<std::vector<typename _TreeEdgeWrap<_Tp>::_TreeEdge>> m_adj;
        VectorTree() = default;
        VectorTree(uint32_t __vertexNum) : m_vertexNum(__vertexNum), m_root(-1), m_parent(__vertexNum, -1), m_adj(__vertexNum) {}
        static VectorTree<bool> fromParentArray(const std::vector<int> &__parent) {
            VectorTree<bool> res(__parent.size());
            for (uint32_t i = 0; i < __parent.size(); i++)
                if (~__parent[i]) res.addEdge(i, __parent[i]);
            return res;
        }
        static VectorTree<bool> fromEdges(const std::vector<std::vector<int>> &__edges) {
            VectorTree<bool> res(__edges.size() + 1);
            for (uint32_t i = 0; i < __edges.size(); i++) res.addEdge(__edges[i][0], __edges[i][1]);
            return res;
        }
        void resize(uint32_t __vertexNum) {
            m_root = -1;
            m_parent.assign(m_vertexNum = __vertexNum, -1);
            m_adj.resize(m_vertexNum);
            for (auto &adj : m_adj) adj.clear();
        }
        void addEdge(uint32_t __a, uint32_t __b) { m_adj[__a].push_back(__b), m_adj[__b].push_back(__a); }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_adj[__a].push_back({__b, __distance}), m_adj[__b].push_back({__a, __distance}); }
        void setRoot(uint32_t __root) {
            auto dfs = [this](auto self, uint32_t i, uint32_t from) -> void {
                m_parent[i] = from;
                for (auto &adj : m_adj[i])
                    if (adj.to != from) self(self, adj.to, i);
            };
            dfs(dfs, m_root = __root, -1);
        }
        uint32_t getParent(uint32_t __i) const { return m_parent[__i]; }
        template <typename _Mapping, typename _Merge, typename _Afterwork = std::nullptr_t>
        auto getSubtreeValues(_Mapping __map, _Merge __merge, _Afterwork __work = _Afterwork()) {
            using _Fp = decltype(__map(0));
            std::vector<_Fp> res(m_vertexNum);
            auto dfs = [&](auto self, uint32_t i) -> void {
                res[i] = __map(i);
                for (auto &adj : m_adj[i])
                    if (uint32_t to = adj.to; to != m_parent[i]) {
                        self(self, to);
                        if constexpr (std::is_invocable_v<_Merge, _Fp &, _Fp &, uint32_t, uint32_t>)
                            __merge(res[i], res[to], i, to);
                        else if constexpr (std::is_invocable_v<_Merge, _Fp &, _Fp &, uint32_t>)
                            __merge(res[i], res[to], i);
                        else
                            __merge(res[i], res[to]);
                    }
                if constexpr (std::is_invocable_v<_Afterwork, _Fp &, uint32_t>) __work(res[i], i);
            };
            dfs(dfs, m_root);
            return res;
        }
        template <typename _Mapping>
        auto getDistanceSum(_Mapping __map) const {
            decltype(__map(0)) size[m_vertexNum];
            struct _Distance {
                distance_type downSum, upSum;
            };
            std::vector<_Distance> res(m_vertexNum);
            auto dfs1 = [&](auto self, uint32_t i) -> void {
                res[i].downSum = 0;
                size[i] = __map(i);
                for (auto &adj : m_adj[i])
                    if (uint32_t to = adj.to; to != m_parent[i]) {
                        self(self, to);
                        size[i] += size[to];
                        res[i].downSum += res[to].downSum + size[to] * adj.getDis();
                    }
            };
            dfs1(dfs1, m_root);
            auto dfs2 = [&](auto self, uint32_t i, distance_type upSum) -> void {
                res[i].upSum = upSum;
                for (auto &adj : m_adj[i])
                    if (uint32_t to = adj.to; to != m_parent[i]) self(self, to, res[i].upSum + res[i].downSum - res[to].downSum + (size[m_root] - size[to] * 2) * adj.getDis());
            };
            dfs2(dfs2, m_root, 0);
            return res;
        }
        std::pair<uint32_t, uint32_t> getCentroid() const {
            std::pair<uint32_t, uint32_t> res{-1, -1};
            auto dfs = [&](auto self, uint32_t i, uint32_t p) -> uint32_t {
                uint32_t size = 1, maxAdj = 0;
                for (auto &adj : m_adj[i])
                    if (uint32_t to = adj.to; to != p) {
                        uint32_t subSize = self(self, to, i);
                        size += subSize;
                        chmax(maxAdj, subSize);
                    }
                chmax(maxAdj, m_vertexNum - size);
                if (maxAdj <= m_vertexNum / 2)
                    if (~res.first)
                        res.second = i;
                    else
                        res.first = i;
                return size;
            };
            dfs(dfs, 0, -1);
            return res;
        }
    };
    VectorTree()->VectorTree<bool>;
    VectorTree(uint32_t)->VectorTree<bool>;
    template <typename _Ostream, typename _Tp>
    _Ostream &operator<<(_Ostream &__out, const VectorTree<_Tp> &__tree) { // http://mshang.ca/syntree/
        auto dfs = [&](auto self, uint32_t i) -> void {
            __out << '[' << i;
            for (auto &adj : __tree.m_adj[i])
                if (uint32_t to = adj.to; to != __tree.m_parent[i]) self(self, to);
            __out << ']';
        };
        dfs(dfs, __tree.m_root);
        return __out;
    }
}

#endif