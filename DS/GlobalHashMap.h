/*
最后修改:
20240325
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_GLOBALHASHMAP__
#define __OY_GLOBALHASHMAP__

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <vector>

namespace OY {
    namespace GHASH {
        using size_type = uint32_t;
        template <typename Tp, size_type L>
        struct Hash {
            static uint64_t s_bias;
            size_type operator()(const Tp &x) const { return (((x + s_bias) * 11995408973635179863ULL) >> (64 - L)); }
            size_type operator()(const Tp &x, const Tp &y) const { return operator()((x << (sizeof(Tp) << 2)) + (x >> (sizeof(Tp) << 1)) + y); }
        };
        template <typename Tp, size_type L>
        uint64_t Hash<Tp, L>::s_bias = std::chrono::steady_clock::now().time_since_epoch().count();
        template <size_type L>
        struct Hash<std::string, L> {
            size_type operator()(const std::string &x) const { return Hash<size_t, L>()(std::hash<std::string>()(x)); }
        };
        template <typename KeyType, typename MappedType>
        struct Node {
            KeyType m_key;
            MappedType m_mapped;
        };
        template <typename KeyType>
        struct Node<KeyType, void> {
            KeyType m_key;
        };
        template <bool Record>
        struct Recorder {};
        template <>
        struct Recorder<true> : std::vector<size_type> {};
        template <typename KeyType, typename MappedType, bool MakeRecord, size_type L>
        struct TableBase {
            static constexpr size_type hash_mask = (1 << L) - 1;
            using node = Node<KeyType, MappedType>;
            struct pair {
                node *m_ptr;
                bool m_flag;
            };
            node m_pool[1 << L];
            std::bitset<1 << L> m_occupied;
            Recorder<MakeRecord> m_recs;
            size_type m_size;
            void reserve(size_type count) {
                if constexpr (MakeRecord) m_recs.reserve(count);
            }
            size_type size() const { return m_size; }
            bool empty() const { return !size(); }
            void clear() {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                for (auto i : m_recs) m_occupied[i] = false;
                m_recs.clear(), m_size = 0;
            }
            template <typename Callback>
            void do_for_each(Callback &&call) {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                for (auto i : m_recs)
                    if (m_occupied[i]) call(m_pool + i);
            }
            pair insert(const KeyType &key) {
                size_type ha = Hash<KeyType, L>()(key), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) return {m_pool + i, false};
                    i = (i + 1) & hash_mask;
                }
                m_pool[i].m_key = key, m_occupied[i] = true, m_size++;
                if constexpr (MakeRecord) m_recs.push_back(i);
                return {m_pool + i, true};
            }
            bool erase(const KeyType &key) {
                size_type ha = Hash<KeyType, L>()(key), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) {
                        m_occupied[i] = false, m_size--;
                        return true;
                    }
                    i = (i + 1) & hash_mask;
                }
                return false;
            }
            node *find(const KeyType &key) const {
                size_type ha = Hash<KeyType, L>()(key), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) return (node *)(m_pool + i);
                    i = (i + 1) & hash_mask;
                }
                return nullptr;
            }
        };
        template <typename KeyType, bool MakeRecord, size_type L>
        struct UnorderedSet : TableBase<KeyType, void, MakeRecord, L> {};
        template <typename KeyType, typename MappedType, bool MakeRecord, size_type L>
        struct UnorderedMap : TableBase<KeyType, MappedType, MakeRecord, L> {
            using typename TableBase<KeyType, MappedType, MakeRecord, L>::pair;
            pair insert_or_assign(const KeyType &key, const MappedType &mapped) {
                pair res = this->insert(key);
                res.m_ptr->m_mapped = mapped;
                return res;
            }
            pair insert_or_ignore(const KeyType &key, const MappedType &mapped) {
                pair res = this->insert(key);
                if (res.m_flag) res.m_ptr->m_mapped = mapped;
                return res;
            }
            MappedType get(const KeyType &key, const MappedType &_default) const {
                auto res = this->find(key);
                return res ? res->m_mapped : _default;
            }
            const MappedType &get(const KeyType &key) const { return this->find(key)->m_mapped; }
        };
    }
}

#endif