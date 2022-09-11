#ifndef __OY_RANGE_MODULE__
#define __OY_RANGE_MODULE__

#include <algorithm>
#include <map>

namespace OY {
    template <typename _Tp>
    struct RangeManager : std::map<_Tp, _Tp> {
        _Tp m_length = 0;
        using std::map<_Tp, _Tp>::map, std::map<_Tp, _Tp>::upper_bound, std::map<_Tp, _Tp>::begin, std::map<_Tp, _Tp>::end, std::map<_Tp, _Tp>::emplace, std::map<_Tp, _Tp>::erase;
        static _Tp length(const std::pair<_Tp, _Tp> &__range) { return __range.second - __range.first + 1; }
        std::pair<std::vector<std::pair<_Tp, _Tp>>, typename std::map<_Tp, _Tp>::iterator> addRange(std::pair<_Tp, _Tp> __range) {
            std::vector<std::pair<_Tp, _Tp>> deleted;
            auto it = upper_bound(__range.first);
            if (it != begin() && std::prev(it)->second + 1 >= __range.first) {
                if (std::prev(it)->second >= __range.second) return {{*std::prev(it)}, std::prev(it)};
                __range.first = std::prev(it)->first;
                deleted.emplace_back(*std::prev(it));
                it = erase(std::prev(it));
            }
            for (; it != end() && it->first <= __range.second + 1; it = erase(it)) {
                if (it->second > __range.second) __range.second = it->second;
                deleted.emplace_back(*it);
            }
            for (auto &range : deleted) m_length -= length(range);
            auto inserted = emplace(__range.first, __range.second).first;
            m_length += length(*inserted);
            return {deleted, inserted};
        }
        std::pair<std::vector<std::pair<_Tp, _Tp>>, std::vector<std::pair<_Tp, _Tp>>> removeRange(std::pair<_Tp, _Tp> __range) {
            std::vector<std::pair<_Tp, _Tp>> deleted, inserted;
            auto it = upper_bound(__range.first);
            if (it != begin() && std::prev(it)->second + 1 >= __range.first) {
                it = std::prev(it);
                deleted.emplace_back(*it);
                if (it->second >= __range.second) {
                    if (it->second > __range.second) inserted.emplace_back(*emplace(__range.second + 1, it->second).first);
                    if (it->first < __range.first) {
                        it->second = __range.first - 1;
                        inserted.emplace_back(*it);
                    } else
                        erase(it);
                    for (auto &range : deleted) m_length -= length(range);
                    for (auto &range : inserted) m_length += length(range);
                    return {deleted, inserted};
                }
                if (it->first < __range.first) {
                    it->second = __range.first - 1;
                    inserted.emplace_back(*it);
                    it = std::next(it);
                } else
                    it = erase(it);
            }
            for (; it != end() && it->first <= __range.second; it = erase(it)) {
                deleted.emplace_back(*it);
                if (it->second > __range.second) inserted.emplace_back(*emplace(__range.second + 1, it->second).first);
            }
            for (auto &range : deleted) m_length -= length(range);
            for (auto &range : inserted) m_length += length(range);
            return {deleted, inserted};
        }
        typename std::map<_Tp, _Tp>::const_iterator any_of(const std::pair<_Tp, _Tp> &__range) const {
            if (auto it = upper_bound(__range.second); it != begin() && std::prev(it)->second >= __range.first) return std::prev(it);
            return end();
        }
        typename std::map<_Tp, _Tp>::const_iterator all_of(const std::pair<_Tp, _Tp> &__range) const {
            if (auto it = upper_bound(__range.second); it != begin() && std::prev(it)->first <= __range.first && std::prev(it)->second >= __range.second) return std::prev(it);
            return end();
        }
        template <typename _Ostream>
        friend _Ostream &operator<<(_Ostream &__os, const RangeManager &__rm) {
            __os << '[';
            uint32_t index = 0;
            for (auto &[l, r] : __rm) {
                if (index++) __os << ", ";
                __os << '[' << l << ", " << r << ']';
            }
            return __os << ']';
        }
    };
}

#endif