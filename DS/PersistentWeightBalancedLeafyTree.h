#ifndef __OY_PERSISTENTWEIGHTBALANCEDLEAFYTREE__
#define __OY_PERSISTENTWEIGHTBALANCEDLEAFYTREE__

#include <functional>
#include <limits>
#include "MemoryPool.h"

namespace OY {
    struct PersistentWeightBalancedLeafySetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentWeightBalancedLeafyMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentWeightBalancedLeafyMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentWeightBalancedLeafyTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentWeightBalancedLeafyTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentWeightBalancedLeafyMultisetTag>
    class PersistentWeightBalancedLeafyTree {
#pragma pack(4)
        struct node : _PersistentWeightBalancedLeafyTreeNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        static constexpr int ratio = 4, bias = 2;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, p->subtree_weight, l, r};
            else
                return new node{p->key, p->subtree_weight, l, r};
        }
        static node *update(node *p) {
            p->subtree_weight = p->lchild->subtree_weight + p->rchild->subtree_weight;
            p->key = p->rchild->key;
            return p;
        }
        static node *set_lchild(node *p, node *l) {
            p->lchild = l;
            return p;
        }
        static node *set_rchild(node *p, node *r) {
            p->rchild = r;
            return p;
        }
        static node *set_lrchild(node *p, node *l, node *r) {
            p->lchild = l;
            p->rchild = r;
            return p;
        }
        static node *lrotate(node *p, int reuse) {
            if (node *r = p->rchild; reuse == 2)
                return update(set_lchild(r, update(set_rchild(p, r->lchild))));
            else if (reuse == 1)
                return update(raw_copy(r, update(set_rchild(p, r->lchild)), r->rchild));
            else
                return update(raw_copy(r, update(raw_copy(p, p->lchild, r->lchild)), r->rchild));
        }
        static node *rrotate(node *p, int reuse) {
            if (node *l = p->lchild; reuse == 2)
                return update(set_rchild(l, update(set_lchild(p, l->rchild))));
            else if (reuse == 1)
                return update(raw_copy(l, l->lchild, update(set_lchild(p, l->rchild))));
            else
                return update(raw_copy(l, l->lchild, update(raw_copy(p, l->rchild, p->rchild))));
        }
        static node *balance(node *cur, bool is_insert) {
            if (cur->lchild->subtree_weight > cur->rchild->subtree_weight * ratio + bias) {
                if (cur->lchild->rchild->subtree_weight > cur->lchild->lchild->subtree_weight * (ratio - 1) + bias)
                    cur->lchild = lrotate(cur->lchild, 0);
                return rrotate(cur, 1 + is_insert);
            } else if (cur->rchild->subtree_weight > cur->lchild->subtree_weight * ratio + bias) {
                if (cur->rchild->lchild->subtree_weight > cur->rchild->rchild->subtree_weight * (ratio - 1) + bias)
                    cur->rchild = rrotate(cur->rchild, 0);
                return lrotate(cur, 1 + is_insert);
            } else
                return update(cur);
        }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentWeightBalancedLeafyTree(_Tp __maxKey = std::numeric_limits<_Tp>::max(), _Compare __comp = _Compare()) : m_comp(__comp) {
            node *root = new node;
            root->key = __maxKey;
            root->subtree_weight = 1;
            root->lchild = nullptr;
            root->rchild = nullptr;
            m_roots.push_back(root);
        }
        ~PersistentWeightBalancedLeafyTree() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, _Tp __key, Args... __args) {
            bool res = _Tag::multi_key;
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur->lchild) {
                    res = true;
                    node *p = raw_copy(cur, new node{__key, __args..., 1, nullptr, nullptr}, cur);
                    p->subtree_weight = 2;
                    return p;
                } else if (m_comp(cur->lchild->key, __key)) {
                    node *r = self(self, cur->rchild);
                    return res ? balance(raw_copy(cur, cur->lchild, r), true) : cur;
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(__key, cur->lchild->key)) return cur;
                    node *l = self(self, cur->lchild);
                    return res ? balance(raw_copy(cur, l, cur->rchild), true) : cur;
                };
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur->lchild) {
                    if (m_comp(__key, cur->key)) {
                        node *p = raw_copy(cur, new node{__key, __value, 1, nullptr, nullptr}, cur);
                        p->subtree_weight = 2;
                        return p;
                    } else {
                        node *p = raw_copy(cur, cur->lchild, cur->rchild);
                        p->value = __value;
                        return p;
                    }
                } else if (m_comp(cur->lchild->key, __key))
                    return balance(raw_copy(cur, cur->lchild, self(self, cur->rchild)), true);
                else
                    return balance(raw_copy(cur, self(self, cur->lchild), cur->rchild), true);
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
        }
        bool erase(int __prevVersion, _Tp __key) {
            bool res = false;
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur->lchild) {
                    if (m_comp(__key, cur->key)) return cur;
                    res = true;
                    return nullptr;
                } else if (!m_comp(cur->lchild->key, __key)) {
                    node *l = self(self, cur->lchild);
                    if (!res) return cur;
                    if (!l) return cur->rchild;
                    return balance(raw_copy(cur, l, cur->rchild), false);
                } else {
                    node *r = self(self, cur->rchild);
                    if (!res) return cur;
                    if (!r) return cur->lchild;
                    return balance(raw_copy(cur, cur->lchild, r), false);
                }
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
            return res;
        }
        void copyVersion(int __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        int rank(int __version, _Tp __key) const {
            int ord = 0;
            for (node *cur = _root(__version); cur->lchild;)
                if (m_comp(cur->lchild->key, __key)) {
                    ord += cur->lchild->subtree_weight;
                    cur = cur->rchild;
                } else
                    cur = cur->lchild;
            return ord;
        }
        const node *kth(int __version, int __k) const {
            node *cur = _root(__version);
            while (cur->lchild)
                if (int l_count = cur->lchild->subtree_weight; __k < l_count)
                    cur = cur->lchild;
                else {
                    __k -= l_count;
                    cur = cur->rchild;
                }
            return cur;
        }
        const node *find(int __version, _Tp __key) const {
            node *cur = _root(__version);
            while (cur->lchild)
                if (m_comp(cur->lchild->key, __key))
                    cur = cur->rchild;
                else
                    cur = cur->lchild;
            return m_comp(__key, cur->key) ? nullptr : cur;
        }
        const node *lower_bound(int __version, _Tp __key) const {
            node *cur = _root(__version);
            while (cur->lchild)
                if (m_comp(cur->lchild->key, __key))
                    cur = cur->rchild;
                else
                    cur = cur->lchild;
            return cur;
        }
        const node *upper_bound(int __version, _Tp __key) const {
            node *cur = _root(__version);
            while (cur->lchild)
                if (m_comp(__key, cur->lchild->key))
                    cur = cur->lchild;
                else
                    cur = cur->rchild;
            return cur;
        }
        const node *smaller_bound(int __version, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = _root(__version); cur->lchild;) {
                if (m_comp(cur->lchild->key, __key)) {
                    res = cur->lchild;
                    cur = cur->rchild;
                } else
                    cur = cur->lchild;
            }
            if (res)
                while (res->rchild) res = res->rchild;
            return res;
        }
        int size(int __version) const { return subtree_weight(_root(__version)) - 1; }
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
    namespace PersistentWeightBalancedLeafyTreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentWeightBalancedLeafyTree<_Tp, bool, _Compare, PersistentWeightBalancedLeafySetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentWeightBalancedLeafyTree<_Tp, bool, _Compare, PersistentWeightBalancedLeafyMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentWeightBalancedLeafyTree<_Tp, _Fp, _Compare, PersistentWeightBalancedLeafyMapTag>;
    }
}

#endif