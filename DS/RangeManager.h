/*
最后修改:
20230829
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_RANGE_MODULE__
#define __OY_RANGE_MODULE__

#include <algorithm>
#include <map>
#include <vector>

#include "TEST/mystd.h"

namespace OY {
    template <typename Tp>
    struct RangeManager {
        using iterator = typename std::map<Tp, Tp>::iterator;
        using const_iterator = typename std::map<Tp, Tp>::const_iterator;
        Tp m_length = 0;
        std::map<Tp, Tp> m_segs;
        static Tp length_of(const std::pair<Tp, Tp> &range) { return range.second - range.first + 1; }
        std::pair<std::vector<std::pair<Tp, Tp>>, const_iterator> add_range(std::pair<Tp, Tp> range) {
            std::vector<std::pair<Tp, Tp>> deleted;
            const_iterator it = upper_bound(range.first);
            if (it != begin()) {
                if ((--it)->second + 1 >= range.first) {
                    if (it->second >= range.second) return {{*it}, it};
                    range.first = it->first;
                    deleted.emplace_back(*it);
                    it = m_segs.erase(it);
                } else
                    ++it;
            }
            for (; it != end() && it->first <= range.second + 1; it = m_segs.erase(it)) {
                if (it->second > range.second) range.second = it->second;
                deleted.emplace_back(*it);
            }
            for (auto &range : deleted) m_length -= length_of(range);
            auto inserted = m_segs.emplace(range.first, range.second).first;
            m_length += length_of(*inserted);
            return {deleted, inserted};
        }
        std::pair<std::vector<std::pair<Tp, Tp>>, std::vector<std::pair<Tp, Tp>>> remove_range(std::pair<Tp, Tp> range) {
            std::vector<std::pair<Tp, Tp>> deleted, inserted;
            iterator it = m_segs.upper_bound(range.first);
            if (it != begin()) {
                if ((--it)->second + 1 >= range.first) {
                    deleted.emplace_back(*it);
                    if (it->second >= range.second) {
                        if (it->second > range.second) inserted.emplace_back(*m_segs.emplace(range.second + 1, it->second).first);
                        if (it->first < range.first) {
                            it->second = range.first - 1;
                            inserted.emplace_back(*it);
                        } else
                            m_segs.erase(it);
                        for (auto &range : deleted) m_length -= length_of(range);
                        for (auto &range : inserted) m_length += length_of(range);
                        return {deleted, inserted};
                    }
                    if (it->first < range.first) {
                        it->second = range.first - 1;
                        inserted.emplace_back(*it);
                        it = std::next(it);
                    } else
                        it = m_segs.erase(it);
                } else
                    ++it;
            }
            for (; it != end() && it->first <= range.second; it = m_segs.erase(it)) {
                deleted.emplace_back(*it);
                if (it->second > range.second) inserted.emplace_back(*m_segs.emplace(range.second + 1, it->second).first);
            }
            for (auto &range : deleted) m_length -= length_of(range);
            for (auto &range : inserted) m_length += length_of(range);
            return {deleted, inserted};
        }
        const_iterator any_of(const std::pair<Tp, Tp> &range) const {
            const_iterator it = upper_bound(range.second);
            if (it == begin()) return end();
            if ((--it)->second < range.first) return end();
            return it;
        }
        const_iterator all_of(const std::pair<Tp, Tp> &range) const {
            const_iterator it = upper_bound(range.second);
            if (it == begin()) return end();
            if ((--it)->first > range.first || it->second < range.second) return end();
            return it;
        }
        Tp length() const { return m_length; }
        void clear() { m_segs.clear(); }
        auto size() const -> decltype(m_segs.size()) { return m_segs.size(); }
        auto begin() const -> decltype(m_segs.begin()) { return m_segs.begin(); }
        auto end() const -> decltype(m_segs.end()) { return m_segs.end(); }
        auto lower_bound(const Tp &key) const -> decltype(m_segs.lower_bound(key)) { return m_segs.lower_bound(key); }
        auto upper_bound(const Tp &key) const -> decltype(m_segs.upper_bound(key)) { return m_segs.upper_bound(key); }
    };
    template <typename Ostream, typename Tp>
    Ostream &operator<<(Ostream &out, const RangeManager<Tp> &x) {
        out << '[';
        for (auto it = x.begin(); it != x.end(); ++it) {
            if (it != x.begin()) out << ", ";
            out << "[" << it->first << ", " << it->second << "]";
        }
        return out << ']';
    }
}

#endif