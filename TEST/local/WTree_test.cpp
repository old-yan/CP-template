#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
 * 求和功能 W 树
 */
void test_add() {
    cout << "test of add WTree:" << endl;
    // 当以长度进行构造的时候，树中元素均为默认初始值
    OY::WTree::Tree<int> T(10);
    cout << T << endl;
    // 单点修改
    for (int i = 0; i < 10; i++) T.add(i, i * 100);
    cout << T << endl;
    // 输出前缀和
    cout << "presum(5) = " << T.presum(5) << endl;
    cout << "presum(3) = " << T.presum(3) << endl;
    // 输出区间和
    cout << "sum(4 ~ 5) = " << T.query(4, 5) << endl;
    cout << endl;
}

/*
 * 求异或和功能 W 树
 */
void test_xor() {
    cout << "test of xor WTree:" << endl;
    // 当以长度进行构造的时候，树中元素均为默认初始值
    OY::WTree::Tree<int, OY::WTree::BitXor> T(10, [](int i) {
        return i * 100;
    });
    cout << T << endl;
    // 输出前缀和
    cout << "presum(3) = " << T.presum(3) << endl;
    cout << "presum(2) = " << T.presum(2) << endl;
    // 查询 3 处的值
    cout << "query(3) = " << T.query(3, 3) << endl;
    cout << endl;
}

int main() {
    test_add();
    test_xor();
}
/*
#输出如下
test of add WTree:
[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500]
presum(5) = 1500
presum(3) = 600
sum(4 ~ 5) = 900

test of xor WTree:
[0, 100, 172, 384, 16, 484, 956, 256, 544, 420]
presum(3) = 384
presum(2) = 172
query(3) = 300

*/