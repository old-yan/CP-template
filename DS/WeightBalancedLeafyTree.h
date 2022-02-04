#ifndef __OY_WEIGHTBALANCEDLEAFYTREE__
#define __OY_WEIGHTBALANCEDLEAFYTREE__

#include <functional>
#include <limits>
#include "MemoryPool.h"

namespace OY {
    struct WeightBalancedLeafySetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct WeightBalancedLeafyMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct WeightBalancedLeafyMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _WeightBalancedLeafyTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _WeightBalancedLeafyTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = WeightBalancedLeafyMultisetTag>
    class WeightBalancedLeafyTree {
        struct node : _WeightBalancedLeafyTreeNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
        node *m_root;
        _Compare m_comp;
        static constexpr int m_ratio = 4, m_bias = 2;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static node *update(node *p) {
            p->subtree_weight = p->lchild->subtree_weight + p->rchild->subtree_weight;
            p->key = p->rchild->key;
            return p;
        }
        static node *lrotate(node *p) {
            node *q = p->rchild;
            p->rchild = q->rchild;
            q->rchild = q->lchild;
            q->lchild = p->lchild;
            p->lchild = update(q);
            return p;
        }
        static node *rrotate(node *p) {
            node *q = p->lchild;
            p->lchild = q->lchild;
            q->lchild = q->rchild;
            q->rchild = p->rchild;
            p->rchild = update(q);
            return p;
        }
        static node *balance(node *cur) {
            if (cur->lchild->subtree_weight > cur->rchild->subtree_weight * m_ratio + m_bias) {
                if (cur->lchild->rchild->subtree_weight > cur->lchild->lchild->subtree_weight * (m_ratio - 1) + m_bias)
                    cur->lchild = lrotate(cur->lchild);
                return update(rrotate(cur));
            } else if (cur->rchild->subtree_weight > cur->lchild->subtree_weight * m_ratio + m_bias) {
                if (cur->rchild->lchild->subtree_weight > cur->rchild->rchild->subtree_weight * (m_ratio - 1) + m_bias)
                    cur->rchild = rrotate(cur->rchild);
                return update(lrotate(cur));
            } else
                return update(cur);
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        WeightBalancedLeafyTree(_Tp __maxKey = std::numeric_limits<_Tp>::max(), _Compare __comp = _Compare()) : m_comp(__comp) {
            m_root = new node;
            m_root->key = __maxKey;
            m_root->subtree_weight = 1;
            m_root->lchild = nullptr;
            m_root->rchild = nullptr;
        }
        ~WeightBalancedLeafyTree() { clear(); }
        void clear() {
            auto dfs = [&](auto self, node *p) -> void {
                delete p;
                if (p->lchild) self(self, p->lchild);
                if (p->rchild) self(self, p->rchild);
            };
            if (m_root->lchild) {
                dfs(dfs, m_root->lchild);
                dfs(dfs, m_root->rchild);
            }
            m_root->subtree_weight = 1;
            m_root->lchild = nullptr;
            m_root->rchild = nullptr;
        }
        template <typename... Args>
        void insert(_Tp __key, Args... __args) {
            auto dfs = [&](auto self, node *cur) -> node * {
                if (cur->lchild && m_comp(cur->lchild->key, __key)) {
                    cur->rchild = self(self, cur->rchild);
                    return balance(cur);
                } else {
                    if (cur->lchild) {
                        if constexpr (!_Tag::multi_key)
                            if (!m_comp(__key, cur->lchild->key)) return cur;
                        cur->lchild = self(self, cur->lchild);
                        return balance(cur);
                    } else {
                        cur->lchild = new node{__key, __args..., 1, nullptr, nullptr};
                        cur->rchild = new node(*cur);
                        cur->rchild->lchild = nullptr;
                        cur->rchild->rchild = nullptr;
                        return update(cur);
                    }
                };
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
                if (!cur->lchild) {
                    if (m_comp(__key, cur->key)) return cur;
                    delete cur;
                    return nullptr;
                } else if (m_comp(cur->lchild->key, __key)) {
                    if (cur->rchild = self(self, cur->rchild)) return balance(cur);
                    delete cur;
                    return cur->lchild;
                } else if (cur->lchild = self(self, cur->lchild))
                    return balance(cur);
                delete cur;
                return cur->rchild;
            };
            int old_weight = size();
            m_root = dfs(dfs, m_root);
            return old_weight != size();
        }
        void erase(_Tp __key, int __count) {
            static_assert(_Tag::multi_key);
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) const {
            int ord = 0;
            for (node *cur = m_root; cur->lchild;)
                if (m_comp(cur->lchild->key, __key)) {
                    ord += cur->lchild->subtree_weight;
                    cur = cur->rchild;
                } else
                    cur = cur->lchild;
            return ord;
        }
        const node *kth(int __k) const {
            node *cur = m_root;
            while (cur->lchild)
                if (int l_count = cur->lchild->subtree_weight; __k < l_count)
                    cur = cur->lchild;
                else {
                    __k -= l_count;
                    cur = cur->rchild;
                }
            return cur;
        }
        const node *find(_Tp __key) const {
            node *cur = m_root;
            while (cur->lchild)
                if (m_comp(cur->lchild->key, __key))
                    cur = cur->rchild;
                else
                    cur = cur->lchild;
            return m_comp(__key, cur->key) ? nullptr : cur;
        }
        const node *lower_bound(_Tp __key) const {
            node *cur = m_root;
            while (cur->lchild)
                if (m_comp(cur->lchild->key, __key))
                    cur = cur->rchild;
                else
                    cur = cur->lchild;
            return cur;
        }
        const node *upper_bound(_Tp __key) const {
            node *cur = m_root;
            while (cur->lchild)
                if (m_comp(__key, cur->lchild->key))
                    cur = cur->lchild;
                else
                    cur = cur->rchild;
            return cur;
        }
        const node *smaller_bound(_Tp __key) const {
            node *res = nullptr;
            for (node *cur = m_root; cur->lchild;) {
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
        int size() const { return subtree_weight(m_root) - 1; }
        bool empty() const { return !size(); }
        int count(_Tp __key) const {
            auto it1 = lower_bound(__key);
            if (!it1) return 0;
            auto it2 = upper_bound(__key);
            if (!it2)
                return size() - rank(__key);
            else
                return rank(it2->key) - rank(__key);
        }
    };
    namespace WeightBalancedLeafyTreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = WeightBalancedLeafyTree<_Tp, bool, _Compare, WeightBalancedLeafySetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = WeightBalancedLeafyTree<_Tp, bool, _Compare, WeightBalancedLeafyMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = WeightBalancedLeafyTree<_Tp, _Fp, _Compare, WeightBalancedLeafyMapTag>;
    }
}

#endif