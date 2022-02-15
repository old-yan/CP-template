#ifndef __OY_FHQTREAPARRAY__
#define __OY_FHQTREAPARRAY__

#include <functional>
#include <random>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp>
    class FHQTreapArray {
        struct node : MemoryPool<node> {
            _Tp key;
            bool reversed;
            uint_fast32_t priority;
            int subtree_weight;
            node *lchild;
            node *rchild;
            void reverse() { reversed = reversed ? false : true; }
            void push_down() {
                reverse();
                std::swap(lchild, rchild);
                if (lchild) lchild->reverse();
                if (rchild) rchild->reverse();
            }
        };
        node *m_root;
        static inline std::mt19937 s_rand;
        static int subtree_weight(node *p) { return p ? p->subtree_weight : 0; }
        static uint_fast32_t subtree_priority(node *p) { return p ? p->priority : 0; }
        static node *update(node *p) {
            p->subtree_weight = 1 + subtree_weight(p->lchild) + subtree_weight(p->rchild);
            return p;
        }
        static void split_l_r(node *p, int k, node *child[2]) {
            if (!p) return;
            if (p->reversed) p->push_down();
            if (!k) { //全部归大边
                child[0] = nullptr;
                child[1] = p;
            } else if (k == p->subtree_weight) {
                child[0] = p;
                child[1] = nullptr;
            } else if (subtree_weight(p->lchild) > k) { //小边够分，小边分大小
                split_l_r(p->lchild, k, child);
                p->lchild = child[1];
                child[1] = update(p);
            } else if (k -= subtree_weight(p->lchild); k) { //小边正好够
                split_l_r(p->rchild, k - 1, child);         //小边不够，分大边
                p->rchild = child[0];
                child[0] = update(p);
            } else {
                child[0] = p->lchild;
                p->lchild = nullptr;
                child[1] = update(p);
            }
        }
        static void split_l_m_r(node *p, int left, int right, node *child[3]) {
            split_l_r(p, right + 1, child + 1);
            split_l_r(child[1], left, child);
        }
        static node *merge(node *l, node *r) {
            if (!l) return r;
            if (!r) return l;
            if (l->priority > r->priority) {
                if (l->reversed) l->push_down();
                l->rchild = merge(l->rchild, r);
                return update(l);
            } else {
                if (r->reversed) r->push_down();
                r->lchild = merge(l, r->lchild);
                return update(r);
            }
        }
        template <typename _Iterator>
        node *make_tree(_Iterator first, _Iterator last) {
            auto _make_tree = [](auto self, _Iterator first, _Iterator last) -> node * {
                if (first == last) return nullptr;
                if (first + 1 == last) return new node{{}, *first, false, s_rand(), 1, nullptr, nullptr};
                _Iterator mid = first + (last - first) / 2;
                node *p = new node{{}, *mid, false, 0, 1, self(self, first, mid), self(self, mid + 1, last)};
                return update(p);
            };
            node *root = _make_tree(_make_tree, first, last);
            static std::vector<uint_fast32_t> s_prior;
            static std::vector<node *> s_queue;
            s_prior.resize(root->subtree_weight);
            s_queue.clear();
            s_queue.reserve(root->subtree_weight);
            for (uint_fast32_t &a : s_prior) a = s_rand();
            std::sort(s_prior.rbegin(), s_prior.rend());
            s_queue.reserve(root->subtree_weight);
            s_queue.push_back(root);
            for (int i = 0; i < s_queue.size(); i++) {
                node *p = s_queue[i];
                p->priority = s_prior[i];
                if (p->lchild) s_queue.push_back(p->lchild);
                if (p->rchild) s_queue.push_back(p->rchild);
            }
            return root;
        }
        void _clear(node *cur) {
            if (cur->lchild) _clear(cur->lchild);
            if (cur->rchild) _clear(cur->rchild);
            delete cur;
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        FHQTreapArray() : m_root(nullptr) {}
        template <typename _Iterator>
        FHQTreapArray(_Iterator __first, _Iterator __last) : m_root(make_tree(__first, __last)) {}
        void clear() {
            // if (m_root) _clear(m_root);
            m_root = nullptr;
        }
        void insert(int __pos, _Tp __key) {
            node *child[2];
            split_l_r(m_root, __pos, child);
            m_root = merge(merge(child[0], new node{{}, __key, false, s_rand(), 1, nullptr, nullptr}), child[1]);
        }
        void update(int __pos, _Tp __key) { at(__pos)->key = __key; }
        void erase(int __pos) {
            node *child[3] = {0};
            split_l_m_r(m_root, __pos, __pos, child);
            delete child[1];
            m_root = merge(child[0], child[2]);
        }
        void erase(int __left, int __right) {
            node *child[3] = {0};
            split_l_m_r(m_root, __left, __right, child);
            if (child[1]) _clear(child[1]);
            m_root = merge(child[0], child[2]);
        }
        template <typename _Iterator>
        void assign(_Iterator __first, _Iterator __last) {
            clear();
            m_root = make_tree(__first, __last);
        }
        void reverse(int __left, int __right) {
            node *child[3] = {0};
            split_l_m_r(m_root, __left, __right, child);
            child[1]->reverse();
            m_root = merge(merge(child[0], child[1]), child[2]);
        }
        void push_front(_Tp __key) { m_root = merge(new node{{}, __key, false, s_rand(), 1, nullptr, nullptr}, m_root); }
        void push_back(_Tp __key) { m_root = merge(m_root, new node{{}, __key, false, s_rand(), 1, nullptr, nullptr}); }
        void pop_front() {
            node *child[2];
            split_l_r(m_root, 1, child);
            delete child[0];
            m_root = child[1];
        }
        void pop_back() {
            node *child[2];
            split_l_r(m_root, m_root->subtree_weight - 1, child);
            delete child[1];
            m_root = child[0];
        }
        node *at(int __pos) const {
            node *cur = m_root;
            while (true) {
                if (cur->reversed) cur->push_down();
                if (subtree_weight(cur->lchild) > __pos)
                    cur = cur->lchild;
                else if (__pos -= subtree_weight(cur->lchild); !__pos--)
                    break;
                else
                    cur = cur->rchild;
            };
            return cur;
        }
        _Tp &operator[](int __index) const { return at(__index)->key; }
        node *front(int __pos) const { return at(0); }
        node *back() const { return at(size() - 1); }
        int size() const { return subtree_weight(m_root); }
        bool empty() const { return !size(); }
        FHQTreapArray<_Tp> subArray(int __left, int __right) {
            node *child[3] = {0};
            split_l_m_r(m_root, __left, __right, child);
            m_root = merge(child[0], child[2]);
            FHQTreapArray<_Tp> sub;
            sub.m_root = child[1];
            return sub;
        }
        void join(FHQTreapArray<_Tp> &__other) {
            m_root = merge(m_root, __other.m_root);
            __other.m_root = nullptr;
        }
        std::vector<_Tp> to_vector() const {
            std::vector<_Tp> v;
            auto dfs = [&](auto self, node *cur) -> void {
                if (cur->reversed) cur->push_down();
                if (cur->lchild) self(self, cur->lchild);
                v.push_back(cur->key);
                if (cur->rchild) self(self, cur->rchild);
            };
            if (m_root) dfs(dfs, m_root);
            return v;
        }
    };
    template <typename _Tp = int>
    FHQTreapArray() -> FHQTreapArray<_Tp>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type>
    FHQTreapArray(_Iterator, _Iterator) -> FHQTreapArray<_Tp>;
}

#endif