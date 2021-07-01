#pragma once
#include <iostream>
using namespace std;
#define OLDYAN_LINKLISTNODE

template<class T>
struct LinklistNode{
    T val;
    LinklistNode*prev,*next;
    LinklistNode():prev(nullptr),next(nullptr){}
    LinklistNode(T _val):val(_val),prev(nullptr),next(nullptr){}
    LinklistNode(T _val,LinklistNode*_next):val(_val),prev(nullptr),next(_next){}
    LinklistNode(T _val,LinklistNode*_prev,LinklistNode*_next):val(_val),prev(_prev),next(_next){}
};