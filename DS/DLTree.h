#pragma once
#include <iostream>
#include <memory.h>
using namespace std;
#define OLDYAN_DLTREE

//动态开点懒惰标记线段树
#define DLTREESIZE 4000000
template<class T,class F=T,uint8_t bsize=1,class Accumulate=T(*)(T,T),class Mapping=T(*)(T,F),class Composition=F(*)(F,F)>class DLTree{
    T val[DLTREESIZE],default_val;
    F inc[DLTREESIZE],default_inc;
    bool lazy[DLTREESIZE];int ch[DLTREESIZE][2],pa[DLTREESIZE],sz[DLTREESIZE],X,Y,cnt=0;
    Accumulate op;Mapping map;Composition com;
    void ini(int i,int p){
        lazy[i]=false,ch[i][0]=ch[i][1]=-1,pa[i]=p,sz[i]=p>=0?sz[p]>>1:X,val[i]=default_val,inc[i]=default_inc;
    }
    int Ch(int cur,int i){
        if(ch[cur][i]<0)ini(ch[cur][i]=cnt++,cur);
        return ch[cur][i];
    }
    void inherite(int i,F _inc){
        if constexpr(bsize)val[i]=map(val[i],_inc*sz[i]);
        else val[i]=map(val[i],_inc);
        inc[i]=com(inc[i],_inc),lazy[i]=true;
    }
    int push_down(int i){
        int cur=0;
        for(int j=Y;j--;cur=Ch(cur,i>>j&1)){
            if(lazy[cur]){
                inherite(Ch(cur,0),inc[cur]);
                inherite(Ch(cur,1),inc[cur]);
                lazy[cur]=false,inc[cur]=default_inc;
            }
        }
        return cur;
    }
    void update(int i){
        val[i]=op(val[Ch(i,0)],val[Ch(i,1)]);
    }
public:
    DLTree(int n,Accumulate _op):DLTree(n,_op,_op,_op){}
    DLTree(int n,Accumulate _op,Mapping _map,Composition _com,T _val=T(),F _inc=F()):op(_op),map(_map),com(_com),default_val(_val),default_inc(_inc){
        reset(n);
    }
    template<class iterator>DLTree(iterator begin,iterator end,Accumulate _op):DLTree(begin,end,_op,_op,_op){}
    template<class iterator>DLTree(iterator begin,iterator end,Accumulate _op,Mapping _map,Composition _com,T _val=T(),F _inc=F()):op(_op),map(_map),com(_com),default_val(_val),default_inc(_inc){
        reset(begin,end);
    }
    void reset(int n){
        Y=32-__builtin_clz(n-1),X=1<<Y;
        ini(0,-1);cnt=1;
    }
    template<class iterator>void reset(iterator begin,iterator end){
        int n=end-begin;
        Y=32-__builtin_clz(n-1),X=1<<Y;
        ini(0,-1);cnt=1;
        for(int i=0;i<n;i++)set(i,*(begin+i));
    }
    void set(int i,T _val){
        if(i<0||i>=X)return;
        for(val[i=push_down(i)]=_val;~pa[i];i=pa[i])update(pa[i]);
    }
    void step(int i,F _inc){
        if(i<0||i>=X)return;
        for(inherite(i=push_down(i),_inc);~pa[i];i=pa[i])update(pa[i]);
    }
    void step(int l,int r,F _inc){
        l=max(l,0),r=min(r,X-1);
        if(l>r)return;if(l==r){step(l,_inc);return;}
        inherite(l=push_down(l),_inc);
        inherite(r=push_down(r),_inc);
        for(;pa[l]!=pa[r];l=pa[l],r=pa[r]){
            if(l==ch[pa[l]][0])inherite(Ch(pa[l],1),_inc);
            if(r==ch[pa[r]][1])inherite(Ch(pa[r],0),_inc);
            update(pa[l]),update(pa[r]);
        }
        while(~pa[l])update(pa[l]),l=pa[l];
    }
    T&operator[](int i){
        return val[push_down(i)];
    }
    T operator()(int l,int r){
        l=max(l,0),r=min(r,X-1);
        if(l>r)return default_val;if(l==r)return (*this)[l];
        int d=32-__builtin_clz(l^r);l=push_down(l),push_down(r);T res=val[l];
        for(int i=0;i<d-1;i++,l=pa[l])if(l==ch[pa[l]][0])res=op(res,val[Ch(pa[l],1)]);
        l=ch[pa[l]][1];
        for(int i=d-2;i>=0;l=ch[l][r>>i--&1])if(r>>i&1)res=op(res,val[ch[l][0]]);
        return op(res,val[l]);
    }
    T all(){
        return val[0];
    }
};
