#include "IO/FastIO.h"
#include "TREE/LinkTree.h"

void test_bool_tree() {
    // 一个无权树
    OY::LinkTree::Tree<bool, 100000> T(6);
    // 加边
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.add_edge(3, 4);
    T.add_edge(3, 5);
    // 预备
    T.prepare();
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
    std::vector<int> dep(6);
    T.tree_dp_vertex(0, [&](int a, int p) { dep[a] = ~p ? dep[p] + 1 : 0; }, {}, {});
    cout << "dep of tree vertexes:\n";
    for (int i = 0; i < 6; i++) cout << dep[i] << (i == 5 ? '\n' : ' ');

    // 树形 dp 求子树大小（需要信息从下往上传播）
    std::vector<int> size(6);
    T.tree_dp_vertex(
        0, [&](int a, int p) { size[a] = 1; }, [&](int a, int to) { size[a] += size[to]; }, {});
    cout << "size of tree vertexes:\n";
    for (int i = 0; i < 6; i++) cout << size[i] << (i == 5 ? '\n' : ' ');
}

void test_int_tree() {
    // 一个边权为 int 类型的树
    OY::LinkTree::Tree<int, 100000> T(6);
    // 加边
    T.add_edge(0, 1, 100);
    T.add_edge(0, 2, 200);
    T.add_edge(0, 3, 50);
    T.add_edge(3, 4, 110);
    T.add_edge(3, 5, 80);
    // 预备
    T.prepare();
    // 定根
    T.set_root(0);
    // 输出观察树形(粘贴到http://mshang.ca/syntree/)
    cout << "T:" << T << endl;

    // 邻接表的遍历
    cout << "neighbors of 3:\n";
    // 此时我们用 do_for_each_adj_edge 来遍历邻接表，不仅可以遍历邻接点，还能知道边权
    // 当然如果你不需要边权信息，还想用 do_for_each_adj_vertex 也是可以的
    T.do_for_each_adj_edge(3, [&](int a, int dis) {
        cout << a << " is connected with 3, distance = " << dis << endl;
    });

    // tree_dp_vertex 仍然可用
    // 这里展示使用 tree_dp_edge 来求到某点的距离
    std::vector<int> distance(6);
    T.tree_dp_edge(0, [&](int a, int p, int dis) { distance[a] = ~p ? distance[p] + dis : 0; }, {}, {});
    cout << "distance of tree vertexes:\n";
    for (int i = 0; i < 6; i++) cout << distance[i] << (i == 5 ? '\n' : ' ');
}

int main() {
    test_bool_tree();
    test_int_tree();
}
/*
#输出如下
T:[0[3[5][4]][2][1]]
neighbors of 3:
5 is connected with 3
4 is connected with 3
0 is connected with 3
dep of tree vertexes:
0 1 1 1 2 2
size of tree vertexes:
6 1 1 3 1 1
T:[0[3[5][4]][2][1]]
neighbors of 3:
5 is connected with 3, distance = 80
4 is connected with 3, distance = 110
0 is connected with 3, distance = 50
distance of tree vertexes:
0 100 200 50 160 130

*/