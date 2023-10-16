#include "DS/DivideTree.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认划分树（排序函数为小于号）
    // 自定义排序函数：按长度排序
    // 注意 C++14 之前必须显式声明比较函数的类
    auto dt = OY::make_DivideTree<std::less<int>, 1000>(A, A + 10);

    cout << "A[3~6] No.1 = " << dt.quantile(3, 6, 0) << endl;
    cout << "A[3~6] No.2 = " << dt.quantile(3, 6, 1) << endl;
    cout << "A[3~6] No.3 = " << dt.quantile(3, 6, 2) << endl;
    cout << "A[3~6] No.4 = " << dt.quantile(3, 6, 3) << endl;

    // 再应用到字符串数组上
    std::vector<std::string> B{"hello", "world", "I", "am", "a", "robot"};
    for (int i = 0; i < B.size(); i++) cout << B[i] << (i == B.size() - 1 ? '\n' : ' ');

        // 自定义排序函数：按长度排序
        // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto comp = [](auto &x, auto &y) {
        return x.size() < y.size();
    };
#else
    struct {
        bool operator()(const std::string &x, const std::string &y) const {
            return x.size() < y.size();
        };
    } comp;
#endif
    auto dt_str = OY::make_DivideTree<decltype(comp), 1000>(B.begin(), B.end());

    cout << "B[1~4] No.1 = " << dt_str.quantile(1, 4, 0) << endl;
    cout << "B[1~4] No.2 = " << dt_str.quantile(1, 4, 1) << endl;
    cout << "B[1~4] No.3 = " << dt_str.quantile(1, 4, 2) << endl;
    cout << "B[1~4] No.4 = " << dt_str.quantile(1, 4, 3) << endl;
    // 可以看出，划分树可以保留相等元素之间的相对顺序
}
/*
#输出如下
1 5 6 3 8 4 4 2 10 1
A[3~6] No.1 = 3
A[3~6] No.2 = 4
A[3~6] No.3 = 4
A[3~6] No.4 = 8
hello world I am a robot
B[1~4] No.1 = I
B[1~4] No.2 = a
B[1~4] No.3 = am
B[1~4] No.4 = world

*/