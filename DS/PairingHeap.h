#ifndef __OY_PAIRINGHEAP__
#define __OY_PAIRINGHEAP__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Compare = std::less<_Tp>>
    class PairingHeap {
        struct node : MemoryPool<node> {
            _Tp val;
            node *sub;
            node *bro;
            node(_Tp _val) : val(_val), sub(nullptr), bro(nullptr) {}
        };
        _Compare m_comp;
        node *m_root;
        int m_size;
        node *merge(node *a, node *b) {
            if (m_comp(a->val, b->val)) std::swap(a, b);
            if (a->sub) b->bro = a->sub;
            a->sub = b;
            return a;
        }
        node *merges(node *x) {
            if (!x || !x->bro) return x;
            node *a = x->bro;
            node *b = a->bro;
            x->bro = a->bro = nullptr;
            return b ? merge(merge(x, a), merges(b)) : merge(x, a);
        }

    public:
        PairingHeap(_Compare __comp = _Compare()) : m_root(nullptr), m_size(0), m_comp(__comp) {}
        template <typename _Iterator>
        PairingHeap(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_root(nullptr), m_size(0), m_comp(__comp) { reset(__first, __last); }
        ~PairingHeap() { clear(); }
        void clear() {
            // auto dfs = [&](auto self, node *cur) {
            //     if (cur->bro) self(self, cur->bro);
            //     if (cur->sub) self(self, cur->sub);
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
            m_root = m_root ? merge(m_root, new node(__val)) : new node(__val);
        }
        void pop() {
            m_size--;
            delete m_root;
            m_root = merges(m_root->sub);
        }
        _Tp top() const { return m_root->val; }
        void join(PairingHeap<_Tp, _Compare> &other) {
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
    PairingHeap(_Compare = _Compare()) -> PairingHeap<_Tp, _Compare>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Compare = std::less<_Tp>>
    PairingHeap(_Iterator, _Iterator, _Compare = _Compare()) -> PairingHeap<_Tp, _Compare>;
}

#endif