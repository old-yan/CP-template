#ifndef __OY_PERSISTENTSEGTREE__
#define __OY_PERSISTENTSEGTREE__

#include <cstdint>
#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Operation = std::plus<_Tp>>
    struct PersistentSegTree {
        struct _TpNode : MemoryPool<_TpNode> {
            _Tp val;
            _TpNode *lchild, *rchild;
            _TpNode(_Tp _val, _TpNode *_lchild, _TpNode *_rchild) : val(_val), lchild(_lchild), rchild(_rchild) {}
        };
        std::vector<_TpNode *> m_roots;
        uint64_t m_length;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            // assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
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
        _TpNode *_root(uint32_t version) const { return ~version ? m_roots[version] : m_roots.back(); }
        void _clear() { m_roots.clear(); }
        static void setBufferSize(uint64_t __count) { MemoryPool<_TpNode>::_reserve(__count); }
        PersistentSegTree(uint64_t __length = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        PersistentSegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(uint64_t __length) {
            _clear();
            if (m_length = __length) m_roots.push_back(new _TpNode(m_defaultValue, nullptr, nullptr));
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
        void copyVersion(uint32_t __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        void update(uint32_t __prevVersion, uint64_t __i, _Tp __val) {
            auto dfs = [&](auto self, _TpNode *prev, uint64_t left, uint64_t right) -> _TpNode * {
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
        void add(uint32_t __prevVersion, uint64_t __i, _Tp __inc) {
            auto dfs = [&](auto self, _TpNode *prev, uint64_t left, uint64_t right) -> _TpNode * {
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
        _Tp query(uint32_t __version, uint64_t __i) const {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (left == right)
                    return cur->val;
                else if (__i <= (left + right) / 2)
                    return cur->lchild ? self(self, cur->lchild, left, (left + right) / 2) : m_defaultValue;
                else
                    return cur->rchild ? self(self, cur->rchild, (left + right) / 2 + 1, right) : m_defaultValue;
            };
            return dfs(dfs, _root(__version), 0, m_length - 1);
        }
        _Tp query(uint32_t __version, uint64_t __left, uint64_t __right) const {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
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
        _Tp queryAll(uint32_t __version) const { return _root(__version)->val; }
        template <typename _Judge>
        uint64_t maxRight(uint32_t __version, uint64_t __left, _Judge __judge) const {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (__left <= left)
                    if (_Tp a(m_op(val, cur->val)); __judge(a)) {
                        val = a;
                        return right;
                    } else if (left == right)
                        return left - 1;
                if (__left <= (left + right) / 2)
                    if (uint64_t res = self(self, cur->lchild, left, (left + right) / 2); res != (left + right) / 2) return res;
                return self(self, cur->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1);
        }
        template <typename _Judge>
        uint64_t minLeft(uint32_t __version, uint64_t __right, _Judge __judge) const {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right) {
                if (__right >= right)
                    if (_Tp a(m_op(cur->val, val)); __judge(a)) {
                        val = a;
                        return left;
                    } else if (left == right)
                        return right + 1;
                if (__right > (left + right) / 2)
                    if (uint64_t res = self(self, cur->rchild, (left + right) / 2 + 1, right); res != (left + right) / 2 + 1) return res;
                return self(self, cur->lchild, left, (left + right) / 2);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1);
        }
        uint64_t kth(uint32_t __version, _Tp __k) const {
            auto dfs = [&](auto self, _TpNode *cur, uint64_t left, uint64_t right, uint64_t k) {
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
        _Tp periodQuery(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __i) const {
            if (__leftVersion == 0) return query(__rightVersion, __i);
            auto dfs = [&](auto self, _TpNode *cur1, _TpNode *cur2, uint64_t left, uint64_t right) {
                if (cur1 == cur2) return 0;
                if (left == right)
                    return cur1 ? cur2->val - cur1->val : cur2->val;
                else if (__i <= (left + right) / 2)
                    return self(self, cur1 ? cur1->lchild : nullptr, cur2->lchild, left, (left + right) / 2);
                else
                    return self(self, cur1 ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1);
        }
        _Tp periodQuery(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __left, uint64_t __right) const {
            if (__leftVersion == 0) return query(__rightVersion, __left, __right);
            auto dfs = [&](auto self, _TpNode *cur1, _TpNode *cur2, uint64_t left, uint64_t right) -> _Tp {
                if (cur1 == cur2) return 0;
                if (left >= __left && right <= __right)
                    return cur1 ? cur2->val - cur1->val : cur2->val;
                else if (__right <= (left + right) / 2)
                    return self(self, cur1 ? cur1->lchild : nullptr, cur2->lchild, left, (left + right) / 2);
                else if (__left > (left + right) / 2)
                    return self(self, cur1 ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right);
                else
                    return self(self, cur1 ? cur1->lchild : nullptr, cur2->lchild, left, (left + right) / 2) + self(self, cur1 ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1);
        }
        template <typename _Judge>
        uint64_t periodMaxRight(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __left, _Judge __judge) const {
            if (__leftVersion == 0) return maxRight(__rightVersion, __left, __judge);
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _TpNode *cur1, _TpNode *cur2, uint64_t left, uint64_t right) {
                if (__left <= left)
                    if (_Tp a(m_op(val, cur2->val - cur1->val)); __judge(a)) {
                        val = a;
                        return right;
                    } else if (left == right)
                        return left - 1;
                if (__left <= (left + right) / 2)
                    if (uint64_t res = self(self, cur1->lchild, cur2->lchild, left, (left + right) / 2); res != (left + right) / 2) return res;
                return self(self, cur1->rchild, cur2->rchild, (left + right) / 2 + 1, right);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1);
        }
        template <typename _Judge>
        uint64_t periodMinLeft(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __right, _Judge __judge) const {
            if (__leftVersion == 0) return minLeft(__rightVersion, __right, __judge);
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _TpNode *cur1, _TpNode *cur2, uint64_t left, uint64_t right) {
                if (__right >= right)
                    if (_Tp a(m_op(cur2->val - cur1->val, val)); __judge(a)) {
                        val = a;
                        return left;
                    } else if (left == right)
                        return right + 1;
                if (__right > (left + right) / 2)
                    if (uint64_t res = self(self, cur1->rchild, cur2->rchild, (left + right) / 2 + 1, right); res != (left + right) / 2 + 1) return res;
                return self(self, cur1->lchild, cur2->lchild, left, (left + right) / 2);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1);
        }
        uint64_t periodKth(uint32_t __leftVersion, uint32_t __rightVersion, _Tp __k) const {
            if (__leftVersion == 0) return kth(__rightVersion, __k);
            auto dfs = [&](auto self, _TpNode *cur1, _TpNode *cur2, uint64_t left, uint64_t right, _Tp k) {
                if (left == right) return left;
                if (!cur1 || !cur1->lchild) {
                    if (!cur2->lchild)
                        return self(self, cur1 && cur1->rchild ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, k);
                    else if (cur2->lchild->val > k)
                        return self(self, nullptr, cur2->lchild, left, (left + right) / 2, k);
                    else
                        return self(self, cur1 && cur1->rchild ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, k - cur2->lchild->val);
                } else if (cur2->lchild->val - cur1->lchild->val > k)
                    return self(self, cur1->lchild, cur2->lchild, left, (left + right) / 2, k);
                else
                    return self(self, cur1->rchild, cur2->rchild, (left + right) / 2 + 1, right, k - cur2->lchild->val + cur1->lchild->val);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, __k);
        }
        uint32_t versionCount() const { return m_roots.size(); }
    };
    template <typename _Tp = int64_t>
    PersistentSegTree(uint64_t, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> PersistentSegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int64_t>
    PersistentSegTree(uint64_t, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    PersistentSegTree(uint64_t = 0, _Operation = _Operation(), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    PersistentSegTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp()) -> PersistentSegTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    PersistentSegTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>>
    PersistentSegTree(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp()) -> PersistentSegTree<_Tp, _Operation>;
}

#endif