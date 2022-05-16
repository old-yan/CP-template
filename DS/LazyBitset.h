#ifndef __OY_LAZYBITSET__
#define __OY_LAZYBITSET__

#include "MemoryPool.h"

namespace OY {
    struct LazyBitset {
        struct _BitsetNode : MemoryPool<_BitsetNode> {
            int sum;
            bool flipped;
            _BitsetNode *lchild;
            _BitsetNode *rchild;
            _BitsetNode(int _sum, bool _flipped, _BitsetNode *_lchild, _BitsetNode *_rchild) : sum(_sum), flipped(_flipped), lchild(_lchild), rchild(_rchild) {}
            void flip(int size) {
                sum = size - sum;
                flipped = flipped ? false : true;
            }
        };
        int m_length;
        _BitsetNode *m_root;
        void _pushDown_one(_BitsetNode *cur) const {
            if (int half = (cur->sum + 1) / 2; !cur->lchild)
                cur->lchild = new _BitsetNode(half, false, nullptr, nullptr);
            else
                cur->lchild->sum = half;
            if (int half = cur->sum / 2; !cur->rchild)
                cur->rchild = new _BitsetNode(half, false, nullptr, nullptr);
            else
                cur->rchild->sum = half;
        }
        void _pushDown_zero(_BitsetNode *cur) const {
            if (cur->lchild) cur->lchild->sum = 0;
            if (cur->rchild) cur->rchild->sum = 0;
        }
        void _pushDown_flip(_BitsetNode *cur, int left, int right) const {
            if (int half = (right - left) / 2 + 1; !cur->lchild)
                cur->lchild = new _BitsetNode(half, false, nullptr, nullptr);
            else
                cur->lchild->flip(half);
            if (int half = (right - left + 1) / 2; !cur->rchild)
                cur->rchild = new _BitsetNode(half, false, nullptr, nullptr);
            else
                cur->rchild->flip(half);
            cur->flipped = false;
        }
        void _update(_BitsetNode *cur) {
            cur->sum = (cur->lchild ? cur->lchild->sum : 0) + (cur->rchild ? cur->rchild->sum : 0);
            cur->flipped = false;
        }
        LazyBitset(int __n = 0) : m_length(__n) { m_root = new _BitsetNode(0, false, nullptr, nullptr); }
        void resize(int __n) {
            m_length = __n;
            reset();
        }
        void set() { m_root->sum = m_length; }
        void set(int __i) {
            auto dfs = [&](auto self, _BitsetNode *&cur, int left, int right) -> void {
                if (left == right) {
                    if (!cur)
                        cur = new _BitsetNode(1, false, nullptr, nullptr);
                    else
                        cur->sum = 1;
                } else if (!cur || cur->sum < right - left + 1) {
                    if (!cur)
                        cur = new _BitsetNode(0, false, nullptr, nullptr);
                    else if (!cur->sum)
                        _pushDown_zero(cur);
                    else if (cur->flipped)
                        _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; __i <= mid)
                        self(self, cur->lchild, left, mid);
                    else
                        self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void set(int __left, int __right) {
            auto dfs = [&](auto self, _BitsetNode *&cur, int left, int right) -> void {
                if (__left <= left && __right >= right) {
                    if (!cur)
                        cur = new _BitsetNode(right - left + 1, false, nullptr, nullptr);
                    else
                        cur->sum = right - left + 1;
                } else if (!cur || cur->sum < right - left + 1) {
                    if (!cur)
                        cur = new _BitsetNode(0, false, nullptr, nullptr);
                    else if (!cur->sum)
                        _pushDown_zero(cur);
                    else if (cur->flipped)
                        _pushDown_flip(cur, left, right);
                    int mid = (left + right) / 2;
                    if (__left <= mid) self(self, cur->lchild, left, mid);
                    if (__right > mid) self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void reset() { m_root->sum = 0; }
        void reset(int __i) {
            auto dfs = [&](auto self, _BitsetNode *&cur, int left, int right) -> void {
                if (left == right) {
                    if (cur) cur->sum = 0;
                } else if (cur && cur->sum) {
                    if (cur->sum == right - left + 1)
                        _pushDown_one(cur);
                    else if (cur->flipped)
                        _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; __i <= mid)
                        self(self, cur->lchild, left, mid);
                    else
                        self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void reset(int __left, int __right) {
            auto dfs = [&](auto self, _BitsetNode *&cur, int left, int right) -> void {
                if (__left <= left && __right >= right) {
                    if (cur) cur->sum = 0;
                } else if (cur && cur->sum) {
                    if (cur->sum == right - left + 1)
                        _pushDown_one(cur);
                    else if (cur->flipped)
                        _pushDown_flip(cur, left, right);
                    int mid = (left + right) / 2;
                    if (__left <= mid) self(self, cur->lchild, left, mid);
                    if (__right > mid) self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void flip() { m_root->flip(m_length); }
        void flip(int __i) {
            auto dfs = [&](auto self, _BitsetNode *&cur, int left, int right) -> void {
                if (left == right) {
                    if (!cur)
                        cur = new _BitsetNode(1, false, nullptr, nullptr);
                    else
                        cur->flip(1);
                } else {
                    if (!cur)
                        cur = new _BitsetNode(0, false, nullptr, nullptr);
                    else if (cur->sum == right - left + 1)
                        _pushDown_one(cur);
                    else if (!cur->sum)
                        _pushDown_zero(cur);
                    else if (cur->flipped)
                        _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; __i <= mid)
                        self(self, cur->lchild, left, mid);
                    else
                        self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        void flip(int __left, int __right) {
            auto dfs = [&](auto self, _BitsetNode *&cur, int left, int right) -> void {
                if (__left <= left && __right >= right) {
                    if (!cur)
                        cur = new _BitsetNode(right - left + 1, false, nullptr, nullptr);
                    else
                        cur->flip(right - left + 1);
                } else {
                    if (!cur)
                        cur = new _BitsetNode(0, false, nullptr, nullptr);
                    else if (cur->sum == right - left + 1)
                        _pushDown_one(cur);
                    else if (!cur->sum)
                        _pushDown_zero(cur);
                    else if (cur->flipped)
                        _pushDown_flip(cur, left, right);
                    int mid = (left + right) / 2;
                    if (__left <= mid) self(self, cur->lchild, left, mid);
                    if (__right > mid) self(self, cur->rchild, mid + 1, right);
                    _update(cur);
                }
            };
            dfs(dfs, m_root, 0, m_length - 1);
        }
        int first() const {
            if (!m_root->sum) return -1;
            auto dfs = [&](auto self, _BitsetNode *cur, int left, int right) -> int {
                if (cur->sum == right - left + 1)
                    return left;
                else {
                    if (cur->flipped) _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; cur->lchild && cur->lchild->sum)
                        return self(self, cur->lchild, left, mid);
                    else
                        return self(self, cur->rchild, mid + 1, right);
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        int prev(int __i) const {
            auto dfs = [&](auto self, _BitsetNode *cur, int left, int right) -> int {
                if (!cur || !cur->sum || __i == left) return -1;
                if (cur->sum == right - left + 1)
                    return std::min(right, __i - 1);
                else {
                    if (cur->flipped) _pushDown_flip(cur, left, right);
                    int mid = (left + right) / 2, res = -1;
                    if (__i > mid + 1) res = self(self, cur->rchild, mid + 1, right);
                    if (!~res) res = self(self, cur->lchild, left, mid);
                    return res;
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        int next(int __i) const {
            auto dfs = [&](auto self, _BitsetNode *cur, int left, int right) -> int {
                if (!cur || !cur->sum || __i == right) return -1;
                if (cur->sum == right - left + 1)
                    return std::max(left, __i + 1);
                else {
                    if (cur->flipped) _pushDown_flip(cur, left, right);
                    int mid = (left + right) / 2, res = -1;
                    if (__i < mid) res = self(self, cur->lchild, left, mid);
                    if (!~res) res = self(self, cur->rchild, mid + 1, right);
                    return res;
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        int last() const {
            if (!m_root->sum) return -1;
            auto dfs = [&](auto self, _BitsetNode *cur, int left, int right) -> int {
                if (cur->sum == right - left + 1)
                    return right;
                else {
                    if (cur->flipped) _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; cur->rchild && cur->rchild->sum)
                        return self(self, cur->rchild, mid + 1, right);
                    else
                        return self(self, cur->lchild, left, mid);
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        int count() const { return m_root->sum; }
        int count(int __left, int __right) const {
            auto dfs = [&](auto self, _BitsetNode *cur, int left, int right) -> int {
                if (__left <= left && __right >= right)
                    return cur ? cur->sum : 0;
                else if (!cur || !cur->sum)
                    return 0;
                else if (cur->sum == right - left + 1)
                    return std::min(right, __right) - std::max(left, __left) + 1;
                else {
                    if (cur->flipped) _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; __right <= mid)
                        return self(self, cur->lchild, left, mid);
                    else if (__left > mid)
                        return self(self, cur->rchild, mid + 1, right);
                    else
                        return self(self, cur->lchild, left, mid) + self(self, cur->rchild, mid + 1, right);
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        bool at(int __i) const {
            auto dfs = [&](auto self, _BitsetNode *cur, int left, int right) -> int {
                if (left == right)
                    return cur && cur->sum;
                else if (!cur || !cur->sum)
                    return 0;
                else if (cur->sum == right - left + 1)
                    return 1;
                else {
                    if (cur->flipped) _pushDown_flip(cur, left, right);
                    if (int mid = (left + right) / 2; __i <= mid)
                        return self(self, cur->lchild, left, mid);
                    else
                        return self(self, cur->rchild, mid + 1, right);
                }
            };
            return dfs(dfs, m_root, 0, m_length - 1);
        }
        bool operator[](int __i) const { return at(__i); }
        bool all() const { return count() == m_length; }
        bool any() const { return count(); }
    };
}

#endif