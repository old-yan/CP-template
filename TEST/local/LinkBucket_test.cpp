#include "DS/LinkBucket.h"
#include "IO/FastIO.h"

int main() {
    OY::LBC::LinkBucket<std::string, 1000, 10000> S(3, 5);
    using node = decltype(S)::node;
    S[0].push_back("apple");
    S[2].push_back("banana");
    S[1].push_back("orange");
    S[1].push_back("cat");
    S[0].push_back("dog");
    for (int i = 0; i < 3; i++) {
        for (auto &item : S[i])
            cout << item << ' ';
        cout << '\n';
    }
}
/*
#输出如下
dog apple
cat orange
banana
*/