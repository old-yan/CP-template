#ifndef __OY_PRIMITIVEROOT__
#define __OY_PRIMITIVEROOT__

#include <cstdint>
#include <functional>
#include "Eratosthenes.h"
#include "PollardPho.h"

namespace OY {
    template <uint32_t _N>
    struct PrimitiveRootSieveSolver {
        static inline EratosthenesSieve<_N, false, false, true, false> es;
        static uint32_t pick(uint32_t P) { return es.querySmallestFactor(P); }
        static std::vector<uint32_t> getSub(uint32_t phi) {
            std::vector<uint32_t> res;
            for (uint32_t cur = phi; cur > 1;) {
                uint32_t prime = es.querySmallestFactor(cur);
                res.push_back(phi / prime);
                while (cur % prime == 0) cur /= prime;
            }
            return res;
        }
    };
    struct PrimitiveRootPollardPhoSolver {
        template <typename _ModType>
        static _ModType pick(_ModType P) {
            while (!isPrime<_ModType>(P)) P = Pollard_Rho::pick<_ModType>(P);
            return P;
        }
        template <typename _ModType>
        static std::vector<_ModType> getSub(_ModType phi) {
            std::vector<_ModType> res;
            auto comps = Pollard_Rho::decomposite<_ModType>(phi);
            res.reserve(comps.size());
            for (auto [p, c] : comps) res.push_back(phi / p);
            return res;
        }
    };
    template <typename _ModType, typename _Solver>
    struct PrimitiveRoot {
        static bool isPrimitiveRoot(_ModType a, _ModType phi, std::vector<_ModType> &sub, Barrett<_ModType> &brt) {
            if (brt.pow(a, phi) != 1) return false;
            for (auto v : sub)
                if (brt.pow(a, v) == 1) return false;
            return true;
        }
        static _ModType queryMin(_ModType P) {
            if (P <= 4) return P - 1;
            _ModType oddP = P & 1 ? P : P >> 1;
            if (!(oddP & 1)) return 0;
            _ModType prime = _Solver::pick(oddP);
            while (oddP % prime == 0) oddP /= prime;
            if (oddP > 1) return 0;
            _ModType phi = (P & 1 ? P : P >> 1) / prime * (prime - 1);
            std::vector<_ModType> sub = _Solver::getSub(phi);
            Barrett<_ModType> brt(P);
            uint32_t cur = 2;
            while (!isPrimitiveRoot(cur, phi, sub, brt)) cur++;
            return cur;
        }
    };
    template <uint32_t _N>
    using PrimitiveRoot32_Sieve = PrimitiveRoot<uint32_t, PrimitiveRootSieveSolver<_N>>;
    using PrimitiveRoot32_PollardPho = PrimitiveRoot<uint32_t, PrimitiveRootPollardPhoSolver>;
    using PrimitiveRoot64_PollardPho = PrimitiveRoot<uint64_t, PrimitiveRootPollardPhoSolver>;
}

#endif