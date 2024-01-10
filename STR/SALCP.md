### 一、模板类别

​	序列：最长公共前缀助手-后缀数组版。

​	练习题目：

1. [P1117 [NOI2016] 优秀的拆分](https://www.luogu.com.cn/problem/P1117)
2. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)


### 二、模板功能

#### 1.构造助手

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示长度、下标类型。

   模板参数 `size_type MAX_LEN` ，表示单次处理的最大区间长度。

   构造参数 `Iterator first` ，表示序列的起始位置。

   构造参数 `Iterator last` ，表示序列的结束位置。（开区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   模板参数 `MAX_LEN` 表示序列长度的最大值。

   **注意：** 
   
   本模板要求序列中所有元素可以类型转换为数字，且元素的值域为非负。若值域过大，请对元素值进行离散化处理；若值域有负值元素，请对元素值进行偏移处理。
   
   如果传入的元素的值域范围超出 `MAX_LEN` 大小，或者最小元素小于 `0` ，则在模板中会对元素进行离散化处理；此时模板的时间复杂度会增加一个 `log` 。

#### 2.查询子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `size_type a` ，表示第一个子串的开始位置下标。

   输入参数 `size_type b` ，表示第二个子串的开始位置下标。

   输入参数 `size_type limit` ，表示要求的最长公共前缀的上限。

2. 时间复杂度

   $O(1)$ ，此处 `n` 指 `limit` 大小。

3. 备注

   本函数不进行参数检查，所以使用者须保证参数合法，即 `limit` 为可能的子串长度。

#### 3.查询子串最长公共前缀(lcp)

1. 数据类型

   输入参数 `size_type a` ，表示第一个子串的开始位置下标。

   输入参数 `size_type b` ，表示第二个子串的开始位置下标。

2. 时间复杂度

   $O(1)$ ，此处 `n` 指序列长度。

#### 4.比较子串大小(compare)

1. 数据类型

   输入参数 `size_type l1` ，表示第一个子串的开始位置下标。

   输入参数 `size_type r1` ，表示第一个子串的结束位置下标。

   输入参数 `size_type l2` ，表示第二个子串的开始位置下标。

   输入参数 `size_type r2` ，表示第二个子串的结束位置下标。

   返回类型 `int` 。

2. 时间复杂度

   $O(1)$ ，此处 `n` 指序列长度。

3. 备注

   两个子串的比较结果可能为小于、等于、大于，分别对应返回值 `-1`、`0`、`1` 。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SALCP.h"

int main() {
    // 给出主串
    std::string s = "abcabbbabcb";
    // 构造助手
    auto S = OY::make_SA_LCP<1000>(s);
    // 查询 lcp
    cout << s.substr(0) << " and " << s.substr(7) << " 's lcp = " << S.lcp(0, 7) << endl;
    // 查询带上限的 lcp
    cout << s.substr(0, 2) << " and " << s.substr(7, 2) << " 's lcp = " << S.lcp(0, 7, 2) << endl;
    // 比较子串
    int res = S.compare(0, 3, 7, 10);
    if (res < 0)
        cout << s.substr(0, 4) << " is smaller than " << s.substr(7, 4) << endl;
    else if (res > 0)
        cout << s.substr(0, 4) << " is bigger than " << s.substr(7, 4) << endl;
    else
        cout << s.substr(0, 4) << " is equal to " << s.substr(7, 4) << endl;
}
```

```
#输出如下
abcabbbabcb and abcb 's lcp = 3
ab and ab 's lcp = 2
abca is smaller than abcb

```

