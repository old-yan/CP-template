#include "DS/CartesianTree.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个值越大越靠近根的笛卡尔树
    auto print_lchild = [](uint32_t i, int lchild) {
        if (lchild == -1)
            cout << i << " has no lchild" << endl;
        else
            cout << "lchild of " << i << " = " << lchild << endl;
    };
    auto print_rchild = [](uint32_t i, int rchild) {
        if (rchild == -1)
            cout << i << " has no rchild" << endl;
        else
            cout << "rchild of " << i << " = " << rchild << endl;
    };
    // 通过回调函数，我们可以观测到建的树
    auto root_max = OY::Cartesian::solve<int>(10, [&](uint32_t i) { return A[i]; }, print_lchild, print_rchild);
    cout << "root_max = " << root_max << endl;

    // 如果我们要建一棵值越小越靠近根的树呢？
    // 而且我们还想把左孩子、右孩子什么的都存下来
    std::vector<int> arr_lchild(10), arr_rchild(10), arr_parent(10);
    auto get_lchild = [&](uint32_t i, int lchild) {
        arr_lchild[i] = lchild;
        if (lchild != -1) arr_parent[lchild] = i;
    };
    auto get_rchild = [&](uint32_t i, int rchild) {
        arr_rchild[i] = rchild;
        if (rchild != -1) arr_parent[rchild] = i;
    };
    auto root_min = OY::Cartesian::solve(std::begin(A), std::end(A), get_lchild, get_rchild, std::greater<int>(), 0);
    cout << "arr_lchild:";
    for (int i = 0; i < 10; i++)
        cout << arr_lchild[i] << (i == 9 ? '\n' : ' ');
    cout << "arr_rchild:";
    for (int i = 0; i < 10; i++)
        cout << arr_rchild[i] << (i == 9 ? '\n' : ' ');
    cout << "root_min = " << root_min << endl;
}
/*
#输出如下
11 5 9 12 8 4 6 15 7 7
0 has no lchild
1 has no lchild
1 has no rchild
lchild of 2 = 1
2 has no rchild
rchild of 0 = 2
lchild of 3 = 0
4 has no lchild
5 has no lchild
5 has no rchild
lchild of 6 = 5
6 has no rchild
rchild of 4 = 6
rchild of 3 = 4
lchild of 7 = 3
8 has no lchild
9 has no lchild
9 has no rchild
rchild of 8 = 9
rchild of 7 = 8
root_max = 7
arr_lchild:-1 0 -1 -1 2 1 -1 -1 7 -1
arr_rchild:-1 4 3 -1 -1 6 8 -1 9 -1
root_min = 5
*/