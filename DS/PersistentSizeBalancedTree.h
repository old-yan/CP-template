#ifndef __OY_PERSISTENTSIZEBALANCEDTREE__
#define __OY_PERSISTENTSIZEBALANCEDTREE__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct PersistentSizeBalancedTreeSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentSizeBalancedTreeMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentSizeBalancedTreeMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentSizeBalancedTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentSizeBalancedTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentSizeBalancedTreeMultisetTag>
    class PersistentSizeBalancedTree {
#pragma pack(4)
        struct node : _PersistentSizeBalancedTreeNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static int lchild_weight(node *p) { return p && p->lchild ? p->lchild->subtree_weight : 0; }
        static int rchild_weight(node *p) { return p && p->rchild ? p->rchild->subtree_weight : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, 1, l, r};
            else
                return new node{p->key, 1, l, r};
        }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
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
        static node *rrotate(node *p, int d) {
            if (node *q = p->lchild; !d)
                return set_rchild(q, update(set_lchild(p, q->rchild)));
            else
                return raw_copy(q, q->lchild, update(set_lchild(p, q->rchild)));
        }
        static node *lrotate(node *p, int d) {
            if (node *q = p->rchild; !d)
                return set_lchild(q, update(set_rchild(p, q->lchild)));
            else
                return raw_copy(q, update(set_rchild(p, q->lchild)), q->rchild);
        }
        static node *lrrotate(node *p, int d) {
            if (node *l = p->lchild, *lr = l->rchild; !d)
                return set_lrchild(lr, update(set_rchild(l, lr->lchild)), update(set_lchild(p, lr->rchild)));
            else if (d == 1)
                return raw_copy(lr, update(raw_copy(l, l->lchild, lr->lchild)), update(set_lchild(p, lr->rchild)));
            else
                return raw_copy(lr, update(raw_copy(l, l->lchild, lr->lchild)), update(raw_copy(p, lr->rchild, p->rchild)));
        }
        static node *rlrotate(node *p, int d) {
            if (node *r = p->rchild, *rl = r->lchild; !d)
                return set_lrchild(rl, update(set_rchild(p, rl->lchild)), update(set_lchild(r, rl->rchild)));
            else if (d == 1)
                return raw_copy(rl, update(set_rchild(p, rl->lchild)), update(raw_copy(r, rl->rchild, r->rchild)));
            else
                return raw_copy(rl, update(raw_copy(p, p->lchild, rl->lchild)), update(raw_copy(r, rl->rchild, r->rchild)));
        }
        static node *lbalance(node *cur, int d) {
            if (!cur) return cur;
            if (subtree_weight(cur->lchild) < lchild_weight(cur->rchild))
                cur = rlrotate(cur, d);
            else if (subtree_weight(cur->lchild) < rchild_weight(cur->rchild))
                cur = lrotate(cur, d);
            else
                return update(cur);
            return rbalance(lbalance(set_lrchild(cur, rbalance(cur->lchild, d + 1), lbalance(cur->rchild, d + 1)), d + 1), d + 1);
        }
        static node *rbalance(node *cur, int d) {
            if (!cur) return cur;
            if (subtree_weight(cur->rchild) < rchild_weight(cur->lchild))
                cur = lrrotate(cur, d);
            else if (subtree_weight(cur->rchild) < lchild_weight(cur->lchild))
                cur = rrotate(cur, d);
            else
                return update(cur);
            return rbalance(lbalance(set_lrchild(cur, rbalance(cur->lchild, d + 1), lbalance(cur->rchild, d + 1)), d + 1), d + 1);
        }
        static node *deleteMin(node *cur, node *&res) {
            if (!cur->lchild) {
                res = cur;
                return cur->rchild;
            } else
                return update(raw_copy(cur, deleteMin(cur->lchild, res), cur->rchild));
        }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentSizeBalancedTree(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentSizeBalancedTree() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, _Tp __key, Args... __args) {
            bool res = _Tag::multi_key;
            auto dfs = [&](auto self, node *cur) {
                if (!cur) {
                    res = true;
                    return new node{__key, __args..., 1, nullptr, nullptr};
                } else if (m_comp(__key, cur->key)) {
                    node *l = self(self, cur->lchild);
                    return res ? rbalance(raw_copy(cur, l, cur->rchild), 0) : cur;
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur;
                    node *r = self(self, cur->rchild);
                    return res ? lbalance(raw_copy(cur, cur->lchild, r), 0) : cur;
                };
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            auto dfs = [&](auto self, node *cur) {
                if (!cur)
                    return new node{__key, __value, 1, nullptr, nullptr};
                else if (m_comp(__key, cur->key))
                    return rbalance(raw_copy(cur, self(self, cur->lchild), cur->rchild));
                else if (m_comp(cur->key, __key))
                    return lbalance(raw_copy(cur, cur->lchild, self(self, cur->rchild)));
                else {
                    node *p = raw_copy(cur, cur->lchild, cur->rchild);
                    p->value = __value;
                    return p;
                };
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
                    return res ? update(raw_copy(cur, l, cur->rchild)) : cur;
                } else if (m_comp(cur->key, __key)) {
                    node *r = self(self, cur->rchild);
                    return res ? update(raw_copy(cur, cur->lchild, r)) : cur;
                } else {
                    res = true;
                    if (!cur->rchild)
                        return cur->lchild;
                    node *res, *r = deleteMin(cur->rchild, res);
                    return update(raw_copy(res, cur->lchild, r));
                }
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
            return res;
        }
        void copyVersion(int __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        int rank(int __version, _Tp __key) const {
            int ord = 0;
            for (node *cur = _root(__version); cur;)
                if (!m_comp(cur->key, __key))
                    cur = cur->lchild;
                else {
                    ord += subtree_weight(cur->lchild);
                    if (m_comp(cur->key, __key)) {
                        ord++;
                        cur = cur->rchild;
                    } else
                        break;
                }
            return ord;
        }
        const node *kth(int __version, int __k) const {
            node *cur = _root(__version);
            while (__k >= 0)
                if (int l_count = subtree_weight(cur->lchild); __k < l_count)
                    cur = cur->lchild;
                else if (__k -= subtree_weight(cur->lchild) + 1; __k >= 0)
                    cur = cur->rchild;
            return cur;
        }
        const node *find(int __version, _Tp __key) const {
            for (node *cur = _root(__version); cur;) {
                if (m_comp(__key, cur->key))
                    cur = cur->lchild;
                else if (m_comp(cur->key, __key))
                    cur = cur->rchild;
                else
                    return cur;
            }
            return nullptr;
        }
        const node *lower_bound(int __version, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = _root(__version); cur;) {
                if (m_comp(__key, cur->key)) {
                    res = cur;
                    cur = cur->lchild;
                } else if (m_comp(cur->key, __key))
                    cur = cur->rchild;
                else
                    return cur;
            }
            return res;
        }
        const node *upper_bound(int __version, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = _root(__version); cur;) {
                if (m_comp(__key, cur->key)) {
                    res = cur;
                    cur = cur->lchild;
                } else
                    cur = cur->rchild;
            }
            return res;
        }
        const node *smaller_bound(int __version, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = _root(__version); cur;) {
                if (m_comp(cur->key, __key)) {
                    res = cur;
                    cur = cur->rchild;
                } else
                    cur = cur->lchild;
            }
            return res;
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
    namespace PersistentSizeBalancedTreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentSizeBalancedTree<_Tp, bool, _Compare, PersistentSizeBalancedTreeSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentSizeBalancedTree<_Tp, bool, _Compare, PersistentSizeBalancedTreeMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentSizeBalancedTree<_Tp, _Fp, _Compare, PersistentSizeBalancedTreeMapTag>;
    }
}

#endif