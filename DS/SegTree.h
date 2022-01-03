#ifndef __OY_SEGTREE__
#define __OY_SEGTREE__

#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <vector>

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    class SegTree {
        struct _TpNode {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
        };
        std::vector<_TpNode> m_buffer;
        _TpNode *m_alloc;
        _TpNode *m_root;
        int m_length;
        _Operation m_op;
        _Tp m_defaultValue;
        _Tp m_initValue;
        _TpNode *lchild(_TpNode *cur) {
            if (!cur->lchild) {
                cur->lchild = m_alloc++;
                *cur->lchild = {m_initValue, nullptr, nullptr};
            }
            return cur->lchild;
        }
        _TpNode *rchild(_TpNode *cur) {
            if (!cur->rchild) {
                cur->rchild = m_alloc++;
                *cur->rchild = {m_initValue, nullptr, nullptr};
            }
            return cur->rchild;
        }
        void _update(_TpNode *cur) {
            cur->val = m_op(cur->lchild ? cur->lchild->val : m_initValue, cur->rchild ? cur->rchild->val : m_initValue);
        }

    public:
        static int s_reserveSize;
        SegTree(int __n = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp(), _Tp __initValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue), m_initValue(__initValue) {
            assert(m_op(m_initValue, m_initValue) == m_initValue);
            resize(__n);
        }
        template <typename _Iterator>
        SegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            assert(m_op(m_initValue, m_initValue) == m_initValue);
            reset(__first, __last);
        }
        void resize(int __n) {
            if (!__n) return;
            m_length = __n;
            m_buffer.resize(std::min(m_length * 2 - 1, s_reserveSize));
            m_alloc = m_buffer.data();
            m_root = m_alloc++;
            *m_root = {m_initValue, nullptr, nullptr};
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_buffer.resize(m_length * 2 - 1);
            m_alloc = m_buffer.data();
            auto dfs = [&](auto self, _Iterator first, _Iterator last) -> _TpNode * {
                _TpNode *cur = m_alloc++;
                if (first + 1 == last) {
                    *cur = {*first, nullptr, nullptr};
                } else {
                    cur->lchild = self(self, first, first + (last - first + 1) / 2);
                    cur->rchild = self(self, first + (last - first + 1) / 2, last);
                    _update(cur);
                }
                return cur;
            };
            m_root = dfs(dfs, __first, __last);
        }
        void update(int __i, _Tp __val) {
            if (__i < 0 || __i >= m_length) return;
            auto dfs = [&](auto self, _TpNode *cur, int left, int right) -> void {
                if (left == right)
                    cur->val = __val;
                else {
                    if (__i <= (left + right) / 2)
                        self(self, lchild(cur), left, (left + right) / 2);
                    else
                        self(self, rchild(cur), (left + right) / 2 + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void add(int __i, _Tp __inc) {
            if (__i < 0 || __i >= m_length) return;
            auto dfs = [&](auto self, _TpNode *cur, int left, int right) -> void {
                if (left == right)
                    cur->val = m_op(cur->val, __inc);
                else {
                    if (__i <= (left + right) / 2)
                        self(self, lchild(cur), left, (left + right) / 2);
                    else
                        self(self, rchild(cur), (left + right) / 2 + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(int __i) const {
            if (__i < 0 || __i >= m_length) return m_defaultValue;
            auto dfs = [&](auto self, _TpNode *cur, int left, int right) {
                if (left == right)
                    return cur->val;
                else if (__i <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_initValue;
                else
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_initValue;
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp query(int __left, int __right) const {
            if (__left < 0) __left = 0;
            if (__right >= m_length) __right = m_length - 1;
            if (__left > __right) return m_defaultValue;
            auto dfs = [&](auto self, _TpNode *cur, int left, int right) {
                if (left >= __left && right <= __right)
                    return cur->val;
                else if (__right <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_initValue;
                else if (__left > (left + right) / 2)
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_initValue;
                else
                    return m_op(cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_initValue, cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_initValue);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp queryAll() const {
            return m_root->val;
        }
        int kth(_Tp __k) const {
            if (__k < 0 || __k >= queryAll()) return -1;
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
            return dfs(dfs, m_root, 0, m_length - 1, __k);
        }
    };
    template <typename _Tp = int64_t>
    SegTree(int, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> SegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int64_t>
    SegTree(int, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> SegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    SegTree(int = 0, _Operation = _Operation(), _Tp = _Tp()) -> SegTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SegTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> SegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SegTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> SegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>>
    SegTree(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp()) -> SegTree<_Tp, _Operation>;
    template <typename _Tp, typename _Operation>
    int SegTree<_Tp, _Operation>::s_reserveSize = 10000000;
}

#endif