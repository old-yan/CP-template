/*
最后修改:
20240425
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_KBIT__
#define __OY_KBIT__

#include <algorithm>
#include <array>
#include <assert.h>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace KBIT {
        using size_type = uint32_t;
        inline size_type lowbit(size_type x) { return x & -x; }
        template <typename Tp, size_type MAXK>
        struct InfoTable {
            static Tp s_qcoefs[MAXK + 1][MAXK][MAXK], s_dividers[MAXK + 1];
            static void prepare() {
                s_qcoefs[1][0][0] = Tp::raw(1);
                for (size_type k = 2; k <= MAXK; k++)
                    for (size_type p = 0; p < k; p++)
                        for (size_type q = 0; p + q < k; q++) s_qcoefs[k][p][q] = (p ? -s_qcoefs[k - 1][p - 1][q] : Tp()) + (q ? s_qcoefs[k - 1][p][q - 1] : Tp()) + (p + q < k - 1 ? s_qcoefs[k - 1][p][q] * Tp::raw(k - 1) : Tp());
                s_dividers[1] = Tp::raw(1);
                for (size_type k = 2; k <= MAXK; k++) s_dividers[k] = s_dividers[k - 1] * Tp::raw(k - 1).inv();
            }
            static Tp query_divider(size_type k) { return s_dividers[k]; }
            static Tp query_qcoef(size_type k, size_type p, size_type q) { return s_qcoefs[k][p][q]; }
        };
        template <typename Tp, size_type MAXK>
        Tp InfoTable<Tp, MAXK>::s_qcoefs[MAXK + 1][MAXK][MAXK];
        template <typename Tp, size_type MAXK>
        Tp InfoTable<Tp, MAXK>::s_dividers[MAXK + 1];
        template <typename Tp, size_type MAXK, typename Table>
        struct KBITNode {
            Tp m_val[MAXK];
            void init(size_type index, Tp val) {
                Tp e = Tp::raw(index);
                for (size_type i = 0; i != MAXK; i++) m_val[i] = val, val *= e;
            }
            Tp calc(size_type i) const {
                Tp pows[MAXK];
                pows[0] = Tp(1);
                for (size_type j = 1; j != MAXK; j++) pows[j] = pows[j - 1] * Tp::raw(i);
                Tp res{};
                for (size_type i = 0; i != MAXK; i++) {
                    Tp res2{};
                    for (size_type j = 0; j != MAXK - i; j++) res2 += pows[j] * Table::query_qcoef(MAXK, i, j);
                    res += res2 * m_val[i];
                }
                return res * Table::query_divider(MAXK);
            }
            Tp calc_dynamic(size_type k, size_type i) const {
                Tp pows[MAXK];
                pows[0] = Tp(1);
                for (size_type j = 1; j != k; j++) pows[j] = pows[j - 1] * Tp::raw(i);
                Tp res{};
                for (size_type i = 0; i != k; i++) {
                    Tp res2{};
                    for (size_type j = 0; j != k - i; j++) res2 += pows[j] * Table::query_qcoef(k, i, j);
                    res += res2 * m_val[i];
                }
                return res * Table::query_divider(k);
            }
            template <size_type K>
            KBITNode<Tp, MAXK, Table> &operator+=(const KBITNode<Tp, K, Table> &rhs) {
                for (size_type i = 0; i != MAXK; i++) m_val[i] += rhs.m_val[i];
                return *this;
            }
        };
        template <typename Tp, size_type MAXK>
        class Tree {
            using table = InfoTable<Tp, MAXK>;
            using node = KBITNode<Tp, MAXK, table>;
            size_type m_length;
            std::vector<node> m_sum;
            void _add(size_type i, Tp inc) {
                node *sum = m_sum.data(), a;
                a.init(i, inc);
                while (i < m_length) {
                    sum[i] += a;
                    i += lowbit(i + 1);
                }
            }
            template <size_type K, size_t L>
            void _add(size_type start, const std::array<Tp, L> &coefs) {
                Tp arr[K]{};
                for (size_type i = 0; i != K; i++) {
                    Tp e = Tp::raw(start + i), s = Tp::raw(1);
                    for (size_type j = 0; j != L; j++) arr[i] += coefs[j] * s, s *= e;
                }
                for (size_type i = 0; i != K; i++) std::adjacent_difference(arr, arr + K, arr);
                for (size_type i = 0, j = start; i != K && j != m_length; i++, j++) _add(j, arr[i]);
            }
            void _add_dynamic(size_type k, size_type start, const std::array<Tp, MAXK> &coefs) {
                Tp arr[MAXK]{};
                for (size_type i = 0; i != k; i++) {
                    Tp e = Tp::raw(start + i), s = Tp::raw(1);
                    for (size_type j = 0; j != k; j++) arr[i] += coefs[j] * s, s *= e;
                }
                for (size_type i = 0; i != k; i++) std::adjacent_difference(arr, arr + k, arr);
                for (size_type i = 0, j = start; i != k && j != m_length; i++, j++) _add(j, arr[i]);
            }
        public:
            static void prepare() { table::prepare(); }
            Tree() = default;
            Tree(size_type length) { resize(length); }
            template <typename InitMapping>
            Tree(size_type length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(size_type length) {
                if (!(m_length = length)) return;
                m_sum.assign(m_length, {});
            }
            template <typename InitMapping>
            void resize(size_type length, InitMapping mapping) {
                if (!(m_length = length)) return;
                m_sum.resize(m_length);
                node *sum = m_sum.data();
                for (size_type i = 0; i != m_length; i++) sum[i].init(i, mapping(i));
                for (size_type i = 0; i != m_length; i++) {
                    size_type j = i + lowbit(i + 1);
                    if (j < m_length) sum[j] += sum[i];
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void add(size_type i, Tp inc) { _add(i, inc); }
            template <size_type K = MAXK, size_t L>
            void add(size_type left, size_type right, std::array<Tp, L> coefs) {
                static_assert(K > 0 && K <= MAXK && L <= K, "K Must Be Between 1 and MAXK, L Mustn't Be Larger Than K");
                _add<K, L>(left, coefs);
                if (right + 1 < m_length) {
                    for (auto &e : coefs) e = -e;
                    _add<K, L>(right + 1, coefs);
                }
            }
            void add_dynamic(size_type k, size_type left, size_type right, std::array<Tp, MAXK> coefs) {
                assert(k > 0 && k <= MAXK);
                _add_dynamic(k, left, coefs);
                if (right + 1 < m_length) {
                    for (auto &e : coefs) e = -e;
                    _add_dynamic(k, right + 1, coefs);
                }
            }
            template <size_type K = MAXK>
            Tp query(size_type i) const {
                static_assert(K > 0 && K <= MAXK, "K Must Be Between 1 And MAXK");
                const node *sum = m_sum.data();
                KBITNode<Tp, K, table> res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += sum[j];
                return res.calc(i);
            }
            Tp query_dynamic(size_type k, size_type i) const {
                assert(k > 0 && k <= MAXK);
                const node *sum = m_sum.data();
                KBITNode<Tp, MAXK, table> res{};
                for (size_type j = i; ~j; j -= lowbit(j + 1)) res += sum[j];
                return res.calc_dynamic(k, i);
            }
            template <size_type K = MAXK - 1>
            Tp query(size_type left, size_type right) const {
                static_assert(K < MAXK, "K Must Be Between 0 And MAXK-1");
                return query<K + 1>(right) - query<K + 1>(left - 1);
            }
            Tp query_dynamic(size_type k, size_type left, size_type right) const {
                assert(k < MAXK);
                return query_dynamic(k + 1, right) - query_dynamic(k + 1, left - 1);
            }
        };
    };
}

#endif