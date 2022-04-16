#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_set>
#if defined(_WIN32)
#include <windows.h>
// split to make windows.h before psapi.h
#include <psapi.h>
#else
#include "sys/time.h"
#endif
using std::cin, std::cout, std::endl;
int _IO = [] {std::ios::sync_with_stdio(0);cin.tie(0); cout.tie(0);
#ifdef OY_LOCAL
    freopen("in.txt","r",stdin);freopen("out.txt","w",stdout);
#endif
return 0; }();

namespace OY {
    struct TrieDigitMapping {
        static constexpr int range() { return 10; }
        int operator()(char __c) const { return __c - '0'; }
    };
    struct TrieLowerMapping {
        static constexpr int range() { return 26; }
        int operator()(char __c) const { return __c - 'a'; }
    };
    struct TrieUpperMapping {
        static constexpr int range() { return 26; }
        int operator()(char __c) const { return __c - 'A'; }
    };
    struct TrieDefaultInfo {
        bool m_isEnd = false;
    };
    template <typename _Mapping = TrieLowerMapping, typename _Info = TrieDefaultInfo, uint32_t _PoolSize = 1000000>
    struct Trie {
        struct TrieNode;
        static inline _Mapping s_map = _Mapping();
        static inline TrieNode s_parent[_PoolSize];
        static inline TrieNode s_child[_PoolSize][_Mapping::range()];
        static inline _Info s_info[_PoolSize];
        static inline uint32_t s_cursor = 1;
        struct TrieNode {
            uint32_t index;
            TrieNode() = default;
            TrieNode(uint32_t index) : index(index) {}
            constexpr TrieNode &parent() { return s_parent[index]; }
            constexpr TrieNode &child_Unchecked(uint32_t i) { return s_child[index][i]; }
            constexpr TrieNode &child_Checked(uint32_t i) {
                if (!s_child[index][i]) {
                    s_parent[s_cursor] = index;
                    s_child[index][i] = s_cursor++;
                }
                return s_child[index][i];
            }
            template <typename _Iterator>
            constexpr TrieNode insert(_Iterator first, _Iterator last) {
                if (first == last)
                    return *this;
                else
                    return child_Checked(s_map(*first)).insert(first + 1, last);
            }
            template <typename _Sequence>
            constexpr TrieNode insert(_Sequence &sequence) { return insert(sequence.begin(), sequence.end()); }
            template <typename _Iterator>
            constexpr TrieNode find(_Iterator first, _Iterator last) const {
                if (first == last)
                    return *this;
                else if (child_Unchecked(s_map(*first)))
                    return child_Unchecked(s_map(*first)).find(first + 1, last);
                else
                    return 0;
            }
            template <typename _Sequence>
            constexpr TrieNode find(_Sequence &sequence) const { return find(sequence.begin(), sequence.end()); }
            constexpr _Info *operator->() const { return s_info + index; }
            explicit operator bool() const { return index; }
            operator uint32_t() const { return index; }
        };
        TrieNode m_root;
        Trie() : m_root(s_cursor++) {}
        template <typename _Iterator>
        TrieNode insert(_Iterator __first, _Iterator __last) { return m_root.insert(__first, __last); }
        template <typename _Sequence>
        TrieNode insert(_Sequence &__sequence) { return m_root.insert(__sequence.begin(), __sequence.end()); }
        template <typename _Iterator>
        TrieNode find(_Iterator __first, _Iterator __last) const { return m_root.find(__first, __last); }
        template <typename _Sequence>
        TrieNode find(_Sequence &__sequence) const { return m_root.find(__sequence.begin(), __sequence.end()); }
    };
    template <typename _Mapping = TrieLowerMapping, typename _Info = TrieDefaultInfo, uint32_t _PoolSize = 1000000>
    Trie() -> Trie<_Mapping, _Info, _PoolSize>;
}
namespace OY {
    struct ACDefaultInfo {
        uint32_t m_value = 0;
        uint32_t m_deg=0;
    };
    template <typename _Mapping = TrieLowerMapping, typename _Info = ACDefaultInfo, uint32_t _PoolSize = 100>
    struct ACAutomaton {
        struct _ACInfo : _Info {
            uint32_t m_fail;
        };
        static inline Trie<_Mapping, _ACInfo, _PoolSize> s_trie;
        using TrieNode = typename Trie<_Mapping, _ACInfo, _PoolSize>::TrieNode;
        template <typename _Sequence, typename _Operation>
        static void Init(const std::vector<_Sequence> &__sequences, _Operation __op) {
            s_trie = Trie<_Mapping, _ACInfo, _PoolSize>();
            for (uint32_t i = 0; i < _Mapping::range(); i++) TrieNode(0).child_Unchecked(i) = s_trie.m_root;
            static struct {
                uint32_t index;
                TrieNode node;
            } queue[_PoolSize];
            uint32_t head = 0, tail = 0;
            auto push = [&](uint32_t index, TrieNode cur) { queue[tail++] = {index, cur}; };
            for (uint32_t i = 0; i < __sequences.size(); i++) push(i, s_trie.m_root);
            for (uint32_t len = 1; uint32_t levelSize = tail - head; len++) {
                while (levelSize--) {
                    auto [index, cur] = queue[head++];
                    uint32_t i = Trie<_Mapping, _ACInfo, _PoolSize>::s_map(__sequences[index][len - 1]);
                    TrieNode it = cur->m_fail;
                    while (!it.child_Unchecked(i)) it = it->m_fail;
                    cur.child_Checked(i)->m_fail = it.child_Unchecked(i);
                    if (__sequences[index].size() > len)
                        push(index, cur.child_Unchecked(i));
                    else
                        __op(cur.child_Unchecked(i), index);
                }
            }
        }
        template <typename _Sequence, typename _Operation>
        static void solve(const _Sequence &__sequence, _Operation __op) {
            TrieNode cur = s_trie.m_root;
            for (auto &c : __sequence) {
                while (!cur.child_Unchecked(Trie<_Mapping, _ACInfo, _PoolSize>::s_map(c))) cur = cur->m_fail;
                cur = cur.child_Unchecked(Trie<_Mapping, _ACInfo, _PoolSize>::s_map(c));
                __op(cur);
            }
        }
    };
};

int main() {
#ifdef OY_LOCAL
#if defined(_WIN32)
    struct TIME_AND_MEMORY {
        static auto GetMicroSecond() {
            static FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            return ft.dwLowDateTime;
        };
        static auto GetMemory() {
            PROCESS_MEMORY_COUNTERS pmc;
            GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
            return pmc.WorkingSetSize;
        }
        uint32_t t0, t1;
        TIME_AND_MEMORY() : t0(GetMicroSecond()) {}
        void stop() {
            t1 = GetMicroSecond();
            cout << "\ntime cost = " << (t1 - t0) / 10000.0 << " ms\nmemory cost = " << (GetMemory() >> 20) << " MB\n ";
            t0 = t1;
        }
        ~TIME_AND_MEMORY() { stop(); }
    } tam;
#else
    struct TIME_AND_MEMORY {
        static auto GetMicroSecond() {
            static timeval ft;
            gettimeofday(&ft, nullptr);
            return ft.tv_sec * 1000000 + ft.tv_usec;
        };
        static auto GetMemory() {
            char _mem_buff[512] = {0};
            int used, now_use;
            fgets(_mem_buff, sizeof(_mem_buff), fopen("/proc/self/statm", "r"));
            sscanf(_mem_buff, "%d %d", &used, &now_use);
            return now_use;
        }
        uint64_t t0, t1;
        TIME_AND_MEMORY() : t0(GetMicroSecond()) {}
        ~TIME_AND_MEMORY() {
            t1 = GetMicroSecond();
            cout << "\ntime cost = " << (t1 - t0) / 1000.0 << " ms\nmemory cost = " << (GetMemory() / 250) << " MB\n ";
        }
    } tam;
#endif
#endif
    while(true){
        uint32_t n;
        cin >> n;
        if(!n)break;
        std::vector<std::string> s(n);
        for (int i = 0; i < n; i++) cin >> s[i];

        std::string S;
        cin >> S;
        using node = OY::ACAutomaton<OY::TrieLowerMapping, OY::ACDefaultInfo, 1000000>::TrieNode;
        node pos[n];
        auto op1 = [&](node &cur, uint32_t index) { pos[index] = cur; };
        node begin = OY::ACAutomaton<OY::TrieLowerMapping, OY::ACDefaultInfo, 1000000>::s_trie.s_cursor;
        OY::ACAutomaton<OY::TrieLowerMapping, OY::ACDefaultInfo, 1000000>::Init(s, op1);
        node end = OY::ACAutomaton<OY::TrieLowerMapping, OY::ACDefaultInfo, 1000000>::s_trie.s_cursor;
        auto op2 = [&](node cur) {cur->m_value++;};
        OY::ACAutomaton<OY::TrieLowerMapping, OY::ACDefaultInfo, 1000000>::solve(S, op2);

        for (node i = begin; i < end; i.index++) {
            node fail = i->m_fail;
            fail->m_deg++;
        }
        node Q[end.index];
        uint32_t head = 0, tail = 0;
        for (node i = begin; i < end; i.index++) {
            if (!i->m_deg) Q[tail++] = i;
        }
        while (head < tail) {
            node i = Q[head++];
            node fail = i->m_fail;
            fail->m_value += i->m_value;
            if (!--(fail->m_deg)) Q[tail++] = fail;
        }
        int ans[end.index];
        std::fill(ans,ans+end,0);
        for (uint32_t i = 0; i < n; i++) ans[i]=pos[i]->m_value;
        auto mx=std::max_element(ans,ans+end.index);
        cout<<*mx<<'\n';
        for(uint32_t i=0;i<n;i++)if(ans[i]==*mx)cout<<s[i]<<'\n';
    }
}