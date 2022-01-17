### 一、模板类别

​	数据结构：划分树

### 二、模板功能

#### 1.建立划分树

1. 数据类型

   模板参数 `typename _Tp`​ ，表示要进行区间第 `k` 序值查询的元素类型。

   模板参数 `typename _Compare`​ ，表示比较函数的类，默认值为 `_Tp` 类的小于号函数类。

   构造参数 `_Iterator __first​` ，表示静态区间的区间头。

   构造参数 `_Iterator __last` ，表示静态区间的区间尾。（开区间）

   构造参数 `_Compare __comp`​ ，表示具体的排序函数，默认值为 `_Compare` 的默认构造实例。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   划分树处理的问题为区间第 `k​` 序值查询，要求区间内的元素能够按照指定排序函数进行排序。

   如果在构造的时候不想直接填区间，可以忽略 `__first` 和 `__last` 两个参数，只填 `__comp` 参数；之后再通过 `reset` 方法来填写区间。
   
   划分树只能处理静态区间上的问题，所以没有提供修改区间的入口。

#### 2.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间查询的区间头。

   输入参数 `_Iterator __last` ，表示区间查询的区间尾。（开区间）

2. 时间复杂度

   $O(n \cdot \log n)$ 。

3. 备注

   使用迭代器进行重置，可以将区间初状态直接赋到划分树里。

#### 3.查询

1. 数据类型

   输入参数 `int __left​` ，表示要查询的区间的左边界下标。

   输入参数 `int __right​` ，表示要查询的区间的右边界下标。（闭区间）

   输入参数 `int __k​` ，表示要查询的元素的次序。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   `k​` 的取值范围从 `0` 开始，表示**如果**将原区间的 `[l,r]` 内按照指定排序函数进行排序，那么排在 `[l+k]` 位置的元素。

   本函数没有进行参数检查，所以请自己确保下标合法，且 `k​` 处于 `[0,r-l]` 范围内。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "DS/DivideTree.h"

int main() {
    //先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    //建立一个默认划分树（排序函数为小于号）
    //一般可以忽略模板参数
    OY::DivideTree dt(A, A + 10);

    cout << "A[3~6] No.1 = " << dt.query(3, 6, 0) << endl;
    cout << "A[3~6] No.2 = " << dt.query(3, 6, 1) << endl;
    cout << "A[3~6] No.3 = " << dt.query(3, 6, 2) << endl;
    cout << "A[3~6] No.4 = " << dt.query(3, 6, 3) << endl;

    //再应用到字符串数组上
    std::vector<std::string> B{"hello", "world", "I", "am", "a", "robot"};
    for (int i = 0; i < B.size(); i++)
        cout << B[i] << (i == B.size() - 1 ? '\n' : ' ');
    //自定义排序函数：按长度排序
    auto comp = [](auto &x, auto &y) {
        return x.size() < y.size();
    };
    OY::DivideTree dt_str(B.begin(), B.end(), comp);

    cout << "B[1~4] No.1 = " << dt_str.query(1, 4, 0) << endl;
    cout << "B[1~4] No.2 = " << dt_str.query(1, 4, 1) << endl;
    cout << "B[1~4] No.3 = " << dt_str.query(1, 4, 2) << endl;
    cout << "B[1~4] No.4 = " << dt_str.query(1, 4, 3) << endl;
    //可以看出，划分树可以保留相等元素之间的相对顺序

    //和 ST 表一样，可以在构造的时候只设置比较函数，然后再去设置区间
    std::function comp2 = [](std::string &x, std::string &y) {
        return x.size() > y.size();
    };
    OY::DivideTree dt_str2(comp2);
    dt_str2.reset(B.begin(), B.end());
    cout << "B[0~5] No.1 = " << dt_str2.query(0, 5, 0) << endl;

    //甚至比较函数也可以按默认，那么类型会默认为 <int,std::less<int>>
    OY::DivideTree dt2;
    dt2.reset(A, A + 10);
    cout << "A[0~9] No.1 = " << dt2.query(0, 9, 0) << endl;
}
```

```
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
B[1~4] No.4 = hello
B[0~5] No.1 = world
A[0~9] No.1 = 1
```

