#include "GRAPH/DynamicFloyd.h"
#include "GRAPH/DynamicSPFA.h"
#include "IO/LeetcodeIO.h"
using namespace std;

/*
[2642. 设计可以求最短路径的图类](https://leetcode.cn/problems/design-graph-with-shortest-path-calculator/)
*/
/**
 * 本题需要一个支持动态加边更新的 floyd
 */

using monoid = OY::DFLOYD::AddGroup<uint32_t>;
// using monoid = OY::DSPFA::AddGroup<uint32_t>;
class Graph {
    OY::StaticDynamicFloyd<monoid, false, 100> m_sol;
    // OY::VectorDynamicFloyd<monoid, false> m_sol;
    // OY::StaticDynamicSPFAs<monoid, false, 100, 99100> m_sol;
    // OY::VectorDynamicSPFAs<monoid, false> m_sol;

public:
    Graph(int n, vector<vector<int>> &edges) {
        m_sol.resize(n);
        for (auto &e : edges) m_sol.add_edge(e[0], e[1], e[2]);
        m_sol.run();
    }
    void addEdge(vector<int> edge) { m_sol.update_by_edge(edge[0], edge[1], edge[2]); }
    int shortestPath(int node1, int node2) {
        auto res = m_sol.query(node1, node2);
        return res == m_sol.infinite() ? -1 : res;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_CLASS(Graph, int, vector<vector<int>> &);
    REGISTER_MEMBERFUNCTION_CLASS(Graph, addEdge, shortestPath);
    while (true) {
        executor.constructClass();
        while (executor) {
            executor.executeClass();
        }
    }
}
#endif