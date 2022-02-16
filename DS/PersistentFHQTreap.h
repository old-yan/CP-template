#ifndef __OY_PERSISTENTFHQTREAP__
#define __OY_PERSISTENTFHQTREAP__

#include <functional>
#include <random>
#include "MemoryPool.h"

namespace OY {
    struct PersistentFHQTreapSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentFHQTreapMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentFHQTreapMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentFHQTreapNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentFHQTreapNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentFHQTreapMultisetTag>
    class PersistentFHQTreap {
#pragma pack(4)
        struct node : _PersistentFHQTreapNode<_Tp, _Fp, _Tag::is_map> {
            uint_fast32_t priority;
            int subtree_weight;
            uint32_t time_stamp;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        static inline std::mt19937 s_rand;
        static inline uint32_t s_timer = 0;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static uint_fast32_t subtree_priority(node *p) { return p ? p->priority : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if (p->time_stamp == s_timer) {
                p->lchild = l;
                p->rchild = r;
                return p;
            }
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, p->priority, p->subtree_weight, s_timer, l, r};
            else
                return new node{p->key, p->priority, p->subtree_weight, s_timer, l, r};
        }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            return p;
        }
        void split_less(node *p, const _Tp &key, node *&l, node *&r) {
            if (!p)
                l = r = nullptr;
            else if (m_comp(p->key, key)) {
                split_less(p->rchild, key, l, r);
                l = update(raw_copy(p, p->lchild, l));
            } else {
                split_less(p->lchild, key, l, r);
                r = update(raw_copy(p, r, p->rchild));
            }
        }
        void split_less_equal(node *p, const _Tp &key, node *&l, node *&r) {
            if (!p)
                l = r = nullptr;
            else if (m_comp(key, p->key)) {
                split_less_equal(p->lchild, key, l, r);
                r = update(raw_copy(p, r, p->rchild));
            } else {
                split_less_equal(p->rchild, key, l, r);
                l = update(raw_copy(p, p->lchild, l));
            }
        }
        void split_by_key(node *p, const _Tp &key, node *&l, node *&mid, node *&r) {
            if (!p)
                l = mid = r = nullptr;
            else if (m_comp(p->key, key)) {
                split_by_key(p->rchild, key, l, mid, r);
                l = update(raw_copy(p, p->lchild, l));
            } else if (m_comp(key, p->key)) {
                split_by_key(p->lchild, key, l, mid, r);
                r = update(raw_copy(p, r, p->rchild));
            } else {
                node *l_mid, *r_mid;
                split_less(p->lchild, key, l, l_mid);
                split_less_equal(p->rchild, key, r_mid, r);
                mid = update(raw_copy(p, l_mid, r_mid));
            }
        }
        void split_by_rank(node *p, int k, node *&l, node *&r) {
            if (!k) {
                l = nullptr;
                r = p;
            } else if (subtree_weight(p->lchild) > k) {
                split_by_rank(p->lchild, k, l, r);
                r = update(raw_copy(p, r, p->rchild));
            } else if (k -= subtree_weight(p->lchild); !k) {
                l = p->lchild;
                r = update(raw_copy(p, nullptr, p->rchild));
            } else {
                split_by_rank(p->rchild, k - 1, l, r);
                l = update(raw_copy(p, p->lchild, l));
            }
        }
        static node *merge(node *l, node *r) {
            if (!l) return r;
            if (!r) return l;
            if (l->priority > r->priority)
                return update(raw_copy(l, l->lchild, merge(l->rchild, r)));
            else
                return update(raw_copy(r, merge(l, r->lchild), r->rchild));
        }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        void roll() { s_timer++; }
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentFHQTreap(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentFHQTreap() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, _Tp __key, Args... __args) {
            node *l, *r;
            split_less_equal(_root(__prevVersion), __key, l, r);
            if constexpr (!_Tag::multi_key) {
                node *p = l;
                if (p)
                    while (p->rchild) p = p->rchild;
                if (p && !m_comp(p->key, __key)) {
                    m_roots.push_back(_root(__prevVersion));
                    s_timer++;
                    return;
                }
            }
            m_roots.push_back(merge(merge(l, new node{__key, __args..., s_rand(), 1, s_timer + 1, nullptr, nullptr}), r));
            s_timer += 2;
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            node *l, *mid, *r;
            split_by_key(_root(__prevVersion), __key, l, mid, r);
            m_roots.push_back(merge(merge(l, new node{__key, __value, s_rand(), 1, s_timer + 1, nullptr, nullptr}), r));
            s_timer += 2;
        }
        bool erase(int __prevVersion, _Tp __key) {
            node *l, *mid, *r;
            split_by_key(_root(__prevVersion), __key, l, mid, r);
            if (!mid)
                m_roots.push_back(merge(l, r));
            else
                m_roots.push_back(mid->subtree_weight > 1 ? merge(merge(l, mid->lchild), merge(mid->rchild, r)) : merge(l, r));
            s_timer++;
            return mid;
        }
        void copyVersion(int __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        int rank(int __version, _Tp __key) {
            node *l, *r;
            split_less(_root(__version), __key, l, r);
            s_timer++;
            return subtree_weight(l);
        }
        const node *kth(int __version, int __k) {
            node *l, *r;
            split_by_rank(_root(__version), __k, l, r);
            node *res = r;
            while (res->lchild) res = res->lchild;
            s_timer++;
            return res;
        }
        const node *find(int __version, _Tp __key) {
            node *l, *mid, *r;
            split_by_key(_root(__version), __key, l, mid, r);
            node *res = mid;
            m_roots.push_back(merge(merge(l, mid), r));
            s_timer++;
            return res;
        }
        const node *lower_bound(int __version, _Tp __key) {
            node *l, *r;
            split_less(_root(__version), __key, l, r);
            node *res = r;
            if (r)
                while (res->lchild) res = res->lchild;
            s_timer++;
            return res;
        }
        const node *upper_bound(int __version, _Tp __key) {
            node *l, *r;
            split_less_equal(_root(__version), __key, l, r);
            node *res = r;
            if (r)
                while (res->lchild) res = res->lchild;
            s_timer++;
            return res;
        }
        const node *smaller_bound(int __version, _Tp __key) {
            node *l, *r;
            split_less(_root(__version), __key, l, r);
            node *res = l;
            if (l)
                while (res->rchild) res = res->rchild;
            s_timer++;
            return res;
        }
        int size(int __version) const { return subtree_weight(_root(__version)); }
        bool empty(int __version) const { return !size(__version); }
        int count(int __version, _Tp __key) {
            node *l, *mid, *r;
            split_by_key(_root(__version), __key, l, mid, r);
            s_timer++;
            return subtree_weight(mid);
        }
        int versionCount() const { return m_roots.size(); }
    };
    namespace PersistentFHQTreapContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentFHQTreap<_Tp, bool, _Compare, PersistentFHQTreapSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentFHQTreap<_Tp, bool, _Compare, PersistentFHQTreapMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentFHQTreap<_Tp, _Fp, _Compare, PersistentFHQTreapMapTag>;
    }
}

#endif