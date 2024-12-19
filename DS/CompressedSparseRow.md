### 一、模板类别

​	数据结构：压缩稀疏行存储。

​	练习题目：

1. [find the safest road](https://acm.hdu.edu.cn/showproblem.php?pid=1596)
2. [Question for the Leader](https://acm.hdu.edu.cn/showproblem.php?pid=5329)
3. [Hotaru's problem](https://acm.hdu.edu.cn/showproblem.php?pid=5371)
4. [P3246 [HNOI2016] 序列](https://www.luogu.com.cn/problem/P3246)
5. [P3261 [JLOI2015] 城池攻占](https://www.luogu.com.cn/problem/P3261)
6. [P3402 可持久化并查集](https://www.luogu.com.cn/problem/P3402)
7. [P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)
8. [P3919 【模板】可持久化线段树 1（可持久化数组）](https://www.luogu.com.cn/problem/P3919)
9. [P4062 [Code+#1] Yazid 的新生舞会](https://www.luogu.com.cn/problem/P4062)
10. [P4151 [WC2011] 最大XOR和路径](https://www.luogu.com.cn/problem/P4151)

### 二、模板功能

​	   本模板与 `LinkBucket` 的区别是，本模板不能边读边写，必须在全部 `push_back` 结束后，执行 `get_buckets` ，然后才可以进行读操作。

​	   本模板在 `get_buckets` 之后不能再对 `buckets` 做进一步的修改。 

### 三、模板示例

```c++
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
```

```
#输出如下
apple dog 
orange cat 
banana pear 

```

