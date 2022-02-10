#ifndef __OY_PERSISTENTSPLAY__
#define __OY_PERSISTENTSPLAY__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct PersistentSplaySetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct PersistentSplayMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct PersistentSplayMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentSplayNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentSplayNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = PersistentSplayMultisetTag>
    class PersistentSplay {
#pragma pack(4)
        struct node : _PersistentSplayNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            unsigned int time_stamp;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
#pragma pack()
        std::vector<node *> m_roots;
        _Compare m_comp;
        int m_state;
        static inline unsigned int s_timer;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static node *raw_copy(node *p, node *l, node *r) {
            if (p->time_stamp == s_timer) return set_lrchild(p, l, r);
            if constexpr (_Tag::is_map)
                return new node{p->key, p->value, 1, s_timer, l, r};
            else
                return new node{p->key, 1, s_timer, l, r};
        }
        static node *update(node *p) {
            if (p) p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
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
        static node *rrotate(node *p) {
            node *l = p->lchild;
            return raw_copy(l, l->lchild, update(raw_copy(p, l->rchild, p->rchild)));
        }
        static node *lrotate(node *p) {
            node *r = p->rchild;
            return raw_copy(r, update(raw_copy(p, p->lchild, r->lchild)), r->rchild);
        }
        static node *rrrotate(node *p) {
            node *l = p->lchild, *ll = l->lchild;
            return raw_copy(ll, ll->lchild, update(raw_copy(l, ll->rchild, update(raw_copy(p, l->rchild, p->rchild)))));
        }
        static node *llrotate(node *p) {
            node *r = p->rchild, *rr = r->rchild;
            return raw_copy(rr, update(raw_copy(r, update(raw_copy(p, p->lchild, r->lchild)), rr->lchild)), rr->rchild);
        }
        static node *lrrotate(node *p) {
            node *l = p->lchild, *lr = l->rchild;
            return raw_copy(lr, update(raw_copy(l, l->lchild, lr->lchild)), update(raw_copy(p, lr->rchild, p->rchild)));
        }
        static node *rlrotate(node *p) {
            node *r = p->rchild, *rl = r->lchild;
            return raw_copy(rl, update(raw_copy(p, p->lchild, rl->lchild)), update(raw_copy(r, rl->rchild, r->rchild)));
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
                else
                    return cur;
            } else {
                m_state = 0;
                return cur;
            }
        }
        node *splay_kth(node *cur, node *const &root, int k) {
            if (k < subtree_weight(cur->lchild)) {
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
                return new node{key, args..., 1, s_timer + 1, nullptr, nullptr};
            } else if (m_comp(key, cur->key)) {
                node *p = splay_insert(cur->lchild, root, key, args...);
                if (m_state < 0) return cur;
                p = raw_copy(cur, p, cur->rchild);
                if (cur == root && !m_state) return rrotate(p);
                return update_from_lchild(p, root);
            } else {
                if constexpr (!_Tag::multi_key) {
                    if (!m_comp(cur->key, key)) {
                        m_state = -1;
                        return cur;
                    }
                }
                node *p = splay_insert(cur->rchild, root, key, args...);
                if (m_state < 0) return cur;
                p = raw_copy(cur, cur->lchild, p);
                if (cur == root && !m_state) return lrotate(p);
                return update_from_rchild(p, root);
            }
        }
        node *splay_update(node *cur, node *const &root, _Tp key, _Fp value) {
            if (!cur) {
                m_state = 0;
                return new node{key, value, 1, s_timer + 1, nullptr, nullptr};
            } else if (m_comp(key, cur->key)) {
                node *p = splay_update(cur->lchild, root, key, value);
                if (m_state < 0) return cur;
                p = raw_copy(cur, p, cur->rchild);
                if (cur == root && !m_state) return rrotate(p);
                return update_from_lchild(p, root);
            } else if (m_comp(cur->key, key)) {
                node *p = splay_update(cur->rchild, root, key, value);
                if (m_state < 0) return cur;
                p = raw_copy(cur, cur->lchild, p);
                if (cur == root && !m_state) return lrotate(p);
                return update_from_rchild(p, root);
            } else {
                m_state = 0;
                node *p = raw_copy(cur, cur->lchild, cur->rchild);
                p->value = value;
                return p;
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
        node *&_root(int version) { return ~version ? m_roots[version] : m_roots.back(); }
        node *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        PersistentSplay(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentSplay() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, Args... __args) {
            m_roots.push_back(update(splay_insert(_root(__prevVersion), _root(__prevVersion), __args...)));
            s_timer += 2;
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            m_roots.push_back(update(splay_update(_root(__prevVersion), _root(__prevVersion), __key, __value)));
            s_timer += 2;
        }
        bool erase(int __prevVersion, _Tp __key) {
            node *root = update(splay_find(_root(__prevVersion), _root(__prevVersion), __key));
            if (~m_state) {
                node *r = root->rchild;
                if (root->lchild) {
                    root = update(splay_max(root->lchild, root->lchild));
                    if (r) root = update(raw_copy(root, root->lchild, r));
                } else
                    root = r;
            }
            m_roots.push_back(root);
            s_timer++;
            return ~m_state;
        }
        void copyVersion(int __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        int rank(int __version, _Tp __key) {
            _root(__version) = update(splay_lower_bound(_root(__version), _root(__version), __key));
            s_timer++;
            return ~m_state ? subtree_weight(_root(__version)->lchild) : subtree_weight(_root(__version));
        }
        const node *kth(int __version, int __k) {
            _root(__version) = update(splay_kth(_root(__version), _root(__version), __k));
            s_timer++;
            return ~m_state ? _root(__version) : nullptr;
        }
        const node *find(int __version, _Tp __key) {
            _root(__version) = update(splay_find(_root(__version), _root(__version), __key));
            s_timer++;
            return ~m_state ? _root(__version) : nullptr;
        }
        const node *lower_bound(int __version, _Tp __key) {
            _root(__version) = update(splay_lower_bound(_root(__version), _root(__version), __key));
            s_timer++;
            return ~m_state ? _root(__version) : nullptr;
        }
        const node *upper_bound(int __version, _Tp __key) {
            _root(__version) = update(splay_upper_bound(_root(__version), _root(__version), __key));
            s_timer++;
            return ~m_state ? _root(__version) : nullptr;
        }
        const node *smaller_bound(int __version, _Tp __key) {
            _root(__version) = update(splay_smaller_bound(_root(__version), _root(__version), __key));
            s_timer++;
            return ~m_state ? _root(__version) : nullptr;
        }
        int size(int __version) const { return subtree_weight(_root(__version)); }
        bool empty(int __version) const { return !size(__version); }
        int count(int __version, _Tp __key) {
            node *root1, *root2;
            if (!(root1 = update(lower_bound(_root(__version), _root(__version), __key)))) return 0;
            int smaller = subtree_weight(root1->lchild);
            if (!(root2 = update(upper_bound(_root(__version), _root(__version), __key)))) {
                s_timer++;
                return size(__version) - smaller;
            } else {
                s_timer++;
                return subtree_weight(root2->lchild) - smaller;
            }
        }
        int versionCount() const { return m_roots.size(); }
    };
    namespace PersistentSplayContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PersistentSplay<_Tp, bool, _Compare, PersistentSplaySetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PersistentSplay<_Tp, bool, _Compare, PersistentSplayMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PersistentSplay<_Tp, _Fp, _Compare, PersistentSplayMapTag>;
    }
}

#endif