#ifndef __OY_SQRTTREE__
#define __OY_SQRTTREE__

#include <cstdint>
#include <functional>
#include "Accumulator.h"
#include "STtable.h"
#include "ZkwTree.h"

namespace OY {
    template <typename _Tp = int64_t, typename _Operation = std::plus<_Tp>>
    class SqrtTree {
        int m_length;
        int m_depth;
        int m_mask;
        int m_blockSize;
        int m_blockCount;
        std::vector<Accumulator<_Tp, _Operation, ACCUMULATE_PREFIX | ACCUMULATE_SUFFIX>> m_fix;
        STTable<_Tp, _Operation> m_inter;
        std::vector<ZkwTree<_Tp, _Operation>> m_subtree;
        _Operation m_op;
        _Tp m_defaultValue;
        void _check() {
            //assert(m_op(m_defaultValue, m_defaultValue) == m_defaultValue);
        }

    public:
        SqrtTree(int __n = 0, _Operation __op = _Operation(), _Tp __defaultValue = _Tp(), _Tp __initValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue), m_inter(0, __op, __defaultValue) {
            _check();
            resize(__n);
        }
        template <typename _Iterator>
        SqrtTree(_Iterator __first, _Iterator __last, _Operation __op = _Operation(), _Tp __defaultValue = _Tp()) : m_op(__op), m_defaultValue(__defaultValue), m_inter(0, __op, __defaultValue) {
            _check();
            reset(__first, __last);
        }
        void resize(int __n) {
            if (!__n) return;
            m_length = __n;
            m_depth = 32 - (m_length > 1 ? __builtin_clz(m_length - 1) : 32) >> 1;
            while (__builtin_ctz(m_length) != m_depth) m_length++;
            m_mask = (1 << m_depth) - 1;
            m_blockSize = 1 << m_depth;
            m_blockCount = m_length >> m_depth;
            m_fix.clear();
            m_fix.reserve(m_blockCount);
            m_fix.emplace_back(m_blockSize, m_op, m_defaultValue);
            for (int i = 1; i < m_blockCount; i++) m_fix.push_back(m_fix[0]);
            m_subtree.clear();
            m_subtree.reserve(m_blockCount);
            m_subtree.emplace_back(m_blockSize, m_op, m_defaultValue);
            for (int i = 1; i < m_blockCount; i++) m_subtree.push_back(m_subtree[0]);
            std::vector<_Tp> sub_val(m_blockCount, m_subtree[0].queryAll());
            m_inter.reset(sub_val.begin(), sub_val.end());
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_depth = 32 - (m_length > 1 ? __builtin_clz(m_length - 1) : 32) >> 1;
            while (__builtin_ctz(m_length) != m_depth) m_length++;
            std::vector<_Tp> data;
            data.reserve(m_length);
            data.insert(data.end(), __first, __last);
            data.insert(data.end(), m_length - (__last - __first), m_defaultValue);
            m_mask = (1 << m_depth) - 1;
            m_blockSize = 1 << m_depth;
            m_blockCount = m_length >> m_depth;
            m_fix.clear();
            m_fix.reserve(m_blockCount);
            for (int i = 0; i < m_blockCount; i++) m_fix.emplace_back(data.data() + i * m_blockSize, data.data() + (i + 1) * m_blockSize, m_op, m_defaultValue);
            m_subtree.clear();
            m_subtree.reserve(m_blockCount);
            for (int i = 0; i < m_blockCount; i++) m_subtree.emplace_back(data.data() + i * m_blockSize, data.data() + (i + 1) * m_blockSize, m_op);
            std::vector<_Tp> sub_val;
            for (int i = 0; i < m_blockCount; i++) sub_val.push_back(m_subtree[i].queryAll());
            m_inter.reset(sub_val.begin(), sub_val.end());
        }
        void update(int __i, _Tp __val) {
            m_fix[__i >> m_depth].update(__i & m_mask, __val);
            m_subtree[__i >> m_depth].update(__i & m_mask, __val);
            m_inter.update(__i >> m_depth, m_subtree[__i >> m_depth].queryAll());
        }
        _Tp query(int __i) const { return m_subtree[__i >> m_depth].query(__i & m_mask); }
        _Tp query(int __left, int __right) const {
            int l1 = __left >> m_depth, l2 = __left & m_mask;
            int r1 = __right >> m_depth, r2 = __right & m_mask;
            if (l1 == r1)
                return m_subtree[l1].query(l2, r2);
            else if (l1 + 1 == r1)
                return m_op(m_fix[l1].querySuffix(l2), m_fix[r1].queryPrefix(r2));
            else
                return m_op(m_op(m_fix[l1].querySuffix(l2), m_inter.query(l1 + 1, r1 - 1)), m_fix[r1].queryPrefix(r2));
        }
        _Tp queryAll() const { return m_inter.queryAll(); }
    };
    template <typename _Tp = int64_t>
    SqrtTree(int, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp(), _Tp = _Tp()) -> SqrtTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Tp = int64_t>
    SqrtTree(int, _Tp (*)(_Tp, _Tp), _Tp = _Tp(), _Tp = _Tp()) -> SqrtTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Operation = std::plus<int64_t>, typename _Tp = std::decay_t<typename decltype(std::mem_fn(&_Operation::operator()))::result_type>>
    SqrtTree(int = 0, _Operation = _Operation(), _Tp = _Tp(), _Tp = _Tp()) -> SqrtTree<_Tp, _Operation>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SqrtTree(_Iterator, _Iterator, const _Tp &(*)(const _Tp &, const _Tp &), _Tp = _Tp(), _Tp = _Tp()) -> SqrtTree<_Tp, const _Tp &(*)(const _Tp &, const _Tp &)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    SqrtTree(_Iterator, _Iterator, _Tp (*)(_Tp, _Tp), _Tp = _Tp(), _Tp = _Tp()) -> SqrtTree<_Tp, _Tp (*)(_Tp, _Tp)>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Operation = std::plus<_Tp>>
    SqrtTree(_Iterator, _Iterator, _Operation = _Operation(), _Tp = _Tp(), _Tp = _Tp()) -> SqrtTree<_Tp, _Operation>;
}

#endif