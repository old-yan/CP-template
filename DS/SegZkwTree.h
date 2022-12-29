#ifndef __OY_SEGZKWTREE__
#define __OY_SEGZKWTREE__

#include <bit>
#include <functional>

#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    struct SegZkwTree {
#pragma pack(4)
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
#pragma pack()
        std::vector<_TpNode *> m_sub;
        uint32_t m_row, m_column, m_depth;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        _TpNode *sub(uint32_t i) {
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
        static void setBufferSize(int __count) { MemoryPool<_TpNode>::_reserve(__count); }
        SegZkwTree(uint32_t __row, uint32_t __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__row, __column);
        }
        template <typename Ref>
        SegZkwTree(Ref __ref, uint32_t __row, uint32_t __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__ref, __row, __column);
        }
        ~SegZkwTree() { _clear(); }
        void resize(uint32_t __row, uint32_t __column) {
            _clear();
            m_row = __row, m_column = __column, m_depth = 32 - (m_row > 1 ? std::__countl_zero(m_row - 1) : 32);
            m_sub.resize(1 << (m_depth + 1), nullptr);
        }
        template <typename Ref>
        void reset(Ref __ref, uint32_t __row, uint32_t __column) {
            _clear();
            m_row = __row, m_column = __column, m_depth = 32 - (m_row > 1 ? std::__countl_zero(m_row - 1) : 32);
            auto build_leaf = [&](auto self, uint32_t left, uint32_t right, uint32_t row) {
                if (left == right) return new _TpNode(__ref(row, left), nullptr, nullptr);
                uint32_t mid = (left + right) / 2;
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
            for (uint32_t i = 0; i < 1 << m_depth; i++) m_sub[i + (1 << m_depth)] = build_leaf(build_leaf, 0, m_column - 1, i);
            for (uint32_t i = (1 << m_depth) - 1; i; i--) m_sub[i] = build_nonleaf(build_nonleaf, m_sub[i << 1], m_sub[i << 1 ^ 1]);
        }
        void add(uint32_t __row, uint32_t __column, _Tp __inc) {
            auto dfs = [&](auto self, _TpNode *cur, uint32_t left, uint32_t right) {
                cur->val = m_op(cur->val, __inc);
                if (left == right) return;
                if (uint32_t mid = (left + right) / 2; __column <= mid)
                    self(self, lchild(cur), left, mid);
                else
                    self(self, rchild(cur), mid + 1, right);
            };
            for (uint32_t i = (1 << m_depth) + __row; i; i >>= 1) dfs(dfs, sub(i), 0, m_column - 1);
        }
        _Tp query(uint32_t __row, uint32_t __column) const {
            auto dfs = [&](auto self, _TpNode *cur, uint32_t left, uint32_t right) {
                if (left == right)
                    return cur->val;
                else if (uint32_t mid = (left + right) / 2; __column <= mid)
                    return cur->lchild ? self(self, cur->lchild, left, mid) : m_defaultValue;
                else
                    return cur->rchild ? self(self, cur->rchild, mid + 1, right) : m_defaultValue;
            };
            return m_sub[(1 << m_depth) + __row] ? dfs(dfs, m_sub[(1 << m_depth) + __row], 0, m_column - 1) : m_defaultValue;
        }
        _Tp query(uint32_t __row1, uint32_t __row2, uint32_t __column1, uint32_t __column2) const {
            __row1 += 1 << m_depth, __row2 += 1 << m_depth;
            auto dfs = [&](auto self, _TpNode *cur, uint32_t left, uint32_t right) {
                if (left >= __column1 && right <= __column2)
                    return cur->val;
                else if (uint32_t mid = (left + right) / 2; __column2 <= mid)
                    return cur->lchild ? self(self, cur->lchild, left, mid) : m_defaultValue;
                else if (__column1 > mid)
                    return cur->rchild ? self(self, cur->rchild, mid + 1, right) : m_defaultValue;
                else
                    return m_op(cur->lchild ? self(self, cur->lchild, left, mid) : m_defaultValue, cur->rchild ? self(self, cur->rchild, mid + 1, right) : m_defaultValue);
            };
            if (__row1 == __row2) return m_sub[__row1] ? dfs(dfs, m_sub[__row1], 0, m_column - 1) : m_defaultValue;
            _Tp res = m_sub[__row1] ? dfs(dfs, m_sub[__row1], 0, m_column - 1) : m_defaultValue;
            uint32_t j = 31 - std::__countl_zero(__row1 ^ __row2);
            for (uint32_t i = 0; i < j; i++)
                if (!(__row1 >> i & 1)) res = m_op(res, m_sub[__row1 >> i ^ 1] ? dfs(dfs, m_sub[__row1 >> i ^ 1], 0, m_column - 1) : m_defaultValue);
            for (uint32_t i = j - 1; ~i; i--)
                if (__row2 >> i & 1) res = m_op(res, m_sub[__row2 >> i ^ 1] ? dfs(dfs, m_sub[__row2 >> i ^ 1], 0, m_column - 1) : m_defaultValue);
            return m_op(res, m_sub[__row2] ? dfs(dfs, m_sub[__row2], 0, m_column - 1) : m_defaultValue);
        }
        _Tp queryAll() const { return query(0, m_row - 1, 0, m_column - 1); }
        uint32_t rowKth(uint32_t __row1, uint32_t __row2, _Tp __k) const {
            static std::vector<_TpNode *> roots_plus;
            roots_plus.clear();
            if (__row1 += 1 << m_depth, __row2 += 1 << m_depth; __row1 < __row2) {
                if (m_sub[__row1]) roots_plus.push_back(m_sub[__row1]);
                uint32_t j = 31 - std::__countl_zero(__row1 ^ __row2);
                for (uint32_t i = 0; i < j; i++)
                    if (!(__row1 >> i & 1) && m_sub[__row1 >> i ^ 1]) roots_plus.push_back(m_sub[__row1 >> i ^ 1]);
                for (uint32_t i = j - 1; ~i; i--)
                    if ((__row2 >> i & 1) && m_sub[__row2 >> i ^ 1]) roots_plus.push_back(m_sub[__row2 >> i ^ 1]);
                if (m_sub[__row2]) roots_plus.push_back(m_sub[__row2]);
            } else if (m_sub[__row1])
                roots_plus.push_back(m_sub[__row1]);
            uint32_t left = 0, right = m_column - 1;
            while (left < right) {
                _Tp sum = 0;
                for (_TpNode *root : roots_plus) sum += root->lchild ? root->lchild->val : 0;
                if (__k < sum) {
                    right = (left + right) / 2;
                    roots_plus.erase(std::remove_if(roots_plus.begin(), roots_plus.end(), [](_TpNode *p) -> bool { return p = p->lchild; }), roots_plus.end());
                } else {
                    left = (left + right) / 2 + 1, __k -= sum;
                    roots_plus.erase(std::remove_if(roots_plus.begin(), roots_plus.end(), [](_TpNode *p) -> bool { return p = p->rchild; }), roots_plus.end());
                }
            }
            return left;
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