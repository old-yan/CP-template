#include "IO/LeetcodeIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt32.h"
using namespace std;

/*
[1359. 有效的快递序列数目](https://leetcode.cn/problems/count-all-valid-pickup-and-delivery-options)
*/
/**
 * 本题可以对每队括号进行隔板法插空
 */

using mint = OY::mint1000000007;
OY::CombinationTable<mint, 1000> ct;
class Solution {
public:
    int countOrders(int n) {
        mint ans = 1;
        for (int i = 0; i < n; i++) ans *= ct.comb(i * 2 + 2, 2);
        return ans.val();
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(countOrders);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif