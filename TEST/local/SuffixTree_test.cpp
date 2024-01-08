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
/*
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

*/