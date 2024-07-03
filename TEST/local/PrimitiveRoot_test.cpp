#include "IO/FastIO.h"
#include "MATH/PollardRho.h"
#include "MATH/PrimitiveRoot.h"
#include "MATH/StaticModInt32.h"

int main() {
    static constexpr uint32_t P = 101;
    using mint = OY::StaticModInt32<P, true>;

    using Solver = OY::PROOT::PollardRhoSolver<mint, OY::PollardRho>;
    if (Solver::has_primitive_root()) {
        Solver sol;
        cout << "primitive root count = " << sol.count() << endl;
        cout << "minimal primitive root = " << sol.query_min() << endl;
        sol.enumerate([](mint proot) {
            cout << "primitive root: " << proot << endl;
        });
    }
}
/*
#输出如下
primitive root count = 40
minimal primitive root = 2
primitive root: 2
primitive root: 8
primitive root: 27
primitive root: 7
primitive root: 28
primitive root: 11
primitive root: 75
primitive root: 98
primitive root: 89
primitive root: 53
primitive root: 40
primitive root: 59
primitive root: 34
primitive root: 35
primitive root: 55
primitive root: 18
primitive root: 72
primitive root: 86
primitive root: 63
primitive root: 50
primitive root: 99
primitive root: 93
primitive root: 74
primitive root: 94
primitive root: 73
primitive root: 90
primitive root: 26
primitive root: 3
primitive root: 12
primitive root: 48
primitive root: 61
primitive root: 42
primitive root: 67
primitive root: 66
primitive root: 46
primitive root: 83
primitive root: 29
primitive root: 15
primitive root: 38
primitive root: 51

*/