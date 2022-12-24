#ifndef __OY_PRUFER__
#define __OY_PRUFER__

#include <algorithm>
#include <cstdint>

namespace OY {
    struct Prufer {
        uint32_t m_vertexNum;
        std::vector<uint32_t> m_deg, m_xor;
        Prufer(uint32_t __vertexNum) : m_vertexNum(__vertexNum), m_deg(__vertexNum), m_xor(__vertexNum) {}
        template <typename _Iterator>
        static std::vector<uint32_t> decode(_Iterator __first, _Iterator __last) {
            const uint32_t n = __last - __first + 2;
            std::vector<uint32_t> res(n), deg(n);
            for (auto it = __first; it != __last; ++it) deg[*it]++;
            auto it = __first;
            for (uint32_t i = 0; i < n; i++)
                if (!deg[i]) {
                    uint32_t cur = i;
                    do {
                        if (it == __last)
                            res[cur] = n - 1;
                        else
                            res[cur] = *it++;
                        cur = res[cur];
                    } while (!--deg[cur] && cur <= i);
                }
            res[n - 1] = -1;
            return res;
        }
        void addEdge(uint32_t __a, uint32_t __b) { m_deg[__a]++, m_deg[__b]++, m_xor[__a] ^= __b, m_xor[__b] ^= __a; }
        std::vector<uint32_t> encode() {
            std::vector<uint32_t> res;
            res.reserve(m_vertexNum - 1);
            for (uint32_t i = 0; i < m_vertexNum; i++)
                if (m_deg[i] == 1) {
                    uint32_t cur = i;
                    do m_xor[m_xor[cur]] ^= cur, res.push_back(cur = m_xor[cur]);
                    while (--m_deg[cur] == 1 && cur <= i);
                }
            res.pop_back();
            return res;
        }
    };
}

#endif