#ifndef __OY_PERSISTENTAVL__
#define __OY_PERSISTENTAVL__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct PersistentAVLSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentAVLMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentAVLMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentAVLNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentAVLNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentAVLMultisetTag>
    class PersistentAVL {
#pragma pack(4)
        struct node : _PersistentAVLNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            int subtree_height;
            unsigned int time_stamp;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        static inline unsigned int s_timer = 0;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static int subtree_height(node *p) { return p ? p->subtree_height : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if (p->time_stamp == s_timer) {
                p->lchild = l;
                p->rchild = r;
                return p;
            }
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, 1, 1, s_timer, l, r};
            else
                return new node{p->key, 1, 1, s_timer, l, r};
        }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            p->subtree_height = 1 + std::max(subtree_height(p->lchild), subtree_height(p->rchild));
            return p;
        }
        static node *balance(node *cur) {
            if (int lh = subtree_height(cur->lchild), rh = subtree_height(cur->rchild); lh > rh + 1) {
                if (node *l = cur->lchild, *lr = l->rchild; subtree_height(lr) > rh)
                    return update(raw_copy(lr, update(raw_copy(l, l->lchild, lr->lchild)), update(raw_copy(cur, lr->rchild, cur->rchild))));
                else
                    return update(raw_copy(l, l->lchild, update(raw_copy(cur, l->rchild, cur->rchild))));
            } else if (rh > lh + 1) {
                if (node *r = cur->rchild, *rl = r->lchild; subtree_height(rl) > lh)
                    return update(raw_copy(rl, update(raw_copy(cur, cur->lchild, rl->lchild)), update(raw_copy(r, rl->rchild, r->rchild))));
                else
                    return update(raw_copy(r, update(raw_copy(cur, cur->lchild, r->lchild)), r->rchild));
            } else
                return update(cur);
        }
        static node *deleteMin(node *cur, node *&res) {
            if (!cur->lchild) {
                res = cur;
                return cur->rchild;
            } else
                return balance(raw_copy(cur, deleteMin(cur->lchild, res), cur->rchild));
        }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentAVL(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentAVL() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, _Tp __key, Args... __args) {
            bool res = _Tag::multi_key;
            auto dfs = [&](auto self, node *cur) {
                if (!cur) {
                    res = true;
                    return new node{__key, __args..., 1, 1, s_timer + 1, nullptr, nullptr};
                } else if (m_comp(__key, cur->key)) {
                    node *l = self(self, cur->lchild);
                    return res ? balance(raw_copy(cur, l, cur->rchild)) : cur;
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur;
                    node *r = self(self, cur->rchild);
                    return res ? balance(raw_copy(cur, cur->lchild, r)) : cur;
                };
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
            s_timer += 2;
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            auto dfs = [&](auto self, node *cur) {
                if (!cur)
                    return new node{__key, __value, 1, 1, s_timer + 1, nullptr, nullptr};
                else if (m_comp(__key, cur->key))
                    return balance(raw_copy(cur, self(self, cur->lchild), cur->rchild));
                else if (m_comp(cur->key, __key))
                    return balance(raw_copy(cur, cur->lchild, self(self, cur->rchild)));
                else {
                    node *p = raw_copy(cur, cur->lchild, cur->rchild);
                    p->value = __value;
                    return p;
                };
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
            s_timer += 2;
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
                } else {
                    res = true;
                    if (!cur->rchild)
                        return cur->lchild;
                    node *res, *r = deleteMin(cur->rchild, res);
                    return update(raw_copy(res, cur->lchild, r));
                }
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion)));
            s_timer++;
            return res;
        }
        void copyVersion(int __version) { m_roots.push_back(_root(__version)); }
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
    namespace PersistentAVLContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentAVL<_Tp, bool, _Compare, PersistentAVLSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentAVL<_Tp, bool, _Compare, PersistentAVLMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentAVL<_Tp, _Fp, _Compare, PersistentAVLMapTag>;
    }
}

#endif