/*
最后修改:
20230916
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/

#ifndef __OY_LEETCODEIO__
#define __OY_LEETCODEIO__

#define MSVC_EXPAND(...) __VA_ARGS__
#define _NUM_ARGS(X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...) N
#define NUM_ARGS(...) MSVC_EXPAND(_NUM_ARGS(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define __REGISTER_MEMBERFUNCTION_CLASS1(class, func) executor.registerMemberFunction_ofClass(#func, &class ::func);
#define _REGISTER_MEMBERFUNCTION_CLASS1(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS1(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS2(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS1(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS2(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS2(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS3(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS2(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS3(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS3(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS4(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS3(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS4(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS4(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS5(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS4(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS5(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS5(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS6(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS5(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS6(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS6(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS7(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS6(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS7(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS7(__VA_ARGS__))
#define __REGISTER_MEMBERFUNCTION_CLASS8(class, func, ...)         \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS7(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS8(...) MSVC_EXPAND(__REGISTER_MEMBERFUNCTION_CLASS8(__VA_ARGS__))
#define REGISTER_CONSTRUCTOR_CLASS(class, ...) \
    OY::LeetcodeExecutor<class> executor;      \
    executor.registerConstructor_ofClass<__VA_ARGS__>();
#define REGISTER_CONSTRUCTOR_SOLUTION        \
    OY::LeetcodeExecutor<Solution> executor; \
    executor.registerConstructor_ofSolution<>();
#define _REGISTER_MEMBERFUNCTION_CLASSN(N, ...) _REGISTER_MEMBERFUNCTION_CLASS##N(__VA_ARGS__)
#define _REGISTER_MEMBERFUNCTIONS(N, ...) MSVC_EXPAND(_REGISTER_MEMBERFUNCTION_CLASSN(N, ##__VA_ARGS__))
#define REGISTER_MEMBERFUNCTION_CLASS(...) _REGISTER_MEMBERFUNCTIONS(NUM_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define REGISTER_MEMBERFUNCTION_SOLUTION(func) \
    executor.registerMemberFunction_ofSolution(#func, &Solution ::func);

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
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "FastIO.h"

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(-1), next(NULL) {}
    ListNode(int x) : val(x), next(NULL) {}
    ListNode(int x, ListNode *_next) : val(x), next(_next) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(NULL), right(NULL) {}
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    TreeNode(int x, TreeNode *_left, TreeNode *_right) : val(x), left(_left), right(_right) {}
};

namespace OY {
#define lcin LeetcodeInputHelper::getStream()
#define lcout LeetcodeOutputHelper::getStream()
    struct LeetcodeInputHelper {
        static IO::InputHelper *s_stream;
        static IO::InputHelper &getStream() {
            if (!s_stream) s_stream = &IO::InputHelper::get_instance();
            return *s_stream;
        }
        static void setStream(IO::InputHelper &in) { s_stream = &in; }
        static char parseChar() {
            char c = lcin.getchar_checked();
            lcin.next();
            if (c != '\\') return c;
            c = lcin.getchar_checked();
            lcin.next();
            switch (c) {
                case 'b': return '\b';
                case 'f': return '\f';
                case 'r': return '\r';
                case 'n': return '\n';
                case 't': return '\t';
                default: return c;
            }
        }
        LeetcodeInputHelper &parse(char &c) {
            lcin >> c;
            assert(c == '\"');
            c = parseChar();
            assert(lcin.getchar_checked() == '\"');
            lcin.next();
            return *this;
        }
        LeetcodeInputHelper &parse(double &num) {
            lcin >> num;
            return *this;
        }
        LeetcodeInputHelper &parse(int &num) {
            lcin >> num;
            return *this;
        }
        LeetcodeInputHelper &parse(int64_t &num) {
            lcin >> num;
            return *this;
        }
        LeetcodeInputHelper &parse(ListNode *&node) {
            std::vector<int> items;
            parse(items);
            ListNode *dummyRoot = new ListNode(0), *ptr = dummyRoot;
            for (int item : items) ptr->next = new ListNode(item), ptr = ptr->next;
            node = dummyRoot->next;
            delete dummyRoot;
            return *this;
        }
        LeetcodeInputHelper &parse(std::string &s) {
            s.clear();
            char ch;
            lcin >> ch;
            assert(ch == '\"');
            while (lcin.getchar_checked() != '\"') s += parseChar();
            lcin.next();
            return *this;
        }
        LeetcodeInputHelper &parse(TreeNode *&node) {
            node = nullptr;
            char ch;
            std::queue<TreeNode *> nodeQueue;
            while (true) {
                while (lcin.is_blank(lcin.getchar_checked())) lcin.next();
                ch = lcin.getchar_checked(), lcin.next();
                if (ch == ']') break;
                while (lcin.is_blank(lcin.getchar_checked())) lcin.next();
                if (nodeQueue.empty()) {
                    if (lcin.getchar_unchecked() == 'n') {
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'n');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'u');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'l');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'l');
                    } else if (lcin.getchar_unchecked() == ']') {
                        lcin.next();
                        break;
                    } else {
                        node = new TreeNode;
                        lcin >> node->val;
                        nodeQueue.push(node);
                    }
                } else {
                    if (lcin.getchar_checked() == 'n') {
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'n');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'u');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'l');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'l');
                    } else {
                        TreeNode *ptr = new TreeNode;
                        lcin >> ptr->val;
                        nodeQueue.front()->left = ptr;
                        nodeQueue.push(ptr);
                    }
                    while (lcin.is_blank(lcin.getchar_checked())) lcin.next();
                    ch = lcin.getchar_checked(), lcin.next();
                    if (ch == ']') break;
                    while (lcin.is_blank(lcin.getchar_checked())) lcin.next();
                    if (lcin.getchar_checked() == 'n') {
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'n');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'u');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'l');
                        ch = lcin.getchar_checked(), lcin.next();
                        assert(ch == 'l');
                    } else {
                        TreeNode *ptr = new TreeNode;
                        lcin >> ptr->val;
                        nodeQueue.front()->right = ptr;
                        nodeQueue.push(ptr);
                    }
                    nodeQueue.pop();
                }
            }
            return *this;
        }
        template <typename Tp>
        LeetcodeInputHelper &parse(std::vector<Tp> &vec) {
            vec.clear();
            while (true) {
                char ch;
                lcin >> ch;
                if (!lcin || ch == ']') break;
                while (lcin.is_blank(lcin.getchar_checked())) lcin.next();
                if (lcin.getchar_checked() == ']') {
                    lcin.next();
                    break;
                }
                Tp item;
                parse(item);
                vec.push_back(std::move(item));
            }
            return *this;
        }
    };
    struct LeetcodeOutputHelper {
        static IO::OutputHelper *s_stream;
        static IO::OutputHelper &getStream() {
            if (!s_stream) s_stream = &IO::OutputHelper::get_instance();
            return *s_stream;
        }
        static void setStream(IO::OutputHelper &out) { s_stream = &out; }
        LeetcodeOutputHelper() { lcout.precision(5); }
        LeetcodeOutputHelper &write(bool ret) {
            lcout << (ret ? "true" : "false");
            return *this;
        }
        LeetcodeOutputHelper &write(double ret) {
            lcout << ret;
            return *this;
        }
        LeetcodeOutputHelper &write(int ret) {
            lcout << ret;
            return *this;
        }
        LeetcodeOutputHelper &write(int64_t ret) {
            lcout << ret;
            return *this;
        }
        LeetcodeOutputHelper &write(ListNode *ret) {
            lcout << "[";
            std::string result;
            while (ret) {
                lcout << ret->val;
                ret = ret->next;
                if (ret) lcout << ", ";
            }
            lcout << "]";
            return *this;
        }
        LeetcodeOutputHelper &write(const std::string &ret) {
            lcout << "\"" << ret << "\"";
            return *this;
        }
        LeetcodeOutputHelper &write(TreeNode *ret) {
            lcout << '[';
            std::queue<TreeNode *> q;
            int number = 0;
            if (ret) {
                q.push(ret);
                number++;
            }
            while (number) {
                TreeNode *node = q.front();
                q.pop();
                if (node) number--;
                if (node == nullptr) {
                    lcout << "null";
                } else {
                    lcout << node->val;
                    q.push(node->left);
                    if (node->left) number++;
                    q.push(node->right);
                    if (node->right) number++;
                }
                if (number) lcout << ", ";
            }
            lcout << ']';
            return *this;
        }
        template <typename T>
        LeetcodeOutputHelper &write(const std::vector<T> &ret) {
            lcout << '[';
            for (int i = 0; i < ret.size(); i++) {
                if (i > 0) lcout << ", ";
                write(ret[i]);
            }
            lcout << ']';
            return *this;
        }
    };
    namespace index_sequence_utils {
        template <typename Tp, Tp... Idx>
        struct integer_sequence {
            using inner_type = integer_sequence<Tp, Idx...>;
            using value_type = Tp;
            static constexpr size_t size() noexcept { return sizeof...(Idx); }
        };
        template <size_t... Idx>
        using index_sequence = integer_sequence<size_t, Idx...>;
        template <typename IndexSequence>
        struct extended_index_sequence;
        template <size_t... Idx>
        struct extended_index_sequence<index_sequence<Idx...>> : index_sequence<Idx..., sizeof...(Idx)> {};
        template <size_t N>
        struct _make_index_sequence : extended_index_sequence<typename _make_index_sequence<N - 1>::inner_type> {};
        template <>
        struct _make_index_sequence<0> : index_sequence<> {};
        template <size_t N>
        using make_index_sequence = typename _make_index_sequence<N>::inner_type;
        template <typename Tuple, size_t... Is>
        void apply_func1(Tuple &&tuple, index_sequence<Is...>) {
            (void)std::initializer_list<int>{(LeetcodeInputHelper().parse(std::get<Is>(tuple)), 0)...};
        }
        template <typename Tuple, size_t... Is>
        void apply_func2(Tuple &&tuple, char &ch, index_sequence<Is...>) {
            (void)std::initializer_list<int>{(LeetcodeInputHelper().parse(std::get<Is>(tuple)), lcin >> ch, 0)...};
        }
    }
    template <typename Tp>
    struct LeetcodeFactoryBase {
        virtual Tp *operator()() { return nullptr; }
    };
    template <bool IsSolution, typename Tp, typename... Args>
    struct LeetcodeConstructorFactory : LeetcodeFactoryBase<Tp> {
        template <size_t... Is>
        Tp *operator()(std::tuple<typename std::decay<Args>::type...> &args, index_sequence_utils::index_sequence<Is...>) {
            if constexpr (!IsSolution) lcout << "null";
            return new Tp(std::get<Is>(args)...);
        }
        Tp *operator()() override {
            std::tuple<typename std::decay<Args>::type...> m_argument;
            char ch;
            if constexpr (IsSolution) {
                index_sequence_utils::apply_func1(m_argument, index_sequence_utils::make_index_sequence<sizeof...(Args)>{});
            } else {
                lcin >> ch;
                assert(ch == '[');
                index_sequence_utils::apply_func2(m_argument, ch, index_sequence_utils::make_index_sequence<sizeof...(Args)>{});
                if constexpr (sizeof...(Args) == 0) lcin >> ch;
                assert(ch == ']');
            }
            return (*this)(m_argument, index_sequence_utils::make_index_sequence<sizeof...(Args)>{});
        }
    };
    template <typename Tp>
    struct LeetcodeMemberFunctionBase {
        virtual void operator()(Tp *obj) {}
    };
    template <bool IsSolution, typename Tp, typename R, typename... Args>
    struct LeetcodeMemberFunction : public LeetcodeMemberFunctionBase<Tp> {
        R(Tp::*m_func)
        (Args...);
        LeetcodeMemberFunction(R (Tp::*func)(Args...)) : m_func(func) {}
        template <size_t... Is>
        void exec(Tp *obj, std::tuple<typename std::decay<Args>::type...> &args, index_sequence_utils::index_sequence<Is...>) {
            if constexpr (std::is_void<R>::value) {
                if constexpr (!IsSolution) lcout << ",null";
                (obj->*m_func)(std::get<Is>(args)...);
            } else {
                if constexpr (!IsSolution) lcout << ",";
                LeetcodeOutputHelper().write(((obj->*m_func))(std::get<Is>(args)...));
                if constexpr (IsSolution) (lcout << endl).flush();
            }
        }
        void operator()(Tp *obj) override {
            std::tuple<typename std::decay<Args>::type...> m_argument;
            char ch;
            if constexpr (IsSolution) {
                index_sequence_utils::apply_func1(m_argument, index_sequence_utils::make_index_sequence<sizeof...(Args)>{});
            } else {
                lcin >> ch;
                assert(ch == '[');
                index_sequence_utils::apply_func2(m_argument, ch, index_sequence_utils::make_index_sequence<sizeof...(Args)>{});
                if constexpr (sizeof...(Args) == 0) lcin >> ch;
                assert(ch == ']');
            }
            exec(obj, m_argument, index_sequence_utils::make_index_sequence<sizeof...(Args)>{});
        }
    };
    template <typename Tp>
    struct LeetcodeExecutor {
        LeetcodeFactoryBase<Tp> *m_constructor;
        std::unordered_map<std::string, LeetcodeMemberFunctionBase<Tp> *> m_funcMap;
        Tp *m_obj = nullptr;
        std::vector<std::string> m_commands;
        int m_cursor;
        template <typename... Args>
        void registerConstructor_ofSolution() {
            m_constructor = new LeetcodeConstructorFactory<true, Tp, Args...>;
        }
        template <typename... Args>
        void registerConstructor_ofClass() {
            m_constructor = new LeetcodeConstructorFactory<false, Tp, Args...>;
        }
        template <typename R, typename... Args>
        void registerMemberFunction_ofSolution(const char *name, R (Tp::*func)(Args...)) {
            m_funcMap[name] = new LeetcodeMemberFunction<true, Tp, R, Args...>(func);
        }
        template <typename R, typename... Args>
        void registerMemberFunction_ofClass(const char *name, R (Tp::*func)(Args...)) {
            m_funcMap[name] = new LeetcodeMemberFunction<false, Tp, R, Args...>(func);
        }
        void construct() {
            m_obj = (*m_constructor)();
        }
        void constructSolution() {
            while (lcin.is_blank(lcin.getchar_checked())) lcin.next();
            if (lcin.getchar_checked() == EOF) exit(0);
            construct();
        }
        void constructClass() {
            char ch;
            if (m_obj) {
                lcin >> ch;
                assert(ch == ']');
                (lcout << "]\n").flush();
            }
            LeetcodeInputHelper().parse(m_commands);
            if (!lcin) exit(0);
            m_cursor = 0;
            lcin >> ch;
            assert(ch == '[');
            lcout << '[';
            construct();
            m_cursor++;
        }
        void executeSolution() {
            assert(m_funcMap.size() == 1);
            (*(m_funcMap.begin()->second))(m_obj);
        }
        void executeClass() {
            char ch;
            lcin >> ch;
            assert(ch == ',');
            (*m_funcMap[m_commands[m_cursor++]])(m_obj);
        }
        explicit operator bool() {
            return m_cursor < m_commands.size();
        }
    };
    IO::InputHelper *LeetcodeInputHelper::s_stream = nullptr;
    IO::OutputHelper *LeetcodeOutputHelper::s_stream = nullptr;
}

#endif