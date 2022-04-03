#ifndef __OY_BITWISEHELPER__
#define __OY_BITWISEHELPER__

#include <cassert>
#include <cstdint>
#include <functional>
#include <string>

namespace OY {
    template <typename _Tp, typename _Increment>
    struct _BitLoop {
        _Tp m_start;
        _Tp m_end;
        mutable _Increment m_inc;
        constexpr _BitLoop(_Tp __start, _Tp __end, _Increment __inc) : m_start(__start), m_end(__end), m_inc(__inc) {}
        struct _BitIterator {
            mutable _Tp value;
            const _BitLoop<_Tp, _Increment> &loop;
            constexpr _BitIterator(_Tp start, const _BitLoop<_Tp, _Increment> &loop) : value(start), loop(loop) {}
            constexpr _BitIterator &operator++() {
                loop.m_inc(value);
                return *this;
            }
            constexpr _Tp operator*() { return value; }
            constexpr bool operator!=(const _BitIterator &other) const { return value != other.value; }
        };
        constexpr _BitIterator begin() const { return _BitIterator(m_start, *this); }
        constexpr _BitIterator end() const { return _BitIterator(m_end, *this); }
    };
    template <typename _Tp, typename _Increment>
    _BitLoop(_Tp, _Tp, _Increment) -> _BitLoop<_Tp, _Increment>;
    template <typename _Tp>
    struct BitwiseHelper {
        static constexpr uint8_t length() { return sizeof(_Tp) * 8; }
        static constexpr uint8_t countOne(_Tp mask) { return std::__popcount(mask); }
        static constexpr bool isOne(_Tp mask, uint8_t i) { return i < length() ? mask >> i & _Tp(1) : false; }
        static constexpr bool isZero(_Tp mask, uint8_t i) { return !isOne(mask, i); }
        static constexpr bool intersect(_Tp mask1, _Tp mask2) { return mask1 & mask2; }
        static constexpr bool contains(_Tp mask1, _Tp mask2) { return (mask1 & mask2) == mask2; }
        static constexpr bool isContained(_Tp mask1, _Tp mask2) { return (mask1 & mask2) == mask1; }
        static constexpr uint8_t countFrontZeros(_Tp mask) { return std::__countl_zero(mask); }
        static constexpr uint8_t countFrontOnes(_Tp mask) { return countFrontZeros(~mask); }
        static constexpr uint8_t countBackZeros(_Tp mask) { return std::__countr_zero(mask); }
        static constexpr uint8_t countBackOnes(_Tp mask) { return countBackZeros(~mask); }
        static constexpr _Tp makeMask() { return -1; }
        static constexpr _Tp makeMask(uint8_t i) { return i < length() ? _Tp(1) << i : _Tp(0); }
        static constexpr _Tp makeMask(uint8_t l, uint8_t r) { return makeBackOnes(r + 1) ^ makeBackOnes(l); }
        static constexpr _Tp makeBackOnes(uint8_t k) { return k >= length() ? _Tp(-1) : (_Tp(1) << k) - 1; }
        static constexpr _Tp getMask(_Tp mask, uint8_t i) { return mask & makeMask(i); }
        static constexpr _Tp getMask(_Tp mask, uint8_t l, uint8_t r) { return mask & makeMask(l, r); }
        static constexpr _Tp getLowestOne(_Tp mask) { return makeMask(countBackZeros(mask)); }
        static constexpr _Tp getCeil(_Tp mask) { return mask ? makeMask(length() - countFrontZeros(mask - 1)) : _Tp(0); }
        static constexpr _Tp getFloor(_Tp mask) { return makeMask(length() - 1 - countFrontZeros(mask)); }
        static constexpr _Tp getBackOnes(_Tp mask) { return makeBackOnes(countBackOnes(mask)); }
        static constexpr void setMask(_Tp &mask) { mask = makeMask(); }
        static constexpr void setMask(_Tp &mask, uint8_t i) { mask |= makeMask(i); }
        static constexpr void setMask(_Tp &mask, uint8_t l, uint8_t r) { mask |= makeMask(l, r); }
        static constexpr void resetMask(_Tp &mask) { mask = 0; }
        static constexpr void resetMask(_Tp &mask, uint8_t i) { mask &= ~makeMask(i); }
        static constexpr void resetMask(_Tp &mask, uint8_t l, uint8_t r) { mask &= ~makeMask(l, r); }
        static constexpr void flipMask(_Tp &mask) { mask = ~mask; }
        static constexpr void flipMask(_Tp &mask, uint8_t i) { mask ^= makeMask(i); }
        static constexpr void flipMask(_Tp &mask, uint8_t l, uint8_t r) { mask ^= makeMask(l, r); }
        static constexpr void fillBackZerosWithOnes(_Tp &mask) { mask |= getBackOnes(~mask); }
        static constexpr void fillBackOnesWithZeros(_Tp &mask) { mask ^= getBackOnes(mask); }
        static constexpr auto enumOne(_Tp mask) {
            return _BitLoop(countBackZeros(mask), countBackZeros(0), [=](uint8_t &k) mutable {
                resetMask(mask, k);
                k = countBackZeros(mask);
            });
        }
        static constexpr auto enumOne_reversed(_Tp mask) {
            return _BitLoop(uint8_t(length() - 1 - countFrontZeros(mask)), uint8_t(-1), [=](uint8_t &k) mutable {
                resetMask(mask, k);
                k = length() - 1 - countFrontZeros(mask);
            });
        }
        static constexpr auto enumChoose(uint8_t n) {
            assert(n < length());
            return _BitLoop(_Tp(0), makeMask(n), [](_Tp &mask) { mask++; });
        }
        static constexpr auto enumChoose_reversed(uint8_t n) {
            assert(n < length());
            return _BitLoop(makeBackOnes(n), _Tp(-1), [](_Tp &mask) { mask--; });
        }
        static constexpr auto enumChoose(uint8_t n, uint8_t k) {
            assert(n < length() && k <= n);
            return _BitLoop(makeBackOnes(k), makeMask(n) | makeBackOnes(k - 1), [k](_Tp &mask) {
                int a = countBackZeros(mask);
                fillBackZerosWithOnes(mask);
                mask += makeBackOnes(countBackOnes(mask) - a - 1) + 1;
            });
        }
        static constexpr auto enumChoose_reversed(uint8_t n, uint8_t k) {
            assert(n < length() && k <= n);
            return _BitLoop(makeMask(n - k, n - 1), _Tp(-1), [k](_Tp &mask) {
                if (uint8_t a = countBackOnes(mask); a < k) {
                    fillBackOnesWithZeros(mask);
                    int b = countBackZeros(mask);
                    flipMask(mask, b - a - 1, b);
                } else
                    mask = _Tp(-1);
            });
        }
        static constexpr auto enumSubOf(_Tp mask) {
            assert(mask != _Tp(-1));
            return _BitLoop(_Tp(0), _Tp(-1), [mask](_Tp &cur) {
                if (cur ^ mask)
                    cur ^= (_Tp(1) << countBackZeros(cur ^ mask) + 1) - 1 & mask;
                else
                    cur = -1;
            });
        }
        static constexpr auto enumSubOf_reversed(_Tp mask) {
            assert(mask != _Tp(-1));
            return _BitLoop(mask, _Tp(-1), [mask](_Tp &cur) { cur = cur ? (cur - 1) & mask : _Tp(-1); });
        }
        static constexpr auto enumSubOf(_Tp mask, uint8_t k) {
            assert(mask != _Tp(-1) && k <= countOne(mask));
            _Tp res[length() + 1]{0};
            for (int i = 0, j = 0; i < length(); i++)
                if (mask & makeMask(i)) res[++j] = mask & makeBackOnes(i + 1);
            return _BitLoop(res[k], _Tp(-1), [=](_Tp &cur) {
                if (_Tp back = makeBackOnes(countBackZeros(cur)) & mask; back < (mask ^ cur)) {
                    uint8_t a = countBackZeros(mask - cur - back);
                    _Tp mid = makeBackOnes(a) & cur;
                    cur += makeMask(a) + res[countOne(mid) - 1] - mid;
                } else
                    cur = -1;
            });
        }
        static constexpr auto enumSubOf_reversed(_Tp mask, uint8_t k) {
            assert(mask != _Tp(-1) && k <= countOne(mask));
            _Tp res[length() + 1]{0};
            for (int i = 0, j = 0; i < length(); i++)
                if (mask & makeMask(i)) res[++j] = mask & makeBackOnes(i + 1);
            return _BitLoop(mask - res[countOne(mask) - k], _Tp(-1), [=](_Tp &cur) {
                if (_Tp back = makeBackOnes(countBackZeros(mask ^ cur)) & mask; back < cur) {
                    cur ^= back ^ (makeBackOnes(countBackZeros(cur ^ back) + 1) & mask);
                    cur ^= res[countOne(cur) - k];
                } else
                    cur = -1;
            });
        }
        template <uint8_t k = length()>
        static std::string to_string(_Tp mask) {
            const uint8_t ki = k > 0 ? k : length();
            std::string res;
            res.reserve(ki);
            for (auto i = ki - 1; i >= 0; i--) res.push_back(isOne(mask, i) ? '1' : '0');
            return res;
        }
    };
}

#endif