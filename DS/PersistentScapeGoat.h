#ifndef __OY_PERSISTENTSCAPEGOAT__
#define __OY_PERSISTENTSCAPEGOAT__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct PersistentScapeGoatTreeSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentScapeGoatTreeMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentScapeGoatTreeMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentScapeGoatTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentScapeGoatTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentScapeGoatTreeMultisetTag>
    class PersistentScapeGoatTree {
#pragma pack(4)
        struct node : _PersistentScapeGoatTreeNode<_Tp, _Fp, _Tag::is_map> {
            bool node_weight;
            int subtree_weight;
            int subtree_capacity;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>().operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        static constexpr int ratio = 4, bias = 4;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static int subtree_capacity(node *p) { return p ? p->subtree_capacity : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, p->node_weight, p->subtree_weight, p->subtree_capacity, l, r};
            else
                return new node{p->key, p->node_weight, p->subtree_weight, p->subtree_capacity, l, r};
        }
        static node *raw_copy_true(node *p, node *l, node *r) {
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, true, p->subtree_weight + 1, p->subtree_capacity, l, r};
            else
                return new node{p->key, true, p->subtree_weight + 1, p->subtree_capacity, l, r};
        }
        static node *raw_copy_false(node *p, node *l, node *r) {
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, false, p->subtree_weight - 1, p->subtree_capacity, l, r};
            else
                return new node{p->key, false, p->subtree_weight - 1, p->subtree_capacity, l, r};
        }
        static node *update(node *p) {
            p->subtree_weight = p->node_weight + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            p->subtree_capacity = 1 + subtree_capacity(p->lchild) + subtree_capacity(p->rchild);
            return p;
        }
        static void traverse(node *p, node **&s_cursor) {
            if (p->lchild) traverse(p->lchild, s_cursor);
            if (p->node_weight) *s_cursor++ = p;
            if (p->rchild) traverse(p->rchild, s_cursor);
        }
        static node *make_tree(node **first, node **last) {
            if (first == last) return nullptr;
            node **mid = first + (last - first) / 2;
            return update(raw_copy(*mid, make_tree(first, mid), make_tree(mid + 1, last)));
        }
        static node *balance(node *cur) {
            update(cur);
            if (subtree_weight(cur->lchild) <= subtree_weight(cur->rchild) * ratio + bias && subtree_weight(cur->rchild) <= subtree_weight(cur->lchild) * ratio + bias && cur->subtree_capacity * 2 <= cur->subtree_weight * 3) return cur;
            static std::vector<node *> s_buffer;
            static node **s_cursor;
            s_buffer.resize(cur->subtree_weight);
            s_cursor = s_buffer.data();
            traverse(cur, s_cursor);
            return make_tree(s_buffer.data(), s_cursor);
        }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentScapeGoatTree(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentScapeGoatTree() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, _Tp __key, Args... __args) {
            bool res = _Tag::multi_key;
            auto dfs = [&](auto self, node *cur) {
                if (!cur) {
                    res = true;
                    return new node{__key, __args..., true, 1, 1, nullptr, nullptr};
                } else if (m_comp(__key, cur->key)) {
                    node *l = self(self, cur->lchild);
                    return res ? balance(raw_copy(cur, l, cur->rchild)) : cur;
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur->node_weight ? cur : raw_copy_true(cur, cur->lchild, cur->rchild);
                    node *r = self(self, cur->rchild);
                    return res ? balance(raw_copy(cur, cur->lchild, r)) : cur;
                }
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            auto dfs = [&](auto self, node *cur) {
                if (!cur)
                    return new node{__key, __value, true, 1, 1, nullptr, nullptr};
                else if (m_comp(__key, cur->key))
                    return balance(raw_copy(cur, self(self, cur->lchild), cur->rchild));
                else if (m_comp(cur->key, __key))
                    return balance(raw_copy(cur, cur->lchild, self(self, cur->rchild)));
                else
                    return cur->node_weight ? cur : raw_copy_true(cur, cur->lchild, cur->rchild);
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
        }
        bool erase(int __prevVersion, _Tp __key) {
            bool res = false;
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur)
                    return nullptr;
                else if (m_comp(__key, cur->key)) {
                    node *l = self(self, cur->lchild);
                    return res ? balance(raw_copy(cur, l, cur->rchild)) : cur;
                } else if (m_comp(cur->key, __key)) {
                    node *r = self(self, cur->rchild);
                    return res ? balance(raw_copy(cur, cur->lchild, r)) : cur;
                } else if (cur->node_weight) {
                    res = true;
                    return raw_copy_false(cur, cur->lchild, cur->rchild);
                } else {
                    node *l = self(self, cur->lchild);
                    if (res) return balance(raw_copy(cur, l, cur->rchild));
                    l = self(self, cur->rchild);
                    return res ? balance(raw_copy(cur, cur->lchild, l)) : cur;
                }
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
            return res;
        }
        void copyVersion(int __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        int rank(int __version, _Tp __key) const {
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
            return dfs(dfs, _root(__version));
        }
        const node *kth(int __version, int __k) const {
            auto dfs = [&](auto self, node *cur, int k) -> node * {
                if (!cur) return nullptr;
                if (int l_count = subtree_weight(cur->lchild); k < l_count)
                    return self(self, cur->lchild, k);
                else if (k -= l_count + cur->node_weight; k < 0)
                    return cur;
                else
                    return self(self, cur->rchild, k);
            };
            return dfs(dfs, _root(__version), __k);
        }
        const node *find(int __version, _Tp __key) const {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) return nullptr;
                if (m_comp(__key, cur->key))
                    return self(self, cur->lchild);
                else if (m_comp(cur->key, __key))
                    return self(self, cur->rchild);
                else if (cur->node_weight)
                    return cur;
                else if (node *res = self(self, cur->rchild))
                    return res;
                else
                    return self(self, cur->lchild);
            };
            return dfs(dfs, _root(__version));
        }
        const node *lower_bound(int __version, _Tp __key) const {
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
            return dfs(dfs, _root(__version));
        }
        const node *upper_bound(int __version, _Tp __key) const {
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
            return dfs(dfs, _root(__version));
        }
        const node *smaller_bound(int __version, _Tp __key) const {
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
            return dfs(dfs, _root(__version));
        }
        int size(int __version) const { return subtree_weight(_root(__version)); }
        bool empty(int __version) const { return !size(__version); }
        int count(int __version, _Tp __key) const {
            auto it1 = lower_bound(__version, __key);
            if (!it1) return 0;
            auto it2 = upper_bound(__version, __key);
            if (!it2)
                return size(__version) - rank(__version, __key);
            else
                return rank(__version, it2->key) - rank(__version, __key);
        }
        int versionCount() const { return m_roots.size(); }
    };
    namespace PersistentScapeGoatContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentScapeGoatTree<_Tp, bool, _Compare, PersistentScapeGoatTreeSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentScapeGoatTree<_Tp, bool, _Compare, PersistentScapeGoatTreeMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentScapeGoatTree<_Tp, _Fp, _Compare, PersistentScapeGoatTreeMapTag>;
    }
}

#endif