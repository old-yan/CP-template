### 一、模板类别

​	数据结构：单调栈。

​	练习题目：

1. [P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
2. [P1901 发射站](https://www.luogu.com.cn/problem/P1901)
3. [P2698 [USACO12MAR] Flowerpot S](https://www.luogu.com.cn/problem/P2698)
4. [P2866 [USACO06NOV] Bad Hair Day S](https://www.luogu.com.cn/problem/P2866)
5. [P3804 【模板】后缀自动机（SAM）](https://www.luogu.com.cn/problem/P3804)
6. [P5788 【模板】单调栈](https://www.luogu.com.cn/problem/P5788)


### 二、模板功能


#### 1.建立单调栈

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `typename Compare` ，表示比较函数的类型。
   
   模板参数 `typename Getter` ，表示从区间下标到值的映射函数的类型。
   
   模板参数 `typename PopCallback` ，表示每个元素从单调栈里被 `pop` 时调用的回调函数的类型。

   构造参数 `size_type length` ，表示区间长度。
   
   构造参数 `Comp comp` ，表示具体的比较函数。
   
   构造参数 `Getter getter` ，表示从区间下标到值的具体的映射函数。
   
   构造参数 `PopCallback pop_call` ，表示每个元素从单调栈里被 `pop` 时调用的具体的回调函数。
   
   构造参数 `const Tp &max`，表示元素的最大值。
   
   构造参数 `bool forward` ，表示是否正向调用单调栈。

2. 时间复杂度

     $O(n)$ 。

3. 备注

   本模板处理的问题为子数组的最值查询，当传递 `std::less<Tp>` 作为比较函数类型时，默认查询区间最大值。
   
   本模板可以分为从左往右（正向）拓展和从右往左（反向）拓展两种版本。正向拓展时，可以调用 `extend_right` 和 `query_on_left_side` 方法；反向拓展时，可以调用 `extend_left` 和 `query_on_right_side` 方法。以下文档均在正向拓展的情况下陈述。
   
   本模板进行查询的区间必须是单调的；形式化的，如果先查询 `[l1, r1]` ，再查询 `[l2, r1]` ，则必有 `r1 <= r2`。（若为反向延伸，则必有 `l1 >= l2`）
   
   在比较函数为 `std::less<Tp>` 时，单调栈为严格单调递减栈，可以查询区间最大值。本模板默认比较函数为 `std::less<Tp>` ，即查询区间最大值。（在比较函数为 `std::greater<Tp>` 时，单调栈为严格单调递增栈，可以查询区间最小值；在比较函数为 `std::less_equal<Tp>` 时，单调栈为非严格单调递减栈，可以查询区间最大值；在比较函数为 `std::greater_equal<Tp>` 时，单调栈为非严格单调递增栈，可以查询区间最小值）
   
   构造参数 `pop_call` 表示每个元素被 `pop` 时调用的回调函数。每个元素 `x` 必然有入栈的时候，而当后续有比 `x` 更大或者和 `x` 一样大的元素入栈时，元素 `x` 就会被弹出栈。本回调往往用于查询右侧第一个大于等于 `x` 的元素的位置。（在比较函数为 `std::greater<Tp>` 时，可以查询右侧第一个小于等于 `x` 的元素的位置；在比较函数为 `std::less_equal<Tp>` 时，可以查询右侧第一个大于 `x` 的元素的位置；在比较函数为 `std::greater_equal<Tp>` 时，可以查询右侧第一个小于 `x` 的元素的位置）
   
   构造参数 `max` 必须满足严格大于区间内的每个元素的值。（在比较函数为 `std::greater<Tp>` 时需满足严格小于区间内的每个元素的值；在比较函数为 `std::less_equal<Tp>` 时需满足大于等于区间内的每个元素的值；在比较函数为 `std::greater_equal<Tp>` 时需满足小于等于区间内的每个元素的值）。
   
   构造参数 `forward` 用于指示本单调栈是从左往右延伸，还是从右往左延伸。若为前者，区间右边缘初始化为 `-1` ，有待逐步向右拓展到 `n - 1` ；若为后者，区间左边缘初始化为 `n`，有待逐步向左拓展到 `0` 。
   
   **注意：**
   
   本模板不进行参数检查，使用者需要保证各种操作合法。在拓展右边界的操作中，右边界初始为 `-1` ，经过拓展后最远到达 `n - 1` ，之后再拓展则为非法操作；在查询操作中，针对的子区间必须为非空窗口，对空窗口进行查询为非法操作。

#### 2.拓展右边界(extend_right)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法使窗口的右边界向右滑动一个位置。
   
   本方法仅在区间边界为正向延伸时使用。
   
#### 3.拓展左边界(extend_left)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法使窗口的左边界向左滑动一个位置。
   
   本方法仅在区间边界为反向延伸时使用。

#### 4.在光标左侧查询区间最值(raw_query_on_left_side)

1. 数据类型
   
   输入参数 `size_type left` ，表示查询的区间的左边界。

   返回类型 `const node *` ，表示区间最值所在的下标以及其值。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法表示，在查询的子区间内的区间最值所在的下标，及其具体值。查询区间的左端为输入参数，查询区间的右端为本模板右边缘移动到的位置。
   
   如果窗口内有多个最值，则返回最后出现的下标。
   
   本方法仅在区间边界为正向延伸时使用。

#### 5.在光标右侧查询区间最值(raw_query_on_right_side)

1. 数据类型
   
   输入参数 `size_type right` ，表示查询的区间的右边界。

   返回类型 `const node *` ，表示区间最值所在的下标以及其值。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   本方法表示，在查询的子区间内的区间最值所在的下标，及其具体值。查询区间的右端为输入参数，查询区间的左端为本模板左边缘移动到的位置。
   
   如果窗口内有多个最值，则返回最后出现的下标。
   
   本方法仅在区间边界为反向延伸时使用。

#### 6.在光标左侧查询区间最值(query_on_left_side)

1. 数据类型
   
   输入参数 `size_type left` ，表示查询的区间的左边界。
   
   输入参数 `size_type right` ，表示查询的区间的右边界。

   返回类型 `const node *` ，表示区间最值所在的下标以及其值。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法与 `raw_query_on_left_side` 类似；指定了右边缘，如果当前右边缘小于等于 `right` ，则会先把右边界拓展到 `right` 再进行查询。
   
   本方法仅在区间边界为正向延伸时使用。
   
#### 6.在光标右侧查询区间最值(query_on_right_side)

1. 数据类型
   
   输入参数 `size_type left` ，表示查询的区间的左边界。
   
   输入参数 `size_type right` ，表示查询的区间的右边界。

   返回类型 `const node *` ，表示区间最值所在的下标以及其值。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本方法与 `raw_query_on_right_side` 类似；指定了左边缘，如果当前左边缘大于等于 `left` ，则会先把左边界拓展到 `left` 再进行查询。
   
   本方法仅在区间边界为反向延伸时使用。

### 三、模板示例

```c++
#include "DS/MonoStack.h"
#include "IO/FastIO.h"

void test_rmq() {
    // 查询每个区间的最值
    int arr[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    cout << "arr: ";
    for (int i = 0; i < 9; i++) cout << arr[i] << " \n"[i == 8];
    auto S_min = OY::make_MonoMaxStack(9, arr);
    for (int right = 0; right < 9; right++) {
        // 将右边界拓展到 right
        S_min.extend_right();
        for (int left = 0; left <= right; left++) {
            auto res = S_min.raw_query_on_left_side(left);
            cout << "max(arr[" << left << "~" << right << "]) = arr[" << res->m_index << "] " << res->m_value << '\n';
        }
    }
    cout << endl;
}

void test_find_right_larger() {
    // 查找每个元素右侧的最近的更大元素所在位置
    int arr[9] = {6, 1, 8, 7, 5, 3, 2, 9, 4};
    cout << "arr: ";
    for (int i = 0; i < 9; i++) cout << arr[i] << " \n"[i == 8];

    std::vector<int> right_larger(9);
    // 比较函数要用 std::less_equal
    auto getter = [&](int i) {
        return arr[i];
    };
    auto pop_call = [&](int popped, int by_who) {
        // popped 表示被弹出栈的元素的下标
        // by_who 表示是被谁淘汰出栈的
        right_larger[popped] = by_who;
    };
    OY::MONOSTACK::Table<int, std::less_equal<int>, decltype(getter), decltype(pop_call)> S(9, {}, getter, pop_call, 999, true);
    for (int right = 0; right < 9; right++) S.extend_right();

    // 检验一下对不对
    for (int i = 0; i < 9; i++) {
        int j = right_larger[i];
        if (j) {
            cout << "arr[" << j << "](" << arr[j] << ") > arr[" << i << "](" << arr[i] << ")\n";
        }
    }
    cout << endl;
}

int main() {
    test_rmq();
    test_find_right_larger();
}
```

```
#输出如下
arr: 4 9 2 3 5 7 8 1 6
max(arr[0~0]) = arr[0] 4
max(arr[0~1]) = arr[1] 9
max(arr[1~1]) = arr[1] 9
max(arr[0~2]) = arr[1] 9
max(arr[1~2]) = arr[1] 9
max(arr[2~2]) = arr[2] 2
max(arr[0~3]) = arr[1] 9
max(arr[1~3]) = arr[1] 9
max(arr[2~3]) = arr[3] 3
max(arr[3~3]) = arr[3] 3
max(arr[0~4]) = arr[1] 9
max(arr[1~4]) = arr[1] 9
max(arr[2~4]) = arr[4] 5
max(arr[3~4]) = arr[4] 5
max(arr[4~4]) = arr[4] 5
max(arr[0~5]) = arr[1] 9
max(arr[1~5]) = arr[1] 9
max(arr[2~5]) = arr[5] 7
max(arr[3~5]) = arr[5] 7
max(arr[4~5]) = arr[5] 7
max(arr[5~5]) = arr[5] 7
max(arr[0~6]) = arr[1] 9
max(arr[1~6]) = arr[1] 9
max(arr[2~6]) = arr[6] 8
max(arr[3~6]) = arr[6] 8
max(arr[4~6]) = arr[6] 8
max(arr[5~6]) = arr[6] 8
max(arr[6~6]) = arr[6] 8
max(arr[0~7]) = arr[1] 9
max(arr[1~7]) = arr[1] 9
max(arr[2~7]) = arr[6] 8
max(arr[3~7]) = arr[6] 8
max(arr[4~7]) = arr[6] 8
max(arr[5~7]) = arr[6] 8
max(arr[6~7]) = arr[6] 8
max(arr[7~7]) = arr[7] 1
max(arr[0~8]) = arr[1] 9
max(arr[1~8]) = arr[1] 9
max(arr[2~8]) = arr[6] 8
max(arr[3~8]) = arr[6] 8
max(arr[4~8]) = arr[6] 8
max(arr[5~8]) = arr[6] 8
max(arr[6~8]) = arr[6] 8
max(arr[7~8]) = arr[8] 6
max(arr[8~8]) = arr[8] 6

arr: 6 1 8 7 5 3 2 9 4
arr[2](8) > arr[0](6)
arr[2](8) > arr[1](1)
arr[7](9) > arr[2](8)
arr[7](9) > arr[3](7)
arr[7](9) > arr[4](5)
arr[7](9) > arr[5](3)
arr[7](9) > arr[6](2)

```

