#pragma once
#include <iostream>
#include <memory.h>
using namespace std;
#define OLDYAN_LCTREE

//李超线段树
#define LCTREEDEPTH 17
template<class line,class Cmp=bool(*)(line,line,int)>
class LCTree{
    line val[1<<(LCTREEDEPTH+1)],default_val;
    Cmp comp;
    int X,Y;
    void push_down(int i,int l,int r,line _val){
        int d=31-__builtin_clz(i),cl=(i&(1<<d)-1)<<(Y-d),cr=(((i&(1<<d)-1)+1)<<(Y-d))-1;
        l=max(l,cl),r=min(r,cr);
        if(l==cl&&r==cr){
            if(comp(val[i],_val,l+r>>1))swap(val[i],_val);
            if(i<X){
                if(comp(val[i],_val,r))push_down(i<<1^1,l,r,_val);
                else push_down(i<<1,l,r,_val);
            }
        }
        else if(l<=cr&&r>=cl){
            push_down(i<<1,l,r,_val);
            push_down(i<<1^1,l,r,_val);
        }
    }
public:
    LCTree(int n,Cmp _comp,line _default_val=line()):comp(_comp),default_val(_default_val){
        reset(n);
    }
    void reset(int n){
        Y=(32-__builtin_clz(n-1)),X=1<<Y;
        fill(val,val+(X<<1),default_val);
    }
    void step(int l,int r,line _val){
        push_down(1,l,r,_val);
    }
    line operator[](int i){
        line res=default_val;
        for(int j=i+X;j;j>>=1)if(comp(res,val[j],i))res=val[j];
        return res;
    }
};