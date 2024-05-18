/*
最后修改:
20240517
测试环境:
gcc11.2,c++14
clang12.0,C++14
msvc14.2,C++14
*/
#ifndef __OY_GRIDCACHEDLAMBDA__
#define __OY_GRIDCACHEDLAMBDA__

#include <algorithm>
#include <cstdint>
#include <type_traits>

namespace OY {
    namespace GRIDCACHE {
        using size_type = uint32_t;
        template <typename Tp>
        struct BaseNode {
            Tp m_val;
            bool m_is_valid = false;
            bool is_valid() const { return m_is_valid; }
            void reset() { m_is_valid = false; }
            void set(const Tp &val) { m_val = val, m_is_valid = true; }
            const Tp &get() const { return m_val; }
        };
        template <typename Node, bool MakeRecord, size_type I, size_type... Is>
        struct StaticMapper {
            using type = typename StaticMapper<Node, MakeRecord, Is...>::type[I];
            type m_data;
            template <typename Arr>
            Node &_find(Arr &&arr, size_type i) { return arr[i]; }
            template <typename Arr, typename... Args>
            Node &_find(Arr &&arr, size_type i, Args... args) { return _find(arr[i], std::forward<Args>(args)...); }
            template <typename... Args>
            Node &find(size_type i, Args... args) {
                return _find(m_data, i, std::forward<Args>(args)...);
            }
        };
        template <typename Node, size_type I, size_type... Is>
        struct StaticMapper<Node, true, I, Is...> {
            using type = typename StaticMapper<Node, true, Is...>::type[I];
            type m_data;
            std::vector<size_type> m_rec;
            Node *_base() const { return (Node *)m_data; }
            template <typename Arr>
            Node &_find(Arr &&arr, size_type i) { return arr[i]; }
            template <typename Arr, typename... Args>
            Node &_find(Arr &&arr, size_type i, Args... args) { return _find(arr[i], std::forward<Args>(args)...); }
            template <typename... Args>
            Node &find(size_type i, Args... args) {
                auto &it = _find(m_data, i, std::forward<Args>(args)...);
                if (!it.is_valid()) m_rec.push_back(&it - _base());
                return it;
            }
            void clear() {
                for (auto i : m_rec) _base()[i].reset();
                m_rec.clear();
            }
        };
        template <typename Node, size_type I>
        struct StaticMapper<Node, false, I> {
            using type = Node[I];
            type m_data;
            Node &find(size_type i) {
                return m_data[i];
            }
        };
        template <typename Node, size_type I>
        struct StaticMapper<Node, true, I> {
            using type = Node[I];
            type m_data;
            std::vector<size_type> m_rec;
            Node &find(size_type i) {
                if (!m_data[i].is_valid()) m_rec.push_back(i);
                return m_data[i];
            }
            void clear() {
                for (auto i : m_rec) m_data[i].reset();
                m_rec.clear();
            }
        };
        template <typename Node, bool MakeRecord, size_type DIM>
        struct DynamicMapper {
            size_type m_units[DIM - 1];
            std::vector<Node> m_data;
            void set(std::initializer_list<size_type> arr) {
                size_type prod = 1;
                for (auto a : arr) prod *= a;
                m_data.resize(prod);
                auto it = arr.end();
                for (size_type i = DIM - 2, j = 1; ~i; i--) m_units[i] = (j *= *--it);
            }
            template <size_type I>
            size_type _find(size_type i) const { return i; }
            template <size_type I, typename... Args>
            size_type _find(size_type i, Args... args) const { return i * m_units[I] + _find<I + 1>(std::forward<Args>(args)...); }
            template <typename... Args>
            Node &find(size_type i, Args... args) { return m_data[_find<0>(i, std::forward<Args>(args)...)]; }
        };
        template <typename Node, size_type DIM>
        struct DynamicMapper<Node, true, DIM> {
            size_type m_units[DIM - 1];
            std::vector<Node> m_data;
            std::vector<size_type> m_rec;
            void set(std::initializer_list<size_type> arr) {
                size_type prod = 1;
                for (auto a : arr) prod *= a;
                m_data.resize(prod);
                auto it = arr.end();
                for (size_type i = DIM - 2, j = 1; ~i; i--) m_units[i] = (j *= *--it);
            }
            template <size_type I>
            size_type _find(size_type i) const { return i; }
            template <size_type I, typename... Args>
            size_type _find(size_type i, Args... args) const { return i * m_units[I] + _find<I + 1>(std::forward<Args>(args)...); }
            template <typename... Args>
            Node &find(size_type i, Args... args) {
                size_type index = _find<0>(i, std::forward<Args>(args)...);
                if (!m_data[index].is_valid()) m_rec.push_back(index);
                return m_data[index];
            }
            void clear() {
                for (auto i : m_rec) m_data[i].reset();
                m_rec.clear();
            }
        };
        template <typename Node>
        struct DynamicMapper<Node, false, 1> {
            std::vector<Node> m_data;
            void set(std::initializer_list<size_type> arr) { m_data.resize(*(arr.begin())); }
            Node &find(size_type i) { return m_data[i]; }
        };
        template <typename Node>
        struct DynamicMapper<Node, true, 1> {
            std::vector<Node> m_data;
            std::vector<size_type> m_rec;
            void set(std::initializer_list<size_type> arr) { m_data.resize(*(arr.begin())); }
            Node &find(size_type i) {
                if (!m_data[i].is_valid()) m_rec.push_back(i);
                return m_data[i];
            }
            void clear() {
                for (auto i : m_rec) m_data[i].reset();
                m_rec.clear();
            }
        };
        template <typename Func, bool MakeRecord, typename Node, typename Mapper, typename Ret, typename... Args>
        struct Solver {
            Func m_func;
            Mapper m_mapper;
            Solver(Func func) : m_func(func) {}
            void set_initial_value(Args... args, Ret val) { m_mapper.find(std::forward<Args>(args)...).set(val); }
            auto query(Args... args) { return operator()(*this, std::forward<Args>(args)...); }
            const Ret &operator()(Solver<Func, MakeRecord, Node, Mapper, Ret, Args...> &, Args... args) {
                auto &it = m_mapper.find(std::forward<Args>(args)...);
                if (!it.is_valid()) it.set(m_func(*this, std::forward<Args>(args)...));
                return it.get();
            }
            void cache_clear() {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                m_mapper.clear();
            }
        };
        template <typename F>
        struct RecursiveLambdaTraits : RecursiveLambdaTraits<decltype(&F::template operator()<F>)> {};
        template <typename Cls, typename ReturnType, typename Self, typename... Args>
        struct RecursiveLambdaTraits<ReturnType (Cls::*)(Self, Args...) const> {
            using return_type = ReturnType;
            template <typename Func, bool MakeRecord, typename Node, typename Mapper>
            using type = Solver<Func, MakeRecord, Node, Mapper, ReturnType, Args...>;
        };
    }
    template <bool MakeRecord, GRIDCACHE::size_type... Args, typename Func, typename Node = GRIDCACHE::BaseNode<typename GRIDCACHE::RecursiveLambdaTraits<Func>::return_type>, typename Mapper = GRIDCACHE::StaticMapper<Node, MakeRecord, Args...>, typename Solver = typename GRIDCACHE::RecursiveLambdaTraits<Func>::template type<Func, MakeRecord, Node, Mapper>>
    auto make_StaticGridCacheSolver(Func func) -> Solver { return Solver(func); }
    template <bool MakeRecord, typename Func, typename... Args, typename Node = GRIDCACHE::BaseNode<typename GRIDCACHE::RecursiveLambdaTraits<Func>::return_type>, typename Mapper = GRIDCACHE::DynamicMapper<Node, MakeRecord, sizeof...(Args)>, typename Solver = typename GRIDCACHE::RecursiveLambdaTraits<Func>::template type<Func, MakeRecord, Node, Mapper>>
    auto make_DynamicGridCacheSolver(Func func, Args... dims) -> Solver {
        Solver sol(func);
        sol.m_mapper.set({std::forward<GRIDCACHE::size_type>(dims)...});
        return sol;
    }
}

#endif
