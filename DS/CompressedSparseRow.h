/*
最后修改:
20241220
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_COMPRESSEDSPARSEROW__
#define __OY_COMPRESSEDSPARSEROW__

#include <algorithm>
#include <cstdint>
#include <functional>
#include <numeric>
#include <vector>

namespace OY {
    namespace CSR {
        using size_type = uint32_t;
        template <typename Tp>
        class Container {
        public:
            struct pair {
                size_type m_buc_id;
                Tp m_value;
            };
            struct bucket {
                Container<Tp> *m_csr;
                size_type m_buc_id;
                bucket(Container<Tp> *csr, size_type buc_id) : m_csr(csr), m_buc_id(buc_id) {}
                size_type size() const { return m_csr->m_deg[m_buc_id]; }
                bool empty() const { return !m_csr->m_deg[m_buc_id]; }
                void push_back(const Tp &val) { m_csr->m_raw_data.push_back({m_buc_id, val}), m_csr->m_deg[m_buc_id]++; }
            };
            struct buckets {
                struct iterator {
                    const Tp *m_ptr;
                    iterator(const Tp *ptr) : m_ptr(ptr) {}
                    iterator &operator++() {
                        m_ptr++;
                        return *this;
                    }
                    iterator operator++(int) {
                        iterator old(*this);
                        m_ptr++;
                        return old;
                    }
                    const Tp &operator*() const { return *m_ptr; }
                    bool operator==(const iterator &rhs) const { return m_ptr == rhs.m_ptr; }
                    bool operator!=(const iterator &rhs) const { return m_ptr != rhs.m_ptr; }
                };
                struct bucket {
                    const buckets *m_csr;
                    size_type m_buc_id;
                    bucket(const buckets *csr, size_type buc_id) : m_csr(csr), m_buc_id(buc_id) {}
                    size_type size() const { return m_csr->m_starts[m_buc_id + 1] - m_csr->m_starts[m_buc_id]; }
                    bool empty() const { return m_csr->m_starts[m_buc_id] == m_csr->m_starts[m_buc_id + 1]; }
                    const Tp& front() const { return m_csr->m_seq[m_csr->m_starts[m_buc_id]]; }
                    const Tp& back() const { return m_csr->m_seq[m_csr->m_starts[m_buc_id + 1] - 1]; }
                    iterator begin() const { return iterator(m_csr->m_seq.data() + m_csr->m_starts[m_buc_id]); }
                    iterator end() const { return iterator(m_csr->m_seq.data() + m_csr->m_starts[m_buc_id + 1]); }
                };
                std::vector<Tp> m_seq;
                std::vector<size_type> m_starts;
                size_type m_bucket_cnt;
                bucket operator[](size_type buc_id) const { return bucket(this, buc_id); }
                iterator bucket_begin(size_type buc_id) { return iterator(m_seq.data() + m_starts[buc_id]); }
                iterator bucket_end(size_type buc_id) { return iterator(m_seq.data() + m_starts[buc_id + 1]); }
            };
        private:
            std::vector<size_type> m_deg;
            std::vector<pair> m_raw_data;
            size_type m_bucket_cnt;
        public:
            Container(size_type bucket_cnt = 0, size_type item_cnt = 0) { resize(bucket_cnt, item_cnt); }
            void resize(size_type bucket_cnt, size_type item_cnt) {
                if (!(m_bucket_cnt = bucket_cnt)) return;
                m_raw_data.reserve(item_cnt), m_deg.assign(m_bucket_cnt, {});
            }
            bucket operator[](size_type buc_id) { return bucket(this, buc_id); }
            buckets get_buckets() const {
                buckets res;
                res.m_bucket_cnt = m_bucket_cnt;
                res.m_starts.resize(m_bucket_cnt + 1);
                std::copy(m_deg.begin(), m_deg.end(), res.m_starts.begin());
                size_type last = 0;
                for (size_type i = 0; i <= m_bucket_cnt; i++) {
                    std::swap(res.m_starts[i], last);
                    if (i) res.m_starts[i] += res.m_starts[i - 1];
                }
                auto copy = res.m_starts;
                res.m_seq.resize(res.m_starts.back());
                for (auto &p : m_raw_data) res.m_seq[copy[p.m_buc_id]++] = p.m_value;
                return res;
            }
        };
    }
};

#endif