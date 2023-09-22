#include "IO/FastIO.h"
#include "TREE/RandTree.h"
#include "TREE/VectorTree.h"

int main() {
    using Tree = OY::VectorTree::Tree<bool>;

    auto T = OY::RandTree::make_tree<Tree>(10);

    T.set_root(4);

    cout << T << endl;
}
/*
# gcc/clang输出如下
[4[9[1[3][6]][7][2[8[0][5]]]]]

# msvc输出如下
[4[2[0][3]][7][1[9[5[6][8]]]]]

*/