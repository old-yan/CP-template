#pragma once
#include <iostream>
#include <memory.h>
using namespace std;
#define OLDYAN_LCTREE

//李超线段树
#define LCTREEDEPTH 17
template<class T,class Pick,class PartPick>
class LCTree{
    T val[1<<(LCTREEDEPTH+1)],default_val;
    int X,Y;
    Pick pick;
    PartPick partpick;
    void push_down(int i,int l,int r,T _val){
        int d=31-__builtin_clz(i),cl=(i&(1<<d)-1)<<(Y-d),cr=(((i&(1<<d)-1)+1)<<(Y-d))-1;
        l=max(l,cl),r=min(r,cr);
        if(l==cl&&r==cr){
            int res=partpick(_val,val[i],l,r);
            if(res&1)swap(val[i],_val);
            if(i<X)push_down(i<<1^(res>>1&1),l,r,_val);
        }
        else if(l<=cr&&r>=cl){
            push_down(i<<1,l,r,_val);
            push_down(i<<1^1,l,r,_val);
        }
    }
public:
    LCTree(int n,Pick _pick=Pick(),PartPick _partpick=PartPick(),T _default_val=T()):pick(_pick),partpick(_partpick),default_val(_default_val){
        reset(n);
    }
    void reset(int n){
        Y=(32-__builtin_clz(n-1)),X=1<<Y;
        fill(val,val+(X<<1),default_val);
    }
    void step(int l,int r,T _val){
        push_down(1,l,r,_val);
    }
    T operator[](int i){
        T res=default_val;
        for(int j=i+X;j;j>>=1)if(pick(val[j],res,i))res=val[j];
        return res;
    }
};