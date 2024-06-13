#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"

// 注意，对象必须开在全局
OY::GHASH::UnorderedSet<int, true, 1009> S;
void test_int_set() {
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
namespace OY {
    namespace GHASH {
        template <>
        struct Hash<pii> {
            uint64_t operator()(const pii &x) const {
                // 把两个 int 连起来当成一个 long long 来求哈希
                return Hash<long long>()(((long long)(x.first) << 32) + x.second);
            }
        };
    }
}
// 注意，对象必须开在全局
OY::GHASH::UnorderedSet<pii, true, 1009> S2;
void test_pair_set() {
    S2.insert({100, 200});
    S2.insert({300, 400});
    S2.insert({100, 400});
    S2.insert({300, 400});
    S2.insert({300, 200});
    cout << "S2.size() = " << S2.size() << endl;
    cout << "S2: ";
    S2.do_for_each([](decltype(S2)::node *p) {
        cout << p->m_key.first << ',' << p->m_key.second << ' ';
    });
    cout << endl;

    S2.clear();
    cout << "S2.size() = " << S2.size() << endl
         << endl;
}

// 注意，对象必须开在全局
OY::GHASH::UnorderedMap<std::string, int, true, 1009> S3;
void test_map() {
    S3["apple"] += 10;
    S3["banana"] += 5;
    S3["apple"] -= 2;
    S3["orange"] += 10;
    S3.do_for_each([](decltype(S3)::node *p) {
        cout << p->m_key << ": " << p->m_mapped << endl;
    });
}

int main() {
    test_int_set();
    test_pair_set();
    test_map();
}
/*
#输出如下
S.size() = 3
S: 100 300 200 
S.size() = 0

S2.size() = 4
S2: 100,200 300,400 100,400 300,200 
S2.size() = 0

apple: 8
banana: 5
orange: 10

*/