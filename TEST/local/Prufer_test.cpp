#include "IO/FastIO.h"
#include "TREE/Prufer.h"

int main() {
    OY::PRUFER::Tree tree(5);
    tree.add_edge(0, 3);
    tree.add_edge(3, 1);
    tree.add_edge(4, 3);
    tree.add_edge(4, 2);
    // 编码
    auto code = tree.encode();
    cout << "prufer code:";
    for (auto a : code) cout << ' ' << a;
    cout << endl;

    // 解码
    auto parent_arr = OY::PRUFER::Tree::decode(code.begin(), code.end());
    for (int i = 0; i < parent_arr.size(); i++)
        if (parent_arr[i] != -1)
            cout << "node " << i << "'s parent: node " << parent_arr[i] << endl;
}
/*
#输出如下
prufer code: 3 3 4
node 0's parent: node 3
node 1's parent: node 3
node 2's parent: node 4
node 3's parent: node 4

*/