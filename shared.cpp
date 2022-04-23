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
using std::cin,std::cout,std::endl;int _IO=[]{std::ios::sync_with_stdio(0);cin.tie(0); cout.tie(0);
#ifdef OY_LOCAL
    freopen("in.txt","r",stdin);freopen("out.txt","w",stdout);
#endif
return 0;}();

int main() {
#ifdef OY_LOCAL
#if defined(_WIN32)
    struct TIME_AND_MEMORY {static auto GetMicroSecond() {static FILETIME ft;GetSystemTimeAsFileTime(&ft);return ft.dwLowDateTime;};static auto GetMemory() {PROCESS_MEMORY_COUNTERS pmc;GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));return pmc.WorkingSetSize;}uint32_t t0, t1;TIME_AND_MEMORY() : t0(GetMicroSecond()) {}void stop() {t1 = GetMicroSecond();cout << "\ntime cost = " << (t1 - t0) / 10000.0 << " ms\nmemory cost = " << (GetMemory() >> 20) << " MB\n ";t0 = t1;}~TIME_AND_MEMORY() { stop(); }} tam;
#else
    struct TIME_AND_MEMORY {static auto GetMicroSecond() {static timeval ft;gettimeofday(&ft, nullptr);return ft.tv_sec * 1000000 + ft.tv_usec;};static auto GetMemory() {char _mem_buff[512] = {0};int used, now_use;fgets(_mem_buff, sizeof(_mem_buff), fopen("/proc/self/statm", "r"));sscanf(_mem_buff, "%d %d", &used, &now_use);return now_use;}uint64_t t0, t1;TIME_AND_MEMORY() : t0(GetMicroSecond()) {}~TIME_AND_MEMORY() {t1 = GetMicroSecond();cout << "\ntime cost = "<<(t1-t0)/1000.0<<" ms\nmemory cost = "<<(GetMemory()/250)<<" MB\n ";}}tam;
#endif
#endif

    // union{
    //     int A;
    //     char c[4];
    // }u;
    // u.A=65504;
    // for(int i=0;i<4;i++)cout<<int(u.c[i])<<endl;
    
    char c[]="abddbd";
    char *d=(char*)memchr(c,0,6);
    if(d)cout<<int(d-c);
    else cout<<'b';


}