#ifndef __OY_TREE__
#define __OY_TREE__

#include "../GRAPH/Graph.h"

namespace OY {
#pragma pack(4)
    template <typename _Tp>
    struct _TreeEdge {
        uint32_t from, to;
        _Tp distance;
    };
    template <>
    struct _TreeEdge<bool> {
        uint32_t from, to;
    };
    template <typename _Tp, uint32_t _N>
    struct _TreeDistance {
        _Tp data[_N];
        void set(uint32_t __i, _Tp __val) { data[__i] = __val; }
        _Tp &operator[](uint32_t __i) { return data[__i]; }
        const _Tp &operator[](uint32_t __i) const { return data[__i]; }
    };
    template <uint32_t _N>
    struct _TreeDistance<bool, _N> {
        void set(uint32_t, bool) {}
        constexpr bool operator[](uint32_t) const { return true; }
    };
#pragma pack()
    template <uint32_t _MAXN, typename _Tp = bool>
    struct Tree {
        _TreeEdge<_Tp> m_edges[_MAXN];
        _TreeDistance<_Tp, _MAXN * 2> m_distances;
        uint32_t m_to[_MAXN * 2], m_starts[_MAXN + 1], m_vertexNum, m_root, m_cursor;
        using distance_type = std::conditional_t<std::is_same_v<_Tp, bool>, uint32_t, _Tp>;
        Tree() = default;
        Tree(uint32_t __vertexNum) : m_vertexNum(__vertexNum), m_root(-1), m_cursor(0) {}
        static Tree<_MAXN, bool> fromParentArray(const std::vector<int> &__parent) {
            Tree<_MAXN, bool> res(__parent.size());
            for (uint32_t i = 0; i < __parent.size(); i++)
                if (~__parent[i]) res.addEdge(i, __parent[i]);
            res.prepare();
            return res;
        }
        static Tree<_MAXN, bool> fromEdges(const std::vector<std::vector<int>> &__edges) {
            Tree<_MAXN, bool> res(__edges.size() + 1);
            for (uint32_t i = 0; i < __edges.size(); i++) res.addEdge(__edges[i][0], __edges[i][1]);
            res.prepare();
            return res;
        }
        void resize(uint32_t __vertexNum) { m_vertexNum = __vertexNum, m_root = -1, m_cursor = 0; }
        void addEdge(uint32_t __a, uint32_t __b) { m_edges[m_cursor++] = {__a, __b}; }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __distance) { m_edges[m_cursor++] = {__a, __b, __distance}; }
        void prepare() {
            std::fill(m_starts, m_starts + m_vertexNum + 1, 0);
            for (uint32_t i = 0; i < m_cursor; i++) {
                m_starts[m_edges[i].from + 1]++;
                m_starts[m_edges[i].to + 1]++;
            }
            std::partial_sum(m_starts, m_starts + m_vertexNum + 1, m_starts);
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts, m_starts + m_vertexNum, cursor);
            for (uint32_t i = 0; i < m_cursor; i++)
                if constexpr (std::is_same_v<_Tp, bool>) {
                    auto &[from, to] = m_edges[i];
                    m_to[cursor[from]++] = to;
                    m_to[cursor[to]++] = from;
                } else {
                    auto &[from, to, distance] = m_edges[i];
                    m_to[cursor[from]] = to;
                    m_distances.set(cursor[from]++, distance);
                    m_to[cursor[to]] = from;
                    m_distances.set(cursor[to]++, distance);
                }
        }
        void setRoot(uint32_t __root) {
            auto dfs = [this](auto self, uint32_t i, uint32_t from) -> void {
                for (uint32_t start = m_starts[i], end = m_starts[i + 1], cur = start; cur != end; cur++)
                    if (uint32_t to = m_to[cur]; to != from)
                        self(self, to, i);
                    else {
                        std::swap(m_to[start], m_to[cur]);
                        if constexpr (!std::is_same_v<_Tp, bool>) std::swap(m_distances[start], m_distances[cur]);
                    }
            };
            dfs(dfs, m_root = __root, -1);
        }
        uint32_t getParent(uint32_t __i) const { return __i == m_root ? -1 : m_to[m_starts[__i]]; }
        std::vector<distance_type> getDistance(uint32_t __source) const {
            std::vector<distance_type> res(m_vertexNum);
            auto dfs = [&](auto self, uint32_t i, uint32_t from, distance_type curDistance) -> void {
                res[i] = curDistance;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur != end; cur++)
                    if (uint32_t to = m_to[cur]; to != from) self(self, to, i, curDistance + m_distances[cur]);
            };
            dfs(dfs, __source, -1, 0);
            return res;
        }
        template <typename _Mapping, typename _Merge, typename _Afterwork = std::nullptr_t>
        auto getSubtreeValues(_Mapping __map, _Merge __merge, _Afterwork __work = _Afterwork()) {
            using _Fp = decltype(__map(0));
            std::vector<_Fp> res(m_vertexNum);
            auto dfs = [&](auto self, uint32_t i) -> void {
                res[i] = __map(i);
                for (uint32_t cur = m_starts[i] + (i != m_root), end = m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_to[cur];
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
                for (uint32_t cur = m_starts[i] + (i != m_root), end = m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_to[cur];
                    self(self, to);
                    size[i] += size[to];
                    res[i].downSum += res[to].downSum + size[to] * m_distances[cur];
                }
            };
            dfs1(dfs1, m_root);
            auto dfs2 = [&](auto self, uint32_t i, distance_type upSum) -> void {
                res[i].upSum = upSum;
                for (uint32_t cur = m_starts[i] + (i != m_root), end = m_starts[i + 1]; cur != end; cur++) {
                    uint32_t to = m_to[cur];
                    self(self, to, res[i].upSum + res[i].downSum - res[to].downSum + (size[m_root] - size[to] * 2) * m_distances[cur]);
                }
            };
            dfs2(dfs2, m_root, 0);
            return res;
        }
        std::pair<uint32_t, uint32_t> getCentroid() const {
            std::pair<uint32_t, uint32_t> res{-1, -1};
            auto dfs = [&](auto self, uint32_t i, uint32_t p) -> uint32_t {
                uint32_t size = 1, maxAdj = 0;
                for (uint32_t cur = m_starts[i], end = m_starts[i + 1]; cur != end; cur++)
                    if (uint32_t to = m_to[cur]; to != p) {
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
    template <typename _Ostream, uint32_t _MAXN, typename _Tp>
    _Ostream &operator<<(_Ostream &__out, const Tree<_MAXN, _Tp> &__tree) { // http://mshang.ca/syntree/
        auto dfs = [&](auto self, uint32_t i) -> void {
            __out << '[' << i;
            for (uint32_t cur = __tree.m_starts[i] + (i != __tree.m_root), end = __tree.m_starts[i + 1]; cur != end; cur++)
                self(self, __tree.m_to[cur]);
            __out << ']';
        };
        dfs(dfs, __tree.m_root);
        return __out;
    }
}

#endif