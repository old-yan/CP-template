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
        using index_type = PerSeg::index_type;
        template <typename SizeType>
        struct Info {
            SizeType m_val, m_size;
            template <typename Ostream>
            friend Ostream &operator<<(Ostream &out, const Info &x) { return out << '(' << x.m_val << ',' << x.m_size << ')'; }
        };
        template <typename SizeType>
        struct RangeMapping {
            Info<SizeType> operator()(SizeType l, SizeType r) const { return Info<SizeType>{l == r ? l : -1, 1}; }
        };
        template <typename uint32_t>
        struct NodeBase {};
        template <>
        struct NodeBase<uint64_t> {
            uint64_t _;
        };
        template <typename SizeType>
        struct Node : NodeBase<SizeType> {
            using value_type = Info<SizeType>;
            using modify_type = Info<SizeType>;
            void pushup(Node *lchild, Node *rchild) {}
            void set(const value_type &info) {
                if (~info.m_val) *(value_type *)this = info;
            }
            value_type get() const { return *(const value_type *)this; }
        };
        template <typename SizeType = uint32_t, index_type MAX_NODE = 1 << 22>
        struct Table {
            using tree_type = PerSeg::Tree<Node<SizeType>, RangeMapping<SizeType>, false, true, SizeType, MAX_NODE>;
            tree_type m_tree;
            SizeType m_group_count;
            Table(SizeType length = 0) { resize(length); }
            Table<SizeType, MAX_NODE> copy() const {
                Table<SizeType, MAX_NODE> other;
                other.m_tree = m_tree.copy(), other.m_group_count = m_group_count;
                return other;
            }
            void resize(SizeType length) {
                if (m_group_count = length) m_tree.resize(length);
            }
            Info<SizeType> find(SizeType i) {
                auto res = m_tree.query(i);
                while (res.m_val != i) i = res.m_val, res = m_tree.query(i);
                return res;
            }
            SizeType size(SizeType i) { return find(i).m_size; }
            void unite_to(const Info<SizeType> &head_a, const Info<SizeType> &head_b) {
                if (head_a.m_val == head_b.m_val) return;
                m_tree.unlock();
                m_tree.modify(head_a.m_val, Info<SizeType>{head_b.m_val, 0});
                m_tree.modify(head_b.m_val, Info<SizeType>{head_b.m_val, head_a.m_size + head_b.m_size});
                m_group_count--;
                m_tree.lock();
            }
            bool unite_by_size(SizeType a, SizeType b) {
                if (a == b) return false;
                auto head_a = find(a), head_b = find(b);
                if (head_a.m_val == head_b.m_val) return false;
                if (head_a.m_size > head_b.m_size) std::swap(head_a, head_b);
                unite_to(head_a, head_b);
                return true;
            }
            bool in_same_group(SizeType a, SizeType b) { return a == b || find(a).m_val == find(b).m_val; }
            bool is_head(SizeType i) { return m_tree.query(i).m_val == i; }
            SizeType count() const { return m_group_count; }
            std::vector<SizeType> heads() {
                std::vector<SizeType> ret;
                ret.reserve(m_group_count);
                for (SizeType i = 0; i < m_tree.m_size; i++)
                    if (is_head(i)) ret.push_back(i);
                return ret;
            }
            std::vector<std::vector<SizeType>> groups() {
                std::vector<std::vector<SizeType>> ret(m_group_count);
                std::vector<SizeType> index(m_tree.m_size);
                for (SizeType i = 0, j = 0; i < m_tree.m_size; i++) {
                    auto head_i = find(i);
                    if (head_i.m_val == i) ret[j].reserve(head_i.m_size), index[i] = j++;
                }
                for (SizeType i = 0; i < m_tree.m_size; i++) ret[index[find(i).m_val]].push_back(i);
                return ret;
            }
        };
        template <typename Ostream, typename SizeType, index_type MAX_NODE = 1 << 22>
        Ostream &operator<<(Ostream &out, const Table<SizeType, MAX_NODE> &x) { return out << x.m_tree; }
    }
    template <typename SizeType = uint32_t, PerUF::index_type MAX_NODE = 1 << 22>
    using PerUnionFind = PerUF::Table<SizeType, MAX_NODE>;
}

#endif