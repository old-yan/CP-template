/*
最后修改:
20230908
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_BITRIE__
#define __OY_BITRIE__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>

namespace OY {
    namespace BinaryTrie {
        using size_type = uint32_t;
        struct NoInit {};
        struct BaseJudge {
            template <typename Iterator>
            bool operator()(Iterator it) const { return it.m_index; }
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
        template <typename Tp = uint32_t, size_type L = 30, typename Info = BaseInfo, BinaryTrie::size_type MAX_NODE = 1 << 20>
        struct Tree {
            struct iterator {
                size_type m_index;
                static iterator newNode() {
                    iterator res;
                    res.m_index = s_use_count++;
                    return res;
                }
                static iterator newNode(iterator parent, size_type i) {
                    s_parent[s_use_count].m_index = parent.m_index, s_child[parent.m_index][i].m_index = s_use_count;
                    return newNode();
                }
                iterator &parent() { return s_parent[m_index]; }
                iterator &child(size_type i) { return s_child[m_index][i]; }
                iterator &child(size_type i) const { return s_child[m_index][i]; }
                iterator &get_child(size_type i) {
                    if (!s_child[m_index][i]) s_child[m_index][i] = newNode(*this, i);
                    return s_child[m_index][i];
                }
                template <typename Iterator, typename Modify = NoInit>
                iterator insert(Iterator first, Iterator last, Modify &&modify = Modify()) {
                    if (first == last) return *this;
                    iterator ch = get_child(*first);
                    if constexpr (!std::is_same<typename std::decay<Modify>::type, NoInit>::value) modify(ch);
                    return ch.insert(++first, last, modify);
                }
                template <typename Sequence, typename Modify = NoInit>
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
                Info *operator->() const { return s_info + m_index; }
                Info &operator*() { return s_info[m_index]; }
                explicit operator bool() const { return m_index; }
                operator size_type() const { return m_index; }
            };
            static iterator s_parent[MAX_NODE];
            static iterator s_child[MAX_NODE][2];
            static Info s_info[MAX_NODE];
            static size_type s_use_count;
            iterator m_root;
            Tree() : m_root(iterator::newNode()) {}
            template <typename Modify = NoInit>
            iterator insert(Tp number, Modify modify = Modify()) { return m_root.insert(NumberInteration<Tp, L>(number), modify); }
            iterator erase(iterator leaf) {
                iterator cur = leaf;
                for (size_type i = 0; i < L; i++) {
                    iterator parent = cur.parent();
                    size_type c = cur == parent.child(1);
                    parent.child(c).m_index = 0;
                    cur = parent;
                    if (cur.child(c ^ 1)) break;
                }
                return cur;
            }
            template <typename Judge = BaseJudge>
            std::pair<iterator, Tp> query_max_same(Tp number, Judge judge = Judge()) {
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
            template <typename Judge = BaseJudge>
            std::pair<iterator, Tp> query_max_bitxor(Tp number, Judge judge = Judge()) {
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
            Tp query_leaf_value(iterator leaf) {
                iterator cur = leaf;
                Tp res{};
                for (size_type i = 0; i < L; i++) {
                    iterator parent = cur.parent();
                    if (cur == parent.child(1)) res |= Tp(1) << i;
                    cur = parent;
                }
                return res;
            }
        };
        template <typename Tp, size_type L, typename Info, size_type MAX_NODE>
        typename Tree<Tp, L, Info, MAX_NODE>::iterator Tree<Tp, L, Info, MAX_NODE>::s_parent[MAX_NODE];
        template <typename Tp, size_type L, typename Info, size_type MAX_NODE>
        typename Tree<Tp, L, Info, MAX_NODE>::iterator Tree<Tp, L, Info, MAX_NODE>::s_child[MAX_NODE][2];
        template <typename Tp, size_type L, typename Info, size_type MAX_NODE>
        Info Tree<Tp, L, Info, MAX_NODE>::s_info[MAX_NODE];
        template <typename Tp, size_type L, typename Info, size_type MAX_NODE>
        size_type Tree<Tp, L, Info, MAX_NODE>::s_use_count = 1;
    }
    template <typename Tp = uint32_t, BinaryTrie::size_type L = 30, typename Info = BinaryTrie::BaseInfo, BinaryTrie::size_type MAX_NODE = 1 << 20>
    using BiTrie = BinaryTrie::Tree<Tp, L, Info, MAX_NODE>;
}

#endif