### 一、模板类别

​	数据结构：单点删除并查集

​	练习题目：

1. [Junk-Mail Filter](https://acm.hdu.edu.cn/showproblem.php?pid=2473)
2. [U130175 myf与前女友](https://www.luogu.com.cn/problem/U130175)
3. [Professional Manager](https://ac.nowcoder.com/acm/problem/15696)


### 二、模板功能


​	   本数据结构为并查集的升级版，除了维护点与点的连通关系外，还可以从一个连通块内把一个点单独删除掉而不影响其他点的连通关系。

​	   本数据结构与普通并查集的一大区别是，普通并查集的 `find(i)` 返回的是 `i` 所在连通块的代表元 `head` ，且保证 `find(head) == head` ，可以说 `head` 和 `i` 是同一层级的概念， `head` 也是一个元素， `i` 也是一个元素；但是，对于本模板， `find(i)` 返回的是 `i` 所在连通块的代表元 `head` ，而 `i` 是一个元素， `head` 并不是一个元素，所以也不保证 `find(head) == head` 。只有在初始时，恰好保证 `find(i) == i` ，在经过各种操作之后， `find(i)` 甚至可能超出 `n` 的范围。

​	   本模板的操作 `extract` 会试图将 `i` 从 `i` 所在的连通块分离出来，并为其分配一个新的 `head` 。如果 `i` 本来就在一个单独的连通块，则不会分配新的 `head` ，而是返回 `i` 当前所在的连通块的 `head` 。


### 三、模板示例

```c++
#include "DS/ErasableDSU.h"
#include "IO/FastIO.h"

int main() {
    OY::EDSU::Table u(6);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    u.unite_to(0, 4);
    u.unite_to(1, 4);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    u.unite_to(4, 2);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    cout << "extract 4 to " << u.extract(4) << endl;
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    u.unite_to(2, 6);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];
}
```

```
#输出如下
0 1 2 3 4 5
4 4 2 3 4 5
2 2 2 3 2 5
extract 4 to 6
2 2 2 3 6 5
6 6 6 3 6 5

```

