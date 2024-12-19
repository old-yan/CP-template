#include "DS/CompressedSparseRow.h"
#include "IO/FastIO.h"

int main() {
    OY::CSR::Container<std::string> S(3, 6);
    S[0].push_back("apple");
    S[2].push_back("banana");
    S[2].push_back("pear");
    S[1].push_back("orange");
    S[1].push_back("cat");
    S[0].push_back("dog");

    auto buckets = S.get_buckets();
    for (int i = 0; i < 3; i++) {
        for (auto &item : buckets[i])
            cout << item << ' ';
        cout << '\n';
    }
}
/*
#输出如下
apple dog 
orange cat 
banana pear 

*/