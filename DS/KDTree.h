#ifndef __OY_KDTREE__
#define __OY_KDTREE__

#include <functional>
#include <limits>
#include "MemoryPool.h"

namespace OY {
    template <typename _Tp, int _K>
    struct KDTreeRectRange {
        _Tp min[_K];
        _Tp max[_K];
        template <typename treenode>
        bool intersect(const treenode &rect) const {
            for (int i = 0; i < _K; i++)
                if (rect.max[i] < min[i] || rect.min[i] > max[i]) return false;
            return true;
        }
        template <typename treenode>
        bool contain_rect(const treenode &rect) const {
            for (int i = 0; i < _K; i++)
                if (rect.min[i] < min[i] || rect.max[i] > max[i]) return false;
            return true;
        }
        template <typename node>
        bool contain_point(const node &point) const {
            for (int i = 0; i < _K; i++)
                if (point.pos[i] < min[i] || point.pos[i] > max[i]) return false;
            return true;
        }
    };
    template <typename _Tp, int _K, typename _TpDistance>
    struct KDTreeCircleRange {
        _Tp pos[_K];
        _TpDistance radius_square;
        template <typename treenode>
        bool intersect(const treenode &rect) const {
            bool out = false;
            for (int i = 0; i < _K; i++)
                if (pos[i] < rect.min[i] || pos[i] > rect.max[i]) {
                    out = true;
                    break;
                }
            if (!out) return true;
            _TpDistance dis = 0;
            for (int i = 0; i < _K; i++) {
                _TpDistance p = 0;
                if (pos[i] < rect.min[i])
                    p = rect.min[i] - pos[i];
                else if (pos[i] > rect.max[i])
                    p = rect.max[i] - pos[i];
                dis += p * p;
            }
            return dis <= radius_square;
        }
        template <typename treenode>
        bool contain_rect(const treenode &rect) const {
            _TpDistance dis = 0;
            for (int i = 0; i < _K; i++) {
                _TpDistance p = std::max(std::abs(rect.min[i] - pos[i]), std::abs(rect.max[i] - pos[i]));
                dis += p * p;
            }
            return dis <= radius_square;
        }
        template <typename node>
        bool contain_point(const node &point) const {
            _TpDistance dis = 0;
            for (int i = 0; i < _K; i++) {
                _TpDistance p = point.pos[i] - pos[i];
                dis += p * p;
            }
            return dis <= radius_square;
        }
    };
    template <typename _Tp, typename _Fp, typename _Operation = std::plus<_Fp>, int _K = 2, int _N = 31>
    class KDTree {
    public:
        struct node : MemoryPool<node> {
            _Tp pos[_K];
            _Fp value;
            node() {}
            node(const std::initializer_list<_Tp> &_pos, _Fp _value = _Fp()) : value(_value) { std::copy(_pos.begin(), _pos.end(), pos); }
        };
        struct treenode : MemoryPool<treenode> {
            _Tp min[_K];
            _Tp max[_K];
            int size;
            _Tp thresh;
            _Fp value;
            std::vector<node *> points;
            treenode *lchild;
            treenode *rchild;
            treenode(_Fp _defaultValue = _Fp()) : size(0), value(_defaultValue), lchild(nullptr), rchild(nullptr) {}
            treenode(node *point) : size(1), points({point}), lchild(nullptr), rchild(nullptr) {
                for (int i = 0; i < _K; i++) min[i] = max[i] = point->pos[i];
                value = point->value;
            }
            bool contain_point(const std::initializer_list<_Tp> &pos) const {
                for (int i = 0; i < _K; i++)
                    if (_Tp x = *(pos.begin() + i); x < min[i] || x > max[i]) return false;
                return true;
            }
        };
        treenode *m_root;

    private:
        _Operation m_op;
        _Fp m_defaultValue;
        std::vector<node *> m_buffer;
        static constexpr int m_ratio = 4, m_bias = 4;
        static treenode *updateRange(treenode *p, node *q) {
            for (int i = 0; i < _K; i++) {
                _Tp pos = q->pos[i];
                if (pos < p->min[i])
                    p->min[i] = pos;
                else if (pos > p->max[i])
                    p->max[i] = pos;
            }
            return p;
        }
        static treenode *updateRange(treenode *p, const std::initializer_list<_Tp> &pos) {
            for (int i = 0; i < _K; i++) {
                _Tp _pos = *(pos.begin() + i);
                if (_pos < p->min[i])
                    p->min[i] = _pos;
                else if (_pos > p->max[i])
                    p->max[i] = _pos;
            }
            return p;
        }
        treenode *updateByChild(treenode *p) {
            std::copy(p->lchild->min, p->lchild->min + _K, p->min);
            std::copy(p->lchild->max, p->lchild->max + _K, p->max);
            for (int i = 0; i < _K; i++)
                if (p->rchild->min[i] < p->min[i]) p->min[i] = p->rchild->min[i];
            for (int i = 0; i < _K; i++)
                if (p->rchild->max[i] > p->max[i]) p->max[i] = p->rchild->max[i];
            p->size = p->lchild->size + p->rchild->size;
            p->value = m_op(p->lchild->value, p->rchild->value);
            return p;
        }
        treenode *updateBySelf(treenode *p) {
            p->size = p->points.size();
            p->value = p->points[0]->value;
            for (int i = 1; i < p->points.size(); i++) p->value = m_op(p->value, p->points[i]->value);
            return p;
        }
        treenode *update(treenode *p, const std::initializer_list<_Tp> &pos) {
            for (int i = 0; i < _K; i++) {
                _Tp _pos = *(pos.begin() + i);
                if (_pos < p->min[i])
                    p->min[i] = _pos;
                else if (_pos > p->max[i])
                    p->max[i] = _pos;
            }
            if (p->lchild) {
                p->size = p->lchild->size + p->rchild->size;
                p->value = m_op(p->lchild->value, p->rchild->value);
            } else {
                p->size = p->points.size();
                p->value = p->points[0]->value;
                for (int i = 1; i < p->points.size(); i++) p->value = m_op(p->value, p->points[i]->value);
            }
            return p;
        }
        treenode *update(treenode *p) {
            if (p->lchild) {
                p->size = p->lchild->size + p->rchild->size;
                p->value = m_op(p->lchild->value, p->rchild->value);
            } else {
                p->size = p->points.size();
                p->value = p->points[0]->value;
                for (int i = 1; i < p->points.size(); i++) p->value = m_op(p->value, p->points[i]->value);
            }
            return p;
        }
        treenode *balance(treenode *cur, int i) {
            if (cur->lchild) {
                if (cur->lchild->size + cur->rchild->size <= _N || cur->lchild->size > cur->rchild->size * m_ratio + m_bias || cur->rchild->size > cur->lchild->size * m_ratio + m_bias) return rebuild(cur, i);
            } else if (cur->points.size() > _N)
                return rebuild(cur, i);
            return cur;
        }
        void traverse(treenode *p) {
            if (!p->lchild && !p->rchild) {
                for (node *q : p->points) m_buffer.push_back(q);
            } else {
                if (p->lchild) traverse(p->lchild);
                if (p->rchild) traverse(p->rchild);
            }
            delete p;
        }
        treenode *make_tree(node **first, node **last, int i) {
            if (first == last) return nullptr;
            if (last - first <= _N) {
                treenode *p = new treenode(*first++);
                while (first < last) {
                    p->points.push_back(*first);
                    p = updateRange(p, *first++);
                }
                return updateBySelf(p);
            } else {
                treenode *p = new treenode(m_defaultValue);
                node **mid = first + (last - first) / 2;
                std::nth_element(first, mid, last, [&](const node *x, const node *y) { return x->pos[i] < y->pos[i]; });
                p->thresh = (*mid)->pos[i];
                p->lchild = make_tree(first, mid, i + 1 < _K ? i + 1 : 0);
                p->rchild = make_tree(mid, last, i + 1 < _K ? i + 1 : 0);
                return updateByChild(p);
            }
        }
        treenode *rebuild(treenode *cur, int i) {
            if (cur) traverse(cur);
            cur = make_tree(m_buffer.data(), m_buffer.data() + m_buffer.size(), i);
            m_buffer.clear();
            return cur;
        }

    public:
        static void setBufferSize(int __treeCount, int __nodeCount) {
            MemoryPool<treenode>::_reserve(__treeCount);
            MemoryPool<node>::_reserve(__nodeCount);
        }
        KDTree(_Operation __op = _Operation(), _Fp __defaultValue = _Fp()) : m_root(nullptr), m_op(__op), m_defaultValue(__defaultValue) {}
        ~KDTree() { clear(); }
        void clear() {
            // auto dfs=[](auto self,treenode*p){
            //     if(p->lchild){
            //         self(self,p->lchild);
            //         self(self,p->rchild);
            //     }
            //     else for(node*q:p->points)delete q;
            //     delete p;
            // };
            // if(m_root)dfs(dfs,m_root);
            m_root = nullptr;
        }
        void prepared_insert(const std::initializer_list<_Tp> &__pos, _Fp __value = _Fp(1)) { m_buffer.push_back(new node(__pos, __value)); }
        void prepared_build() {
            m_root = make_tree(m_buffer.data(), m_buffer.data() + m_buffer.size(), 0);
            m_buffer.clear();
        }
        void insert(const std::initializer_list<_Tp> &__pos, _Fp __value = _Fp(1)) {
            auto dfs = [&](auto self, treenode *cur, int i) -> treenode * {
                if (!cur)
                    return new treenode(new node(__pos, __value));
                else if (!cur->lchild)
                    cur->points.push_back(new node(__pos, __value));
                else if (*(__pos.begin() + i) < cur->thresh)
                    cur->lchild = self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                else
                    cur->rchild = self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                return balance(update(cur, __pos), i);
            };
            m_root = dfs(dfs, m_root, 0);
        }
        bool erase(const std::initializer_list<_Tp> &__pos) {
            bool res = false;
            auto dfs = [&](auto self, treenode *cur, int i) -> treenode * {
                if (!cur)
                    return nullptr;
                else if (!cur->lchild) {
                    auto it = std::find_if(cur->points.begin(), cur->points.end(), [&](const node *p) {
                        for (int i = 0; i < _K; i++)
                            if (p->pos[i] != *(__pos.begin() + i)) return false;
                        return true;
                    });
                    if (it != cur->points.end()) {
                        res = true;
                        cur->points.erase(it);
                    }
                } else {
                    if (*(__pos.begin() + i) <= cur->thresh) cur->lchild = self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                    if (!res && *(__pos.begin() + i) >= cur->thresh) cur->rchild = self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                }
                return res ? balance(update(cur), i) : cur;
            };
            m_root = dfs(dfs, m_root, 0);
            return res;
        }
        const node *find(const std::initializer_list<_Tp> &__pos) const {
            node *res = nullptr;
            auto dfs = [&](auto self, treenode *cur, int i) {
                if (!cur)
                    return;
                else if (!cur->lchild) {
                    auto it = std::find_if(cur->points.begin(), cur->points.end(), [&](const node *p) {
                        for (int i = 0; i < _K; i++)
                            if (p->pos[i] != *(__pos.begin() + i)) return false;
                        return true;
                    });
                    if (it != cur->points.end()) res = *it;
                } else {
                    if (*(__pos.begin() + i) <= cur->thresh) self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                    if (!res && *(__pos.begin() + i) >= cur->thresh) self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                }
            };
            dfs(dfs, m_root, 0);
            return res;
        }
        template <typename _Range>
        _Fp queryNumber(const _Range &__range) const {
            _Fp res = m_defaultValue;
            auto dfs = [&](auto self, treenode *cur) {
                if (!__range.intersect(*cur)) return;
                if (__range.contain_rect(*cur))
                    res = m_op(res, cur->value);
                else if (cur->lchild) {
                    if (cur->lchild) self(self, cur->lchild);
                    if (cur->rchild) self(self, cur->rchild);
                } else
                    for (node *point : cur->points)
                        if (__range.contain_point(*point)) res = m_op(res, point->value);
            };
            if (m_root) dfs(dfs, m_root);
            return res;
        }
        _Fp queryNumber_rect(const KDTreeRectRange<_Tp, _K> &__range) const {
            return queryNumber(__range);
        }
        template <typename _TpDistance = _Tp>
        _Fp queryNumber_circle(const KDTreeCircleRange<_Tp, _K, _TpDistance> &__range) const {
            return queryNumber(__range);
        }
        template <typename _TpDistance, bool _DropZero, typename _DistanceConvert>
        _TpDistance queryClosest(const std::initializer_list<_Tp> &__pos, _DistanceConvert __conv) const {
            _TpDistance dis = std::numeric_limits<_TpDistance>::max();
            auto dfs = [&](auto self, treenode *cur, int i) {
                if (!cur->contain_point(__pos)) {
                    _TpDistance d = 0;
                    for (int i = 0; i < _K; i++) {
                        _TpDistance di = 0;
                        if (_Tp pos = *(__pos.begin() + i); pos < cur->min[i])
                            di = cur->min[i] - pos;
                        else if (pos > cur->max[i])
                            di = pos - cur->max[i];
                        d += __conv(di);
                    }
                    if (dis <= d) return;
                }
                if (cur->lchild) {
                    if (*(__pos.begin() + i) < cur->thresh) {
                        self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                        self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                    } else {
                        self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                        self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                    }
                } else {
                    for (auto p : cur->points) {
                        _TpDistance d = 0;
                        for (int i = 0; i < _K; i++) {
                            _TpDistance di;
                            if (_Tp pos = *(__pos.begin() + i); pos < p->pos[i])
                                di = p->pos[i] - pos;
                            else
                                di = pos - p->pos[i];
                            d += __conv(di);
                        }
                        if constexpr (_DropZero) {
                            if (d && d < dis) dis = d;
                        } else {
                            if (d < dis) dis = d;
                        }
                    }
                }
            };
            if (m_root) dfs(dfs, m_root, 0);
            return dis;
        }
        template <typename _TpDistance = _Tp>
        _TpDistance queryClosest_Euclidean(const std::initializer_list<_Tp> &__pos) const {
            return queryClosest<_TpDistance, true>(__pos, [](auto x) { return x * x; });
        }
        template <typename _TpDistance = _Tp>
        _TpDistance queryClosest_Manhattan(const std::initializer_list<_Tp> &__pos) const {
            return queryClosest<_TpDistance, true>(__pos, [](auto x) { return x; });
        }
        template <typename _TpDistance, typename _DistanceConvert>
        _TpDistance queryFurthest(const std::initializer_list<_Tp> &__pos, _DistanceConvert __conv) const {
            _TpDistance dis = 0;
            auto dfs = [&](auto self, treenode *cur, int i) {
                _TpDistance d = 0;
                for (int i = 0; i < _K; i++) {
                    _Tp pos = *(__pos.begin() + i);
                    _TpDistance di = std::max(std::abs(pos - cur->min[i]), std::abs(pos - cur->max[i]));
                    d += __conv(di);
                }
                if (dis >= d) return;
                if (cur->lchild) {
                    if (*(__pos.begin() + i) < cur->thresh) {
                        self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                        self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                    } else {
                        self(self, cur->lchild, i + 1 < _K ? i + 1 : 0);
                        self(self, cur->rchild, i + 1 < _K ? i + 1 : 0);
                    }
                } else {
                    for (auto p : cur->points) {
                        _TpDistance d = 0;
                        for (int i = 0; i < _K; i++) {
                            _TpDistance di;
                            if (_Tp pos = *(__pos.begin() + i); pos < p->pos[i])
                                di = p->pos[i] - pos;
                            else
                                di = pos - p->pos[i];
                            d += __conv(di);
                        }
                        if (d > dis) dis = d;
                    }
                }
            };
            if (m_root) dfs(dfs, m_root, 0);
            return dis;
        }
        template <typename _TpDistance = _Tp>
        _TpDistance queryFurthest_Euclidean(const std::initializer_list<_Tp> &__pos) const {
            return queryFurthest<_TpDistance>(__pos, [](auto x) { return x * x; });
        }
        template <typename _TpDistance = _Tp>
        _TpDistance queryFurthest_Manhattan(const std::initializer_list<_Tp> &__pos) const {
            return queryFurthest<_TpDistance>(__pos, [](auto x) { return x; });
        }
    };
}

#endif