#ifndef __OY_FLOWNETWORK__
#define __OY_FLOWNETWORK__

#include "Graph.h"

namespace OY {
    template <typename _Tp>
    struct FlowNetwork {
#pragma pack(4)
        union Edge {
            struct {
                union {
                    struct {
                        uint32_t index;
                        union {
                            struct {
                                uint32_t from;
                                struct {
                                    uint32_t to;
                                    _Tp value;
                                } adj;
                            } data_a;
                            struct {
                                uint32_t from, to;
                                _Tp value;
                            } data_b;
                        } data;
                    } info_a;
                    struct {
                        uint32_t index, from, to;
                        _Tp value;
                    } info_b;
                } info;
                uint32_t reverse;
            } edge_a;
            struct {
                uint32_t index, from, to;
                _Tp value;
                uint32_t reverse;
            } edge_b;
        };
#pragma pack()
        struct EdgeGetter {
            auto &operator()(Edge &e) const { return e.edge_b; }
        };
        struct EdgeInfoGetter {
            auto &operator()(Edge &e) const { return e.edge_a.info.info_b; }
        };
        struct EdgeDataGetter {
            auto &operator()(Edge &e) const { return e.edge_a.info.info_a.data.data_b; }
        };
        struct EdgeAdjGetter {
            auto &operator()(Edge &e) const { return e.edge_a.info.info_a.data.data_a.adj; }
        };
        template <typename _Getter>
        struct EdgesOf {
            struct EdgesOfIterator : std::vector<Edge>::iterator {
                auto &operator*() { return _Getter()(std::vector<Edge>::iterator::operator*()); }
            };
            typename std::vector<Edge>::iterator first, last;
            EdgesOfIterator begin() const { return {first}; }
            EdgesOfIterator end() const { return {last}; }
        };
        using value_type = _Tp;
        uint32_t m_vertexNum;
        uint32_t m_edgeNum;
        std::vector<uint32_t> m_starts;
        std::vector<decltype(Edge().edge_a.info.info_a.data.data_b)> m_edgeBuffer;
        std::vector<Edge> m_edges;
        FlowNetwork(uint32_t __n, uint32_t __m) : m_vertexNum(__n), m_edgeNum(__m * 2) {
            m_starts.resize(m_vertexNum + 1, 0);
            m_edgeBuffer.reserve(__m);
        }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __value) {
            if (__a == __b) return;
            m_starts[__a + 1]++;
            m_starts[__b + 1]++;
            m_edgeBuffer.push_back({__a, __b, __value});
        }
        void prepare() {
            std::partial_sum(m_starts.begin(), m_starts.end(), m_starts.begin());
            m_edges.resize(m_edgeNum = m_edgeBuffer.size() * 2);
            uint32_t cursor[m_vertexNum];
            std::copy(m_starts.begin(), m_starts.begin() + m_vertexNum, cursor);
            for (auto [from, to, value] : m_edgeBuffer) {
                m_edges[cursor[from]] = {cursor[from], from, to, value, cursor[to]};
                m_edges[cursor[to]] = {cursor[to], to, from, 0, cursor[from]};
                cursor[from]++;
                cursor[to]++;
            }
        }
        template <typename _Compare>
        void prepareSorted(_Compare __comp = _Compare()) {
            std::sort(m_edgeBuffer.begin(), m_edgeBuffer.end(), __comp);
            prepare();
        }
        auto &getEdge(uint32_t __index) { return EdgeGetter()(m_edges[__index]); }
        auto &getEdgeInfo(uint32_t __index) { return EdgeInfoGetter()(m_edges[__index]); }
        auto &getEdgeData(uint32_t __index) { return EdgeDataGetter()(m_edges[__index]); }
        auto &getEdgeAdj(uint32_t __index) { return EdgeAdjGetter()(m_edges[__index]); }
        auto getEdgesOf(uint32_t __from) { return EdgesOf<EdgeGetter>{m_edges.begin() + m_starts[__from], m_edges.begin() + m_starts[__from + 1]}; }
        auto getEdgesInfoOf(uint32_t __from) { return EdgesOf<EdgeInfoGetter>{m_edges.begin() + m_starts[__from], m_edges.begin() + m_starts[__from + 1]}; }
        auto getEdgesDataOf(uint32_t __from) { return EdgesOf<EdgeDataGetter>{m_edges.begin() + m_starts[__from], m_edges.begin() + m_starts[__from + 1]}; }
        auto getEdgesAdjOf(uint32_t __from) { return EdgesOf<EdgeAdjGetter>{m_edges.begin() + m_starts[__from], m_edges.begin() + m_starts[__from + 1]}; }
        auto getEdges() { return EdgesOf<EdgeGetter>{m_edges.begin(), m_edges.end()}; }
        auto getEdgesInfo() { return EdgesOf<EdgeInfoGetter>{m_edges.begin(), m_edges.end()}; }
        auto getEdgesData() { return EdgesOf<EdgeDataGetter>{m_edges.begin(), m_edges.end()}; }
        auto getEdgesAdj() { return EdgesOf<EdgeAdjGetter>{m_edges.begin(), m_edges.end()}; }
        uint32_t getReversed(uint32_t __i) const { return m_edges[__i].edge_b.reverse; }
        bool isReversed(uint32_t __a, uint32_t __b) const { return m_edges[__a].edge_b.reverse == __b; }
    };
    template <typename _Tp>
    struct LinkedFlowNetwork {
#pragma pack(4)
        union Edge {
            struct {
                union {
                    struct {
                        uint32_t index;
                        union {
                            struct {
                                uint32_t from;
                                struct {
                                    uint32_t to;
                                    _Tp value;
                                } adj;
                            } data_a;
                            struct {
                                uint32_t from, to;
                                _Tp value;
                            } data_b;
                        } data;
                    } info_a;
                    struct {
                        uint32_t index, from, to;
                        _Tp value;
                    } info_b;
                } info;
                uint32_t next;
            } edge_a;
            struct {
                uint32_t index, from, to;
                _Tp value;
                uint32_t next;
            } edge_b;
        };
#pragma pack()
        struct EdgeGetter {
            auto &operator()(Edge &e) const { return e.edge_b; }
        };
        struct EdgeInfoGetter {
            auto &operator()(Edge &e) const { return e.edge_a.info.info_b; }
        };
        struct EdgeDataGetter {
            auto &operator()(Edge &e) const { return e.edge_a.info.info_a.data.data_b; }
        };
        struct EdgeAdjGetter {
            auto &operator()(Edge &e) const { return e.edge_a.info.info_a.data.data_a.adj; }
        };
        template <typename _Getter, bool _IsOf>
        struct EdgesOf {
            Edge *edges;
            uint32_t first;
            struct EdgeOfIterator {
                Edge *edges;
                uint32_t index;
                EdgeOfIterator &operator++() {
                    if constexpr (_IsOf)
                        index = EdgeGetter()(edges[index]).next;
                    else
                        index++;
                    return *this;
                }
                auto &operator*() { return _Getter()(edges[index]); }
                bool operator!=(const EdgeOfIterator &other) const { return index != other.index; }
            };
            EdgeOfIterator begin() const {
                if constexpr (_IsOf)
                    return EdgeOfIterator{edges, first};
                else
                    return EdgeOfIterator{edges, 0};
            }
            EdgeOfIterator end() const {
                if constexpr (_IsOf)
                    return EdgeOfIterator{edges, uint32_t(-1)};
                else
                    return EdgeOfIterator{edges, first + 1};
            }
        };
        using value_type = _Tp;
        uint32_t m_vertexNum;
        uint32_t m_edgeNum;
        std::vector<uint32_t> m_starts;
        std::vector<uint32_t> m_edgeCount;
        std::vector<Edge> m_edges;
        LinkedFlowNetwork(uint32_t __n, uint32_t __m) : m_vertexNum(__n), m_edgeNum(__m * 2) {
            m_edges.reserve(m_edgeNum);
            m_starts.resize(m_vertexNum, -1);
            m_edgeCount.resize(m_vertexNum, 0);
        }
        void addEdge(uint32_t __a, uint32_t __b, _Tp __value) {
            m_edges.push_back({uint32_t(m_edges.size()), __a, __b, __value});
            m_edges.push_back({uint32_t(m_edges.size()), __b, __a, 0});
        }
        void prepare() {
            m_edgeNum = m_edges.size();
            for (uint32_t i = m_edgeNum - 1; ~i; i--) {
                auto &[index, from, to, value, next] = m_edges[i].edge_b;
                next = m_starts[from];
                m_starts[from] = index = i;
                m_edgeCount[from]++;
            }
        }
        auto &getEdge(uint32_t __index) { return EdgeGetter()(m_edges[__index]); }
        auto &getEdgeInfo(uint32_t __index) { return EdgeInfoGetter()(m_edges[__index]); }
        auto &getEdgeData(uint32_t __index) { return EdgeDataGetter()(m_edges[__index]); }
        auto &getEdgeAdj(uint32_t __index) { return EdgeAdjGetter()(m_edges[__index]); }
        auto getEdgesOf(uint32_t __from) { return EdgesOf<EdgeGetter, true>{m_edges.data(), m_starts[__from]}; }
        auto getEdgesInfoOf(uint32_t __from) { return EdgesOf<EdgeInfoGetter, true>{m_edges.data(), m_starts[__from]}; }
        auto getEdgesDataOf(uint32_t __from) { return EdgesOf<EdgeDataGetter, true>{m_edges.data(), m_starts[__from]}; }
        auto getEdgesAdjOf(uint32_t __from) { return EdgesOf<EdgeAdjGetter, true>{m_edges.data(), m_starts[__from]}; }
        auto getEdges() { return EdgesOf<EdgeGetter, false>{m_edges.data(), uint32_t(m_edgeNum - 1)}; }
        auto getEdgesInfo() { return EdgesOf<EdgeInfoGetter, false>{m_edges.data(), uint32_t(m_edgeNum - 1)}; }
        auto getEdgesData() { return EdgesOf<EdgeDataGetter, false>{m_edges.data(), uint32_t(m_edgeNum - 1)}; }
        auto getEdgesAdj() { return EdgesOf<EdgeAdjGetter, false>{m_edges.data(), uint32_t(m_edgeNum - 1)}; }
        uint32_t getReversed(uint32_t __i) const { return __i ^ 1; }
        bool isReversed(uint32_t __a, uint32_t __b) const { return (__a ^ __b) == 1; }
    };
}

#endif