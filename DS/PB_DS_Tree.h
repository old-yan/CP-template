#ifndef __OY_PB_DS_TREE__
#define __OY_PB_DS_TREE__

#include <ext/pb_ds/assoc_container.hpp>

namespace OY {
    struct PB_DS_TreeSetTag {
        static constexpr bool is_map = false;
        static constexpr bool multi_key = false;
    };
    struct PB_DS_TreeMultisetTag {
        static constexpr bool is_map = false;
        static constexpr bool multi_key = true;
    };
    struct PB_DS_TreeMapTag {
        static constexpr bool is_map = true;
        static constexpr bool multi_key = false;
    };
    template <typename _Compare, bool multi_key>
    struct PB_DS_less_equal {
        _Compare __comp;
        PB_DS_less_equal(_Compare __comp = _Compare()) : __comp(__comp) {}
        template <typename _Tp, typename _Fp>
        bool operator()(const _Tp &x, const _Fp &y) const {
            if (multi_key)
                return !__comp(y.key, x.key);
            else
                return __comp(x.key, y.key);
        }
    };
    template <typename _Tp, typename _Fp, bool is_map>
    struct _PB_DS_TreeNode {
        _Tp key;
        mutable _Fp value;
    };
    template <typename _Tp, typename _Fp>
    struct _PB_DS_TreeNode<_Tp, _Fp, false> { _Tp key; };
    template <typename _Tp, typename _Fp = int, typename _Compare = std::less<_Tp>, typename _Tag = PB_DS_TreeMultisetTag>
    class PB_DS_Tree {
    public:
        using node = _PB_DS_TreeNode<_Tp, _Fp, _Tag::is_map>;
        using iterator = typename __gnu_pbds::tree<node, __gnu_pbds::null_type, PB_DS_less_equal<_Compare, _Tag::multi_key>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>::iterator;
        __gnu_pbds::tree<node, __gnu_pbds::null_type, PB_DS_less_equal<_Compare, _Tag::multi_key>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> m_tree;
        _Compare m_comp;
        static node makeNode(_Tp __key) {
            if constexpr (_Tag::is_map)
                return {__key, _Fp()};
            else
                return {__key};
        }

    public:
        PB_DS_Tree(_Compare __comp = _Compare()) : m_comp(__comp), m_tree(__comp) {}
        void clear() { m_tree.clear(); }
        template <typename... Args>
        void insert(Args... __args) {
            m_tree.insert({__args...});
        }
        void update(_Tp __key, _Fp __value) {
            static_assert(_Tag::is_map);
            if (auto p = find(__key); p != end())
                p->value = __value;
            else
                insert(__key, __value);
        }
        void erase(_Tp __key) { m_tree.erase(find(__key)); }
        void erase(_Tp __key, int __count) {
            static_assert(_Tag::multi_key);
            for (auto it = find(__count); __count-- && it != end() && !m_comp(__key, it->key);) it = m_tree.erase(it);
        }
        int rank(_Tp __key) const { return m_tree.order_of_key(makeNode(__key)); }
        iterator kth(int __k) const { return m_tree.find_by_order(__k); }
        iterator find(_Tp __key) const {
            if (auto p = lower_bound(__key); p != end() && !m_comp(__key, p->key))
                return p;
            else
                return m_tree.end();
        }
        iterator lower_bound(_Tp __key) const {
            if constexpr (_Tag::multi_key)
                return m_tree.upper_bound(makeNode(__key));
            else
                return m_tree.lower_bound(makeNode(__key));
        }
        iterator upper_bound(_Tp __key) const {
            if constexpr (_Tag::multi_key)
                return m_tree.lower_bound(makeNode(__key));
            else
                return m_tree.upper_bound(makeNode(__key));
        }
        iterator smaller_bound(_Tp __key) const { return prev(lower_bound(__key)); }
        int size() const { return m_tree.size(); }
        bool empty() const { return !size(); }
        iterator end() const { return m_tree.end(); }
        int count(_Tp __key) const {
            auto it1 = lower_bound(__key);
            if (it1 == end()) return 0;
            auto it2 = upper_bound(__key);
            if constexpr (!_Tag::multi_key)
                return it2 != it1;
            else {
                if (it2 == end())
                    return size() - rank(__key);
                else
                    return rank(it2->key) - rank(__key);
            }
        }
    };
    namespace PB_DS_TreeContainer {
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Set = PB_DS_Tree<_Tp, bool, _Compare, PB_DS_TreeSetTag>;
        template <typename _Tp, typename _Compare = std::less<_Tp>>
        using Multiset = PB_DS_Tree<_Tp, __gnu_pbds::null_type, _Compare, PB_DS_TreeMultisetTag>;
        template <typename _Tp, typename _Fp, typename _Compare = std::less<_Tp>>
        using Map = PB_DS_Tree<_Tp, _Fp, _Compare, PB_DS_TreeMapTag>;
    }
}

#endif