### 一、模板类别

​	数学：质数筛。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `uint32_t _N` ，表示查找质数的范围。

2. 时间复杂度

   $O(n\cdot \log \log n)$ 。

3. 备注

   本模板用于从小到大找出一定范围里的所有质数。
   
   欧拉筛的时间复杂度为线性，但是实际运行速度并不如埃氏筛。本筛是在埃氏筛的基础上优化而来。
   
   **注意：** 本模板运行的时候，依赖于打表生成的小范围奇质数表。把 `[0, 10000]` 范围内的奇质数用其他判质方法计算出来，然后复制到 `SievePairs` 类里，才能进行后续的计算。而这个**小范围**至少要达到模板参数 `_N` 的根号大小。所以如果筛的范围是 `100000000` 及以下，那么 `10000` 的小范围是足够的；但是如果要筛 `100000000`  以上的质数，需要手动修改 `SievePairs` 的内容。比如，要计算 `400000000` 范围内的质数，请在本地计算出 `20000` 以内的奇质数，发现有 `2261` 个。将 `SievePairs` 修改如下：
   
   ```
   	static constexpr int smallBound = 20000;
       static constexpr int smallPrimeCount =2261;
       static constexpr int smallPrimes[smallPrimeCount]{3,5,7,...,19993,19997};
   ```

#### 2.将所有数的判质结果转到 bitset

1. 数据类型

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本方法是为了方便查询某数是否为质数。

#### 3.查询从小到大的第 k 个质数

1. 数据类型

   输入参数 `int __k` ，表示要查询的次序。

2. 时间复杂度

   $O(1)$ 。


#### 4.查询质数总数

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


### 三、模板示例

```c++
#include "MATH/PrimeSieve.h"
#include "IO/FastIO.h"

int main(){
    //筛法找质数
    OY::PrimeSieve<100000000>ps;
    cout<<"number of primes in [0,100000000]: "<<ps.count()<<endl;
    cout<<"No.0 prime is: "<<ps.queryKthPrime(0)<<endl;
    cout<<"No.1 prime is: "<<ps.queryKthPrime(1)<<endl;
    cout<<"No.2 prime is: "<<ps.queryKthPrime(2)<<endl;
    cout<<"No.3 prime is: "<<ps.queryKthPrime(3)<<endl;
    cout<<"No.4 prime is: "<<ps.queryKthPrime(4)<<endl;
    cout<<"No.5000000 prime is: "<<ps.queryKthPrime(5000000)<<endl;

    //但是这样很难查询某个数字是否为质数
    //可以考虑把所有数字是否为质数的信息转到 bitset 里
    auto B=ps.to_bitset();
    cout<<"1234567 is prime?"<<(B[1234567]?"yes":"no")<<endl;
    cout<<"123457 is prime?"<<(B[123457]?"yes":"no")<<endl;
}
```

```
#输出如下
number of primes in [0,100000000]: 5761455
No.0 prime is: 2
No.1 prime is: 3
No.2 prime is: 5
No.3 prime is: 7
No.4 prime is: 11
No.5000000 prime is: 86028157
1234567 is prime?no
1234569 is prime?no
123457 is prime?yes

```

