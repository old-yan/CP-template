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
    struct Manacher {
        int m_length;
        std::vector<int> m_oddArm;
        std::vector<int> m_evenArm;
        template <typename _Iterator>
        Manacher(_Iterator __first, _Iterator __last) {
            m_length = __last - __first;
            m_oddArm.reserve(m_length);
            m_evenArm.reserve(m_length);
            for (int l = -1, r = -1, i = 0; i < m_length; i++) {
                int k = i <= r ? std::min(m_oddArm[l + r - i], r - i) + 1 : 1;
                while (k <= i && k < m_length - i && __first[i - k] == __first[i + k]) k++;
                m_oddArm.push_back(k - 1);
                if (i + k - 1 > r) {
                    l = i - k + 1;
                    r = i + k - 1;
                }
            }
            for (int l = 0, r = -1, i = 0; i < m_length; i++) {
                int k = i <= r ? std::min(m_evenArm[l + r - i], r - i + 1) + 1 : 1;
                while (k <= i && k < m_length - i + 1 && __first[i - k] == __first[i + k - 1]) k++;
                m_evenArm.push_back(k - 1);
                if (i + k - 2 > r) {
                    l = i - k + 1;
                    r = i + k - 2;
                }
            }
        }
        bool query(int __left, int __right) const {
            if (__left ^ __right & 1)
                return m_evenArm[(__left + __right + 1) >> 1] > (__right - __left) >> 1;
            else
                return m_oddArm[(__left + __right) >> 1] >= (__right - __left) >> 1;
        }
    };
}

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
    std::string s;
    cin >> s;
    OY::Manacher mc(s.begin(), s.end());
    auto a = *std::max_element(mc.m_oddArm.begin(), mc.m_oddArm.end());
    auto b = *std::max_element(mc.m_evenArm.begin(), mc.m_evenArm.end());
    cout << std::max(a * 2 + 1, b * 2);
}