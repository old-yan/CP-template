#ifndef __OY_BINARYHEAP__
#define __OY_BINARYHEAP__

#include <functional>

namespace OY {
    template <typename _Tp, typename _Compare = std::less<_Tp>>
    class BinaryHeap {
        std::vector<_Tp> m_data;
        _Compare m_comp;

    public:
        BinaryHeap(_Compare __comp = _Compare()) : m_comp(__comp) {}
        template <typename _Iterator>
        BinaryHeap(_Iterator __first, _Iterator __last, _Compare __comp = _Compare()) : m_comp(__comp) { reset(__first, __last); }
        void clear() { m_data.clear(); }
        template <typename _Iterator>
        void reset(_Iterator __first, _Iterator __last) {
            m_data.assign(__first, __last);
            std::make_heap(m_data.begin(), m_data.end(), m_comp);
        }
        void push(_Tp __val) {
            m_data.push_back(__val);
            std::push_heap(m_data.begin(), m_data.end(), m_comp);
        }
        void pop() {
            std::pop_heap(m_data.begin(), m_data.end(), m_comp);
            m_data.pop_back();
        }
        _Tp top() const { return m_data.front(); }
        void join(BinaryHeap<_Tp, _Compare> &other) {
            if (m_data.size() < other.m_data.size()) std::swap(m_data, other.m_data);
            for (auto &item : other.m_data) push(item);
        }
        int size() const { return m_data.size(); }
        bool empty() const { return m_data.empty(); }
    };
    template <typename _Compare = std::less<int>, typename _Tp = std::decay_t<typename decltype(std::function(std::declval<_Compare>()))::first_argument_type>>
    BinaryHeap(_Compare = _Compare()) -> BinaryHeap<_Tp, _Compare>;
    template <typename _Iterator, typename _Tp = typename std::iterator_traits<_Iterator>::value_type, typename _Compare = std::less<_Tp>>
    BinaryHeap(_Iterator, _Iterator, _Compare = _Compare()) -> BinaryHeap<_Tp, _Compare>;
}

#endif