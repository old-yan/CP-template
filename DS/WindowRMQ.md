### 一、模板类别

​	数据结构：滑动窗口。

​	练习题目：

1. [P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
2. [P2216 [HAOI2007] 理想的正方形](https://www.luogu.com.cn/problem/P2216)



### 二、模板功能


#### 1.建立滑动窗口

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `typename Compare` ，表示比较函数的类型。
   
   模板参数 `typename Getter` ，表示从区间下标到值的映射函数的类型。

   构造参数 `size_type length` ，表示区间长度。
   
   构造参数 `size_type window_len` ，表示维护滑动窗口的长度。
   
   构造参数 `Comp comp` ，表示具体的比较函数。
   
   构造参数 `Getter getter` ，表示从区间下标到值的具体的映射函数。

2. 时间复杂度

     $O(n)$ 。

3. 备注

   本模板处理的问题为定长子数组的最值查询，当传递 `std::less<Tp>` 作为比较函数类型时，默认查询区间最大值。
   
   本模板进行查询的区间必须是从左往右的；只能先查询 `[0, k-1]` ，再查询 `[1, k]` ，再往后...而不能打乱。
   
   初始时区间右边界为 `-1` ，表示维护空区间，需要在拓展右边界之后再进行查询。
   
   **注意：**
   
   本模板不进行参数检查，使用者需要保证各种操作合法。在拓展右边界的操作中，右边界初始为 `-1` ，经过拓展后最远到达 `n - 1` ，之后再拓展则为非法操作；在查询操作中，针对的窗口必须为非空窗口，对空窗口进行查询为非法操作。

#### 2.拓展右边界(extend_right)

1. 数据类型

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法使窗口的右边界向右滑动一个位置；由于维护定长滑动窗口，所以左边界也自动向右滑动一个位置。
   
   特别的，如果拓展后的区间长度仍然没有达到指定长度（也就是右边界小于 `m_window_len - 1`），左边界就不会跟着移动。
   
#### 3.拓展右边界到某位置(extend_to)

1. 数据类型

   输入参数 `size_type right` ，表示想令有边界拓展到的位置。

2. 时间复杂度

   均摊 $O(n)$ ，此处 `n` 指目标位置和当前位置的距离。

3. 备注

   本方法使窗口的右边界不断向右滑动直到来到 `right` 位置。

#### 4.查询区间最值(raw_query)

1. 数据类型
   
   返回类型 `const node *` ，表示区间最值所在的下标以及其值。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   本方法表示，在当前维护的窗口内，区间最值所在的下标，及其具体值。
   
   如果窗口内有多个最值，则返回最后出现的下标。

#### 5.查询下一个窗口(next)

1. 数据类型

   返回类型 `const node *` ，表示区间最值所在的下标以及其值。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法将右边界向右拓展一位，然后查询当前窗口的最值。

#### 6.查询当前窗口的右边界(cursor)

1. 数据类型

   返回类型 `size_type` ，表示当前维护的窗口的右边界。

2. 时间复杂度

    $O(1)$ 。
    
#### 7.查询滑动窗口是否已经滑完(is_finished)

1. 数据类型

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

### 三、模板示例

```c++
#include "DS/WindowRMQ.h"
#include "IO/FastIO.h"

void test_arr_rmq() {
    // 一般来说，对静态数组进行查询可以使用 make_ 系列函数
    int arr[9] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    cout << "arr: ";
    for (int i = 0; i < 9; i++) cout << arr[i] << " \n"[i == 8];

    // 最小值窗口
    auto S_min = OY::make_MinWindow(9, 3, arr);
    for (int i = 0; i < 9; i++) {
        auto res = S_min.next();
        cout << "min(arr[" << std::max(0, i - 2) << "~" << i << "]) = arr[" << res->m_index << "] " << res->m_value << '\n';
    }

    std::vector<std::string> arr2{"44", "99", "22", "33", "55", "77", "88", "11", "66"};
    cout << "\narr2: ";
    for (int i = 0; i < 9; i++) cout << arr2[i] << " \n"[i == 8];
    // 最大值窗口
    auto S_max = OY::make_MaxWindow(9, 5, arr2);
    // 如果不想查询长度太短的窗口，可以先把右边界拓展到长度差一位就够的地方
    S_max.extend_to(3);
    for (int i = 4; i < 9; i++) {
        auto res = S_max.next();
        cout << "max(arr2[" << std::max(0, i - 4) << "~" << i << "]) = arr2[" << res->m_index << "] " << res->m_value << '\n';
    }

    cout << endl
         << endl;
}

void test_custom_rmq() {
    // 一些自定义的序列上的应用
    int a[5][2] = {{3, 159},
                   {9, 325},
                   {7, 916},
                   {8, 287},
                   {6, 730}};
    // 对 [3, 9, 7, 8, 6] 建立最大值窗口
    cout << "col1: ";
    for (int i = 0; i < 5; i++) cout << a[i][0] << " \n"[i == 4];
    auto getter = [&](int i) {
        return a[i][0];
    };
    auto S_max = OY::MaxWindow<int, decltype(getter)>(5, 3, {}, getter);
    S_max.extend_to(1);
    for (int i = 2; i < 5; i++) {
        auto res = S_max.next();
        cout << "max(col1[" << std::max(0, i - 2) << "~" << i << "]) = col1[" << res->m_index << "] " << res->m_value << '\n';
    }

    // 对 [159, 325, 916, 287, 730] 建立窗口，求十位数最大的数字
    cout << "\ncol2: ";
    for (int i = 0; i < 5; i++) cout << a[i][1] << " \n"[i == 4];
    // 自定义比较函数
    auto comp = [](int x, int y) {
        return (x % 100 / 10) < (y % 100 / 10);
    };
    auto getter2 = [&](int i) {
        return a[i][1];
    };
    auto S_max2 = OY::WINDOW::Table<int, decltype(comp), decltype(getter2)>(5, 2, comp, getter2);
    S_max2.extend_to(0);
    for (int i = 1; i < 5; i++) {
        auto res = S_max2.next();
        cout << "max(col2[" << std::max(0, i - 1) << "~" << i << "]) = col2[" << res->m_index << "] " << res->m_value << '\n';
    }
}

int main() {
    test_arr_rmq();
    test_custom_rmq();
}
```

```
#输出如下
arr: 4 9 2 3 5 7 8 1 6
min(arr[0~0]) = arr[0] 4
min(arr[0~1]) = arr[0] 4
min(arr[0~2]) = arr[2] 2
min(arr[1~3]) = arr[2] 2
min(arr[2~4]) = arr[2] 2
min(arr[3~5]) = arr[3] 3
min(arr[4~6]) = arr[4] 5
min(arr[5~7]) = arr[7] 1
min(arr[6~8]) = arr[7] 1

arr2: 44 99 22 33 55 77 88 11 66
max(arr2[0~4]) = arr2[1] 99
max(arr2[1~5]) = arr2[1] 99
max(arr2[2~6]) = arr2[6] 88
max(arr2[3~7]) = arr2[6] 88
max(arr2[4~8]) = arr2[6] 88


col1: 3 9 7 8 6
max(col1[0~2]) = col1[1] 9
max(col1[1~3]) = col1[1] 9
max(col1[2~4]) = col1[3] 8

col2: 159 325 916 287 730
max(col2[0~1]) = col2[0] 159
max(col2[1~2]) = col2[1] 325
max(col2[2~3]) = col2[3] 287
max(col2[3~4]) = col2[3] 287

```

