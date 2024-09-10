#include "DS/MonoStack.h"
#include "DS/RollbackCatTree.h"
#include "DS/RollbackMonoZkwTree.h"
#include "DS/RollbackSparseTable.h"
#include "DS/RollbackSqrtTree.h"
#include "IO/FastIO.h"
#include "MATH/Barrett32.h"
#include "MATH/Lemire32.h"

/*
[P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
*/
/**
 * 显然，本题可以用线段树解决
 * 考虑到区间查询的右边界始终在右移，所以也可以用单调栈+二分解决
 */

static constexpr uint32_t M = 200000;
template <typename Table>
void solve_rollbackds() {
    uint32_t m, D, lst{};
    cin >> m >> D;
    OY::Barrett32 L(D);
    // OY::Lemire32 L(D);

    Table S;
    S.reserve(m);
    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            int x;
            cin >> x;
            S.push_back((x + lst) % L);
        } else {
            uint32_t l;
            cin >> l;
            lst = S.query(S.size() - l, S.size() - 1);
            cout << lst << endl;
        }
    }
}

void solve_monostack() {
    uint32_t m, D;
    cin >> m >> D;

    uint32_t lst{};
    auto getter = [&, L = OY::Barrett32(D)](uint32_t) {
        // auto getter = [&, L = OY::Lemire32(D)](uint32_t) {
        int x;
        cin >> x;
        return (x + lst) % L;
    };
    OY::MonoMaxStack<uint32_t, decltype(getter)> S(m, {}, getter, {}, D, true);

    while (m--) {
        char op;
        cin >> op;
        if (op == 'A') {
            S.extend_right();
        } else {
            uint32_t len;
            cin >> len;
            lst = S.query_on_left_side(S.cursor() - len + 1, S.cursor())->m_value;
            cout << lst << endl;
        }
    }
}

int main() {
    solve_rollbackds<OY::RollbackSqrtMaxTable<uint32_t, OY::RollbackSQRT::RandomController<>, 12>>();
    // solve_rollbackds<OY::RollbackCatMaxTable<uint32_t, 18>>();
    // solve_rollbackds<OY::RollbackSTMaxTable<uint32_t, 18>>();
    // solve_rollbackds<OY::RollbackMonoMaxTree<uint32_t>>();
    // solve_monostack();
}