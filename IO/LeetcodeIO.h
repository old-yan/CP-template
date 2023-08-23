/*
最后修改:
20230121
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
            if (!s_stream) s_stream = &IO::InputHelper::getInstance();
            return *s_stream;
        }
        static void setStream(IO::InputHelper &in) { s_stream = &in; }
        static char parseChar() {
            char c = lcin.getChar_Checked();
            lcin.next();
            if (c != '\\') return c;
            c = lcin.getChar_Checked();
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
        LeetcodeInputHelper &parse(char &__c) {
            lcin >> __c;
            assert(__c == '\"');
            __c = parseChar();
            assert(lcin.getChar_Checked() == '\"');
            lcin.next();
            return *this;
        }
        LeetcodeInputHelper &parse(double &__num) {
            lcin >> __num;
            return *this;
        }
        LeetcodeInputHelper &parse(int &__num) {
            lcin >> __num;
            return *this;
        }
        LeetcodeInputHelper &parse(int64_t &__num) {
            lcin >> __num;
            return *this;
        }
        LeetcodeInputHelper &parse(ListNode *&__node) {
            std::vector<int> items;
            parse(items);
            ListNode *dummyRoot = new ListNode(0), *ptr = dummyRoot;
            for (int item : items) ptr->next = new ListNode(item), ptr = ptr->next;
            __node = dummyRoot->next;
            delete dummyRoot;
            return *this;
        }
        LeetcodeInputHelper &parse(std::string &__s) {
            __s.clear();
            char ch;
            lcin >> ch;
            assert(ch == '\"');
            while (lcin.getChar_Checked() != '\"') __s += parseChar();
            lcin.next();
            return *this;
        }
        LeetcodeInputHelper &parse(TreeNode *&__node) {
            __node = nullptr;
            char ch;
            std::queue<TreeNode *> nodeQueue;
            while (true) {
                while (lcin.isBlank(lcin.getChar_Checked())) lcin.next();
                ch = lcin.getChar_Checked(), lcin.next();
                if (ch == ']') break;
                while (lcin.isBlank(lcin.getChar_Checked())) lcin.next();
                if (nodeQueue.empty()) {
                    if (lcin.getChar_Unchecked() == 'n') {
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'n');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'u');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'l');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'l');
                    } else if (lcin.getChar_Unchecked() == ']') {
                        lcin.next();
                        break;
                    } else {
                        __node = new TreeNode;
                        lcin >> __node->val;
                        nodeQueue.push(__node);
                    }
                } else {
                    if (lcin.getChar_Checked() == 'n') {
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'n');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'u');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'l');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'l');
                    } else {
                        TreeNode *ptr = new TreeNode;
                        lcin >> ptr->val;
                        nodeQueue.front()->left = ptr;
                        nodeQueue.push(ptr);
                    }
                    while (lcin.isBlank(lcin.getChar_Checked())) lcin.next();
                    ch = lcin.getChar_Checked(), lcin.next();
                    if (ch == ']') break;
                    while (lcin.isBlank(lcin.getChar_Checked())) lcin.next();
                    if (lcin.getChar_Checked() == 'n') {
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'n');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'u');
                        ch = lcin.getChar_Checked(), lcin.next();
                        assert(ch == 'l');
                        ch = lcin.getChar_Checked(), lcin.next();
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
        template <typename _Tp>
        LeetcodeInputHelper &parse(std::vector<_Tp> &__vec) {
            __vec.clear();
            while (true) {
                char ch;
                lcin >> ch;
                if (!lcin || ch == ']') break;
                while (lcin.isBlank(lcin.getChar_Checked())) lcin.next();
                if (lcin.getChar_Checked() == ']') {
                    lcin.next();
                    break;
                }
                _Tp item;
                parse(item);
                __vec.push_back(std::move(item));
            }
            return *this;
        }
    };
    struct LeetcodeOutputHelper {
        static IO::OutputHelper *s_stream;
        static IO::OutputHelper &getStream() {
            if (!s_stream) s_stream = &IO::OutputHelper::getInstance();
            return *s_stream;
        }
        static void setStream(IO::OutputHelper &out) { s_stream = &out; }
        LeetcodeOutputHelper() { lcout.precision(5); }
        LeetcodeOutputHelper &write(bool __ret) {
            lcout << (__ret ? "true" : "false");
            return *this;
        }
        LeetcodeOutputHelper &write(double __ret) {
            lcout << __ret;
            return *this;
        }
        LeetcodeOutputHelper &write(int __ret) {
            lcout << __ret;
            return *this;
        }
        LeetcodeOutputHelper &write(int64_t __ret) {
            lcout << __ret;
            return *this;
        }
        LeetcodeOutputHelper &write(ListNode *__ret) {
            lcout << "[";
            std::string result;
            while (__ret) {
                lcout << __ret->val;
                __ret = __ret->next;
                if (__ret) lcout << ", ";
            }
            lcout << "]";
            return *this;
        }
        LeetcodeOutputHelper &write(const std::string &__ret) {
            lcout << "\"" << __ret << "\"";
            return *this;
        }
        LeetcodeOutputHelper &write(TreeNode *__ret) {
            lcout << '[';
            std::queue<TreeNode *> q;
            int number = 0;
            if (__ret) {
                q.push(__ret);
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
        LeetcodeOutputHelper &write(const std::vector<T> &__ret) {
            lcout << '[';
            for (int i = 0; i < __ret.size(); i++) {
                if (i > 0) lcout << ", ";
                write(__ret[i]);
            }
            lcout << ']';
            return *this;
        }
    };
    namespace index_sequence_utils {
        template <typename _Tp, _Tp... _Idx>
        struct integer_sequence {
            using inner_type = integer_sequence<_Tp, _Idx...>;
            using value_type = _Tp;
            static constexpr size_t size() noexcept { return sizeof...(_Idx); }
        };
        template <size_t... _Idx>
        using index_sequence = integer_sequence<size_t, _Idx...>;
        template <typename _IndexSequence>
        struct extended_index_sequence;
        template <size_t... _Idx>
        struct extended_index_sequence<index_sequence<_Idx...>> : index_sequence<_Idx..., sizeof...(_Idx)> {};
        template <size_t _N>
        struct _make_index_sequence : extended_index_sequence<typename _make_index_sequence<_N - 1>::inner_type> {};
        template <>
        struct _make_index_sequence<0> : index_sequence<> {};
        template <size_t _N>
        using make_index_sequence = typename _make_index_sequence<_N>::inner_type;
        template <typename _Tuple, size_t... _Is>
        void apply_func1(_Tuple &&__tuple, index_sequence<_Is...>) {
            std::initializer_list<int>{(LeetcodeInputHelper().parse(std::get<_Is>(__tuple)), 0)...};
        }
        template <typename _Tuple, size_t... _Is>
        void apply_func2(_Tuple &&__tuple, char &__ch, index_sequence<_Is...>) {
            std::initializer_list<int>{(LeetcodeInputHelper().parse(std::get<_Is>(__tuple)), lcin >> __ch, 0)...};
        }
    }
    template <typename _Tp>
    struct _LeetcodeFactoryBase {
        virtual _Tp *operator()() { return nullptr; }
    };
    template <bool _IsSolution, typename _Tp, typename... _Args>
    struct _LeetcodeConstructorFactory : _LeetcodeFactoryBase<_Tp> {
        template <size_t... __Is>
        _Tp *operator()(std::tuple<typename std::decay<_Args>::type...> &__args, index_sequence_utils::index_sequence<__Is...>) {
            if constexpr (!_IsSolution) lcout << "null";
            return new _Tp(std::get<__Is>(__args)...);
        }
        _Tp *operator()() override {
            std::tuple<typename std::decay<_Args>::type...> m_argument;
            char ch;
            if constexpr (_IsSolution) {
                index_sequence_utils::apply_func1(m_argument, index_sequence_utils::make_index_sequence<sizeof...(_Args)>{});
            } else {
                lcin >> ch;
                assert(ch == '[');
                index_sequence_utils::apply_func2(m_argument, ch, index_sequence_utils::make_index_sequence<sizeof...(_Args)>{});
                if constexpr (sizeof...(_Args) == 0) lcin >> ch;
                assert(ch == ']');
            }
            return (*this)(m_argument, index_sequence_utils::make_index_sequence<sizeof...(_Args)>{});
        }
    };
    template <typename _Tp>
    struct _LeetcodeMemberFunctionBase {
        virtual void operator()(_Tp *__obj) {}
    };
    template <bool _IsSolution, typename _Tp, typename _R, typename... _Args>
    struct _LeetcodeMemberFunction : public _LeetcodeMemberFunctionBase<_Tp> {
        _R(_Tp::*m_func)
        (_Args...);
        _LeetcodeMemberFunction(_R (_Tp::*func)(_Args...)) : m_func(func) {}
        template <size_t... __Is>
        void exec(_Tp *__obj, std::tuple<typename std::decay<_Args>::type...> &__args, index_sequence_utils::index_sequence<__Is...>) {
            if constexpr (std::is_void<_R>::value) {
                if constexpr (!_IsSolution) lcout << ",null";
                (__obj->*m_func)(std::get<__Is>(__args)...);
            } else {
                if constexpr (!_IsSolution) lcout << ",";
                LeetcodeOutputHelper().write(((__obj->*m_func))(std::get<__Is>(__args)...));
                if constexpr (_IsSolution) (lcout << endl).flush();
            }
        }
        void operator()(_Tp *__obj) override {
            std::tuple<typename std::decay<_Args>::type...> m_argument;
            char ch;
            if constexpr (_IsSolution) {
                index_sequence_utils::apply_func1(m_argument, index_sequence_utils::make_index_sequence<sizeof...(_Args)>{});
            } else {
                lcin >> ch;
                assert(ch == '[');
                index_sequence_utils::apply_func2(m_argument, ch, index_sequence_utils::make_index_sequence<sizeof...(_Args)>{});
                if constexpr (sizeof...(_Args) == 0) lcin >> ch;
                assert(ch == ']');
            }
            exec(__obj, m_argument, index_sequence_utils::make_index_sequence<sizeof...(_Args)>{});
        }
    };
    template <typename _Tp>
    struct LeetcodeExecutor {
        _LeetcodeFactoryBase<_Tp> *m_constructor;
        std::unordered_map<std::string, _LeetcodeMemberFunctionBase<_Tp> *> m_funcMap;
        _Tp *m_obj = nullptr;
        std::vector<std::string> m_commands;
        int m_cursor;
        template <typename... _Args>
        void registerConstructor_ofSolution() {
            m_constructor = new _LeetcodeConstructorFactory<true, _Tp, _Args...>;
        }
        template <typename... _Args>
        void registerConstructor_ofClass() {
            m_constructor = new _LeetcodeConstructorFactory<false, _Tp, _Args...>;
        }
        template <typename _R, typename... _Args>
        void registerMemberFunction_ofSolution(const char *__name, _R (_Tp::*__func)(_Args...)) {
            m_funcMap[__name] = new _LeetcodeMemberFunction<true, _Tp, _R, _Args...>(__func);
        }
        template <typename _R, typename... _Args>
        void registerMemberFunction_ofClass(const char *__name, _R (_Tp::*__func)(_Args...)) {
            m_funcMap[__name] = new _LeetcodeMemberFunction<false, _Tp, _R, _Args...>(__func);
        }
        void construct() {
            m_obj = (*m_constructor)();
        }
        void constructSolution() {
            while (lcin.isBlank(lcin.getChar_Checked())) lcin.next();
            if (lcin.getChar_Checked() == EOF) exit(0);
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