#ifndef __OY_SIZEBALANCEDZKWTREE__
#define __OY_SIZEBALANCEDZKWTREE__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct SizeBalancedZkwTreeSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct SizeBalancedZkwTreeMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct SizeBalancedZkwTreeMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _SizeBalancedZkwTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _SizeBalancedZkwTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = bool, typename _Compare = std::less<_Tp>, typename _Tag = SizeBalancedZkwTreeMultisetTag>
    class SizeBalancedZkwTree {
#pragma pack(4)
        struct node : _SizeBalancedZkwTreeNode<_Tp, _Fp, _Tag::is_map> {
            int subtree_weight;
            node *lchild;
            node *rchild;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::recycle((node *)p); }
        };
#pragma pack()
        std::vector<node *> m_sub;
        _Compare m_comp;
        int m_length;
        int m_depth;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static int lchild_weight(node *p) { return p && p->lchild ? p->lchild->subtree_weight : 0; }
        static int rchild_weight(node *p) { return p && p->rchild ? p->rchild->subtree_weight : 0; }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
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
        static node *lbalance(node *cur) {
            if (!cur) return cur;
            if (subtree_weight(cur->lchild) < lchild_weight(cur->rchild))
                cur = rlrotate(cur);
            else if (subtree_weight(cur->lchild) < rchild_weight(cur->rchild))
                cur = lrotate(cur);
            else
                return update(cur);
            cur->lchild = rbalance(cur->lchild);
            cur->rchild = lbalance(cur->rchild);
            return rbalance(lbalance(cur));
        }
        static node *rbalance(node *cur) {
            if (!cur) return cur;
            if (subtree_weight(cur->rchild) < rchild_weight(cur->lchild))
                cur = lrrotate(cur);
            else if (subtree_weight(cur->rchild) < lchild_weight(cur->lchild))
                cur = rrotate(cur);
            else
                return update(cur);
            cur->lchild = rbalance(cur->lchild);
            cur->rchild = lbalance(cur->rchild);
            return rbalance(lbalance(cur));
        }
        static node *deleteMin(node *cur, node *&res) {
            if (!cur) return nullptr;
            if (!cur->lchild) {
                res = cur;
                return cur->rchild;
            } else {
                cur->lchild = deleteMin(cur->lchild, res);
                return update(cur);
            }
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        SizeBalancedZkwTree(int __n = 0, _Compare __comp = _Compare()) : m_comp(__comp) { resize(__n); }
        template <typename _Iterator>
        SizeBalancedZkwTree(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_comp(__comp) { reset(__first, __last); }
        ~SizeBalancedZkwTree() { clear(); }
        void resize(int __n) {
            clear();
            if (!__n) return;
            m_length = __n;
            m_depth = 32 - (m_length > 1 ? __builtin_clz(m_length - 1) : 32);
            m_sub.assign(1 << (m_depth + 1), nullptr);
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            static_assert(!_Tag::is_map);
            clear();
            auto makenode = [&](auto self, auto first, auto last) -> node * {
                if (first == last) return nullptr;
                auto mid = first + (last - first) / 2;
                return new node{*mid, int(last - first), self(self, first, mid), self(self, mid + 1, last)};
            };
            std::vector<_Tp> data(__first, __last);
            m_length = data.size();
            m_depth = 32 - (m_length > 1 ? __builtin_clz(m_length - 1) : 32);
            m_sub.assign(1 << (m_depth + 1), nullptr);
            for (int d = m_depth, len = 1; d >= 0; d--, len <<= 1) {
                int it = 0;
                int i = 1 << d, o = m_length - len;
                for (; it <= o; i++, it += len) {
                    if (len > 1) std::inplace_merge(data.data() + it, data.data() + it + len / 2, data.data() + it + len);
                    m_sub[i] = makenode(makenode, data.data() + it, data.data() + it + len);
                }
                if (len > 1 and m_length - it > len / 2) std::inplace_merge(data.data() + it, data.data() + it + len / 2, data.data() + m_length);
                m_sub[i] = makenode(makenode, data.data() + it, data.data() + m_length);
            }
        }
        void clear() {
            // auto dfs = [&](auto self, node *cur) -> void {
            //     if (cur->lchild) self(self, cur->lchild);
            //     if (cur->rchild) self(self, cur->rchild);
            //     delete cur;
            // };
            // for (node *root : m_sub)
            //     if (root) dfs(dfs, root);
            m_sub.clear();
        }
        template <typename... Args>
        void insert(int __i, _Tp __key, Args... __args) {
            for (int i = __i + (1 << m_depth); i; i >>= 1) m_sub[i] = insert(m_sub[i], __key, __args...);
        }
        void update(int __i, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            for (int i = __i + (1 << m_depth); i; i >>= 1) m_sub[i] = update(m_sub[i], __key, __value);
        }
        void erase(int __i, _Tp __key) {
            for (int i = __i + (1 << m_depth); i; i >>= 1) m_sub[i] = erase(m_sub[i], __key);
        }
        int rank(int __left, int __right, _Tp __key) const {
            int sum = 0;
            for (auto root : find_roots(__left, __right)) sum += rank(root, __key);
            return sum;
        }
        node *kth(int __left, int __right, int __k, _Tp __min, _Tp __max) const {
            std::vector<node *> roots = find_roots(__left, __right);
            int len = __max - __min;
            while (len) {
                int half = len + 1 >> 1;
                auto mid = __min + half;
                int sum = 0;
                for (node *root : roots) sum += rank(root, mid);
                if (sum > __k) {
                    __max = mid - 1;
                    len = half - 1;
                } else {
                    __min = mid;
                    len -= half;
                }
            }
            for (node *root : roots)
                if (auto it = find(root, __min)) return it;
            return nullptr;
        }
        node *lower_bound(int __left, int __right, _Tp __key) const {
            node *res = nullptr;
            for (auto root : find_roots(__left, __right))
                if (auto it = lower_bound(root, __key); it && (!res || m_comp(it->key, res->key))) res = it;
            return res;
        }
        node *upper_bound(int __left, int __right, _Tp __key) const {
            node *res = nullptr;
            for (auto root : find_roots(__left, __right))
                if (auto it = upper_bound(root, __key); it && (!res || m_comp(it->key, res->key))) res = it;
            return res;
        }
        node *smaller_bound(int __left, int __right, _Tp __key) const {
            node *res = nullptr;
            for (auto root : find_roots(__left, __right))
                if (auto it = smaller_bound(root, __key); it && (!res || m_comp(res->key, it->key))) res = it;
            return res;
        }
        int size(int __left, int __right) const {
            int sum = 0;
            for (auto root : find_roots(__left, __right)) sum += size(root);
            return sum;
        }
        bool empty(int __left, int __right) const {
            for (auto root : find_roots(__left, __right))
                if (size(root)) return false;
            return true;
        }
        int count(int __left, int __right, _Tp __key) const {
            int sum = 0;
            for (auto root : find_roots(__left, __right)) sum += count(root, __key);
            return sum;
        }
        std::vector<node *> find_roots(int __left, int __right) const {
            static std::vector<node *> res;
            res.clear();
            __left += 1 << m_depth;
            __right += 1 << m_depth;
            if (__left < __right) {
                if (m_sub[__left]) res.push_back(m_sub[__left]);
                int j = 31 - __builtin_clz(__left ^ __right);
                for (int i = 0; i < j; i++)
                    if (!(__left >> i & 1) && m_sub[__left >> i ^ 1]) res.push_back(m_sub[__left >> i ^ 1]);
                for (int i = j - 1; i >= 0; i--)
                    if ((__right >> i & 1) && m_sub[__right >> i ^ 1]) res.push_back(m_sub[__right >> i ^ 1]);
                if (m_sub[__right]) res.push_back(m_sub[__right]);
            } else if (m_sub[__left])
                res.push_back(m_sub[__left]);
            return res;
        }
        template <typename... Args>
        node *insert(node *__root, _Tp __key, Args... __args) {
            bool res = false;
            auto dfs = [&](auto self, node *cur) {
                if (!cur) {
                    res = true;
                    return new node{__key, __args..., 1, nullptr, nullptr};
                } else if (m_comp(__key, cur->key)) {
                    cur->lchild = self(self, cur->lchild);
                    return _Tag::multi_key || res ? rbalance(cur) : cur;
                } else {
                    if constexpr (!_Tag::multi_key)
                        if (!m_comp(cur->key, __key)) return cur;
                    cur->rchild = self(self, cur->rchild);
                    return _Tag::multi_key || res ? lbalance(cur) : cur;
                };
            };
            return dfs(dfs, __root);
        }
        node *update(node *__root, _Tp __key, _Fp __value) {
            if (auto p = find(__root, __key))
                p->value = __value;
            else
                __root = insert(__root, __key, __value);
            return __root;
        }
        node *erase(node *__cur, _Tp __key) {
            if (!__cur)
                return nullptr;
            else if (m_comp(__key, __cur->key))
                __cur->lchild = erase(__cur->lchild, __key);
            else if (m_comp(__cur->key, __key))
                __cur->rchild = erase(__cur->rchild, __key);
            else if (!__cur->rchild) {
                delete __cur;
                return __cur->lchild;
            } else {
                node *res;
                __cur->rchild = deleteMin(__cur->rchild, res);
                res->lchild = __cur->lchild;
                res->rchild = __cur->rchild;
                delete __cur;
                __cur = res;
            }
            return update(__cur);
        }
        int rank(node *__root, _Tp __key) const {
            int ord = 0;
            for (node *cur = __root; cur;)
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
        node *kth(node *__root, int __k) const {
            node *cur = __root;
            while (__k >= 0)
                if (int l_count = subtree_weight(cur->lchild); __k < l_count)
                    cur = cur->lchild;
                else if (__k -= subtree_weight(cur->lchild) + 1; __k >= 0)
                    cur = cur->rchild;
            return cur;
        }
        node *find(node *__root, _Tp __key) const {
            for (node *cur = __root; cur;) {
                if (m_comp(__key, cur->key))
                    cur = cur->lchild;
                else if (m_comp(cur->key, __key))
                    cur = cur->rchild;
                else
                    return cur;
            }
            return nullptr;
        }
        node *lower_bound(node *__root, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = __root; cur;) {
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
        node *upper_bound(node *__root, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = __root; cur;) {
                if (m_comp(__key, cur->key)) {
                    res = cur;
                    cur = cur->lchild;
                } else
                    cur = cur->rchild;
            }
            return res;
        }
        node *smaller_bound(node *__root, _Tp __key) const {
            node *res = nullptr;
            for (node *cur = __root; cur;) {
                if (m_comp(cur->key, __key)) {
                    res = cur;
                    cur = cur->rchild;
                } else
                    cur = cur->lchild;
            }
            return res;
        }
        int size(node *__root) const { return subtree_weight(__root); }
        bool empty(node *__root) const { return !size(__root); }
        int count(node *__root, _Tp __key) const {
            auto it1 = lower_bound(__root, __key);
            if (!it1) return 0;
            auto it2 = upper_bound(__root, __key);
            if (!it2)
                return size(__root) - rank(__root, __key);
            else
                return rank(__root, it2->key) - rank(__root, __key);
        }
    };
    namespace SizeBalancedZkwTreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = SizeBalancedZkwTree<_Tp, bool, _Compare, SizeBalancedZkwTreeSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = SizeBalancedZkwTree<_Tp, bool, _Compare, SizeBalancedZkwTreeMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = SizeBalancedZkwTree<_Tp, _Fp, _Compare, SizeBalancedZkwTreeMapTag>;
    }
}

#endif