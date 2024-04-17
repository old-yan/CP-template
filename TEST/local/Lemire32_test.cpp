#include "IO/FastIO.h"
#include "MATH/Lemire32.h"

int main() {
    uint32_t P = 101;
    OY::Lemire32 L(P);

    uint32_t a = 321;
    cout << a << " / " << P << " = " << a / L << endl;
    cout << a << " % " << P << " = " << a % L << endl;
}
/*
#输出如下
321 / 101 = 3
321 % 101 = 18

*/