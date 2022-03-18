### 一、模板类别

​	数学：位运算助手

### 二、模板功能

​	位运算助手的所有函数都是静态函数，不需要构造实例就可以调用。当然，方便起见，也可以生成一个实例。生成时需要指定模板的元素类型，为 `int` ， `unsigned int` ， `long long` 或者 `unsigned long long`  。

​	下文中，当用序号表示某一位时，按照从低位到高位升序编号。即“个位”编号为 `0` ，“十位”编号为 `1` ，……。`32` 位数字的编号范围为 `[0,31]` ，`64` 位数字的编号范围为 `[0,63]` 。

#### 1.查询位长

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

3. 备注

   如果元素类型为 `int` 或 `unsigned int` ，返回 `32` ；如果元素类型为 `long long` 或者 `unsigned long long` ，返回 `64` 。

#### 2.查询 1 的数量

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。


#### 3.查询某一位是否为 1

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

   输入参数 `uint8_t i` ，表示要查询的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `i` 超出 `[0, length())` 范围，会特判返回 `false` 。

#### 4.查询某一位是否为 0

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

   输入参数 `uint8_t i` ，表示要查询的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `i` 超出 `[0, length())` 范围，会特判返回 `true` 。

#### 5.查询两元素是否有交集

1. 数据类型

   输入参数 `_Tp mask1` ，表示要查询的元素。

   输入参数 `_Tp mask2` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。

#### 6.查询前者是否包含后者

1. 数据类型

   输入参数 `_Tp mask1` ，表示要查询的元素。

   输入参数 `_Tp mask2` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。


#### 7.查询后者是否包含前者

1. 数据类型

   输入参数 `_Tp mask1` ，表示要查询的元素。

   输入参数 `_Tp mask2` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。


#### 8.查询开头的 0 的数量

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `mask==0` ，会特判返回 `length()` 的值。

#### 9.查询开头的 1 的数量

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `mask==-1` ，会特判返回 `length()` 的值。

#### 10.查询结尾的 0 的数量

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   若 `mask==0` ，会特判返回 `length()` 。

#### 11.查询结尾的 1 的数量

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `mask==-1` ，会特判返回 `length()` 的值。

#### 12.生成全部为 1 的掩模

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


#### 13.生成某位为 1 的掩模

1. 数据类型

   输入参数 `uint8_t i` ，表示要生成的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   当输入的 `i>=length()`  的情况下，返回 `0` 。


#### 14.生成某个范围全为 1 的掩模

1. 数据类型

   输入参数 `uint8_t l` ，表示要生成的范围的起始下标。

   输入参数 `uint8_t r` ，表示要生成的范围的结尾下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（`l<=r`）

#### 15.生成末尾有若干个 1 的掩模

1. 数据类型

   输入参数 `uint8_t k` ，表示要生成的 `1` 的数量。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   当输入的 `i>length()`  时，相当于 `i=length()` ，也即相当于生成全部为 `1` 的掩模。

#### 16.获取某元素的某一位掩模

1. 数据类型

   输入参数 `_Tp mask` ，表示要获取的元素。

   输入参数 `uint8_t i` ，表示要获取的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   当输入的 `i>=length()`  的情况下，返回 `0` 。

#### 17.获取某元素的某一范围的掩模

1. 数据类型

   输入参数 `_Tp mask` ，表示要获取的元素。

   输入参数 `uint8_t l` ，表示要获取的范围的起始下标。

   输入参数 `uint8_t r` ，表示要获取的范围的结尾下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（`l<=r`）

#### 18.获取某元素的最低位的 1

1. 数据类型

   输入参数 `_Tp mask` ，表示要获取的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   经测试，若 `mask==0` ，`clang` 对无符号数返回返回 `0` ，对有符号数返回随机数； `gcc` 统一返回`0` 。

#### 19.查询向上取整到最近的 2 的幂

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `mask==0`  ，返回 `0` 。

   若 `mask` 最高位为 `1` 且含有至少两个 `1`，返回`0` 。

#### 20.查询向下取整到最近的 2 的幂

1. 数据类型

   输入参数 `_Tp mask` ，表示要查询的元素。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `mask==0`  ，返回 `0` 。

#### 21.获取某元素的最低位的连续的 1

1. 数据类型

   输入参数 `_Tp mask` ，表示要获取的元素。

2. 时间复杂度

   $O(1)$ 。

#### 22.将某元素设为全部为 1

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

2. 时间复杂度

   $O(1)$ 。

#### 23.将某元素某位设为 1

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

   输入参数 `uint8_t i` ，表示要设置的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `i>=length()` ，设置无效。

#### 24.将某元素某一范围设为 1

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

   输入参数 `uint8_t l` ，表示要设置的范围的起始下标。

   输入参数 `uint8_t r` ，表示要设置的范围的结尾下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（`l<=r`）

#### 25.将某元素设为全部为 0

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

2. 时间复杂度

   $O(1)$ 。

#### 26.将某元素某位设为 0

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

   输入参数 `uint8_t i` ，表示要设置的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `i>=length()` ，设置无效。

#### 27.将某元素某一范围设为 0

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

   输入参数 `uint8_t l` ，表示要设置的范围的起始下标。

   输入参数 `uint8_t r` ，表示要设置的范围的结尾下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（`l<=r`）

#### 28.将某元素全部翻转

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

2. 时间复杂度

   $O(1)$ 。

#### 29.将某元素某位翻转

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

   输入参数 `uint8_t i` ，表示要设置的位。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   若 `i>=length()` ，设置无效。

#### 30.将某元素某一范围翻转

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

   输入参数 `uint8_t l` ，表示要设置的范围的起始下标。

   输入参数 `uint8_t r` ，表示要设置的范围的结尾下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（`l<=r`）

#### 31.将某元素结尾的连续的 0 替换为 1

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

2. 时间复杂度

   $O(1)$ 。

#### 32.将某元素结尾的连续的 1 替换为 0

1. 数据类型

   输入参数 `_Tp &mask` ，表示要设置的元素。

2. 时间复杂度

   $O(1)$ 。

#### 33.遍历某元素中所有 1 所在的位置

1. 数据类型

   输入参数 `_Tp mask` ，表示要遍历的元素。

2. 时间复杂度

   $O(count(mask))$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

#### 34.遍历某元素中所有 1 所在的位置（从大到小）

1. 数据类型

   输入参数 `_Tp mask` ，表示要遍历的元素。

2. 时间复杂度

   $O(count(mask))$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

#### 35.遍历 n 个二进制状态的所有子状态

1. 数据类型

   输入参数 `uint8_t n` ，表示独立的二进制状态数。

2. 时间复杂度

   $O(2^n)$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `n<length()` 。

#### 36.遍历 n 个二进制状态的所有子状态（从大到小）

1. 数据类型

   输入参数 `uint8_t n` ，表示独立的二进制状态数。

2. 时间复杂度

   $O(2^n)$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `n<length()` 。

#### 37.遍历 n 个二进制状态的所有含有 k 个 1 的子状态

1. 数据类型

   输入参数 `uint8_t n` ，表示独立的二进制状态数。

   输入参数 `uint8_t k` ，表示要保留的 `1` 的数量。

2. 时间复杂度

   $O(C_n^k)$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `n<length()`  且 `k<=n` 。

#### 38.遍历 n 个二进制状态的所有含有 k 个 1 的子状态（从大到小）

1. 数据类型

   输入参数 `uint8_t n` ，表示独立的二进制状态数。

   输入参数 `uint8_t k` ，表示要保留的 `1` 的数量。

2. 时间复杂度

   $O(C_n^k)$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `n<length()`  且 `k<=n` 。

#### 39.遍历某个元素的所有子状态

1. 数据类型

   输入参数 `_Tp mask` ，表示要遍历的元素。

2. 时间复杂度

   $O(2^{count(mask)})$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `mask!=-1` 。

   本函数效率较低，一般枚举子集不采取本函数的迭代方式。

#### 40.遍历某个元素的所有子状态（从大到小）

1. 数据类型

   输入参数 `_Tp mask` ，表示要遍历的元素。

2. 时间复杂度

   $O(2^{count(mask)})$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `mask!=-1` 。

   本函数效率较高，一般枚举子集采取本函数的迭代方式。

#### 41.遍历某个元素的所有含有 k 个 1 的子状态

1. 数据类型

   输入参数 `_Tp mask` ，表示要遍历的元素。

   输入参数 `uint8_t k` ，表示要保留的 `1` 的数量。

2. 时间复杂度

   $O(C_{count(mask)}^k)$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `mask!=-1` 且 `k<=n` 。

#### 42.遍历某个元素的所有含有 k 个 1 的子状态（从大到小）

1. 数据类型

   输入参数 `_Tp mask` ，表示要遍历的元素。

   输入参数 `uint8_t k` ，表示要保留的 `1` 的数量。

2. 时间复杂度

   $O(C_{count(mask)}^k)$ 。

3. 备注

   本函数返回一个容器，该容器可以用 `for` 范围遍历。

   本函数要求 `mask!=-1` 且 `k<=n` 。

#### 43.查询二进制状态字符串

1. 数据类型

   模板参数 `uint8_t k` ，表示要查询多少位。当 `k==0` 时，表示查询全部位。

2. 时间复杂度

   $O(length())$ 。

### 三、模板示例

```c++
#include "MATH/BitwiseHelper.h"
#include "IO/FastIO.h"

int main() {
    //一般对 int 的位运算用得较多。所以使用 int 类型
    cout << "int length: " << OY::BitwiseHelper<int>::length() << endl;
    cout << "one's number in 49:" << OY::BitwiseHelper<int>::countOne(49) << endl;

    //输出 49 的二进制表示
    cout << "49 is like:" << OY::BitwiseHelper<int>::to_string<10>(49) << endl;
    cout << "49's position-0 is one?" << (OY::BitwiseHelper<int>::isOne(49, 0) ? "Yes\n" : "No\n");
    cout << "49's position-1 is one?" << (OY::BitwiseHelper<int>::isOne(49, 1) ? "Yes\n" : "No\n");
    cout << "49's position-3 is zero?" << (OY::BitwiseHelper<int>::isZero(49, 3) ? "Yes\n" : "No\n");
    cout << "49's position-4 is zero?" << (OY::BitwiseHelper<int>::isZero(49, 4) ? "Yes\n" : "No\n");
    cout << "14 is like:" << OY::BitwiseHelper<int>::to_string<10>(14) << endl;
    cout << "49 has common ones with 14?" << (OY::BitwiseHelper<int>::intersect(49, 14) ? "Yes\n" : "No\n");
    cout << "16 is like:" << OY::BitwiseHelper<int>::to_string<10>(16) << endl;
    cout << "49 has common ones with 16?" << (OY::BitwiseHelper<int>::intersect(49, 16) ? "Yes\n" : "No\n");
    cout << "49 contains all ones of 16?" << (OY::BitwiseHelper<int>::contains(49, 16) ? "Yes\n" : "No\n");
    cout << "all ones of 16 is contained in 49?" << (OY::BitwiseHelper<int>::isContained(16, 49) ? "Yes\n" : "No\n");
    cout << "number of front zeros in 49:" << OY::BitwiseHelper<int>::countFrontZeros(49) << endl;
    cout << "number of front ones in 49:" << OY::BitwiseHelper<int>::countFrontOnes(49) << endl;
    cout << "number of back zeros in 49:" << OY::BitwiseHelper<int>::countBackZeros(49) << endl;
    cout << "number of back ones in 49:" << OY::BitwiseHelper<int>::countBackOnes(49) << endl;

    cout << "make mask of all ones:" << OY::BitwiseHelper<int>::makeMask() << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::makeMask()) << ")" << endl;
    cout << "make mask of one in position-3:" << OY::BitwiseHelper<int>::makeMask(3) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::makeMask(3)) << ")" << endl;
    cout << "make mask of ones from position-0 to position-3:" << OY::BitwiseHelper<int>::makeMask(0, 3) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::makeMask(0, 3)) << ")" << endl;
    cout << "make mask of ones below position-4:" << OY::BitwiseHelper<int>::makeBackOnes(4) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::makeBackOnes(4)) << ")" << endl;
    cout << "get position-4 mask of 49:" << OY::BitwiseHelper<int>::getMask(49, 4) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::getMask(49, 4)) << ")" << endl;
    cout << "get position-4 ~ position=5 mask of 49:" << OY::BitwiseHelper<int>::getMask(49, 4, 5) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::getMask(49, 4, 5)) << ")" << endl;
    cout << "get lowest one of 49:" << OY::BitwiseHelper<int>::getLowestOne(49) << endl;
    cout << "get smallest 2's pow that >= 49:" << OY::BitwiseHelper<int>::getCeil(49) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::getCeil(49)) << ")" << endl;
    cout << "get biggest 2's pow that <= 49:" << OY::BitwiseHelper<int>::getFloor(49) << "(" << OY::BitwiseHelper<int>::to_string(OY::BitwiseHelper<int>::getFloor(49)) << ")" << endl;
    cout << "get back ones of 49:" << OY::BitwiseHelper<int>::getBackOnes(49) << endl;

    int cur = 0;
    OY::BitwiseHelper<int>::setMask(cur);
    cout << "after setmask, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 0;
    OY::BitwiseHelper<int>::setMask(cur, 3);
    cout << "after setmask 3, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 0;
    OY::BitwiseHelper<int>::setMask(cur, 1, 3);
    cout << "after setmask 1 ~ 3, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 16383;
    cout << "16383 is like:" << OY::BitwiseHelper<int>::to_string(16383) << endl;
    OY::BitwiseHelper<int>::resetMask(cur);
    cout << "after resetmask, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 16383;
    OY::BitwiseHelper<int>::resetMask(cur, 3);
    cout << "after resetmask 3, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 16383;
    OY::BitwiseHelper<int>::resetMask(cur, 1, 3);
    cout << "after resetmask 1 ~ 3, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 26841;
    cout << "26841 is like:" << OY::BitwiseHelper<int>::to_string(26841) << endl;
    OY::BitwiseHelper<int>::flipMask(cur);
    cout << "after flipmask, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 26841;
    OY::BitwiseHelper<int>::flipMask(cur, 3);
    cout << "after flipmask 3, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 26841;
    OY::BitwiseHelper<int>::flipMask(cur, 3, 7);
    cout << "after flipmask 3 ~ 7, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    cur = 49;
    OY::BitwiseHelper<int>::fillBackOnesWithZeros(cur);
    cout << "after fill back ones with zeros, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;
    OY::BitwiseHelper<int>::fillBackZerosWithOnes(cur);
    cout << "after fill back zeros with ones, cur=" << cur << "(" << OY::BitwiseHelper<int>::to_string(cur) << ")" << endl;

    //从小到大计数 49 的一的位置
    for (auto i : OY::BitwiseHelper<int>::enumOne(49)) cout << i << ' ';
    cout << endl;
    //从大到小计数 49 的一的位置
    for (auto i : OY::BitwiseHelper<int>::enumOne_reversed(49)) cout << i << ' ';
    cout << endl;

    //从小到大枚举 5 个二进制位的子集
    for (auto i : OY::BitwiseHelper<int>::enumChoose(5)) cout << i << ' ';
    cout << endl;
    //从大到小枚举 5 个二进制位的子集
    for (auto i : OY::BitwiseHelper<int>::enumChoose_reversed(5)) cout << i << ' ';
    cout << endl;

    //从小到大枚举 5 个二进制位的含有 3 个 1 的子集
    for (auto i : OY::BitwiseHelper<int>::enumChoose(5, 3)) cout << i << '(' << OY::BitwiseHelper<int>::to_string<10>(i) << ") ";
    cout << endl;
    //从大到小枚举 5 个二进制位的含有 3 个 1 的子集
    for (auto i : OY::BitwiseHelper<int>::enumChoose_reversed(5, 3)) cout << i << '(' << OY::BitwiseHelper<int>::to_string<10>(i) << ") ";
    cout << endl;

    //从小到大枚举 49 的子集
    for (auto i : OY::BitwiseHelper<int>::enumSubOf(49)) cout << i << ' ';
    cout << endl;
    //从大到小枚举 49 的子集
    for (auto i : OY::BitwiseHelper<int>::enumSubOf_reversed(49)) cout << i << ' ';
    cout << endl;

    //从小到大枚举 49 的含有 2 个 1 的子集
    for (auto i : OY::BitwiseHelper<int>::enumSubOf(49, 2))cout << i << '(' << OY::BitwiseHelper<int>::to_string<10>(i) << ") ";
    cout << endl;
    //从大到小枚举 49 的含有 2 个 1 的子集
    for (auto i : OY::BitwiseHelper<int>::enumSubOf_reversed(49, 2)) cout << i << '(' << OY::BitwiseHelper<int>::to_string<10>(i) << ") ";
    cout << endl;
}
```

```
#输出如下
int length: 32
one's number in 49:3
49 is like:0000110001
49's position-0 is one?Yes
49's position-1 is one?No
49's position-3 is zero?Yes
49's position-4 is zero?No
14 is like:0000001110
49 has common ones with 14?No
16 is like:0000010000
49 has common ones with 16?Yes
49 contains all ones of 16?Yes
all ones of 16 is contained in 49?Yes
number of front zeros in 49:26
number of front ones in 49:0
number of back zeros in 49:0
number of back ones in 49:1
make mask of all ones:-1(11111111111111111111111111111111)
make mask of one in position-3:8(00000000000000000000000000001000)
make mask of ones from position-0 to position-3:15(00000000000000000000000000001111)
make mask of ones below position-4:15(00000000000000000000000000001111)
get position-4 mask of 49:16(00000000000000000000000000010000)
get position-4 ~ position=5 mask of 49:48(00000000000000000000000000110000)
get lowest one of 49:1
get smallest 2's pow that >= 49:64(00000000000000000000000001000000)
get biggest 2's pow that <= 49:32(00000000000000000000000000100000)
get back ones of 49:1
after setmask, cur=-1(11111111111111111111111111111111)
after setmask 3, cur=8(00000000000000000000000000001000)
after setmask 1 ~ 3, cur=14(00000000000000000000000000001110)
16383 is like:00000000000000000011111111111111
after resetmask, cur=0(00000000000000000000000000000000)
after resetmask 3, cur=16375(00000000000000000011111111110111)
after resetmask 1 ~ 3, cur=16369(00000000000000000011111111110001)
26841 is like:00000000000000000110100011011001
after flipmask, cur=-26842(11111111111111111001011100100110)
after flipmask 3, cur=26833(00000000000000000110100011010001)
after flipmask 3 ~ 7, cur=26657(00000000000000000110100000100001)
after fill back ones with zeros, cur=48(00000000000000000000000000110000)
after fill back zeros with ones, cur=63(00000000000000000000000000111111)
0 4 5 
5 4 0 
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 
7(0000000111) 11(0000001011) 13(0000001101) 14(0000001110) 19(0000010011) 21(0000010101) 22(0000010110) 25(0000011001) 26(0000011010) 28(0000011100) 
28(0000011100) 26(0000011010) 25(0000011001) 22(0000010110) 21(0000010101) 19(0000010011) 14(0000001110) 13(0000001101) 11(0000001011) 7(0000000111) 
0 1 16 17 32 33 48 49 
49 48 33 32 17 16 1 0 
17(0000010001) 33(0000100001) 48(0000110000) 
48(0000110000) 33(0000100001) 17(0000010001) 

```

