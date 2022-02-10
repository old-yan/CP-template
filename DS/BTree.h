#ifndef __OY_BTREE__
#define __OY_BTREE__

#include <functional>
#include "MemoryPool.h"

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
        struct block : MemoryPool<block> {
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
        _Compare m_comp;
        block *m_root;
        static void getLeftLast(block *parent, int pos) {
            block *left = parent->child[pos - 1];
            block *right = parent->child[pos];
            bool is_leaf = !right->child[0];
            right->push_front(parent->keys[pos - 1], is_leaf ? nullptr : left->child[left->keyNum]);
            parent->keys[pos - 1] = left->keys[left->keyNum - 1];
            left->pop_back();
            int w = is_leaf ? 1 : right->child[0]->weight + 1;
            left->max = is_leaf ? left->keys[left->keyNum - 1] : left->child[left->keyNum]->max;
            right->min = is_leaf ? right->keys[0] : right->child[0]->min;
            left->weight -= w;
            right->weight += w;
        }
        static void getRightFirst(block *parent, int pos) {
            block *left = parent->child[pos];
            block *right = parent->child[pos + 1];
            bool is_leaf = !right->child[0];
            left->push_back(parent->keys[pos], right->child[0]);
            parent->keys[pos] = right->keys[0];
            right->pop_front();
            int w = is_leaf ? 1 : left->child[left->keyNum]->weight + 1;
            left->max = is_leaf ? left->keys[left->keyNum - 1] : left->child[left->keyNum]->max;
            right->min = is_leaf ? right->keys[0] : right->child[0]->min;
            left->weight += w;
            right->weight -= w;
        }
        static void mergeAt(block *parent, int pos) {
            block *left = parent->child[pos];
            block *right = parent->child[pos + 1];
            left->keys[left->keyNum] = parent->keys[pos];
            std::copy(right->keys, right->keys + right->keyNum, left->keys + left->keyNum + 1);
            std::copy(right->child, right->child + right->keyNum + 1, left->child + left->keyNum + 1);
            left->keyNum = _K * 2 - 1;
            left->max = right->max;
            left->weight += right->weight + 1;
            delete right;
            std::copy(parent->keys + pos + 1, parent->keys + parent->keyNum, parent->keys + pos);
            std::copy(parent->child + pos + 2, parent->child + parent->keyNum + 1, parent->child + pos + 1);
            parent->keyNum--;
        }
        static void splitAt(block *parent, int pos) {
            block *left = parent->child[pos];
            block *right = new block;
            left->keyNum = right->keyNum = _K - 1;
            std::copy(left->keys + _K, left->keys + (_K * 2 - 1), right->keys);
            if (left->child[0]) {
                int w = _K - 1;
                for (int i = 0; i < _K; i++) {
                    right->child[i] = left->child[_K + i];
                    w += right->child[i]->weight;
                }
                right->min = right->child[0]->min;
                right->max = left->max;
                right->weight = w;
                left->max = left->child[_K - 1]->max;
                left->weight -= w + 1;
            } else {
                right->min = right->keys[0];
                right->max = left->max;
                right->weight = _K - 1;
                left->max = left->keys[_K - 2];
                left->weight -= _K;
            }
            parent->child[pos + 1] = right;
            parent->keys[pos] = left->keys[_K - 1];
            parent->keyNum++;
        }
        const node *key_lower_bound(const node *first, const node *last, _Tp __val) const {
            int len = last - first;
            while (len) {
                int half = len / 2;
                const node *mid = first + half;
                if (m_comp(mid->key, __val)) {
                    first = mid + 1;
                    len -= half + 1;
                } else
                    len = half;
            }
            return first;
        }
        const node *key_upper_bound(const node *first, const node *last, _Tp __val) const {
            int len = last - first;
            while (len) {
                int half = len / 2;
                const node *mid = first + half;
                if (m_comp(__val, mid->key))
                    len = half;
                else {
                    first = mid + 1;
                    len -= half + 1;
                }
            }
            return first;
        }
        block *_insert(block *cur, const node &item, bool &res) {
            int pos = key_upper_bound(cur->keys, cur->keys + cur->keyNum, item.key) - cur->keys;
            if constexpr (!_Tag::multi_key)
                if (pos && !m_comp(cur->keys[pos - 1].key, item.key)) return cur;
            if (!cur->child[0]) {
                std::copy_backward(cur->keys + pos, cur->keys + cur->keyNum, cur->keys + cur->keyNum + 1);
                cur->keys[pos] = item;
                cur->keyNum++;
                res = true;
            } else {
                if (cur->child[pos]->keyNum == _K * 2 - 1) {
                    std::copy_backward(cur->keys + pos, cur->keys + cur->keyNum, cur->keys + cur->keyNum + 1);
                    std::copy_backward(cur->child + pos + 1, cur->child + cur->keyNum + 1, cur->child + cur->keyNum + 2);
                    splitAt(cur, pos);
                    if (m_comp(cur->keys[pos].key, item.key)) pos++;
                }
                _insert(cur->child[pos], item, res);
            }
            if (res) {
                cur->weight++;
                if (m_comp(item.key, cur->min.key)) cur->min = item;
                if (m_comp(cur->max.key, item.key)) cur->max = item;
            }
            return cur;
        }
        block *_erase(block *cur, _Tp key, bool &res) {
            int pos = key_lower_bound(cur->keys, cur->keys + cur->keyNum, key) - cur->keys;
            if (pos == cur->keyNum || m_comp(key, cur->keys[pos].key)) {
                if (!cur->child[0]) return cur;
                if (cur->child[pos]->keyNum == _K - 1) {
                    if (pos && cur->child[pos - 1]->keyNum >= _K)
                        getLeftLast(cur, pos);
                    else if (pos < cur->keyNum && cur->child[pos + 1]->keyNum >= _K)
                        getRightFirst(cur, pos);
                    else {
                        if (pos) pos--;
                        mergeAt(cur, pos);
                        if (!cur->keyNum) {
                            m_root = _erase(cur->child[0], key, res);
                            delete cur;
                            return m_root;
                        }
                    }
                }
                cur->child[pos] = _erase(cur->child[pos], key, res);
                if (res) {
                    cur->weight--;
                    cur->min = cur->child[0]->min;
                    cur->max = cur->child[cur->keyNum]->max;
                }
                return cur;
            } else {
                res = true;
                if (!--cur->weight) {
                    delete cur;
                    return nullptr;
                }
                if (!cur->child[0]) {
                    std::copy(cur->keys + pos + 1, cur->keys + cur->keyNum, cur->keys + pos);
                    cur->keyNum--;
                    cur->updateMinMax();
                } else if (cur->child[pos]->keyNum >= _K) {
                    node leftMax = cur->child[pos]->max;
                    cur->keys[pos] = leftMax;
                    cur->child[pos] = _erase(cur->child[pos], leftMax.key, res);
                } else if (cur->child[pos + 1]->keyNum >= _K) {
                    node rightMin = cur->child[pos + 1]->min;
                    cur->keys[pos] = rightMin;
                    cur->child[pos + 1] = _erase(cur->child[pos + 1], rightMin.key, res);
                } else {
                    mergeAt(cur, pos);
                    if (!cur->keyNum) {
                        m_root = _erase(cur->child[0], key, res);
                        delete cur;
                        return m_root;
                    }
                    cur->child[pos] = _erase(cur->child[pos], key, res);
                }
                return cur;
            }
        }

    public:
        static void setBufferSize(int __count) { block::_reserve(__count); }
        BTree(_Compare __comp = _Compare()) : m_root(nullptr), m_comp(__comp) {}
        void clear() {
            // auto dfs = [&](auto self, block *p) -> void {
            //     delete p;
            //     if (p->child[0])
            //         for (int i = 0; i <= p->keyNum; i++) self(self, p->child[i]);
            // };
            // if (m_root) dfs(dfs, m_root);
            m_root = nullptr;
        }
        template <typename... Args>
        void insert(_Tp __key, Args... __args) {
            node item = node{__key, __args...};
            bool res = false;
            if (!m_root) {
                m_root = new block;
                m_root->min = m_root->max = m_root->keys[0] = item;
                m_root->keyNum = m_root->weight = 1;
            } else {
                if (m_root->keyNum == _K * 2 - 1) {
                    block *p = new block;
                    p->keyNum = 0;
                    p->child[0] = m_root;
                    p->weight = m_root->weight;
                    splitAt(p, 0);
                    m_root = _insert(p, item, res);
                } else
                    m_root = _insert(m_root, item, res);
            }
        }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key); p)
                p->value = __value;
            else
                insert(__key, __value);
        }
        bool erase(_Tp __key) {
            bool res = false;
            if (m_root) m_root = _erase(m_root, __key, res);
            return res;
        }
        void erase(_Tp __key, int __count) {
            while (__count-- && erase(__key))
                ;
        }
        int rank(_Tp __key) const {
            block *cur = m_root;
            int ord = 0;
            while (cur) {
                int pos = key_lower_bound(cur->keys, cur->keys + cur->keyNum, __key) - cur->keys;
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
            for (int i;; cur = cur->child[i]) {
                if (i = key_lower_bound(cur->keys, cur->keys + cur->keyNum, __key) - cur->keys; i < cur->keyNum && !m_comp(__key, cur->keys[i].key)) return cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *smaller_bound(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            for (int i;; cur = cur->child[i]) {
                if (i = key_lower_bound(cur->keys, cur->keys + cur->keyNum, __key) - cur->keys) res = cur->keys + i - 1;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *lower_bound(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            for (int i; cur; cur = i ? cur->child[i - 1] : nullptr) {
                if (i = key_lower_bound(cur->keys, cur->keys + cur->keyNum, __key) - cur->keys; i < cur->keyNum) res = cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *upper_bound(_Tp __key) const {
            block *cur = m_root;
            if (!cur) return nullptr;
            node *res = nullptr;
            for (int i; cur; cur = cur->child[i]) {
                if (i = key_upper_bound(cur->keys, cur->keys + cur->keyNum, __key) - cur->keys; i < cur->keyNum) res = cur->keys + i;
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