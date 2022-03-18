// #include <algorithm>
// #include <array>
// #include <bit>
// #include <bitset>
// #include <cassert>
// #include <climits>
// #include <cmath>
// #include <functional>
// #include <iostream>
// #include <limits>
// #include <list>
// #include <map>
// #include <memory>
// #include <numeric>
// #include <queue>
// #include <random>
// #include <set>
// #include <sstream>
// #include <stack>
// #include <string.h>
// #include <type_traits>
// #include <unordered_map>
// #include <unordered_set>
// #if defined(_WIN32)
// #include <windows.h>
// // split to make windows.h before psapi.h
// #include <psapi.h>
// #else
// #include "sys/time.h"
// #endif
// namespace OY {
// #define cin OY::inputHelper<1 << 10, 20>::getInstance()
// #define getchar() ({char c=cin.getChar_Checked();cin.next();c; })
// #define cout OY::outputHelper<1 << 20>::getInstance()
// #define putchar cout.putChar
// #define endl '\n'
// #define putlog(...) OY::printLog(", ", __VA_ARGS__)
//     template <uint64_t _BufferSize = 1 << 10, uint64_t _BlockSize = 20>
//     class inputHelper {
//         FILE *m_filePtr;
//         char m_buf[_BufferSize], *m_end, *m_cursor;
//         bool m_ok;
//         void flush() {
//             uint64_t a = m_end - m_cursor;
//             if (a >= _BlockSize) return;
//             memmove(m_buf, m_cursor, a);
//             uint64_t b = fread(m_buf + a, 1, _BufferSize - a, m_filePtr);
//             m_cursor = m_buf;
//             if (a + b < _BufferSize) {
//                 m_end = m_buf + a + b;
//                 *m_end = EOF;
//             }
//         }

//     public:
//         explicit inputHelper(const char *inputFileName) : m_ok(true) {
//             if (!*inputFileName)
//                 m_filePtr = stdin;
//             else
//                 m_filePtr = fopen(inputFileName, "rt");
//             m_end = m_cursor = m_buf + _BufferSize;
//         }
//         ~inputHelper() { fclose(m_filePtr); }
//         static inputHelper<_BufferSize, _BlockSize> &getInstance() {
// #ifdef OY_LOCAL
//             static inputHelper<_BufferSize, _BlockSize> s_obj("in.txt");
// #else
//             static inputHelper<_BufferSize, _BlockSize> s_obj("");
// #endif
//             return s_obj;
//         }
//         static constexpr bool isBlank(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
//         static constexpr bool isEndline(char c) { return c == '\n' || c == EOF; }
//         const char &getChar_Checked() {
//             if (m_cursor < m_end) return *m_cursor;
//             uint64_t b = fread(m_buf, 1, _BufferSize, m_filePtr);
//             m_cursor = m_buf;
//             if (b < _BufferSize) {
//                 m_end = m_buf + b;
//                 *m_end = EOF;
//             }
//             return *m_cursor;
//         }
//         const char &getChar_Unchecked() const { return *m_cursor; }
//         void next() { ++m_cursor; }
//         void setState(bool _ok) { m_ok = _ok; }
//         template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp> & std::is_integral_v<_Tp>> * = nullptr>
//         inputHelper<_BufferSize, _BlockSize> &operator>>(_Tp &ret) {
//             while (isBlank(getChar_Checked())) next();
//             flush();
//             if (getChar_Unchecked() == '-') {
//                 next();
//                 if (isdigit(getChar_Unchecked())) {
//                     ret = -(getChar_Unchecked() - '0');
//                     while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 - (getChar_Unchecked() - '0');
//                 } else
//                     m_ok = false;
//             } else {
//                 if (isdigit(getChar_Unchecked())) {
//                     ret = getChar_Unchecked() - '0';
//                     while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 + (getChar_Unchecked() - '0');
//                 } else
//                     m_ok = false;
//             }
//             return *this;
//         }
//         template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp> & std::is_integral_v<_Tp>> * = nullptr>
//         inputHelper<_BufferSize, _BlockSize> &operator>>(_Tp &ret) {
//             while (isBlank(getChar_Checked())) next();
//             flush();
//             if (isdigit(getChar_Unchecked())) {
//                 ret = getChar_Unchecked() - '0';
//                 while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 + (getChar_Unchecked() - '0');
//             } else
//                 m_ok = false;
//             return *this;
//         }
//         template <typename _Tp, std::enable_if_t<std::is_floating_point_v<_Tp>> * = nullptr>
//         inputHelper<_BufferSize, _BlockSize> &operator>>(_Tp &ret) {
//             bool neg = false, integer = false, decimal = false;
//             while (isBlank(getChar_Checked())) next();
//             flush();
//             if (getChar_Unchecked() == '-') {
//                 neg = true;
//                 next();
//             }
//             if (!isdigit(getChar_Unchecked()) && getChar_Unchecked() != '.') {
//                 m_ok = false;
//                 return *this;
//             }
//             if (isdigit(getChar_Unchecked())) {
//                 integer = true;
//                 ret = getChar_Unchecked() - '0';
//                 while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 + (getChar_Unchecked() - '0');
//             }
//             if (getChar_Unchecked() == '.') {
//                 next();
//                 if (isdigit(getChar_Unchecked())) {
//                     if (!integer) ret = 0;
//                     decimal = true;
//                     _Tp unit = 0.1;
//                     ret += unit * (getChar_Unchecked() - '0');
//                     while (next(), isdigit(getChar_Unchecked())) {
//                         unit *= 0.1;
//                         ret += unit * (getChar_Unchecked() - '0');
//                     }
//                 }
//             }
//             if (!integer && !decimal)
//                 m_ok = false;
//             else if (neg)
//                 ret = -ret;
//             return *this;
//         }
//         inputHelper<_BufferSize, _BlockSize> &operator>>(char &ret) {
//             while (isBlank(getChar_Checked())) next();
//             ret = getChar_Checked();
//             next();
//             return *this;
//         }
//         inputHelper<_BufferSize, _BlockSize> &operator>>(std::string &ret) {
//             while (isBlank(getChar_Checked())) next();
//             if (getChar_Checked() != EOF) {
//                 ret.clear();
//                 do {
//                     ret += getChar_Checked();
//                     next();
//                 } while (!isBlank(getChar_Checked()) && getChar_Unchecked() != EOF);
//             } else
//                 m_ok = false;
//             return *this;
//         }
//         explicit operator bool() { return m_ok; }
//     };
//     template <uint64_t _BufferSize = 1 << 20>
//     class outputHelper {
//         FILE *m_filePtr = nullptr;
//         char m_buf[_BufferSize], *m_end, *m_cursor;
//         char m_tempBuf[50], *m_tempBufCursor, *m_tempBufDot;
//         uint64_t m_floatReserve, m_floatRatio;

//     public:
//         outputHelper(const char *outputFileName, int prec = 6) : m_end(m_buf + _BufferSize) {
//             if (!*outputFileName)
//                 m_filePtr = stdout;
//             else
//                 m_filePtr = fopen(outputFileName, "wt");
//             m_cursor = m_buf;
//             m_tempBufCursor = m_tempBuf;
//             precision(prec);
//         }
//         static outputHelper<_BufferSize> &getInstance() {
// #ifdef OY_LOCAL
//             static outputHelper<_BufferSize> s_obj("out.txt");
// #else
//             static outputHelper<_BufferSize> s_obj("");
// #endif
//             return s_obj;
//         }
//         ~outputHelper() {
//             flush();
//             fclose(m_filePtr);
//         }
//         void precision(int prec) {
//             m_floatReserve = prec;
//             m_floatRatio = pow(10, prec);
//             m_tempBufDot = m_tempBuf + prec;
//         }
//         outputHelper<_BufferSize> &flush() {
//             fwrite(m_buf, 1, m_cursor - m_buf, m_filePtr);
//             fflush(m_filePtr);
//             m_cursor = m_buf;
//             return *this;
//         }
//         void putChar(const char &c) {
//             if (m_cursor == m_end) flush();
//             *m_cursor++ = c;
//         }
//         void putS(const char *c) {
//             while (*c) putChar(*c++);
//         }
//         template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp> & std::is_integral_v<_Tp>> * = nullptr>
//         outputHelper<_BufferSize> &operator<<(const _Tp &ret) {
//             _Tp _ret = _Tp(ret);
//             if (_ret >= 0) {
//                 do {
//                     *m_tempBufCursor++ = '0' + _ret % 10;
//                     _ret /= 10;
//                 } while (_ret);
//                 do putChar(*--m_tempBufCursor);
//                 while (m_tempBufCursor > m_tempBuf);
//             } else {
//                 putChar('-');
//                 do {
//                     *m_tempBufCursor++ = '0' - _ret % 10;
//                     _ret /= 10;
//                 } while (_ret);
//                 do putChar(*--m_tempBufCursor);
//                 while (m_tempBufCursor > m_tempBuf);
//             }
//             return *this;
//         }
//         template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp> & std::is_integral_v<_Tp>> * = nullptr>
//         outputHelper<_BufferSize> &operator<<(const _Tp &ret) {
//             _Tp _ret = _Tp(ret);
//             do {
//                 *m_tempBufCursor++ = '0' + _ret % 10;
//                 _ret /= 10;
//             } while (_ret);
//             do putChar(*--m_tempBufCursor);
//             while (m_tempBufCursor > m_tempBuf);
//             return *this;
//         }
//         template <typename _Tp, std::enable_if_t<std::is_floating_point_v<_Tp>> * = nullptr>
//         outputHelper<_BufferSize> &operator<<(const _Tp &ret) {
//             if (ret < 0) {
//                 putChar('-');
//                 return *this << -ret;
//             }
//             _Tp _ret = ret * m_floatRatio;
//             uint64_t integer = _ret;
//             if (_ret - integer >= 0.4999999999) integer++;
//             do {
//                 *m_tempBufCursor++ = '0' + integer % 10;
//                 integer /= 10;
//             } while (integer);
//             if (m_tempBufCursor > m_tempBufDot) {
//                 do putChar(*--m_tempBufCursor);
//                 while (m_tempBufCursor > m_tempBufDot);
//                 putChar('.');
//             } else {
//                 putS("0.");
//                 for (int i = m_tempBufDot - m_tempBufCursor; i--;) putChar('0');
//             }
//             do putChar(*--m_tempBufCursor);
//             while (m_tempBufCursor > m_tempBuf);
//             return *this;
//         }
//         outputHelper<_BufferSize> &operator<<(const char &ret) {
//             putChar(ret);
//             return *this;
//         }
//         outputHelper<_BufferSize> &operator<<(const std::string &ret) {
//             putS(ret.data());
//             return *this;
//         }
//     };
//     template <uint64_t _BufferSize, uint64_t _BlockSize>
//     inputHelper<_BufferSize, _BlockSize> &getline(inputHelper<_BufferSize, _BlockSize> &ih, std::string &ret) {
//         ret.clear();
//         if (ih.getChar_Checked() == EOF)
//             ih.setState(false);
//         else {
//             while (!inputHelper<_BufferSize, _BlockSize>::isEndline(ih.getChar_Checked())) {
//                 ret += ih.getChar_Unchecked();
//                 ih.next();
//             }
//             ih.next();
//         }
//         return ih;
//     }
//     template <typename D, typename T, typename... S>
//     void printLog(D delim, const T &x, S... rest) {
//         cout << x;
//         if constexpr (sizeof...(rest) > 0) {
//             cout << delim;
//             printLog(delim, rest...);
//         }
//     }
// }
// using OY::getline;
// namespace OY {
//     template <typename _ModType>
//     struct Barrett {
//         _ModType m_P;
//         __uint128_t m_Pinv;
//         constexpr Barrett() = default;
//         constexpr explicit Barrett(_ModType __P) : m_P(__P), m_Pinv(-uint64_t(__P) / __P + 1) {}
//         constexpr _ModType mod() const { return m_P; }
//         constexpr _ModType mod(uint64_t __a) const {
//             __a -= uint64_t(m_Pinv * __a >> 64) * m_P + m_P;
//             if (__a >= m_P) __a += m_P;
//             return __a;
//         }
//         constexpr _ModType multiply(uint64_t __a, uint64_t __b) const {
//             if constexpr (std::is_same_v<_ModType, uint64_t>)
//                 return multiply_ld(__a, __b);
//             else
//                 return multiply_64(__a, __b);
//         }
//         constexpr _ModType multiply_64(uint64_t __a, uint64_t __b) const {
//             // assert(__a * __b < 1ull << 64);
//             return mod(__a * __b);
//         }
//         constexpr _ModType multiply_128(uint64_t __a, uint64_t __b) const {
//             if (__builtin_clzll(__a) + __builtin_clzll(__b) >= 64) return multiply_64(__a, __b);
//             return __uint128_t(__a) * __b % m_P;
//         }
//         constexpr _ModType multiply_ld(uint64_t __a, uint64_t __b) const {
//             // assert(m_P < 1ull << 63 && __a < m_P && __b < m_P);
//             if (__builtin_clzll(__a) + __builtin_clzll(__b) >= 64) return multiply_64(__a, __b);
//             int64_t res = __a * __b - uint64_t(1.L / m_P * __a * __b) * m_P;
//             if (res < 0)
//                 res += m_P;
//             else if (res >= m_P)
//                 res -= m_P;
//             return res;
//         }
//         constexpr _ModType pow(uint64_t __a, uint64_t __n) const {
//             if constexpr (std::is_same_v<_ModType, uint64_t>)
//                 return pow_ld(__a, __n);
//             else
//                 return pow_64(__a, __n);
//         }
//         constexpr _ModType pow_64(uint64_t __a, uint64_t __n) const {
//             // assert(m_P < 1ull << 32);
//             _ModType res = 1, b = mod(__a);
//             while (__n) {
//                 if (__n & 1) res = multiply_64(res, b);
//                 b = multiply_64(b, b);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         constexpr _ModType pow_128(uint64_t __a, uint64_t __n) const {
//             _ModType res = 1, b = mod(__a);
//             while (__n) {
//                 if (__n & 1) res = multiply_128(res, b);
//                 b = multiply_128(b, b);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         constexpr _ModType pow_ld(uint64_t __a, uint64_t __n) const {
//             _ModType res = 1, b = mod(__a);
//             while (__n) {
//                 if (__n & 1) res = multiply_ld(res, b);
//                 b = multiply_ld(b, b);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         template <typename _Tp>
//         constexpr _Tp divide(_Tp __a) const {
//             if (__a < m_P) return 0;
//             _Tp res = m_Pinv * __a >> 64;
//             if (__a - res * m_P >= m_P) res++;
//             return res;
//         }
//         template <typename _Tp>
//         constexpr std::pair<_Tp, _Tp> divmod(_Tp __a) const {
//             _Tp quo = (__a * m_Pinv) >> 64, rem = __a - quo * m_P;
//             if (rem >= m_P) {
//                 quo++;
//                 rem -= m_P;
//             }
//             return {quo, rem};
//         }
//     };
//     using Barrett32 = Barrett<uint32_t>;
//     using Barrett64 = Barrett<uint64_t>;
// }
// namespace OY {
//     template <typename _ModType>
//     struct _MontgomeryTag;
//     template <>
//     struct _MontgomeryTag<uint32_t> {
//         using long_type = uint64_t;
//         static constexpr uint32_t limit = (1u << 30) - 1;
//         static constexpr uint32_t inv_loop = 4;
//         static constexpr uint32_t length = 32;
//     };
//     template <>
//     struct _MontgomeryTag<uint64_t> {
//         using long_type = __uint128_t;
//         static constexpr uint64_t limit = (1ull << 63) - 1;
//         static constexpr uint32_t inv_loop = 5;
//         static constexpr uint32_t length = 64;
//     };
//     template <typename _ModType>
//     struct Montgomery {
//         using _FastType = _ModType;
//         using _LongType = typename _MontgomeryTag<_ModType>::long_type;
//         _ModType m_P;
//         _ModType m_Pinv;
//         _ModType m_Ninv;
//         Barrett<_ModType> m_brt;
//         constexpr Montgomery() = default;
//         constexpr explicit Montgomery(_ModType __P) : m_P(__P), m_Pinv(__P), m_Ninv(-_LongType(__P) % __P), m_brt(__P) {
//             for (int i = 0; i < _MontgomeryTag<_ModType>::inv_loop; i++) m_Pinv *= _ModType(2) - __P * m_Pinv;
//         }
//         constexpr _ModType mod() const { return m_brt.mod(); }
//         constexpr _ModType mod(uint64_t __a) const { return m_brt.mod(__a); }
//         constexpr _FastType init(uint64_t __a) const { return reduce(_LongType(mod(__a)) * m_Ninv); }
//         constexpr _FastType raw_init(uint64_t __a) const { return reduce(_LongType(__a) * m_Ninv); }
//         constexpr _FastType reduce(_LongType __a) const {
//             _FastType res = (__a >> _MontgomeryTag<_ModType>::length) - _ModType(_LongType(_ModType(__a) * m_Pinv) * m_P >> _MontgomeryTag<_ModType>::length);
//             if (res >= mod()) res += mod();
//             return res;
//         }
//         constexpr _ModType reduce(_FastType __a) const {
//             _ModType res = -_ModType(_LongType(__a * m_Pinv) * m_P >> _MontgomeryTag<_ModType>::length);
//             if (res >= mod()) res += mod();
//             return res;
//         }
//         constexpr _FastType multiply(_FastType __a, _FastType __b) const { return reduce(_LongType(__a) * __b); }
//         constexpr _FastType pow(_FastType __a, uint64_t __n) const {
//             _FastType res = reduce(_LongType(1) * m_Ninv);
//             while (__n) {
//                 if (__n & 1) res = multiply(res, __a);
//                 __a = multiply(__a, __a);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         template <typename _Tp>
//         constexpr _Tp divide(_Tp __a) const { return m_brt.divide(__a); }
//         template <typename _Tp>
//         constexpr std::pair<_Tp, _Tp> divmod(_Tp __a) const { return m_brt.divmod(__a); }
//     };
//     using Montgomery32 = Montgomery<uint32_t>;
//     using Montgomery64 = Montgomery<uint64_t>;
// }
// namespace OY {
//     template <typename _ModType, _ModType _P>
//     struct Modular {
//         static constexpr _ModType mod() { return _P; }
//         static constexpr _ModType mod(uint64_t __a) { return __a % _P; }
//         static constexpr _ModType multiply(uint64_t __a, uint64_t __b) {
//             if constexpr (std::is_same_v<_ModType, uint64_t>)
//                 return multiply_ld(__a, __b);
//             else
//                 return multiply_64(__a, __b);
//         }
//         static constexpr _ModType multiply_64(uint64_t __a, uint64_t __b) {
//             // assert(__a * __b < 1ull << 64);
//             return mod(__a * __b);
//         }
//         static constexpr _ModType multiply_128(uint64_t __a, uint64_t __b) { return __uint128_t(__a) * __b % _P; }
//         static constexpr _ModType multiply_ld(uint64_t __a, uint64_t __b) {
//             // assert(m_P < 1ull << 63 && __a < m_P && __b < m_P);
//             if (__builtin_clzll(__a) + __builtin_clzll(__b) >= 64) return multiply_64(__a, __b);
//             int64_t res = __a * __b - uint64_t(1.L / _P * __a * __b) * _P;
//             if (res < 0)
//                 res += _P;
//             else if (res >= _P)
//                 res -= _P;
//             return res;
//         }
//         static constexpr _ModType pow(uint64_t __a, uint64_t __n) {
//             if constexpr (std::is_same_v<_ModType, uint64_t>)
//                 return pow_ld(__a, __n);
//             else
//                 return pow_64(__a, __n);
//         }
//         static constexpr _ModType pow_64(uint64_t __a, uint64_t __n) {
//             // assert(m_P < 1ull << 32);
//             _ModType res = 1, b = mod(__a);
//             while (__n) {
//                 if (__n & 1) res = multiply_64(res, b);
//                 b = multiply_64(b, b);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         static constexpr _ModType pow_128(uint64_t __a, uint64_t __n) {
//             _ModType res = 1, b = mod(__a);
//             while (__n) {
//                 if (__n & 1) res = multiply_128(res, b);
//                 b = multiply_128(b, b);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         static constexpr _ModType pow_ld(uint64_t __a, uint64_t __n) {
//             _ModType res = 1, b = mod(__a);
//             while (__n) {
//                 if (__n & 1) res = multiply_ld(res, b);
//                 b = multiply_ld(b, b);
//                 __n >>= 1;
//             }
//             return res;
//         }
//         template <typename _Tp>
//         static constexpr _Tp divide(_Tp __a) { return __a / _P; }
//         template <typename _Tp>
//         static constexpr std::pair<_Tp, _Tp> divmod(_Tp __a) {
//             _Tp quo = __a / _P, rem = __a - quo * _P;
//             return {quo, rem};
//         }
//     };
//     template <uint32_t _P>
//     using Modular32 = Modular<uint32_t, _P>;
//     template <uint64_t _P>
//     using Modular64 = Modular<uint64_t, _P>;
// }
// namespace OY {
//     template <uint32_t _P>
//     struct StaticLucasTable {
//         // static_assert(isPrime(_P));
//         static inline uint32_t s_factorial[_P];
//         static inline uint32_t s_factorialInverse[_P];
//         static inline bool s_inited = [] {
//             s_factorial[0] = 1;
//             for (uint32_t i = 1; i < _P; i++) s_factorial[i] = Modular32<_P>::multiply_64(s_factorial[i - 1], i);
//             s_factorialInverse[_P - 1] = Modular32<_P>::pow_64(s_factorial[_P - 1], _P - 2);
//             for (uint32_t i = _P - 1; i; i--) s_factorialInverse[i - 1] = Modular32<_P>::multiply_64(s_factorialInverse[i], i);
//         }();
//         static uint32_t rawQuery(uint32_t __n, uint32_t __m) {
//             // assert(__n < _P && __m < _P);
//             if (!__m || __n == __m) return 1;
//             if (__n < __m) return 0;
//             return Modular32<_P>::multiply_64(s_factorial[__n], Modular32<_P>::multiply_64(s_factorialInverse[__m], s_factorialInverse[__n - __m]));
//         }
//         static uint32_t query(uint64_t __n, uint64_t __m) {
//             if (__n < __m) return 0;
//             uint32_t res = 1;
//             while (__m && __n != __m) {
//                 uint64_t n = __n / _P, m = __m / _P;
//                 res = Modular32<_P>::multiply_64(res, rawQuery(__n - n * _P, __m - m * _P));
//                 __n = n;
//                 __m = m;
//             }
//             return res;
//         }
//     };
// }

// int main() {
// #ifdef OY_LOCAL
// #if defined(_WIN32)
//     struct TIME_AND_MEMORY {
//         static auto GetMicroSecond() {
//             static FILETIME ft;
//             GetSystemTimeAsFileTime(&ft);
//             return ft.dwLowDateTime;
//         };
//         static auto GetMemory() {
//             PROCESS_MEMORY_COUNTERS pmc;
//             GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
//             return pmc.WorkingSetSize;
//         }
//         uint32_t t0, t1;
//         TIME_AND_MEMORY() : t0(GetMicroSecond()) {}
//         ~TIME_AND_MEMORY() {
//             t1 = GetMicroSecond();
//             cout << "\ntime cost = " << (t1 - t0) / 10000.0 << " ms\nmemory cost = " << (GetMemory() >> 20) << " MB\n ";
//         }
//     } tam;
// #else
//     struct TIME_AND_MEMORY {
//         static auto GetMicroSecond() {
//             static timeval ft;
//             gettimeofday(&ft, nullptr);
//             return ft.tv_sec * 1000000 + ft.tv_usec;
//         };
//         static auto GetMemory() {
//             char _mem_buff[512] = {0};
//             int used, now_use;
//             fgets(_mem_buff, sizeof(_mem_buff), fopen("/proc/self/statm", "r"));
//             sscanf(_mem_buff, "%d %d", &used, &now_use);
//             return now_use;
//         }
//         uint64_t t0, t1;
//         TIME_AND_MEMORY() : t0(GetMicroSecond()) {}
//         ~TIME_AND_MEMORY() {
//             t1 = GetMicroSecond();
//             cout << "\ntime cost = " << (t1 - t0) / 1000.0 << " ms\nmemory cost = " << (GetMemory() / 250) << " MB\n ";
//         }
//     } tam;
// #endif
// #endif
//     int t;
//     cin >> t;
//     while (t--) {
//         uint32_t n, m, p;
//         cin >> n >> m >> p;
//         cout << OY::Lucas(p).query(n + m, n) << endl;
//     }
// }


#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <string.h>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#if defined(_WIN32)
#include <windows.h>
// split to make windows.h before psapi.h
#include <psapi.h>
#else
#include "sys/time.h"
#endif
#include "IO/FastIO.h"
#include "MATH/DynamicLucasTable.h"
#include "MATH/PrimeCheck.h"
#include "MATH/StaticLucasTable.h"

int main() {
#ifdef OY_LOCAL
#if defined(_WIN32)
    struct TIME_AND_MEMORY {static auto GetMicroSecond() {static FILETIME ft;GetSystemTimeAsFileTime(&ft);return ft.dwLowDateTime;};static auto GetMemory() {PROCESS_MEMORY_COUNTERS pmc;GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));return pmc.WorkingSetSize;}uint32_t t0, t1;TIME_AND_MEMORY() : t0(GetMicroSecond()) {}~TIME_AND_MEMORY() {t1 = GetMicroSecond();cout << "\ntime cost = "<<(t1-t0)/10000.0<<" ms\nmemory cost = "<<(GetMemory()>>20)<<" MB\n ";}}tam;
#else
    struct TIME_AND_MEMORY {static auto GetMicroSecond() {static timeval ft;gettimeofday(&ft, nullptr);return ft.tv_sec * 1000000 + ft.tv_usec;};static auto GetMemory() {char _mem_buff[512] = {0};int used, now_use;fgets(_mem_buff, sizeof(_mem_buff), fopen("/proc/self/statm", "r"));sscanf(_mem_buff, "%d %d", &used, &now_use);return now_use;}uint64_t t0, t1;TIME_AND_MEMORY() : t0(GetMicroSecond()) {}~TIME_AND_MEMORY() {t1 = GetMicroSecond();cout << "\ntime cost = "<<(t1-t0)/1000.0<<" ms\nmemory cost = "<<(GetMemory()/250)<<" MB\n ";}}tam;
#endif
#endif
    std::mt19937_64 rr;
    uint64_t s=0;
    using table=OY::StaticLucasTable<100003>;
    // table::init();
    for(int i=0;i<10000000;i++){
        auto a=rr()%1000000000ull;
        auto b=rr()%a;
        s^=table::query(a,b);
    }
    cout<<s<<endl;
}