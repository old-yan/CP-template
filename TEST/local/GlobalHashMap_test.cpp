#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"

void test_int_set() {
    OY::GHASH::UnorderedSet<int, 10, true> S;
    S.insert(100);
    S.insert(300);
    S.insert(200);
    S.insert(300);
    cout << "S.size() = " << S.size() << endl;
    cout << "S: ";
    S.do_for_each([](decltype(S)::node *p) {
        cout << p->m_key << ' ';
    });
    cout << endl;

    S.clear();
    cout << "S.size() = " << S.size() << endl
         << endl;
}

using pii = std::pair<int, int>;
template <uint32_t L>
struct OY::GHASH::Hash<pii, L> {
    uint32_t operator()(const pii &x) const {
        // 把两个 int 连起来当成一个 long long 来求哈希
        return OY::GHASH::Hash<long long, L>()(((long long)(x.first) << 32) + x.second);
    }
};
void test_pair_set() {
    OY::GHASH::UnorderedSet<pii, 10, true> S;
    S.insert({100, 200});
    S.insert({300, 400});
    S.insert({100, 400});
    S.insert({300, 400});
    S.insert({300, 200});
    cout << "S.size() = " << S.size() << endl;
    cout << "S: ";
    S.do_for_each([](decltype(S)::node *p) {
        cout << p->m_key.first << ',' << p->m_key.second << ' ';
    });
    cout << endl;

    S.clear();
    cout << "S.size() = " << S.size() << endl
         << endl;
}

int main() {
    test_int_set();
    test_pair_set();
}
/*
#输出如下
S.size() = 3
S: 100 300 200 
S.size() = 0

S.size() = 4
S: 100,200 300,400 100,400 300,200 
S.size() = 0

*/