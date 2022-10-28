#ifndef __OY_PERSISTENTLAZYSEGTREE__
#define __OY_PERSISTENTLAZYSEGTREE__

#include <cstdint>
#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Fp>
    struct PersistentLazySegAdd {
        _Tp operator()(const _Fp &__x, const _Tp &__y, uint64_t __size) const { return __x * __size + __y; }
    };
    template <typename _Tp = int64_t, typename _Fp = _Tp, typename _Operation = std::plus<_Tp>, typename _Mapping = PersistentLazySegAdd<_Tp, _Fp>, typename _Composition = std::plus<_Fp>>
    struct PersistentLazySegTree {
        struct _Tp_FpNode : MemoryPool<_Tp_FpNode> {
            _Tp val;
            _Fp inc;
            _Tp_FpNode *lchild, *rchild;
            _Tp_FpNode(_Tp _val, _Fp _inc, _Tp_FpNode *_lchild, _Tp_FpNode *_rchild) : val(_val), inc(_inc), lchild(_lchild), rchild(_rchild) {}
        };
        std::vector<_Tp_FpNode *> m_roots;
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
        _Tp_FpNode *lchild(_Tp_FpNode *cur) {
            if (!cur->lchild) cur->lchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            return cur->lchild;
        }
        _Tp_FpNode *rchild(_Tp_FpNode *cur) {
            if (!cur->rchild) cur->rchild = new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr);
            return cur->rchild;
        }
        _Tp _map(_Fp inc, _Tp val, uint64_t length) const {
            if constexpr (std::is_invocable_v<_Mapping, _Fp, _Tp, uint64_t>)
                return m_map(inc, val, length);
            else
                return m_map(inc, val);
        }
        _Tp_FpNode *_update(_Tp_FpNode *cur, uint64_t length) {
            cur->val = _map(cur->inc, m_op(cur->lchild ? cur->lchild->val : m_defaultValue, cur->rchild ? cur->rchild->val : m_defaultValue), length);
            return cur;
        }
        _Tp_FpNode *_root(uint32_t version) const { return ~version ? m_roots[version] : m_roots.back(); }
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
        static void setBufferSize(uint32_t __count) { MemoryPool<_Tp_FpNode>::_reserve(__count); }
        PersistentLazySegTree(uint64_t __length = 0, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            resize(__length);
        }
        template <typename _Iterator>
        PersistentLazySegTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Mapping __map = _Mapping(), _Composition __com = _Composition(), _Tp __defaultValue = _Tp(), _Fp __defaultIncrement = _Fp()) : m_op(__op), m_map(__map), m_com(__com), m_defaultValue(__defaultValue), m_defaultIncrement(__defaultIncrement) {
            _check();
            reset(__first, __last);
        }
        ~PersistentLazySegTree() { _clear(); }
        void resize(uint64_t __length) {
            _clear();
            if (m_length = __length) m_roots.push_back(new _Tp_FpNode(m_defaultValue, m_defaultIncrement, nullptr, nullptr));
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
        void copyVersion(uint32_t __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        void add(uint32_t __prevVersion, uint64_t __i, _Fp __inc) {
            auto dfs = [&](auto self, _Tp_FpNode *prev, uint64_t left, uint64_t right) -> _Tp_FpNode * {
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
        void add(uint32_t __prevVersion, uint64_t __left, uint64_t __right, _Fp __inc) {
            auto dfs = [&](auto self, _Tp_FpNode *prev, uint64_t left, uint64_t right) -> _Tp_FpNode * {
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
        _Tp query(uint32_t __version, uint64_t __i) const {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right, _Fp inc) {
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
        _Tp query(uint32_t __version, uint64_t __left, uint64_t __right) const {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right, _Fp inc) {
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
        _Tp queryAll(uint32_t __version) const { return _root(__version)->val; }
        template <typename _Judge>
        uint64_t maxRight(uint32_t __version, uint64_t __left, _Judge __judge) const {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right, _Fp inc) {
                if (__left <= left)
                    if (_Tp a(m_op(val, _map(inc, cur->val, right - left + 1))); __judge(a)) {
                        val = a;
                        return right;
                    } else if (left == right)
                        return left - 1;
                inc = m_com(cur->inc, inc);
                if (__left <= (left + right) / 2)
                    if (uint64_t res = self(self, cur->lchild, left, (left + right) / 2, inc); res != (left + right) / 2) return res;
                return self(self, cur->rchild, (left + right) / 2 + 1, right, inc);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1, m_defaultIncrement);
        }
        template <typename _Judge>
        uint64_t minLeft(uint32_t __version, uint64_t __right, _Judge __judge) const {
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right, _Fp inc) {
                if (__right >= right)
                    if (_Tp a(m_op(_map(inc, cur->val, right - left + 1), val)); __judge(a)) {
                        val = a;
                        return left;
                    } else if (left == right)
                        return right + 1;
                inc = m_com(cur->inc, inc);
                if (__right > (left + right) / 2)
                    if (uint64_t res = self(self, cur->rchild, (left + right) / 2 + 1, right, inc); res != (left + right) / 2 + 1) return res;
                return self(self, cur->lchild, left, (left + right) / 2, inc);
            };
            return dfs(dfs, _root(__version), 0, m_length - 1, m_defaultIncrement);
        }
        uint64_t kth(uint32_t __version, _Tp __k) const {
            auto dfs = [&](auto self, _Tp_FpNode *cur, uint64_t left, uint64_t right, _Tp k, _Fp inc) {
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
        _Tp periodQuery(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __i) const {
            if (__leftVersion == 0) return query(__rightVersion, __i);
            auto dfs = [&](auto self, _Tp_FpNode *cur1, _Tp_FpNode *cur2, uint64_t left, uint64_t right, _Fp inc1, _Fp inc2) {
                if (cur1 == cur2) return 0;
                if (left == right)
                    return _map(inc2, cur2->val, right - left + 1) - _map(inc1, cur1 ? cur1->val : m_defaultValue, right - left + 1);
                else {
                    inc1 = m_com(cur1 ? cur1->inc : m_defaultIncrement, inc1), inc2 = m_com(cur2->inc, inc2);
                    if (__i <= (left + right) / 2)
                        return self(self, cur1 ? cur1->lchild : nullptr, cur2->lchild, left, (left + right) / 2, inc1, inc2);
                    else
                        return self(self, cur1 ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, inc1, inc2);
                }
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, m_defaultIncrement, m_defaultIncrement);
        }
        _Tp periodQuery(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __left, uint64_t __right) const {
            if (__leftVersion == 0) return query(__rightVersion, __left, __right);
            auto dfs = [&](auto self, _Tp_FpNode *cur1, _Tp_FpNode *cur2, uint64_t left, uint64_t right, _Fp inc1, _Fp inc2) -> _Tp {
                if (cur1 == cur2) return 0;
                if (left >= __left && right <= __right)
                    return _map(inc2, cur2->val, right - left + 1) - _map(inc1, cur1 ? cur1->val : m_defaultValue, right - left + 1);
                else {
                    inc1 = m_com(cur1 ? cur1->inc : m_defaultIncrement, inc1), inc2 = m_com(cur2->inc, inc2);
                    if (__right <= (left + right) / 2)
                        return self(self, cur1 ? cur1->lchild : nullptr, cur2->lchild, left, (left + right) / 2, inc1, inc2);
                    else if (__left > (left + right) / 2)
                        return self(self, cur1 ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, inc1, inc2);
                    else
                        return m_op(self(self, cur1 ? cur1->lchild : nullptr, cur2->lchild, left, (left + right) / 2, inc1, inc2), self(self, cur1 ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, inc1, inc2));
                }
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, m_defaultIncrement, m_defaultIncrement);
        }
        template <typename _Judge>
        uint64_t periodMaxRight(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __left, _Judge __judge) const {
            if (!__leftVersion) return maxRight(__rightVersion, __left, __judge);
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _Tp_FpNode *cur1, _Tp_FpNode *cur2, uint64_t left, uint64_t right, _Fp inc1, _Fp inc2) {
                if (__left <= left)
                    if (_Tp a(m_op(val, _map(inc2, cur2->val, right - left + 1) - _map(inc1, cur1 ? cur1->val : m_defaultValue, right - left + 1))); __judge(a)) {
                        val = a;
                        return right;
                    } else if (left == right)
                        return left - 1;
                inc1 = m_com(cur1 ? cur1->inc : m_defaultIncrement, inc1), inc2 = m_com(cur2->inc, inc2);
                if (__left <= (left + right) / 2)
                    if (uint64_t res = self(self, cur1->lchild, cur2->lchild, left, (left + right) / 2, inc1, inc2); res != (left + right) / 2) return res;
                return self(self, cur1->rchild, cur2->rchild, (left + right) / 2 + 1, right, inc1, inc2);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, m_defaultIncrement, m_defaultIncrement);
        }
        template <typename _Judge>
        uint64_t periodMinLeft(uint32_t __leftVersion, uint32_t __rightVersion, uint64_t __right, _Judge __judge) const {
            if (!__leftVersion) return maxRight(__rightVersion, __right, __judge);
            _Tp val(m_defaultValue);
            auto dfs = [&](auto self, _Tp_FpNode *cur1, _Tp_FpNode *cur2, uint64_t left, uint64_t right, _Fp inc1, _Fp inc2) {
                if (__right >= right)
                    if (_Tp a(m_op(_map(inc2, cur2->val, right - left + 1) - _map(inc1, cur1 ? cur1->val : m_defaultValue, right - left + 1), val)); __judge(a)) {
                        val = a;
                        return left;
                    } else if (left == right)
                        return right + 1;
                inc1 = m_com(cur1 ? cur1->inc : m_defaultIncrement, inc1), inc2 = m_com(cur2->inc, inc2);
                if (__right > (left + right) / 2)
                    if (uint64_t res = self(self, cur1->rchild, cur2->rchild, (left + right) / 2 + 1, right, inc1, inc2); res != (left + right) / 2 + 1) return res;
                return self(self, cur1->lchild, cur2->lchild, left, (left + right) / 2, inc1, inc2);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, m_defaultIncrement, m_defaultIncrement);
        }
        uint64_t periodKth(uint32_t __leftVersion, uint32_t __rightVersion, _Tp __k) const {
            if (__leftVersion == 0) return kth(__rightVersion, __k);
            auto dfs = [&](auto self, _Tp_FpNode *cur1, _Tp_FpNode *cur2, uint64_t left, uint64_t right, _Tp k, _Fp inc1, _Fp inc2) {
                if (left == right) return left;
                inc1 = m_com(cur1 ? cur1->inc : m_defaultIncrement, inc1), inc2 = m_com(cur2->inc, inc2);
                if (!cur1 || !cur1->lchild) {
                    if (!cur2->lchild)
                        return self(self, cur1 && cur1->rchild ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, k, inc1, inc2);
                    else if (_Tp lval(_map(inc2, cur2->lchild->val, (left + right) / 2 - left + 1)); lval > k)
                        return self(self, nullptr, cur2->lchild, left, (left + right) / 2, k, inc1, inc2);
                    else
                        return self(self, cur1 && cur1->rchild ? cur1->rchild : nullptr, cur2->rchild, (left + right) / 2 + 1, right, k - lval, inc1, inc2);
                } else if (_Tp lval(_map(inc2, cur2->lchild->val, (left + right) / 2 - left + 1) - _map(inc1, cur1->lchild->val, (left + right) / 2 - left + 1)); lval > k)
                    return self(self, cur1->lchild, cur2->lchild, left, (left + right) / 2, k, inc1, inc2);
                else
                    return self(self, cur1->rchild, cur2->rchild, (left + right) / 2 + 1, right, k - lval, inc1, inc2);
            };
            return dfs(dfs, _root(__leftVersion - 1), _root(__rightVersion), 0, m_length - 1, __k, m_defaultIncrement, m_defaultIncrement);
        }
        uint32_t versionCount() const { return m_roots.size(); }
    };
    template <typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    PersistentLazySegTree(uint64_t = 0, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> PersistentLazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
    template <typename _Iterator, typename _Operation, typename _Mapping, typename _Composition, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>, typename _Fp = std::decay_t<typename decltype(std::mem_fn(&_Composition::operator()))::result_type>>
    PersistentLazySegTree(_Iterator, _Iterator, _Operation = _Operation(), _Mapping = _Mapping(), _Composition = _Composition(), _Tp = _Tp(), _Fp = _Fp()) -> PersistentLazySegTree<_Tp, _Fp, _Operation, _Mapping, _Composition>;
}

#endif