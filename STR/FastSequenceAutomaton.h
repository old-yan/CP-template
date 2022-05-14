#ifndef __OY_FASTSEQUENCEAUTOMATON__
#define __OY_FASTSEQUENCEAUTOMATON__

#include <functional>
#include <numeric>
#include "Trie.h"

namespace OY {
    template <typename _Mapping = TrieLowerMapping, uint32_t _PoolSize = 1000000>
    struct FastSequenceAutomaton {
        static inline uint32_t s_next[_PoolSize][_Mapping::range()];
        static inline uint32_t s_length;
        static inline _Mapping s_map = _Mapping();
        template <typename _Iterator>
        static void Insert(_Iterator __first, _Iterator __last) {
            s_length = __last - __first;
            std::fill(s_next[s_length - 1], s_next[s_length], s_length);
            s_next[s_length - 1][s_map(__first[s_length - 1])]--;
            for (uint32_t i = s_length - 2; ~i; i--) {
                std::copy(s_next[i + 1], s_next[i + 2], s_next[i]);
                s_next[i][s_map(__first[i])] = i;
            }
        }
        template <typename _Tp>
        static uint32_t Next(uint32_t __current, _Tp __c) { return s_next[__current][s_map(__c)]; }
        template <typename _Iterator>
        static bool Has(_Iterator __first, _Iterator __last) {
            uint32_t current = 0;
            for (auto it = __first; it != __last; ++it)
                if (current = Next(current, *it) + 1; current > s_length) return false;
            return true;
        }
    };
}

#endif