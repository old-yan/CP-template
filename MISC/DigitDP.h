/*
最后修改:
20240924
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

namespace OY {
    namespace DIGITDP {
        using size_type = uint32_t;
        template <uint32_t Base>
        struct IntegerString {
            std::vector<uint32_t> m_str;
            IntegerString() = default;
            IntegerString(uint64_t val) {
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
        struct IntegerString<10> {
            std::string m_str;
            IntegerString() = default;
            IntegerString(uint64_t val) {
                do m_str += '0' + val % 10, val /= 10;
                while (val);
            }
            IntegerString(std::string str) : m_str(str.rbegin(), str.rend()) {}
            explicit IntegerString(const char *str) : IntegerString(std::string(str)) {}
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
        template <uint32_t Base>
        IntegerString<Base> prev_number(IntegerString<Base> s) {
            size_type n = s.size(), i = 0;
            while (i != n && !s.get(i)) s.set(i++, Base - 1);
            s.set(i, s.get(i) - 1);
            if (n > 1 && i == n - 1 && !s.get(i)) s.pop_high();
            return s;
        }
        template <uint32_t Base>
        IntegerString<Base> next_number(IntegerString<Base> s) {
            size_type n = s.size(), i = 0;
            while (i != n && s.get(i) == Base - 1) s.set(i++, 0);
            if (i == n)
                s.push_high(1);
            else
                s.set(i, s.get(i) + 1);
            return s;
        }
        template <typename Tp, uint32_t Base, bool Trace>
        struct Tracer {};
        template <typename Tp, uint32_t Base>
        struct Tracer<Tp, Base, true> {
            std::vector<std::vector<std::vector<std::pair<uint32_t, uint32_t>>>> m_from;
            IntegerString<Base> m_str;
            void init(uint32_t state_count, IntegerString<Base> number) {
                m_str = number;
                m_from.assign(number.size(), std::vector<std::vector<std::pair<uint32_t, uint32_t>>>(state_count));
            }
            void add_trace(uint32_t state, uint32_t prev_state, size_type prev_len, uint32_t c) { m_from[prev_len][state].emplace_back(prev_state, c); }
            template <typename Callback>
            void _dfs(size_type idx, uint32_t state, bool equal, IntegerString<Base> &str, Callback &&call) {
                if (!~idx)
                    call(str);
                else {
                    for (auto &prev_state_c : m_from[idx][state]) {
                        auto prev_state = prev_state_c.first;
                        auto c = prev_state_c.second;
                        if (equal && c > m_str.get(idx)) continue;
                        if (!c && idx == str.size() - 1 && idx) continue;
                        str.set(idx, c);
                        _dfs(idx - 1, prev_state, equal && c == m_str.get(idx), str, call);
                    }
                }
            }
            template <typename Callback>
            void enumerate(uint32_t state, size_type len, Callback &&call) {
                IntegerString<Base> str;
                str.assign(len);
                _dfs(len - 1, state, len == m_str.size(), str, call);
            }
        };
        template <typename Tp, uint32_t Base, bool Trace = false>
        struct Solver : Tracer<Tp, Base, Trace> {
            using base_type = Tracer<Tp, Base, Trace>;
            size_type n;
            std::vector<std::array<Tp, 2>> dp, dp2;
            template <typename Transfer, typename PlusCallback, typename MinusCallback>
            void _solve_callback(IntegerString<Base> number, uint32_t state_count, Transfer &&transfer, PlusCallback &&plus, MinusCallback &&minus) {
                n = number.size(), dp.assign(state_count, {}), dp2.assign(state_count, {});
                if constexpr (Trace) base_type::init(state_count, number);
                for (uint32_t c = 0; c != Base; c++) {
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
                    for (uint32_t c = 0; c != Base; c++) {
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
            Tp solve(IntegerString<Base> number, uint32_t state_count, Transfer &&transfer, ValueMapping &&mapping) {
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
                        if (this->m_from[len - 1][state].size() && mapping(state, len)) base_type::enumerate(state, len, call);
            }
        };
        using IntStr10 = IntegerString<10>;
        IntStr10 prev_number_base10(std::string s) { return prev_number<10>(IntStr10(std::move(s))); }
        IntStr10 next_number_base10(std::string s) { return next_number<10>(IntStr10(std::move(s))); }
    }
}

#endif
