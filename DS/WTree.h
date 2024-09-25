/*
最后修改:
20240922
测试环境:
gcc11.2,c++17
clang12.0,C++17
*/
#ifndef __OY_WTREE__
#define __OY_WTREE__

#include <algorithm>
#include <cstdint>
#include <numeric>

namespace OY {
    namespace WTree {
        using size_type = size_t;
        struct Plus {
            template <typename Tp1, typename Tp2>
            void operator()(Tp1 &a, const Tp2 &b) const { a += b; }
            template <typename Tp>
            void operator()(Tp &a) const { a = -a; }
        };
        struct BitXor {
            template <typename Tp1, typename Tp2>
            void operator()(Tp1 &a, const Tp2 &b) const { a ^= b; }
            template <typename Tp>
            void operator()(Tp &a) const {}
        };
        template <typename Tp, typename Operation = Plus>
        class Tree {
        public:
            static constexpr size_type Z = 64, W = Z / sizeof(Tp), b = __builtin_ctz(W);
            typedef Tp vec_type __attribute((vector_size(Z / 2)));
        private:
            static constexpr size_type _calc_height(size_type n) { return n <= W ? 1 : _calc_height((n + W - 1) / W) + 1; }
            static constexpr size_type _calc_offset(size_type h, size_type len) {
                size_type s = 0, n = len + 1;
                while (h--) s += (n + W - 1) & -W, n = (n + W - 1) >> b;
                return s;
            }
            struct Pre {
                Tp m_mask[W][W];
                constexpr Pre() : m_mask{} {
                    for (size_type i = 0; i != W; i++)
                        for (size_type j = i + 1; j != W; j++) m_mask[i][j] = -1;
                }
            };
            static constexpr Pre pre{};
            Tp *m_data;
            size_type m_size, m_height, m_offset[10];
            template <typename InitMapping>
            Tp _init(size_type h, size_type cur, size_type &index, InitMapping &&mapping) {
                Tp sum{};
                if (!h)
                    for (size_type i = 0; i != W; i++, index++) {
                        m_data[cur + i] = sum;
                        if (index < m_size) Operation()(sum, mapping(index));
                    }
                else {
                    size_type nxt = (cur - m_offset[h]) * W + m_offset[h - 1];
                    for (size_type i = 0; i != W && index <= m_size; i++) m_data[cur + i] = sum, Operation()(sum, _init(h - 1, nxt, index, mapping)), nxt += W;
                }
                return sum;
            }
        public:
            Tree() : m_data{} {}
            Tree(size_type length) : m_data{} { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) : m_data{} { resize(length, mapping); }
            size_type size() const { return m_size; }
            void resize(size_type length) {
                clear();
                m_size = length;
                m_height = _calc_height(m_size + 1);
                for (size_type i = 0; i != m_height; i++) m_offset[i] = _calc_offset(i, m_size);
                size_type buf_len = _calc_offset(m_height, m_size);
                m_data = new (std::align_val_t(sizeof(Tp) * W)) Tp[buf_len]{};
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                clear();
                m_size = length;
                m_height = _calc_height(m_size + 1);
                for (size_type i = 0; i != m_height; i++) m_offset[i] = _calc_offset(i, m_size);
                size_type buf_len = _calc_offset(m_height, m_size);
                m_data = new (std::align_val_t(sizeof(Tp) * W)) Tp[buf_len]{};
                size_type index = 0;
                _init(m_height - 1, m_offset[m_height - 1], index, mapping);
            }
            ~Tree() { clear(); }
            void clear() {
                if (m_data) ::operator delete[](m_data, std::align_val_t(sizeof(Tp) * W));
            }
            void regard_as(size_type length) {
                m_size = length;
                m_height = _calc_height(m_size + 1);
                for (size_type i = 0; i != m_height; i++) m_offset[i] = _calc_offset(i, m_size);
            }
            Tp presum(size_type i) const {
                Tp res{};
#pragma GCC unroll 64
                for (size_type h = 0; h != m_height; h++) Operation()(res, m_data[m_offset[h] + (i + 1 >> (h * b))]);
                return res;
            }
            Tp query(size_type left, size_type right) const {
                auto vl = presum(left - 1), vr = presum(right);
                Operation()(vl), Operation()(vr, vl);
                return vr;
            }
            Tp query_all() const { return presum(m_size - 1); }
            void add(size_type i, const Tp &inc) {
                vec_type v{};
                v += inc;
#pragma GCC unroll 64
                for (size_type h = 0; h != m_height; h++) {
                    auto t = (vec_type *)&m_data[m_offset[h] + (i >> (h * b) & -W)];
                    auto m = (vec_type *)pre.m_mask[i >> (h * b) & (W - 1)];
                    Operation()(t[0], v & m[0]), Operation()(t[1], v & m[1]);
                }
            }
        };
        template <typename Ostream, typename Tp, typename Operation>
        Ostream &operator<<(Ostream &out, const Tree<Tp, Operation> &x) {
            out << "[";
            for (size_type i = 0; i != x.size(); i++) {
                if (i) out << ", ";
                out << x.presum(i);
            }
            return out << "]";
        }
    }
    template <typename Tp>
    using WSumTree = WTree::Tree<Tp, WTree::Plus>;
    template <typename Tp>
    using WXorTree = WTree::Tree<Tp, WTree::BitXor>;
}

#endif