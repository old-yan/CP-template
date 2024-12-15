### 一、模板类别

​	数据结构：珂朵莉 `zkw` 线段树。

​	练习题目：

1. [218. 天际线问题](https://leetcode.cn/problems/the-skyline-problem/)
2. [P1503 鬼子进村](https://www.luogu.com.cn/problem/P1503)
3. [P1840 Color the Axis](https://www.luogu.com.cn/problem/P1840)
4. [P2434 [SDOI2005] 区间](https://www.luogu.com.cn/problem/P2434)
5. [P3740 [HAOI2014] 贴海报](https://www.luogu.com.cn/problem/P3740)
6. [P4979 矿洞：坍塌](https://www.luogu.com.cn/problem/P4979)
7. [T526317 帕鲁大陆染色的一天](https://www.luogu.com.cn/problem/T526317)



### 二、模板功能

​		本模板为珂朵莉树的 `zkw` 线段树版本。底层为 `AssignZkwTree` ，以实现区间推平。

​		如果只想实现区间推平操作，可以直接使用 `ChthollyZkwTree` 。

​		如果想实现自定义的区间信息查询，需要给模板传递一个半群。由于编写半群比较抽象，所以我给半群分成三种类型，每种半群只需要实现一个 `segment` 类即可。该 `segment` 类型需满足：

1. 定义类型 `value_type`，作为单点的值类型；

2. 定义无参构造函数，还有从 `value_type` 转换而来的构造函数；

3. 定义加法运算，结果为两个相邻 `segment` 信息聚合的结果；

4. 定义成员函数 `has_change` 。定义一个 `segment` 为同值区间，当且仅当该 `segment` 所表示的区间内的每个点的值都相等。同值区间的 `has_change` 返回 `false` ，非同值区间的 `has_change` 返回 `true`。

5. 定义成员函数 `get` ，表示若本 `segment` 的 `has_change` 返回 `false` ，返回任一点的值。

   ​	除了这些要求外，不同类型的 `segment` 半群还需要满足一些额外要求：

1.   `ChthollyValLazyZkwTree` ，需要传递一个 `segment` 类型，这个类型的 `segment` 加法运算要求满足，如果两个同值区间的点值相等，则相加结果仍为其本身。

2.   `ChthollyFastPowZkwTree` ，需要传递一个 `segment` 类型，这个类型的 `segment` 定义了乘法，一个长度为 `1` 的 `segment` 乘以 `n` 后可以快速得到长度为 `n` 的同值区间。

3.   `ChthollyLazyZkwTree` ，需要传递一个 `segment` 类型，无特殊要求。

### 三、模板示例

```c++
#include "DS/ChthollyZkwTree.h"
#include "IO/FastIO.h"

int main() {
    // 不妨以珂朵莉树的起源， cf896c 为例
    OY::ChthollyZkwTree<int> S(10);
    for (int i = 0; i < 10; i++) S.modify(i, 100 * i);
    cout << S << endl;

    // 操作一，区间覆盖
    S.modify(1, 3, 200);
    S.modify(6, 8, 900);
    S.modify(3, 6, 10);
    cout << S << endl;

    // 操作二，区间加
    auto range_add = [&](int l, int r) {
        // 首先把区间找出来
        std::vector<std::pair<std::pair<int, int>, int>> rs;
        S.enumerate(l, r, [&](int L, int R, int val) { rs.emplace_back(std::make_pair(L, R), val); });
        // 最后统一进行区间加
        for (int i = 0; i < rs.size(); i++) {
            int L = rs[i].first.first;
            int R = rs[i].first.second;
            int val = rs[i].second;
            S.modify(L, R, val + 1);
        }
    };
    range_add(1, 8);
    cout << S << endl;

    // 操作三，区间排序后的第 k 个元素
    auto range_kth = [&](int l, int r, int k) {
        // 首先把区间找出来
        std::vector<std::pair<int, std::pair<int, int>>> rs;
        S.enumerate(l, r, [&](int L, int R, int val) { rs.emplace_back(val, std::make_pair(L, R)); });
        // 排序后找出第 k 个
        std::sort(rs.begin(), rs.end());
        int cnt = 0;
        for (int i = 0; i < rs.size(); i++) {
            int val = rs[i].first;
            int L = rs[i].second.first;
            int R = rs[i].second.second;
            if (cnt + R - L + 1 >= k) return val;
            cnt += R - L + 1;
        }
        return -1;
    };
    cout << "smallest number in [1,8] = " << range_kth(1, 8, 1) << endl;
    cout << "biggest number in [1,8] = " << range_kth(1, 8, 8) << endl;

    // 操作四和操作三同理，不过不需要存到 vector 里排序，只需要在 enumerate 的时候计算就行
}
```

```
#输出如下
[0, 100, 200, 300, 400, 500, 600, 700, 800, 900]
[0, 200, 200, 10, 10, 10, 10, 900, 900, 900]
[0, 201, 201, 11, 11, 11, 11, 901, 901, 900]
smallest number in [1,8] = 11
biggest number in [1,8] = 901

```

