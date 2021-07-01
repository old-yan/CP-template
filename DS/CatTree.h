#pragma once
#include <iostream>
using namespace std;
#define OLDYAN_CATTREE

//ST表
#define CATTREEDEPTH 17
template<class T,class Max=T(*)(T,T)>struct CatTree{
    T dp[CATTREEDEPTH+1][1<<(CATTREEDEPTH+1)];
    Max max;
    template<class iterator>CatTree(iterator begin,iterator end,Max _max=Max()):max(_max){
        int n=end-begin,d=32-__builtin_clz(n-1);
        for(int i=0,j=1;i<=d;i++,j<<=1)
            for(int k1=0,k2=min(k1+j,n);k1<n;k1+=j,k2=min(k1+j,n)){
                for(int l=k1;l<k2;l++)dp[i][l<<1]=l==k1?*(begin+l):max(dp[i][l-1<<1],*(begin+l));
                for(int l=k2-1;l>=k1;l--)dp[i][l<<1^1]=l==k2-1?*(begin+l):max(dp[i][l+1<<1^1],*(begin+l));
            }
    }
    T operator()(int l,int r){
        if(l==r)return dp[0][l<<1];
        int d=31-__builtin_clz(l^r);
        return max(dp[d][l<<1^1],dp[d][r<<1]);
    }
};
