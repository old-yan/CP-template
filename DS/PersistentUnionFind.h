/*
最后修改:
20230827
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTUNIONFIND__
#define __OY_PERSISTENTUNIONFIND__

#include <vector>

#include "PersistentSegTree.h"

namespace OY {
    namespace PerUF {
        using size_type = PerSeg::index_type;
        struct Info {
            size_type m_val, m_size;
            template <typename Ostream>
            friend Ostream &operator<<(Ostream &out, const Info &x) { return out << '(' << x.m_val << ',' << x.m_size << ')'; }
        };
        struct Node {
            using value_type = Info;
            using modify_type = Info;
            void pushup(Node *lchild, Node *rchild) {}
            void set(const Info &info) { *(Info *)this = info; }
            void set(size_type i) { *(Info *)this = Info{i, 1}; }
            Info get() const { return *(const Info *)this; }
        };
        template <size_type MAX_NODE = 1 << 22>
        struct Table {
            PerSeg::Tree<Node, PerSeg::NoInit, true, true, size_type, MAX_NODE> m_tree;
            size_type m_group_count;
            using node = typename PerSeg::Tree<Node, PerSeg::NoInit, true, true, size_type, MAX_NODE>::node;
            Table(size_type length = 0) { resize(length); }
            Table<MAX_NODE> copy() const {
                Table<MAX_NODE> other;
                other.m_tree = m_tree.copy(), other.m_group_count = m_group_count;
                return other;
            }
            void resize(size_type length) {
                if (m_group_count = length) m_tree.resize(length, [&](size_type i) { return i; });
            }
            Info find(size_type i) {
                Info res = m_tree.query(i);
                while (res.m_val != i) i = res.m_val, res = m_tree.query(i);
                return res;
            }
            size_type size(size_type i) { return find(i).m_size; }
            void unite_to(const Info &head_a, const Info &head_b) {
                if (head_a.m_val == head_b.m_val) return;
                m_tree.unlock();
                m_tree.modify(head_a.m_val, Info{head_b.m_val, 0});
                m_tree.modify(head_b.m_val, Info{head_b.m_val, head_a.m_size + head_b.m_size});
                m_group_count--;
                m_tree.lock();
            }
            bool unite_by_size(size_type a, size_type b) {
                if (a == b) return false;
                Info head_a = find(a), head_b = find(b);
                if (head_a.m_val == head_b.m_val) return false;
                if (head_a.m_size > head_b.m_size) std::swap(head_a, head_b);
                unite_to(head_a, head_b);
                return true;
            }
            bool in_same_group(size_type a, size_type b) { return a == b || find(a).m_val == find(b).m_val; }
            bool is_head(size_type i) { return m_tree.query(i).m_val == i; }
            size_type count() const { return m_group_count; }
            std::vector<size_type> heads() {
                std::vector<size_type> ret;
                ret.reserve(m_group_count);
                for (size_type i = 0; i < m_tree.m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<size_type>> groups() {
                std::vector<std::vector<size_type>> ret(m_group_count);
                std::vector<size_type> index(m_tree.m_size);
                for (size_type i = 0, j = 0; i < m_tree.m_size; i++) {
                    auto head_i = find(i);
                    if (head_i.m_val == i) ret[j].reserve(head_i.m_size), index[i] = j++;
                }
                for (size_type i = 0; i < m_tree.m_size; i++) ret[index[find(i).m_val]].push_back(i);
                return ret;
            }
        };
        template <typename Ostream, size_type MAX_NODE = 1 << 22>
        Ostream &operator<<(Ostream &out, const Table<MAX_NODE> &x) { return out << x.m_tree; }
    }
    template <PerUF::size_type MAX_NODE = 1 << 22>
    using PerUnionFind = PerUF::Table<MAX_NODE>;
}

#endif