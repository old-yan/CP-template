#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/HashLCP.h"
#include "STR/SequenceMultiHash.h"

using mint0 = OY::mint998244353;
using mint1 = OY::mint1000000007;
int main() {
    // 可以只有一组模数，也可以多模数
    // 对字符串进行哈希时， base 设为 128 就足够
    // 为了避免被 hack，可以使用 set_random_base 来设置 base
    // 在使用前要先做预备
    using hash_type = OY::SEQHASH::SeqHash<mint0, mint1>;
    hash_type::s_info.set_base(128, 131);
    hash_type::s_info.prepare(1000);

    // 给出主串
    std::string s = "abcabbbabcb";
    // 构造助手
    auto S = OY::make_hash_LCP<mint0, mint1>(s);
    // 查询 lcp
    cout << s.substr(0) << " and " << s.substr(7) << " 's lcp = " << S.lcp(0, 7) << endl;
    // 查询带上限的 lcp
    cout << s.substr(0, 2) << " and " << s.substr(7, 2) << " 's lcp = " << S.lcp(0, 7, 2) << endl;
    // 比较子串
    int res = S.compare(0, 3, 7, 10);
    if (res < 0)
        cout << s.substr(0, 4) << " is smaller than " << s.substr(7, 4) << endl;
    else if (res > 0)
        cout << s.substr(0, 4) << " is bigger than " << s.substr(7, 4) << endl;
    else
        cout << s.substr(0, 4) << " is equal to " << s.substr(7, 4) << endl;
}
/*
#输出如下
abcabbbabcb and abcb 's lcp = 3
ab and ab 's lcp = 2
abca is smaller than abcb

*/