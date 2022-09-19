#ifndef __OY_BITRIE__
#define __OY_BITRIE__

#include <algorithm>
#include <tuple>
#include "../STR/Trie.h"

namespace OY {
    struct BiTrieDefaultInfo {};
    template <typename _Tp, uint8_t _L>
    struct BiTrieIteration {
        _Tp m_number;
        struct BiTrieIterator {
            _Tp number;
            uint32_t index;
            BiTrieIterator(_Tp _number, uint32_t _index) : number(_number), index(_index) {}
            BiTrieIterator &operator++() {
                index--;
                return *this;
            }
            BiTrieIterator operator+(uint32_t __i) const { return BiTrieIterator(number, index - __i); }
            bool operator*() const { return number >> index & 1; }
            bool operator==(const BiTrieIterator &_other) const { return !~index; }
            bool operator!=(const BiTrieIterator &_other) const { return ~index; }
        };
        BiTrieIteration(_Tp __number) : m_number(__number) {}
        BiTrieIterator begin() const { return BiTrieIterator(m_number, _L - 1); }
        BiTrieIterator end() const { return BiTrieIterator(m_number, -1); }
    };
    template <typename _Tp = uint32_t, uint8_t _L = 30, typename _Info = BiTrieDefaultInfo, uint32_t _PoolSize = 2000000>
    struct BiTrie {
        struct BiTrieMapping {
            static constexpr int range() { return 2; }
            uint32_t operator()(bool __c) const { return __c; }
        };
        static inline Trie<BiTrieMapping, _Info, _PoolSize> s_trie;
        using TrieNode = typename Trie<BiTrieMapping, _Info, _PoolSize>::TrieNode;
        static void Init() { s_trie = Trie<BiTrieMapping, _Info, _PoolSize>(); }
        static TrieNode Insert(const BiTrieIteration<_Tp, _L> &__numberIteration) { return s_trie.insert(__numberIteration); }
        static std::pair<TrieNode, _Tp> QueryMaxSame(_Tp __number) {
            TrieNode cur = s_trie.m_root;
            _Tp res(0);
            for (bool c : BiTrieIteration<_Tp, _L>(__number)) {
                res *= 2;
                if (TrieNode child = cur.child(c)) {
                    cur = child;
                    res++;
                } else
                    cur = cur.child(c ^ 1);
            }
            return {cur, res};
        }
        static std::pair<TrieNode, _Tp> QueryMaxBitxor(_Tp __number) {
            __number ^= (_Tp(1) << _L) - 1;
            TrieNode cur = s_trie.m_root;
            _Tp res(0);
            for (bool c : BiTrieIteration<_Tp, _L>(__number)) {
                res *= 2;
                if (TrieNode child = cur.child(c)) {
                    cur = child;
                    res++;
                } else
                    cur = cur.child(c ^ 1);
            }
            return {cur, res};
        }
        static _Tp QueryLeafValue(TrieNode __leaf) {
            TrieNode cur = __leaf;
            _Tp res(0);
            for (uint32_t i = 0; i < _L; i++) {
                TrieNode parent = cur.parent();
                if (cur == parent.child(1)) res |= _Tp(1) << i;
                cur = parent;
            }
            return res;
        }
    };
}

#endif