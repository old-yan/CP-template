/*
最后修改:
20240610
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_SEQUENCEMULTIHASH__
#define __OY_SEQUENCEMULTIHASH__

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <random>
#include <string>
#include <vector>

#include "SequenceHash.h"

namespace OY {
    namespace SEQMULHASH {
        using size_type = uint32_t;
        template <typename... Tps>
        struct MultiHashModInt;
        template <typename Tp>
        struct MultiHashModInt<Tp> {
            using mint = MultiHashModInt<Tp>;
            using mod_type = typename Tp::mod_type;
            Tp m_single;
            template <size_type I>
            using inner_type = Tp;
            MultiHashModInt() = default;
            template <typename Fp, typename std::enable_if<std::is_signed<Fp>::value>::type * = nullptr>
            MultiHashModInt(Fp val) : m_single(val) {}
            template <typename Fp, typename std::enable_if<std::is_unsigned<Fp>::value>::type * = nullptr>
            MultiHashModInt(Fp val) : m_single(val) {}
            static mint raw(mod_type val) {
                mint res;
                res.m_single = Tp::raw(val);
                return res;
            }
            mint pow(uint64_t n) const {
                mint res;
                res.m_single = m_single.pow(n);
                return res;
            }
            mint inv() const {
                mint res;
                res.m_single = m_single.inv();
                return res;
            }
            mint &operator++() {
                ++m_single;
                return *this;
            }
            mint &operator--() {
                --m_single;
                return *this;
            }
            mint operator++(int) {
                mint old(*this);
                ++*this;
                return old;
            }
            mint operator--(int) {
                mint old(*this);
                --*this;
                return old;
            }
            mint &operator+=(const mint &rhs) {
                m_single += rhs.m_single;
                return *this;
            }
            mint &operator-=(const mint &rhs) {
                m_single -= rhs.m_single;
                return *this;
            }
            mint &operator*=(const mint &rhs) {
                m_single *= rhs.m_single;
                return *this;
            }
            mint &operator/=(const mint &rhs) {
                m_single /= rhs.m_single;
                return *this;
            }
            mint operator+() const { return *this; }
            mint operator-() const {
                mint res;
                res.m_single = -m_single;
                return res;
            }
            bool operator==(const mint &rhs) const { return m_single == rhs.m_single; }
            bool operator!=(const mint &rhs) const { return m_single != rhs.m_single; }
            bool operator<(const mint &rhs) const { return m_single < rhs.m_single; }
            friend mint operator+(const mint &a, const mint &b) {
                mint res;
                res.m_single = a.m_single + b.m_single;
                return res;
            }
            friend mint operator-(const mint &a, const mint &b) {
                mint res;
                res.m_single = a.m_single - b.m_single;
                return res;
            }
            friend mint operator*(const mint &a, const mint &b) {
                mint res;
                res.m_single = a.m_single * b.m_single;
                return res;
            }
            friend mint operator/(const mint &a, const mint &b) {
                mint res;
                res.m_single = a.m_single / b.m_single;
                return res;
            }
        };
        template <typename First, typename... Rest>
        struct MultiHashModInt<First, Rest...> {
            using mint = MultiHashModInt<First, Rest...>;
            using rest_type = MultiHashModInt<Rest...>;
            using mod_type = typename std::common_type<typename First::mod_type, typename rest_type::mod_type>::type;
            template <size_type I>
            using inner_type = typename std::conditional<(I > 1), typename rest_type::template inner_type<I - 1>, First>::type;
            First m_first;
            rest_type m_rest;
            MultiHashModInt() = default;
            template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value>::type * = nullptr>
            MultiHashModInt(Tp val) : m_first(val), m_rest(val) {}
            template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value>::type * = nullptr>
            MultiHashModInt(Tp val) : m_first(val), m_rest(val) {}
            static mint raw(mod_type val) {
                mint res;
                res.m_first = First::raw(val);
                res.m_rest = rest_type::raw(val);
                return res;
            }
            template <typename... Args>
            static mint raw(typename First::mod_type first_val, Args... args) {
                mint res;
                res.m_first = First::raw(first_val);
                res.m_rest = rest_type::raw(args...);
                return res;
            }
            mint pow(uint64_t n) const {
                mint res;
                res.m_first = m_first.pow(n);
                res.m_rest = m_rest.pow(n);
                return res;
            }
            mint inv() const {
                mint res;
                res.m_first = m_first.inv();
                res.m_rest = m_rest.inv();
                return res;
            }
            mint &operator++() {
                ++m_first, ++m_rest;
                return *this;
            }
            mint &operator--() {
                --m_first, --m_rest;
                return *this;
            }
            mint operator++(int) {
                mint old(*this);
                ++*this;
                return old;
            }
            mint operator--(int) {
                mint old(*this);
                --*this;
                return old;
            }
            mint &operator+=(const mint &rhs) {
                m_first += rhs.m_first;
                m_rest += rhs.m_rest;
                return *this;
            }
            mint &operator-=(const mint &rhs) {
                m_first -= rhs.m_first;
                m_rest -= rhs.m_rest;
                return *this;
            }
            mint &operator*=(const mint &rhs) {
                m_first *= rhs.m_first;
                m_rest *= rhs.m_rest;
                return *this;
            }
            mint &operator/=(const mint &rhs) {
                m_first /= rhs.m_first;
                m_rest /= rhs.m_rest;
                return *this;
            }
            mint operator+() const { return *this; }
            mint operator-() const {
                mint res;
                res.m_first = -m_first;
                res.m_rest = -m_rest;
                return res;
            }
            bool operator==(const mint &rhs) const { return m_first == rhs.m_first && m_rest == rhs.m_rest; }
            bool operator!=(const mint &rhs) const { return m_first != rhs.m_first || m_rest != rhs.m_rest; }
            bool operator<(const mint &rhs) const { return m_first < rhs.m_first; }
            friend mint operator+(const mint &a, const mint &b) {
                mint res;
                res.m_first = a.m_first + b.m_first, res.m_rest = a.m_rest + b.m_rest;
                return res;
            }
            friend mint operator-(const mint &a, const mint &b) {
                mint res;
                res.m_first = a.m_first - b.m_first, res.m_rest = a.m_rest - b.m_rest;
                return res;
            }
            friend mint operator*(const mint &a, const mint &b) {
                mint res;
                res.m_first = a.m_first * b.m_first, res.m_rest = a.m_rest * b.m_rest;
                return res;
            }
            friend mint operator/(const mint &a, const mint &b) {
                mint res;
                res.m_first = a.m_first / b.m_first, res.m_rest = a.m_rest / b.m_rest;
                return res;
            }
        };
    }
    namespace SEQHASH {
        template <typename First, typename... Rest>
        struct SeqHashInfo<First, Rest...> {
            static constexpr size_type dim = sizeof...(Rest) + 1;
            using value_type = SEQMULHASH::MultiHashModInt<First, Rest...>;
            using mod_type = typename value_type::mod_type;
            mod_type m_bases[dim];
            value_type *m_u{}, *m_ui{}, *m_de{};
            template <typename... Args>
            void _set_random_base(mod_type min_base, Args... args) {
                if constexpr (sizeof...(Args) == dim)
                    set_base({(mod_type)args...});
                else {
                    mod_type low = min_base, high = value_type::template inner_type<sizeof...(Args)>::mod();
                    _set_random_base(min_base, args..., low + std::chrono::steady_clock::now().time_since_epoch().count() * 11995408973635179863ULL % (high - low));
                }
            }
            template <typename... Args>
            value_type _get_base(Args... args) {
                if constexpr (sizeof...(Args) == dim)
                    return value_type::raw(args...);
                else
                    return _get_base(args..., m_bases[sizeof...(Args)]);
            }
            void set_base(std::initializer_list<mod_type> bases) { std::copy(bases.begin(), bases.end(), m_bases); }
            template <typename... Args>
            void set_base(Args... bases) { set_base({(mod_type)bases...}); }
            void set_random_base(mod_type min_base = 128) { _set_random_base(min_base); }
            void set_random_base_and_prepare(size_type length, mod_type min_base = 128) { set_random_base(min_base), prepare(length); }
            void prepare(size_type length) {
                prepare_unit(length), prepare_unit_inv(length), prepare_de(length);
            }
            void prepare_unit(size_type length) {
                if (m_u) delete[] m_u;
                m_u = new value_type[length + 1];
                m_u[0] = value_type::raw(1);
                value_type u = _get_base();
                for (size_type i = 0; i != length; i++) m_u[i + 1] = m_u[i] * u;
            }
            void prepare_unit_inv(size_type length) {
                if (m_ui) delete[] m_ui;
                m_ui = new value_type[length + 1];
                m_ui[0] = value_type::raw(1);
                value_type ui = _get_base().inv();
                for (size_type i = 0; i != length; i++) m_ui[i + 1] = m_ui[i] * ui;
            }
            void prepare_de(size_type length) {
                if (m_de) delete[] m_de;
                m_de = new value_type[length + 1];
                m_de[0] = value_type::raw(1);
                value_type u = _get_base(), one = value_type::raw(1), s = one;
                for (size_type i = 0; i != length; i++) s *= u, m_de[i + 1] = s - one;
                for (size_type i = length - 1; i; i--) m_de[i] *= m_de[i + 1];
                value_type inv = m_de[1].inv();
                s = one;
                for (size_type i = 1; i != length; i++) s *= u, m_de[i] = inv * m_de[i + 1], inv *= s - one;
            }
        };
    }
}

#endif