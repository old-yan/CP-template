#include "DS/GlobalRangeCounter.h"
#include "IO/FastIO.h"

OY::GlobalRangeCounter<std::string, 1009> GRC;
void test() {
    // 本模板功能很简单，不多介绍
    std::string arr[5] = {"hello", "app", "apple", "hello", "app"};
    GRC.reset(arr, arr + 5);

    cout << "\"app\"  count of arr[1~3] = " << GRC.query(1, 3, "app") << endl;
    cout << "\"app\"  count of arr[1~4] = " << GRC.query(1, 4, "app") << endl;
    cout << "\"hell\" count of arr[0~4] = " << GRC.query(0, 4, "hell") << endl;
}

int main() {
    test();
}
/*
#输出如下
"app"  count of arr[1~3] = 1
"app"  count of arr[1~4] = 2
"hell" count of arr[0~4] = 0

*/