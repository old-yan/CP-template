#ifndef __OY_BINOMIALHEAP__
#define __OY_BINOMIALHEAP__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, typename _Compare = std::less<_Tp>>
    class BinomialHeap {
        struct node : MemoryPool<node> {
            _Tp val;
            int deg;
            node *sub;
            node *bro;
            node(_Tp _val = _Tp()) : val(_val), deg(0), sub(nullptr), bro(nullptr) {}
        };
        _Compare m_comp;
        node *m_dummy = nullptr;
        int m_size;
        node *merge(node *a, node *b) {
            if (m_comp(a->val, b->val)) std::swap(a, b);
            if (!a->sub)
                a->sub = b;
            else {
                node *p = a->sub;
                while (p->bro) p = p->bro;
                p->bro = b;
            }
            a->bro = b->bro = nullptr;
            a->deg++;
            return a;
        }
        void _join(node *s1, node *s2) {
            node *s = m_dummy;
            node *p = nullptr;
            auto append = [&](node *&t) {
                s->bro = t;
                t = t->bro;
                s = s->bro;
                s->bro = nullptr;
            };
            auto merge_s1 = [&] {
                node *t = s1;
                s1 = s1->bro;
                p = merge(p, t);
            };
            while (s1 || s2) {
                if (s1 && s2 && s1->deg == s2->deg) {
                    if (p) append(p);
                    p = s2;
                    s2 = s2->bro;
                    merge_s1();
                } else {
                    if (!s1 || (s2 && s1->deg > s2->deg)) std::swap(s1, s2);
                    if (p && p->deg == s1->deg)
                        merge_s1();
                    else {
                        if (p) append(p);
                        append(s1);
                    }
                }
            }
            if (p) append(p);
        }

    public:
        BinomialHeap(_Compare __comp = _Compare()) : m_dummy(new node), m_size(0), m_comp(__comp) {}
        template <typename _Iterator>
        BinomialHeap(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_size(0), m_comp(__comp) { reset(__first, __last); }
        ~BinomialHeap() { clear(); }
        void clear() {
            // auto dfs = [&](auto self, node *cur) -> void {
            //     for (node *p = cur->sub; p; p = p->bro) self(self, p);
            //     if (cur->bro) self(self, cur->bro);
            //     delete cur;
            // };
            // if (m_dummy->bro) dfs(dfs, m_dummy->bro);
            m_dummy->bro = nullptr;
            m_size = 0;
        }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            while (__first < __last) push(*__first++);
        }
        void push(_Tp __val) {
            m_size++;
            node *p = new node(__val);
            while (m_dummy->bro && m_dummy->bro->deg == p->deg) {
                node *q = m_dummy->bro;
                m_dummy->bro = q->bro;
                p = merge(p, q);
            }
            p->bro = m_dummy->bro;
            m_dummy->bro = p;
        }
        _Tp pop() {
            m_size--;
            node *res = nullptr;
            for (node *p = m_dummy->bro; p; p = p->bro)
                if (!res || m_comp(res->val, p->val)) res = p;
            node *p = m_dummy;
            while (p->bro != res) p = p->bro;
            p->bro = res->bro;
            _join(m_dummy->bro, res->sub);
            return res->val;
        }
        _Tp top() const {
            node *res = nullptr;
            for (node *p = m_dummy->bro; p; p = p->bro)
                if (!res || m_comp(res->val, p->val)) res = p;
            return res->val;
        }
        void join(BinomialHeap<_Tp, _Compare> &other) {
            _join(m_dummy->bro, other.m_dummy->bro);
            m_size += other.m_size;
        }
        int size() const { return m_size; }
        bool empty() const { return !m_size; }
    };
    template <typename _Compare = std::less<int>, typename _Tp = std::decay_t<typename decltype(std::function(std::declval<_Compare>()))::first_argument_type>>
    BinomialHeap(_Compare = _Compare()) -> BinomialHeap<_Tp, _Compare>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Compare = std::less<_Tp>>
    BinomialHeap(_Iterator, _Iterator, _Compare = _Compare()) -> BinomialHeap<_Tp, _Compare>;
}

#endif