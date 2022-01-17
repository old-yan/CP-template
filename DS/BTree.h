#ifndef __OY_BTREE__
#define __OY_BTREE__

#include "MemoryPool.h"
#include <functional>

namespace OY {
    struct BTreeSetTag {
        static constexpr bool is_map = false;
        static constexpr bool multi_key = false;
    };
    struct BTreeMultisetTag {
        static constexpr bool is_map = false;
        static constexpr bool multi_key = true;
    };
    struct BTreeMapTag {
        static constexpr bool is_map = true;
        static constexpr bool multi_key = false;
    };
    template <typename _Compare>
    struct BTreeLess {
        _Compare __comp;
        BTreeLess(_Compare __comp = _Compare()) : __comp(__comp) {}
        template <typename _Tp>
        bool operator()(const _Tp &x, const _Tp &y) const { return __comp(x.key, y.key); }
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _BTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _BTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = int, typename _Compare = std::less<_Tp>, typename _Tag = BTreeMultisetTag, int _K = 3>
    class BTree {
        using node = _BTreeNode<_Tp, _Fp, _Tag::is_map>;
        struct block : MemoryPool<block, (1 << 15) / _K> {
            node keys[_K * 2 - 1];
            int keyNum;
            node min;
            node max;
            int weight;
            block *child[_K * 2];
            block() { child[0] = nullptr; }
            void push_front(node key, block *p) {
                std::copy_backward(keys, keys + keyNum, keys + keyNum + 1);
                if (child[0]) std::copy_backward(child, child + keyNum + 1, child + keyNum + 2);
                keys[0] = key;
                child[0] = p;
                keyNum++;
            }
            void push_back(node key, block *p) {
                keys[keyNum] = key;
                child[keyNum + 1] = p;
                keyNum++;
            }
            void pop_front() {
                std::copy(keys + 1, keys + keyNum, keys);
                bool is_leaf = !child[0];
                std::copy(child + 1, child + keyNum + 1, child);
                if (is_leaf) child[0] = nullptr;
                keyNum--;
            }
            void pop_back() { keyNum--; }
            void erase(int pos) {
                if (pos + 1 < keyNum) {
                    std::copy(keys + pos + 1, keys + keyNum, keys + pos);
                    std::copy(child + pos + 2, child + keyNum + 1, child + pos + 1);
                }
                keyNum--;
            }
            void insertKey(int pos, node key) {
                if (pos < keyNum) std::copy_backward(keys + pos, keys + keyNum, keys + keyNum + 1);
                keys[pos] = key;
                keyNum++;
            }
            void eraseKey(int pos) {
                if (pos + 1 < keyNum) std::copy(keys + pos + 1, keys + keyNum, keys + pos);
                keyNum--;
            }
            void updateMinMax() {
                if (!child[0]) {
                    min = keys[0];
                    max = keys[keyNum - 1];
                } else {
                    min = child[0]->min;
                    max = child[keyNum]->max;
                }
            }
        };
        BTreeLess<_Compare> m_comp;
        block *m_root;
        static inline int s_reserveSize = 100000;
        static node makeNode(_Tp __key) {
            if constexpr (_Tag::is_map)
                return {__key, _Fp()};
            else
                return {__key};
        }
        static void getLeftLast(block *cur, int pos) {
            block *p1 = cur->child[pos - 1];
            block *p2 = cur->child[pos];
            bool is_leaf = !p2->child[0];
            p2->push_front(cur->keys[pos - 1], is_leaf ? nullptr : p1->child[p1->keyNum]);
            cur->keys[pos - 1] = p1->keys[p1->keyNum - 1];
            p1->pop_back();

            int w = is_leaf ? 1 : p2->child[0]->weight + 1;
            p1->max = is_leaf ? p1->keys[p1->keyNum - 1] : p1->child[p1->keyNum]->max;
            p2->min = is_leaf ? p2->keys[0] : p2->child[0]->min;
            p1->weight -= w;
            p2->weight += w;
        }
        static void getRightFirst(block *cur, int pos) {
            block *p1 = cur->child[pos];
            block *p2 = cur->child[pos + 1];
            p1->push_back(cur->keys[pos], p2->child[0]);
            cur->keys[pos] = p2->keys[0];
            p2->pop_front();
            bool is_leaf = !p2->child[0];
            int w = is_leaf ? 1 : p1->child[p1->keyNum]->weight + 1;
            p1->max = is_leaf ? p1->keys[p1->keyNum - 1] : p1->child[p1->keyNum]->max;
            p2->min = is_leaf ? p2->keys[0] : p2->child[0]->min;
            p1->weight += w;
            p2->weight -= w;
        }
        static void mergeAt(block *cur, int pos) {
            block *p1 = cur->child[pos];
            block *p2 = cur->child[pos + 1];
            p1->keys[p1->keyNum] = cur->keys[pos];
            std::copy(p2->keys, p2->keys + p2->keyNum, p1->keys + p1->keyNum + 1);
            std::copy(p2->child, p2->child + p2->keyNum + 1, p1->child + p1->keyNum + 1);
            p1->keyNum += p2->keyNum + 1;
            p1->max = p2->max;
            p1->weight += p2->weight + 1;
            cur->erase(pos);
        }
        static void splitAt(block *cur, block *parent, int pos) {
            cur->keyNum = _K - 1;
            block *nxt = new block;
            nxt->keyNum = _K - 1;
            std::copy(cur->keys + _K, cur->keys + (_K * 2 - 1), nxt->keys);
            if (cur->child[0]) {
                int w = _K - 1;
                for (int i = 0; i < _K; i++) {
                    nxt->child[i] = cur->child[_K + i];
                    w += nxt->child[i]->weight;
                }
                nxt->min = nxt->child[0]->min;
                nxt->max = cur->max;
                nxt->weight = w;
                cur->max = cur->child[_K - 1]->max;
                cur->weight -= w + 1;
            } else {
                nxt->min = nxt->keys[0];
                nxt->max = cur->max;
                nxt->weight = _K - 1;
                cur->max = cur->keys[_K - 2];
                cur->weight -= _K;
            }
            parent->child[pos + 1] = nxt;
            parent->keys[pos] = cur->keys[_K - 1];
            parent->keyNum++;
        }
        bool _insert(block *cur, block *parent, const node &item) {
            bool new_root = false;
            if (cur->keyNum == _K * 2 - 1) {
                if (!parent) {
                    new_root = true;
                    parent = new block;
                    parent->keyNum = 0;
                    parent->child[0] = cur;
                    parent->min = m_comp(item, cur->min) ? item : cur->min;
                    parent->max = m_comp(cur->max, item) ? item : cur->max;
                    parent->weight = cur->weight;
                    m_root = parent;
                }
                int i = parent->keyNum;
                while (parent->child[i] != cur) {
                    parent->child[i + 1] = parent->child[i];
                    parent->keys[i] = parent->keys[i - 1];
                    i--;
                }
                splitAt(cur, parent, i);
                if (m_comp(parent->keys[i], item)) cur = parent->child[i + 1];
            }
            if (m_comp(item, cur->min)) cur->min = item;
            if (m_comp(cur->max, item)) cur->max = item;
            int pos = std::upper_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys;
            if constexpr (!_Tag::multi_key)
                if (pos && !m_comp(cur->keys[pos - 1], item)) return false;
            if (!cur->child[0]) {
                cur->insertKey(pos, item);
                if (new_root) m_root->weight++;
                cur->weight++;
                return true;
            } else if (_insert(cur->child[pos], cur, item)) {
                if (new_root) m_root->weight++;
                cur->weight++;
                return true;
            } else
                return false;
        }
        void _insert(const node &item) {
            if (!m_root) {
                m_root = new block;
                m_root->keys[0] = item;
                m_root->keyNum = 1;
                m_root->min = item;
                m_root->max = item;
                m_root->weight = 1;
            } else
                _insert(m_root, nullptr, item);
        }
        bool _erase(block *cur, const node &item) {
            int pos = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys;
            if (pos == cur->keyNum || m_comp(item, cur->keys[pos])) {
                if (!cur->child[0])
                    return false;
                else if (cur->child[pos]->keyNum == _K - 1) {
                    if (pos && cur->child[pos - 1]->keyNum >= _K)
                        getLeftLast(cur, pos);
                    else if (pos < cur->keyNum && cur->child[pos + 1]->keyNum >= _K)
                        getRightFirst(cur, pos);
                    else {
                        if (pos) pos--;
                        mergeAt(cur, pos);
                    }
                }
                if (!cur->keyNum) {
                    delete cur;
                    return _erase(m_root = cur->child[0], item);
                }
                bool res = _erase(cur->child[pos], item);
                if (res) {
                    cur->weight--;
                    cur->updateMinMax();
                }
                return res;
            } else {
                cur->weight--;
                if (!cur->child[0]) {
                    cur->eraseKey(pos);
                    if (!cur->weight) {
                        delete cur;
                        m_root = nullptr;
                    }
                    cur->updateMinMax();
                } else if (cur->child[pos]->keyNum >= _K) {
                    node leftMax = cur->child[pos]->max;
                    cur->keys[pos] = leftMax;
                    _erase(cur->child[pos], leftMax);
                } else if (pos < cur->keyNum && cur->child[pos + 1]->keyNum >= _K) {
                    node rightMin = cur->child[pos + 1]->min;
                    cur->keys[pos] = rightMin;
                    _erase(cur->child[pos + 1], rightMin);
                } else {
                    mergeAt(cur, pos);
                    _erase(cur->child[pos], item);
                }
                return true;
            }
        }

    public:
        static void setBufferSize(int __count) { block::_reserve(__count); }
        BTree(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
        ~BTree() { clear(); }
        void clear() {
            auto dfs = [&](auto self, block *p) -> void {
                delete p;
                if (p->child[0])
                    for (int i = 0; i <= p->keyNum; i++) self(self, p->child[i]);
            };
            if (m_root) dfs(dfs, m_root);
            m_root = nullptr;
        }
        template <typename... Args>
        void insert(Args... __args) { _insert(node{__args...}); }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key); p)
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) { return m_root && _erase(m_root, makeNode(__key)); }
        void erase(_Tp __key, int __count) {
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) const {
            block *cur = m_root;
            node item = makeNode(__key);
            int ord = 0;
            while (cur) {
                int pos = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys;
                ord += pos;
                if (!cur->child[0]) break;
                for (int i = 0; i < pos; i++) ord += cur->child[i]->weight;
                cur = cur->child[pos];
            }
            return ord;
        }
        const node *kth(int k) const {
            block *cur = m_root;
            for (int i; cur->child[0]; cur = cur->child[i])
                for (i = 0; k >= cur->child[i]->weight; i++) {
                    k -= cur->child[i]->weight;
                    if (!k--) return cur->keys + i;
                }
            return cur->keys + k;
        }
        const node *find(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i;; cur = cur->child[i]) {
                if (i = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; i < cur->keyNum && !m_comp(item, cur->keys[i])) return cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *smaller_bound(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i;; cur = cur->child[i]) {
                if (i = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys) res = cur->keys + i - 1;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *lower_bound(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i; cur; cur = i ? cur->child[i - 1] : nullptr) {
                if (i = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; i < cur->keyNum) res = cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *upper_bound(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i; cur; cur = cur->child[i]) {
                if (i = std::upper_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; i < cur->keyNum) res = cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        int size() const { return m_root ? m_root->weight : 0; }
        bool empty() const { return !size(); }
        int count(_Tp __key) const {
            auto it1 = lower_bound(__key);
            if (!it1) return 0;
            auto it2 = upper_bound(__key);
            if constexpr (!_Tag::multi_key)
                return it2 != it1;
            else {
                if (!it2)
                    return size() - rank(__key);
                else
                    return rank(it2->key) - rank(__key);
            }
        }
    };
    namespace BTreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>, int _K = 28>
        using Set = BTree<_Tp, bool, _Compare, BTreeSetTag, _K>;
        template <typename _Tp, typename _Compare = std::less<_Tp>, int _K = 28>
        using Multiset = BTree<_Tp, bool, _Compare, BTreeMultisetTag, _K>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>, int _K = 28>
        using Map = BTree<_Tp, _Fp, _Compare, BTreeMapTag, _K>;
    }
}

#endif