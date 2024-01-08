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
/*
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

*/