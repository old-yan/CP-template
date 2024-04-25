#ifndef __OY_SEQUENCEAUTOMATON__
#define __OY_SEQUENCEAUTOMATON__

#include <algorithm>
#include <cstdint>
#include <map>
#include <numeric>
#include <vector>

namespace OY {
    template <typename Container>
    struct SequenceAutomatonIndex {};
    template <>
    struct SequenceAutomatonIndex<std::vector<std::vector<uint32_t>>> {
        std::vector<std::vector<uint32_t>> m_indexes;
        void resize(uint32_t size) { m_indexes.resize(size); }
        void insert(uint32_t index, uint32_t value) { m_indexes[value].push_back(index); }
        uint32_t find_prev(uint32_t last_pos, uint32_t elem) const {
            if (elem >= m_indexes.size() || !~last_pos) return -1;
            auto &v = m_indexes[elem];
            auto it = std::lower_bound(v.begin(), v.end(), last_pos);
            return it == v.begin() ? -1 : *std::prev(it);
        }
        uint32_t find_next(uint32_t last_pos, uint32_t elem, uint32_t length) const {
            if (elem >= m_indexes.size()) return length;
            auto &v = m_indexes[elem];
            auto it = std::lower_bound(v.begin(), v.end(), last_pos + 1);
            return it == v.end() ? length : *it;
        }
    };
    template <>
    struct SequenceAutomatonIndex<std::map<uint32_t, std::vector<uint32_t>>> {
        std::map<uint32_t, std::vector<uint32_t>> m_indexes;
        void resize(uint32_t size) {}
        void insert(uint32_t index, uint32_t value) { m_indexes[value].push_back(index); }
        uint32_t find_prev(uint32_t last_pos, uint32_t elem) const {
            if (!~last_pos) return -1;
            auto itv = m_indexes.find(elem);
            if (itv == m_indexes.end()) return -1;
            auto &v = itv->second;
            auto it = std::lower_bound(v.begin(), v.end(), last_pos);
            return it == v.begin() ? -1 : *std::prev(it);
        }
        uint32_t find_next(uint32_t last_pos, uint32_t elem, uint32_t length) const {
            auto itv = m_indexes.find(elem);
            if (itv == m_indexes.end()) return length;
            auto &v = itv->second;
            auto it = std::lower_bound(v.begin(), v.end(), last_pos + 1);
            return it == v.end() ? length : *it;
        }
    };
    template <typename IndexContainer>
    struct SequenceAutomaton {
        uint32_t m_length;
        SequenceAutomatonIndex<IndexContainer> m_container;
        SequenceAutomaton() = default;
        template <typename InitMapping>
        SequenceAutomaton(uint32_t length, InitMapping mapping) { resize(length, mapping); }
        template <typename Iterator>
        SequenceAutomaton(Iterator first, Iterator last) { reset(first, last); }
        template <typename InitMapping>
        void resize(uint32_t length, InitMapping mapping) {
            m_length = length;
            uint32_t Max = 0;
            for (uint32_t i = 0; i != length; i++) Max = std::max<uint32_t>(Max, mapping(i));
            m_container.resize(Max + 1);
            for (uint32_t i = 0; i != length; i++) m_container.insert(i, mapping(i));
        }
        template <typename Iterator>
        void reset(Iterator first, Iterator last) {
            resize(last - first, [&](uint32_t i) { return *(first + i); });
        }
        uint32_t prev(uint32_t last_pos, uint32_t elem) const { return m_container.find_prev(last_pos, elem); }
        uint32_t next(uint32_t last_pos, uint32_t elem) const { return m_container.find_next(last_pos, elem, m_length); }
        template <typename Iterator>
        bool contains(Iterator first, Iterator last) const {
            uint32_t last_pos = -1, p = m_length - (last - first);
            for (auto it = first; it != last; ++it) {
                last_pos = next(last_pos, *it);
                if (last_pos > p++) return false;
            }
            return true;
        }
    };
    using SequenceAutomaton_vector = SequenceAutomaton<std::vector<std::vector<uint32_t>>>;
    using SequenceAutomaton_map = SequenceAutomaton<std::map<uint32_t, std::vector<uint32_t>>>;
}

#endif