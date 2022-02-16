#ifndef __OY_DYNAMICMODINT__
#define __OY_DYNAMICMODINT__

namespace OY{
    template<uint32_t _Id>
    struct DynamicModInt{
        static inline uint32_t _Mod=0;
        uint32_t m_val;
        DynamicModInt():m_val(0){}
        template<typename _Tp,std::enable_if_t<std::is_signed_v<_Tp>>* =nullptr>
        DynamicModInt(_Tp __val){
            int64_t x= int64_t(__val)%int64_t(_Mod);
            if(x<0)x+=_Mod;
            m_val=x;
        }
        template<typename _Tp,std::enable_if_t<std::is_unsigned_v<_Tp>>* =nullptr>
        DynamicModInt(_Tp __val){
            m_val=__val%_Mod;
        }
        static void set(uint32_t __Mod){_Mod=__Mod;}
        static DynamicModInt<_Id> raw(uint32_t __val){
            DynamicModInt<_Id> res;
            res.m_val=__val;
            return res;
        }
        uint32_t val()const{return m_val;}
        DynamicModInt<_Id>pow(uint64_t __n)const{
            DynamicModInt<_Id>x(*this),res=raw(1);
            while(__n){
                if(__n&1)res*=x;
                x*=x;
                __n>>=1;
            }
            return res;
        }
        DynamicModInt<_Id>inv()const{
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
        DynamicModInt<_Id>&operator++(){
            if(++m_val==_Mod)m_val=0;
            return *this;
        }
        DynamicModInt<_Id>&operator--(){
            if(m_val==0)m_val=_Mod;
            m_val--;
            return *this;
        }
        DynamicModInt<_Id>operator++(int){
            DynamicModInt<_Id>old(*this);
            ++*this;
            return old;
        }
        DynamicModInt<_Id>operator--(int){
            DynamicModInt<_Id>old(*this);
            --*this;
            return old;
        }
        DynamicModInt<_Id>&operator+=(const DynamicModInt<_Id>&__other){
            m_val+=__other.m_val;
            if(m_val>=_Mod)m_val-=_Mod;
            return *this;
        }
        DynamicModInt<_Id>&operator-=(const DynamicModInt<_Id>&__other){
            m_val-=__other.m_val;
            if(m_val>=_Mod)m_val+=_Mod;
            return *this;
        }
        DynamicModInt<_Id>&operator*=(const DynamicModInt<_Id>&__other){
            m_val=uint64_t(m_val)*__other.m_val%_Mod;
            return *this;
        }
        DynamicModInt<_Id>&operator/=(const DynamicModInt<_Id>&__other){
            return *this*=__other.inv();
        }
        DynamicModInt<_Id>operator+(){return *this;}
        DynamicModInt<_Id>operator-(){return _Mod-*this;}
        bool operator==(const DynamicModInt<_Id>&__other)const{return m_val==__other.m_val;}
        bool operator!=(const DynamicModInt<_Id>&__other)const{return m_val!=__other.m_val;}
        bool operator<(const DynamicModInt<_Id>&__other)const{return m_val<__other.m_val;}
        bool operator>(const DynamicModInt<_Id>&__other)const{return m_val>__other.m_val;}
        bool operator<=(const DynamicModInt<_Id>&__other)const{return m_val<=__other.m_val;}
        bool operator>=(const DynamicModInt<_Id>&__other)const{return m_val<=__other.m_val;}
        template<typename _Tp>
        explicit operator _Tp()const{return _Tp(m_val);}
        friend DynamicModInt<_Id>operator+(const DynamicModInt<_Id>&a,const DynamicModInt<_Id>&b){return DynamicModInt<_Id>(a)+=b;}
        friend DynamicModInt<_Id>operator-(const DynamicModInt<_Id>&a,const DynamicModInt<_Id>&b){return DynamicModInt<_Id>(a)-=b;}
        friend DynamicModInt<_Id>operator*(const DynamicModInt<_Id>&a,const DynamicModInt<_Id>&b){return DynamicModInt<_Id>(a)*=b;}
        friend DynamicModInt<_Id>operator/(const DynamicModInt<_Id>&a,const DynamicModInt<_Id>&b){return DynamicModInt<_Id>(a)/=b;}
        template<typename _Istream>
        friend _Istream& operator>>(_Istream&is,DynamicModInt<_Id>&self){return is>>self.m_val;}
        template<typename _Ostream>
        friend _Ostream& operator<<(_Ostream&os,const DynamicModInt<_Id>&self){return os<<self.m_val;}
    };
}

#endif