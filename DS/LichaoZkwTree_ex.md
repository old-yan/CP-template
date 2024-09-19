### 一、模板类别

​	数据结构：李超线段树加强版

​	练习题目：

1. [P4069 [SDOI2016] 游戏](https://www.luogu.com.cn/problem/P4069)

### 二、模板功能

​		本数据结构为 `LichaoZkwTree` 的进阶版，可以进行区间加修改操作，可以进行区间最值查询。

​		本数据结构支持更强的功能，但是相应的，需要更紧的约束：

​		模板参数 `typename Tp` 为值类型，要求必须为有符号数字类型；

​		模板参数 `Tp BackGroundValue` 为树中每个点的背景值；

​		模板参数 `bool MaintainMax` 表示是维护区间最大值还是维护区间最小值；

​		模板参数 `bool RangeAdd` 表示是否需要进行区间加操作；

​		模板参数 `typename Line` 表示线段类型，默认为 `BaseLine<Tp>` 类型；

​		模板参数 `typename BackGroundJudger` 表示一个仿函数类型，默认为 `BaseJudger<Tp, BackGroundValue>` 类型。

​		本数据结构侧重维护值信息，而不是线段信息。 `add_line` 操作表示对相应位置的点执行 `a[i] = max(a[i], k * i + b)` 操作； `add_value` 操作表示对相应位置的点执行 `a[i] = a[i] + inc` 操作 ； `query` 不会返回线段信息，只会返回区间最值。

**注意：**

		1. 在进行区间加操作时，本数据结构会把背景值所在的水平线也进行区间加。所以背景值最好和后续加入的线段的值拉开差距，避免混淆；
  		2. 本模板无法重载比较函数，默认的比较函数就是 `Tp` 自身的小于号。通过 `MaintainMax` 参数，控制维护最大值还是维护最小值；
  		3. 模板参数 `typename Line` 要求满足区间内单调；提供静态函数 `make_constant_line` 生成常数线段；提供 `add` 方法以对线段进行整体加；提供 `calc` 方法求单点值；提供 `is_ascending` 方法判断是否单调增；提供 `is_horizontal` 方法判断是否水平。可以参考 `BaseLine` 的实现；
  		4. 模板参数 `BackGroundJudger` 是一个仿函数，用于判断某条线段是否为背景线段。如前文所说，背景线段也会被区间加操作影响，所以背景线段最好和普通插入线段保持较大距离，以便判别；默认的判别函数为 `BaseJudger` ，当检测到某条线段为水平，且绝对值达到背景值的一半时，将此线段判定为背景线段。

### 三、模板示例

```c++
#include "DS/LichaoZkwTree_ex.h"
#include "IO/FastIO.h"

int main() {
    // 仅展示与普通版不同之处
    constexpr int inf = 1000000000;
    OY::LichaoSlopeChmaxZkwTree_ex<int, -inf, true> S(50);
    // 在 [10, 20] 插入 y = x + 3
    S.add_line(10, 20, {1, 3});

    // 在 [15, 25] 整体增加 1000
    S.add_value(15, 25, 1000);

    // 在 2<=x<=3 时，最大值为 -inf 
    cout << "max([2~3]) = " << S.query(2, 3) << endl;
    
    // 在 10<=x<=14 时，最大值为 17
    cout << "max([10~14]) = " << S.query(10, 14) << endl;
    
    // 在 15<=x<=25 时，最大值为 1023
    cout << "max([15~25]) = " << S.query(15, 25) << endl;
    
    // 在 21<=x<=22 时，最大值为 -inf+1000
    cout << "max([21~22]) = " << S.query(21, 22) << endl;
}
```

```
#输出如下
max([2~3]) = -1000000000
max([10~14]) = 17
max([15~25]) = 1023
max([21~22]) = -999999000

```

