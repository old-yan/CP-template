### 一、模板类别

​	序列： `Primitive Repetition` 助手。

### 二、模板功能

#### 1. 构造

1. 数据类型

   模板参数 `typename _Tp` ，表示序列中元素的类型。

   模板参数 `typename _SASolver` ，表示计算后缀数组的类。默认为 `GetSuffixArray` 。

   模板参数 `typename _STSolver` ，表示进行区间查询的类。默认为 `STTable` 。

   构造参数 `_Iterator first` ，表示序列开始位置。

   构造参数 `_Iterator last` ，表示序列结束位置。（开区间）

   构造参数 `uint32_t __alpha` ，表示序列中的元素值域范围。默认为 `128` 。

2. 时间复杂度

   $O(n)\sim O(n\cdot \log n)$ ，取决于区间查询的效率

3. 备注

   本模板适用于包括字符串在内的序列问题。由于所有序列问题本质是一样的，所以以下以字符串为例进行分析。

   在字符串中可以发现很多的周期重复，如果有个子串最小周期为 `q` ，而这个子串在满足周期 `q` 的条件下向左向右延伸，最大长度达到 `q` 的二倍，那么称这个子串为一个 `Run` 。注意 `Run` 的长度不一定为 `q` 的倍数。

   如果添加限定，子串的最小周期为 `q` ，且子串长度为 `q*2` ，那么这样的子串称为 `Primitive Repetition` ，即本原重串。

   显然，一个 `Run` 可以拆分出一个或者多个 `Primitive Repetition` 。

#### 2. 获取本原重串

1. 数据类型

   返回类型 `std::vector<_PrimitiveRepetition`  ，表示所有的本原重串。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   本方法的返回值按照 `left` 为第一关键字，`right` 为第二关键字升序排列。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/RepetitionHelper_sa.h"

int main() {
    //给出一个字符串
    std::string s = "aababaababb";
    //构造助手
    OY::RepetitionHelper_sa<> rh(s.begin(), s.end(), 128);
    //查看所有的 Run
    for (auto &[left, right, length] : rh.m_runs)
        cout << "run: s[" << left << '~' << right << "]: " << s.substr(left, right - left + 1) << endl;
    //查看所有的 Primitive Repetition
    for (auto &[left, right] : rh.getPrimitiveRepetition())
        cout << "primitive repetition: s[" << left << '~' << right << "]: " << s.substr(left, right - left + 1) << endl;
}
```

```
#输出如下
run: s[0~1]: aa
run: s[0~9]: aababaabab
run: s[1~5]: ababa
run: s[3~8]: abaaba
run: s[5~6]: aa
run: s[6~9]: abab
run: s[9~10]: bb
primitive repetition: s[0~1]: aa
primitive repetition: s[0~9]: aababaabab
primitive repetition: s[1~4]: abab
primitive repetition: s[2~5]: baba
primitive repetition: s[3~8]: abaaba
primitive repetition: s[5~6]: aa
primitive repetition: s[6~9]: abab
primitive repetition: s[9~10]: bb

```

