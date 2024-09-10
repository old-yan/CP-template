/*
最后修改:
20240416
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_MONOSTACK__
#define __OY_MONOSTACK__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <numeric>
#include <vector>

namespace OY {
    namespace MONOSTACK {
        using size_type = uint32_t;
        struct Ignore {};
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
        template <typename Tp, typename Compare, typename Getter, typename PopCallback = Ignore>
        class Table {
            struct node {
                size_type m_index;
                Tp m_value;
            };
            std::vector<node> m_data;
            size_type m_cur;
            Compare m_comp;
            Getter m_getter;
            PopCallback m_pop_call;
        public:
            Table(size_type length, Compare comp, Getter getter, PopCallback pop_call, const Tp &max, bool forward = true) : m_comp(comp), m_getter(getter), m_pop_call(pop_call) {
                m_data.reserve(length + 1);
                m_cur = forward ? -1 : length;
                m_data.push_back({forward ? size_type(-1) : length, max});
            }
            void extend_left() {
                Tp val = m_getter(--m_cur);
                while (!m_comp(val, m_data.back().m_value)) {
                    if constexpr (!std::is_same<PopCallback, Ignore>::value) m_pop_call(m_data.back().m_index, m_cur);
                    m_data.pop_back();
                }
                m_data.push_back({m_cur, val});
            }
            void extend_right() {
                Tp val = m_getter(++m_cur);
                while (!m_comp(val, m_data.back().m_value)) {
                    if constexpr (!std::is_same<PopCallback, Ignore>::value) m_pop_call(m_data.back().m_index, m_cur);
                    m_data.pop_back();
                }
                m_data.push_back({m_cur, val});
            }
            const node *raw_query_on_left_side(size_type left) const {
                size_type l = 1, r = m_data.size() - 1;
                while (l != r) {
                    size_type mid = (l + r) / 2;
                    if (m_data[mid].m_index >= left)
                        r = mid;
                    else
                        l = mid + 1;
                }
                return &m_data[l];
            }
            const node *raw_query_on_right_side(size_type right) const {
                size_type l = 1, r = m_data.size() - 1;
                while (l != r) {
                    size_type mid = (l + r) / 2;
                    if (m_data[mid].m_index <= right)
                        r = mid;
                    else
                        l = mid + 1;
                }
                return &m_data[l];
            }
            const node *query_on_left_side(size_type left, size_type right) {
                while (m_cur != right) extend_right();
                return raw_query_on_left_side(left);
            }
            const node *query_on_right_side(size_type left, size_type right) {
                while (m_cur != left) extend_left();
                return raw_query_on_right_side(right);
            }
            size_type cursor() const { return m_cur; }
            bool is_finished() const { return (~m_data.front().m_index) ? !m_data.back().m_index : (m_data.back().m_index + 1 == m_data.capacity()); }
        };
    }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMaxStack(MONOSTACK::size_type length, Tp *arr, PopCallback pop_call = PopCallback(), const Tp &max = std::numeric_limits<Tp>::max(), bool forward = true) -> MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, max, forward); }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMinStack(MONOSTACK::size_type length, Tp *arr, PopCallback pop_call = PopCallback(), const Tp &min = std::numeric_limits<Tp>::min(), bool forward = true) -> MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::PointerGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, min, forward); }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMaxStack(MONOSTACK::size_type length, std::vector<Tp> &arr, PopCallback pop_call = PopCallback(), const Tp &max = std::numeric_limits<Tp>::max(), bool forward = true) -> MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::less<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, max, forward); }
    template <typename Tp, typename PopCallback = MONOSTACK::Ignore>
    auto make_MonoMinStack(MONOSTACK::size_type length, std::vector<Tp> &arr, PopCallback pop_call = PopCallback(), const Tp &min = std::numeric_limits<Tp>::min(), bool forward = true) -> MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback> { return MONOSTACK::Table<Tp, std::greater<Tp>, MONOSTACK::VectorGetter<Tp>, PopCallback>(length, {}, {arr}, pop_call, min, forward); }
    template <typename Tp, typename Getter, typename PopCallback = MONOSTACK::Ignore>
    using MonoMaxStack = MONOSTACK::Table<Tp, std::less<Tp>, Getter, PopCallback>;
    template <typename Tp, typename Getter, typename PopCallback = MONOSTACK::Ignore>
    using MonoMinStack = MONOSTACK::Table<Tp, std::greater<Tp>, Getter, PopCallback>;
}

#endif