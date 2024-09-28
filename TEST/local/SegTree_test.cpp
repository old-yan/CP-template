#include "DS/SegTree.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

void test_normal_tree() {
    // 线段树可以实现 st 表的所有统计功能，但是查询速度稍微慢一些
    // 考虑到线段树是常用数据结构，所以会尽量给出各种使用示范

    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

        // 建立一个区间最大值线段树
        // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    // 建立一个区间最大值累加器
    auto my_max = [](int x, int y) {
        return x > y ? x : y;
    };
    auto tree_max = OY::make_SegTree(A, A + 10, my_max);
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } my_max;
    auto tree_max = OY::make_SegTree(A, A + 10, my_max);
#endif
    cout << "max(A[3~6])     =" << tree_max.query(3, 6) << endl;

    // 建立一个区间位操作线段树
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto tree_bit_and = OY::make_SegTree(A, A + 10, std::bit_and<int>()); // 按位与
    cout << "bit_and(A[3~6]) =" << tree_bit_and.query(3, 6) << endl;

    auto tree_bit_or = OY::make_SegTree(A, A + 10, std::bit_or<int>()); // 按位或
    cout << "bit_or(A[3~6])  =" << tree_bit_or.query(3, 6) << endl;

    auto tree_bit_xor = OY::make_SegTree(A, A + 10, std::bit_xor<int>());
    cout << "bit_xor(A[3~6]) =" << tree_bit_xor.query(3, 6) << endl;

    // 建立一个区间乘 ST 表
    using MulNode = OY::Seg::CustomNode<int64_t, std::multiplies<int64_t>>;
    OY::Seg::Tree<MulNode, OY::Seg::Ignore, uint32_t> tree_mul(0);
    tree_mul.reset(A, A + 10);
    cout << "prod(A[3~6])    =" << tree_mul.query(3, 6) << endl;
    // 树上二分查询，从下标 3 开始，最多乘到哪个位置，乘积就会超过 2304
    int max_r = tree_mul.max_right(3, [](long long x) { return x <= 2304; });
    cout << "max_r = " << max_r << endl;
    // 树上二分查询，从下标 6 开始，最多向左乘到哪个位置，乘积就会超过 2304
    int min_l = tree_mul.min_left(6, [](long long x) { return x <= 2304; });
    cout << "min_l = " << min_l << endl;

    // 便利化措施：由于实际使用的时候，往往是求和树较多，所以无参构造为求和树
    OY::VectorSegSumTree<int, uint32_t> tree_default;
    tree_default.reset(A, A + 10);
    cout << "sum(A[0~9])     =" << tree_default.query(0, 9) << endl;
    cout << "A[4]            =" << tree_default.query(4) << endl;
    tree_default.modify(4, 18);
    cout << tree_default << endl;
    tree_default.add(4, 100);
    cout << tree_default << endl;
    // 查询排名第 0 的元素是谁
    cout << "A.kth(0)        =" << tree_default.kth(0).m_index << endl;
    // 查询排名第 15 的元素是谁
    cout << "A.kth(15)       =" << tree_default.kth(15).m_index << endl;
    // 查询排名第 16 的元素是谁
    cout << "A.kth(16)       =" << tree_default.kth(16).m_index << endl;
}

void test_lazy_tree() {
    // 带懒惰标记的线段树可以实现线段树的所有统计功能
    // 重复功能不再展示，主要展示构造方法和一些特有的功能

    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 默认无参构造就是日常用的最多的求和树
    OY::VectorSegLazySumTree<int, uint32_t> T(A, A + 10);
    cout << "sum(A[3~6])     =" << T.query(3, 6) << endl;
    // 对区间 [4,5] 赋予 10 的增量变化
    T.add(4, 5, 10);
    // 查看当前树的值
    cout << T << endl;
    cout << "sum(A[3~6])     =" << T.query(3, 6) << endl;
    // 查询排名第 54 的元素是谁
    cout << "A.kth(54)       =" << T.kth(54).m_index << endl;
    // 查询排名第 55 的元素是谁
    cout << "A.kth(55)       =" << T.kth(55).m_index << endl;

    // 增值函数、囤积函数可以和区间操作函数完全不同
    // 比如，统计用的是最大值函数，修改用的是加法
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto getmax = [](int x, int y) { return x > y ? x : y; };
    auto map = [](int x, int y, int) { return x + y; }; // 尤其注意 map 必须支持三个参数
    auto com = [](int x, int y) { return x + y; };
    auto T_max_add = OY::make_lazy_SegTree<int, int, false>(A, A + 10, getmax, map, com);
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; };
    } getmax;
    struct {
        int operator()(int x, int y, int) const { return x + y; };
    } map; // 尤其注意 map 必须支持三个参数
    struct {
        int operator()(int x, int y) const { return x + y; };
    } com;
    auto T_max_add = OY::make_lazy_SegTree<int, int, false>(A, A + 10, getmax, map, com);
#endif
    cout << "max(A[3~6])     =" << T_max_add.query(3, 6) << endl;
    // 对区间 [4,5] 赋予 10 的增量变化
    T_max_add.add(4, 5, 10);
    cout << T_max_add << endl;
    cout << "max(A[3~6])     =" << T_max_add.query(3, 6) << endl;

    // 维护一个整数序列，对其进行乘法、加法的修改，则可以把乘法加法的增量抽象为一个运算结点
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    struct opNode {
        int mul = 1, add = 0;
    };
    auto op = [](int x, int y) { return x + y; };
    auto map2 = [](opNode x, int y, int size) { return y * x.mul + x.add * size; };
    auto com2 = [](opNode x, opNode y) { return opNode{y.mul * x.mul, y.add * x.mul + x.add}; };
    auto T2 = OY::make_lazy_SegTree<int, opNode, false>(A, A + 10, op, map2, com2, opNode());
    cout << T2 << endl;
    T2.add(2, 5, {1, 5});
    cout << T2 << endl;
    T2.add(4, 6, {2, 0});
    cout << T2 << endl;
    T2.add(3, 7, {1, 10});
    cout << T2 << endl;
#else
    struct opNode {
        int mul = 1, add = 0;
        static opNode make_opNode(int mul, int add) {
            opNode res;
            res.mul = mul, res.add = add;
            return res;
        };
    };
    struct {
        int operator()(int x, int y) const { return x + y; };
    } op;
    struct {
        int operator()(opNode x, int y, int size) const { return y * x.mul + x.add * size; };
    } map2; // 尤其注意 map2 必须支持三个参数
    struct {
        opNode operator()(opNode x, opNode y) const { return opNode::make_opNode(y.mul * x.mul, y.add * x.mul + x.add); }
    } com2;
    auto T2 = OY::make_lazy_SegTree<int, opNode, false>(A, A + 10, op, map2, com2);
    cout << T2 << endl;
    T2.add(2, 5, opNode::make_opNode(1, 5));
    cout << T2 << endl;
    T2.add(4, 6, opNode::make_opNode(2, 0));
    cout << T2 << endl;
    T2.add(3, 7, opNode::make_opNode(1, 10));
    cout << T2 << endl;
#endif
    cout << "sum(A[~])       =" << T2.query_all() << endl;
}

void tricks() {
    // 在 oj 做题时，往往要把一个连续数组构建成线段树。
    // 如果先存到 vector 再存线段树，未免有脱裤子放屁之嫌
    // 按我这么写即可在线段树内逐个输入数据，不需要外来的 vector
    OY::VectorSegSumTree<int, uint32_t> tree_by_cin(0, [](uint32_t) {
        int64_t num;
        cin >> num;
        return num;
    });

    // 如果有个长达 1e9 的区间，区间初始值为 a[i]=i，要维护区间和
    // 显然，一开始不能初始化全部结点
    struct RangeMapping {
        int64_t operator()(int64_t l, int64_t r) const {
            return (l + r) * (r - l + 1) / 2;
        }
    };
    auto tree = OY::make_SegTree<int64_t, RangeMapping, uint32_t>(1000000000u, std::plus<int64_t>());
    // 来查询一下 1~80000000 的和
    cout << tree.query(1, 80000000) << endl;
}

int main() {
    test_normal_tree();
    test_lazy_tree();
    tricks();
}
/*
#输出如下
11 5 9 12 8 4 6 15 7 7
max(A[3~6])     =12
bit_and(A[3~6]) =0
bit_or(A[3~6])  =14
bit_xor(A[3~6]) =6
prod(A[3~6])    =2304
max_r = 6
min_l = 3
sum(A[0~9])     =84
A[4]            =8
[11, 5, 9, 12, 18, 4, 6, 15, 7, 7]
[11, 5, 9, 12, 118, 4, 6, 15, 7, 7]
A.kth(0)        =0
A.kth(15)       =1
A.kth(16)       =2
11 5 9 12 8 4 6 15 7 7
sum(A[3~6])     =30
[11, 5, 9, 12, 18, 14, 6, 15, 7, 7]
sum(A[3~6])     =50
A.kth(54)       =4
A.kth(55)       =5
max(A[3~6])     =12
[11, 5, 9, 12, 18, 14, 6, 15, 7, 7]
max(A[3~6])     =18
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
[11, 5, 14, 17, 13, 9, 6, 15, 7, 7]
[11, 5, 14, 17, 26, 18, 12, 15, 7, 7]
[11, 5, 14, 27, 36, 28, 22, 25, 7, 7]
sum(A[~])       =182
3200000040000000

*/