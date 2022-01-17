#ifndef __OY_SCAPEGOADTREE__
#define __OY_SCAPEGOADTREE__

#include "DS/MemoryPool.h"
#include <functional>

namespace OY {
    struct ScapeGoatTreeSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
        using weight_type = bool;
    };
    struct ScapeGoatTreeMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
        using weight_type = int;
    };
    struct ScapeGoatTreeMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
        using weight_type = bool;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _ScapeGoatTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _ScapeGoatTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = ScapeGoatTreeMultisetTag>
    class ScapeGoatTree {
        struct node : _ScapeGoatTreeNode<_Tp, _Fp, _Tag::is_map> {
            typename _Tag::weight_type node_weight;
            int subtree_weight;
            int subtree_size;
            int subtree_capacity;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>().operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
        node *m_root;
        _Compare m_comp;
        static constexpr int m_ratio = 4, m_bias = 4;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static int subtree_size(node *p) { return p ? p->subtree_size : 0; }
        static int subtree_capacity(node *p) { return p ? p->subtree_capacity : 0; }
        static node *update(node *p) {
            p->subtree_weight = p->node_weight + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            p->subtree_size = (p->node_weight != 0) + subtree_size(p->lchild) + subtree_size(p->rchild);
            p->subtree_capacity = 1 + subtree_capacity(p->lchild) + subtree_capacity(p->rchild);
            return p;
        }
        static void traverse(node *p, node **&s_cursor) {
            if (p->lchild) traverse(p->lchild, s_cursor);
            if (p->node_weight)
                *s_cursor++ = p;
            else
                delete p;
            if (p->rchild) traverse(p->rchild, s_cursor);
        }
        static node *make_tree(node **first, node **last) {
            if (first == last) return nullptr;
            node **mid = first + (last - first) / 2;
            (*mid)->lchild = make_tree(first, mid);
            (*mid)->rchild = make_tree(mid + 1, last);
            return update(*mid);
        }
        static node *balance(node *cur) {
            update(cur);
            if (subtree_size(cur->lchild) <= subtree_size(cur->rchild) * m_ratio + m_bias && subtree_size(cur->rchild) <= subtree_size(cur->lchild) * m_ratio + m_bias && cur->subtree_capacity * 2 <= cur->subtree_size * 3) return cur;
            static std::vector<node *> s_buffer;
            static node **s_cursor;
            s_buffer.resize(cur->subtree_size);
            s_cursor = s_buffer.data();
            traverse(cur, s_cursor);
            return make_tree(s_buffer.data(), s_cursor);
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        ScapeGoatTree(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
        ~ScapeGoatTree() { clear(); }
        void clear() {
            auto dfs = [&](auto self, node *p) -> void {
                delete p;
                if (p->lchild) self(self, p->lchild);
                if (p->rchild) self(self, p->rchild);
            };
            if (m_root) dfs(dfs, m_root);
            m_root = nullptr;
        }
        template <typename... Args>
        void insert(_Tp key, Args... __args) {
            auto dfs = [&](auto self, node *cur) {
                if (!cur)
                    return new node{key, __args..., 1, 1, 1, 1, nullptr, nullptr};
                else if (m_comp(key, cur->key)) {
                    cur->lchild = self(self, cur->lchild);
                    return balance(cur);
                } else if (m_comp(cur->key, key)) {
                    cur->rchild = self(self, cur->rchild);
                    return balance(cur);
                } else {
                    if constexpr (_Tag::multi_key) {
                        cur->node_weight++;
                        cur->subtree_weight++;
                        if (cur->node_weight == 1) cur->subtree_size++;
                    } else if (!cur->node_weight) {
                        cur->node_weight = true;
                        cur->subtree_weight++;
                        cur->subtree_size++;
                    }
                    return cur;
                }
            };
            m_root = dfs(dfs, m_root);
        }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key))
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur)
                    return nullptr;
                else if (m_comp(__key, cur->key))
                    cur->lchild = self(self, cur->lchild);
                else if (m_comp(cur->key, __key))
                    cur->rchild = self(self, cur->rchild);
                else if (cur->node_weight) {
                    if constexpr (_Tag::multi_key) {
                        cur->node_weight--;
                        cur->subtree_weight--;
                        if (!cur->node_weight) cur->subtree_size--;
                    } else if (cur->node_weight) {
                        cur->node_weight = false;
                        cur->subtree_weight--;
                        cur->subtree_size--;
                    }
                }
                return balance(cur);
            };
            int oldsize = size();
            m_root = dfs(dfs, m_root);
            return oldsize != size();
        }
        void erase(_Tp __key, int __count) {
            static_assert(_Tag::multi_key);
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) const {
            auto dfs = [&](auto self, node *cur) -> int {
                if (!cur)
                    return 0;
                else if (m_comp(__key, cur->key))
                    return self(self, cur->lchild);
                else if (m_comp(cur->key, __key))
                    return subtree_weight(cur->lchild) + cur->node_weight + self(self, cur->rchild);
                else
                    return subtree_weight(cur->lchild);
            };
            return dfs(dfs, m_root);
        }
        const node *kth(int __k) const {
            auto dfs = [&](auto self, node *cur, int k) -> node * {
                if (!cur) return nullptr;
                if (int l_count = subtree_weight(cur->lchild); k < l_count)
                    return self(self, cur->lchild, k);
                else if (k < l_count + cur->node_weight)
                    return cur;
                else
                    return self(self, cur->rchild, k - l_count - cur->node_weight);
            };
            return dfs(dfs, m_root, __k);
        }
        const node *find(_Tp __key) const {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) return nullptr;
                if (m_comp(__key, cur->key))
                    return self(self, cur->lchild);
                else if (m_comp(cur->key, __key))
                    return self(self, cur->rchild);
                else if (cur->node_weight)
                    return cur;
                else
                    return nullptr;
            };
            return dfs(dfs, m_root);
        }
        const node *lower_bound(_Tp __key) const {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) return nullptr;
                if (m_comp(__key, cur->key)) {
                    node *res = self(self, cur->lchild);
                    if (!res && cur->node_weight) res = cur;
                    if (!res) res = self(self, cur->rchild);
                    return res;
                } else if (!m_comp(cur->key, __key) && cur->node_weight)
                    return cur;
                else
                    return self(self, cur->rchild);
            };
            return dfs(dfs, m_root);
        }
        const node *upper_bound(_Tp __key) const {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) return nullptr;
                if (m_comp(__key, cur->key)) {
                    node *res = self(self, cur->lchild);
                    if (!res && cur->node_weight) res = cur;
                    if (!res) res = self(self, cur->rchild);
                    return res;
                } else
                    return self(self, cur->rchild);
            };
            return dfs(dfs, m_root);
        }
        const node *smaller_bound(_Tp __key) const {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) return nullptr;
                if (m_comp(cur->key, __key)) {
                    node *res = self(self, cur->rchild);
                    if (!res && cur->node_weight) res = cur;
                    if (!res) res = self(self, cur->lchild);
                    return res;
                } else
                    return self(self, cur->lchild);
            };
            return dfs(dfs, m_root);
        }
        int size() const { return subtree_weight(m_root); }
        bool empty() const { return !size(); }
        int count(_Tp __key) const {
            auto p = find(__key);
            return p ? p->node_weight : 0;
        }
    };
    namespace ScapeGoatContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = ScapeGoatTree<_Tp, bool, _Compare, ScapeGoatTreeSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = ScapeGoatTree<_Tp, bool, _Compare, ScapeGoatTreeMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = ScapeGoatTree<_Tp, _Fp, _Compare, ScapeGoatTreeMapTag>;
    }
}

#endif