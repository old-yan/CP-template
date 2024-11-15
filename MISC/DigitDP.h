/*
最后修改:
20241017
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_DIGITDP__
#define __OY_DIGITDP__

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "../TEST/std_bit.h"

namespace OY {
    namespace DIGITDP {
        using size_type = uint32_t;
        template <uint32_t Base>
        struct StaticIntegerString {
            std::vector<uint32_t> m_str;
            static constexpr uint32_t base() { return Base; }
            StaticIntegerString() = default;
            StaticIntegerString(uint64_t val) {
                do m_str.push_back(val % Base), val /= Base;
                while (val);
            }
            size_type size() const { return m_str.size(); }
            void assign(size_type n) { m_str.assign(n, 0); }
            uint32_t get(size_type i) const { return m_str[i]; }
            void set(size_type i, uint32_t val) { m_str[i] = val; }
            void push_high(uint32_t val) { m_str.push_back(val); }
            void pop_high() { m_str.pop_back(); }
            operator uint64_t() const {
                uint64_t val = 0;
                for (size_type i = size() - 1; ~i; i--) val = val * Base + get(i);
                return val;
            }
        };
        template <>
        struct StaticIntegerString<2> {
            std::vector<uint64_t> m_str;
            size_type m_size;
            static constexpr uint32_t base() { return 2; }
            StaticIntegerString() = default;
            StaticIntegerString(uint64_t val) {
                m_str.assign(1, val);
                m_size = val ? std::bit_width(val) : 1;
            }
            StaticIntegerString(std::string str) {
                size_type i = str.find('1');
                if (i == size_type(std::string::npos))
                    assign(1);
                else {
                    assign(str.size() - i);
                    for (size_type j = str.size() - 1; j != i - 1; j--) set(str.size() - 1 - j, str[j] - '0');
                }
            }
            explicit StaticIntegerString(const char *str) : StaticIntegerString(std::string(str)) {}
            size_type size() const { return m_size; }
            void assign(size_type n) { m_str.assign((n + 63) >> 6, 0), m_size = n; }
            uint32_t get(size_type i) const { return m_str[i >> 6] >> (i & 63) & 1; }
            void set(size_type i, uint32_t val) {
                if (val)
                    m_str[i >> 6] |= uint64_t(1) << (i & 63);
                else
                    m_str[i >> 6] &= ~(uint64_t(1) << (i & 63));
            }
            void push_high(uint32_t val) {
                if (!(m_size++ & 63))
                    m_str.push_back(val);
                else
                    set(m_size - 1, val);
            }
            void pop_high() {
                if (!(--m_size & 63))
                    m_str.pop_back();
                else
                    set(m_size, 0);
            }
            operator std::string() const {
                std::string str(m_size, '0');
                for (size_type i = 0; i != m_size; i++) str[i] = '0' + get(m_size - 1 - i);
                return str;
            }
            operator uint64_t() const { return m_str[0]; }
        };
        template <>
        struct StaticIntegerString<10> {
            std::string m_str;
            static constexpr uint32_t base() { return 10; }
            StaticIntegerString() = default;
            StaticIntegerString(uint64_t val) {
                do m_str += '0' + val % 10, val /= 10;
                while (val);
            }
            StaticIntegerString(std::string str) : m_str(str.rbegin(), str.rend()) {}
            explicit StaticIntegerString(const char *str) : StaticIntegerString(std::string(str)) {}
            size_type size() const { return m_str.size(); }
            void assign(size_type n) { m_str.assign(n, '0'); }
            uint32_t get(size_type i) const { return m_str[i] - '0'; }
            void set(size_type i, uint32_t val) { m_str[i] = '0' + val; }
            void push_high(uint32_t val) { m_str += '0' + val; }
            void pop_high() { m_str.pop_back(); }
            operator std::string() const { return std::string(m_str.rbegin(), m_str.rend()); }
            operator uint64_t() const {
                uint64_t val = 0;
                for (size_type i = size() - 1; ~i; i--) val = val * 10 + get(i);
                return val;
            }
        };
        struct DynamicIntegerString {
            std::vector<uint32_t> m_str;
            static uint32_t s_base;
            static void set_base(uint32_t base) { s_base = base; }
            static uint32_t base() { return s_base; }
            DynamicIntegerString() = default;
            DynamicIntegerString(uint64_t val) {
                do m_str.push_back(val % s_base), val /= s_base;
                while (val);
            }
            size_type size() const { return m_str.size(); }
            void assign(size_type n) { m_str.assign(n, 0); }
            uint32_t get(size_type i) const { return m_str[i]; }
            void set(size_type i, uint32_t val) { m_str[i] = val; }
            void push_high(uint32_t val) { m_str.push_back(val); }
            void pop_high() { m_str.pop_back(); }
            operator uint64_t() const {
                uint64_t val = 0;
                for (size_type i = size() - 1; ~i; i--) val = val * s_base + get(i);
                return val;
            }
        };
        uint32_t DynamicIntegerString::s_base;
        template <typename Ostream, uint32_t Base>
        Ostream &operator<<(Ostream &out, const StaticIntegerString<Base> &str) {
            for (size_type i = str.size() - 1; ~i; i--) out << str.get(i);
            return out;
        }
        template <typename Ostream>
        Ostream &operator<<(Ostream &out, const DynamicIntegerString &str) {
            for (size_type i = str.size() - 1; ~i; i--) out << str.get(i);
            return out;
        }
        template <typename IntStr>
        IntStr prev_number(IntStr s) {
            size_type n = s.size(), i = 0;
            while (i != n && !s.get(i)) s.set(i++, IntStr::base() - 1);
            s.set(i, s.get(i) - 1);
            if (n > 1 && i == n - 1 && !s.get(i)) s.pop_high();
            return s;
        }
        template <typename IntStr>
        IntStr next_number(IntStr s) {
            size_type n = s.size(), i = 0;
            while (i != n && s.get(i) == IntStr::base() - 1) s.set(i++, 0);
            if (i == n)
                s.push_high(1);
            else
                s.set(i, s.get(i) + 1);
            return s;
        }
        template <typename Tp, typename IntStr, bool Trace>
        struct Tracer {};
        template <typename Tp, typename IntStr>
        class Tracer<Tp, IntStr, true> {
        protected:
            std::vector<std::vector<std::vector<std::pair<uint32_t, uint32_t>>>> m_from;
            IntStr m_str;
            void init(uint32_t state_count, IntStr number) {
                m_str = number;
                m_from.assign(number.size(), std::vector<std::vector<std::pair<uint32_t, uint32_t>>>(state_count));
            }
            void add_trace(uint32_t state, uint32_t prev_state, size_type prev_len, uint32_t c) { m_from[prev_len][state].emplace_back(prev_state, c); }
            template <typename Callback>
            void _dfs1(size_type idx, uint32_t state, bool equal, IntStr &str, Callback &&call) {
                if (!~idx)
                    call(str);
                else {
                    for (auto &prev_state_c : m_from[idx][state]) {
                        auto prev_state = prev_state_c.first;
                        auto c = prev_state_c.second;
                        if (equal && c > m_str.get(idx)) continue;
                        if (!c && idx == str.size() - 1 && idx) continue;
                        str.set(idx, c);
                        _dfs1(idx - 1, prev_state, equal && c == m_str.get(idx), str, call);
                    }
                }
            }
            template <typename Callback>
            void _dfs2(size_type idx, uint32_t state, int comp, IntStr &str, Callback &&call) {
                if (!~idx) {
                    if (str.size() < m_str.size() || comp != 1) call(str);
                } else {
                    uint32_t cur = m_str.get(str.size() - 1 - idx);
                    for (auto &prev_state_c : m_from[idx][state]) {
                        auto prev_state = prev_state_c.first;
                        auto c = prev_state_c.second;
                        if (!idx && !c) continue;
                        str.set(str.size() - 1 - idx, c);
                        _dfs2(idx - 1, prev_state, c > cur ? 1 : (c < cur ? -1 : comp), str, call);
                    }
                }
            }
            template <typename Callback>
            void enumerate1(uint32_t state, size_type len, Callback &&call) {
                IntStr str;
                str.assign(len);
                _dfs1(len - 1, state, len == m_str.size(), str, call);
            }
            template <typename Callback>
            void enumerate2(uint32_t state, size_type len, Callback &&call) {
                IntStr str;
                str.assign(len);
                _dfs2(len - 1, state, 0, str, call);
            }
        };
        template <typename Tp, typename IntStr, bool Trace = false>
        struct AppendHighSolver : Tracer<Tp, IntStr, Trace> {
            using base_type = Tracer<Tp, IntStr, Trace>;
            size_type n;
            std::vector<std::array<Tp, 2>> dp, dp2;
            template <typename Transfer, typename PlusCallback, typename MinusCallback>
            void _solve_callback(IntStr number, uint32_t state_count, Transfer &&transfer, PlusCallback &&plus, MinusCallback &&minus) {
                n = number.size(), dp.assign(state_count, {}), dp2.assign(state_count, {});
                if constexpr (Trace) base_type::init(state_count, number);
                for (uint32_t c = 0; c != IntStr::base(); c++) {
                    uint32_t state = transfer(-1, 0, c);
                    if (!~state) continue;
                    dp[state][c > number.get(0)]++;
                    if constexpr (Trace) base_type::add_trace(state, -1, 0, c);
                    if (c) continue;
                    if (n - 1)
                        for (uint32_t state = 0; state != state_count; state++) minus(dp[state][0] + dp[state][1], state, 1);
                    else
                        for (uint32_t state = 0; state != state_count; state++) minus(dp[state][0], state, 1);
                }
                if (n - 1)
                    for (uint32_t state = 0; state != state_count; state++) plus(dp[state][0] + dp[state][1], state, 1);
                else
                    for (uint32_t state = 0; state != state_count; state++) plus(dp[state][0], state, 1);
                for (size_type i = 1; i != n; i++) {
                    uint32_t cur = number.get(i);
                    dp.swap(dp2);
                    dp.assign(state_count, {});
                    for (uint32_t c = 0; c != IntStr::base(); c++) {
                        for (uint32_t state = 0; state != state_count; state++)
                            if (dp2[state][0] || dp2[state][1]) {
                                uint32_t state2 = transfer(state, i, c);
                                if (~state2) {
                                    dp[state2][c > cur] += dp2[state][0], dp[state2][c >= cur] += dp2[state][1];
                                    if constexpr (Trace) base_type::add_trace(state2, state, i, c);
                                }
                            }
                        if (!c)
                            if (i + 1 < n)
                                for (uint32_t state = 0; state != state_count; state++) minus(dp[state][0] + dp[state][1], state, i + 1);
                            else
                                for (uint32_t state = 0; state != state_count; state++) minus(dp[state][0], state, i + 1);
                    }
                    if (i + 1 < n)
                        for (uint32_t state = 0; state != state_count; state++) plus(dp[state][0] + dp[state][1], state, i + 1);
                    else
                        for (uint32_t state = 0; state != state_count; state++) plus(dp[state][0], state, i + 1);
                }
            }
            template <typename Transfer, typename ValueMapping>
            Tp solve(IntStr number, uint32_t state_count, Transfer &&transfer, ValueMapping &&mapping) {
                Tp ans{};
                auto plus = [&](Tp cnt, uint32_t state, size_type len) { ans += cnt * mapping(state, len); };
                auto minus = [&](Tp cnt, uint32_t state, size_type len) { ans -= cnt * mapping(state, len); };
                _solve_callback(std::move(number), state_count, transfer, plus, minus);
                return ans;
            }
            template <typename ValueMapping, typename Callback>
            void enumerate(ValueMapping &&mapping, Callback &&call) {
                static_assert(Trace, "Trace Must Be True");
                uint32_t state_count = dp.size();
                for (size_type len = 1; len <= this->m_str.size(); len++)
                    for (uint32_t state = 0; state != state_count; state++)
                        if (this->m_from[len - 1][state].size() && mapping(state, len)) base_type::enumerate1(state, len, call);
            }
        };
        template <typename Tp, typename IntStr, bool Trace = false>
        struct AppendLowSolver : Tracer<Tp, IntStr, Trace> {
            using base_type = Tracer<Tp, IntStr, Trace>;
            size_type n;
            std::vector<std::array<Tp, 2>> dp, dp2;
            template <typename Transfer, typename Callback>
            void _solve_callback(IntStr number, uint32_t state_count, Transfer &&transfer, Callback &&call) {
                n = number.size(), dp.assign(state_count, {}), dp2.assign(state_count, {});
                std::pair<uint32_t, Tp> ceil{};
                if constexpr (Trace) base_type::init(state_count, number);
                for (uint32_t c = 1; c != IntStr::base(); c++) {
                    uint32_t state = transfer(-1, 0, c);
                    if (!~state) continue;
                    if (c == number.get(n - 1))
                        ceil = std::make_pair(state, 1);
                    else
                        dp[state][c > number.get(n - 1)]++;
                    if constexpr (Trace) base_type::add_trace(state, -1, 0, c);
                }
                if (n == 1)
                    for (uint32_t state = 0; state != state_count; state++) call(dp[state][0], state, 1);
                else
                    for (uint32_t state = 0; state != state_count; state++) call(dp[state][0] + dp[state][1], state, 1);
                call(ceil.second, ceil.first, 1);
                for (size_type i = n - 2; ~i; i--) {
                    uint32_t cur = number.get(i);
                    dp.swap(dp2);
                    dp.assign(state_count, {});
                    for (uint32_t state = 0; state != state_count; state++)
                        if (dp2[state][0] || dp2[state][1])
                            for (uint32_t c = 0; c != IntStr::base(); c++) {
                                uint32_t state2 = transfer(state, n - i - 1, c);
                                if (!~state2) continue;
                                dp[state2][0] += dp2[state][0], dp[state2][1] += dp2[state][1];
                                if constexpr (Trace) base_type::add_trace(state2, state, n - i - 1, c);
                            }
                    if (ceil.second) {
                        std::pair<uint32_t, Tp> next{};
                        for (uint32_t c = 0; c != IntStr::base(); c++) {
                            uint32_t state2 = transfer(ceil.first, n - i - 1, c);
                            if (!~state2) continue;
                            if (c == cur)
                                next = std::make_pair(state2, ceil.second);
                            else
                                dp[state2][c > cur] += ceil.second;
                            if constexpr (Trace) base_type::add_trace(state2, ceil.first, n - i - 1, c);
                        }
                        ceil = next;
                    }
                    if (!i)
                        for (uint32_t state = 0; state != state_count; state++) call(dp[state][0], state, n - i);
                    else
                        for (uint32_t state = 0; state != state_count; state++) call(dp[state][0] + dp[state][1], state, n - i);
                    call(ceil.second, ceil.first, n - i);
                }
            }
            template <typename Transfer, typename ValueMapping>
            Tp solve(IntStr number, uint32_t state_count, Transfer &&transfer, ValueMapping &&mapping) {
                Tp ans{};
                auto call = [&](Tp cnt, uint32_t state, size_type len) { ans += cnt * mapping(state, len); };
                _solve_callback(std::move(number), state_count, transfer, call);
                return ans;
            }
            template <typename ValueMapping, typename Callback>
            void enumerate(ValueMapping &&mapping, Callback &&call) {
                static_assert(Trace, "Trace Must Be True");
                uint32_t state_count = dp.size();
                for (size_type len = 1; len <= this->m_str.size(); len++)
                    for (uint32_t state = 0; state != state_count; state++)
                        if (this->m_from[len - 1][state].size() && mapping(state, len)) base_type::enumerate2(state, len, call);
            }
        };
        using IntStr2 = StaticIntegerString<2>;
        using IntStr10 = StaticIntegerString<10>;
        IntStr2 prev_number_base2(std::string s) { return prev_number(IntStr2(std::move(s))); }
        IntStr2 next_number_base2(std::string s) { return next_number(IntStr2(std::move(s))); }
        IntStr10 prev_number_base10(std::string s) { return prev_number(IntStr10(std::move(s))); }
        IntStr10 next_number_base10(std::string s) { return next_number(IntStr10(std::move(s))); }
    }
}

#endif
