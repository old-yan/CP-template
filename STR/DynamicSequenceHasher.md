### 一、模板类别

​	序列：序列哈希助手（动态模数）。

### 二、模板功能

#### 1.构造哈希助手

1. 数据类型

   模板参数 `typename _ModType` ，可以为 `uint32_t` 或者 `uint64_t` ，表示模数的类型。

   构造参数 `_ModType __base` ，表示把序列视为一个 `__base` 进制的数字。

   构造参数 `_ModType __P` ，表示序列视为的数字要对 `__P` 取模。

   构造参数 `uint32_t __maxLength` ，表示序列的最大长度。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指 `__maxLength` 。

3. 备注

   本模板要求 `__base` 大于序列中的所有值。比如对于字符串，`__base` 至少为 `128` 。但是，如果将一个仅含有小写字母的字符串映射到 `[0, 26)` ，`__base` 取 `26` 也是可以的。

   本模板要求 `__P` 为质数，且 `__P` 尽量大，以减少哈希值冲突。假设要比较的哈希值有 `n` 个，那么出现意外冲突的概率为 $1-e^{-\frac{n\times(n-1)}{2\times \_\_P}}$ ，取 `__P=1000000007`，取 `n=100000`，则概率为 `99.3%`；
   
   取 `n=10000`，则概率为 `4.9%` 。所以在随机数据情况下，最好让 `n<sqrt(__P)` 。
   
   本模板支持 `64` 位模数，但是速度比起 `32` 位模数会慢一倍。

#### 2.对序列进行哈希

1. 数据类型

   输入参数 `_Iterator __first` ，表示序列区间开始位置。

   输入参数 `_Iterator __last` ，表示序列区间结束位置。（开区间）

   输入参数 `_Mapping __map` ，表示要对序列里的元素进行的映射。默认情况下为自身映射，即将字符直接转为相应的 `ASCII` 码值。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指序列长度。

3. 备注

   在有特殊需求时，可以给 `__map` 通过匿名函数传递映射规则。比如，如果想对一个全是小写字母的字符串进行哈希，且想让每个元素的值映射到 `[0, 26)` 范围内，可以传递 `__map=[](char c){return c-'a';}` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/DynamicSequenceHasher.h"

int main() {
    //为了方便查看效果，先用 10 作为乘数
    // 10为进位时的乘数，1007为模数，100表示字符串最大长度
    OY::DynamicSequenceHasher32 ssh(10, 1007, 100);
    char c[] = "31415926";
    auto hash = ssh.hash(c, c + 8, [](char c) { return c - '0'; });
    // 31415926 % 1007 = 547
    cout << "c[0~7] 's hashcode = " << hash.query(0, 7) << endl;
    //从这里也能看出代码实现的思路：类似于前缀和差分，截取区间值
    cout << "c[0~2] 's hashcode = " << hash.query(0, 2) << endl;
    cout << "c[0~1] 's hashcode = " << hash.query(0, 1) << endl;
    cout << "c[2~2] 's hashcode = " << hash.query(2, 2) << endl;
    cout << "c[3~5] 's hashcode = " << hash.query(3, 5) << endl;
    cout << "c[6~7] 's hashcode = " << hash.query(6, 7) << endl;

    //一般情况下尽量用较大的质数做模数，避免哈希值撞车
    OY::DynamicSequenceHasher32 ssh2(100003, 1000000007, 10000);
    //默认情况下字符采用自身的值参与哈希计算
    auto hash2 = ssh2.hash(c, c + 8);
    cout << "c[0~7] 's hashcode = " << hash2.query(0, 7) << endl;

    //理论上你可以对数组进行哈希
    int A[] = {11, 22, 33, 44};
    int B[] = {22, 33, 44, 55};
    auto hash3 = ssh2.hash(A, A + 4);
    auto hash4 = ssh2.hash(B, B + 4);
    cout << hash3.querySuffix(1) << " == " << hash4.queryPrefix(2) << endl;
}
```

```
#输出如下
c[0~7] 's hashcode = 547
c[0~2] 's hashcode = 314
c[0~1] 's hashcode = 31
c[2~2] 's hashcode = 4
c[3~5] 's hashcode = 159
c[6~7] 's hashcode = 26
c[0~7] 's hashcode = 760030927
16498801 == 16498801

```

