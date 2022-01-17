#ifndef __OY_LLRBTREE__
#define __OY_LLRBTREE__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct LLRBSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct LLRBMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct LLRBMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, typename _Tag>
    struct _LLRBNode;
    template <typename _Tp, typename _Fp>
    struct _LLRBNode<_Tp, _Fp, LLRBSetTag> { _Tp key; };
    template <typename _Tp, typename _Fp>
    struct _LLRBNode<_Tp, _Fp, LLRBMultisetTag> {
        _Tp key;
        int _node_weight;
    };
    template <typename _Tp, typename _Fp>
    struct _LLRBNode<_Tp, _Fp, LLRBMapTag> {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = LLRBMultisetTag>
    class LLRB {
        struct node : _LLRBNode<_Tp, _Fp, _Tag> {
            int subtree_weight;
            bool node_color;
            node *lchild;
            node *rchild;
            constexpr int node_weight() {
                if constexpr (_Tag::multi_key)
                    return this->_node_weight;
                else
                    return 1;
            }
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
        node *m_root;
        _Compare m_comp;
        static int node_weight(node *p) { return p ? p->node_weight() : 0; }
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static node *update(node *p) {
            p->subtree_weight = p->node_weight() + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            return p;
        }
        static bool is_red(const node *p) { return p && p->node_color; }
        static bool neg_node_color(bool b) { return !b; }
        static node *lrotate(node *p) {
            node *q = p->rchild;
            p->rchild = q->lchild;
            q->lchild = update(p);
            q->node_color = p->node_color;
            p->node_color = true;
            return q;
        }
        static node *rrotate(node *p) {
            node *q = p->lchild;
            p->lchild = q->rchild;
            q->rchild = update(p);
            q->node_color = p->node_color;
            p->node_color = true;
            return q;
        }
        static node *rrotate_flip(node *p) {
            node *q = p->lchild;
            p->lchild = q->rchild;
            p->rchild->node_color = true;
            p->node_color = false;
            q->rchild = update(p);
            q->node_color = true;
            q->lchild->node_color = false;
            return q;
        }
        static node *rlrotate_flip(node *p) {
            node *q = p->rchild;
            node *r = q->lchild;
            q->lchild = r->rchild;
            p->rchild = r->lchild;
            p->node_color = false;
            p->lchild->node_color = true;
            r->subtree_weight = p->subtree_weight;
            r->node_color = true;
            r->lchild = p;
            r->rchild = update(q);
            return r;
        }
        static node *node_color_flip(node *p) {
            p->node_color = neg_node_color(p->node_color);
            p->lchild->node_color = neg_node_color(p->lchild->node_color);
            p->rchild->node_color = neg_node_color(p->rchild->node_color);
            return p;
        }
        static node *move_red_right(node *cur) {
            if (is_red(cur->lchild->lchild))
                return rrotate_flip(cur);
            else
                return node_color_flip(cur);
        }
        static node *move_red_left(node *cur) {
            if (is_red(cur->rchild->lchild))
                return rlrotate_flip(cur);
            else
                return node_color_flip(cur);
        }
        static node *balance(node *cur) {
            if (is_red(cur->rchild) && !is_red(cur->lchild))
                cur = lrotate(cur);
            if (is_red(cur->lchild) && is_red(cur->lchild->lchild))
                cur = rrotate(cur);
            if (is_red(cur->lchild) && is_red(cur->rchild))
                node_color_flip(cur);
            return update(cur);
        }
        static node *deleteMin(node *cur, node *&res) {
            if (cur->lchild == nullptr) {
                res->key = cur->key;
                if constexpr (_Tag::multi_key)
                    res->_node_weight = cur->_node_weight;
                delete cur;
                return nullptr;
            }
            if (!is_red(cur->lchild) && !is_red(cur->lchild->lchild)) cur = move_red_left(cur);
            cur->lchild = deleteMin(cur->lchild, res);
            return balance(cur);
        }
        template <typename... Args>
        node *_insert(node *cur, _Tp key, Args... args) {
            if (cur == nullptr) return new node{key, args..., 1, true, nullptr, nullptr};
            if (m_comp(key, cur->key))
                cur->lchild = _insert(cur->lchild, key, args...);
            else if (m_comp(cur->key, key))
                cur->rchild = _insert(cur->rchild, key, args...);
            else {
                if constexpr (_Tag::multi_key) {
                    cur->_node_weight++;
                    cur->subtree_weight++;
                }
            }
            return balance(cur);
        }
        node *deleteArbitrary(node *cur, _Tp key) {
            if (m_comp(key, cur->key)) {
                if (!is_red(cur->lchild) && !is_red(cur->lchild->lchild)) cur = move_red_left(cur);
                cur->lchild = deleteArbitrary(cur->lchild, key);
            } else {
                if (is_red(cur->lchild)) cur = rrotate(cur);
                if (!m_comp(cur->key, key) && !cur->rchild) {
                    delete cur;
                    return nullptr;
                }
                if (!is_red(cur->rchild) && !is_red(cur->rchild->lchild)) cur = move_red_right(cur);
                if (m_comp(cur->key, key))
                    cur->rchild = deleteArbitrary(cur->rchild, key);
                else
                    cur->rchild = deleteMin(cur->rchild, cur);
            }
            return balance(cur);
        }
        int decrease(node *cur, _Tp key) {
            if (!cur) return 0;
            int old;
            if (m_comp(key, cur->key))
                old = decrease(cur->lchild, key);
            else if (m_comp(cur->key, key))
                old = decrease(cur->rchild, key);
            else {
                old = cur->node_weight();
                if constexpr (_Tag::multi_key) cur->_node_weight--;
            }
            if (old) cur->subtree_weight--;
            return old;
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        LLRB(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
        ~LLRB() { clear(); }
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
        void insert(Args... __args) {
            if constexpr (_Tag::multi_key)
                m_root = _insert(m_root, __args..., 1);
            else
                m_root = _insert(m_root, __args...);
            m_root->node_color = false;
        }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key))
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) {
            if (int old = decrease(m_root, __key); old == 1) {
                if (!is_red(m_root->lchild) && !is_red(m_root->rchild)) m_root->node_color = true;
                if (m_root = deleteArbitrary(m_root, __key)) m_root->node_color = false;
                return true;
            } else
                return old;
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
                        ord += cur->node_weight();
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
                else if (__k -= subtree_weight(cur->lchild) + cur->node_weight(); __k >= 0)
                    cur = cur->rchild;
            return cur;
        }
        const node *find(_Tp __key) const {
            node *x = m_root;
            while (x != nullptr) {
                if (__key == x->key) return x;
                if (m_comp(__key, x->key))
                    x = x->lchild;
                else
                    x = x->rchild;
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
        int count(_Tp __key) const { return node_weight(find(__key)); }
    };
    namespace LLRBContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = LLRB<_Tp, bool, _Compare, LLRBSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = LLRB<_Tp, bool, _Compare, LLRBMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = LLRB<_Tp, _Fp, _Compare, LLRBMapTag>;
    }
}

#endif