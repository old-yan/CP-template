#ifndef __OY_LAZYSEGTREE__
#define __OY_LAZYSEGTREE__

#include <cstdint>
#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct LazySegAdd {
        _Tp operator()(const _Fp &__x, const _Tp &__y, uint64_t __size) const { return __x * __size + __y; }
    };
    template <typename _Tp = int64_t, typename _Fp = _Tp, typename _Operation = std::plus<_Tp>, typename _Mapping = LazySegAdd<_Tp, _Fp>, typename _Composition = std::plus<_Fp>>
    struct LazySegTree {
        struct _Tp_FpNode : MemoryPool<_Tp_FpNode> {
            _Tp val;
            _Fp inc;
            _Tp_FpNode *lchild, *rchild;
            _Tp_FpNode(_Tp _val, _Fp _inc, _Tp_FpNode *_lchild, _Tp_FpNode *_rchild) : val(_val), inc(_inc), lchild(_lchild), rchild(_rchild) {}
        };
        _Tp_FpNode *m_root;
        uint64_t m_length;
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
        _Tp _applied(_Tp __val, _Fp __inc, uint64_t __left, uint64_t __right) const {
            if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, uint64_t>)
                return m_map(__inc, __val, __right - __left + 1);
            else
                return m_map(__inc, __val);
        }
        void _apply(_Tp_FpNode *__cur, _Fp __inc, uint64_t __left, uint64_t __right) {
            __cur->val = _applied(__cur->val, __inc, __left, __right);
            if (__right > __left) __cur->inc = m_com(__inc, __cur->inc);
        }
        _Tp_FpNode *_update(_Tp_FpNode *__cur) {
            __cur->val = m_op(__cur->lchild->val, __cur->rchild->val);
            return __cur;
        }
        void _pushDown(_Tp_FpNode *__cur, uint64_t __left, uint64_t __right) {
            if (!__cur->lchild) {
                __cur->lchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
                __cur->rchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            }
            _apply(__cur->lchild, __cur->inc, __left, (__left + __right) / 2);
            _apply(__cur->rchild, __cur->inc, (__left + __right) / 2 + 1, __right);
            __cur->inc = m_defaultIncrement;
        }
        void _clear(_Tp_FpNode *p) {
            // if (p->lchild) _clear(p->lchild);
            // if (p->rchild) _clear(p->rchild);
            // delete p;
        }
        static void setBufferSize(uint32_t __count) { MemoryPool<_Tp_FpNode>::_reserve(__count); }
        LazySegTree(uint64_t __length = 0, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_root(nullptr), m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        LazySegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_root(nullptr), m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            reset(__first, __last);
        }
        ~LazySegTree() {
            if (m_root) _clear(m_root);
        }
        void resize(uint64_t __length) {
            if (m_root) _clear(m_root);
            if (m_length = __length)
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
        void update(uint64_t __i, _Tp __val) {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) -> void {
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
        void add(uint64_t __i, _Fp __inc) {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) -> void {
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
        void add(uint64_t __left, uint64_t __right, _Fp __inc) {
            if (__left == __right) {
                add(__left, __inc);
                return;
            }
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) -> void {
                if (__left <= left && __right >= right)
                    _apply(cur, __inc, left, right);
                else {
                    _pushDown(cur, left, right);
                    uint64_t mid = (left + right) / 2;
                    if (__left <= mid) self(self, cur->lchild, left, mid);
                    if (__right > mid) self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(uint64_t __i) {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) {
                if (left == right)
                    return cur->val;
                else {
                    if (__i <= (left + right) / 2)
                        return cur->lchild ? _pushDown(cur, left, right), self(self, cur->lchild, left, (left + right) / 2) : _applied(m_defaultValue, cur->inc, __i, __i);
                    else
                        return cur->rchild ? _pushDown(cur, left, right), self(self, cur->rchild, (left + right) / 2 + 1, right) : _applied(m_defaultValue, cur->inc, __i, __i);
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(uint64_t __left, uint64_t __right) {
            if (__left == __right) return query(__left);
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) -> _Tp {
                if (__left <= left && __right >= right)
                    return cur->val;
                else {
                    if (uint64_t mid = (left + right) / 2; __left > mid)
                        return cur->rchild ? _pushDown(cur, left, right), self(self, cur->rchild, mid + 1, right) : _applied(m_defaultValue, cur->inc, __left, __right < right ? __right : right);
                    else if (__right <= mid)
                        return cur->lchild ? _pushDown(cur, left, right), self(self, cur->lchild, left, mid) : _applied(m_defaultValue, cur->inc, __left > left ? __left : left, __right);
                    else
                        return cur->lchild ? _pushDown(cur, left, right), m_op(self(self, cur->lchild, left, mid), self(self, cur->rchild, mid + 1, right)) : m_op(_applied(m_defaultValue, cur->inc, __left > left ? __left : left, mid), _applied(m_defaultValue, cur->inc, mid + 1, __right < right ? __right : right));
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp queryAll() const { return m_root->val; }
        template <typename _Judge>
        uint64_t maxRight(uint64_t __left, _Judge __judge) {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) {
                if (__left <= left)
                    if (_Tp a(m_op(val, cur->val)); __judge(a)) {
                        val = a;
                        return right;
                    } else if (left == right)
                        return left - 1;
                _pushDown(cur, left, right);
                if (__left <= (left + right) / 2)
                    if (uint64_t res = self(self, cur->lchild, left, (left + right) / 2); res != (left + right) / 2) return res;
                return self(self, cur->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        template <typename _Judge>
        uint64_t minLeft(uint64_t __right, _Judge __judge) {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right) {
                if (__right >= right)
                    if (_Tp a(m_op(cur->val, val)); __judge(a)) {
                        val = a;
                        return left;
                    } else if (left == right)
                        return right + 1;
                _pushDown(cur, left, right);
                if (__right > (left + right) / 2)
                    if (uint64_t res = self(self, cur->rchild, (left + right) / 2 + 1, right); res != (left + right) / 2 + 1) return res;
                return self(self, cur->lchild, left, (left + right) / 2);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        uint64_t kth(_Tp __k) {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right, uint64_t k) {
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
    LazySegTree(uint64_t = 0, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> LazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
    template <typename _Iterator, typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    LazySegTree(_Iterator, _Iterator, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> LazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
}

#endif