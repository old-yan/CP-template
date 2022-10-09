#ifndef __OY_DISCRETIZER__
#define __OY_DISCRETIZER__

#include <algorithm>
#include <vector>

namespace OY {
    template <typename _Tp = int>
    struct Discretizer : std::vector<_Tp> {
        using std::vector<_Tp>::vector, std::vector<_Tp>::begin, std::vector<_Tp>::end, std::vector<_Tp>::insert, std::vector<_Tp>::push_back, std::vector<_Tp>::resize;
        Discretizer &operator<<(const _Tp &__item) {
            push_back(__item);
            return *this;
        }
        Discretizer &operator<<(const std::vector<_Tp> &__items) {
            insert(end(), __items.begin(), __items.end());
            return *this;
        }
        Discretizer &operator<<(const std::vector<std::vector<_Tp>> &__items) {
            for (auto &each : __items) *this << each;
            return *this;
        }
        void prepare() {
            std::sort(begin(), end());
            resize(std::unique(begin(), end()) - begin());
        }
        int rank(const _Tp &__item) {
            return lower_bound(__item);
        }
        int lower_bound(const _Tp &__item) {
            return std::lower_bound(begin(), end(), __item) - begin();
        }
        int upper_bound(const _Tp &__item) {
            return std::upper_bound(begin(), end(), __item) - begin();
        }
    };
}

#endif