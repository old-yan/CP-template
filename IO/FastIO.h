/*
最后修改:
20230815
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/

#ifndef __OY_FASTIO__
#define __OY_FASTIO__

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

namespace OY {
#define cin OY::InputHelper::getInstance()
#define cout OY::OutputHelper::getInstance()
#define endl '\n'
    struct InputHelper {
        static constexpr size_t BUFFER_SIZE = 1 << 10, MAX_INTEGER_SIZE = 20, MAX_FLOAT_SIZE = 20;
        FILE *m_filePtr;
        char m_buf[BUFFER_SIZE], *m_end, *m_cursor;
        bool m_ok;
        InputHelper &setBad() {
            m_ok = false;
            return *this;
        }
        template <size_t BlockSize>
        void flush() {
            size_t a = m_end - m_cursor;
            if (a >= BlockSize) return;
            memmove(m_buf, m_cursor, a);
            m_cursor = m_buf;
            size_t b = fread(m_buf + a, 1, BUFFER_SIZE - a, m_filePtr);
            if (a + b < BUFFER_SIZE) m_end = m_buf + a + b, *m_end = EOF;
        }
        template <typename Tp, typename BinaryOperation>
        InputHelper &fillInteger(Tp &ret, BinaryOperation op) {
            if (!isdigit(getChar_Unchecked())) return setBad();
            ret = op(Tp(0), getChar_Unchecked() - '0');
            while (next(), isdigit(getChar_Unchecked())) ret = op(ret * 10, getChar_Unchecked() - '0');
            return *this;
        }
        explicit InputHelper(const char *inputFileName) : m_ok(true), m_buf{}, m_cursor(m_buf + BUFFER_SIZE), m_end(m_buf + BUFFER_SIZE) { m_filePtr = *inputFileName ? fopen(inputFileName, "rt") : stdin; }
        ~InputHelper() { fclose(m_filePtr); }
        static InputHelper &getInstance() {
#ifdef OY_LOCAL
            static InputHelper s_obj("in.txt");
#else
            static InputHelper s_obj("");
#endif
            return s_obj;
        }
        static bool isBlank(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
        static bool isEndline(char c) { return c == '\n' || c == EOF; }
        const char &getChar_Checked() {
            if (m_cursor < m_end) return *m_cursor;
            flush<1>();
            return *m_cursor;
        }
        const char &getChar_Unchecked() const { return *m_cursor; }
        void next() { ++m_cursor; }
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
        InputHelper &operator>>(Tp &num) {
            while (isBlank(getChar_Checked())) next();
            flush<MAX_INTEGER_SIZE>();
            if (getChar_Unchecked() != '-') return fillInteger(num, std::plus<Tp>());
            next();
            return fillInteger(num, std::minus<Tp>());
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
        InputHelper &operator>>(Tp &num) {
            while (isBlank(getChar_Checked())) next();
            flush<MAX_INTEGER_SIZE>();
            return fillInteger(num, std::plus<Tp>());
        }
        template <typename Tp, typename std::enable_if<std::is_floating_point<Tp>::value>::type * = nullptr>
        InputHelper &operator>>(Tp &num) {
            bool neg = false, integer = false, decimal = false;
            while (isBlank(getChar_Checked())) next();
            flush<MAX_FLOAT_SIZE>();
            if (getChar_Unchecked() == '-') {
                neg = true;
                next();
            }
            if (!isdigit(getChar_Unchecked()) && getChar_Unchecked() != '.') return setBad();
            if (isdigit(getChar_Unchecked())) {
                integer = true;
                num = getChar_Unchecked() - '0';
                while (next(), isdigit(getChar_Unchecked())) num = num * 10 + (getChar_Unchecked() - '0');
            }
            if (getChar_Unchecked() == '.')
                if (next(), isdigit(getChar_Unchecked())) {
                    if (!integer) num = 0;
                    decimal = true;
                    Tp unit = 0.1;
                    num += unit * (getChar_Unchecked() - '0');
                    while (next(), isdigit(getChar_Unchecked())) num += (unit *= 0.1) * (getChar_Unchecked() - '0');
                }
            if (!integer && !decimal) return setBad();
            if (neg) num = -num;
            return *this;
        }
        InputHelper &operator>>(char &c) {
            while (isBlank(getChar_Checked())) next();
            if (getChar_Checked() == EOF) return setBad();
            c = getChar_Checked();
            next();
            return *this;
        }
        InputHelper &operator>>(std::string &s) {
            while (isBlank(getChar_Checked())) next();
            if (getChar_Checked() == EOF) return setBad();
            s.clear();
            do {
                s += getChar_Checked();
                next();
            } while (!isBlank(getChar_Checked()) && getChar_Unchecked() != EOF);
            return *this;
        }
        explicit operator bool() { return m_ok; }
    };
    struct OutputHelper {
        static constexpr size_t BUFFER_SIZE = 1 << 20, MAX_NUMBER_SIZE = 50;
        FILE *m_filePtr = nullptr;
        char m_buf[BUFFER_SIZE], *m_end, *m_cursor;
        char m_tempBuf[MAX_NUMBER_SIZE], *m_tempBufCursor, *m_tempBufDot;
        uint64_t m_floatReserve, m_floatRatio;
        OutputHelper(const char *outputFileName, int prec = 6) {
            if (!*outputFileName)
                m_filePtr = stdout;
            else
                m_filePtr = fopen(outputFileName, "wt");
            m_cursor = m_buf;
            m_end = m_buf + BUFFER_SIZE;
            m_tempBufCursor = m_tempBuf;
            precision(prec);
        }
        static OutputHelper &getInstance() {
#ifdef OY_LOCAL
            static OutputHelper s_obj("out.txt");
#else
            static OutputHelper s_obj("");
#endif
            return s_obj;
        }
        ~OutputHelper() {
            flush();
            fclose(m_filePtr);
        }
        void precision(int prec) {
            m_floatReserve = prec;
            m_floatRatio = uint64_t(pow(10, prec));
            m_tempBufDot = m_tempBuf + prec;
        }
        OutputHelper &flush() {
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
        template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
        OutputHelper &operator<<(Tp ret) {
            if (ret >= 0) {
                do {
                    *m_tempBufCursor++ = '0' + ret % 10;
                    ret /= 10;
                } while (ret);
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBuf);
            } else {
                putChar('-');
                do {
                    *m_tempBufCursor++ = '0' - ret % 10;
                    ret /= 10;
                } while (ret);
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBuf);
            }
            return *this;
        }
        template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
        OutputHelper &operator<<(Tp ret) {
            do {
                *m_tempBufCursor++ = '0' + ret % 10;
                ret /= 10;
            } while (ret);
            do putChar(*--m_tempBufCursor);
            while (m_tempBufCursor > m_tempBuf);
            return *this;
        }
        template <typename Tp, typename std::enable_if<std::is_floating_point<Tp>::value>::type * = nullptr>
        OutputHelper &operator<<(Tp ret) {
            if (ret < 0) {
                putChar('-');
                return *this << -ret;
            }
            ret *= m_floatRatio;
            uint64_t integer = ret;
            if (ret - integer >= 0.4999999999) integer++;
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
        OutputHelper &operator<<(const char &ret) {
            putChar(ret);
            return *this;
        }
        OutputHelper &operator<<(const std::string &ret) {
            putS(ret.data());
            return *this;
        }
    };
    InputHelper &getline(InputHelper &ih, std::string &line) {
        line.clear();
        if (ih.getChar_Checked() == EOF) return ih.setBad();
        while (!InputHelper::isEndline(ih.getChar_Checked())) {
            line += ih.getChar_Unchecked();
            ih.next();
        }
        if (ih.getChar_Unchecked() != EOF) ih.next();
        return ih;
    }
}
using OY::getline;

#endif /*__OY_FASTIO__*/