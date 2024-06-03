/*
最后修改:
20240604
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
#include <vector>

namespace OY {
    namespace BiTrie {
        using size_type = uint32_t;
        struct Ignore {};
        struct CountInfo {
            void add_one() { ++*(size_type *)(this); }
            void remove_one() { --*(size_type *)(this); }
            size_type count() const { return *(size_type *)(this); }
        };
        template <typename Info>
        struct Node : Info {
            size_type m_child[2];
        };
        struct BaseQueryJudger {
            template <typename Node>
            bool operator()(Node *) const { return true; }
        };
        template <typename Tp, size_type L>
        struct NumberIteration {
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
            NumberIteration(Tp number) : m_number(number) {}
            NumberInterator begin() const { return NumberInterator(m_number, L - 1); }
            NumberInterator end() const { return NumberInterator(m_number, -1); }
        };
        template <typename Tp = uint32_t, size_type L = 30, typename Info = Ignore>
        struct Tree {
            using node = Node<Info>;
            std::vector<node> m_data;
            size_type &_child(size_type cur, size_type i) { return m_data[cur].m_child[i]; }
            size_type _child(size_type cur, size_type i) const { return m_data[cur].m_child[i]; }
            size_type _get_child(size_type cur, size_type i) {
                if (!m_data[cur].m_child[i]) m_data.push_back({}), m_data[cur].m_child[i] = m_data.size() - 1;
                return m_data[cur].m_child[i];
            }
            node *_child_of(const node *p, size_type i) { return p->m_child[i] ? &m_data[p->m_child[i]] : nullptr; }
            template <typename Iterator, typename Modify>
            size_type _insert(size_type it, Iterator first, Iterator last, Modify &&modify = Modify()) {
                if (first == last) return it;
                auto ch = _get_child(it, *first);
                if constexpr (!std::is_same<typename std::decay<Modify>::type, Ignore>::value) modify(&m_data[ch]);
                return _insert(ch, ++first, last, modify);
            }
            template <typename Iterator, typename Judger>
            bool _erase(size_type it, Iterator first, Iterator last, Judger &&judge) {
                if (first != last) {
                    size_type c = *first, &ch = _child(it, c);
                    if (!ch) return false;
                    if (!_erase(ch, ++first, last, judge)) return false;
                    ch = 0;
                    return !_child(it, c ^ 1);
                } else if constexpr (std::is_same<Info, CountInfo>::value) {
                    m_data[it].remove_one();
                    return !m_data[it].count();
                } else if constexpr (!std::is_same<typename std::decay<Judger>::type, Ignore>::value)
                    return judge(&m_data[it]);
                else
                    return true;
            }
            template <typename Modify>
            void _trace(size_type it, typename NumberIteration<Tp, L>::NumberInterator first, typename NumberIteration<Tp, L>::NumberInterator last, Modify &&modify) {
                if (first != last) {
                    size_type ch = _child(it, *first);
                    _trace(ch, ++first, last, modify);
                }
                modify(&m_data[it]);
            }
            static constexpr Tp mask() { return (L == sizeof(Tp) << 3) ? -1 : (Tp(1) << L) - 1; }
            Tree() : m_data(1) {}
            void clear() { m_data.clear(), m_data.resize(1); }
            void reserve_by_count(size_type cnt) {
                size_type sum = 1, cur = 1, mi = std::min<Tp>(cnt, Tp(1) << L);
                for (size_type i = L; i--;) sum += std::min<size_type>(mi, cur *= 2);
                reserve(sum);
            }
            void reserve(size_type capacity) { m_data.reserve(capacity); }
            node *root() const { return (node *)m_data.data(); }
            bool empty() const { return !m_data[0].m_child[0] && !m_data[0].m_child[1]; }
            template <typename Modify = Ignore>
            node *insert(Tp number, Modify &&modify = Modify()) {
                NumberIteration<Tp, L> num(number);
                node *res = &m_data[_insert(0, num.begin(), num.end(), modify)];
                if constexpr (std::is_same<Info, CountInfo>::value) res->add_one();
                return res;
            }
            template <typename Judger = Ignore>
            void erase(Tp number, Judger &&judge = Judger()) {
                NumberIteration<Tp, L> num(number);
                _erase(0, num.begin(), num.end(), judge);
            }
            template <typename Modify = Ignore>
            void trace(Tp number, Modify &&modify = Modify()) {
                NumberIteration<Tp, L> num(number);
                _trace(0, num.begin(), num.end(), modify);
            }
            template <typename Judger = BaseQueryJudger>
            std::pair<node *, Tp> query_max_same(Tp number, Judger &&judge = Judger()) const {
                size_type it{};
                Tp res{};
                for (size_type c : NumberIteration<Tp, L>(number)) {
                    res *= 2;
                    size_type ch = _child(it, c);
                    if (ch && judge((node *)&m_data[ch])) {
                        it = ch;
                        res++;
                    } else
                        it = _child(it, c ^ 1);
                }
                return {(node *)&m_data[it], res};
            }
            template <typename Judger = BaseQueryJudger>
            std::pair<node *, Tp> query_max_bitxor(Tp number, Judger &&judge = Judger()) const {
                number ^= L < sizeof(Tp) << 3 ? (Tp(1) << L) - 1 : Tp(-1);
                size_type it{};
                Tp res{};
                for (size_type c : NumberIteration<Tp, L>(number)) {
                    res *= 2;
                    size_type ch = _child(it, c);
                    if (ch && judge((node *)&m_data[ch])) {
                        it = ch;
                        res++;
                    } else
                        it = _child(it, c ^ 1);
                }
                return {(node *)&m_data[it], res};
            }
        };
    }
    template <BiTrie::size_type L = 30, typename Info = BiTrie::Ignore>
    using BiTrie32 = BiTrie::Tree<uint32_t, L, Info>;
    template <BiTrie::size_type L = 30>
    using ErasableBiTrie32 = BiTrie::Tree<uint32_t, L, BiTrie::CountInfo>;
}

#endif