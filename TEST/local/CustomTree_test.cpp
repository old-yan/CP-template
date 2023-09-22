#include "IO/FastIO.h"
#include "TREE/CustomTree.h"

#include "TEST/mystd.h"

struct AdjCallback {
    template <typename Callback>
    void operator()(int a, Callback &&call) const {
        if (a) call((a - 1) / 2, 1);
        if (a * 2 + 1 < 8) call(a * 2 + 1, 1);
        if (a * 2 + 2 < 8) call(a * 2 + 2, 1);
    }
};
int main() {
    // 一个满二叉树
#if CPP_STANDARD >= 201402L
    // 此处 call 泛指当找到 a 的邻居之后，对邻居调用的回调函数
    // 注意，call 需要支持两个入参，第一个入参为邻居，第二个入参为边权
    auto adj_call = [](int a, auto call) -> void {
        if (a) call((a - 1) / 2, 1);
        if (a * 2 + 1 < 8) call(a * 2 + 1, 1);
        if (a * 2 + 2 < 8) call(a * 2 + 2, 1);
    };
#else
    // 语言版本低的话，只能把 AdjCallback 声明在外面，而不能用匿名函数
    auto adj_call = AdjCallback();
#endif
    OY::CustomTree::Tree<bool, decltype(adj_call)> T(8, adj_call);
    // 定根
    T.set_root(0);
    // 输出观察树形(粘贴到http://mshang.ca/syntree/)
    cout << "T:" << T << endl;

    // 邻接表的遍历
    cout << "neighbors of 3:\n";
    T.do_for_each_adj_vertex(3, [&](int a) {
        cout << a << " is connected with 3\n";
    });

    // 一般情况下我们可以利用 tree_dp_vertex 快捷地进行树形 dp
    // 树形 dp 求深度（需要信息从上往下传播）
    std::vector<int> dep(8);
    T.tree_dp_vertex(0, [&](int a, int p) { dep[a] = ~p ? dep[p] + 1 : 0; }, {}, {});
    cout << "dep of tree vertexes:\n";
    for (int i = 0; i < 8; i++) cout << dep[i] << (i == 5 ? '\n' : ' ');

    // 树形 dp 求子树大小（需要信息从下往上传播）
    std::vector<int> size(8);
    T.tree_dp_vertex(
        0, [&](int a, int p) { size[a] = 1; }, [&](int a, int to) { size[a] += size[to]; }, {});
    cout << "size of tree vertexes:\n";
    for (int i = 0; i < 8; i++) cout << size[i] << (i == 5 ? '\n' : ' ');
}
/*
# 输出如下
T:[0[1[3[7]][4]][2[5][6]]]
neighbors of 3:
1 is connected with 3
7 is connected with 3
dep of tree vertexes:
0 1 1 2 2 2
2 3 size of tree vertexes:
8 4 3 2 1 1
1 1 
*/