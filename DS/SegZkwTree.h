#ifndef __OY_SEGZKWTREE__
#define __OY_SEGZKWTREE__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    class SegZkwTree {
#pragma pack(4)
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
#pragma pack()
        std::vector<_TpNode *> m_sub;
        int m_row;
        int m_column;
        int m_depth;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        _TpNode *sub(int i) {
            if (!m_sub[i]) m_sub[i] = new _TpNode(m_defaultValue, nullptr, nullptr);
            return m_sub[i];
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
        void _add(_TpNode *cur, int left, int right, int col, _Tp inc) {
            cur->val = m_op(cur->val, inc);
            if (left == right) return;
            if (int mid = (left + right) / 2; col <= mid)
                _add(lchild(cur), left, mid, col, inc);
            else
                _add(rchild(cur), mid + 1, right, col, inc);
        }
        _Tp _query(_TpNode *cur, int left, int right, int col) const {
            if (left == right)
                return cur->val;
            else if (int mid = (left + right) / 2; col <= mid)
                return cur->lchild ? _query(cur->lchild, left, mid, col) : m_defaultValue;
            else
                return cur->rchild ? _query(cur->rchild, mid + 1, right, col) : m_defaultValue;
        }
        _Tp _query(_TpNode *cur, int left, int right, int col1, int col2) const {
            if (left >= col1 && right <= col2)
                return cur->val;
            else if (int mid = (left + right) / 2; col2 <= mid)
                return cur->lchild ? _query(cur->lchild, left, mid, col1, col2) : m_defaultValue;
            else if (col1 > mid)
                return cur->rchild ? _query(cur->rchild, mid + 1, right, col1, col2) : m_defaultValue;
            else
                return m_op(cur->lchild ? _query(cur->lchild, left, mid, col1, col2) : m_defaultValue, cur->rchild ? _query(cur->rchild, mid + 1, right, col1, col2) : m_defaultValue);
        }
        void _clear(_TpNode *cur) {
            if (cur->lchild) _clear(cur->lchild);
            if (cur->rchild) _clear(cur->rchild);
            delete cur;
        }
        void _clear() {
            // for (_TpNode *root : m_sub)
            //     if (root) _clear(root);
            m_sub.clear();
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<_TpNode>::_reserve(__count); }
        SegZkwTree(int __row, int __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__row, __column);
        }
        template <typename Ref>
        SegZkwTree(Ref __ref, int __row, int __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__ref, __row, __column);
        }
        ~SegZkwTree() { _clear(); }
        void resize(int __row, int __column) {
            _clear();
            m_row = __row;
            m_column = __column;
            m_depth = 32 - (m_row > 1 ? __builtin_clz(m_row - 1) : 32);
            m_sub.resize(1 << (m_depth + 1), nullptr);
        }
        template <typename Ref>
        void reset(Ref __ref, int __row, int __column) {
            _clear();
            m_row = __row;
            m_column = __column;
            m_depth = 32 - (m_row > 1 ? __builtin_clz(m_row - 1) : 32);
            auto build_leaf = [&](auto self, int left, int right, int row) {
                if (left == right) return new _TpNode(__ref(row, left), nullptr, nullptr);
                int mid = (left + right) / 2;
                _TpNode *lchild = self(self, left, mid, row);
                _TpNode *rchild = self(self, mid + 1, right, row);
                return new _TpNode(m_op(lchild->val, rchild->val), lchild, rchild);
            };
            auto build_nonleaf = [&](auto self, _TpNode *lc, _TpNode *rc) -> _TpNode * {
                _TpNode *p = new _TpNode(m_defaultValue, nullptr, nullptr);
                p->val = m_op(lc->val, rc->val);
                if (lc->lchild) {
                    p->lchild = self(self, lc->lchild, rc->lchild);
                    p->rchild = self(self, lc->rchild, rc->rchild);
                }
                return p;
            };
            m_sub.resize(1 << (m_depth + 1));
            for (int i = 0; i < m_row; i++) m_sub[i + (1 << m_depth)] = build_leaf(build_leaf, 0, m_column - 1, i);
            for (int i = (1 << m_depth) - 1; i; i--) m_sub[i] = build_nonleaf(build_nonleaf, m_sub[i << 1], m_sub[i << 1 ^ 1]);
        }
        void add(int __row, int __column, _Tp __inc) {
            for (int i = __row + (1 << m_depth); i; i >>= 1) _add(sub(i), 0, m_column - 1, __column, __inc);
        }
        _Tp query(int __row, int __column) const { return m_sub[__row] ? _query(m_sub[__row], 0, m_column - 1, __column) : m_defaultValue; }
        _Tp query(int __row1, int __row2, int __column1, int __column2) const {
            __row1 += 1 << m_depth;
            __row2 += 1 << m_depth;
            if (__row1 == __row2) return m_sub[__row1] ? _query(m_sub[__row1], 0, m_column - 1, __column1, __column2) : m_defaultValue;
            _Tp res = m_sub[__row1] ? _query(m_sub[__row1], 0, m_column - 1, __column1, __column2) : m_defaultValue;
            int j = 31 - __builtin_clz(__row1 ^ __row2);
            for (int i = 0; i < j; i++)
                if (!(__row1 >> i & 1)) res = m_op(res, m_sub[__row1 >> i ^ 1] ? _query(m_sub[__row1 >> i ^ 1], 0, m_column - 1, __column1, __column2) : m_defaultValue);
            for (int i = j - 1; i >= 0; i--)
                if (__row2 >> i & 1) res = m_op(res, m_sub[__row2 >> i ^ 1] ? _query(m_sub[__row2 >> i ^ 1], 0, m_column - 1, __column1, __column2) : m_defaultValue);
            return m_op(res, m_sub[__row2] ? _query(m_sub[__row2], 0, m_column - 1, __column1, __column2) : m_defaultValue);
        }
        _Tp queryAll() const { return query(0, m_row - 1, 0, m_column - 1); }
        int rowKth(int __row1, int __row2, _Tp __k) const {
            static std::vector<_TpNode *> roots_plus;
            roots_plus.clear();
            __row1 += 1 << m_depth;
            __row2 += 1 << m_depth;
            if (__row1 < __row2) {
                if (m_sub[__row1]) roots_plus.push_back(m_sub[__row1]);
                int j = 31 - __builtin_clz(__row1 ^ __row2);
                for (int i = 0; i < j; i++)
                    if (!(__row1 >> i & 1) && m_sub[__row1 >> i ^ 1]) roots_plus.push_back(m_sub[__row1 >> i ^ 1]);
                for (int i = j - 1; i >= 0; i--)
                    if ((__row2 >> i & 1) && m_sub[__row2 >> i ^ 1]) roots_plus.push_back(m_sub[__row2 >> i ^ 1]);
                if (m_sub[__row2]) roots_plus.push_back(m_sub[__row2]);
            } else if (m_sub[__row1])
                roots_plus.push_back(m_sub[__row1]);
            int left = 0, right = m_column - 1;
#define FILTER(vec, prop)                    \
    {                                        \
        int i = 0;                           \
        for (auto a : vec)                   \
            if (a->prop) vec[i++] = a->prop; \
        vec.resize(i);                       \
    }
            while (left < right) {
                _Tp sum = 0;
                for (_TpNode *root : roots_plus) sum += root->lchild ? root->lchild->val : 0;
                if (__k < sum) {
                    right = (left + right) / 2;
                    FILTER(roots_plus, lchild);
                } else {
                    left = (left + right) / 2 + 1;
                    __k -= sum;
                    FILTER(roots_plus, rchild);
                }
            }
            return left;
#undef FILTER
        }
    };
    template <typename _Tp = int, typename _Operation = std::plus<_Tp>>
    SegZkwTree(int, int, _Operation = _Operation(), _Tp = _Tp()) -> SegZkwTree<_Tp, _Operation>;
    template <typename Ref, typename _Tp = typename std::iterator_traits<typename decltype(std::mem_fn(&Ref::operator()))::result_type>::value_type, typename _Operation = std::plus<_Tp>>
    SegZkwTree(Ref, int, int, _Operation = _Operation(), std::enable_if_t<std::is_invocable_v<Ref, int>, _Tp> = _Tp()) -> SegZkwTree<_Tp, _Operation>;
    template <typename Ref, typename _Tp = typename std::decay_t<typename decltype(std::mem_fn(&Ref::operator()))::result_type>, typename _Operation = std::plus<_Tp>>
    SegZkwTree(Ref, int, int, _Operation = _Operation(), std::enable_if_t<std::is_invocable_v<Ref, int, int>, _Tp> = _Tp()) -> SegZkwTree<_Tp, _Operation>;
}

#endif