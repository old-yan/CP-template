/*
最后修改:
20231112
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_PERSISTENTBITRIE__
#define __OY_PERSISTENTBITRIE__

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <numeric>

namespace OY {
    namespace PerBiTrie {
        using size_type = uint32_t;
        struct Ignore {};
        struct BaseQueryJudge {
            template <typename Iterator>
            bool operator()(Iterator it) const { return it.m_index; }
        };
        struct BaseEraseJudger {
            template <typename Iterator>
            bool operator()(Iterator it) const { return !it.child(0) && !it.child(1); }
        };
        struct BaseInfo {};
        template <typename Tp, size_type L>
        struct NumberInteration {
            Tp m_number;
            struct NumberInterator {
                Tp m_number;
                size_type m_index;
                NumberInterator(Tp number, size_type index) : m_number(number), m_index(index) {}
                NumberInterator &operator++() {
                    m_index--;
                    return *this;
                }
                size_type operator*() const { return m_number >> m_index & 1; }
                bool operator==(const NumberInterator &rhs) const { return !~m_index; }
                bool operator!=(const NumberInterator &rhs) const { return ~m_index; }
            };
            NumberInteration(Tp number) : m_number(number) {}
            NumberInterator begin() const { return NumberInterator(m_number, L - 1); }
            NumberInterator end() const { return NumberInterator(m_number, -1); }
        };
        template <typename Tp = uint32_t, size_type L = 30, typename Info = BaseInfo, bool Lock = false, size_type MAX_NODE = 1 << 20>
        struct Tree {
            struct iterator;
            struct node : Info {
                iterator m_child[2];
            };
            struct iterator {
                size_type m_index;
                static iterator new_node() {
                    iterator res;
                    res.m_index = s_use_count++;
                    return res;
                }
                static iterator new_node(iterator parent, size_type i) {
                    s_buffer[parent.m_index].m_child[i].m_index = s_use_count;
                    return new_node();
                }
                static iterator copy_node(iterator it) {
                    if constexpr (!Lock) {
                        s_buffer[s_use_count] = s_buffer[it.m_index];
                        return new_node();
                    } else if (!s_lock) {
                        s_buffer[s_use_count] = s_buffer[it.m_index];
                        return new_node();
                    } else
                        return it;
                }
                iterator &child(size_type i) { return s_buffer[m_index].m_child[i]; }
                iterator &child(size_type i) const { return s_buffer[m_index].m_child[i]; }
                iterator &get_child(size_type i) {
                    if (!s_buffer[m_index].m_child[i])
                        s_buffer[m_index].m_child[i] = new_node(*this, i);
                    else
                        s_buffer[m_index].m_child[i] = copy_node(s_buffer[m_index].m_child[i]);
                    return s_buffer[m_index].m_child[i];
                }
                template <typename Iterator, typename Modify = Ignore>
                iterator insert(Iterator first, Iterator last, Modify &&modify = Modify()) {
                    if (first == last) return *this;
                    iterator ch = get_child(*first);
                    if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(ch);
                    return ch.insert(++first, last, modify);
                }
                template <typename Sequence, typename Modify = Ignore>
                iterator insert(const Sequence &sequence, Modify &&modify = Modify()) { return insert(sequence.begin(), sequence.end(), modify); }
                template <typename Iterator>
                iterator find(Iterator first, Iterator last) const {
                    if (first == last) return *this;
                    iterator ch = child(*first);
                    if (ch.m_index) return ch.find(++first, last);
                    return iterator{};
                }
                template <typename Sequence>
                iterator find(const Sequence &sequence) const { return find(sequence.begin(), sequence.end()); }
                node *operator->() const { return s_buffer + m_index; }
                node &operator*() { return s_buffer[m_index]; }
                explicit operator bool() const { return m_index; }
                operator size_type() const { return m_index; }
            };
            static node s_buffer[MAX_NODE];
            static size_type s_use_count;
            static bool s_lock;
            iterator m_root;
            static void lock() { s_lock = true; }
            static void unlock() { s_lock = false; }
            template <typename Judger>
            static Tp reduce_max_same(const Tree<Tp, L, Info, Lock, MAX_NODE> &base, const Tree<Tp, L, Info, Lock, MAX_NODE> &end, Tp number, Judger &&judger) {
                iterator base_cur = base.m_root, end_cur = end.m_root;
                Tp res{};
                for (size_type c : NumberInteration<Tp, L>(number)) {
                    res *= 2;
                    iterator base_child = base_cur.child(c), end_child = end_cur.child(c);
                    if (judge(base_child, end_child)) {
                        base_cur = base_child, end_cur = end_child;
                        res++;
                    } else
                        base_cur = base_cur.child(c ^ 1), end_cur = end_cur.child(c ^ 1);
                }
                return res;
            }
            template <typename Judger>
            static Tp reduce_max_bitxor(const Tree<Tp, L, Info, Lock, MAX_NODE> &base, const Tree<Tp, L, Info, Lock, MAX_NODE> &end, Tp number, Judger &&judger) {
                number ^= L < sizeof(Tp) << 3 ? (Tp(1) << L) - 1 : Tp(-1);
                iterator base_cur = base.m_root, end_cur = end.m_root;
                Tp res{};
                for (size_type c : NumberInteration<Tp, L>(number)) {
                    res *= 2;
                    iterator base_child = base_cur.child(c), end_child = end_cur.child(c);
                    if (judger(base_child, end_child)) {
                        base_cur = base_child, end_cur = end_child;
                        res++;
                    } else
                        base_cur = base_cur.child(c ^ 1), end_cur = end_cur.child(c ^ 1);
                }
                return res;
            }
            template <typename Judger>
            static bool _erase(iterator it, typename NumberInteration<Tp, L>::NumberInterator cur, typename NumberInteration<Tp, L>::NumberInterator end, Judger &&judger) {
                if (cur != end) {
                    size_type c = *cur;
                    iterator &child = it.child(c);
                    if (!child.m_index) return false;
                    child = it.get_child(c);
                    if (!_erase(child, ++cur, end, judger)) return false;
                    child.m_index = 0;
                }
                return judger(it);
            }
            template <typename Modify>
            static void _trace(iterator it, typename NumberInteration<Tp, L>::NumberInterator cur, typename NumberInteration<Tp, L>::NumberInterator end, Modify &&modify) {
                if (cur != end) {
                    size_type c = *cur;
                    if (it.child(c).m_index) _trace(it.get_child(c), ++cur, end, modify);
                }
                modify(it);
            }
            void init() { m_root = iterator::new_node(); }
            Tree<Tp, L, Info, Lock, MAX_NODE> copy() const {
                Tree<Tp, L, Info, Lock, MAX_NODE> res;
                res.m_root = iterator::copy_node(m_root);
                return res;
            }
            template <typename Modify = Ignore>
            iterator insert(Tp number, Modify &&modify = Modify()) { return m_root.insert(NumberInteration<Tp, L>(number), modify); }
            template <typename Judger = BaseEraseJudger>
            bool erase(Tp number, Judger &&judger = Judger()) {
                NumberInteration<Tp, L> num(number);
                return _erase(m_root, num.begin(), num.end(), judger);
            }
            template <typename Modify = Ignore>
            void trace(Tp number, Modify &&modify = Modify()) {
                NumberInteration<Tp, L> num(number);
                _trace(m_root, num.begin(), num.end(), modify);
            }
            template <typename Judger = BaseQueryJudge>
            std::pair<iterator, Tp> query_max_same(Tp number, Judger &&judge = Judger()) {
                iterator cur = m_root;
                Tp res{};
                for (size_type c : NumberInteration<Tp, L>(number)) {
                    res *= 2;
                    iterator child = cur.child(c);
                    if (judge(child)) {
                        cur = child;
                        res++;
                    } else
                        cur = cur.child(c ^ 1);
                }
                return {cur, res};
            }
            template <typename Judger = BaseQueryJudge>
            std::pair<iterator, Tp> query_max_bitxor(Tp number, Judger &&judge = Judger()) {
                number ^= L < sizeof(Tp) << 3 ? (Tp(1) << L) - 1 : Tp(-1);
                iterator cur = m_root;
                Tp res{};
                for (size_type c : NumberInteration<Tp, L>(number)) {
                    res *= 2;
                    iterator child = cur.child(c);
                    if (judge(child)) {
                        cur = child;
                        res++;
                    } else
                        cur = cur.child(c ^ 1);
                }
                return {cur, res};
            }
        };
        template <typename Tp, size_type L, typename Info, bool Lock, size_type MAX_NODE>
        typename Tree<Tp, L, Info, Lock, MAX_NODE>::node Tree<Tp, L, Info, Lock, MAX_NODE>::s_buffer[MAX_NODE];
        template <typename Tp, size_type L, typename Info, bool Lock, size_type MAX_NODE>
        size_type Tree<Tp, L, Info, Lock, MAX_NODE>::s_use_count = 1;
        template <typename Tp, size_type L, typename Info, bool Lock, size_type MAX_NODE>
        bool Tree<Tp, L, Info, Lock, MAX_NODE>::s_lock = true;
    }
    template <PerBiTrie::size_type L = 30, typename Info = PerBiTrie::BaseInfo, bool Lock = false, PerBiTrie::size_type MAX_NODE = 1 << 20>
    using PerBiTrie32 = PerBiTrie::Tree<uint32_t, L, Info, Lock, MAX_NODE>;
}

#endif