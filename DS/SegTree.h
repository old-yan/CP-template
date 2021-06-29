#pragma once
#include <iostream>
#include <memory.h>
using namespace std;
#define OLDYAN_SEGTREE

//线段树
#define SEGTREEDEPTH 17
template<class T,class Accumulate=T(*)(T,T)>class SegTree{
    T data[1<<(SEGTREEDEPTH+1)],default_val;
    int X;
    Accumulate op;
    void update(int i){
        data[i]=op(data[i*2],data[i*2+1]);
    }
public:
    SegTree(int n,Accumulate _op,T _val=T()):op(_op),default_val(_val){
        reset(n);
    }
    template<class iterator>SegTree(iterator begin,iterator end,Accumulate _op,T _val=T()):op(_op),default_val(_val){
        reset(begin,end);
    }
    void reset(int n){
        X=1<<(32-__builtin_clz(n-1));
        for(int i=0;i<X;i++)data[X+i]=default_val;
        for(int i=X-1;i;i--)update(i);
    }
    template<class iterator>void reset(iterator begin,iterator end){
        int n=end-begin;
        X=1<<(32-__builtin_clz(n-1));
        for(int i=0;i<n;i++)data[X+i]=*(begin+i);
        fill(data+X+n,data+X*2,default_val);
        for(int i=X-1;i;i--)update(i);
    }
    void set(int i,T val){
        if(i<0||i>=X)return;i+=X;
        for(data[i]=val;i>>=1;)update(i);
    }
    void step(int i,T inc){
        if(i<0||i>=X)return;i+=X;
        for(data[i]=op(data[i],inc);i>>=1;)update(i);
    }
    T&operator[](int i){
        if(i<0||i>=X)return default_val;
        return data[i+X];
    }
    T operator()(int l,int r){
        l=max(l,0);r=min(r,X-1);
        if(l>r)return default_val;
        if(l==r)return data[l+X];
        int cur;l+=X;r+=X;T res=data[l];
        for(cur=0;l>>(cur+1)!=r>>(cur+1);cur++)if(!(l>>cur&1))res=op(res,data[l>>cur^1]);
        while(--cur>=0)if(r>>cur&1)res=op(res,data[r>>cur^1]);
        return op(res,data[r]);
    }
    T all(){
        return data[1];
    }
    int find_nth(T n){
        if(n<0||n>=data[1])return -1;
        int i=1;
        while(i<X)if(data[i<<=1]<n+1)n-=data[i++];
        return i-X;
    }
};

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
    LazyTree(int n,Accumulate _op):LazyTree(n,_op,_op,_op){}
    LazyTree(int n,Accumulate _op,Mapping _map,Composition _com,T _val=T(),F _inc=F()):op(_op),map(_map),com(_com),default_val(_val),default_inc(_inc){
        reset(n);
    }
    template<class iterator>LazyTree(iterator begin,iterator end,Accumulate _op):LazyTree(begin,end,_op,_op,_op){}
    template<class iterator>LazyTree(iterator begin,iterator end,Accumulate _op,Mapping _map,Composition _com,T _val=T(),F _inc=F()):op(_op),map(_map),com(_com),default_val(_val),default_inc(_inc){
        reset(begin,end);
    }
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
