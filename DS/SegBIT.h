#ifndef __OY_SEGBIT__
#define __OY_SEGBIT__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    class SegBIT {
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
        int m_length;
        _Plus m_plus;
        _Minus m_minus;
        _Tp m_defaultValue;
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
            cur->val = m_plus(cur->val, inc);
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
                return m_plus(cur->lchild ? _query(cur->lchild, left, mid, col1, col2) : m_defaultValue, cur->rchild ? _query(cur->rchild, mid + 1, right, col1, col2) : m_defaultValue);
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
        SegBIT(int __row, int __column, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) { resize(__row, __column); }
        template <typename Ref>
        SegBIT(Ref __ref, int __row, int __column, _Plus __plus = _Plus(), _Minus __minus = _Minus(), _Tp __defaultValue = _Tp()) : m_plus(__plus), m_minus(__minus), m_defaultValue(__defaultValue) { reset(__ref, __row, __column); }
        ~SegBIT() { _clear(); }
        void resize(int __row, int __column) {
            _clear();
            m_row = __row;
            m_column = __column;
            m_length = m_row > 1 ? 1 << (32 - std::__countl_zero<uint32_t>(m_row - 1)) : 1;
            m_sub.assign(m_length, nullptr);
        }
        template <typename Ref>
        void reset(Ref __ref, int __row, int __column) {
            _clear();
            m_row = __row;
            m_column = __column;
            m_length = m_row > 1 ? 1 << (32 - std::__countl_zero<uint32_t>(m_row - 1)) : 1;
            auto build_leaf = [&](auto self, int left, int right, int row) {
                if (left == right) return new _TpNode(__ref(row, left), nullptr, nullptr);
                int mid = (left + right) / 2;
                _TpNode *lchild = self(self, left, mid, row);
                _TpNode *rchild = self(self, mid + 1, right, row);
                return new _TpNode(m_plus(lchild->val, rchild->val), lchild, rchild);
            };
            auto build_sum = [&](auto self, _TpNode *cur, _TpNode *other) {
                cur->val = m_plus(cur->val, other->val);
                if (!other->lchild) return;
                self(self, lchild(cur), other->lchild);
                self(self, rchild(cur), other->rchild);
            };
            m_sub.resize(m_length);
            for (int i = 0; i < m_row; i++) m_sub[i] = build_leaf(build_leaf, 0, m_column - 1, i);
            for (int i = 0; i < m_length; i++)
                if (int j = i + (1 << std::__countr_zero<uint32_t>(i + 1)); j < m_length) build_sum(build_sum, sub(j), m_sub[i]);
        }
        void add(int __row, int __column, _Tp __inc) {
            while (__row < m_length) {
                _add(sub(__row), 0, m_column - 1, __column, __inc);
                __row += 1 << std::__countr_zero<uint32_t>(__row + 1);
            }
        }
        _Tp presum(int __row, int __column) const {
            _Tp ret = m_defaultValue;
            while (__row >= 0) {
                ret = m_plus(ret, m_sub[__row] ? _query(m_sub[__row], 0, m_column - 1, __column) : m_defaultValue);
                __row -= 1 << std::__countr_zero<uint32_t>(__row + 1);
            }
            return ret;
        }
        _Tp presum(int __row, int __column1, int __column2) const {
            _Tp ret = m_defaultValue;
            while (__row >= 0) {
                ret = m_plus(ret, m_sub[__row] ? _query(m_sub[__row], 0, m_column - 1, __column1, __column2) : m_defaultValue);
                __row -= 1 << std::__countr_zero<uint32_t>(__row + 1);
            }
            return ret;
        }
        _Tp query(int __row, int __column) const { return m_minus(presum(__row, __column), presum(__row - 1, __column)); }
        _Tp query(int __row1, int __row2, int __column1, int __column2) const { return m_minus(presum(__row2, __column1, __column2), presum(__row1 - 1, __column1, __column2)); }
        _Tp queryAll() const { return presum(m_row - 1, 0, m_column - 1); }
        int rowKth(int __row1, int __row2, _Tp __k) const {
            static std::vector<_TpNode *> roots_plus, roots_minus;
            roots_plus.clear();
            roots_minus.clear();
            while (__row2 >= 0) {
                if (m_sub[__row2]) roots_plus.push_back(m_sub[__row2]);
                __row2 -= 1 << std::__countr_zero<uint32_t>(__row2 + 1);
            }
            __row1--;
            while (__row1 >= 0) {
                if (m_sub[__row1]) roots_minus.push_back(m_sub[__row1]);
                __row1 -= 1 << std::__countr_zero<uint32_t>(__row1 + 1);
            }
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
                for (_TpNode *root : roots_minus) sum -= root->lchild ? root->lchild->val : 0;
                if (__k < sum) {
                    right = (left + right) / 2;
                    FILTER(roots_plus, lchild);
                    FILTER(roots_minus, lchild);
                } else {
                    left = (left + right) / 2 + 1;
                    __k -= sum;
                    FILTER(roots_plus, rchild);
                    FILTER(roots_minus, rchild);
                }
            }
            return left;
#undef FILTER
        }
    };
    template <typename _Tp = int, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    SegBIT(int, int, _Plus = _Plus(), _Minus = _Minus(), _Tp = _Tp()) -> SegBIT<_Tp, _Plus, _Minus>;
    template <typename Ref, typename _Tp = typename std::iterator_traits<typename decltype(std::mem_fn(&Ref::operator()))::result_type>::value_type, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    SegBIT(Ref, int, int, _Plus = _Plus(), _Minus = _Minus(), std::enable_if_t<std::is_invocable_v<Ref, int>, _Tp> = _Tp()) -> SegBIT<_Tp, _Plus, _Minus>;
    template <typename Ref, typename _Tp = typename std::decay_t<typename decltype(std::mem_fn(&Ref::operator()))::result_type>, typename _Plus = std::plus<_Tp>, typename _Minus = std::minus<_Tp>>
    SegBIT(Ref, int, int, _Plus = _Plus(), _Minus = _Minus(), std::enable_if_t<std::is_invocable_v<Ref, int, int>, _Tp> = _Tp()) -> SegBIT<_Tp, _Plus, _Minus>;
}

#endif