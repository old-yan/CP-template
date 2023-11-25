#include "IO/FastIO.h"
#include "MATH/PrimeCheck.h"

int main() {
    for (int i = 1000; i <= 1100; i++) {
        if (OY::is_prime32(i)) {
            cout << i << " is prime\n";
        }
    }
}
/*
#输出如下
1009 is prime
1013 is prime
1019 is prime
1021 is prime
1031 is prime
1033 is prime
1039 is prime
1049 is prime
1051 is prime
1061 is prime
1063 is prime
1069 is prime
1087 is prime
1091 is prime
1093 is prime
1097 is prime

*/