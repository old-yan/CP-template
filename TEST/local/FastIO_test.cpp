#include "IO/FastIO.h"

int main() {

    // 读入一个整数 123
    int x;
    cin >> x;
    cout << x << '\n';

    // 读入一个字符串 "abc"
    std::string s;
    cin >> s;
    cout << s << '\n';

    // 读入一行字符串，失败，只读了 "this"
    std::string line;
    cin >> line;
    cout << line << '\n';

    // 读入一行字符串，成功，" is a line"
    getline(cin, line);
    cout << line << '\n';

    // 读入一行字符串，成功，"this is another line"
    getline(cin, line);
    cout << line << '\n';

    if (cin >> s)
        cout << "still have input\n";
    else
        cout << "no more input\n";
}
/*
#以下为 "in.txt" 中的内容
123
abc
this is a line
this is another line
*/
/*
#以下为程序运行之后， "out.txt" 中的内容
123
abc
this
 is a line
this is another line
no more input

*/