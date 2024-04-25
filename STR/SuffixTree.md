### 一、模板类别

​	序列：后缀树。

​	练习题目：

1. [P2408 不同子串个数](https://www.luogu.com.cn/problem/P2408)
2. [P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
3. [P3804 【模板】后缀自动机（SAM）](https://www.luogu.com.cn/problem/P3804)
4. [P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)
5. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)
6. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)




### 二、模板功能

​	后缀树的构建采用 `Ukkonen` 算法。本算法与 `SAM` 的区别是，整个结构呈现为一个多叉树的状态；与普通字典树的区别是，树上每条边不仅可以表示一个字符，也可以表示多个字符。每个结点处的 `m_pos` 属性表示当前结点所表示的子串在原串中的起始下标； `m_length` 属性表示当前结点所表示的子串的长度。

​	后缀树的结点，可以使用数组来维护所有的孩子，也可以使用哈希表、平衡树等数据结构来维护所有的孩子。下文中，时间复杂度均以前者为准；当使用者使用后者情况的结点时，时间复杂度会有相应的变化。

#### 1.构造后缀树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示下标类型。

   模板参数 `typename ChildGetter` ，表示后缀树里的结点寻找孩子结点时的方法类。

   模板参数 `typename Sequence` ，表示后缀树里存储原序列的容器类型。

   构造参数 `size_type length` ，表示序列长度。

   构造参数 `InitMapping mapping` ，表示序列下标到元素的映射。 

2. 时间复杂度

   $O(n\cdot\Sigma)$ ，此处 `Σ` 代表字符集的大小。

3. 备注

   对于字符集有限的情况，可以使用 `StaticChildGetter` 封装的结点，通过数组存储孩子结点；对于字符集较大的情况，可以自己手写使用哈希表或者平衡树存储孩子的结点。

   **注意：**

   后缀树本身就是树结构，这是后缀树与各种自动机最大的区别。

   **注意：**

   在构建后缀树的过程中，后缀树一直保持隐式后缀树的形态，即若从根到叶的路径上不产生分叉，则这条路径就只有一条树边，不产生分化，因而无法轻易分辨出这条路径上的若干个后缀子串。

   建议在插入所有字符之后，再插入一个从未使用过的字符，以将隐式后缀树转化为后缀树，方便使用。

   **注意：**

   因为本后缀树是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。

   如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 2.构造后缀树

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

#### 9.获取结点指针(get_node)

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
#include "STR/SuffixTree.h"

#include <map>

template <typename STree>
void traverse(const STree &S, const std::string &s, int cur, int level, std::string path) {
    auto p = S.get_node(cur);
    if (p->m_pos < s.size())
        if (p->m_length == STree::inf)
            path += s.substr(p->m_pos);
        else
            path += s.substr(p->m_pos, p->m_length);
    cout << std::string(level, '\t') << "node[" << cur << "]: " << path << endl;
    if (p->m_length == STree::inf) {
        return;
    } else
        for (int i = 0; i < 27; i++) {
            if (p->get_child(i)) {
                traverse(S, s, p->get_child(i), level + 1, path);
            }
        }
}
void test_traverse() {
    std::string s = "abcaabcaabeaab";
    using STree = OY::StaticSufTree_string<27>;
    STree S;
    // 将字符串插入后缀树中
    for (char c : s) S.push_back(c - 'a');
    // 最后插入一个结束字符
    S.push_back(26);

    // 遍历输出每个结点
    // 根结点为 0
    traverse(S, s, 0, 0, "");
}

struct Node {
    std::map<uint32_t, uint32_t> m_child;
    void set_child(uint32_t index, uint32_t child) { m_child[index] = child; }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.find(index);
        return it == m_child.end() ? 0 : it->second;
    }
    void copy_children(const Node &rhs) { m_child = rhs.m_child; }
};
template <typename STree>
void map_node_traverse(const STree &S, const std::string &s, int cur, int level, std::string path) {
    auto p = S.get_node(cur);
    if (p->m_pos < s.size())
        if (p->m_length == STree::inf)
            path += s.substr(p->m_pos);
        else
            path += s.substr(p->m_pos, p->m_length);
    cout << std::string(level, '\t') << "node[" << cur << "]: " << path << endl;
    if (p->m_length == STree::inf) {
        return;
    } else
        for (auto &[i, child] : p->m_child)
            map_node_traverse(S, s, child, level + 1, path);
}
void test_map_node_traverse() {
    std::string s = "abcaabcaabeaab";
    using STree = OY::SUFTREE::Tree<Node, std::string>;
    STree S;
    // 将字符串插入后缀树中
    for (char c : s) S.push_back(c - 'a');
    // 最后插入一个结束字符
    // 既然是用 map 管理孩子，那么序列里出现负数都是可行的
    S.push_back(-1);

    // 遍历输出每个结点
    // 根结点为 0
    map_node_traverse(S, s, 0, 0, "");
}

int main() {
    test_traverse();
    test_map_node_traverse();
}
```

```
#输出如下
node[0]: 
	node[4]: a
		node[12]: aab
			node[5]: aabcaabeaab
			node[13]: aabeaab
			node[19]: aab
		node[14]: ab
			node[6]: abcaab
				node[1]: abcaabcaabeaab
				node[7]: abcaabeaab
			node[15]: abeaab
			node[20]: ab
	node[16]: b
		node[8]: bcaab
			node[2]: bcaabcaabeaab
			node[9]: bcaabeaab
		node[17]: beaab
		node[21]: b
	node[10]: caab
		node[3]: caabcaabeaab
		node[11]: caabeaab
	node[18]: eaab
	node[22]: 
node[0]: 
	node[4]: a
		node[12]: aab
			node[5]: aabcaabeaab
			node[13]: aabeaab
			node[19]: aab
		node[14]: ab
			node[6]: abcaab
				node[1]: abcaabcaabeaab
				node[7]: abcaabeaab
			node[15]: abeaab
			node[20]: ab
	node[16]: b
		node[8]: bcaab
			node[2]: bcaabcaabeaab
			node[9]: bcaabeaab
		node[17]: beaab
		node[21]: b
	node[10]: caab
		node[3]: caabcaabeaab
		node[11]: caabeaab
	node[18]: eaab
	node[22]: 

```

