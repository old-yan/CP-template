#include <algorithm>
#include <array>
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
#include <unordered_set>
#include <unordered_map>
// using namespace std;
namespace OY {
#define OY_INPUTSIZE (1 << 20)
#define cin OY::inputHelper::getInstance()
#define getchar OY::inputHelper::getInstance().getChar
#define OY_OUTPUTSIZE (1 << 20)
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

// namespace OY{
//     struct StaticHashTranslation{
//         template<typename _Tp>
//         uint32_t operator()(const _Tp&ch){return ch;}
//     };
//     template<typename _Translation=StaticHashTranslation,uint32_t _Base=233,uint32_t _Mod=1000000007>
//     class StaticHashHelper{
//         std::vector<uint32_t>m_val;
//         int m_length;
//         _Translation m_tran;
//     public:
//         template<typename _Iterator>
//         StaticHashHelper(_Iterator __first,_Iterator __last,_Translation __tran):m_tran(__tran){
//             reset(__first,__last);
//             // for(int i=0;i<=n;i++)units[i]=i?units[i-1]*B%M:1;
//             // for(int i=0;i<=n;i++)val[i]=i?(val[i-1]*B+s[i-1])%M:0;
//             // for(int i=0;i<=n;i++)rval[n-i]=i?(rval[n-i+1]*B+s[n-i])%M:0;
//         }
//         template<typename _Iterator>
//         void reset(_Iterator __first,_Iterator __last){
//             m_length=__last-__first;
//             m_val.clear();
//             m_val.reserve(m_length+1);
//             m_val.push_back(0);
//             for(auto it=__first;it!=__last;++it)m_val.push_back((m_val.back()*_Base+m_trans(*it))%_Mod);
//         }
//         uint64_t

//     };
// }
namespace OY{
    template<uint32_t _Mod>
    struct StaticModInt{
        uint32_t m_val;
        StaticModInt():m_val(0){}
        template<typename _Tp,std::enable_if_t<std::is_signed_v<_Tp>>* =nullptr>
        StaticModInt(_Tp __val){
            int64_t x= int64_t(__val)%int64_t(_Mod);
            if(x<0)x+=_Mod;
            m_val=x;
        }
        template<typename _Tp,std::enable_if_t<std::is_unsigned_v<_Tp>>* =nullptr>
        StaticModInt(_Tp __val){
            m_val=__val%_Mod;
        }
        uint32_t val()const{return m_val;}
    };
}

// namespace OY{
//     #ifdef OY_LOCAL
//         #define MODINTMEM 1000
//     #else
//         #define MODINTMEM 1000000
//     #endif
//     #ifndef MODP
//         #define MODP 1000000007
//     #endif
//     bool isPrime(long long n){//对1e16以下有效
//         if(n<=1)return false;
//         if(n==2||n==3||n==7||n==61||n==24251)return true;
//         if(n%2==0||n==46856248255981ll)return false;
//         int shift=__builtin_ctzll(n-1);
//         long long d=(n-1)>>shift;
//         constexpr long long bases[5]={2,3,7,61,24251};
//         for(long long a:bases){
//             unsigned long long r=1,y=a>=n?a%n:a;
//             for(long long t=d;;){
//                 if(t&1)r=r*y%n;
//                 if(t>>=1)y=y*y%n;
//                 else break;
//             }
//             long long t=d;
//             while(t!=n-1&&r!=1&&r!=n-1)r=r*r%n,t>>=1;
//             if(r!=n-1&&t%2==0)return false;
//         }
//         return true;
//     }
//     struct modint{
//         #if MODP == 0
//             static unsigned long long modP;
//         #else
//             static constexpr unsigned long long modP=MODP;
//         #endif
//         typedef unsigned long long ull;
//         typedef unsigned int uint;        
//         static modint inv_mem[MODINTMEM+1];
//         int val;
//         modint(long long _val=0){
//             if(_val<0)_val=_val%modP+modP;
//             else if(_val>=modP)_val%=modP;
//             val=_val;
//         }
//         template<class T>modint(T _val):modint((long long)(_val)){}
//         explicit operator int()const{return val;}
//         explicit operator uint()const{return val;}
//         explicit operator long long()const{return val;}
//         explicit operator ull()const{return val;}
//         explicit operator double()const{return val;}
//         explicit operator long double()const{return val;}
//         explicit operator bool()const{return val;}
//         explicit operator char()const{return val;}
//         modint&operator+=(const modint&other){
//             val-=modP-other.val;
//             if (val<0)val+=modP;
//             return *this;
//         }
//         modint&operator-=(const modint&other){
//             val-=other.val;
//             if(val<0)val+=modP;
//             return *this;
//         }
//         static ull fast_mod(ull x,ull m=0){
//             m=m?m:modP;
//         #if !defined(_WIN32) || defined(_WIN64)
//             return ull(x%m);
//         #endif
//             unsigned x_high=ull(x>>32),x_low=ull(x);
//             unsigned quot,rem;
//             __asm__("divl %4\n"
//             : "=a" (quot), "=d" (rem)
//             : "d" (x_high), "a" (x_low), "r" (m));
//             return rem;
//         }
//         modint&operator*=(const modint&other){
//             val=fast_mod(ull(val)*other.val,modP);
//             return *this;
//         }
//         modint&operator/=(const modint&other){
//             return *this*=other.inv();
//         }
//         friend modint operator+(const modint &a,const modint &b){return modint(a)+=b;}
//         friend modint operator-(const modint &a,const modint &b){return modint(a)-=b;}
//         friend modint operator*(const modint &a,const modint &b){return modint(a)*=b;}
//         friend modint operator/(const modint &a,const modint &b){return modint(a)/=b;}
//         modint&operator++(){
//             val=val==modP-1?0:val+1;
//             return *this;
//         }
//         modint&operator--(){
//             val=val?val-1:modP-1;
//             return *this;
//         }
//         modint operator++(int){
//             modint old=*this;
//             ++*this;
//             return old;
//         }
//         modint operator--(int){
//             modint old=*this;
//             --*this;
//             return old;
//         }
//         modint operator-()const{
//             return val?modP-val:0;
//         }
//         friend bool operator==(const modint &a,const modint &b){return a.val==b.val;}
//         friend bool operator!=(const modint &a,const modint &b){return a.val!=b.val;}
//         friend bool operator<(const modint &a,const modint &b){return a.val<b.val;}
//         friend bool operator>(const modint &a,const modint &b){return a.val>b.val;}
//         friend bool operator<=(const modint &a,const modint &b){return a.val<=b.val;}
//         friend bool operator>=(const modint &a,const modint &b){return a.val>=b.val;}
//         static void inv_init(){
//             if(!isPrime(modP))exit(-1);
//             inv_mem[0]=0,inv_mem[1]=1;
//             for(int i=2;i<=MODINTMEM;i++)inv_mem[i]=inv_mem[modP%i]*(modP-modP/i);
//         }
//         modint inv()const{
//             if(inv_mem[1]==0)inv_init();
//             if(val<=MODINTMEM)return inv_mem[val];
//             modint product=1;
//             int v=val;
//             while(v>MODINTMEM){
//                 product*=modP-modP/v;
//                 v=modP%v;
//             }
//             return product*inv_mem[v];
//         }
//         modint pow(long long p)const{
//             if(p<0)return inv().pow(-p);
//             if(p==0)return 1;
//             modint a=*this,result=1;
//             while(true){
//                 if(p&1)result*=a;
//                 if(p>>=1)a*=a;
//                 else break;
//             }
//             return result;
//         }
//         friend istream&operator>>(istream&is,modint&m){
//             return is>>m.val;
//         }
//         friend ostream&operator<<(ostream&os,const modint&m){
//             return os<<m.val;
//         }
//     };
//     #if MODP == 0
//         unsigned long long modint::modP=1000000007;
//     #endif
//     modint modint::inv_mem[MODINTMEM+1];
// };



int main(){
    constexpr int a=-34;
    constexpr auto b=uint64_t(a)%10;
}