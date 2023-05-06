/*
最后修改:
20230419
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
        template <size_t _BlockSize>
        void flush() {
            size_t a = m_end - m_cursor;
            if (a >= _BlockSize) return;
            memmove(m_buf, m_cursor, a);
            m_cursor = m_buf;
            size_t b = fread(m_buf + a, 1, BUFFER_SIZE - a, m_filePtr);
            if (a + b < BUFFER_SIZE) m_end = m_buf + a + b, *m_end = EOF;
        }
        template <typename _Tp, typename _BinaryOperation>
        InputHelper &fillInteger(_Tp &__ret, _BinaryOperation __op) {
            if (!isdigit(getChar_Unchecked())) return setBad();
            __ret = __op(_Tp(0), getChar_Unchecked() - '0');
            while (next(), isdigit(getChar_Unchecked())) __ret = __op(__ret * 10, getChar_Unchecked() - '0');
            return *this;
        }
        explicit InputHelper(const char* __inputFileName) : m_ok(true), m_buf{}, m_cursor(m_buf + BUFFER_SIZE), m_end(m_buf + BUFFER_SIZE) { m_filePtr = *__inputFileName ? fopen(__inputFileName, "rt") : stdin; }
        ~InputHelper() { fclose(m_filePtr); }
        static InputHelper &getInstance() {
#ifdef OY_LOCAL
            static InputHelper s_obj("in.txt");
#else
            static InputHelper s_obj("");
#endif
            return s_obj;
        }
        static bool isBlank(char __c) { return __c == ' ' || __c == '\t' || __c == '\n' || __c == '\r'; }
        static bool isEndline(char __c) { return __c == '\n' || __c == EOF; }
        const char &getChar_Checked() {
            if (m_cursor < m_end) return *m_cursor;
            flush<1>();
            return *m_cursor;
        }
        const char &getChar_Unchecked() const { return *m_cursor; }
        void next() { ++m_cursor; }
        template <typename _Tp, typename std::enable_if<std::is_signed<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        InputHelper &operator>>(_Tp &__num) {
            while (isBlank(getChar_Checked())) next();
            flush<MAX_INTEGER_SIZE>();
            if (getChar_Unchecked() != '-') return fillInteger(__num, std::plus<_Tp>());
            next();
            return fillInteger(__num, std::minus<_Tp>());
        }
        template <typename _Tp, typename std::enable_if<std::is_unsigned<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        InputHelper &operator>>(_Tp &__num) {
            while (isBlank(getChar_Checked())) next();
            flush<MAX_INTEGER_SIZE>();
            return fillInteger(__num, std::plus<_Tp>());
        }
        template <typename _Tp, typename std::enable_if<std::is_floating_point<_Tp>::value>::type * = nullptr>
        InputHelper &operator>>(_Tp &__num) {
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
                __num = getChar_Unchecked() - '0';
                while (next(), isdigit(getChar_Unchecked())) __num = __num * 10 + (getChar_Unchecked() - '0');
            }
            if (getChar_Unchecked() == '.')
                if (next(), isdigit(getChar_Unchecked())) {
                    if (!integer) __num = 0;
                    decimal = true;
                    _Tp unit = 0.1;
                    __num += unit * (getChar_Unchecked() - '0');
                    while (next(), isdigit(getChar_Unchecked())) __num += (unit *= 0.1) * (getChar_Unchecked() - '0');
                }
            if (!integer && !decimal) return setBad();
            if (neg) __num = -__num;
            return *this;
        }
        InputHelper &operator>>(char &__c) {
            while (isBlank(getChar_Checked())) next();
            if (getChar_Checked() == EOF) return setBad();
            __c = getChar_Checked();
            next();
            return *this;
        }
        InputHelper &operator>>(std::string &__s) {
            while (isBlank(getChar_Checked())) next();
            if (getChar_Checked() == EOF) return setBad();
            __s.clear();
            do {
                __s += getChar_Checked();
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
        OutputHelper(const char *__outputFileName, int __prec = 6) {
            if (!*__outputFileName)
                m_filePtr = stdout;
            else
                m_filePtr = fopen(__outputFileName, "wt");
            m_cursor = m_buf;
            m_end = m_buf + BUFFER_SIZE;
            m_tempBufCursor = m_tempBuf;
            precision(__prec);
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
        void precision(int __prec) {
            m_floatReserve = __prec;
            m_floatRatio = uint64_t(pow(10, __prec));
            m_tempBufDot = m_tempBuf + __prec;
        }
        OutputHelper &flush() {
            fwrite(m_buf, 1, m_cursor - m_buf, m_filePtr);
            fflush(m_filePtr);
            m_cursor = m_buf;
            return *this;
        }
        void putChar(const char &__c) {
            if (m_cursor == m_end) flush();
            *m_cursor++ = __c;
        }
        void putS(const char *__c) {
            while (*__c) putChar(*__c++);
        }
        template <typename _Tp, typename std::enable_if<std::is_signed<_Tp>::value & std::is_integral<_Tp>::value>::type * = nullptr>
        OutputHelper &operator<<(const _Tp &__ret) {
            _Tp _ret = _Tp(__ret);
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
        OutputHelper &operator<<(const _Tp &__ret) {
            _Tp _ret = _Tp(__ret);
            do {
                *m_tempBufCursor++ = '0' + _ret % 10;
                _ret /= 10;
            } while (_ret);
            do putChar(*--m_tempBufCursor);
            while (m_tempBufCursor > m_tempBuf);
            return *this;
        }
        template <typename _Tp, typename std::enable_if<std::is_floating_point<_Tp>::value>::type * = nullptr>
        OutputHelper &operator<<(const _Tp &__ret) {
            if (__ret < 0) {
                putChar('-');
                return *this << -__ret;
            }
            _Tp _ret = __ret * m_floatRatio;
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
        OutputHelper &operator<<(const char &__ret) {
            putChar(__ret);
            return *this;
        }
        OutputHelper &operator<<(const std::string &__ret) {
            putS(__ret.data());
            return *this;
        }
    };
    InputHelper &getline(InputHelper &__ih, std::string &__line) {
        __line.clear();
        if (__ih.getChar_Checked() == EOF) return __ih.setBad();
        while (!InputHelper::isEndline(__ih.getChar_Checked())) {
            __line += __ih.getChar_Unchecked();
            __ih.next();
        }
        if (__ih.getChar_Unchecked() != EOF) __ih.next();
        return __ih;
    }
}
using OY::getline;

#endif /*__OY_FASTIO__*/