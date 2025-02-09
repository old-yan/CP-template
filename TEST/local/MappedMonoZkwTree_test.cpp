#include "DS/MappedMonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // 假如，有 10 个桶
    // 查询 query(l, r) 表示查询 bucket[l~r] 这些桶的最大值的和
    // 空桶的贡献是 0
    using monoid = OY::MonoSumTree<int>::group;
    OY::MaxPqMonoZkwTree<monoid, int, 0> S(10);
    // 记得对桶做修改之后，要调用 S.update
    S[2].push(25), S[2].push(50);
    S.update(2);
    
    S[4].push(30);
    S.update(4);
    
    S[5].push(100), S[5].push(8);
    S.update(5);
    
    S[8].push(88);
    S.update(8);
    
    cout << "S.query(4, 6) = " << S.query(4, 6) << endl;
}

int main() {
    test();
}
/*
#输出如下
S.query(4, 6) = 130

*/