#pragma once
#include <iostream>
#include <numeric>
using namespace std;
#define OLDYAN_UNION

//并查集
#define UNIONSIZE 1000000
struct Union{
    int find[UNIONSIZE],size[UNIONSIZE],n,group;
    Union(int _n=-1){
        reset(_n);
    }
    void reset(int _n=-1){
        group=n=_n<0?UNIONSIZE:_n;
        iota(find,find+n,0);
        fill(size,size+n,1);
    }
    int Find(int i){
        return find[i]==i?i:find[i]=Find(find[i]);
    }
    bool unite(int i,int j){
        if((i=Find(i))==(j=Find(j)))return false;
        if(i<j)find[j]=i,size[i]+=size[j];
        else find[i]=j,size[j]+=size[i];
        group--;
        return true;
    }
    bool same(int i,int j){
        return Find(i)==Find(j);
    }
};