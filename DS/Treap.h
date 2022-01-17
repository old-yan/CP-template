#ifndef __OY_TREAP__
#define __OY_TREAP__

#include <functional>
#include <random>
#include "MemoryPool.h"

namespace OY {
    struct TreapSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct TreapMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct TreapMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _TreapNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _TreapNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = TreapMultisetTag>
    class Treap {
        struct node : _TreapNode<_Tp, _Fp, _Tag::is_map> {
            uint_fast32_t priority;
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        };
        node *m_root;
        _Compare m_comp;
        static inline std::mt19937 s_rand;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static uint_fast32_t subtree_priority(node *p) { return p ? p->priority : 0; }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
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

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        Treap(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
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
            bool res = false;
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) {
                    res = true;
                    cur = new node{__key, __args..., s_rand(), 1, nullptr, nullptr};
                } else if (m_comp(__key, cur->key)) {
                    cur->lchild = self(self, cur->lchild);
                    if (_Tag::multi_key || res) {
                        if (subtree_priority(cur->lchild) > cur->priority)
                            return rrotate(cur);
                        else
                            cur->subtree_weight++;
                    }
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur;
                    cur->rchild = self(self, cur->rchild);
                    if (_Tag::multi_key || res) {
                        if (subtree_priority(cur->rchild) > cur->priority)
                            return lrotate(cur);
                        else
                            cur->subtree_weight++;
                    }
                }
                return cur;
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
                    if (!cur->lchild)
                        return cur->rchild;
                    else if (!cur->rchild)
                        return cur->lchild;
                    else if (cur->lchild->subtree_weight > cur->rchild->subtree_weight) {
                        node *q = cur->lchild;
                        cur->lchild = q->rchild;
                        q->rchild = self(self, cur);
                        cur = q;
                    } else {
                        node *q = cur->rchild;
                        cur->rchild = q->lchild;
                        q->lchild = self(self, cur);
                        cur = q;
                    }
                }
                return update(cur);
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
    namespace TreapContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = Treap<_Tp, bool, _Compare, TreapSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = Treap<_Tp, bool, _Compare, TreapMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = Treap<_Tp, _Fp, _Compare, TreapMapTag>;
    }
}

#endif