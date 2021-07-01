#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory.h>
#include <functional>
using namespace std;
#define OLDYAN_HEAP

//二叉堆
#define HEAPSIZE 100000
template<class T,class Cmp=less<T>,uint8_t mask=0>class BiHeap;
template<class T,class Cmp>class BiHeap<T,Cmp,0>{
    Cmp comp;
    int heap[HEAPSIZE+1],pos[HEAPSIZE]={0},Size=0;
public:
    BiHeap(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        memset(pos,0,sizeof(pos));
    }
    void push(int val){
        if(!pos[val])pos[val]=++Size,heap[Size]=val;
        for(int cur=pos[val],par;(par=cur>>1)&&comp(heap[par],val);)
            swap(heap[cur],heap[par]),swap(pos[heap[cur]],pos[heap[par]]);
    }
    void emplace(int val){
        push(val);
    }
    void sink(int val){
        if(!pos[val])return;
        for(int cur=pos[val],son;(son=cur<<1)<=Size;cur=son){
            if(son+1<=Size&&comp(heap[son],heap[son+1]))son++;
            if(!comp(val,heap[son]))break;
            swap(heap[cur],heap[son]),swap(pos[heap[cur]],pos[heap[son]]);
        }
    }
    int&top(){
        return heap[1];
    }
    void pop(){
        if(!Size)return;
        pos[heap[1]]=0;
        if(!--Size)return;
        pos[heap[1]=heap[Size+1]]=1;
        sink(heap[1]);
    }
    int size(){
        return Size;
    }
    bool empty(){
        return !Size;
    }
};
template<class T,class Cmp>class BiHeap<T,Cmp,1>{
    int heap[HEAPSIZE+1],pos[HEAPSIZE],cnt=0;
    T datamap[HEAPSIZE];
    Cmp comp;
    unordered_map<T,int>idmap;
    int makeid(const T&val){
        if(idmap.count(val))return idmap[val];
        int id=cnt++;
        idmap[val]=id;
        datamap[id]=val;
        pos[id]=idmap.size();
        return heap[idmap.size()]=id;
    }
public:
    BiHeap(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        idmap.clear();
        cnt=0;
    }
    void push(const T&val){
        emplace(val);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        int id=makeid(T(forward<_Args>(__args)...));
        T&val=datamap[id];
        for(int cur=pos[id],par;(par=cur>>1)&&comp(datamap[heap[par]],val);)
            swap(heap[cur],heap[par]),swap(pos[heap[cur]],pos[heap[par]]);
    }
    void sink(const T&val){
        if(!idmap.count(val))return;
        int id=idmap[val];
        for(int cur=pos[id],son;(son=cur<<1)<=idmap.size();cur=son){
            if(son+1<=idmap.size()&&comp(datamap[heap[son]],datamap[heap[son+1]]))son++;
            if(!comp(val,datamap[heap[son]]))break;
            swap(heap[cur],heap[son]),swap(pos[heap[cur]],pos[heap[son]]);
        }
    }
    T&top(){
        return datamap[heap[1]];
    }
    void pop(){
        if(idmap.empty())return;
        idmap.erase(datamap[heap[1]]);
        if(idmap.empty())return;
        heap[1]=heap[idmap.size()+1];
        pos[heap[1]]=1;
        sink(datamap[heap[1]]);
    }
    int size(){
        return idmap.size();
    }
    bool empty(){
        return idmap.empty();
    }
};
template<class T,class Cmp>class BiHeap<T,Cmp,2>{
    T heap[HEAPSIZE+1];
    Cmp comp;
    int Size=0;
public:
    BiHeap(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        Size=0;
    }
    void push(const T&val){
        heap[++Size]=val;
        for(int cur=Size,par;(par=cur>>1)&&comp(heap[par],val);cur=par)
            swap(heap[cur],heap[par]);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        heap[++Size]=T(forward<_Args>(__args)...);
        for(int cur=Size,par;(par=cur>>1)&&comp(heap[par],heap[cur]);cur=par)
            swap(heap[cur],heap[par]);
    }
    void sink(){
        for(int cur=1,son;(son=cur<<1)<=Size;cur=son){
            if(son+1<=Size&&comp(heap[son],heap[son+1]))son++;
            if(!comp(heap[cur],heap[son]))break;
            swap(heap[cur],heap[son]);
        }
    }
    T&top(){
        return heap[1];
    }
    void pop(){
        heap[1]=heap[Size--];
        sink();
    }
    int size(){
        return Size;
    }
    bool empty(){
        return !Size;
    }
};

//配对堆
#define HEAPSIZE 100000
template<class T,class Cmp=less<T>,uint8_t mask=0>class PairHeap;
template<class T,class Cmp>class PairHeap<T,Cmp,0>{
    Cmp comp;
    int ch[HEAPSIZE],xd[HEAPSIZE],fa[HEAPSIZE],root=-1,Size=0;
    int merge(int x,int y){
        if(x<0)return y;if(y<0)return x;
        if(comp(x,y))swap(x,y);
        xd[y]=ch[x];if(ch[x]>=0)fa[ch[x]]=y;
        ch[x]=y;fa[y]=x;
        return x;
    }
    int merges(int x) {
        if(x<0)return x;
        fa[x]==1;
        if (xd[x]<0)return x;
        int a=xd[x],b=xd[a];
        xd[x]=xd[a]=fa[a]=0;
        return merge(merge(x,a),merges(b));
    }
public:
    PairHeap(Cmp _comp=Cmp()):comp(_comp){
        clear();
    }
    void clear(){
        root=-1,Size=0;
        memset(fa,0xff,sizeof(fa));
    }
    void push(int val){
        if(fa[val]<0){
            if(root==val)return;
            ch[val]=xd[val]=fa[val]=-1,Size++;
        }
        else{
            if(val==ch[fa[val]])ch[fa[val]]=xd[val];
            else xd[fa[val]]=xd[val];
            if(xd[val]>=0)fa[xd[val]]=fa[val],xd[val]=-1;
            fa[val]=-1;
        }
        root=merge(root,val);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        push(forward<_Args>(__args)...);
    }
    int&top(){
        return root;
    }
    void pop(){
        if(root<0)return;
        root=merges(ch[root]);
        Size--;
    }
    int size(){
        return Size;
    }
    bool empty(){
        return !Size;
    }
};
template<class T,class Cmp>class PairHeap<T,Cmp,1>{
    T heap[HEAPSIZE+1];
    Cmp comp;
    int ch[HEAPSIZE+1],xd[HEAPSIZE+1],fa[HEAPSIZE+1],root=0,cnt=0;
    unordered_map<T,int>idmap;
    int merge(int x,int y){
        if(!x||!y)return x|y;
        if(comp(heap[x],heap[y]))swap(x,y);
        xd[y]=ch[x];if(ch[x])fa[ch[x]]=y;
        ch[x]=y;fa[y]=x;
        return x;
    }
    int merges(int x) {
        if(!x)return x;
        fa[x]=0;
        if (!xd[x])return x;
        int a=xd[x],b=xd[a];
        xd[x]=xd[a]=fa[a]=0;
        return merge(merge(x,a),merges(b));
    }
public:
    PairHeap(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        idmap.clear();
        root=0,cnt=0;
    }
    void push(const T&val){
        int id;
        if(!idmap.count(val)){
            id=++cnt;
            heap[id]=val;idmap[val]=id;
            ch[id]=xd[id]=fa[id]=0;
        }
        else id=idmap[val];
        if(fa[id]){
            if(id==ch[fa[id]])ch[fa[id]]=xd[id];
            else xd[fa[id]]=xd[id];
            if(xd[id])fa[xd[id]]=fa[id],xd[id]=0;
            fa[id]=0;
        }
        if(root!=id)root=merge(root,id);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        heap[cnt+1]=T(forward<_Args>(__args)...);
        int id;
        if(!idmap.count(heap[cnt+1])){
            id=++cnt;
            idmap[heap[id]]=id;
            ch[id]=xd[id]=fa[id]=0;
        }
        else id=idmap[heap[cnt+1]];
        if(fa[id]){
            if(id==ch[fa[id]])ch[fa[id]]=xd[id];
            else xd[fa[id]]=xd[id];
            if(xd[id])fa[xd[id]]=fa[id],xd[id]=0;
            fa[id]=0;
        }
        if(root!=id)root=merge(root,id);
    }
    T&top(){
        return heap[root];
    }
    void pop(){
        if(!root)return;
        idmap.erase(heap[root]);
        root=merges(ch[root]);
    }
    int size(){
        return idmap.size();
    }
    bool empty(){
        return idmap.empty();
    }
};
template<class T,class Cmp>class PairHeap<T,Cmp,2>{
    T heap[HEAPSIZE+1];
    Cmp comp;
    int ch[HEAPSIZE+1],xd[HEAPSIZE+1],fa[HEAPSIZE+1],root=0,cnt=0,Size=0;
    int merge(int x,int y){
        if(!x||!y)return x|y;
        if(comp(heap[x],heap[y]))swap(x,y);
        xd[y]=ch[x];if(ch[x])fa[ch[x]]=y;
        ch[x]=y;fa[y]=x;
        return x;
    }
    int merges(int x) {
        if(!x)return x;
        fa[x]=0;
        if (!xd[x])return x;
        int a=xd[x],b=xd[a];
        xd[x]=xd[a]=fa[a]=0;
        return merge(merge(x,a),merges(b));
    }
public:
    PairHeap(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        root=0,cnt=0,Size=0;
    }
    void push(const T&val){
        emplace(val);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        int id=++cnt;
        heap[id]=T(forward<_Args>(__args)...);
        ch[id]=xd[id]=fa[id]=0;
        root=merge(root,id);
        Size++;
    }
    T&top(){
        return heap[root];
    }
    void pop(){
        if(!root)return;
        root=merges(ch[root]);
        Size--;
    }
    int size(){
        return Size;
    }
    bool empty(){
        return !Size;
    }
};

//左偏树
#define HEAPSIZE 100000
template<class T,class Cmp=less<T>,uint8_t mask=0>class LeftistTree;
template<class T,class Cmp>class LeftistTree<T,Cmp,0>{
    Cmp comp;
    int lc[HEAPSIZE],rc[HEAPSIZE],_dist[HEAPSIZE+1]={0},root,Size;
    int*dist=_dist+1;
    int merge(int x,int y){
        if(x<0)return y;if(y<0)return x;
        if(comp(x,y))swap(x,y);
        rc[x]=merge(rc[x],y);
        if(dist[rc[x]]>dist[lc[x]])swap(lc[x],rc[x]);
        dist[x]=dist[rc[x]]+1;
        return x;
    }
public:
    LeftistTree(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        root=-1,Size=0;
        memset(_dist,0,sizeof(_dist));
    }
    void push(int val){
        if(!dist[val]){
            if(root==val)return;
            lc[val]=rc[val]=-1,dist[val]=1,Size++;
        }
        if(root!=val)root=merge(root,val);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        push(forward<_Args>(__args)...);
    }
    int&top(){
        return root;
    }
    void pop(){
        if(root<0)return;
        dist[root]=0;
        root=merge(lc[root],rc[root]);
        Size--;
    }
    int size(){
        return Size;
    }
    bool empty(){
        return !Size;
    }
};
template<class T,class Cmp>class LeftistTree<T,Cmp,1>{
    T heap[HEAPSIZE+1];
    Cmp comp;
    int lc[HEAPSIZE+1],rc[HEAPSIZE+1],dist[HEAPSIZE+1]={0},root=0,cnt=0;
    unordered_map<T,int>idmap;
    int merge(int x,int y){
        if(!x||!y)return x|y;
        if(comp(heap[x],heap[y]))swap(x,y);
        rc[x]=merge(rc[x],y);
        if(dist[rc[x]]>dist[lc[x]])swap(lc[x],rc[x]);
        dist[x]=dist[rc[x]]+1;
        return x;
    }
public:
    LeftistTree(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        idmap.clear();
        root=0,cnt=0;
    }
    void push(const T&val){
        int id;
        if(!idmap.count(val)){
            id=++cnt;
            heap[id]=val;idmap[val]=id;
            lc[id]=rc[id]=0;dist[id]=1;
        }
        else id=idmap[val];
        if(root!=id)root=merge(root,id);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        heap[cnt+1]=T(forward<_Args>(__args)...);
        int id;
        if(!idmap.count(heap[cnt+1])){
            id=++cnt;
            idmap[heap[id]]=id;
            lc[id]=rc[id]=0;dist[id]=1;
        }
        else id=idmap[heap[cnt+1]];
        if(root!=id)root=merge(root,id);
    }
    T&top(){
        return heap[root];
    }
    void pop(){
        if(!root)return;
        idmap.erase(heap[root]);
        root=merge(lc[root],rc[root]);
    }
    int size(){
        return idmap.size();
    }
    bool empty(){
        return idmap.empty();
    }
};
template<class T,class Cmp>class LeftistTree<T,Cmp,2>{
    T heap[HEAPSIZE+1];
    Cmp comp;
    int lc[HEAPSIZE+1],rc[HEAPSIZE+1],dist[HEAPSIZE+1]={0},root=0,cnt=0,Size=0;
    int merge(int x,int y){
        if(!x||!y)return x|y;
        if(comp(heap[x],heap[y]))swap(x,y);
        rc[x]=merge(rc[x],y);
        if(dist[rc[x]]>dist[lc[x]])swap(lc[x],rc[x]);
        dist[x]=dist[rc[x]]+1;
        return x;
    }
public:
    LeftistTree(Cmp _comp=Cmp()):comp(_comp){}
    void clear(){
        root=0,cnt=0,Size=0;
    }
    void push(const T&val){
        emplace(val);
    }
    template<class... _Args>void emplace(_Args&&... __args){
        int id=++cnt;
        heap[id]=T(forward<_Args>(__args)...);
        lc[id]=rc[id]=0;
        dist[id]=1;
        root=merge(root,id);
        Size++;
    }
    T&top(){
        return heap[root];
    }
    void pop(){
        if(!root)return;
        root=merge(lc[root],rc[root]);
        Size--;
    }
    int size(){
        return Size;
    }
    bool empty(){
        return !Size;
    }
};
