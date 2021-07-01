#pragma once
#include <iostream>
using namespace std;
#define OLDYAN_STTABLE

//ST表
#define STDEPTH 17
<<<<<<< HEAD
template<class T,class Max=T(*)(T,T)>struct STtable{
    T dp[1<<STDEPTH][STDEPTH];
    Max max;
    template<class iterator>
    STtable(iterator begin,iterator end,Max _max=Max()):max(_max){
        int n=end-begin,d=32-__builtin_clz(n-1);
        for(int i=0;i<d;i++)
=======
template<class T,class Max>struct STtable{
    T dp[1<<STDEPTH][STDEPTH+1];
    Max max;
    int n,d;
    template<class iterator>
    STtable(iterator begin,iterator end,Max _max=Max()):max(_max){
        n=end-begin;
        for(d=4;(1<<d)<end-begin;d++);
        for(int i=0;i<=d;i++)
>>>>>>> remotes/origin/main
            for(int j=0;j<=n-(1<<i);j++)
                dp[j][i]=i?max(dp[j][i-1],dp[j+(1<<i-1)][i-1]):*(begin+j);
    }
    T operator()(int l,int r){
<<<<<<< HEAD
        int d=31-__builtin_clz(r-l+1);
        return max(dp[l][d],dp[r-(1<<d)+1][d]);
    }
};

=======
        if(l==r)return dp[l][0];
        d=31-__builtin_clz(r-l+1);
        return max(dp[l][d],dp[r-(1<<d)+1][d]);
    }
};
>>>>>>> remotes/origin/main
