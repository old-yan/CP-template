#include "IO/FastIO.h"
#include "TREE/LinkTree.h"
#include "TREE/SubtreeKindCounter.h"

void test() {
    OY::LinkTree::Tree<bool, 1000> S(5);
    S.add_edge(0, 1);
    S.add_edge(0, 2);
    S.add_edge(1, 3);
    S.add_edge(1, 4);
    S.prepare(), S.set_root(0);

    std::string elem[5] = {"apple", "banana", "banana", "orange", "orange"};

    auto res = OY::MapTreeKindCounter<1000>::solve(&S, [&](int i) {
        return elem[i];
    });

    for (int i = 0; i < 5; i++)
        cout << "kind count of subtree[" << i << "] = " << res[i] << endl;
}

int main() {
    test();
}
/*
#输出如下
kind count of subtree[0] = 3
kind count of subtree[1] = 2
kind count of subtree[2] = 1
kind count of subtree[3] = 1
kind count of subtree[4] = 1

*/