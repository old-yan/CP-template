/*
最后修改:
20240521
测试环境:
gcc11.2,c++11
clang22.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTBIT__
#define __OY_PERSISTENTBIT__

#include <algorithm>

#include "../TEST/std_bit.h"
#include "GlobalHashMap.h"

namespace OY {
    namespace PerBIT {
        using size_type = uint32_t;
        template <typename SizeType>
        inline SizeType lowbit(SizeType x) { return x & -x; }
        template <typename Tp, typename SizeType>
        struct BITAdjacentNode {
            Tp m_val[2];
            Tp calc(SizeType i) const { return m_val[0] * i - m_val[1]; }
            BITAdjacentNode<Tp, SizeType> &operator+=(const BITAdjacentNode<Tp, SizeType> &rhs) {
                m_val[0] += rhs.m_val[0], m_val[1] += rhs.m_val[1];
                return *this;
            }
            BITAdjacentNode<Tp, SizeType> &operator-=(const BITAdjacentNode<Tp, SizeType> &rhs) {
                m_val[0] -= rhs.m_val[0], m_val[1] -= rhs.m_val[1];
                return *this;
            }
        };
        template <bool Rollbackable>
        struct VerMaintainer {
            std::vector<size_type> m_vers;
            std::vector<bool> m_isvalid;
            bool operator()(size_type ver) const { return m_isvalid[ver]; }
        };
        template <>
        struct VerMaintainer<false> {
            size_type m_cnt;
        };
        struct VectorTag {
            template <typename SizeType, typename Pair>
            struct type {
                std::vector<std::vector<Pair>> m_data;
                void resize(SizeType size) { m_data.assign(size, {}); }
                std::vector<Pair> *try_get(SizeType i) const { return i < m_data.size() ? (std::vector<Pair> *)&m_data[i] : nullptr; }
                std::vector<Pair> *get(SizeType i) const { return (std::vector<Pair> *)&m_data[i]; }
            };
        };
        template <bool MakeRecord, size_type BUFFER>
        struct HashmapTag {
            template <typename SizeType, typename Pair>
            struct type {
                using hash_table = GHASH::UnorderedMap<SizeType, std::vector<Pair> *, MakeRecord, BUFFER>;
                mutable hash_table m_data;
                ~type() {
                    if constexpr (MakeRecord) m_data.do_for_each([](typename hash_table::node *p) { delete p->m_mapped; });
                }
                void resize(SizeType size) {}
                std::vector<Pair> *try_get(SizeType i) const {
                    auto res = m_data.find(i);
                    return res ? res->m_mapped : nullptr;
                }
                std::vector<Pair> *get(SizeType i) const {
                    auto res = m_data.insert(i);
                    if (res.m_flag) res.m_ptr->m_mapped = new std::vector<Pair>;
                    return res.m_ptr->m_mapped;
                }
            };
        };
        template <typename Tp, typename SizeType, typename Tag, bool RangeUpdate = false, bool Rollbackable = false>
        class Tree {
            using node = typename std::conditional<RangeUpdate, BITAdjacentNode<Tp, SizeType>, Tp>::type;
            struct pair {
                size_type m_ver;
                node m_val;
            };
            struct vec_result {
                bool m_flag;
                std::vector<pair> *m_vec;
            };
            struct view {
                const Tree &m_tree;
                size_type m_ver;
                Tp query_all() const { return presum(m_tree.m_capacity - 1); }
                Tp query(SizeType i) const {
                    if constexpr (RangeUpdate) {
                        Tp res{};
                        for (SizeType j = i; ~j; j -= lowbit(j + 1)) m_tree._call_ver(m_ver, j, [&res](node &val) { res += val.m_val[0]; });
                        return res;
                    } else {
                        Tp res{};
                        m_tree._call_ver(m_ver, i, [&res](node &val) { res += val; });
                        for (SizeType j = i - 1, k = lowbit(i + 1); k >>= 1; j -= lowbit(j + 1)) m_tree._call_ver(m_ver, j, [&res](node &val) { res -= val; });
                        return res;
                    }
                }
                Tp query(SizeType left, SizeType right) const {
                    node res1{}, res2{};
                    right++;
                    SizeType mt = (left & -(SizeType(1) << std::bit_width<SizeType>(left ^ right))) - 1;
                    for (SizeType j = left - 1; j != mt; j -= lowbit(j + 1)) m_tree._call_ver(m_ver, j, [&res1](node &val) { res1 += val; });
                    for (SizeType j = right - 1; j != mt; j -= lowbit(j + 1)) m_tree._call_ver(m_ver, j, [&res2](node &val) { res2 += val; });
                    for (SizeType j = mt; ~j; j -= lowbit(j + 1)) m_tree._call_ver(m_ver, j, [&res1, &res2](node &val) { res1 += val, res2 += val; });
                    if constexpr (RangeUpdate)
                        return res2.m_val[0] * right - res2.m_val[1] - (res1.m_val[0] * left - res1.m_val[1]);
                    else
                        return res2 - res1;
                }
                Tp presum(SizeType i) const {
                    node res{};
                    for (SizeType j = i; ~j; j -= lowbit(j + 1)) m_tree._call_ver(m_ver, j, [&res](node &val) { res += val; });
                    if constexpr (RangeUpdate)
                        return res.m_val[0] * (i + 1) - res.m_val[1];
                    else
                        return res;
                }
                template <typename Ostream>
                friend Ostream &operator<<(Ostream &out, const view &x) {
                    out << "[";
                    for (SizeType i = 0; i < x.m_tree.m_size; i++) {
                        if (i) out << ", ";
                        out << x.query(i);
                    }
                    return out << "]";
                }
            };
            SizeType m_size, m_capacity;
            size_type m_ver;
            mutable typename Tag::template type<SizeType, pair> m_sum;
            VerMaintainer<Rollbackable> m_vm;
            vec_result _try_get(SizeType i) const {
                auto *ptr = m_sum.try_get(i);
                return {bool(ptr), ptr};
            }
            vec_result _try_get_reduce(SizeType i) const {
                auto res = _try_get(i);
                if constexpr (Rollbackable)
                    if (res.m_flag)
                        while (!res.m_vec->empty() && !m_vm(res.m_vec->back().m_ver)) res.m_vec->pop_back();
                return res;
            }
            std::vector<pair> &_get(SizeType i) const { return *m_sum.get(i); }
            std::vector<pair> &_get_and_reduce(SizeType i) const {
                auto &vec = _get(i);
                if constexpr (Rollbackable)
                    while (!vec.empty() && !m_vm(vec.back().m_ver)) vec.pop_back();
                return vec;
            }
            size_type _smallerbound(const std::vector<pair> &vec, size_type ver) const {
                size_type left = 0, right = vec.size();
                while (left != right) {
                    size_type mid = (left + right) / 2;
                    if (ver < vec[mid].m_ver)
                        right = mid;
                    else
                        left = mid + 1;
                }
                return left - 1;
            }
            template <typename Callback>
            void _call(SizeType i, Callback &&call) const {
                auto res = _try_get_reduce(i);
                if (res.m_flag && !res.m_vec->empty()) call(res.m_vec->back().m_val);
            }
            template <typename Callback>
            void _call_ver(size_type ver, SizeType i, Callback &&call) const {
                auto res = _try_get_reduce(i);
                if (res.m_flag) {
                    size_type pos = _smallerbound(*res.m_vec, ver);
                    if (~pos) call((*res.m_vec)[pos].m_val);
                }
            }
            void _add(SizeType i, const node &inc) {
                while (i < m_capacity) {
                    auto &vec = _get_and_reduce(i);
                    if (vec.empty())
                        vec.push_back({m_ver, inc});
                    else {
                        if (vec.back().m_ver != m_ver) vec.push_back({m_ver, vec.back().m_val});
                        vec.back().m_val += inc;
                    }
                    i += lowbit(i + 1);
                }
            }
        public:
            Tree() = default;
            Tree(SizeType length) { resize(length); }
            template <typename InitMapping>
            Tree(SizeType length, InitMapping mapping) { resize(length, mapping); }
            template <typename Iterator>
            Tree(Iterator first, Iterator last) { reset(first, last); }
            void resize(SizeType length) {
                if (!(m_size = length)) return;
                m_ver = 0, m_sum.resize(m_capacity = std::bit_ceil(length));
                if constexpr (Rollbackable) m_vm.m_vers.assign({m_ver}), m_vm.m_isvalid.assign(1, true);
            }
            template <typename InitMapping>
            void resize(SizeType length, InitMapping mapping) {
                if (!(m_size = length)) return;
                m_ver = 0, m_sum.resize(m_capacity = std::bit_ceil(length));
                if constexpr (Rollbackable) m_vm.m_vers.assign({m_ver}), m_vm.m_isvalid.assign(1, true);
                if constexpr (RangeUpdate) {
                    Tp temp{};
                    for (SizeType i = 0; i != length; i++) {
                        auto &vec = _get(i);
                        Tp cur = mapping(i);
                        vec.push_back({});
                        vec.back().m_val.m_val[0] = cur - temp, vec.back().m_val.m_val[1] = (cur - temp) * i, temp = cur;
                    }
                } else
                    for (SizeType i = 0; i != length; i++) _get(i).push_back({0, Tp(mapping(i))});
                for (SizeType i = 0; i != m_capacity; i++) {
                    auto find = _try_get(i);
                    if (find.m_flag && !find.m_vec->empty()) {
                        SizeType j = i + lowbit(i + 1);
                        if (j < m_capacity) {
                            auto &vec = _get(j);
                            if (vec.empty())
                                vec.push_back({0, (*find.m_vec)[0].m_val});
                            else
                                vec[0].m_val += (*find.m_vec)[0].m_val;
                        }
                    }
                }
            }
            template <typename Iterator>
            void reset(Iterator first, Iterator last) {
                resize(last - first, [&](size_type i) { return *(first + i); });
            }
            void reserve(size_type max_version) {
                if constexpr (Rollbackable) m_vm.m_vers.reserve(max_version + 1), m_vm.m_isvalid.reserve(max_version + 1);
            }
            void copy_as(size_type ver) {
                m_ver = ver;
                if constexpr (Rollbackable)
                    m_vm.m_vers.push_back(m_ver), m_vm.m_isvalid.resize(m_ver + 1, true);
                else
                    m_vm.m_cnt++;
            }
            size_type version_count() const {
                if constexpr (Rollbackable)
                    return m_vm.m_vers.size();
                else
                    return m_vm.m_cnt;
            }
            size_type kth_version(size_type k) const {
                static_assert(Rollbackable, "Rollbackable Must Be True");
                return m_vm.m_vers[k];
            }
            void rollback() {
                static_assert(Rollbackable, "Rollbackable Must Be True");
                m_vm.m_isvalid[m_vm.m_vers.back()] = false, m_vm.m_vers.pop_back();
            }
            void add(SizeType i, const Tp &inc) {
                if constexpr (RangeUpdate) {
                    _add(i, node{inc, Tp(inc * i)});
                    _add(i + 1, node{-inc, Tp(-inc * (i + 1))});
                } else
                    _add(i, inc);
            }
            void add(SizeType left, SizeType right, const Tp &inc) {
                static_assert(RangeUpdate, "RangeUpdate Must Be True");
                _add(right + 1, node{-inc, Tp(-inc * (right + 1))});
                _add(left, node{inc, Tp(inc * left)});
            }
            Tp presum(SizeType i) const {
                node res{};
                for (SizeType j = i; ~j; j -= lowbit(j + 1)) _call(j, [&res](node &val) { res += val; });
                if constexpr (RangeUpdate)
                    return res.m_val[0] * (i + 1) - res.m_val[1];
                else
                    return res;
            }
            Tp query(SizeType i) const {
                if constexpr (RangeUpdate) {
                    Tp res{};
                    for (SizeType j = i; ~j; j -= lowbit(j + 1)) _call(j, [&res](node &val) { res += val; });
                    return res;
                } else {
                    Tp res{};
                    _call(i, [&res](node &val) { res += val; });
                    for (SizeType j = i - 1, k = lowbit(i + 1); k >>= 1; j -= lowbit(j + 1)) _call(j, [&res](node &val) { res -= val; });
                    return res;
                }
            }
            Tp query(SizeType left, SizeType right) const {
                node res1{}, res2{};
                right++;
                SizeType mt = (left & -(SizeType(1) << std::bit_width<SizeType>(left ^ right))) - 1;
                for (SizeType j = left - 1; j != mt; j -= lowbit(j + 1)) _call(j, [&res1](node &val) { res1 += val; });
                for (SizeType j = right - 1; j != mt; j -= lowbit(j + 1)) _call(j, [&res2](node &val) { res2 += val; });
                for (SizeType j = mt; ~j; j -= lowbit(j + 1)) _call(j, [&res1, &res2](node &val) { res1 += val, res2 += val; });
                if constexpr (RangeUpdate)
                    return res2.m_val[0] * right - res2.m_val[1] - (res1.m_val[0] * left - res1.m_val[1]);
                else
                    return res2 - res1;
            }
            Tp query_all() const { return presum(m_capacity - 1); }
            SizeType reduce_kth(size_type base_ver, size_type end_ver, Tp k) {
                if constexpr (RangeUpdate) {
                    SizeType cursor = -1;
                    node cur{};
                    for (SizeType d = m_capacity >> 1; d; d >>= 1) {
                        auto res = _try_get_reduce(cursor + d);
                        if (!res.m_flag) continue;
                        auto &vec = *res.m_vec;
                        size_type pos1 = _smallerbound(vec, base_ver), pos2 = _smallerbound(vec, end_ver);
                        node val{cur};
                        if (~pos1) val -= vec[pos1].m_val;
                        if (~pos2) val += vec[pos2].m_val;
                        if (val.calc(cursor + d + 1) <= k) cur = val, cursor += d;
                    }
                    return cursor + 1;
                } else {
                    SizeType cursor = -1;
                    for (SizeType d = m_capacity >> 1; d; d >>= 1) {
                        auto res = _try_get_reduce(cursor + d);
                        if (!res.m_flag) {
                            cursor += d;
                            continue;
                        }
                        auto &vec = *res.m_vec;
                        size_type pos1 = _smallerbound(vec, base_ver), pos2 = _smallerbound(vec, end_ver);
                        Tp val{};
                        if (~pos1) val -= vec[pos1].m_val;
                        if (~pos2) val += vec[pos2].m_val;
                        if (val <= k) k -= val, cursor += d;
                    }
                    return cursor + 1;
                }
            }
            view get_version_view(size_type ver) const { return {*this, ver}; }
        };
    }
}

#endif