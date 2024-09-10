### 一、模板类别

​	数据结构： 压缩线段树。

​	练习题目：

1. [U311262 求区间后继](https://www.luogu.com.cn/problem/U311262)

### 二、模板功能


​		本模板为 `SegTree` 在只有单点修改、没有区间修改情况下的特化。由于没有懒标记的下传，所以通过虚树建树优化了空间，空间不会超过操作次数。

​		本模板要求提供模板参数 `typename Monoid` 半群类型。半群中规定了元素类型、幺元的值、元素结合操作。除此外，仍需和 `SegTree` 一样提供 `typename SizeType` 参数和 `template <typename> typename BufferType` 参数。

​		半群运算要求满足**结合律**。特别的， `add` 方法要求操作满足交换律。

​		为了方便使用，本模板提供了 `MinTree` ， `MaxTree` ， `GcdTree` ， `LcmTree` ， `BitAndTree` ， `BitOrTree` ， `BitXorTree` ， `SumTree` 八种特化形式，需要填写的参数更少。

#### 1.查询是否包含某元素(contains)

1. 数据类型

   输入参数 `SizeType i` ，表示要查询的元素位置。
   
   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   只有在初始化时创建，或者在后续通过 `add` ， `modify` 方式创建的元素，才会返回 `true` 。
   
   对于不包含的元素，仍然可以查询其值，返回幺元。
   
#### 2.枚举所有包含的元素(enumerate)

1. 数据类型

   输入参数 `Callback &&call` ，表示回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   只有在初始化时创建，或者在后续通过 `add` ， `modify` 方式创建的元素，才会调用回调。
   
### 三、模板示例

```c++
#include "DS/CompressedTree.h"
#include "IO/FastIO.h"

void test() {
    // Compressed 和 Seg 相同之处不再展示
    // 展示一个单点修改的求乘积树的写法
    int A[] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    // 幺元为 1
    struct Monoid {
        using value_type = uint32_t;
        static value_type identity() { return 1; }
        static value_type op(value_type x, value_type y) { return x * y; }
    };
    OY::CPTREE::Tree<Monoid> S(A, A + 9);
    cout << S << endl;
    S.modify(3, 100);
    cout << S << endl;
    cout << "prod = " << S.query_all() << endl;
}

int main() {
    test();
}
```

```
#输出如下
[0:4,1:9,2:2,3:3,4:5,5:7,6:8,7:1,8:6]
[0:4,1:9,2:2,3:100,4:5,5:7,6:8,7:1,8:6]
prod = 12096000

```

