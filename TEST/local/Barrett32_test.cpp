#include "IO/FastIO.h"
#include "MATH/Barrett32.h"

int main() {
    uint32_t P = 101;
    OY::Barrett32 B(P);

    uint32_t a = 321;
    cout << a << " / " << P << " = " << a / B << endl;
    cout << a << " % " << P << " = " << a % B << endl;
}
/*
#输出如下
321 / 101 = 3
321 % 101 = 18

*/