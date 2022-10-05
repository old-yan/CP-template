### 一、模板类别

​	序列： `Duval` 算法获取 `Lyndon` 分解。

### 二、模板功能

#### 1.

1. 数据类型

   输入参数 `_Iterator first` ，表示序列开始位置。

   输入参数 `_Iterator last` ，表示序列结束位置。（开区间）

   返回类型 `std::vector<uint32_t>` ，表示分割出的每个子串的起始下标。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   如果一个字符串，经过任意的旋转平移之后，字典序只会变得更大，那么称这个字符串是一个 **`Lyndon` 串**。
   
   如果将一个字符串，分割成若干个子串，每个子串都是一个 `Lyndon` 串，而且各个子串之间呈字典序非递增顺序。那么称这个分割方法为 **`Lyndon` 分解。**

   获取 `Lyndon` 分解的结果是唯一的。算法可以有很多种。本模板采用的方法是 `Duval` 算法。
   
   **注意：**经过 `Lyndon` 分解后，原串的最小表示的起始下标，必在最后一个子串的起始处，或在倒数第二个子串起始处（如果有的话）。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/Lyndon.h"

int main() {
    //给出一个字符串
    std::string s = "ccbcabcabcabaa";
    auto split = OY::Duval(s.begin(), s.end());
    for (uint32_t i = 0; i < split.size(); i++) {
        uint32_t start = split[i];
        uint32_t end = i + 1 == split.size() ? s.size() : split[i + 1];
        cout << "No." << i << " Lyndon subtr: " << s.substr(start, end - start) << '\n';
    }
}
```

```
#输出如下
No.0 Lyndon subtr: c
No.1 Lyndon subtr: c
No.2 Lyndon subtr: bc
No.3 Lyndon subtr: abc
No.4 Lyndon subtr: abc
No.5 Lyndon subtr: ab
No.6 Lyndon subtr: a
No.7 Lyndon subtr: a

```

