#include"IO/LeetcodeIO.h"
using namespace std;

#define all(a) std::begin(a), std::end(a)
template <typename _Tp, typename _Fp, typename _Compare = std::less<void>>
bool chmax(_Tp &a, const _Fp &b, _Compare __comp = _Compare()) { return __comp(a, b) ? a = b, true : false; }
template <typename _Tp, typename _Fp, typename _Compare = std::less<void>>
bool chmin(_Tp &a, const _Fp &b, _Compare __comp = _Compare()) { return __comp(b, a) ? a = b, true : false; }
template <typename _Tp>
constexpr std::array<std::array<_Tp, 2>, 4> getNeighbors(_Tp i, _Tp j) { return {{{i + 1, j}, {i, j + 1}, {i - 1, j}, {i, j - 1}}}; }

namespace OY {
    class UnionFind {
        int m_groupCnt;
        std::vector<int> m_parent, m_size;

    public:
        UnionFind(int n = 1 << 20) {
            resize(n);
        }
        void resize(int n) {
            m_groupCnt = n;
            m_parent.resize(n);
            std::iota(m_parent.begin(), m_parent.end(), 0);
            m_size.resize(n);
            std::fill(m_size.begin(), m_size.end(), 1);
        }
        int find(int i) {
            return m_parent[i] == i ? i : m_parent[i] = find(m_parent[i]);
        }
        int size(int i) {
            return m_size[find(i)];
        }
        void uniteTo(int headA, int headB) {
            if (headA == headB) return;
            m_parent[headA] = headB;
            m_size[headB] += m_size[headA];
            m_groupCnt--;
        }
        bool uniteBySize(int a, int b) {
            if (a = find(a), b = find(b); a == b) return false;
            if (m_size[a] > m_size[b]) std::swap(a, b);
            uniteTo(a, b);
            return true;
        }
        bool uniteByID(int a, int b) {
            if (a = find(a), b = find(b); a == b) return false;
            if (a < b) std::swap(a, b);
            uniteTo(a, b);
            return true;
        }
        bool isSame(int a, int b) {
            return find(a) == find(b);
        }
        bool isHead(int i) {
            return i == m_parent[i];
        }
        int count() const { return m_groupCnt; }
        std::vector<int> heads() {
            std::vector<int> ret;
            ret.reserve(m_groupCnt);
            for (int i = 0; i < m_parent.size(); i++)
                if (isHead(i)) ret.push_back(i);
            return ret;
        }
        std::vector<std::vector<int>> groups() {
            std::vector<std::vector<int>> ret;
            ret.resize(m_groupCnt);
            std::vector<int> index(m_parent.size());
            for (int i = 0, j = 0; i < m_parent.size(); i++)
                if (isHead(i)) {
                    ret[j].reserve(m_size[i]);
                    index[i] = j++;
                }
            for (int i = 0; i < m_parent.size(); i++)
                ret[index[find(i)]].push_back(i);
            return ret;
        }
    };
}
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
}
namespace OY {
    template <typename _Net, typename _Tp = typename _Net::value_type>
    struct HLPP {
        _Net &m_net;
        uint32_t m_source, m_target;
        HLPP(_Net &__net, uint32_t __source, uint32_t __target) : m_net(__net), m_source(__source), m_target(__target) {}
        _Tp calc(_Tp __infinite = std::numeric_limits<_Tp>::max()) {
            uint32_t queue[m_net.m_vertexNum], height[m_net.m_vertexNum], ex_next[m_net.m_vertexNum * 2], gap_prev[m_net.m_vertexNum * 2], gap_next[m_net.m_vertexNum * 2], ex_highest = 0, gap_highest = 0, discharge_count;
            _Tp ex[m_net.m_vertexNum];
            decltype(m_net.getEdgesInfoOf(0).begin()) it[m_net.m_vertexNum], end[m_net.m_vertexNum];
            auto ex_insert = [&](uint32_t i, uint32_t h) {
                ex_next[i] = ex_next[m_net.m_vertexNum + h];
                ex_next[m_net.m_vertexNum + h] = i;
                chmax(ex_highest, h);
            };
            auto gap_insert = [&](uint32_t i, uint32_t h) {
                gap_prev[i] = m_net.m_vertexNum + h;
                gap_next[i] = gap_next[m_net.m_vertexNum + h];
                gap_prev[gap_next[i]] = gap_next[gap_prev[i]] = i;
                chmax(gap_highest, h);
            };
            auto gap_erase = [&](uint32_t i) {
                gap_next[gap_prev[i]] = gap_next[i];
                gap_prev[gap_next[i]] = gap_prev[i];
            };
            auto ex_add = [&](uint32_t i, _Tp flow) {
                ex[i] += flow;
                if (ex[i] == flow) ex_insert(i, height[i]);
            };
            auto ex_remove = [&](uint32_t i, _Tp flow) { ex[i] -= flow; };
            auto update_height = [&](uint32_t i, uint32_t h) {
                if (~height[i]) gap_erase(i);
                height[i] = h;
                if (~h) {
                    gap_insert(i, h);
                    if (ex[i] > 0) ex_insert(i, h);
                }
            };
            auto global_relabel = [&] {
                discharge_count = 0;
                std::iota(ex_next + m_net.m_vertexNum, ex_next + m_net.m_vertexNum * 2, m_net.m_vertexNum);
                std::iota(gap_prev + m_net.m_vertexNum, gap_prev + m_net.m_vertexNum * 2, m_net.m_vertexNum);
                std::iota(gap_next + m_net.m_vertexNum, gap_next + m_net.m_vertexNum * 2, m_net.m_vertexNum);
                std::fill(height, height + m_net.m_vertexNum, -1);
                height[m_target] = 0;
                uint32_t head = 0, tail = 0;
                queue[tail++] = m_target;
                while (head < tail)
                    for (auto [index, from, to, value] : m_net.getEdgesInfoOf(queue[head++]))
                        if (m_net.getEdge(m_net.getReversed(index)).value && height[to] > height[from] + 1) {
                            update_height(to, height[from] + 1);
                            queue[tail++] = to;
                        }
            };
            auto push = [&](uint32_t index, uint32_t from, uint32_t to, _Tp flow) {
                ex_remove(from, flow);
                ex_add(to, flow);
                m_net.getEdge(index).value -= flow;
                m_net.getEdge(m_net.getReversed(index)).value += flow;
            };
            auto discharge = [&](uint32_t i) {
                uint32_t h = m_net.m_vertexNum;
                auto cur = it[i];
                for (auto &e = it[i]; e != end[i]; ++e)
                    if (auto [index, from, to, value] = *e; value) {
                        if (height[from] == height[to] + 1) {
                            push(index, from, to, std::min(ex[i], value));
                            if (!ex[i]) return;
                        } else
                            chmin(h, height[to]);
                    }
                it[i] = m_net.getEdgesInfoOf(i).begin();
                for (auto &e = it[i]; e != cur; ++e)
                    if (auto [index, from, to, value] = *e; value) {
                        if (height[from] == height[to] + 1) {
                            push(index, from, to, std::min(ex[i], value));
                            if (!ex[i]) return;
                        } else
                            chmin(h, height[to]);
                    }
                discharge_count++;
                if (gap_next[gap_next[m_net.m_vertexNum + height[i]]] < m_net.m_vertexNum)
                    update_height(i, h == m_net.m_vertexNum ? -1 : h + 1);
                else {
                    uint32_t oldh = height[i];
                    for (h = oldh; h <= gap_highest; h++)
                        while (gap_next[m_net.m_vertexNum + h] < m_net.m_vertexNum) {
                            uint32_t j = gap_next[m_net.m_vertexNum + h];
                            height[j] = -1;
                            gap_erase(j);
                        }
                    gap_highest = oldh - 1;
                }
            };
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) it[i] = m_net.getEdgesInfoOf(i).begin();
            for (uint32_t i = 0; i < m_net.m_vertexNum; i++) end[i] = m_net.getEdgesInfoOf(i).end();
            std::fill(ex, ex + m_net.m_vertexNum, 0);
            global_relabel();
            ex_add(m_source, __infinite);
            ex_remove(m_target, __infinite);
            while (~ex_highest) {
                while (true) {
                    uint32_t i = ex_next[m_net.m_vertexNum + ex_highest];
                    if (i >= m_net.m_vertexNum) break;
                    ex_next[m_net.m_vertexNum + ex_highest] = ex_next[i];
                    if (height[i] != ex_highest) continue;
                    discharge(i);
                    if (discharge_count >= m_net.m_vertexNum) global_relabel();
                }
                ex_highest--;
            }
            return ex[m_target] + __infinite;
        }
    };
}
class Solution {
    int source,target=-1,P=-1;
public:
    int guardCastle(vector<string>& grid) {
        OY::UnionFind u;
        OY::FlowNetwork<int>fn(m*n,m*n*4);
        int m=grid.size(),n=grid[0].size();
        u.resize(m*n);
        for(int i=0;i<m;i++)for(int j=0;j<n;j++){
            if(grid[i][j]=='C')source=i*n+j;
            else if(grid[i][j]=='S'){
                if(target<0)target=i*n+j;
                else u.uniteBySize(target,i*n+j);
            }
            else if(grid[i][j]=='P'){
                if(P<0)P=i*n+j;
                else u.uniteByID(P,i*n+j);
            }
        }
        for(int i=0;i<m;i++)for(int j=0;j<n;j++)if(grid[i][j]!='.'&&grid[i][j]!='#')for(auto [ii,jj]:getNeighbors(i,j)){
            if(ii>=0 and ii<m and jj>=0 and jj<n and grid[ii][jj]!='.'&&grid[ii][jj]!='#')u.uniteBySize(i*n+j,ii*n+jj);
        }
        if(u.isSame(source,target))return -1;
        for(int i=0;i<m;i++)for(int j=0;j<n;j++)if(grid[i][j]!='#')for(auto [ii,jj]:getNeighbors(i,j)){
            if(ii>=0 and ii<m and jj>=0 and jj<n and grid[ii][jj]!='#'){
                fn.addEdge(u.find(i*n+j),u.find(ii*n+jj),1);
            }
        }
        OY::HLPP sol(fn,u.find(source),u.find(target));
        return sol.calc();
    }
};

int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION();
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}