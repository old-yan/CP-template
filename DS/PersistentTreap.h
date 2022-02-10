#ifndef __OY_PERSISTENTTREAP__
#define __OY_PERSISTENTTREAP__

#include <functional>
#include <random>
#include "MemoryPool.h"

namespace OY {
    struct PersistentTreapSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentTreapMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentTreapMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentTreapNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentTreapNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentTreapMultisetTag>
    class PersistentTreap {
#pragma pack(4)
        struct node : _PersistentTreapNode<_Tp, _Fp, _Tag::is_map> {
            uint_fast32_t priority;
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        static inline std::mt19937 s_rand;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static uint_fast32_t subtree_priority(node *p) { return p ? p->priority : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, p->priority, p->subtree_weight, l, r};
            else
                return new node{p->key, p->priority, p->subtree_weight, l, r};
        }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            return p;
        }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentTreap(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentTreap() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, _Tp __key, Args... __args) {
            bool res = _Tag::multi_key;
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur) {
                    res = true;
                    return new node{__key, __args..., s_rand(), 1, nullptr, nullptr};
                } else if (m_comp(__key, cur->key)) {
                    node *l = self(self, cur->lchild);
                    if (!res) return cur;
                    if (subtree_priority(l) > cur->priority) {
                        l->rchild = update(raw_copy(cur, l->rchild, cur->rchild));
                        return update(l);
                    } else {
                        cur = raw_copy(cur, l, cur->rchild);
                        cur->subtree_weight++;
                        return cur;
                    }
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur;
                    node *r = self(self, cur->rchild);
                    if (!res) return cur;
                    if (subtree_priority(r) > cur->priority) {
                        r->lchild = update(raw_copy(cur, cur->lchild, r->lchild));
                        return update(r);
                    } else {
                        cur = raw_copy(cur, cur->lchild, r);
                        cur->subtree_weight++;
                        return cur;
                    }
                }
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            auto dfs = [&](auto self, node *cur) -> node * {
                if (!cur)
                    return new node{__key, __value, s_rand(), 1, nullptr, nullptr};
                else if (m_comp(__key, cur->key)) {
                    node *l = self(self, cur->lchild);
                    if (subtree_priority(l) > cur->priority) {
                        l->rchild = update(raw_copy(cur, l->rchild, cur->rchild));
                        return update(l);
                    } else {
                        cur = raw_copy(cur, l, cur->rchild);
                        cur->subtree_weight++;
                        return cur;
                    }
                } else if (m_comp(cur->key, __key)) {
                    node *r = self(self, cur->rchild);
                    if (subtree_priority(r) > cur->priority) {
                        r->lchild = update(raw_copy(cur, cur->lchild, r->lchild));
                        return update(r);
                    } else {
                        cur = raw_copy(cur, cur->lchild, r);
                        cur->subtree_weight++;
                        return cur;
                    }
                } else {
                    node *p = raw_copy(cur, cur->lchild, cur->rchild);
                    p->value = __value;
                    return p;
                }
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
                    if (node *l = cur->lchild, *r = cur->rchild; !l)
                        return r;
                    else if (!r)
                        return l;
                    else if (l->subtree_weight > r->subtree_weight)
                        return update(raw_copy(l, l->lchild, self(self, raw_copy(cur, l->rchild, r))));
                    else
                        return update(raw_copy(r, self(self, raw_copy(cur, l, r->lchild)), r->rchild));
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
    namespace PersistentTreapContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentTreap<_Tp, bool, _Compare, PersistentTreapSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentTreap<_Tp, bool, _Compare, PersistentTreapMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentTreap<_Tp, _Fp, _Compare, PersistentTreapMapTag>;
    }
}

#endif