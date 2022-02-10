#ifndef __OY_SEGTREE2D__
#define __OY_SEGTREE2D__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    class SegTree2d {
#pragma pack(4)
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
        struct _TpTree : MemoryPool<_TpTree> {
            _TpNode *root;
            _TpTree *lchild;
            _TpTree *rchild;
            _TpTree() : root(nullptr), lchild(nullptr), rchild(nullptr) {}
        };
#pragma pack()
        _TpTree *m_root;
        int m_row;
        int m_column;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }
        static _TpTree *lchild(_TpTree *cur) {
            if (!cur->lchild)
                cur->lchild = new _TpTree;
            return cur->lchild;
        }
        static _TpTree *rchild(_TpTree *cur) {
            if (!cur->rchild)
                cur->rchild = new _TpTree;
            return cur->rchild;
        }
        _TpNode *root(_TpTree *cur) {
            if (!cur->root)
                cur->root = new _TpNode(m_defaultValue, nullptr, nullptr);
            return cur->root;
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
        void _clear(_TpTree *cur) {
            if (cur->root) _clear(cur->root);
            if (cur->lchild) _clear(cur->lchild);
            if (cur->rchild) _clear(cur->rchild);
            delete cur;
        }

    public:
        static void setBufferSize(int __treeCount, int __nodeCount) {
            MemoryPool<_TpTree>::_reserve(__treeCount);
            MemoryPool<_TpNode>::_reserve(__nodeCount);
        }
        SegTree2d(int __row, int __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_root(nullptr), m_defaultValue(__defaultValue) {
            _check();
            resize(__row, __column);
        }
        template <typename Ref>
        SegTree2d(Ref __ref, int __row, int __column, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_root(nullptr), m_defaultValue(__defaultValue) {
            _check();
            reset(__ref, __row, __column);
        }
        ~SegTree2d() {
            if (m_root) {
                // _clear(m_root);
                m_root = nullptr;
            }
        }
        void resize(int __row, int __column) {
            if (m_root) _clear(m_root);
            m_row = __row;
            m_column = __column;
            m_root = new _TpTree;
        }
        template <typename Ref>
        void reset(Ref __ref, int __row, int __column) {
            if (m_root) _clear(m_root);
            m_row = __row;
            m_column = __column;
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
            auto build = [&](auto self, int left, int right) -> _TpTree * {
                _TpTree *p = new _TpTree;
                if (left == right)
                    p->root = build_leaf(build_leaf, 0, m_column - 1, left);
                else {
                    int mid = (left + right) / 2;
                    p->lchild = self(self, left, mid);
                    p->rchild = self(self, mid + 1, right);
                    p->root = build_nonleaf(build_nonleaf, p->lchild->root, p->rchild->root);
                }
                return p;
            };
            m_root = build(build, 0, m_row - 1);
        }
        void add(int __row, int __column, _Tp __inc) {
            auto dfs = [&](auto self, _TpTree *cur, int row1, int row2) {
                _add(root(cur), 0, m_column - 1, __column, __inc);
                if (row1 == row2) return;
                if (int mid = (row1 + row2) / 2; __row <= mid)
                    self(self, lchild(cur), row1, mid);
                else
                    self(self, rchild(cur), mid + 1, row2);
            };
            dfs(dfs, m_root, 0, m_row - 1);
        }
        _Tp query(int __row, int __column) const {
            auto dfs = [&](auto self, _TpTree *cur, int left, int right) {
                if (left == right)
                    return cur->root ? _query(cur->root, 0, m_column - 1, __column) : m_defaultValue;
                else if (int mid = (left + right) / 2; __row <= mid)
                    return cur->lchild ? self(self, cur->lchild, left, mid) : m_defaultValue;
                else
                    return cur->rchild ? self(self, cur->rchild, mid + 1, right) : m_defaultValue;
            };
            return dfs(dfs, m_root, 0, m_row - 1);
        }
        _Tp query(int __row1, int __row2, int __column1, int __column2) const {
            auto dfs = [&](auto self, _TpTree *cur, int left, int right) {
                if (left >= __row1 && right <= __row2)
                    return cur->root ? _query(cur->root, 0, m_column - 1, __column1, __column2) : m_defaultValue;
                else if (int mid = (left + right) / 2; __row2 <= mid)
                    return cur->lchild ? self(self, cur->lchild, left, mid) : m_defaultValue;
                else if (__row1 > (left + right) / 2)
                    return cur->rchild ? self(self, cur->rchild, mid + 1, right) : m_defaultValue;
                else
                    return m_op(cur->lchild ? self(self, cur->lchild, left, mid) : m_defaultValue, cur->rchild ? self(self, cur->rchild, mid + 1, right) : m_defaultValue);
            };
            return dfs(dfs, m_root, 0, m_row - 1);
        }
        _Tp queryAll() const { return query(0, m_row - 1, 0, m_column - 1); }
        int rowKth(int __row1, int __row2, _Tp __k) const {
            static std::vector<_TpNode *> roots_plus;
            roots_plus.clear();
            auto dfs = [&](auto self, _TpTree *cur, int left, int right) {
                if (left >= __row1 && right <= __row2) {
                    if (cur->root) roots_plus.push_back(cur->root);
                    return;
                }
                int mid = (left + right) / 2;
                if (__row1 <= mid && cur->lchild) self(self, cur->lchild, left, mid);
                if (__row2 > mid && cur->rchild) self(self, cur->rchild, mid + 1, right);
            };
            if (m_root) dfs(dfs, m_root, 0, m_row - 1);
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
    SegTree2d(int, int, _Operation = _Operation(), _Tp = _Tp()) -> SegTree2d<_Tp, _Operation>;
    template <typename Ref, typename _Tp = typename std::iterator_traits<typename decltype(std::mem_fn(&Ref::operator()))::result_type>::value_type, typename _Operation = std::plus<_Tp>>
    SegTree2d(Ref, int, int, _Operation = _Operation(), std::enable_if_t<std::is_invocable_v<Ref, int>, _Tp> = _Tp()) -> SegTree2d<_Tp, _Operation>;
    template <typename Ref, typename _Tp = typename std::decay_t<typename decltype(std::mem_fn(&Ref::operator()))::result_type>, typename _Operation = std::plus<_Tp>>
    SegTree2d(Ref, int, int, _Operation = _Operation(), std::enable_if_t<std::is_invocable_v<Ref, int, int>, _Tp> = _Tp()) -> SegTree2d<_Tp, _Operation>;
}

#endif