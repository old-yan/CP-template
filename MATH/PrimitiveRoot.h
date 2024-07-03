/*
最后修改:
20240608
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PRIMITIVEROOT__
#define __OY_PRIMITIVEROOT__

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace OY {
    namespace PROOT {
        template <typename Tp, typename Container = std::vector<typename Tp::mod_type>>
        struct Solver {
            using mod_type = typename Tp::mod_type;
            mod_type m_phi;
            Container m_phi_rest;
            bool is_primitive_root(Tp a) const {
                const auto one = Tp::raw(1);
                if (a.pow(m_phi) != one) return false;
                for (auto x : m_phi_rest)
                    if (a.pow(x) == one) return false;
                return true;
            }
            Tp query_random() const {
                if (Tp::mod() <= 4) return Tp::mod() - 1;
                std::mt19937_64 rng(std::random_device{}());
                std::uniform_int_distribution<typename Tp::mod_type> uid(2, Tp::mod() - 1);
                Tp cur;
                while (!is_primitive_root(cur = uid(rng))) {}
                return cur;
            }
            Tp query_min() const {
                if (Tp::mod() <= 4) return Tp::mod() - 1;
                auto cur = Tp::raw(2);
                while (!is_primitive_root(cur)) ++cur;
                return cur;
            }
            template <typename Callback>
            void enumerate(Callback &&call) {
                auto min = query_min();
                std::vector<bool> cop(this->m_phi + 1);
                for (auto p : this->m_phi_rest) {
                    auto q = this->m_phi / p;
                    if (q != 2) {
                        for (auto x = q; x <= this->m_phi; x += q * 2) cop[x] = true;
                    }
                }
                auto cur = min, min2 = min * min;
                for (uint32_t i = 1; i <= this->m_phi; i += 2, cur *= min2)
                    if (!cop[i]) call(cur);
            }
        };
        template <typename Tp, typename PollardRho>
        struct PollardRhoSolver : Solver<Tp, std::vector<typename Tp::mod_type>> {
            static bool has_primitive_root() {
                if (Tp::mod() <= 4) return true;
                auto P = Tp::mod();
                if (!(P & 1)) P >>= 1;
                if (!(P & 1)) return false;
                auto p = P;
                while (!PollardRho::is_prime(p)) p = PollardRho::pick(p);
                while (P % p == 0) P /= p;
                return P == 1;
            }
            typename Tp::mod_type count() const { return PollardRho::get_Euler_Phi(this->m_phi); }
            PollardRhoSolver() {
                this->m_phi = Tp::mod();
                for (auto &&pair : PollardRho::decomposite(Tp::mod())) this->m_phi = this->m_phi / pair.m_prime * (pair.m_prime - 1);
                for (auto &&pair : PollardRho::template decomposite<true>(this->m_phi)) this->m_phi_rest.push_back(this->m_phi / pair.m_prime);
            }
        };
        template <typename Tp, typename Sieve>
        struct SieveSolver : Solver<Tp, std::vector<typename Tp::mod_type>> {
            Sieve m_sieve;
            bool has_primitive_root() const {
                if (Tp::mod() <= 4) return true;
                auto P = Tp::mod();
                if (!(P & 1)) P >>= 1;
                if (!(P & 1)) return false;
                auto p = m_sieve.query_smallest_factor(P);
                while (P % p == 0) P /= p;
                return P == 1;
            }
            typename Tp::mod_type count() const { return m_sieve.get_Euler_Phi(this->m_phi); }
            SieveSolver(Sieve &sieve) : m_sieve(sieve) {
                this->m_phi = Tp::mod();
                for (auto &&pair : m_sieve.decomposite(Tp::mod())) this->m_phi = this->m_phi / pair.m_prime * (pair.m_prime - 1);
                for (auto &&pair : m_sieve.decomposite(this->m_phi)) this->m_phi_rest.push_back(this->m_phi / pair.m_prime);
                std::sort(this->m_phi_rest.begin(), this->m_phi_rest.end());
            }
        };
    }
}

#endif