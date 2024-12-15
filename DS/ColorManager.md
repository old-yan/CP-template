### 一、模板类别

​	数据结构：颜色管理。

​	练习题目：

1. [P3201 [HNOI2009] 梦幻布丁](https://www.luogu.com.cn/problem/P3201)


### 二、模板功能

​	   本数据结构用于解决以下问题：初始有若干个物体，颜色各不相同。每次操作将某种颜色的物体涂成另一种颜色，在线查询某个物块的颜色，以及某个颜色的物块数量。

​	    `query` 方法输入初始颜色，查询当前颜色；

​	    `query_count` 方法输入当前颜色，查询此事为这个颜色的物块数量；

### 三、模板示例

```c++
#include "DS/ColorManager.h"
#include "IO/FastIO.h"

int main() {
    OY::CM::Table cm(10);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cm.modify(1, 6);
    cm.modify(2, 6);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cm.modify(6, 2);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cm.modify(3, 6);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cout << "item count of color2: " << cm.query_count(2) << endl;
}
```

```
#输出如下
0 1 2 3 4 5 6 7 8 9
0 6 6 3 4 5 6 7 8 9
0 2 2 3 4 5 2 7 8 9
0 2 2 6 4 5 2 7 8 9
item count of color2: 3

```

