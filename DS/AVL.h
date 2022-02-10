#ifndef __OY_AVL__
#define __OY_AVL__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct AVLSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct AVLMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct AVLMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _AVLNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _AVLNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = AVLMultisetTag>
    class AVL {
        struct node : _AVLNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            int subtree_height;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        };
        node *m_root;
        _Compare m_comp;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static int subtree_height(node *p) { return p ? p->subtree_height : 0; }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            p->subtree_height = 1 + std::max(subtree_height(p->lchild), subtree_height(p->rchild));
            return p;
        }
        static node *rrotate(node *p) {
            node *q = p->lchild;
            p->lchild = q->rchild;
            q->rchild = update(p);
            return update(q);
        }
        static node *lrotate(node *p) {
            node *q = p->rchild;
            p->rchild = q->lchild;
            q->lchild = update(p);
            return update(q);
        }
        static node *lrrotate(node *p) {
            node *q = p->lchild;
            node *r = q->rchild;
            q->rchild = r->lchild;
            p->lchild = r->rchild;
            r->lchild = update(q);
            r->rchild = update(p);
            return update(r);
        }
        static node *rlrotate(node *p) {
            node *q = p->rchild;
            node *r = q->lchild;
            q->lchild = r->rchild;
            p->rchild = r->lchild;
            r->rchild = update(q);
            r->lchild = update(p);
            return update(r);
        }
        static node *balance(node *cur) {
            if (!cur) return cur;
            if (int lh = subtree_height(cur->lchild), rh = subtree_height(cur->rchild); lh > rh + 1) {
                if (subtree_height(cur->lchild->lchild) > rh)
                    return rrotate(cur);
                else
                    return lrrotate(cur);
            } else if (rh > lh + 1) {
                if (subtree_height(cur->rchild->rchild) > lh)
                    return lrotate(cur);
                else
                    return rlrotate(cur);
            } else
                return update(cur);
        }
        static node *deleteMin(node *cur, node *&res) {
            if (!cur->lchild) {
                res = cur;
                return cur->rchild;
            } else {
                cur->lchild = deleteMin(cur->lchild, res);
                return balance(cur);
            }
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        AVL(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
        ~AVL() { clear(); }
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
        void insert(_Tp __key, Args... __args) {
            auto dfs = [&](auto self, node *cur) {
                if (!cur)
                    return new node{__key, __args..., 1, 1, nullptr, nullptr};
                else if (m_comp(__key, cur->key)) {
                    cur->lchild = self(self, cur->lchild);
                    return balance(cur);
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur;
                    cur->rchild = self(self, cur->rchild);
                    return balance(cur);
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
                if (!cur)
                    return nullptr;
                else if (m_comp(__key, cur->key))
                    cur->lchild = self(self, cur->lchild);
                else if (m_comp(cur->key, __key))
                    cur->rchild = self(self, cur->rchild);
                else {
                    delete cur;
                    if (!cur->rchild)
                        return cur->lchild;
                    else {
                        node *res;
                        cur->rchild = deleteMin(cur->rchild, res);
                        res->lchild = cur->lchild;
                        res->rchild = cur->rchild;
                        cur = res;
                    }
                }
                return balance(cur);
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
            for (node *cur = m_root; cur;)
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
        const node *kth(int __k) const {
            node *cur = m_root;
            while (__k >= 0)
                if (int l_count = subtree_weight(cur->lchild); __k < l_count)
                    cur = cur->lchild;
                else if (__k -= subtree_weight(cur->lchild) + 1; __k >= 0)
                    cur = cur->rchild;
            return cur;
        }
        const node *find(_Tp __key) const {
            for (node *cur = m_root; cur;) {
                if (m_comp(__key, cur->key))
                    cur = cur->lchild;
                else if (m_comp(cur->key, __key))
                    cur = cur->rchild;
                else
                    return cur;
            }
            return nullptr;
        }
        const node *lower_bound(_Tp __key) const {
            node *res = nullptr;
            for (node *cur = m_root; cur;) {
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
        const node *upper_bound(_Tp __key) const {
            node *res = nullptr;
            for (node *cur = m_root; cur;) {
                if (m_comp(__key, cur->key)) {
                    res = cur;
                    cur = cur->lchild;
                } else
                    cur = cur->rchild;
            }
            return res;
        }
        const node *smaller_bound(_Tp __key) const {
            node *res = nullptr;
            for (node *cur = m_root; cur;) {
                if (m_comp(cur->key, __key)) {
                    res = cur;
                    cur = cur->rchild;
                } else
                    cur = cur->lchild;
            }
            return res;
        }
        int size() const { return subtree_weight(m_root); }
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
    namespace AVLContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = AVL<_Tp, bool, _Compare, AVLSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = AVL<_Tp, bool, _Compare, AVLMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = AVL<_Tp, _Fp, _Compare, AVLMapTag>;
    }
}

#endif