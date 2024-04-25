### 一、模板类别

​	序列：`SAM` （后缀自动机）。

​	练习题目：

1. [P2408 不同子串个数](https://www.luogu.com.cn/problem/P2408)
2. [P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
3. [P3804 【模板】后缀自动机（SAM）](https://www.luogu.com.cn/problem/P3804)
4. [P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)
5. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)

### 二、模板功能

​	后缀自动机的结点，可以使用数组来维护所有的孩子，也可以使用哈希表、平衡树等数据结构来维护所有的孩子。下文中，时间复杂度均以前者为准；当使用者使用后者情况的结点时，时间复杂度会有相应的变化。

#### 1.构造后缀自动机

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   模板参数 `typename ChildGetter` ，表示自动机里的结点寻找孩子结点时的方法类。

   构造参数 `size_type length` ，表示序列长度。

   构造参数 `InitMapping mapping` ，表示序列下标到元素的映射。 

2. 时间复杂度

    $O(n\cdot\Sigma)$ ，此处 `Σ` 代表字符集的大小。
   
3. 备注

   对于字符集有限的情况，可以使用 `StaticChildGetter` 封装的结点，通过数组存储孩子结点；对于字符集较大的情况，可以自己手写使用哈希表或者平衡树存储孩子的结点。
   
   如果从每个结点向其孩子结点连有向边，则会形成一个有向无环图，唯一的起点为结点 `0`。
   
   如果从每个结点向其失配结点连无向边，则会形成一棵树，树的根为结点 `0` 。
   
   **注意：**
   
   因为本后缀自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。
   
   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 2.构造后缀自动机

1. 数据类型

   构造参数 `Iterator first` ，表示序列区间开始位置。

   构造参数 `Iterator last` ，表示序列区间结束位置。（开区间）

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示序列长度。

   输入参数 `InitMapping mapping` ，表示序列下标到元素的映射。

2. 时间复杂度

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示序列区间开始位置。

   输入参数 `Iterator last` ，表示序列区间结束位置。（开区间）

2. 时间复杂度

   同上。


#### 5.预留空间(reserve)

1. 数据类型

   输入参数 `size_type length` ，表示预备维护的区间长度。

2. 时间复杂度

   $O(n\cdot\Sigma)$ ，此处 `n` 指序列长度， `Σ` 表示字符集大小。

3. 备注

   本方法强制将表清空，然后预留 `n` 个空间。

#### 6.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 7.查询模板内元素数量(size)

1. 数据类型

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

   $O(1)$ 。

#### 8.在尾部添加元素(push_back)

1. 数据类型

   输入参数 ` const ValueType &elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(\Sigma)$ ，此处 `Σ` 表示字符集大小。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 9.查询序列元素对应的后缀自动机结点(query_node_index)

1. 数据类型

   输入参数 `size_type i` ，表示序列的下标。

   返回类型 `size_type` ，表示序列相应元素对应的后缀自动机结点索引。

2. 时间复杂度

   $O(1)$ 。

#### 10.获取失配后的索引(query_fail)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `size_type` ，表示发生失配后的索引。

2. 时间复杂度

   $O(1)$ 。

#### 11.获取结点指针(get_node)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `node *` ，表示相应的结点指针。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**

   因为本后缀自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 12.获取失配后的结点指针(get_fail_node)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `node *` ，表示失配后的的结点指针。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**

   因为本后缀自动机是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生

#### 13.按照扩展顺序对各个结点调用回调函数(do_for_extending_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 表示后缀自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是逆着失配指针的方向对各个结点调用回调函数。

   针对每个结点和其孩子结点，本方法保证先对父结点调用回调函数，后对孩子结点调用回调函数。

   针对每个结点和其失配结点，本方法保证先对失配结点调用回调函数，后对当前结点调用回调函数。

#### 14.按照失配顺序对各个结点调用回调函数(do_for_failing_nodes)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 表示后缀自动机内的总结点数。

3. 备注

   失配指针，是较长序列对应的结点指向较短序列对应的结点。所以本方法就是顺着失配指针的方向对各个结点调用回调函数。

   针对每个结点和其孩子结点，本方法保证先对孩子结点调用回调函数，后对父结点调用回调函数。

   针对每个结点和其失配结点，本方法保证先对当前结点调用回调函数，后对失配结点调用回调函数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/SAM.h"

#include <map>

void test_substr_cnt() {
    using SAM = OY::StaticSAM_string<26>;
    std::string s = "abbababcababbcca";
    SAM sam;
    std::vector<int> cnt(s.size() * 2);
    for (char c : s) {
        sam.push_back(c - 'a');
        cnt[sam.query_node_index(sam.size() - 1)] = 1;
    }
    sam.prepare();

    // 在 fail 树上求树上前缀和
    sam.do_for_failing_nodes([&](uint32_t a) {
        uint32_t p = sam.query_fail(a);
        if (~p) cnt[p] += cnt[a];
    });

    // 查询某个子串出现次数
    auto query_cnt = [&](std::string &&s) {
        uint32_t a = 0;
        for (char c : s) a = sam.get_node(a)->get_child(c - 'a');
        return cnt[a];
    };

    cout << "ab appeared " << query_cnt("ab") << " times\n";
    cout << "abb appeared " << query_cnt("abb") << " times\n";
    cout << "ca appeared " << query_cnt("ca") << " times\n";
    cout << endl;
}

// 此处，在定义 MapNode 的时候，把 cnt 也放到了成员变量里，外面就不需要额外的 cnt 数组了
struct MapNode {
    std::map<uint32_t, uint32_t> m_child;
    uint32_t m_cnt;
    void set_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.find(index);
        return it == m_child.end() ? 0 : it->second;
    }
    void copy_children(const MapNode &rhs) { m_child = rhs.m_child; }
};
void test_map_node() {
    using SAM = OY::SAM::Automaton<MapNode>;
    std::vector<int> s{9999, 1437, 9999, 1437, 1437, 9999, 1437, 9999};
    SAM sam;
    for (int c : s) {
        sam.push_back(c);
        sam.get_node(sam.query_node_index(sam.size() - 1))->m_cnt = 1;
    }
    sam.prepare();

    // 在 fail 树上求树上前缀和
    sam.do_for_failing_nodes([&](uint32_t a) {
        uint32_t p = sam.query_fail(a);
        if (~p) sam.get_node(p)->m_cnt += sam.get_node(a)->m_cnt;
    });

    // 查询某个子串出现次数
    auto query_cnt = [&](std::vector<int> &&s) {
        uint32_t a = 0;
        for (int c : s) a = sam.get_node(a)->get_child(c);
        return sam.get_node(a)->m_cnt;
    };

    cout << "[1437, 9999] appeared " << query_cnt({1437, 9999}) << " times\n";
    cout << "[9999] appeared " << query_cnt({9999}) << " times\n";
    cout << endl;
}

int main() {
    test_substr_cnt();
    test_map_node();
}
```

```
#输出如下
ab appeared 5 times
abb appeared 2 times
ca appeared 2 times

[1437, 9999] appeared 3 times
[9999] appeared 4 times

```

