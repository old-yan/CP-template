#pragma once
#include <iostream>
#include <memory.h>
using namespace std;
#define OLDYAN_LAZYTREE

//懒惰标记线段树
#define LAZYTREEDEPTH 17
template<class T,class F=T,uint8_t bsize=1,class Accumulate=T(*)(T,T),class Mapping=T(*)(T,F),class Composition=F(*)(F,F)>class LazyTree{
    T val[1<<(LAZYTREEDEPTH+1)],default_val;
    F inc[1<<(LAZYTREEDEPTH+1)],default_inc;
    bool lazy[1<<(LAZYTREEDEPTH+1)];int X,Y;
    Accumulate op;Mapping map;Composition com;
    inline int size(int i){
        return 1<<(__builtin_clz(i)+Y-31);
    }
    void inherite(int i,F _inc){
        if constexpr(bsize)val[i]=map(val[i],_inc*size(i));
        else val[i]=map(val[i],_inc);
        if(i<X)inc[i]=com(inc[i],_inc),lazy[i]=true;
    }
    void update(int i){
        val[i]=op(val[i<<1],val[i<<1^1]);
    }
    void push_down(int i){
        if(!lazy[i])return;
        inherite(i<<1,inc[i]),inherite(i<<1^1,inc[i]);
        inc[i]=default_inc,lazy[i]=false;
    }
public:
    LazyTree(int n,Accumulate _op,Mapping _map,Composition _com,T _default_val=T(),F _default_inc=F()):op(_op),map(_map),com(_com),default_val(_default_val),default_inc(_default_inc){
        reset(n);
    }
    LazyTree(int n,Accumulate _op):LazyTree(n,_op,_op,_op){}
    template<class iterator>LazyTree(iterator begin,iterator end,Accumulate _op,Mapping _map,Composition _com,T _default_val=T(),F _default_inc=F()):op(_op),map(_map),com(_com),default_val(_default_val),default_inc(_default_inc){
        reset(begin,end);
    }
    template<class iterator>LazyTree(iterator begin,iterator end,Accumulate _op):LazyTree(begin,end,_op,_op,_op){}
    void reset(int n){
        Y=32-__builtin_clz(n-1),X=1<<Y;
        for(int i=0;i<X;i++)val[X+i]=default_val;
        for(int i=X-1;i;i--)update(i);
        fill(inc,inc+X,default_inc);
        fill(lazy,lazy+X,false);
    }
    template<class iterator>void reset(iterator begin,iterator end){
        int n=end-begin;
        Y=32-__builtin_clz(n-1),X=1<<Y;
        for(int i=0;i<n;i++)val[X+i]=*(begin+i);
        fill(val+X+n,val+X*2,default_val);
        for(int i=X-1;i;i--)update(i);
        fill(inc,inc+X,default_inc);
        fill(lazy,lazy+X,false);
    }
    void set(int i,T _val){
        if(i<0||i>=X)return;i+=X;
        for(int d=Y;d;d--)push_down(i>>d);
        for(val[i]=_val;i>>=1;update(i));
    }
    void step(int i,F _inc){
        if(i<0||i>=X)return;i+=X;
        for(int d=Y;d;d--)push_down(i>>d);
        for(inherite(i,_inc);i>>=1;update(i));
    }
    void step(int l,int r,F _inc){
        l=max(l,0),r=min(r,X-1);
        if(l>r)return;if(l==r){step(l,_inc);return;}
        l+=X,r+=X;
        for(int d=Y;d;d--)push_down(l>>d),push_down(r>>d);
        inherite(l,_inc),inherite(r,_inc);
        while((l>>1)!=(r>>1)){
            if(!(l&1))inherite(l^1,_inc);
            if(r&1)inherite(r^1,_inc);
            update(l>>=1),update(r>>=1);
        }
        while(l>>=1)update(l);
    }
    T&operator[](int i){
        if(i<0||i>=X)return default_val;i+=X;
        for(int d=Y;d;d--)push_down(i>>d);
        return val[i];
    }
    T operator()(int l,int r){
        l=max(l,0),r=min(r,X-1);
        if(l>r)return default_val;if(l==r)return (*this)[l];
        l+=X,r+=X;
        for(int d=Y;d;d--)push_down(l>>d),push_down(r>>d);
        int cur,d=32-__builtin_clz(l^r);T res=val[l];
        for(int i=0;i<d-1;i++)if(!(l>>i&1))res=op(res,val[l>>i^1]);
        for(int i=d-2;i>=0;i--)if(r>>i&1)res=op(res,val[r>>i^1]);
        return op(res,val[r]);
    }
    T all(){
        return val[1];
    }
    int find_nth(T n){
        if(n>=val[1])return -1;
        int i=1;
        while(i<X){
            push_down(i);
            if(val[i<<=1]<n+1)n-=val[i++];
        }
        return i-X;
    }
};