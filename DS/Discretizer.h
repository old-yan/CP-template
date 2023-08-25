/*
最后修改:
20230825
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_DISCRETIZER__
#define __OY_DISCRETIZER__

#include <algorithm>
#include <array>
#include <numeric>
#include <tuple>
#include <vector>

namespace OY {
    template <typename Tp>
    struct Discretizer : std::vector<Tp> {
        using std::vector<Tp>::vector;
        Discretizer &operator<<(const Tp &item) {
            std::vector<Tp>::push_back(item);
            return *this;
        }
        Discretizer &operator<<(const std::vector<Tp> &items) {
            std::vector<Tp>::insert(std::end(*this), items.begin(), items.end());
            return *this;
        }
        Discretizer &operator<<(const std::vector<std::vector<Tp>> &items) {
            for (auto &each : items) *this << each;
            return *this;
        }
        Discretizer &operator<<(const std::pair<Tp, Tp> &items) { return *this << items.first << items.second; }
        template <size_t N>
        Discretizer &operator<<(const std::array<Tp, N> &items) {
            std::vector<Tp>::insert(std::end(*this), items.begin(), items.end());
            return *this;
        }
        void prepare() {
            std::sort(std::begin(*this), std::end(*this));
            std::vector<Tp>::resize(std::unique(std::begin(*this), std::end(*this)) - std::begin(*this));
        }
        typename std::vector<Tp>::size_type rank(const Tp &item) { return std::distance(std::begin(*this), std::lower_bound(std::begin(*this), std::end(*this), item)); }
    };
}

#endif