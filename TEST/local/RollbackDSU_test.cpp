#include "DS/RollbackDSU.h"
#include "IO/FastIO.h"

int main() {
    /**
     * 本模板的其余功能与普通并查集类似，所以只展示本模板特有的功能
     */
    // 建立大小为 10 的并查集
    OY::RollbackDSU::Table u(10);

    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    u.unite_by_size(2, 5);
    u.unite_by_size(3, 6);
    u.unite_by_size(6, 5);
    // 2 和 3 的连接失败，所以不做保存
    u.unite_by_size(2, 3);
    u.unite_by_size(6, 8);
    // 查询 6 所在的分组大小
    cout << "6's group size is:      " << u.size(6) << endl;
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    // 撤销 6 和 8 的连接
    u.cancle();
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    // 撤销 6 和 5 的连接
    u.cancle();
    // 查询 6 所在的分组大小
    cout << "6's group size is:      " << u.size(6) << endl;
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;
}
/*
#输出如下
5 and 6 in same group?  false
6's group size is:      5
5 and 6 in same group?  true
5 and 6 in same group?  true
6's group size is:      2
5 and 6 in same group?  false

*/