#ifndef __OY_SKIPLIST__
#define __OY_SKIPLIST__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct SkipListSetTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = false;
    };
    struct SkipListMultisetTag {
        static constexpr bool multi_key = true;
        static constexpr bool is_map = false;
    };
    struct SkipListMapTag {
        static constexpr bool multi_key = false;
        static constexpr bool is_map = true;
    };
    template <typename _Tp, typename _Fp, typename _Tag>
    struct _SkipListNode;
    template <typename _Tp, typename _Fp>
    struct _SkipListNode<_Tp, _Fp, SkipListSetTag> { _Tp key; };
    template <typename _Tp, typename _Fp>
    struct _SkipListNode<_Tp, _Fp, SkipListMultisetTag> {
        _Tp key;
        int _node_weight;
    };
    template <typename _Tp, typename _Fp>
    struct _SkipListNode<_Tp, _Fp, SkipListMapTag> {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp = int, typename _Compare = std::less<_Tp>, typename _Tag = SkipListMultisetTag>
    class SkipList {
        struct node : _SkipListNode<_Tp, _Fp, _Tag> {
            constexpr int node_weight() {
                if constexpr (_Tag::multi_key)
                    return this->_node_weight;
                else
                    return 1;
            }
            struct node_next {
                node *pointer;
                int distance;
            };
            std::vector<node_next> next;
            static void *operator new(size_t count) { return MemoryPool<node>::operator new(count); }
            static void operator delete(void *p) { MemoryPool<node>::operator delete(p); }
        };
        node *m_head;
        _Compare m_comp;
        int m_height;
        int m_size;
        static bool testJump() { return rand() % 100 < 53; }
        template <typename... Args>
        int _insert(node *cur, int h, node *&res, _Tp __key, Args... __args) {
            int distance = 0;
            while (true) {
                auto &&[nxt, dis] = cur->next[h];
                if (!nxt || m_comp(__key, nxt->key))
                    break;
                else if (m_comp(nxt->key, __key)) {
                    distance += dis + nxt->node_weight();
                    cur = nxt;
                } else {
                    if constexpr (_Tag::multi_key) {
                        res = nxt;
                        res->_node_weight++;
                        return distance + dis;
                    } else
                        return -1;
                }
            }
            if (h) {
                int next_level_distance = _insert(cur, h - 1, res, __key, __args...);
                if (!res) return -1;
                auto &&[nxt, dis] = cur->next[h];
                dis++;
                if (next_level_distance < 0) return -1;
                if (!testJump()) {
                    res->next.shrink_to_fit();
                    return -1;
                }
                res->next.push_back({nxt, dis - next_level_distance - res->node_weight()});
                cur->next[h] = {res, next_level_distance};
                return distance + next_level_distance;
            } else {
                node *nxt = cur->next[h].pointer;
                res = new node{__key, __args..., {{nxt, 0}}};
                cur->next[h] = {res, 0};
                return distance;
            }
        }
        node *_erase(node *cur, int h, _Tp key) {
            while (node *nxt = cur->next[h].pointer) {
                if (!m_comp(nxt->key, key)) break;
                cur = nxt;
            }
            if (h) {
                if (node *res = _erase(cur, h - 1, key)) {
                    if (cur->next[h].pointer != res)
                        cur->next[h].distance--;
                    else if (!_Tag::multi_key || !res->node_weight())
                        cur->next[h] = {res->next[h].pointer, cur->next[h].distance + res->next[h].distance};
                    return res;
                } else
                    return nullptr;
            } else if (node *nxt = cur->next[h].pointer; !nxt || m_comp(key, nxt->key))
                return nullptr;
            else {
                if constexpr (_Tag::multi_key) nxt->_node_weight--;
                if (!_Tag::multi_key || !nxt->node_weight()) cur->next[h].pointer = nxt->next[h].pointer;
                return nxt;
            }
        }

    public:
        static void setBufferSize(int __count) { MemoryPool<node>::_reserve(__count); }
        SkipList() : m_height(1), m_size(0) {
            m_head = new node;
            m_head->next.push_back({nullptr, 0});
        }
        void clear() {
            node *nxt = m_head->next[0].pointer;
            while (nxt) {
                node *cur = nxt;
                nxt = nxt->next[0].pointer;
                delete cur;
            }
            if (nxt) delete nxt;
            m_head = nullptr;
        }
        template <typename... Args>
        void insert(_Tp __key, Args... __args) {
            node *res = nullptr;
            int distance;
            if constexpr (_Tag::multi_key)
                distance = _insert(m_head, m_height - 1, res, __key, __args..., 1);
            else
                distance = _insert(m_head, m_height - 1, res, __key, __args...);
            m_size += bool(res);
            if (distance < 0 || !testJump()) return;
            m_height++;
            m_head->next.push_back({res, distance});
            res->next.push_back({nullptr, m_size - distance - res->node_weight()});
        }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key))
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) {
            if (node *res = _erase(m_head, m_height - 1, __key)) {
                if (!_Tag::multi_key || !res->node_weight()) delete res;
                m_size--;
                return true;
            } else
                return false;
        }
        void erase(_Tp __key, int __count) {
            static_assert(_Tag::multi_key);
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) const {
            node *cur = m_head;
            int ord = 0;
            for (int h = m_height - 1; ~h; h--)
                while (cur->next[h].pointer && m_comp(cur->next[h].pointer->key, __key)) {
                    ord += cur->next[h].distance + cur->next[h].pointer->node_weight();
                    cur = cur->next[h].pointer;
                }
            return ord;
        }
        const node *kth(int __k) const {
            if (__k < 0 || __k >= m_size) return nullptr;
            node *cur = m_head;
            for (int h = m_height - 1; ~h; h--)
                while (cur->next[h].distance <= __k) {
                    __k -= cur->next[h].distance;
                    if (__k -= cur->next[h].pointer->node_weight(); __k < 0)
                        return cur->next[h].pointer;
                    else
                        cur = cur->next[h].pointer;
                }
            return cur;
        }
        const node *find(_Tp __key) const {
            node *cur = m_head;
            for (int h = m_height - 1; ~h; h--)
                while (cur->next[h].pointer) {
                    if (m_comp(cur->next[h].pointer->key, __key))
                        cur = cur->next[h].pointer;
                    else if (!m_comp(__key, cur->next[h].pointer->key))
                        return cur->next[h].pointer;
                    else
                        break;
                }
            return nullptr;
        }
        const node *smaller_bound(_Tp __key) const {
            node *cur = m_head;
            for (int h = m_height - 1; ~h; h--)
                while (cur->next[h].pointer && m_comp(cur->next[h].pointer->key, __key)) cur = cur->next[h].pointer;
            return cur;
        }
        const node *lower_bound(_Tp __key) const {
            node *cur = m_head;
            for (int h = m_height - 1; ~h; h--)
                while (cur->next[h].pointer) {
                    if (m_comp(cur->next[h].pointer->key, __key))
                        cur = cur->next[h].pointer;
                    else if (!m_comp(__key, cur->next[h].pointer->key))
                        return cur->next[h].pointer;
                    else
                        break;
                }
            return nullptr;
        }
        const node *upper_bound(_Tp __key) const {
            node *cur = m_head;
            for (int h = m_height - 1; ~h; h--)
                while (cur->next[h].pointer && !m_comp(__key, cur->next[h].pointer->key)) cur = cur->next[h].pointer;
            return cur->next[0].pointer;
        }
        int size() const { return m_size; }
        bool empty() const { return !size(); }
        int count(_Tp __key) const {
            if (auto it = find(__key))
                return it->node_weight();
            else
                return 0;
        }
    };
    namespace SkipListContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = SkipList<_Tp, bool, _Compare, SkipListSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = SkipList<_Tp, bool, _Compare, SkipListMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = SkipList<_Tp, _Fp, _Compare, SkipListMapTag>;
    }
}

#endif