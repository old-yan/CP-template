### 一、模板类别

​	数据结构：可持久化左偏树。

### 二、模板功能

​		本数据结构的功能与实现与左偏树几乎一致，在此之外仅仅添加了可持久化的功能。所以与左偏树相同部分的内容不再介绍。

​		在可持久化数据结构中，不同版本的数据结构可能会共享很多数据，因此，当某一个版本进行写操作时，必须将涉及到的数据拷贝出来一份再进行修改，以避免对其他版本造成影响。

#### 1.拷贝(copy)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在可持久化数据结构中，一棵树的拷贝就是其根节点的拷贝。至于子孙结点，并不需要即时全部拷贝，等到之后进行访问的时候，才会逐个进行拷贝。
   
   **注意：**本函数只涉及根结点的拷贝。


### 三、模板示例

```c++
#include "DS/PersistentLeftist.h"
#include "IO/FastIO.h"

int main() {
    OY::VectorPerLeftistHeap<int, std::less<int>> S;

    // 初始堆含有五个元素
    int A[5] = {400, 200, 500, 800, 600};
    for (int a : A) S.push(a);

    // 第二个堆多了一个元素
    auto S2 = S.copy();
    S2.push(250);

    // 第三个堆少了一个元素
    auto S3 = S.copy();
    S3.pop();

    // 第四个堆是第三个堆和自己的合并，也就是元素数量翻倍
    auto S4 = S3.copy();
    S4.join(S3);

    // 现在我们来观测一下每个堆里的东西
    auto print_and_clear = [&](OY::VectorPerLeftistHeap<int, std::less<int>> &x) {
        while (!x.empty()) {
            cout << x.top() << ' ';
            x.pop();
        }
        cout << '\n';
    };

    cout << "S :" << endl;
    print_and_clear(S);

    cout << "S2 :" << endl;
    print_and_clear(S2);

    cout << "S3 :" << endl;
    print_and_clear(S3);

    cout << "S4 :" << endl;
    print_and_clear(S4);
}
```

```
#输出如下
S :
800 600 500 400 200 
S2 :
800 600 500 400 250 200 
S3 :
600 500 400 200 
S4 :
600 600 500 500 400 400 200 200 

```

