#ifndef __OY_LAZYSEGTREE__
#define __OY_LAZYSEGTREE__

#include <cstdint>
#include <functional>
#include <numeric>
#include <type_traits>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct LazySegAdd {
        _Tp operator()(const _Fp &__x, const _Tp &__y, int __size) const { return __x * __size + __y; }
    };
    template <typename _Tp = int64_t, typename _Fp = _Tp, typename _Operation = std::plus<_Tp>, typename _Mapping = OY::LazySegAdd<_Tp, _Fp>, typename _Composition = std::plus<_Fp>>
    class LazySegTree {
        struct _Tp_FpNode : MemoryPool<_Tp_FpNode> {
            _Tp val;
            _Fp inc;
            _Tp_FpNode *lchild;
            _Tp_FpNode *rchild;
            _Tp_FpNode(_Tp _val, _Fp _inc, _Tp_FpNode *_lchild, _Tp_FpNode *_rchild) : val(_val), inc(_inc), lchild(_lchild), rchild(_rchild) {}
        };
        _Tp_FpNode *m_root;
        int m_length;
        _Operation m_op;
        _Mapping m_map;
        _Composition m_com;
        _Tp m_defaultValue;
        _Fp m_defaultIncrement;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue&&m_com(m_defaultIncrement,m_defaultIncrement)==m_defaultIncrement);
            // if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, int>)assert(m_map(m_defaultIncrement,m_defaultValue,1)==m_defaultValue);
            // else assert(m_map(m_defaultIncrement,m_defaultValue)==m_defaultValue);
        }
        void _apply(_Tp_FpNode *cur, _Fp inc, int left, int right) {
            if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, int>)
                cur->val = m_map(inc, cur->val, right - left + 1);
            else
                cur->val = m_map(inc, cur->val);
            if (right > left) cur->inc = m_com(inc, cur->inc);
        }
        _Tp_FpNode *_update(_Tp_FpNode *cur) {
            cur->val = m_op(cur->lchild->val, cur->rchild->val);
            return cur;
        }
        void _pushDown(_Tp_FpNode *cur, int left, int right) {
            if (!cur->lchild) {
                cur->lchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
                cur->rchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            }
            _apply(cur->lchild, cur->inc, left, (left + right) / 2);
            _apply(cur->rchild, cur->inc, (left + right) / 2 + 1, right);
            cur->inc = m_defaultIncrement;
        }
        void _clear(_Tp_FpNode *p) {
            delete p;
            if (p->lchild) _clear(p->lchild);
            if (p->rchild) _clear(p->rchild);
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<_Tp_FpNode>::_reserve(__count); }
        LazySegTree(int __n = 0, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_root(nullptr), m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        LazySegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_root(nullptr), m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            reset(__first, __last);
        }
        ~LazySegTree() {
            if (m_root) _clear(m_root);
        }
        void resize(int __n) {
            if (m_root) _clear(m_root);
            if (m_length = __n)
                m_root = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            else
                m_root = nullptr;
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            if (m_root) _clear(m_root);
            m_length = __last - __first;
            auto dfs = [&](auto self, _Iterator first, _Iterator last) -> _Tp_FpNode * {
                if (first + 1 == last)
                    return new _Tp_FpNode(*first, m_defaultIncrement, nullptr, nullptr);
                else
                    return _update(new _Tp_FpNode(m_defaultValue, m_defaultIncrement, self(self, first, first + (last - first + 1) / 2), self(self, first + (last - first + 1) / 2, last)));
            };
            m_root = dfs(dfs, __first, __last);
        }
        void update(int __i, _Tp __val) {
            if (__i < 0 || __i >= m_length) return;
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right) -> void {
                if (left == right)
                    cur->val = __val;
                else {
                    _pushDown(cur, left, right);
                    if (__i <= (left + right) / 2)
                        self(self, cur->lchild, left, (left + right) / 2);
                    else
                        self(self, cur->rchild, (left + right) / 2 + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void add(int __i, _Fp __inc) {
            if (__i < 0 || __i >= m_length) return;
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right) -> void {
                if (left == right)
                    _apply(cur, __inc, left, right);
                else {
                    _pushDown(cur, left, right);
                    if (__i <= (left + right) / 2)
                        self(self, cur->lchild, left, (left + right) / 2);
                    else
                        self(self, cur->rchild, (left + right) / 2 + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void add(int __left, int __right, _Fp __inc) {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length;
            if (__left > __right) return;
            if (__left == __right) {
                add(__left, __inc);
                return;
            }
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right) -> void {
                if (__left <= left && __right >= right)
                    _apply(cur, __inc, left, right);
                else {
                    _pushDown(cur, left, right);
                    int mid = (left + right) / 2;
                    if (__left <= mid) self(self, cur->lchild, left, mid);
                    if (__right > mid) self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(int __i) {
            if (__i < 0 || __i >= m_length) return m_defaultValue;
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right) {
                if (left == right)
                    return cur->val;
                else {
                    if (__i <= (left + right) / 2)
                        return cur->lchild ? _pushDown(cur, left, right), self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                    else
                        return cur->rchild ? _pushDown(cur, left, right), self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(int __left, int __right) {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length;
            if (__left > __right) return m_defaultValue;
            if (__left == __right) return query(__left);
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right) -> _Tp {
                if (__left <= left && __right >= right)
                    return cur->val;
                else {
                    if (int mid = (left + right) / 2; __left > mid)
                        return cur->rchild ? _pushDown(cur, left, right), self(self, cur->rchild, mid + 1, right) : m_defaultValue;
                    else if (__right <= mid)
                        return cur->lchild ? _pushDown(cur, left, right), self(self, cur->lchild, left, mid) : m_defaultValue;
                    else
                        return cur->lchild ? _pushDown(cur, left, right), m_op(self(self, cur->lchild, left, mid), self(self, cur->rchild, mid + 1, right)) : m_defaultValue;
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp queryAll() const {
            return m_root->val;
        }
        int kth(_Tp __k) {
            if (__k < 0 || __k >= queryAll()) return -1;
            auto dfs = [&](auto self, _Tp_FpNode *cur, int left, int right, int k) {
                if (left == right) return left;
                _pushDown(cur, left, right);
                if (cur->lchild->val > k)
                    return self(self, cur->lchild, left, (left + right) / 2, k);
                else
                    return self(self, cur->rchild, (left + right) / 2 + 1, right, k - cur->lchild->val);
            };
            return dfs(dfs, m_root, 0, m_length - 1, __k);
        }
    };
    template <typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    LazySegTree(int = 0, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> LazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
    template <typename _Iterator, typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    LazySegTree(_Iterator, _Iterator, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> LazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
}

#endif