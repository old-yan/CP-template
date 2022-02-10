#ifndef __OY_SPLAY__
#define __OY_SPLAY__

#include "DS/MemoryPool.h"
#include <functional>

namespace OY {
    struct SplaySetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct SplayMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct SplayMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _SplayNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _SplayNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = SplayMultisetTag>
    class Splay {
        struct node : _SplayNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
        node *m_root;
        _Compare m_comp;
        int m_state;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static node *update(node *p) {
            if (p) p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            return p;
        }
        static node *rrotate(node *p) {
            node *q = p->lchild;
            p->lchild = q->rchild;
            q->rchild = update(p);
            return q;
        }
        static node *lrotate(node *p) {
            node *q = p->rchild;
            p->rchild = q->lchild;
            q->lchild = update(p);
            return q;
        }
        static node *rrrotate(node *p) {
            node *q = p->lchild;
            node *s = q->lchild;
            p->lchild = q->rchild;
            q->rchild = update(p);
            q->lchild = s->rchild;
            s->rchild = update(q);
            return s;
        }
        static node *llrotate(node *p) {
            node *q = p->rchild;
            node *s = q->rchild;
            p->rchild = q->lchild;
            q->lchild = update(p);
            q->rchild = s->lchild;
            s->lchild = update(q);
            return s;
        }
        static node *lrrotate(node *p) {
            node *q = p->lchild;
            node *r = q->rchild;
            q->rchild = r->lchild;
            p->lchild = r->rchild;
            r->lchild = update(q);
            r->rchild = update(p);
            return r;
        }
        static node *rlrotate(node *p) {
            node *q = p->rchild;
            node *r = q->lchild;
            q->lchild = r->rchild;
            p->rchild = r->lchild;
            r->rchild = update(q);
            r->lchild = update(p);
            return r;
        }
        node *update_from_lchild(node *cur, node *root) {
            m_state = m_state * 2 + 1;
            if (m_state == 3) {
                m_state = 0;
                return rrrotate(cur);
            } else if (m_state == 5) {
                m_state = 0;
                return lrrotate(cur);
            } else if (m_state == 1 && cur == root)
                return rrotate(cur);
            else
                return cur;
        }
        node *update_from_rchild(node *cur, node *root) {
            m_state = m_state * 2 + 2;
            if (m_state == 4) {
                m_state = 0;
                return rlrotate(cur);
            } else if (m_state == 6) {
                m_state = 0;
                return llrotate(cur);
            } else if (m_state == 2 && cur == root)
                return lrotate(cur);
            else
                return cur;
        }
        node *splay_max(node *cur, node *const &root) {
            if (cur->rchild) {
                cur->rchild = splay_max(cur->rchild, root);
                m_state++;
                if (m_state == 2) {
                    m_state = 0;
                    return llrotate(cur);
                } else if (m_state == 1 && cur == root)
                    return lrotate(cur);
            } else
                m_state = 0;
            return cur;
        }
        node *splay_kth(node *cur, node *const &root, int k) {
            if (!cur) {
                m_state = -1;
                return nullptr;
            } else if (k < subtree_weight(cur->lchild)) {
                cur->lchild = splay_kth(cur->lchild, root, k);
                return update_from_lchild(cur, root);
            } else if (k -= subtree_weight(cur->lchild); k--) {
                cur->rchild = splay_kth(cur->rchild, root, k);
                return update_from_rchild(cur, root);
            } else {
                m_state = 0;
                return cur;
            }
        }
        template <typename... Args>
        node *splay_insert(node *cur, node *const &root, _Tp key, Args... args) {
            if (!cur) {
                m_state = 0;
                return new node{key, args..., 1, nullptr, nullptr};
            } else if (m_comp(key, cur->key)) {
                cur->lchild = splay_insert(cur->lchild, root, key, args...);
                return update_from_lchild(cur, root);
            } else {
                if constexpr (!_Tag::multi_key) {
                    if (!m_comp(cur->key, key)) {
                        m_state = -1;
                        return cur;
                    }
                }
                cur->rchild = splay_insert(cur->rchild, root, key, args...);
                return update_from_rchild(cur, root);
            }
        }
        node *splay_find(node *cur, node *const &root, const _Tp &key) {
            if (!cur) {
                m_state = -1;
                return nullptr;
            } else if (m_comp(key, cur->key)) {
                cur->lchild = splay_find(cur->lchild, root, key);
                return update_from_lchild(cur, root);
            } else if (m_comp(cur->key, key)) {
                cur->rchild = splay_find(cur->rchild, root, key);
                return ~m_state ? update_from_rchild(cur, root) : cur;
            } else {
                m_state = 0;
                return cur;
            }
        }
        node *splay_smaller_bound(node *cur, node *const &root, const _Tp &key) {
            if (!cur) {
                m_state = -1;
                return nullptr;
            } else if (m_comp(cur->key, key)) {
                cur->rchild = splay_smaller_bound(cur->rchild, root, key);
                return update_from_rchild(cur, root);
            } else {
                cur->lchild = splay_smaller_bound(cur->lchild, root, key);
                return update_from_lchild(cur, root);
            }
        }
        node *splay_upper_bound(node *cur, node *const &root, const _Tp &key) {
            if (!cur) {
                m_state = -1;
                return nullptr;
            } else if (m_comp(key, cur->key)) {
                cur->lchild = splay_upper_bound(cur->lchild, root, key);
                if (m_state < 0) {
                    m_state = 0;
                    return cur;
                }
                return update_from_lchild(cur, root);
            } else {
                cur->rchild = splay_upper_bound(cur->rchild, root, key);
                return ~m_state ? update_from_rchild(cur, root) : cur;
            }
        }
        node *splay_lower_bound(node *cur, node *const &root, const _Tp &key) {
            if (!cur) {
                m_state = -1;
                return nullptr;
            } else if (!m_comp(cur->key, key)) {
                cur->lchild = splay_lower_bound(cur->lchild, root, key);
                if (m_state < 0) {
                    m_state = 0;
                    return cur;
                }
                return update_from_lchild(cur, root);
            } else {
                cur->rchild = splay_lower_bound(cur->rchild, root, key);
                return ~m_state ? update_from_rchild(cur, root) : cur;
            }
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        Splay(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
        ~Splay() { clear(); }
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
        void insert(Args... __args) { m_root = update(splay_insert(m_root, m_root, __args...)); }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key))
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) {
            m_root = update(splay_find(m_root, m_root, __key));
            if (!~m_state) return false;
            delete m_root;
            node *r = m_root->rchild;
            if (m_root->lchild) {
                m_root = update(splay_max(m_root->lchild, m_root->lchild));
                if (r) {
                    m_root->rchild = r;
                    update(m_root);
                }
            } else
                m_root = r;
            return true;
        }
        void erase(_Tp __key, int __count) {
            static_assert(_Tag::multi_key);
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) {
            m_root = update(splay_lower_bound(m_root, m_root, __key));
            return ~m_state ? subtree_weight(m_root->lchild) : subtree_weight(m_root);
        }
        const node *kth(int __k) {
            m_root = update(splay_kth(m_root, m_root, __k));
            return ~m_state ? m_root : nullptr;
        }
        const node *find(_Tp __key) {
            m_root = update(splay_find(m_root, m_root, __key));
            return ~m_state ? m_root : nullptr;
        }
        const node *lower_bound(_Tp __key) {
            m_root = update(splay_lower_bound(m_root, m_root, __key));
            return ~m_state ? m_root : nullptr;
        }
        const node *upper_bound(_Tp __key) {
            m_root = update(splay_upper_bound(m_root, m_root, __key));
            return ~m_state ? m_root : nullptr;
        }
        const node *smaller_bound(_Tp __key) {
            m_root = update(splay_smaller_bound(m_root, m_root, __key));
            return ~m_state ? m_root : nullptr;
        }
        int size() const { return subtree_weight(m_root); }
        bool empty() const { return !size(); }
        int count(_Tp __key) {
            if (!(m_root = update(lower_bound(m_root, m_root, __key)))) return 0;
            int smaller = subtree_weight(m_root->lchild);
            if (!(m_root = update(upper_bound(m_root, m_root, __key))))
                return size() - smaller;
            else
                return subtree_weight(m_root->lchild) - smaller;
        }
    };
    namespace SplayContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = Splay<_Tp, bool, _Compare, SplaySetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = Splay<_Tp, bool, _Compare, SplayMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = Splay<_Tp, _Fp, _Compare, SplayMapTag>;
    }
}

#endif