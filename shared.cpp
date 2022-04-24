#include "IO/FastIO.h"
#include "STR/MinimalSequence.h"

int main() {
    std::string s = "59813621";
    for (int i = 0; i < s.size(); i++) {
        cout << "starts with index " << i << " : " << s.substr(i) + s.substr(0, i) << endl;
    }

    auto start = OY::MinimalSequence(s.begin(), s.end());
    cout << s.substr(start) + s.substr(0, start) << " is the smallest\n";
}