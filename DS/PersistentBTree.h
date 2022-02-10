#ifndef __PERSISTENTBTREE__
#define __PERSISTENTBTREE__

#include <functional>
#include "MemoryPool.h"

namespace OY {
    struct PersistentBTreeSetTag {
        static constexpr bool is_map = false;
        static constexpr bool multi_key = false;
    };
    struct PersistentBTreeMultisetTag {
        static constexpr bool is_map = false;
        static constexpr bool multi_key = true;
    };
    struct PersistentBTreeMapTag {
        static constexpr bool is_map = true;
        static constexpr bool multi_key = false;
    };
    template <typename _Compare>
    struct PersistentBTreeLess {
        _Compare __comp;
        PersistentBTreeLess(_Compare __comp = _Compare()) : __comp(__comp) {}
        template <typename _Tp>
        bool operator()(const _Tp &x, const _Tp &y) const { return __comp(x.key, y.key); }
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PersistentBTreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PersistentBTreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = int, typename _Compare = std::less<_Tp>, typename _Tag = PersistentBTreeMultisetTag, int _K = 3>
    class PersistentBTree {
        using node = _PersistentBTreeNode<_Tp, _Fp, _Tag::is_map>;
        struct block : MemoryPool<block> {
            node keys[_K * 2 - 1];
            int keyNum;
            node min;
            node max;
            int weight;
            block *child[_K * 2];
            block() { child[0] = nullptr; }
            block(const node &item) {
                keys[0] = min = max = item;
                keyNum = weight = 1;
                child[0] = nullptr;
            }
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
                if (pos < keyNum)
                    std::copy_backward(keys + pos, keys + keyNum, keys + keyNum + 1);
                keys[pos] = key;
                keyNum++;
            }
            void eraseKey(int pos) {
                if (pos + 1 < keyNum)
                    std::copy(keys + pos + 1, keys + keyNum, keys + pos);
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
        PersistentBTreeLess<_Compare> m_comp;
        std::vector<block *> m_roots;
        static node makeNode(_Tp __key) {
            if constexpr (_Tag::is_map)
                return {__key, _Fp()};
            else
                return {__key};
        }
        static void getLeftLast(block *parent, int pos) {
            block *left = new block{*parent->child[pos - 1]};
            block *old_right = parent->child[pos];
            block *right = new block;
            bool is_leaf = !old_right->child[0];
            right->keys[0] = parent->keys[pos - 1]; //  p1->keys[p1->keyNum];
            right->child[0] = left->child[left->keyNum];
            parent->keys[pos - 1] = left->keys[left->keyNum - 1];
            left->pop_back();
            std::copy(old_right->keys, old_right->keys + old_right->keyNum, right->keys + 1);
            std::copy(old_right->child, old_right->child + old_right->keyNum + 1, right->child + 1);
            int w = is_leaf ? 1 : right->child[0]->weight + 1;
            left->max = is_leaf ? left->keys[left->keyNum - 1] : left->child[left->keyNum]->max;
            left->weight -= w;
            right->min = is_leaf ? right->keys[0] : right->child[0]->min;
            right->max = old_right->max;
            right->weight = old_right->weight + w;
            right->keyNum = old_right->keyNum + 1;
            parent->child[pos - 1] = left;
            parent->child[pos] = right;
        }
        static void getRightFirst(block *parent, int pos) {
            block *left = new block{*parent->child[pos]};
            block *old_right = parent->child[pos + 1];
            block *right = new block;
            bool is_leaf = !old_right->child[0];
            int w = is_leaf ? 1 : old_right->child[0]->weight + 1;
            left->push_back(parent->keys[pos], old_right->child[0]);
            parent->keys[pos] = old_right->keys[0];
            std::copy(old_right->keys + 1, old_right->keys + old_right->keyNum, right->keys);
            std::copy(old_right->child + 1, old_right->child + old_right->keyNum + 1, right->child);
            left->max = is_leaf ? left->keys[left->keyNum - 1] : left->child[left->keyNum]->max;
            left->weight += w;
            right->min = is_leaf ? right->keys[0] : right->child[0]->min;
            right->max = old_right->max;
            right->weight = old_right->weight - w;
            right->keyNum = old_right->keyNum - 1;
            parent->child[pos] = left;
            parent->child[pos + 1] = right;
        }
        static void mergeAt(block *parent, int pos) {
            block *left = parent->child[pos];
            block *right = parent->child[pos + 1];
            block *now = new block;
            std::copy(left->keys, left->keys + _K - 1, now->keys);
            now->keys[_K - 1] = parent->keys[pos];
            std::copy(right->keys, right->keys + _K - 1, now->keys + _K);
            std::copy(left->child, left->child + _K, now->child);
            std::copy(right->child, right->child + _K, now->child + _K);
            now->min = left->min;
            now->max = right->max;
            now->keyNum = _K * 2 - 1;
            now->weight = left->weight + right->weight + 1;
            parent->child[pos] = now;
            parent->erase(pos);
        }
        static void splitAt(block *old, block *parent, int pos) {
            block *left = new block;
            block *right = new block;
            left->keyNum = right->keyNum = _K - 1;
            std::copy(old->keys, old->keys + _K - 1, left->keys);
            std::copy(old->keys + _K, old->keys + (_K * 2 - 1), right->keys);
            if (old->child[0]) {
                int w = _K - 1;
                std::copy(old->child, old->child + _K, left->child);
                for (int i = 0; i < _K; i++) {
                    right->child[i] = old->child[_K + i];
                    w += right->child[i]->weight;
                }
                right->min = right->child[0]->min;
                right->max = old->max;
                right->weight = w;
                left->min = old->min;
                left->max = old->child[_K - 1]->max;
                left->weight = old->weight - (w + 1);
            } else {
                right->min = right->keys[0];
                right->max = old->max;
                right->weight = _K - 1;
                left->min = old->min;
                left->max = left->keys[_K - 2];
                left->weight = _K - 1;
            }
            parent->keys[pos] = old->keys[_K - 1];
            parent->child[pos] = left;
            parent->child[pos + 1] = right;
            parent->keyNum++;
        }
        block *_insert(block *cur, const node &item, bool &res) {
            int pos = std::upper_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys;
            if constexpr (!_Tag::multi_key)
                if (pos && !m_comp(cur->keys[pos - 1], item)) return cur;
            if (!cur->child[0]) {
                res = true;
                cur->insertKey(pos, item);
            } else {
                if (cur->child[pos]->keyNum == _K * 2 - 1) {
                    if (pos < cur->keyNum) {
                        std::copy_backward(cur->keys + pos, cur->keys + cur->keyNum, cur->keys + cur->keyNum + 1);
                        std::copy_backward(cur->child + pos + 1, cur->child + cur->keyNum + 1, cur->child + cur->keyNum + 2);
                    }
                    splitAt(cur->child[pos], cur, pos);
                    if (m_comp(cur->keys[pos], item)) pos++;
                    cur->child[pos] = _insert(cur->child[pos], item, res);
                } else
                    cur->child[pos] = _insert(new block{*cur->child[pos]}, item, res);
            }
            if (res) {
                cur->weight++;
                cur->updateMinMax();
            }
            return cur;
        }
        block *_update(block *cur, const node &item, bool &res) {
            int pos = std::upper_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys;
            if constexpr (!_Tag::multi_key)
                if (pos && !m_comp(cur->keys[pos - 1], item)) {
                    node *p = new block(*cur);
                    p->keys[pos - 1] = item;
                    return p;
                }
            if (!cur->child[0]) {
                res = true;
                cur->insertKey(pos, item);
            } else {
                if (cur->child[pos]->keyNum == _K * 2 - 1) {
                    if (pos < cur->keyNum) {
                        std::copy_backward(cur->keys + pos, cur->keys + cur->keyNum, cur->keys + cur->keyNum + 1);
                        std::copy_backward(cur->child + pos + 1, cur->child + cur->keyNum + 1, cur->child + cur->keyNum + 2);
                    }
                    splitAt(cur->child[pos], cur, pos);
                    if (m_comp(cur->keys[pos], item)) pos++;
                    cur->child[pos] = _insert(cur->child[pos], item, res);
                } else
                    cur->child[pos] = _insert(new block{*cur->child[pos]}, item, res);
            }
            if (res) cur->weight++;
            cur->updateMinMax();
            return cur;
        }
        block *_erase(block *cur, const node &item, bool &res) {
            if (int pos = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; pos == cur->keyNum || m_comp(item, cur->keys[pos])) {
                if (!cur->child[0]) return cur;
                if (cur->child[pos]->keyNum == _K - 1) {
                    if (pos && cur->child[pos - 1]->keyNum >= _K)
                        getLeftLast(cur, pos);
                    else if (pos < cur->keyNum && cur->child[pos + 1]->keyNum >= _K)
                        getRightFirst(cur, pos);
                    else {
                        if (pos) pos--;
                        mergeAt(cur, pos);
                        if (!cur->keyNum) return _erase(cur->child[0], item, res);
                    }
                } else
                    cur->child[pos] = new block{*cur->child[pos]};
                cur->child[pos] = _erase(cur->child[pos], item, res);
                if (res) {
                    cur->weight--;
                    cur->updateMinMax();
                }
                return cur;
            } else {
                res = true;
                if (!--cur->weight) {
                    delete cur;
                    return nullptr;
                }
                if (!cur->child[0]) {
                    cur->eraseKey(pos);
                    cur->updateMinMax();
                } else if (cur->child[pos]->keyNum >= _K) {
                    node leftMax = cur->child[pos]->max;
                    cur->keys[pos] = leftMax;
                    cur->child[pos] = _erase(new block{*cur->child[pos]}, leftMax, res);
                } else if (pos < cur->keyNum && cur->child[pos + 1]->keyNum >= _K) {
                    node rightMin = cur->child[pos + 1]->min;
                    cur->keys[pos] = rightMin;
                    cur->child[pos + 1] = _erase(new block{*cur->child[pos + 1]}, rightMin, res);
                } else {
                    mergeAt(cur, pos);
                    if (!cur->keyNum) return _erase(cur->child[0], item, res);
                    cur->child[pos] = _erase(new block{*cur->child[pos]}, item, res);
                }
                return cur;
            }
        }
        block *&_root(int version) { return ~version ? m_roots[version] : m_roots.back(); }
        block *_root(int version) const { return ~version ? m_roots[version] : m_roots.back(); }

    public:
        static void setBufferSize(int __count) { block::_reserve(__count); }
        PersistentBTree(_Compare __comp = _Compare()) : m_comp(__comp) { m_roots.push_back(nullptr); }
        ~PersistentBTree() { clear(); }
        void clear() { m_roots.resize(1); }
        template <typename... Args>
        void insert(int __prevVersion, Args... __args) {
            block *root = _root(__prevVersion);
            const node &item = node{__args...};
            bool res = false;
            if (!root)
                root = new block(item);
            else {
                if (root->keyNum == _K * 2 - 1) {
                    block *p = new block;
                    p->keyNum = 0;
                    p->child[0] = root;
                    p->weight = root->weight;
                    splitAt(root, p, 0);
                    root = _insert(p, item, res);
                } else
                    root = _insert(new block{*root}, item, res);
            }
            m_roots.push_back(root);
        }
        void update(int __prevVersion, _Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            block *root = _root(__prevVersion);
            const node &item = node{__key, __value};
            bool res = false;
            if (!root)
                root = new block(item);
            else {
                if (root->keyNum == _K * 2 - 1) {
                    block *p = new block;
                    p->keyNum = 0;
                    p->child[0] = root;
                    p->weight = root->weight;
                    splitAt(root, p, 0);
                    root = _update(p, item, res);
                } else
                    root = _update(new block{*root}, item, res);
            }
            m_roots.push_back(root);
        }
        bool erase(int __prevVersion, _Tp __key) {
            bool res = false;
            m_roots.push_back(_root(__prevVersion) ? _erase(new block{*_root(__prevVersion)}, makeNode(__key), res) : nullptr);
            return res;
        }
        void copyVersion(int __prevVersion) { m_roots.push_back(_root(__prevVersion)); }
        int rank(int __version, _Tp __key) const {
            block *cur = _root(__version);
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
        const node *kth(int __version, int k) const {
            block *cur = _root(__version);
            for (int i; cur->child[0]; cur = cur->child[i])
                for (i = 0; k >= cur->child[i]->weight; i++) {
                    k -= cur->child[i]->weight;
                    if (!k--) return cur->keys + i;
                }
            return cur->keys + k;
        }
        const node *find(int __version, _Tp __key) const {
            block *cur = _root(__version);
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i;; cur = cur->child[i]) {
                if (i = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; i < cur->keyNum && !m_comp(item, cur->keys[i])) return cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *smaller_bound(int __version, _Tp __key) const {
            block *cur = _root(__version);
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i;; cur = cur->child[i]) {
                if (i = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys) res = cur->keys + i - 1;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *lower_bound(int __version, _Tp __key) const {
            block *cur = _root(__version);
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i; cur; cur = i ? cur->child[i - 1] : nullptr) {
                if (i = std::lower_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; i < cur->keyNum) res = cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        const node *upper_bound(int __version, _Tp __key) const {
            block *cur = _root(__version);
            if (!cur) return nullptr;
            node *res = nullptr;
            node item = makeNode(__key);
            for (int i; cur; cur = cur->child[i]) {
                if (i = std::upper_bound(cur->keys, cur->keys + cur->keyNum, item, m_comp) - cur->keys; i < cur->keyNum) res = cur->keys + i;
                if (!cur->child[0]) break;
            }
            return res;
        }
        int size(int __version) const { return _root(__version) ? _root(__version)->weight : 0; }
        bool empty(int __version) const { return !size(__version); }
        int count(int __version, _Tp __key) const {
            auto it1 = lower_bound(__version, __key);
            if (!it1) return 0;
            auto it2 = upper_bound(__version, __key);
            if constexpr (!_Tag::multi_key)
                return it2 != it1;
            else {
                if (!it2)
                    return size(__version) - rank(__version, __key);
                else
                    return rank(__version, it2->key) - rank(__version, __key);
            }
        }
        int versionCount() const { return m_roots.size(); }
    };
    namespace PersistentBTreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>, int _K = 6>
        using Set = PersistentBTree<_Tp, bool, _Compare, PersistentBTreeSetTag, _K>;
        template <typename _Tp, typename _Compare = std::less<_Tp>, int _K = 6>
        using Multiset = PersistentBTree<_Tp, bool, _Compare, PersistentBTreeMultisetTag, _K>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>, int _K = 6>
        using Map = PersistentBTree<_Tp, _Fp, _Compare, PersistentBTreeMapTag, _K>;
    }
}

#endif