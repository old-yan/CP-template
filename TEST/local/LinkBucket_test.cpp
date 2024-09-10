#include "DS/LinkBucket.h"
#include "IO/FastIO.h"

int main() {
    OY::LBC::LinkBucket<std::string> S(3, 6);
    S[0].push_front("apple");
    S[2].push_front("banana");
    S[2].push_front("pear");
    S[1].push_front("orange");
    S[1].push_front("cat");
    S[0].push_front("dog");
    S[2].pop_front();
    
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