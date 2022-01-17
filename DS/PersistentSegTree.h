#ifndef __OY_PERSISTENTSEGTREE__
#define __OY_PERSISTENTSEGTREE__

#include <cassert>
#include <cstdint>
#include <functional>
#include <vector>

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    class SegTree {
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild;
            _TpNode *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
        _TpNode *m_root;
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
        void _clear(_TpNode *p) {
            delete p;
            if (p->lchild) delete p->lchild;
            if (p->rchild) delete p->rchild;
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<_TpNode>::_reserve(__count); }
        SegTree(int __n = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_root(nullptr), m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        SegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_root(nullptr), m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        ~SegTree() {
            if (m_root) _clear(m_root);
        }
        void resize(int __n) {
            if (m_root) _clear(m_root);
            m_root = (m_length = __n) ? new _TpNode(m_defaultValue, nullptr, nullptr) : nullptr;
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            if (m_root) _clear(m_root);
            m_length = __last - __first;
            auto dfs = [&](auto self, _Iterator first, _Iterator last) -> _TpNode * {
                if (first + 1 == last)
                    return new _TpNode(*first, nullptr, nullptr);
                else
                    return _update(new _TpNode(m_defaultValue, self(self, first, first + (last - first + 1) / 2), self(self, first + (last - first + 1) / 2, last)));
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
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
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
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else if (__left > (left + right) / 2)
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
                else
                    return m_op(cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue, cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue);
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        _Tp queryAll() const { return m_root->val; }
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
}

#endif