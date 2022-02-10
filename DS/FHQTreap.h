#ifndef __OY_FHQTREAP__
#define __OY_FHQTREAP__

#include <functional>
#include <random>
#include "MemoryPool.h"

namespace OY {
    struct FHQTreapSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct FHQTreapMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct FHQTreapMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _FHQTreapNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _FHQTreapNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = FHQTreapMultisetTag>
    class FHQTreap {
        struct node : _FHQTreapNode<_Tp, _Fp, _Tag::is_map> {
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
        void split_less(node *p, const _Tp &key, node *&l, node *&r) {
            if (!p)
                l = r = nullptr;
            else if (m_comp(p->key, key)) {
                split_less(p->rchild, key, l, r);
                p->rchild = l;
                l = update(p);
            } else {
                split_less(p->lchild, key, l, r);
                p->lchild = r;
                r = update(p);
            }
        }
        void split_less_equal(node *p, const _Tp &key, node *&l, node *&r) {
            if (!p)
                l = r = nullptr;
            else if (m_comp(key, p->key)) {
                split_less_equal(p->lchild, key, l, r);
                p->lchild = r;
                r = update(p);
            } else {
                split_less_equal(p->rchild, key, l, r);
                p->rchild = l;
                l = update(p);
            }
        }
        void split_by_key(node *p, const _Tp &key, node *&l, node *&mid, node *&r) {
            if (!p)
                l = mid = r = nullptr;
            else if (m_comp(p->key, key)) {
                split_by_key(p->rchild, key, l, mid, r);
                p->rchild = l;
                l = update(p);
            } else if (m_comp(key, p->key)) {
                split_by_key(p->lchild, key, l, mid, r);
                p->lchild = r;
                r = update(p);
            } else {
                node *l_mid, *r_mid;
                split_less(p->lchild, key, l, l_mid);
                split_less_equal(p->rchild, key, r_mid, r);
                p->lchild = l_mid;
                p->rchild = r_mid;
                mid = update(p);
            }
        }
        void split_by_rank(node *p, int k, node *&l, node *&r) {
            if (!k) {
                l = nullptr;
                r = p;
            } else if (subtree_weight(p->lchild) > k) {
                split_by_rank(p->lchild, k, l, r);
                p->lchild = r;
                r = update(p);
            } else if (k -= subtree_weight(p->lchild); !k) {
                l = p->lchild;
                p->lchild = nullptr;
                r = update(p);
            } else {
                split_by_rank(p->rchild, k - 1, l, r);
                p->rchild = l;
                l = update(p);
            }
        }
        static node *merge(node *l, node *r) {
            if (!l) return r;
            if (!r) return l;
            if (l->priority > r->priority) {
                l->rchild = merge(l->rchild, r);
                return update(l);
            } else {
                r->lchild = merge(l, r->lchild);
                return update(r);
            }
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        FHQTreap(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
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
            node *l, *r;
            split_less_equal(m_root, __key, l, r);
            m_root = merge(merge(l, new node{__key, __args..., s_rand(), 1, nullptr, nullptr}), r);
        }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key))
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) {
            node *l, *mid, *r;
            split_by_key(m_root, __key, l, mid, r);
            if (!mid)
                m_root = merge(l, r);
            else {
                delete mid;
                m_root = mid->subtree_weight > 1 ? merge(merge(l, mid->lchild), merge(mid->rchild, r)) : merge(l, r);
            }
            return mid;
        }
        void erase(_Tp __key, int __count) {
            static_assert(_Tag::multi_key);
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) {
            node *l, *r;
            split_less(m_root, __key, l, r);
            int ord = subtree_weight(l);
            m_root = merge(l, r);
            return ord;
        }
        const node *kth(int __k) {
            node *l, *r;
            split_by_rank(m_root, __k, l, r);
            node *res = r;
            while (res->lchild) res = res->lchild;
            m_root = merge(l, r);
            return res;
        }
        const node *find(_Tp __key) {
            node *l, *mid, *r;
            split_by_key(m_root, __key, l, mid, r);
            node *res = mid;
            m_root = merge(merge(l, mid), r);
            return res;
        }
        const node *lower_bound(_Tp __key) {
            node *l, *r;
            split_less(m_root, __key, l, r);
            node *res = r;
            while (res->lchild) res = res->lchild;
            m_root = merge(l, r);
            return res;
        }
        const node *upper_bound(_Tp __key) {
            node *l, *r;
            split_less_equal(m_root, __key, l, r);
            node *res = r;
            while (res->lchild) res = res->lchild;
            m_root = merge(l, r);
            return res;
        }
        const node *smaller_bound(_Tp __key) {
            node *l, *r;
            split_less(m_root, __key, l, r);
            node *res = l;
            while (res->rchild) res = res->rchild;
            m_root = merge(l, r);
            return res;
        }
        int size() const { return subtree_weight(m_root); }
        bool empty() const { return !size(); }
        int count(_Tp __key) {
            node *l, *mid, *r;
            split_by_key(m_root, __key, l, mid, r);
            int res = subtree_weight(mid);
            m_root = merge(merge(l, mid), r);
            return res;
        }
    };
    namespace FHQTreapContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = FHQTreap<_Tp, bool, _Compare, FHQTreapSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = FHQTreap<_Tp, bool, _Compare, FHQTreapMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = FHQTreap<_Tp, _Fp, _Compare, FHQTreapMapTag>;
    }
}

#endif