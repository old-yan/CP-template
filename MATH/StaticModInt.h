#ifndef __OY_STATICMODINT__
#define __OY_STATICMODINT__

#include <cstdint>
#include <functional>

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
        static StaticModInt<_Mod> raw(uint32_t __val){
            StaticModInt<_Mod> res;
            res.m_val=__val;
            return res;
        }
        uint32_t val()const{return m_val;}
        StaticModInt<_Mod>pow(uint64_t __n)const{
            StaticModInt<_Mod>x(*this),res=raw(1);
            while(__n){
                if(__n&1)res*=x;
                x*=x;
                __n>>=1;
            }
            return res;
        }
        StaticModInt<_Mod>inv()const{
            uint32_t x=_Mod,y=m_val,m0=0,m1=1;
            while(y){
                uint32_t z=x/y;
                x-=y*z;
                m0-=m1*z;
                std::swap(x,y);
                std::swap(m0,m1);
            }
            if(m0>=_Mod)m0+=_Mod/x;
            return raw(m0);
        }
        StaticModInt<_Mod>&operator++(){
            if(++m_val==_Mod)m_val=0;
            return *this;
        }
        StaticModInt<_Mod>&operator--(){
            if(m_val==0)m_val=_Mod;
            m_val--;
            return *this;
        }
        StaticModInt<_Mod>operator++(int){
            StaticModInt<_Mod>old(*this);
            ++*this;
            return old;
        }
        StaticModInt<_Mod>operator--(int){
            StaticModInt<_Mod>old(*this);
            --*this;
            return old;
        }
        StaticModInt<_Mod>&operator+=(const StaticModInt<_Mod>&__other){
            m_val+=__other.m_val;
            if(m_val>=_Mod)m_val-=_Mod;
            return *this;
        }
        StaticModInt<_Mod>&operator-=(const StaticModInt<_Mod>&__other){
            m_val-=__other.m_val;
            if(m_val>=_Mod)m_val+=_Mod;
            return *this;
        }
        StaticModInt<_Mod>&operator*=(const StaticModInt<_Mod>&__other){
            m_val=uint64_t(m_val)*__other.m_val%_Mod;
            return *this;
        }
        StaticModInt<_Mod>&operator/=(const StaticModInt<_Mod>&__other){
            return *this*=__other.inv();
        }
        StaticModInt<_Mod>operator+(){return *this;}
        StaticModInt<_Mod>operator-(){return _Mod-*this;}
        bool operator==(const StaticModInt<_Mod>&__other)const{return m_val==__other.m_val;}
        bool operator!=(const StaticModInt<_Mod>&__other)const{return m_val!=__other.m_val;}
        bool operator<(const StaticModInt<_Mod>&__other)const{return m_val<__other.m_val;}
        bool operator>(const StaticModInt<_Mod>&__other)const{return m_val>__other.m_val;}
        bool operator<=(const StaticModInt<_Mod>&__other)const{return m_val<=__other.m_val;}
        bool operator>=(const StaticModInt<_Mod>&__other)const{return m_val<=__other.m_val;}
        template<typename _Tp>
        explicit operator _Tp()const{return _Tp(m_val);}
        friend StaticModInt<_Mod>operator+(const StaticModInt<_Mod>&a,const StaticModInt<_Mod>&b){return StaticModInt<_Mod>(a)+=b;}
        friend StaticModInt<_Mod>operator-(const StaticModInt<_Mod>&a,const StaticModInt<_Mod>&b){return StaticModInt<_Mod>(a)-=b;}
        friend StaticModInt<_Mod>operator*(const StaticModInt<_Mod>&a,const StaticModInt<_Mod>&b){return StaticModInt<_Mod>(a)*=b;}
        friend StaticModInt<_Mod>operator/(const StaticModInt<_Mod>&a,const StaticModInt<_Mod>&b){return StaticModInt<_Mod>(a)/=b;}
        template<typename _Istream>
        friend _Istream& operator>>(_Istream&is,StaticModInt<_Mod>&self){return is>>self.m_val;}
        template<typename _Ostream>
        friend _Ostream& operator<<(_Ostream&os,const StaticModInt<_Mod>&self){return os<<self.m_val;}
    };
    using StaticModInt_1000000007=StaticModInt<1000000007>;
}

#endif