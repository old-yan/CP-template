#include <algorithm>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_set>
#if defined(_WIN32)
#include <windows.h>
// split to make windows.h before psapi.h
#include <psapi.h>
#else
#include "sys/time.h"
#endif
#define all(a) std::begin(a), std::end(a)
template <typename _Tp, typename _Fp, typename _Compare = std::less<void>>
bool chmax(_Tp &a, const _Fp &b, _Compare __comp = _Compare()) { return __comp(a, b) ? a = b, true : false; }
template <typename _Tp, typename _Fp, typename _Compare = std::less<void>>
bool chmin(_Tp &a, const _Fp &b, _Compare __comp = _Compare()) { return __comp(b, a) ? a = b, true : false; }
template <typename _Tp>
constexpr std::array<std::array<_Tp, 2>, 4> getNeighbors(_Tp i, _Tp j) { return {{{i + 1, j}, {i, j + 1}, {i - 1, j}, {i, j - 1}}}; }
using std::cin, std::cout, std::endl;
namespace OY {
#define cin OY::inputHelper<1 << 10, 20>::getInstance()
#define getchar() ({char c=cin.getChar_Checked();cin.next();c; })
#define cout OY::outputHelper<1 << 20>::getInstance()
#define putchar cout.putChar
#define endl '\n'
#define putlog(...) OY::printLog(", ", __VA_ARGS__)
    template <uint64_t _BufferSize = 1 << 10, uint64_t _BlockSize = 20>
    class inputHelper {
    public:
        FILE *m_filePtr;
        char m_buf[_BufferSize], *m_end, *m_cursor;
        bool m_ok;
        void flush() {
            uint64_t a = m_end - m_cursor;
            if (a >= _BlockSize) return;
            memmove(m_buf, m_cursor, a);
            uint64_t b = fread(m_buf + a, 1, _BufferSize - a, m_filePtr);
            m_cursor = m_buf;
            if (a + b < _BufferSize) {
                m_end = m_buf + a + b;
                *m_end = EOF;
            }
        }

    public:
        explicit inputHelper(const char *inputFileName) : m_ok(true) {
            if (!*inputFileName)
                m_filePtr = stdin;
            else
                m_filePtr = fopen(inputFileName, "rt");
            m_end = m_cursor = m_buf + _BufferSize;
        }
        ~inputHelper() { fclose(m_filePtr); }
        static inputHelper<_BufferSize, _BlockSize> &getInstance() {
#ifdef OY_LOCAL
            static inputHelper<_BufferSize, _BlockSize> s_obj("in.txt");
#else
            static inputHelper<_BufferSize, _BlockSize> s_obj("");
#endif
            return s_obj;
        }
        static constexpr bool isBlank(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
        static constexpr bool isEndline(char c) { return c == '\n' || c == EOF; }
        const char &getChar_Checked() {
            if (m_cursor < m_end) return *m_cursor;
            uint64_t b = fread(m_buf, 1, _BufferSize, m_filePtr);
            m_cursor = m_buf;
            if (b < _BufferSize) {
                m_end = m_buf + b;
                *m_end = EOF;
            }
            return *m_cursor;
        }
        const char &getChar_Unchecked() const { return *m_cursor; }
        void next() { ++m_cursor; }
        void setState(bool _ok) { m_ok = _ok; }
        template <typename _Tp, typename std::enable_if<std::is_signed<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        inputHelper<_BufferSize, _BlockSize> &operator>>(_Tp &ret) {
            while (isBlank(getChar_Checked())) next();
            flush();
            if (getChar_Unchecked() == '-') {
                next();
                if (isdigit(getChar_Unchecked())) {
                    ret = -(getChar_Unchecked() - '0');
                    while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 - (getChar_Unchecked() - '0');
                } else
                    m_ok = false;
            } else {
                if (isdigit(getChar_Unchecked())) {
                    ret = getChar_Unchecked() - '0';
                    while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 + (getChar_Unchecked() - '0');
                } else
                    m_ok = false;
            }
            return *this;
        }
        template <typename _Tp, typename std::enable_if<std::is_unsigned<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        inputHelper<_BufferSize, _BlockSize> &operator>>(_Tp &ret) {
            while (isBlank(getChar_Checked())) next();
            flush();
            if (isdigit(getChar_Unchecked())) {
                ret = getChar_Unchecked() - '0';
                while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 + (getChar_Unchecked() - '0');
            } else
                m_ok = false;
            return *this;
        }
        template <typename _Tp, typename std::enable_if<std::is_floating_point<_Tp>::value>::type * = nullptr>
        inputHelper<_BufferSize, _BlockSize> &operator>>(_Tp &ret) {
            bool neg = false, integer = false, decimal = false;
            while (isBlank(getChar_Checked())) next();
            flush();
            if (getChar_Unchecked() == '-') {
                neg = true;
                next();
            }
            if (!isdigit(getChar_Unchecked()) && getChar_Unchecked() != '.') {
                m_ok = false;
                return *this;
            }
            if (isdigit(getChar_Unchecked())) {
                integer = true;
                ret = getChar_Unchecked() - '0';
                while (next(), isdigit(getChar_Unchecked())) ret = ret * 10 + (getChar_Unchecked() - '0');
            }
            if (getChar_Unchecked() == '.') {
                next();
                if (isdigit(getChar_Unchecked())) {
                    if (!integer) ret = 0;
                    decimal = true;
                    _Tp unit = 0.1;
                    ret += unit * (getChar_Unchecked() - '0');
                    while (next(), isdigit(getChar_Unchecked())) {
                        unit *= 0.1;
                        ret += unit * (getChar_Unchecked() - '0');
                    }
                }
            }
            if (!integer && !decimal)
                m_ok = false;
            else if (neg)
                ret = -ret;
            return *this;
        }
        inputHelper<_BufferSize, _BlockSize> &operator>>(char &ret) {
            while (isBlank(getChar_Checked())) next();
            ret = getChar_Checked();
            if (ret == EOF)
                m_ok = false;
            else
                next();
            return *this;
        }
        inputHelper<_BufferSize, _BlockSize> &operator>>(std::string &ret) {
            while (isBlank(getChar_Checked())) next();
            if (getChar_Checked() != EOF) {
                ret.clear();
                do {
                    ret += getChar_Checked();
                    next();
                } while (!isBlank(getChar_Checked()) && getChar_Unchecked() != EOF);
            } else
                m_ok = false;
            return *this;
        }
        explicit operator bool() { return m_ok; }
    };
    template <uint64_t _BufferSize = 1 << 20>
    class outputHelper {
        FILE *m_filePtr = nullptr;
        char m_buf[_BufferSize], *m_end, *m_cursor;
        char m_tempBuf[50], *m_tempBufCursor, *m_tempBufDot;
        uint64_t m_floatReserve, m_floatRatio;

    public:
        outputHelper(const char *outputFileName, int prec = 6) : m_end(m_buf + _BufferSize) {
            if (!*outputFileName)
                m_filePtr = stdout;
            else
                m_filePtr = fopen(outputFileName, "wt");
            m_cursor = m_buf;
            m_tempBufCursor = m_tempBuf;
            precision(prec);
        }
        static outputHelper<_BufferSize> &getInstance() {
#ifdef OY_LOCAL
            static outputHelper<_BufferSize> s_obj("out.txt");
#else
            static outputHelper<_BufferSize> s_obj("");
#endif
            return s_obj;
        }
        ~outputHelper() {
            flush();
            fclose(m_filePtr);
        }
        void precision(int prec) {
            m_floatReserve = prec;
            m_floatRatio = pow(10, prec);
            m_tempBufDot = m_tempBuf + prec;
        }
        outputHelper<_BufferSize> &flush() {
            fwrite(m_buf, 1, m_cursor - m_buf, m_filePtr);
            fflush(m_filePtr);
            m_cursor = m_buf;
            return *this;
        }
        void putChar(const char &c) {
            if (m_cursor == m_end) flush();
            *m_cursor++ = c;
        }
        void putS(const char *c) {
            while (*c) putChar(*c++);
        }
        template <typename _Tp, typename std::enable_if<std::is_signed<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        outputHelper<_BufferSize> &operator<<(const _Tp &ret) {
            _Tp _ret = _Tp(ret);
            if (_ret >= 0) {
                do {
                    *m_tempBufCursor++ = '0' + _ret % 10;
                    _ret /= 10;
                } while (_ret);
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBuf);
            } else {
                putChar('-');
                do {
                    *m_tempBufCursor++ = '0' - _ret % 10;
                    _ret /= 10;
                } while (_ret);
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBuf);
            }
            return *this;
        }
        template <typename _Tp, typename std::enable_if<std::is_unsigned<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        outputHelper<_BufferSize> &operator<<(const _Tp &ret) {
            _Tp _ret = _Tp(ret);
            do {
                *m_tempBufCursor++ = '0' + _ret % 10;
                _ret /= 10;
            } while (_ret);
            do putChar(*--m_tempBufCursor);
            while (m_tempBufCursor > m_tempBuf);
            return *this;
        }
        template <typename _Tp, typename std::enable_if<std::is_floating_point<_Tp>::value>::type * = nullptr>
        outputHelper<_BufferSize> &operator<<(const _Tp &ret) {
            if (ret < 0) {
                putChar('-');
                return *this << -ret;
            }
            _Tp _ret = ret * m_floatRatio;
            uint64_t integer = _ret;
            if (_ret - integer >= 0.4999999999) integer++;
            do {
                *m_tempBufCursor++ = '0' + integer % 10;
                integer /= 10;
            } while (integer);
            if (m_tempBufCursor > m_tempBufDot) {
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBufDot);
                putChar('.');
            } else {
                putS("0.");
                for (int i = m_tempBufDot - m_tempBufCursor; i--;) putChar('0');
            }
            do putChar(*--m_tempBufCursor);
            while (m_tempBufCursor > m_tempBuf);
            return *this;
        }
        outputHelper<_BufferSize> &operator<<(const char &ret) {
            putChar(ret);
            return *this;
        }
        outputHelper<_BufferSize> &operator<<(const std::string &ret) {
            putS(ret.data());
            return *this;
        }
    };
    template <uint64_t _BufferSize, uint64_t _BlockSize>
    inputHelper<_BufferSize, _BlockSize> &getline(inputHelper<_BufferSize, _BlockSize> &ih, std::string &ret) {
        ret.clear();
        if (ih.getChar_Checked() == EOF)
            ih.setState(false);
        else {
            while (!inputHelper<_BufferSize, _BlockSize>::isEndline(ih.getChar_Checked())) {
                ret += ih.getChar_Unchecked();
                ih.next();
            }
            ih.next();
        }
        return ih;
    }
    template <typename D, typename T, typename... S>
    void printLog(D delim, const T &x, S... rest) {
        cout << x;
        if constexpr (sizeof...(rest) > 0) {
            cout << delim;
            printLog(delim, rest...);
        }
    }
}
using OY::getline;
namespace OY {
    template <typename _Tp>
    struct KuhnMunkres {
        std::vector<std::vector<_Tp>> m_costs;
        std::vector<uint32_t> m_leftMatch;
        std::vector<uint32_t> m_rightMatch;
        uint32_t m_leftNum;
        uint32_t m_rightNum;
        uint32_t m_maxNum;
        KuhnMunkres(uint32_t __leftNum, uint32_t __rightNum, _Tp __initCost = std::numeric_limits<_Tp>::min() / 2) : m_costs(std::max(__leftNum, __rightNum), std::vector<_Tp>(std::max(__leftNum, __rightNum), __initCost)), m_leftNum(__leftNum), m_rightNum(__rightNum), m_maxNum(std::max(__leftNum, __rightNum)) {}
        void addEdge(uint32_t __a, uint32_t __b, _Tp __cost) { chmax(m_costs[__a][__b], __cost); }
        _Tp calc(_Tp __infiniteCost = std::numeric_limits<_Tp>::max() / 2) {
            uint32_t queue[m_maxNum], from[m_maxNum];
            _Tp left_label[m_maxNum], right_label[m_maxNum], slack[m_maxNum];
            std::vector<bool> leftVisit(m_maxNum), rightVisit(m_maxNum);
            auto aug = [&](uint32_t v) {while(~v)std::swap(v,m_leftMatch[m_rightMatch[v]=from[v]]); };
            auto bfs = [&](uint32_t s) {
                std::fill(leftVisit.begin(), leftVisit.end(), false);
                std::fill(rightVisit.begin(), rightVisit.end(), false);
                std::fill(slack, slack + m_maxNum, __infiniteCost);
                uint32_t head = 0, tail = 0;
                queue[tail++] = s;
                leftVisit[s] = true;
                while (true) {
                    while (head < tail)
                        for (uint32_t a = queue[head++], b = 0; b < m_maxNum; b++)
                            if (!rightVisit[b] && chmin(slack[b], left_label[a] + right_label[b] - m_costs[a][b])) {
                                from[b] = a;
                                if (slack[b]) continue;
                                if (!~m_rightMatch[b]) return aug(b);
                                rightVisit[b] = true;
                                queue[tail++] = m_rightMatch[b];
                                leftVisit[m_rightMatch[b]] = true;
                            }
                    _Tp rmin = __infiniteCost;
                    for (uint32_t b = 0; b < m_maxNum; b++)
                        if (!rightVisit[b]) chmin(rmin, slack[b]);
                    for (uint32_t a = 0; a < m_maxNum; a++)
                        if (leftVisit[a]) left_label[a] -= rmin;
                    for (uint32_t b = 0; b < m_maxNum; b++)
                        if (rightVisit[b])
                            right_label[b] += rmin;
                        else
                            slack[b] -= rmin;
                    for (uint32_t b = 0; b < m_maxNum; b++)
                        if (!rightVisit[b] && !slack[b])
                            if (~m_rightMatch[b]) {
                                rightVisit[b] = true;
                                queue[tail++] = m_rightMatch[b];
                                leftVisit[m_rightMatch[b]] = true;
                            } else
                                return aug(b);
                }
            };
            m_leftMatch.resize(m_maxNum, -1);
            m_rightMatch.resize(m_maxNum, -1);
            std::fill(left_label, left_label + m_maxNum, 0);
            std::fill(right_label, right_label + m_maxNum, 0);
            for (uint32_t a = 0; a < m_maxNum; a++)
                for (uint32_t b = 0; b < m_maxNum; b++) chmax(left_label[a], m_costs[a][b]);
            for (uint32_t a = 0; a < m_maxNum; a++) bfs(a);
            _Tp res = 0;
            for (uint32_t a = 0; a < m_maxNum; a++) res += m_costs[a][m_leftMatch[a]];
            return res;
        }
    };
}

int main() {
    uint32_t nl, nr, m;
    cin >> nl >> nr >> m;
    OY::KuhnMunkres<int64_t> km(nl, nr, 0);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        int32_t c;
        cin >> a >> b >> c;
        km.addEdge(a - 1, b - 1, c);
    }
    cout << km.calc() << '\n';
    for (uint32_t a = 0; a < nl; a++) {
        uint32_t b = km.m_leftMatch[a];
        if (km.m_costs[a][b])
            cout << b + 1 << ' ';
        else
            cout << 0 << ' ';
        // cout << km.m_rightMatch[b] + 1 << ' ';
    }
    cout << '\n';
}