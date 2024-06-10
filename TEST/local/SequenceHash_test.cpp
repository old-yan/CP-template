#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

// 此处使用模数为 998244353 的自取模数
using mint = OY::mint998244353;
using table_type = OY::SEQHASH::SeqHashPresumTable<mint>;
using hash_type = table_type::hash_type;

void test_hash_type() {
    cout << "test for hash type:\n";
    std::string s1 = "abcd";
    auto v1 = hash_type(s1);
    cout << s1 << ": len = " << v1.m_len << ", hash = " << v1.m_val << endl;

    std::string s2 = "efg";
    auto v2 = hash_type(s2);
    cout << s2 << ": len = " << v2.m_len << ", hash = " << v2.m_val << endl;

    std::string s3 = "abcdefg";
    auto v3 = hash_type(s3);
    cout << s3 << ": len = " << v3.m_len << ", hash = " << v3.m_val << endl;

    // 字符串哈希结果本身是可以进行拼接的
    auto v4 = v1.append_right(v2);
    cout << s1 + s2 << ": len = " << v4.m_len << ", hash = " << v4.m_val << endl;

    auto v5 = v2.append_left(v1);
    cout << s1 + s2 << ": len = " << v5.m_len << ", hash = " << v5.m_val << endl;

    // 字符串哈希结果也可以进行差分
    auto v6 = v3.remove_prefix(v1);
    cout << s3.substr(s1.size()) << ": len = " << v6.m_len << ", hash = " << v6.m_val << endl;

    auto v7 = v3.remove_suffix(v2);
    cout << s3.substr(0, s3.size() - s2.size()) << ": len = " << v7.m_len << ", hash = " << v7.m_val << endl;

    // 字符串哈希结果也可以进行倍增
    auto v8 = v3.repeat_for(3);
    cout << s3 + s3 + s3 << ": len = " << v8.m_len << ", hash = " << v8.m_val << endl;
    cout << endl;
}

void test_hash_table() {
    cout << "test for hash table:\n";
    std::string s = "oxabcdyyefgabcdefg";
    table_type table(s);
    // 查询区间的哈希结果
    auto v = table.query_hash(2, 5);
    cout << s.substr(2, 4) << ": len = " << v.m_len << ", hash = " << v.m_val << endl;

    auto v2 = table.query_hash(8, 10);
    cout << s.substr(8, 3) << ": len = " << v2.m_len << ", hash = " << v2.m_val << endl;
    cout << endl;
}

void test_lcp() {
    cout << "test for compare:\n";
    std::string s1 = "xabcy";
    std::string s2 = "pollxabcz";
    table_type table1(s1);
    table_type table2(s2);
    // 求 lcp
    cout << s1.substr(0) << " and " << s2.substr(4) << "'s lcp length = " << table_type::lcp(table1, 0, table2, 4) << endl;
    // 求子串比较结果
    int res = table_type::compare(table1, 1, 4, table2, 5, 8);
    if (res < 0)
        cout << s1.substr(1, 4) << " is smaller than " << s2.substr(5, 4) << endl;
    else if (res > 0)
        cout << s1.substr(1, 4) << " is bigger than " << s2.substr(5, 4) << endl;
    else
        cout << s1.substr(1, 4) << " is equal to " << s2.substr(5, 4) << endl;

    // 但是这个函数不一定能比出字典序大小
    // 假设元素会做对 128 取模的映射
    std::vector<int> arr1{30, -10, 0, 0, 0, 0};
    std::vector<int> arr2{30, 118, 0, 0, 0, 0};
    std::vector<int> arr3{30, 3, 0, 0, 0, 0};
    auto mapping = [](int i) { return i & 127; };
    cout << "arr1:";
    for (auto a : arr1) cout << ' ' << a;
    cout << endl;
    cout << "arr2:";
    for (auto a : arr2) cout << ' ' << a;
    cout << endl;
    cout << "arr3:";
    for (auto a : arr3) cout << ' ' << a;
    cout << endl;
    int res2 = table_type::compare(table_type(arr1, mapping), 0, 5, table_type(arr2, mapping), 0, 5);
    if (res2 < 0)
        cout << "arr1 is smaller than arr2\n";
    else if (res2 > 0)
        cout << "arr1 is bigger than arr2\n";
    else
        cout << "arr1 is equal to arr2\n";
    int res3 = table_type::compare(table_type(arr1, mapping), 0, 5, table_type(arr3, mapping), 0, 5);
    if (res3 < 0)
        cout << "arr1 is smaller than arr3\n";
    else if (res3 > 0)
        cout << "arr1 is bigger than arr3\n";
    else
        cout << "arr1 is equal to arr3\n";

    cout << endl;
}

void test_ex() {
    cout << "test for advanced:\n";
    std::string s = "Hello World";
    auto v = hash_type(s);
    cout << s << ": len = " << v.m_len << ", hash = " << v.m_val << endl;

    // 怎样快速求出 Hello 首字母小写后的哈希值？
    // 移除前缀，再加个新前缀
    auto v2 = v.remove_prefix(std::string("H")).append_left(std::string("h"));
    s[0] = 'h';
    cout << s << ": len = " << v2.m_len << ", hash = " << v2.m_val << endl;

    // 怎样快速求出 World 首字母小写后的哈希值？现在可不是在最前面进行修改了
    // 首先需要知道，ASCII 表中，W = 87，w = 119，所以我们希望下标 6 的地方的字符增加 32
    // 我们可以造个下标 6 处为 32，其余地方全是 0 的哈希值
    std::vector<int> inc{0, 0, 0, 0, 0, 0, 32};
    auto v3 = v2 + hash_type(inc);
    s[6] += 32;
    cout << s << ": len = " << v3.m_len << ", hash = " << v3.m_val << endl;

    // 检验一下
    auto v4 = hash_type(s);
    cout << s << ": len = " << v4.m_len << ", hash = " << v4.m_val << endl;

    // 怎么把所有的小写转成大写？
    // 把相应的位都减去 32
    auto v32_5 = hash_type::single(32).repeat_for(5);
    auto v0 = hash_type::zero(1);
    auto v5 = v4 - v32_5.append_right(v0).append_right(v32_5);
    for (auto &c : s) c = std::toupper(c);
    cout << s << ": len = " << v5.m_len << ", hash = " << v5.m_val << endl;
}

int main() {
    // 对于一个模数、一个 base，全局只需要预处理一次
    // 对字符串进行哈希时， base 设为 128 就足够
    // 为了避免被 hack，可以使用 set_random_base 来设置 base
    hash_type::s_info.set_base(128);
    hash_type::s_info.prepare(1000);
    test_hash_type();
    test_hash_table();
    test_lcp();
    test_ex();
}
/*
#输出如下
test for hash type:
abcd: len = 4, hash = 211349857
efg: len = 3, hash = 1700709
abcdefg: len = 7, hash = 722597612
abcdefg: len = 7, hash = 722597612
abcdefg: len = 7, hash = 722597612
efg: len = 3, hash = 1700709
abcd: len = 4, hash = 211349857
abcdefgabcdefgabcdefg: len = 21, hash = 727137999

test for hash table:
abcd: len = 4, hash = 211349857
efg: len = 3, hash = 1700709

test for compare:
xabcy and xabcz's lcp length = 4
abcy is smaller than abcz
arr1: 30 -10 0 0 0 0
arr2: 30 118 0 0 0 0
arr3: 30 3 0 0 0 0
arr1 is equal to arr2
arr1 is bigger than arr3

test for advanced:
Hello World: len = 11, hash = 468107406
hello World: len = 11, hash = 468107438
hello world: len = 11, hash = 476355061
hello world: len = 11, hash = 476355061
HELLO WORLD: len = 11, hash = 232941766

*/