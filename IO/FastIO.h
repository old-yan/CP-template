#ifndef __OY_FASTIO__
#define __OY_FASTIO__

#include <cmath>
#include <iostream>
#include <type_traits>

namespace OY {
#define OY_INPUTSIZE (1 << 10)
#define cin OY::inputHelper::getInstance()
#define getchar OY::inputHelper::getInstance().getChar
#define OY_OUTPUTSIZE (1 << 10)
#define cout OY::outputHelper::getInstance()
#define putchar OY::outputHelper::getInstance().putChar
#define endl '\n'
#define log(...) OY::printLog(", ", __VA_ARGS__)
    class inputHelper {
        FILE *m_filePtr = nullptr;
        char m_fileBuf[OY_INPUTSIZE], *m_fileBufEnd, *m_fileCursor, m_current;
        bool m_fail, m_preview;

    public:
        explicit inputHelper(const char *inputFileName) : m_fail(false), m_preview(false) {
            if (!*inputFileName)
                m_filePtr = stdin;
            else
                m_filePtr = fopen(inputFileName, "rt");
            m_fileBufEnd = m_fileCursor = m_fileBuf;
        }
        static inputHelper &getInstance() {
#ifdef OY_LOCAL
            static inputHelper s_obj("in.txt");
#else
            static inputHelper s_obj("");
#endif
            return s_obj;
        }
        ~inputHelper() {
            fclose(m_filePtr);
        }
        char getChar() {
            if (m_preview) {
                m_preview = false;
                return m_current;
            }
            if (!m_fail && m_fileCursor == m_fileBufEnd) {
                if (m_fileBufEnd = m_fileBuf + fread(m_fileBuf, 1, OY_INPUTSIZE, m_filePtr);m_fileBufEnd == m_fileBuf)
                    m_fail = true;
                else
                    m_fileCursor = m_fileBuf;
            }
            if (!m_fail)
                return *m_fileCursor++;
            else
                return EOF;
        }
        char nextChar() {
            if (m_preview) return m_current;
            if (!m_fail && m_fileCursor == m_fileBufEnd) {
                if (m_fileBufEnd = m_fileBuf + fread(m_fileBuf, 1, OY_INPUTSIZE, m_filePtr);m_fileBufEnd == m_fileBuf)
                    m_fail = true;
                else
                    m_fileCursor = m_fileBuf;
            }
            if (!m_fail) {
                m_preview = true;
                return m_current = *m_fileCursor++;
            } else
                return EOF;
        }
        void popNext() {
            m_preview = false;
        }
        static constexpr bool isBlank(char c) {
            return c == ' ' || c == '\t' || c == '\n' || c == '\r';
        }
        static constexpr bool isEndline(char c) {
            return c == '\n' || c == EOF;
        }
        template <typename T>
        std::enable_if_t<std::is_signed_v<T> & std::is_integral_v<T>, inputHelper &> operator>>(T &ret) {
            bool sign = false;
            while (isBlank(nextChar())) popNext();
            if (nextChar() == '-') {
                sign = true;
                popNext();
            }
            if (isdigit(nextChar())) {
                ret = 0;
                if (sign) do
                        ret = ret * 10 - (getChar() - '0');
                    while (isdigit(nextChar()));
                else
                    do ret = ret * 10 + (getChar() - '0');
                    while (isdigit(nextChar()));
            } else
                m_fail = true;
            return *this;
        }
        template <typename T>
        std::enable_if_t<std::is_unsigned_v<T> & std::is_integral_v<T>, inputHelper &> operator>>(T &ret) {
            while (isBlank(nextChar())) popNext();
            if (isdigit(nextChar())) {
                ret = 0;
                do
                    ret = ret * 10 + (getChar() - '0');
                while (isdigit(nextChar()));
            } else
                m_fail = true;
            return *this;
        }
        template <typename T>
        std::enable_if_t<std::is_floating_point_v<T>, inputHelper &> operator>>(T &ret) {
            bool sign = false, integer = false, decimal = false;
            while (isBlank(nextChar())) popNext();
            if (nextChar() == '-') {
                sign = true;
                popNext();
            }
            if (!isdigit(nextChar()) && nextChar() != '.') {
                m_fail = true;
                return *this;
            }
            if (isdigit(nextChar())) {
                integer = true;
                ret = 0;
                do ret = ret * 10 + (getChar() - '0');
                while (isdigit(nextChar()));
            }
            if (nextChar() == '.') {
                popNext();
                if (isdigit(nextChar())) {
                    if (!integer) ret = 0;
                    decimal = true;
                    T unit = 1;
                    do {
                        unit *= 0.1;
                        ret += unit * (getChar() - '0');
                    } while (isdigit(nextChar()));
                }
            }
            if (!integer && !decimal)
                m_fail = true;
            else if (sign)
                ret = -ret;
            return *this;
        }
        inputHelper &operator>>(char &ret) {
            while (isBlank(nextChar())) popNext();
            ret = getChar();
            return *this;
        }
        inputHelper &operator>>(std::string &ret) {
            while (isBlank(nextChar())) popNext();
            if (nextChar() != EOF) {
                ret.clear();
                do ret += getChar();
                while (!isBlank(nextChar()) && nextChar() != EOF);
            } else
                m_fail = true;
            return *this;
        }
        explicit operator bool() {
            return !m_fail;
        }
        friend inline inputHelper &getline(inputHelper &ih, std::string &ret);
    };
    class outputHelper {
        FILE *m_filePtr = nullptr;
        char m_fileBuf[OY_OUTPUTSIZE], *m_fileBufEnd, *m_fileCursor;
        char m_tempBuf[50], *m_tempBufCursor, *m_tempBufDot;
        uint64_t m_floatReserve, m_floatRatio;

    public:
        outputHelper(const char *outputFileName, int prec = 6) {
            if (!*outputFileName)
                m_filePtr = stdout;
            else
                m_filePtr = fopen(outputFileName, "wt");
            m_fileCursor = m_fileBuf;
            m_fileBufEnd = m_fileBuf + OY_OUTPUTSIZE;
            m_tempBufCursor = m_tempBuf;
            precision(prec);
        }
        static outputHelper &getInstance() {
#ifdef OY_LOCAL
            static outputHelper s_obj("out.txt");
#else
            static outputHelper s_obj("");
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
        outputHelper &flush() {
            if (m_fileCursor > m_fileBuf) {
                fwrite(m_fileBuf, 1, m_fileCursor - m_fileBuf, m_filePtr);
                fflush(m_filePtr);
                m_fileCursor = m_fileBuf;
            }
            return *this;
        }
        void putChar(const char &c) {
            if (m_fileCursor == m_fileBufEnd) flush();
            *m_fileCursor++ = c;
        }
        void putS(const char *c) {
            while (*c) putChar(*c++);
        }
        template <typename T>
        std::enable_if_t<std::is_signed_v<T> & std::is_integral_v<T>, outputHelper &> operator<<(const T &ret) {
            T _ret = (T)ret;
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
        template <typename T>
        std::enable_if_t<std::is_unsigned_v<T> & std::is_integral_v<T>, outputHelper &> operator<<(const T &ret) {
            T _ret = (T)ret;
            do {
                *m_tempBufCursor++ = '0' + _ret % 10;
                _ret /= 10;
            } while (_ret);
            do putChar(*--m_tempBufCursor);
            while (m_tempBufCursor > m_tempBuf);
            return *this;
        }
        template <typename T>
        std::enable_if_t<std::is_floating_point_v<T>, outputHelper &> operator<<(const T &ret) {
            if (ret < 0) {
                putChar('-');
                return *this << -ret;
            }
            T _ret = ret * m_floatRatio;
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
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBuf);
            } else {
                putS("0.");
                for (int i = m_tempBufDot - m_tempBufCursor; i--;) putChar('0');
                do putChar(*--m_tempBufCursor);
                while (m_tempBufCursor > m_tempBuf);
            }
            return *this;
        }
        outputHelper &operator<<(const char &ret) {
            putChar(ret);
            return *this;
        }
        outputHelper &operator<<(const std::string &ret) {
            putS(ret.data());
            return *this;
        }
    };
    inputHelper &getline(inputHelper &ih, std::string &ret) {
        ret.clear();
        while (!inputHelper::getInstance().isEndline(ih.nextChar()) && ih.nextChar() != EOF) ret += ih.getChar();
        ih.popNext();
        return ih;
    }
    template <typename D, typename T, typename... S>
    void printLog(D delim, const T &x, S... rest) {
        cout << x;
        if constexpr (sizeof...(rest)) {
            cout << delim;
            printLog(delim, rest...);
        }
    }
}
using OY::getline;

#endif