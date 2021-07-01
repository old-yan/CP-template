#pragma once
#include <iostream>
#include <memory.h>
using namespace std;
#define OLDYAN_SEGTREE

//线段树
#define SEGTREEDEPTH 17
template<class T,class Accumulate=T(*)(T,T)>class SegTree{
    T val[1<<(SEGTREEDEPTH+1)],default_val;
    int X;
    Accumulate op;
    void update(int i){
        val[i]=op(val[i*2],val[i*2+1]);
    }
public:
    SegTree(int n,Accumulate _op=Accumulate(),T _default_val=T()):op(_op),default_val(_default_val){
        reset(n);
    }
    template<class iterator>SegTree(iterator begin,iterator end,Accumulate _op=Accumulate(),T _default_val=T()):op(_op),default_val(_default_val){
        reset(begin,end);
    }
    void reset(int n){
        X=1<<(32-__builtin_clz(n-1));
        for(int i=0;i<X;i++)val[X+i]=default_val;
        for(int i=X-1;i;i--)update(i);
    }
    template<class iterator>void reset(iterator begin,iterator end){
        int n=end-begin;
        X=1<<(32-__builtin_clz(n-1));
        for(int i=0;i<n;i++)val[X+i]=*(begin+i);
        fill(val+X+n,val+X*2,default_val);
        for(int i=X-1;i;i--)update(i);
    }
    void set(int i,T _val){
        if(i<0||i>=X)return;i+=X;
        for(val[i]=_val;i>>=1;)update(i);
    }
    void step(int i,T inc){
        if(i<0||i>=X)return;i+=X;
        for(val[i]=op(val[i],inc);i>>=1;)update(i);
    }
    T&operator[](int i){
        if(i<0||i>=X)return default_val;
        return val[i+X];
    }
    T operator()(int l,int r){
        l=max(l,0);r=min(r,X-1);
        if(l>r)return default_val;
        if(l==r)return val[l+X];
        int cur;l+=X;r+=X;T res=val[l];
        for(cur=0;l>>(cur+1)!=r>>(cur+1);cur++)if(!(l>>cur&1))res=op(res,val[l>>cur^1]);
        while(--cur>=0)if(r>>cur&1)res=op(res,val[r>>cur^1]);
        return op(res,val[r]);
    }
    T all(){
        return val[1];
    }
    int find_nth(T n){
        if(n<0||n>=val[1])return -1;
        int i=1;
        while(i<X)if(val[i<<=1]<n+1)n-=val[i++];
        return i-X;
    }
};
