#include "IO/FastIO.h"
#include "STR/ZAlgorithm.h"

int main() {
    //给出模式串并进行预处理
    std::string p = "abcdabceabcd";
    OY::ZAlgorithm_string Z(p);
    for (int i = 0; i < p.size(); i++) {
        int j = Z.query_Z(i);
        if (j > 0) {
            cout << "index " << i << ": " << p << " startwith " << p.substr(i, j) << endl;
        }
    }

    //给出文本串并进行查找第一次出现位置
    std::string s = "abcdabcdabcabcdabceabcdabceabcdabceabcd";
    int first = Z.contained_by(s.begin(), s.end());
    cout << first << " " << s.substr(first, p.size()) << endl;

    //如果要查找所有出现位置
    for (int l = -1, r = -1, i = 0; i < s.size(); i++) {
        int z = Z.adjust(l, r, i, s.size(), s);
        if (z == p.size()) {
            int index = i;
            cout << index << " " << s.substr(index, p.size()) << endl;
        }
    }

    //借助 Z 函数可以快速求出字符串周期
    std::string q = "abcabcabcabcabcabcabc";
    // q 长度为 21
    int n = q.size();
    OY::ZAlgorithm_string Z2(q);
    for (int i = 1; i < n; i++) {
        if (i + Z2.query_Z(i) == n && n % i == 0) {
            cout << i << " is minimum cycle of string " << q << endl;
            break;
        }
    }
}
/*
#输出如下
index 4: abcdabceabcd startwith abc
index 8: abcdabceabcd startwith abcd
11 abcdabceabcd
11 abcdabceabcd
19 abcdabceabcd
27 abcdabceabcd
3 is minimum cycle of string abcabcabcabcabcabcabc

*/