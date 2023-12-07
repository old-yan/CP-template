#include "IO/FastIO.h"
#include "STR/MinimalSequence.h"

int main() {
    std::string s = "59813621";
    for (int i = 0; i < s.size(); i++) {
        cout << "starts with index " << i << " : " << s.substr(i) + s.substr(0, i) << endl;
    }

    auto start = OY::get_minimal_rotation(s.begin(), s.end());
    cout << s.substr(start) + s.substr(0, start) << " is the smallest\n";
}
/*
#输出如下
starts with index 0 : 59813621
starts with index 1 : 98136215
starts with index 2 : 81362159
starts with index 3 : 13621598
starts with index 4 : 36215981
starts with index 5 : 62159813
starts with index 6 : 21598136
starts with index 7 : 15981362
13621598 is the smallest

*/