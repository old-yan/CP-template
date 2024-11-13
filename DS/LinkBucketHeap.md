### 一、模板类别

​	数据结构：链桶堆。

​	练习题目：

1. [Deleting Edges](https://acm.hdu.edu.cn/showproblem.php?pid=6026)
2. [1368. 使网格图至少有一条有效路径的最小代价](https://leetcode.cn/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/)
3. [2290. 到达角落需要移除障碍物的最小数目](https://leetcode.cn/problems/minimum-obstacle-removal-to-reach-corner/)
4. [P1608 路径统计](https://www.luogu.com.cn/problem/P1608)
5. [P2984 [USACO10FEB] Chocolate Giving S](https://www.luogu.com.cn/problem/P2984)
6. [P3371 【模板】单源最短路径（弱化版）](https://www.luogu.com.cn/problem/P3371)
7. [P4667 [BalticOI 2011 Day1] Switch the Lamp On 电路维修](https://www.luogu.com.cn/problem/P4667)
8. [P4943 密室](https://www.luogu.com.cn/problem/P4943)
9. [矩阵](https://ac.nowcoder.com/acm/problem/253341)



### 二、模板功能

​		本数据结构为链桶实现的模拟堆。本模板主要为配合 `Dijkstra` 模板使用。

​		本模板只支持数字作为键值，仅支持小根堆类型。
   
### 三、模板示例

```c++
#include "DS/LinkBucketHeap.h"
#include "IO/FastIO.h"

int main() {
    int val[8]{};
    auto map = [&](int i) { return val[i]; };
    OY::LinkBucketHeap<100, decltype(map)> q(8, map);
    val[3] = 60;
    q.push(3);
    val[5] = 20;
    q.push(5);
    val[6] = 70;
    q.push(6);
    cout << "top = " << q.top() << endl;
    q.pop();
    cout << "size = " << q.size() << endl;
    cout << "top = " << q.top() << endl;
    val[7] = 130;
    q.push(7);
    cout << "size = " << q.size() << endl;
    cout << "top = " << q.top() << endl;
}
```

```
#输出如下
top = 5
size = 2
top = 3
size = 3
top = 3

```

