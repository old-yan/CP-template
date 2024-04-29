#include "IO/FastIO.h"
#include "TREE/CustomTree.h"
#include "TREE/HeavyLightDecomposition.h"

struct AdjCallback {
    template <typename Callback>
    void operator()(int a, Callback &&call) const {
        if (a) call((a - 1) / 2, 1);
        if (a * 2 + 1 < 8) call(a * 2 + 1, 1);
        if (a * 2 + 2 < 8) call(a * 2 + 2, 1);
    }
};
void test_complete_binary_tree() {
    // 一个完全二叉树
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
    cout << endl;
}

void test_adapt_to_vector() {
    // 假如你特别喜欢用 std::vector<std::vector<int>> 存图，你又想用我的 hld 模板求 lca
    // 而且你还喜欢下标从 1 开始
    std::vector<std::vector<int>> V;
    int n = 7;
    V.resize(n + 1);
    auto add_edge = [&](int a, int b) {
        V[a].push_back(b);
        V[b].push_back(a);
    };
    add_edge(4, 2);
    add_edge(4, 7);
    add_edge(7, 1);
    add_edge(7, 3);
    add_edge(7, 5);
    add_edge(5, 6);

    // 好了，现在你自己的树存完了。现在，你需要写个函数找到每个点的邻居
    // 注意这个函数是要传给我的模板的，所以下标要从 0 开始，调整一下
#if CPP_STANDARD >= 201402L
    auto call_adj = [&](int a, auto call) {
        for (auto b : V[a + 1]) call(b - 1, 1);
    };
#else
    auto call_adj = [&](int a, std::function<void(int, bool)> call) {
        for (auto b : V[a + 1]) call(b - 1, 1);
    };
#endif
    OY::CustomTree::Tree<bool, decltype(call_adj)> T(n, call_adj);
    // 注意，我这里根是 3，说明在你那儿，根是 4
    T.set_root(3);
    // 可以进行输出
    cout << T << endl;
    // 对，就这么简单，也没有额外浪费空间，你的图数据的信息已经被存好了

    // 现在尝试套一下我的 lca 模板
    OY::HLD::Table<decltype(T)> hld(&T);
    // 求一下 lca(1,5) （对于你来说是 lca(2,6)）
    // 结果是 3（对于你来说是 4)
    cout << "lca(1, 5) = " << hld.calc(1, 5) << endl;
}

int main() {
    test_complete_binary_tree();
    test_adapt_to_vector();
}
/*
#输出如下
T:[0[1[3[7]][4]][2[5][6]]]
neighbors of 3:
1 is connected with 3
7 is connected with 3
dep of tree vertexes:
0 1 1 2 2 2
2 3 size of tree vertexes:
8 4 3 2 1 1
1 1 
[3[1][6[0][2][4[5]]]]
lca(1, 5) = 3

*/