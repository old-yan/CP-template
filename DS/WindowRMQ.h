/*
最后修改:
20240416
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_WINDOWRMQ__
#define __OY_WINDOWRMQ__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

namespace OY {
    namespace WINDOW {
        using size_type = uint32_t;
        template <typename Tp>
        struct PointerGetter {
            Tp *const m_arr;
            Tp operator()(size_type i) const { return m_arr[i]; }
        };
        template <typename Tp>
        struct VectorGetter {
            std::vector<Tp> &m_arr;
            Tp operator()(size_type i) const { return m_arr[i]; }
        };
        template <typename Tp, typename Compare, typename Getter>
        class Table {
            struct node {
                size_type m_index;
                Tp m_value;
            };
            std::vector<node> m_data;
            size_type m_cur, m_window_len, m_ql, m_qr;
            Compare m_comp;
            Getter m_getter;
        public:
            Table(size_type length, size_type window_len, Compare comp, Getter getter) : m_cur(-1), m_window_len(window_len), m_ql(0), m_qr(0), m_comp(comp), m_getter(getter) { m_data.resize(length); }
            void extend_right() {
                Tp val = m_getter(++m_cur);
                while (m_ql != m_qr && !m_comp(val, m_data[m_qr - 1].m_value)) m_qr--;
                if (m_ql != m_qr && m_data[m_ql].m_index == m_cur - m_window_len) m_ql++;
                m_data[m_qr++] = {m_cur, val};
            }
            void extend_to(size_type right) {
                for (size_type i = 0; i != right + 1; i++) extend_right();
            }
            const node *raw_query() const { return &m_data[m_ql]; }
            const node *next() {
                extend_right();
                return raw_query();
            }
            size_type cursor() const { return m_cur; }
            bool is_finished() const { return cursor() + 1 == m_data.capacity(); }
        };
    }
    template <typename Tp>
    auto make_MaxWindow(WINDOW::size_type length, WINDOW::size_type window_len, Tp *arr) -> WINDOW::Table<Tp, std::less<Tp>, WINDOW::PointerGetter<Tp>> { return WINDOW::Table<Tp, std::less<Tp>, WINDOW::PointerGetter<Tp>>(length, window_len, {}, {arr}); }
    template <typename Tp>
    auto make_MinWindow(WINDOW::size_type length, WINDOW::size_type window_len, Tp *arr) -> WINDOW::Table<Tp, std::greater<Tp>, WINDOW::PointerGetter<Tp>> { return WINDOW::Table<Tp, std::greater<Tp>, WINDOW::PointerGetter<Tp>>(length, window_len, {}, {arr}); }
    template <typename Tp>
    auto make_MaxWindow(WINDOW::size_type length, WINDOW::size_type window_len, std::vector<Tp> &arr) -> WINDOW::Table<Tp, std::less<Tp>, WINDOW::VectorGetter<Tp>> { return WINDOW::Table<Tp, std::less<Tp>, WINDOW::VectorGetter<Tp>>(length, window_len, {}, {arr}); }
    template <typename Tp>
    auto make_MinWindow(WINDOW::size_type length, WINDOW::size_type window_len, std::vector<Tp> &arr) -> WINDOW::Table<Tp, std::greater<Tp>, WINDOW::VectorGetter<Tp>> { return WINDOW::Table<Tp, std::greater<Tp>, WINDOW::VectorGetter<Tp>>(length, window_len, {}, {arr}); }
    template <typename Tp, typename Getter>
    using MaxWindow = WINDOW::Table<Tp, std::less<Tp>, Getter>;
    template <typename Tp, typename Getter>
    using MinWindow = WINDOW::Table<Tp, std::greater<Tp>, Getter>;
}

#endif