#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"

void test_diameter() {
    // 众所周知，求直径长度需要两次 dfs，可以通过换根 dp 完成
    // 我们来换个问题：求经过每个点的最长简单路径
    OY::FlatTree::Tree<int, 1000> S(7);
    S.add_edge(0, 4, 10);
    S.add_edge(0, 5, 6);
    S.add_edge(1, 4, 8);
    S.add_edge(2, 3, 9);
    S.add_edge(2, 4, 8);
    S.add_edge(2, 6, 2);
    S.prepare(), S.set_root(0);

    // 维护每个点出发的最长的两条射线
    // m_val[0] 最长，m_val[1] 次长，且两条射线没有公共边
    struct Top2 {
        int m_val[2];
    };
    // 初始化时，每个结点都不包含任何射线
    auto mapping = [](uint32_t i) -> Top2 { return {}; };
    // 每个子结点可以给父结点提供一个射线
    auto merge = [](Top2 &dp_a, Top2 dp_son, uint32_t a, uint32_t son, int edge_value) {
        int offer = dp_son.m_val[0] + edge_value;
        if (offer > dp_a.m_val[0])
            dp_a.m_val[1] = dp_a.m_val[0], dp_a.m_val[0] = offer;
        else if (offer > dp_a.m_val[1])
            dp_a.m_val[1] = offer;
    };
    // 从父结点向子结点换根时，首先把子结点对父结点的贡献剥离掉
    // 然后再把剩余部分赋给子结点
    auto exclude = [](Top2 &dp_a, Top2 dp_fa, uint32_t a, uint32_t fa, int edge_value) {
        // 注意，如果 fa 是 -1，那就是根结点，不需要换根
        if (~fa) {
            int offer = dp_a.m_val[0] + edge_value;
            int rest = offer == dp_fa.m_val[0] ? dp_fa.m_val[1] : dp_fa.m_val[0];
            rest += edge_value;
            if (rest > dp_a.m_val[0])
                dp_a.m_val[1] = dp_a.m_val[0], dp_a.m_val[0] = rest;
            else if (rest > dp_a.m_val[1])
                dp_a.m_val[1] = rest;
        }
    };
    auto dp = OY::TreeTransfer::solve<Top2>(S, mapping, merge, exclude);
    for (int i = 0; i < S.vertex_cnt(); i++)
        cout << "longest simple path covering " << i << ": " << dp[i].m_val[0] + dp[i].m_val[1] << endl;
}

int main() {
    test_diameter();
}
/*
#输出如下
longest simple path covering 0: 33
longest simple path covering 1: 25
longest simple path covering 2: 33
longest simple path covering 3: 33
longest simple path covering 4: 33
longest simple path covering 5: 33
longest simple path covering 6: 26

*/