### 一、模板类别

​	数据结构：`01` 树状数组

​	练习题目：

1. [Hotaru's problem](https://acm.hdu.edu.cn/showproblem.php?pid=5371)
2. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)

### 二、模板功能


​		本模板为 `BIT` 在单点的元素值只有 `0` 和 `1` 两种情况的特化。

​		本模板支持单点修改，区间查询。

​		本模板的速度不如 `IntegerSet` ，优点在于长度可以运行期确定；而且支持 `kth` 操作。

#### 三、模板示例

```c++
#include "DS/BIT01.h"
#include "IO/FastIO.h"

int main() {
    OY::BIT01::Tree B(1000);
    // 全部置为 1
    for (int i = 0; i < B.size(); i++) B.set(i);
    cout << "B.query_all() = " << B.query_all() << endl;
    for (int i = 139; i < 405; i += 50)
        cout << "index " << i << ":" << B.query(i) << endl;
    // 全部置为 0
    for (int i = 0; i < B.size(); i++) B.reset(i);
    cout << "B.query_all() = " << B.query_all() << endl;
    for (int i = 139; i < 405; i += 50)
        cout << "index " << i << ":" << B.query(i) << endl;

    B.set(16);
    B.set(980);
    B.set(357);
    B.set(2);
    for (int i = B.lower_bound(0); i >= 0; i = B.lower_bound(i + 1))
        cout << i << ' ';
    cout << endl;
    for (int i = B.smaller_bound(B.size()); i >= 0; i = B.smaller_bound(i))
        cout << i << ' ';
    cout << endl;
}
```

```
#输出如下
B.query_all() = 1000
index 139:1
index 189:1
index 239:1
index 289:1
index 339:1
index 389:1
B.query_all() = 0
index 139:0
index 189:0
index 239:0
index 289:0
index 339:0
index 389:0
2 16 357 980 
980 357 16 2 

```

