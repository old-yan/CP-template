#pragma once
#include <iostream>
using namespace std;
#define OLDYAN_COMBINATION

//组合数计算器
template<int range=1000000,int P=1000000007>class CombinationHelper{
    int factorial[range+1],inverse[range+1],factorial_inverse[range+1];
    int cur,all;
public:
    CombinationHelper():cur(1),all(0){
        for(int i=0;i<=range;i++)factorial[i]=i?1ll*factorial[i-1]*i%P:1;
        for(int i=1;i<=range;i++)inverse[i]=i>1?1ll*(P-P/i)*inverse[P%i]%P:1;
        for(int i=range;i>=0;i--)factorial_inverse[i]=i==range?inv(factorial[i]):1ll*factorial_inverse[i+1]*(i+1)%P;
    }
    int pow(int a, int n){
        int res=1;
        for(;n;n>>=1,a=1ll*a*a%P)if(n&1)res=1ll*res*a%P;
        return res;
    }
    int f(int a){
        return factorial[a];
    }
    int inv(int a){
        return a>range?pow(a,P-2):inverse[a];
    }
    int finv(int a){
        return factorial_inverse[a];
    }
    int comb(int m,int n){
        return n>m||n<0?0:1ll*f(m)*finv(n)%P*finv(m-n)%P;
    }
    void operator<<(int a){
        cur=1ll*cur*finv(a)%P,all+=a;
    }
    template<class T>void operator>>(T&a){
        a=1ll*a*cur%P*f(all)%P,cur=1,all=0;
    }
};