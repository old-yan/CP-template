### 一、模板类别

​	数据结构： `MonoZkw` 二维线段树。

​	练习题目：

1. [Luck and Love](https://acm.hdu.edu.cn/showproblem.php?pid=1823)
2. [Check Corners](http://acm.hdu.edu.cn/showproblem.php?pid=2888)
3. [Mosaic](https://acm.hdu.edu.cn/showproblem.php?pid=4819)
4. [P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)

### 二、模板功能


​		本模板为 `MonoZkwTree` 的二维版本。



​		本模板通过模板参数 `typename CommutativeMonoid` 确定交换幺半群。交换幺半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

    本模板要求区间操作函数的运算符满足**结合律**以及**交换律**。

​		为了方便使用，本模板提供了 `MinTree` ， `MaxTree` ， `GcdTree` ， `LcmTree` ， `BitAndTree` ， `BitOrTree` ， `BitXorTree` ， `SumTree` 八种特化形式，需要填写的参数更少。


### 三、模板示例

```c++
#include "DS/MonoZkwTree2D.h"
#include "IO/FastIO.h"

int main() {
    // 二维线段树与一维线段树的用法基本类似，所以只展示一下特殊方法
    int matrix[4][5] = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
    };
    // 除了行数、列数，还需要传递一个寻址函数
    OY::MonoSumTree2D<int> T(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T << endl;
    // 输出[0,2]行，[1,4]列的和
    cout << "sum(matrix[0~2][1~4])=" << T.query(0, 2, 1, 4) << endl;

    // 来写一个二维树状数组做不到的：区域乘积
    struct Monoid {
        using value_type = int;
        static value_type identity() { return 1; }
        static value_type op(value_type x, value_type y) { return x * y; }
    };
    OY::MONOZKW2D::Tree<Monoid> T_mul(4, 5, [&](int i, int j) { return matrix[i][j]; });
    cout << T_mul << endl;
    // 输出[0,2]行，[1,4]列的最大值
    cout << "prod(matrix[0~1][2~4])=" << T_mul.query(0, 1, 2, 4) << endl;
}
```

```
#输出如下
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
sum(matrix[0~2][1~4])=27
[[3, 0, 1, 4, 2], [5, 6, 3, 2, 1], [1, 2, 0, 1, 5], [4, 1, 0, 1, 7]]
prod(matrix[0~1][2~4])=48

```

