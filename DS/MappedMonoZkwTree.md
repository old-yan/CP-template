### 一、模板类别

​	数据结构： `MappedMonoZkw` 线段树。

​	练习题目：

1. [弩蚊怒夏](https://ac.nowcoder.com/acm/problem/226861)
2. [小红的数组操作](https://ac.nowcoder.com/acm/problem/277952)

### 二、模板功能


​		本模板基本类似 `MonoZkwTree` 。区别在于， `MonoZkw` 的叶结点处，只有一个普普通通的元素值；而 `MappedMonoZkw` 的叶结点处，可能并非一个普通元素值。可能是一个容器，由这个容器推出一个代表元素作为此叶结点处的值；也可能是一些别的类型的东西，经过计算得到元素值。

​		通过 `Mapping` 模板参数，可以控制叶结点处的数据类型，以及计算叶结点的值的方法。默认的提供了 `MaxPqMonoZkwTree` 和 `MinPqMonoZkwTree` ，表示每个叶结点处有一个堆，由堆顶作为代表元素充当线段树的叶结点处的值。

### 三、模板示例

```c++
#include "DS/MappedMonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // 假如，有 10 个桶
    // 查询 query(l, r) 表示查询 bucket[l~r] 这些桶的最大值的和
    // 空桶的贡献是 0
    using monoid = OY::MonoSumTree<int>::group;
    OY::MaxPqMonoZkwTree<monoid, int, 0> S(10);
    // 记得对桶做修改之后，要调用 S.update
    S[2].push(25), S[2].push(50);
    S.update(2);
    
    S[4].push(30);
    S.update(4);
    
    S[5].push(100), S[5].push(8);
    S.update(5);
    
    S[8].push(88);
    S.update(8);
    
    cout << "S.query(4, 6) = " << S.query(4, 6) << endl;
}

int main() {
    test();
}
```

```
#输出如下
S.query(4, 6) = 130

```

