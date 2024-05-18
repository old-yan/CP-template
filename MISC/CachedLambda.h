/*
最后修改:
20240517
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_CACHEDLAMBDA__
#define __OY_CACHEDLAMBDA__

#include <tuple>
#include <type_traits>

#include "../DS/GlobalHashMap.h"

namespace OY {
    namespace GHASH {
        template <size_t I, typename Tuple>
        uint64_t HashGet(const Tuple &x) { return Hash<typename std::decay<decltype(std::get<I>(x))>::type>()(std::get<I>(x)); }
        template <typename... Args>
        struct Hash<std::tuple<Args...>> {
            template <size_t... Is>
            uint64_t operator()(const std::tuple<Args...> &x, std::index_sequence<Is...>) const {
                if constexpr (sizeof...(Args) == 1)
                    return HashGet<0>(x);
                else
                    return HashCombine()(HashGet<Is>(x)...);
            }
            uint64_t operator()(const std::tuple<Args...> &x) const { return operator()(x, std::make_index_sequence<sizeof...(Args)>()); }
        };
    }
    namespace CACHE {
        using size_type = uint32_t;
        template <typename Func, bool MakeRecord, size_type BUFFER, typename KeyType, typename Ret, typename... Args>
        struct Solver {
            Func m_func;
            GHASH::UnorderedMap<KeyType, Ret, MakeRecord, BUFFER> m_cache;
            template <size_type I, typename Fi, typename... RestArgs>
            uint64_t _hash(Fi &&first, RestArgs &&...args) {
                if constexpr (I == sizeof...(Args))
                    return GHASH::HashCombine()(first, args...);
                else
                    return _hash<I + 1>(args..., GHASH::Hash<Fi>()(first));
            }
            Solver(Func func) : m_func(func) {}
            void set_initial_value(Args... args, Ret val) { m_cache.insert_or_assign(KeyType{std::forward<Args>(args)...}, val); }
            auto query(Args... args) { return operator()(*this, std::forward<Args>(args)...); }
            const Ret &operator()(Solver<Func, MakeRecord, BUFFER, KeyType, Ret, Args...> &, Args... args) {
                auto it = m_cache.insert(KeyType{std::forward<Args>(args)...});
                if (it.m_flag) it.m_ptr->m_mapped = m_func(*this, std::forward<Args>(args)...);
                return it.m_ptr->m_mapped;
            }
            void cache_clear() {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                m_cache.clear();
            }
        };
        template <typename F>
        struct RecursiveLambdaTraits : RecursiveLambdaTraits<decltype(&F::template operator()<F>)> {};
        template <typename Cls, typename ReturnType, typename Self, typename... Args>
        struct RecursiveLambdaTraits<ReturnType (Cls::*)(Self, Args...) const> {
            using tuple_type = std::tuple<typename std::decay<Args>::type...>;
            using return_type = ReturnType;
            template <typename Func, bool MakeRecord, size_type BUFFER, typename KeyType>
            using type = Solver<Func, MakeRecord, BUFFER, KeyType, ReturnType, Args...>;
        };
    }
    template <bool MakeRecord, CACHE::size_type BUFFER, typename Func, typename KeyType = typename CACHE::RecursiveLambdaTraits<Func>::tuple_type, typename Solver = typename CACHE::RecursiveLambdaTraits<Func>::template type<Func, MakeRecord, BUFFER, KeyType>>
    auto make_CacheSolver(Func func) -> Solver { return Solver(func); }
}

#endif
