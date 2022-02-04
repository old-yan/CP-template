#ifndef __OY_PERSISTENTLAZYSEGTREE__
#define __OY_PERSISTENTLAZYSEGTREE__

#include <cstdint>
#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct PersistentLazySegAdd {
        _Tp operator()(const _Fp &__x, const _Tp &__y, int __size) const { return __x * __size + __y; }
    };
    template <typename _Tp = int64_t, typename _Fp = _Tp, typename _Operation = std::plus<_Tp>, typename _Mapping = OY::PersistentLazySegAdd<_Tp, _Fp>, typename _Composition = std::plus<_Fp>>
    class PersistentLazySegTree {
        struct _Tp_FpNode : MemoryPool<_Tp_FpNode> {
            _Tp val;
            _Fp inc;
            _Tp_FpNode *lchild;
            _Tp_FpNode *rchild;
            _Tp_FpNode(_Tp _val, _Fp _inc, _Tp_FpNode *_lchild, _Tp_FpNode *_rchild) : val(_val), inc(_inc), lchild(_lchild), rchild(_rchild) {}
        };
        std::vector<_Tp_FpNode *> m_roots;
        int m_length;
        _Operation m_op;
        _Mapping m_map;
        _Composition m_com;
        _Tp m_defaultValue;
        _Fp m_defaultIncrement;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue && m_com(m_defaultIncrement, m_defaultIncrement) == m_defaultIncrement);
            // if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, int>) assert(m_map(m_defaultIncrement, m_defaultValue, 1) == m_defaultValue);
            // else assert(m_map(m_defaultIncrement, m_defaultValue) == m_defaultValue);
        }
        _Tp_FpNode *lchild(_Tp_FpNode *cur) {
            if (!cur->lchild)
                cur->lchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            return cur->lchild;
        }
        _Tp_FpNode *rchild(_Tp_FpNode *cur) {
            if (!cur->rchild)
                cur->rchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            return cur->rchild;
        }
        _Tp _map(_Fp inc, _Tp val, int length) const {
            if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, int>)
                return m_map(inc, val, length);
            else
                return m_map(inc, val);
        }
        _Tp_FpNode *_update(_Tp_FpNode *cur, int length) {
            cur->val = _map(cur->inc, m_op(cur->lchild ? cur->lchild->val : m_defaultValue, cur->rchild ? cur->rchild->val : m_defaultValue), length);
            return cur;
        }
        _Tp_FpNode *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }
        void _clear(_Tp_FpNode *p) {
            static _Tp_FpNode *q = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            if (p->lchild == q) return;
            delete p;
            if (p->lchild) _clear(p->lchild);
            if (p->rchild) _clear(p->rchild);
            p->lchild = q;
        }
        void _clear() {
            // for(_Tp_FpNode*p:m_roots)_clear(p);
            m_roots.clear();
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<_Tp_FpNode>::_reserve(__count); }
        PersistentLazySegTree(int __n = 0, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        PersistentLazySegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            reset(__first, __last);
        }
        ~PersistentLazySegTree() { _clear(); }
        void resize(int __n) {
            _clear();
            if (m_length = __n) m_roots.push_back(new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr));
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            _clear();
            m_length = __last - __first;
            auto dfs = [&](auto self, _Iterator first, _Iterator last) -> _Tp_FpNode * {
                if (first + 1 == last)
                    return new _Tp_FpNode(*first, m_defaultIncrement, nullptr, nullptr);
                else
                    return _update(new _Tp_FpNode(*first, m_defaultIncrement, self(self, first, first + (last - first + 1) / 2), self(self, first + (last - first + 1) / 2, last)), last - first);
            };
            m_roots.push_back(dfs(dfs, __first, __last));
        }
        void copyVerion(int __prevVersion) {
            m_roots.push_back(_root(__prevVersion));
        }
        void add(int __prevVersion, int __i, _Fp __inc) {
            auto dfs = [&](auto self, _Tp_FpNode *prev, int left, int right) -> _Tp_FpNode * {
                _Tp_FpNode *cur = prev ? new _Tp_FpNode(*prev) : new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
                if (left == right) {
                    cur->val = _map(__inc, cur->val, 1);
                    cur->inc = m_com(__inc, cur->inc);
                } else {
                    if (__i <= (left + right) / 2)
                        cur->lchild = self(self, cur->lchild, left, (left + right) / 2);
                    else
                        cur->rchild = self(self, cur->rchild, (left + right) / 2 + 1, right);
                    _update(cur, right - left + 1);
                }
                return cur;
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion), 0, m_length - 1));
        }
        void add(int __prevVersion, int __left, int __right, _Fp __inc) {
            auto dfs = [&](auto self, _Tp_FpNode *prev, int left, int right) -> _Tp_FpNode * {
                _Tp_FpNode *cur = prev ? new _Tp_FpNode(*prev) : new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
                if (left >= __left && right <= __right) {
                    cur->val = _map(__inc, cur->val, right - left + 1);
                    cur->inc = m_com(__inc, cur->inc);
                } else {
                    if (__left <= (left + right) / 2)
                        cur->lchild = self(self, cur->lchild, left, (left + right) / 2);
                    if (__right > (left + right) / 2)
                        cur->rchild = self(self, cur->rchild, (left + right) / 2 + 1, right);
                    _update(cur, right - left + 1);
                }
                return cur;
            };
            m_roots.push_back(dfs(dfs, _root(__prevVersion), 0, m_length - 1));
        }
        _Tp query(int __version, int __i) const {
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right, _Fp inc) {
                if (left == right)
                    return _map(inc, cur->val, 1);
                else {
                    inc = m_com(cur->inc, inc);
                    if (__i <= (left + right) / 2)
                        return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2, inc) : _map(inc, m_defaultValue, 1);
                    else
                        return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right, inc) : _map(inc, m_defaultValue, 1);
                }
            };
            return dfs(dfs, _root(__version), 0, m_length - 1, m_defaultIncrement);
        }
        _Tp query(int __version, int __left, int __right) const {
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right, _Fp inc) {
                if (left >= __left && right <= __right)
                    return _map(inc, cur->val, right - left + 1);
                else {
                    inc = m_com(cur->inc, inc);
                    if (__right <= (left + right) / 2)
                        return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2, inc) : _map(inc, m_defaultValue, __right - left + 1);
                    else if (__left > (left + right) / 2)
                        return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right, inc) : _map(inc, m_defaultValue, right - __left + 1);
                    else
                        return _map(inc, m_op(cur->lchild ? self(self, cur->lchild, left, (left + right) / 2, m_defaultIncrement) : m_defaultValue, cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right, m_defaultIncrement) : m_defaultValue), std::min(right, __right) - std::max(left, __left) + 1);
                }
            };
            return dfs(dfs, _root(__version), 0, m_length - 1, m_defaultIncrement);
        }
        _Tp queryAll(int __version) const {
            return _root(__version)->val;
        }
        int kth(int __version, _Tp __k) const {
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right, int k, _Fp inc) {
                if (left == right) return left;
                inc += cur->inc;
                if (cur->lchild) {
                    if (_Tp lval = cur->lchild->val + inc * ((left + right) / 2 - left + 1); lval > k)
                        return self(self, cur->lchild, left, (left + right) / 2, k, inc);
                    else
                        return self(self, cur->rchild, (left + right) / 2 + 1, right, k - lval, inc);
                } else
                    return self(self, cur->rchild, (left + right) / 2 + 1, right, k, inc);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1, __k, 0);
        }
        _Tp periodQuery(int __leftVersion, int __rightVersion, int __i) const {
            if (__leftVersion == 0) return query(__rightVersion, __i);
            auto dfs = [&](auto self, _Tp_FpNode *root1, _Tp_FpNode *root2, int left, int right, _Fp inc) {
                if (root1 == root2) return 0;
                if (left == right)
                    return inc + (root1 ? root2->val - root1->val : root2->val);
                else {
                    inc += root2->inc - (root1 ? root1->inc : 0);
                    if (__i <= (left + right) / 2)
                        return self(self, root1 ? root1->lchild : nullptr, root2->lchild, left, (left + right) / 2, inc);
                    else
                        return self(self, root1 ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, inc);
                }
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, 0);
        }
        _Tp periodQuery(int __leftVersion, int __rightVersion, int __left, int __right) const {
            if (__leftVersion == 0) return query(__rightVersion, __left, __right);
            auto dfs = [&](auto self, _Tp_FpNode *root1, _Tp_FpNode *root2, int left, int right, _Fp inc) -> _Tp {
                if (root1 == root2) return 0;
                if (left >= __left && right <= __right)
                    return inc + (root1 ? root2->val - root1->val : root2->val, right - left + 1);
                else {
                    inc += root2->inc + (root1 ? root1->inc : 0);
                    if (__right <= (left + right) / 2)
                        return self(self, root1 ? root1->lchild : nullptr, root2->lchild, left, (left + right) / 2, inc);
                    else if (__left > (left + right) / 2)
                        return self(self, root1 ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, inc);
                    else
                        return inc * (std::min(right, __right) - std::max(left, __left) + 1) + self(self, root1 ? root1->lchild : nullptr, root2->lchild, left, (left + right) / 2, 0) + self(self, root1 ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, 0);
                }
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, 0);
        }
        int periodKth(int __leftVersion, int __rightVersion, _Tp __k) const {
            if (__leftVersion == 0) return kth(__rightVersion, __k);
            auto dfs = [&](auto self, _Tp_FpNode *root1, _Tp_FpNode *root2, int left, int right, int k, _Fp inc) {
                if (left == right) return left;
                inc += (root2 ? root2->inc : 0) - (root1 ? root1->inc : 0);
                if (!root1 || !root1->lchild) {
                    if (!root2->lchild)
                        return self(self, root1 && root1->rchild ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, k, inc);
                    else if (_Tp lval = root2->lchild->val + inc * ((left + right) / 2 - left + 1); lval > k)
                        return self(self, nullptr, root2->lchild, left, (left + right) / 2, k, inc);
                    else
                        return self(self, root1 && root1->rchild ? root1->rchild : nullptr, root2->rchild, (left + right) / 2 + 1, right, k - lval, inc);
                } else if (_Tp lval = root2->lchild->val - root1->lchild->val + inc * ((left + right) / 2 - left + 1); lval > k)
                    return self(self, root1->lchild, root2->lchild, left, (left + right) / 2, k, inc);
                else
                    return self(self, root1->rchild, root2->rchild, (left + right) / 2 + 1, right, k - lval, inc);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, __k, 0);
        }
        int versionCount() const { return m_roots.size(); }
    };
    template <typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    PersistentLazySegTree(int = 0, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> PersistentLazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
    template <typename _Iterator, typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    PersistentLazySegTree(_Iterator, _Iterator, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> PersistentLazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
}

#endif