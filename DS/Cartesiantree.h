/*
最后修改:
20230922
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_CARTESIANTREE__
#define __OY_CARTESIANTREE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>

namespace OY {
    namespace Cartesian {
        using size_type = uint32_t;
        template <typename Tp, size_type MAX_NODE>
        struct SolverHelper {
            struct node {
                size_type m_index, m_rchild;
                Tp m_value;
            };
            static node s_stack[MAX_NODE];
        };
        template <typename Tp, size_type MAX_NODE>
        typename SolverHelper<Tp, MAX_NODE>::node SolverHelper<Tp, MAX_NODE>::s_stack[MAX_NODE];
        template <typename Tp, size_type MAX_NODE = 1 << 20, typename InitMapping, typename LchildCallback, typename RchildCallback, typename Compare = std::less<Tp>>
        size_type solve(size_type length, InitMapping mapping, LchildCallback &&lchild_call, RchildCallback &&rchild_call, Compare comp = Compare(), const Tp &max = std::numeric_limits<Tp>::max()) {
            SolverHelper<Tp, MAX_NODE>::s_stack[0].m_index = -1;
            SolverHelper<Tp, MAX_NODE>::s_stack[0].m_value = max;
            size_type len = 1;
            for (size_type i = 0; i < length; i++) {
                Tp x = mapping(i);
                size_type last = -1;
                while (comp(SolverHelper<Tp, MAX_NODE>::s_stack[len - 1].m_value, x)) {
                    last = SolverHelper<Tp, MAX_NODE>::s_stack[--len].m_index;
                    rchild_call(last, SolverHelper<Tp, MAX_NODE>::s_stack[len].m_rchild);
                }
                SolverHelper<Tp, MAX_NODE>::s_stack[len - 1].m_rchild = i;
                lchild_call(i, last);
                SolverHelper<Tp, MAX_NODE>::s_stack[len].m_index = i;
                SolverHelper<Tp, MAX_NODE>::s_stack[len].m_value = x;
                SolverHelper<Tp, MAX_NODE>::s_stack[len].m_rchild = -1;
                len++;
            }
            while (len > 1) {
                size_type last = SolverHelper<Tp, MAX_NODE>::s_stack[--len].m_index;
                rchild_call(last, SolverHelper<Tp, MAX_NODE>::s_stack[len].m_rchild);
            }
            return SolverHelper<Tp, MAX_NODE>::s_stack[0].m_rchild;
        }
        template <size_type MAX_NODE = 1 << 20, typename Iterator, typename LchildCallback, typename RchildCallback, typename Tp = typename std::iterator_traits<Iterator>::value_type, typename Compare = std::less<Tp>>
        size_type solve(Iterator first, Iterator last, LchildCallback &&lchild_call, RchildCallback &&rchild_call, Compare comp = Compare(), const Tp &max = std::numeric_limits<Tp>::max()) {
            return solve<Tp, MAX_NODE>(
                last - first, [&](size_type i) { return *(first + i); }, lchild_call, rchild_call, comp, max);
        }
    }
}

#endif