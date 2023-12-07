#include "IO/FastIO.h"
#include "STR/Manacher.h"

int main() {
    std::string s = "abcdaeeabc";
    OY::Manacher<true, true> mc(s);
    for (int l = 0; l < s.size(); l++) {
        for (int r = l; r < s.size(); r++) {
            cout << s.substr(l, r - l + 1) << " is " << (mc.query(l, r) ? "palindromic\n" : "not palindromic\n");
        }
    }
}
/*
#输出如下
a is palindromic
ab is not palindromic
abc is not palindromic
abcd is not palindromic
abcda is not palindromic
abcdae is not palindromic
abcdaee is not palindromic
abcdaeea is not palindromic
abcdaeeab is not palindromic
abcdaeeabc is not palindromic
b is palindromic
bc is not palindromic
bcd is not palindromic
bcda is not palindromic
bcdae is not palindromic
bcdaee is not palindromic
bcdaeea is not palindromic
bcdaeeab is not palindromic
bcdaeeabc is not palindromic
c is palindromic
cd is not palindromic
cda is not palindromic
cdae is not palindromic
cdaee is not palindromic
cdaeea is not palindromic
cdaeeab is not palindromic
cdaeeabc is not palindromic
d is palindromic
da is not palindromic
dae is not palindromic
daee is not palindromic
daeea is not palindromic
daeeab is not palindromic
daeeabc is not palindromic
a is palindromic
ae is not palindromic
aee is not palindromic
aeea is palindromic
aeeab is not palindromic
aeeabc is not palindromic
e is palindromic
ee is palindromic
eea is not palindromic
eeab is not palindromic
eeabc is not palindromic
e is palindromic
ea is not palindromic
eab is not palindromic
eabc is not palindromic
a is palindromic
ab is not palindromic
abc is not palindromic
b is palindromic
bc is not palindromic
c is palindromic

*/