#ifndef __OY_PERSISTENTSEGTREE__
#define __OY_PERSISTENTSEGTREE__

#include <cstdint>
#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    class PersistentSegTree {
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
        std::vector<_TpNode *> m_roots;
        int m_length;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            //assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        _TpNode *lchild(_TpNode *cur) {
            if (!cur->lchild)
                cur->lchild = new _TpNode(m_defaultValue, nullptr, nullptr);
            return cur->lchild;
        }
        _TpNode *rchild(_TpNode *cur) {
            if (!cur->rchild)
                cur->rchild = new _TpNode(m_defaultValue, nullptr, nullptr);
            return cur->rchild;
        }
        _TpNode *_update(_TpNode *cur) {
            cur->val = m_op(cur->lchild ? cur->lchild->val : m_defaultValue, cur->rchild ? cur->rchild->val : m_defaultValue);
            return cur;
        }
        _TpNode *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }
        void _clear() { m_roots.clear(); }

    public:
        static void setBufferSize(int __count) { MemoryPool<_TpNode>::_reserve(__count); }
        PersistentSegTree(int __n = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        PersistentSegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(int __n) {
            _clear();
            if (m_length = __n) m_roots.push_back(new _TpNode(m_defaultValue, nullptr, nullptr));
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            _clear();
            m_length = __last - __first;
            auto dfs = [&](auto self, _Iterator first, _Iterator last) -> _TpNode * {
                if (first + 1 == last)
                    return new _TpNode(*first, nullptr, nullptr);
                else
                    return _update(new _TpNode(*first, self(self, first, first + (last - first + 1) / 2), self(self, first + (last - first + 1) / 2, last)));
            };
            m_roots.push_back(dfs(dfs, __first, __last));
        }
        void copyVersion(int __prevVersion) {
            m_roots.push_back(_root(__prevVersion));
        }
        void update(int __prevVersion, int __i, _Tp __val) {
            auto dfs = [&](auto self, _TpNode *prev, int left, int right) -> _TpNode * {
                _TpNode *cur = prev ? new _TpNode(*prev) : new _TpNode(m_defaultValue, nullptr, nullptr);
                if (left == right)
                    cur->val = __val;
                else {
                    if (__i <= (left + right) / 2)
                        cur->lchild = self(self, cur->lchild, left, (left + right) / 2);
                    else
                        cur->rchild = self(self, cur->rchild, (left + right) / 2 + 1, right);
                    _update(cur);
                }
                return cur;
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion), 0, m_length - 1));
        }
        void add(int __prevVersion, int __i, _Tp __inc) {
            auto dfs = [&](auto self, _TpNode *prev, int left, int right) -> _TpNode * {
                _TpNode *cur = prev ? new _TpNode(*prev) : new _TpNode(m_defaultValue, nullptr, nullptr);
                if (left == right)
                    cur->val = m_op(cur->val, __inc);
                else {
                    if (__i <= (left + right) / 2)
                        cur->lchild = self(self, cur->lchild, left, (left + right) / 2);
                    else
                        cur->rchild = self(self, cur->rchild, (left + right) / 2 + 1, right);
                    _update(cur);
                }
                return cur;
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion), 0, m_length - 1));
        }
        _Tp query(int __version, int __i) const {
            auto dfs = [&](auto self, _TpNode *cur, int left, int right) {
                if (left == right)
                    return cur->val;
                else if (__i <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
            };
            return dfs(dfs, _root(__version), 0, m_length - 1);
        }
        _Tp query(int __version, int __left, int __right) const {
            auto dfs = [&](auto self, _TpNode *cur, int left, int right) {
                if (left >= __left && right <= __right)
                    return cur->val;
                else if (__right <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else if (__left > (left + right) / 2)
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
                else
                    return m_op(cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue, cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1);
        }
        _Tp queryAll(int __version) const {
            return _root(__version)->val;
        }
        int kth(int __version, _Tp __k) const {
            auto dfs = [&](auto self, _TpNode *cur, int left, int right, int k) {
                if (left == right) return left;
                if (cur->lchild) {
                    if (cur->lchild->val > k)
                        return self(self, cur->lchild, left, (left + right) / 2, k);
                    else
                        return self(self, cur->rchild, (left + right) / 2 + 1, right, k - cur->lchild->val);
                } else
                    return self(self, cur->rchild, (left + right) / 2 + 1, right, k);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1, __k);
        }
        _Tp periodQuery(int __leftVersion, int __rightVersion, int __i) const {
            if (__leftVersion == 0) return query(__rightVersion, __i);
            auto dfs = [&](auto self, _TpNode *root1, _TpNode *root2, int left, int right) {
                if (root1 == root2) return 0;
                if (left == right)
                    return root1 ? root2->val - root1->val : root2->val;
                else if (__i <= (left + right) / 2)
                    return self(self, root1 ? root1->lchild : nullptr, root2->lchild, left, (left + right) / 2);
                else
                    return self(self, root1 ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1);
        }
        _Tp periodQuery(int __leftVersion, int __rightVersion, int __left, int __right) const {
            if (__leftVersion == 0) return query(__rightVersion, __left, __right);
            auto dfs = [&](auto self, _TpNode *root1, _TpNode *root2, int left, int right) -> _Tp {
                if (root1 == root2) return 0;
                if (left >= __left && right <= __right)
                    return root1 ? root2->val - root1->val : root2->val;
                else if (__right <= (left + right) / 2)
                    return self(self, root1 ? root1->lchild : nullptr, root2->lchild, left, (left + right) / 2);
                else if (__left > (left + right) / 2)
                    return self(self, root1 ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right);
                else
                    return self(self, root1 ? root1->lchild : nullptr, root2->lchild, left, (left + right) / 2) + self(self, root1 ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1);
        }
        int periodKth(int __leftVersion, int __rightVersion, _Tp __k) const {
            if (__leftVersion == 0) return kth(__rightVersion, __k);
            auto dfs = [&](auto self, _TpNode *root1, _TpNode *root2, int left, int right, int k) {
                if (left == right) return left;
                if (!root1 || !root1->lchild) {
                    if (!root2->lchild)
                        return self(self, root1 && root1->rchild ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, k);
                    else if (root2->lchild->val > k)
                        return self(self, nullptr, root2->lchild, left, (left + right) / 2, k);
                    else
                        return self(self, root1 && root1->rchild ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, k - root2->lchild->val);
                } else if (root2->lchild->val - root1->lchild->val > k)
                    return self(self, root1->lchild, root2->lchild, left, (left + right) / 2, k);
                else
                    return self(self, root1->rchild, root2->rchild, (left + right) / 2 + 1, right, k - root2->lchild->val + root1->lchild->val);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, __k);
        }
        int versionCount() const { return m_roots.size(); }
    };
    template <typename _Tp = int64_t>
    PersistentSegTree(int, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> PersistentSegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int64_t>
    PersistentSegTree(int, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    PersistentSegTree(int = 0, _Operation = _Operation(), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    PersistentSegTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> PersistentSegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    PersistentSegTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>>
    PersistentSegTree(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Operation>;
}

#endif