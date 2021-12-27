#ifndef __OY_LEETCODEIO__
#define __OY_LEETCODEIO__

#define REGISTER_CONSTRUCTOR_CLASS(class, ...) \
    OY::LeetcodeExecutor<class> executor;      \
    executor.registerConstructor_ofClass<__VA_ARGS__>();
#define REGISTER_MEMBERFUNCTION_CLASS(...) _REGISTER_MEMBERFUNCTIONS(NUM_ARGS(__VA_ARGS__), ##__VA_ARGS__)
#define REGISTER_CONSTRUCTOR_SOLUTION        \
    OY::LeetcodeExecutor<Solution> executor; \
    executor.registerConstructor_ofSolution<>();
#define REGISTER_MEMBERFUNCTION_SOLUTION(func) \
    executor.registerMemberFunction_ofSolution(#func, &Solution ::func);

#define _NUM_ARGS(X7, X6, X5, X4, X3, X2, X1, N, ...) N
#define NUM_ARGS(...) _NUM_ARGS(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define _REGISTER_MEMBERFUNCTION_CLASS1(class, func) \
    executor.registerMemberFunction_ofClass(#func, &class ::func);
#define _REGISTER_MEMBERFUNCTION_CLASS2(class, func, ...)          \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS1(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS3(class, func, ...)          \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS2(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS4(class, func, ...)          \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS3(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS5(class, func, ...)          \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS4(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASS6(class, func, ...)          \
    executor.registerMemberFunction_ofClass(#func, &class ::func); \
    _REGISTER_MEMBERFUNCTION_CLASS5(class, __VA_ARGS__)
#define _REGISTER_MEMBERFUNCTION_CLASSN(N, ...) _REGISTER_MEMBERFUNCTION_CLASS##N(__VA_ARGS__)
#define _REGISTER_MEMBERFUNCTIONS(N, ...) _REGISTER_MEMBERFUNCTION_CLASSN(N, ##__VA_ARGS__)

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
#include <unordered_set>
#include <unordered_map>
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
    class LeetcodeInputHelper {
    public:
        LeetcodeInputHelper &operator>>(char &ret) {
            char ch;
            cin >> ch;
            assert(ch == '\"');
            ch = getchar();
            if (ch == '\\') {
                ch = getchar();
                switch (ch) {
                    case '\"': ret = '\"'; break;
                    case '/': ret = '/'; break;
                    case '\\': ret = '\\'; break;
                    case 'b': ret = '\b'; break;
                    case 'f': ret = '\f'; break;
                    case 'r': ret = '\r'; break;
                    case 'n': ret = '\n'; break;
                    case 't': ret = '\t'; break;
                    default: break;
                }
            } else
                ret = ch;
            getchar();
            return *this;
        }
        LeetcodeInputHelper &operator>>(double &ret) {
            cin >> ret;
            return *this;
        }
        LeetcodeInputHelper &operator>>(int &ret) {
            cin >> ret;
            return *this;
        }
        LeetcodeInputHelper &operator>>(int64_t &ret) {
            cin >> ret;
            return *this;
        }
        LeetcodeInputHelper &operator>>(ListNode *&ret) {
            std::vector<int> items;
            *this >> items;
            ListNode *dummyRoot = new ListNode(0);
            ListNode *ptr = dummyRoot;
            for (int item : items) {
                ptr->next = new ListNode(item);
                ptr = ptr->next;
            }
            ret = dummyRoot->next;
            delete dummyRoot;
            return *this;
        }
        LeetcodeInputHelper &operator>>(std::string &ret) {
            ret.clear();
            char ch;
            cin >> ch;
            assert(ch == '\"');
            while ((ch = getchar()) != '\"') {
                if (ch == '\\') {
                    ch = getchar();
                    switch (ch) {
                        case '\"': ret += '\"'; break;
                        case '/': ret += '/'; break;
                        case '\\': ret += '\\'; break;
                        case 'b': ret += '\b'; break;
                        case 'f': ret += '\f'; break;
                        case 'r': ret += '\r'; break;
                        case 'n': ret += '\n'; break;
                        case 't': ret += '\t'; break;
                        default: break;
                    }
                } else
                    ret += ch;
            }
            return *this;
        }
        LeetcodeInputHelper &operator>>(TreeNode *&ret) {
            ret = nullptr;
            char ch;
            std::queue<TreeNode *> nodeQueue;
            while (true) {
                while (OY::inputHelper::isBlank(cin.nextChar())) cin.popNext();
                if (cin.getChar() == ']') {
                    break;
                }
                if (nodeQueue.empty()) {
                    ret = new TreeNode;
                    cin >> ret->val;
                    nodeQueue.push(ret);
                } else {
                    if (cin.nextChar() == 'n') {
                        cin.popNext();
                        assert(getchar() == 'u');
                        assert(getchar() == 'l');
                        assert(getchar() == 'l');
                    } else {
                        TreeNode *ptr = new TreeNode;
                        cin >> ptr->val;
                        nodeQueue.front()->left = ptr;
                        nodeQueue.push(ptr);
                    }
                    while (OY::inputHelper::isBlank(cin.nextChar())) cin.popNext();
                    if (cin.getChar() == ']') {
                        break;
                    }
                    if (cin.nextChar() == 'n') {
                        cin.popNext();
                        assert(getchar() == 'u');
                        assert(getchar() == 'l');
                        assert(getchar() == 'l');
                    } else {
                        TreeNode *ptr = new TreeNode;
                        cin >> ptr->val;
                        nodeQueue.front()->right = ptr;
                        nodeQueue.push(ptr);
                    }
                    nodeQueue.pop();
                }
            }
            return *this;
        }
        template <typename T>
        LeetcodeInputHelper &operator>>(std::vector<T> &ret) {
            ret.clear();
            while (true) {
                while (OY::inputHelper::isBlank(cin.nextChar())) cin.popNext();
                if (!cin) break;
                if (cin.getChar() == ']') break;
                T item;
                *this >> item;
                ret.push_back(item);
            }
            return *this;
        }
    };
    class LeetcodeOutputHelper {
    public:
        LeetcodeOutputHelper() {
            cout.precision(5);
        }
        LeetcodeOutputHelper &operator<<(bool ret) {
            cout << (ret ? "true" : "false");
            return *this;
        }
        LeetcodeOutputHelper &operator<<(double ret) {
            cout << ret;
            return *this;
        }
        LeetcodeOutputHelper &operator<<(int ret) {
            cout << ret;
            return *this;
        }
        LeetcodeOutputHelper &operator<<(int64_t ret) {
            cout << ret;
            return *this;
        }
        LeetcodeOutputHelper &operator<<(ListNode *ret) {
            cout << "[";
            std::string result;
            while (ret) {
                cout << ret->val;
                ret = ret->next;
                if (ret) cout << ", ";
            }
            cout << "]";
            return *this;
        }
        LeetcodeOutputHelper &operator<<(const std::string &ret) {
            cout << "\"" << ret << "\"";
            return *this;
        }
        LeetcodeOutputHelper &operator<<(TreeNode *ret) {
            cout << '[';
            std::queue<TreeNode *> q;
            if (ret) q.push(ret);
            while (!q.empty()) {
                TreeNode *node = q.front();
                q.pop();

                if (node == nullptr) {
                    cout << "null, ";
                } else {
                    cout << node->val << ", ";
                    q.push(node->left);
                    q.push(node->right);
                }
            }
            cout << ']';
            return *this;
        }
        template <typename T>
        LeetcodeOutputHelper &operator<<(const std::vector<T> &ret) {
            cout << '[';
            for (int i = 0; i < ret.size(); i++) {
                if (i > 0) cout << ", ";
                *this << ret[i];
            }
            cout << ']';
            return *this;
        }
    };
    template <typename _Tp>
    struct _LeetcodeFactoryBase {
        virtual _Tp *operator()() {
            return nullptr;
        }
    };
    template <bool _IsSolution, typename _Tp, typename... _Args>
    struct _LeetcodeConstructorFactory : _LeetcodeFactoryBase<_Tp> {
        template <size_t... __Is>
        _Tp *operator()(std::tuple<std::decay_t<_Args>...> &__args, std::index_sequence<__Is...>) {
            if constexpr (!_IsSolution) cout << "null";
            return new _Tp(std::get<__Is>(__args)...);
        }
        _Tp *operator()() override {
            std::tuple<std::decay_t<_Args>...> m_argument;
            char ch;
            if constexpr (_IsSolution) {
                std::apply([&](auto &&...args) { ((LeetcodeInputHelper() >> args), ...); }, m_argument);
            } else {
                cin >> ch;
                assert(ch == '[');
                std::apply([&](auto &&...args) { ((LeetcodeInputHelper() >> args, cin >> ch), ...); }, m_argument);
                if constexpr (sizeof...(_Args) == 0) cin >> ch;
                assert(ch == ']');
            }
            return (*this)(m_argument, std::make_index_sequence<sizeof...(_Args)>());
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
        void exec(_Tp *__obj, std::tuple<std::decay_t<_Args>...> &__args, std::index_sequence<__Is...>) {
            if constexpr (std::is_void_v<_R>) {
                if constexpr (!_IsSolution) cout << ",null";
                (__obj->*m_func)(std::get<__Is>(__args)...);
            } else {
                if constexpr (!_IsSolution) cout << ",";
                LeetcodeOutputHelper() << ((__obj->*m_func))(std::get<__Is>(__args)...);
                if constexpr (_IsSolution) cout << endl;
            }
        }
        void operator()(_Tp *__obj) override {
            std::tuple<std::decay_t<_Args>...> m_argument;
            char ch;
            if constexpr (_IsSolution) {
                std::apply([&](auto &&...args) { ((LeetcodeInputHelper() >> args), ...); }, m_argument);
            } else {
                cin >> ch;
                assert(ch == '[');
                std::apply([&](auto &&...args) { ((LeetcodeInputHelper() >> args, cin >> ch), ...); }, m_argument);
                if constexpr (sizeof...(_Args) == 0) cin >> ch;
                assert(ch == ']');
            }
            exec(__obj, m_argument, std::make_index_sequence<sizeof...(_Args)>());
        }
    };
    template <typename _Tp>
    class LeetcodeExecutor {
        _LeetcodeFactoryBase<_Tp> *m_constructor;
        std::unordered_map<std::string, _LeetcodeMemberFunctionBase<_Tp> *> m_funcMap;
        _Tp *m_obj = nullptr;
        std::vector<std::string> m_commands;
        int m_cursor;

    public:
        template <typename... _Args>
        void registerConstructor_ofSolution() {
            m_constructor = new _LeetcodeConstructorFactory<true, _Tp, _Args...>;
        }
        template <typename... _Args>
        void registerConstructor_ofClass() {
            m_constructor = new _LeetcodeConstructorFactory<false, _Tp, _Args...>;
        }
        template <typename R, typename... _Args>
        void registerMemberFunction_ofSolution(const char *name, R (_Tp::*func)(_Args...)) {
            m_funcMap[name] = new _LeetcodeMemberFunction<true, _Tp, R, _Args...>(func);
        }
        template <typename R, typename... _Args>
        void registerMemberFunction_ofClass(const char *name, R (_Tp::*func)(_Args...)) {
            m_funcMap[name] = new _LeetcodeMemberFunction<false, _Tp, R, _Args...>(func);
        }
        void construct() {
            m_obj = (*m_constructor)();
        }
        void constructSolution() {
            char ch;
            while (OY::inputHelper::isBlank(ch = cin.nextChar())) {
                cin.popNext();
            }
            if (!cin) exit(0);
            construct();
        }
        void constructClass() {
            char ch;
            if (m_obj) {
                cin >> ch;
                assert(ch == ']');
                cout << "]\n";
            }
            LeetcodeInputHelper() >> m_commands;
            if (!cin) exit(0);
            m_cursor = 0;
            cin >> ch;
            assert(ch == '[');
            cout << '[';
            construct();
            m_cursor++;
        }
        void executeSolution() {
            assert(m_funcMap.size() == 1);
            (*(m_funcMap.begin()->second))(m_obj);
        }
        void executeClass() {
            char ch;
            cin >> ch;
            assert(ch == ',');
            (*m_funcMap[m_commands[m_cursor++]])(m_obj);
        }
        explicit operator bool() {
            return m_cursor < m_commands.size();
        }
    };
}

#endif