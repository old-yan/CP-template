#ifndef __OY_LEFTIST__
#define __OY_LEFTIST__

#include "MemoryPool.h"
#include <functional>

namespace OY {
    template <typename _Tp, typename _Compare = std::less<_Tp>>
    class Leftist {
        struct node : MemoryPool<node> {
            _Tp val;
            int s;
            node *lchild;
            node *rchild;
            node(_Tp _val) : val(_val), s(1), lchild(nullptr), rchild(nullptr) {}
        };
        _Compare m_comp;
        node *m_root;
        int m_size;
        static int node_s(node *x) { return x ? x->s : 0; }
        node *merge(node *a, node *b) {
            if (!b) return a;
            if (m_comp(a->val, b->val)) std::swap(a, b);
            a->rchild = merge(b, a->rchild);
            if (a->rchild->s > node_s(a->lchild)) std::swap(a->lchild, a->rchild);
            a->s = a->lchild->s + 1;
            return a;
        }

    public:
        Leftist(_Compare __comp = _Compare()) : m_root(nullptr), m_size(0), m_comp(__comp) {}
        template <typename _Iterator>
        Leftist(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_root(nullptr), m_size(0), m_comp(__comp) { reset(__first, __last); }
        ~Leftist() { clear(); }
        void clear() {
            // auto dfs = [&](auto self, node *cur) -> void {
            //     if (cur->lchild) self(self, cur->lchild);
            //     if (cur->rchild) self(self, cur->rchild);
            //     delete cur;
            // };
            // if (m_root) dfs(dfs, m_root);
            m_root = nullptr;
            m_size = 0;
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            while (__first < __last) push(*__first++);
        }
        void push(_Tp __val) {
            m_size++;
            m_root = merge(new node(__val), m_root);
        }
        void pop() {
            m_size--;
            delete m_root;
            m_root = m_root->lchild ? merge(m_root->lchild, m_root->rchild) : nullptr;
        }
        _Tp top() const { return m_root->val; }
        void join(Leftist<_Tp, _Compare> &other) {
            if (!other.m_root) return;
            m_root = m_root ? merge(m_root, other.m_root) : other.m_root;
            other.m_root = nullptr;
            m_size += other.m_size;
            other.m_size = 0;
        }
        int size() const { return m_size; }
        bool empty() const { return !m_size; }
    };
    template <typename _Compare = std::less<int>, typename _Tp = std::decay_t<typename decltype(std::function(std::declval<_Compare>()))::first_argument_type>>
    Leftist(_Compare = _Compare()) -> Leftist<_Tp, _Compare>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Compare = std::less<_Tp>>
    Leftist(_Iterator, _Iterator, _Compare = _Compare()) -> Leftist<_Tp, _Compare>;
}

#endif