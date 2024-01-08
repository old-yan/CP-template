### 一、模板类别

​	序列：双向后缀树。

​	练习题目：

1. [P2408 不同子串个数](https://www.luogu.com.cn/problem/P2408)
2. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)
3. [# 5362. I'm always close to you](https://qoj.ac/problem/5362)

### 二、模板功能

​	双向后缀树的构建结合了 `Blumer` 的后缀自动机构造算法和 `Ukkonen` 算法。本算法与 `SAM` 的区别是，整个结构呈现为一个多叉树的状态；与普通字典树的区别是，树上每条边不仅可以表示一个字符，也可以表示多个字符。每个结点处的 `m_ptr` 属性表示从根结点到当前结点所表示的串在原串中的起始指针； `m_length` 属性表示从根结点到当前结点所表示的串的长度。

​	后缀树的结点，可以使用数组来维护所有的孩子，也可以使用哈希表、平衡树等数据结构来维护所有的孩子。下文中，时间复杂度均以前者为准；当使用者使用后者情况的结点时，时间复杂度会有相应的变化。

#### 1.构造后缀树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   模板参数 `typename Tp` ，表示序列里的元素类型。

   模板参数 `typename ChildGetter` ，表示树中结点获取孩子结点时的类的实现方式。

   模板参数 `size_type MAX_LEN` ，表示序列最大长度。

2. 时间复杂度

   $O(n)$ ，此处 `n` 代表序列最大长度。

3. 备注

   对于字符集有限的情况，可以使用 `StaticChildGetter` 封装的结点，通过数组存储孩子结点；对于字符集较大的情况，可以自己手写使用哈希表或者平衡树存储孩子的结点。

   **注意：**

   后缀树本身就是树结构，这是后缀树与各种自动机最大的区别。

   **注意：**

   在构建后缀树的过程中，后缀树一直保持隐式后缀树的形态，即若从根到叶的路径上不产生分叉，则这条路径就只有一条树边，不产生分化，因而无法轻易分辨出这条路径上的若干个后缀子串。

   **注意：**

   因为本后缀树是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。


#### 2.预留空间(reserve)

1. 数据类型

   输入参数 `size_type length` ，表示预备维护的区间长度。

2. 时间复杂度

   $O(n\cdot\Sigma)$ ，此处 `n` 指序列长度， `Σ` 表示字符集大小。

3. 备注

   本方法强制将表清空，然后预留 `n` 个空间。

#### 3.清空(clear)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 4.查询模板内元素数量(size)

1. 数据类型

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

   $O(1)$ 。

#### 5.在尾部添加元素(push_back)

1. 数据类型

   输入参数 ` const Tp &elem` ，表示在末尾添加的元素值。

2. 时间复杂度

   均摊 $O(\Sigma)$ ，此处 `Σ` 表示字符集大小。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 6.在头部添加元素(push_front)

1. 数据类型

   输入参数 ` const Tp &elem` ，表示在头部添加的元素值。

2. 时间复杂度

   均摊 $O(\Sigma)$ ，此处 `Σ` 表示字符集大小。

3. 备注

   本方法是为了动态延伸区间而准备的。

#### 7.获取结点指针(get_node)

1. 输入类型

   输入参数 `size_type node_index` ，表示结点的索引。

   返回类型 `node *` ，表示相应的结点指针。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**

   因为本后缀树是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/BiSuffixTree.h"

#include <map>

template <typename Iterator>
void print_uint(Iterator first, Iterator last) {
    // 少输出一位，是因为最后添加的是 char(26)，并非原字符串的一部分
    for (auto it = first; it != last - 1; ++it) cout << char('a' + *it);
    cout << endl;
}

template <typename STree>
void traverse(const STree &S, const std::string &s, int cur, int level) {
    auto p = S.get_node(cur);
    if (cur) {
        cout << std::string(level, '\t') << "node[" << cur << "]: ";
        if (p->m_length == STree::inf)
            print_uint(p->m_ptr, S.m_last);
        else
            print_uint(p->m_ptr, p->m_ptr + p->m_length);
    }
    if (p->m_length == STree::inf) {
        return;
    } else
        for (int i = 0; i < 27; i++) {
            if (p->get_child(i)) {
                traverse(S, s, p->get_child(i), level + 1);
            }
        }
}
void test_traverse() {
    std::string s = "abcaabcaabeaab";
    using BiSTree = OY::StaticBiSufTree_string<uint8_t, 27, 1000>;
    BiSTree S;
    // 将字符串插入后缀树中
    int l = 7, r = 7;
    while (l > 0 || r < s.size()) {
        if (l > 0) S.push_front(s[--l] - 'a');
        if (r < s.size()) S.push_back(s[r++] - 'a');
    }
    // 最后插入一个结束字符
    S.push_back(26);

    // 遍历输出每个结点
    // 根结点为 0
    traverse(S, s, 0, 0);
}

struct ChildGetter {
    std::map<uint32_t, uint32_t> m_child;
    bool has_child(uint32_t index) const { return m_child.find(index) != m_child.end(); }
    void set_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.find(index);
        return it == m_child.end() ? 0 : it->second;
    }
    void copy_children(const ChildGetter &rhs) { m_child = rhs.m_child; }
};
template <typename STree>
void map_node_traverse(const STree &S, const std::string &s, int cur, int level) {
    auto p = S.get_node(cur);
    if (cur) {
        cout << std::string(level, '\t') << "node[" << cur << "]: ";
        if (p->m_length == STree::inf)
            print_uint(p->m_ptr, S.m_last);
        else
            print_uint(p->m_ptr, p->m_ptr + p->m_length);
    }
    if (p->m_length == STree::inf) {
        return;
    } else
        for (auto &[i, child] : p->m_child)
            map_node_traverse(S, s, child, level + 1);
}
void test_map_node_traverse() {
    std::string s = "abcaabcaabeaab";
    using BiSTree = OY::BISUFTREE::Tree<uint32_t, ChildGetter, 1000>;
    BiSTree S;
    // 将字符串插入后缀树中
    int l = 7, r = 7;
    while (l > 0 || r < s.size()) {
        if (l > 0) S.push_front(s[--l] - 'a');
        if (r < s.size()) S.push_back(s[r++] - 'a');
    }
    // 最后插入一个结束字符
    // 既然是用 map 管理孩子，那么序列里出现负数都是可行的
    S.push_back(-1);

    // 遍历输出每个结点
    // 根结点为 0
}

int main() {
    test_traverse();
    test_map_node_traverse();
}
```

```
#输出如下
	node[5]: 
		node[6]: aa
			node[2]: aabcaabeaab
			node[7]: aabeaab
			node[19]: aab
		node[8]: a
			node[18]: abcaa
				node[17]: abcaabcaabeaab
				node[4]: abcaabeaab
			node[9]: abeaab
			node[20]: ab
	node[10]: 
		node[16]: bcaa
			node[15]: bcaabcaabeaab
			node[3]: bcaabeaab
		node[11]: beaab
		node[21]: b
	node[14]: caa
		node[13]: caabcaabeaab
		node[1]: caabeaab
	node[12]: eaab
	node[22]: 

```

