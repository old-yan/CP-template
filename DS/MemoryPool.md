### 一、模板类别

​	数据结构：内存池。

### 二、模板功能

​	本数据结构用于建立自己的内存池，从而减少 `new` 和 `delete` 的时间消耗。

使用方法：

1. 如果某一数据类型需要进行大量的、零碎的动态分配，我们可以预估程序运行过程的分配总量，然后在程序一开始调用 `_reserve` 方法，将一大块所需空间直接分配到自己手里。
2. 重载 `operator new`，以后在申请分配该元素类型的空间时，由我们预先分配的空间割一小块分配给它。
3. 重载 `operator delete` ，以后在删除某元素指针时，将其空间会受到我们手里。

备注：

1. 模板参数一为元素类型，模板参数二为默认分配块大小，默认值为 `1<<15`，也就是说默认一次性申请 `32768` 个元素的空间块到手里。如果单个元素很大，需要手动将模板参数二改小一点。
2. 即使一开始无法准确预留分配总量，甚至一开始不进行预留，也是可以的。当 `new` 的时候，如果检测到没有可分配的空间，将会自动调用 `reserve` 函数，按照模板参数二的大小申请一大块空间。
3. `recycle` 函数是 `operator delete` 的替代品。由于未知原因，一些情况下调用`operator delete` 速度会慢一些，此时可以使用`recycle` 。（当然，只会慢一丁点）

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "DS/MemoryPool.h"

int main() {
    //使用本内存池的方法有两种

    //方法一：如果可以找到类型的基类，可以直接继承内存池
    struct node:OY::MemoryPool<node,4>{
        node(){
            char c[20];
            sprintf(c,"node address:%x",this);
            cout<<c<<endl;
        }
    };
    //预留三个空间
    node::_reserve(3);
    //可以看到，动态申请的三个指针，在内存上是挨着的，这不是巧合，因为这是我们手动分配的
    node *p1=new node;
    node *p2=new node;
    node *p3=new node;
    //将 p3 指向的空间释放之后，申请 p4，可以使用 p3 刚刚回收的空间
    delete p3;
    node *p4=new node;
    //预留的空间用完后，再次申请空间，就和之前的不连续了
    //可以看到 p5,p6,p7,p8 是连续的
    node *p5=new node;
    node *p6=new node;
    node *p7=new node;
    node *p8=new node;
    //p9,p10,p11,p12 是连续的
    node *p9=new node;
    node *p10=new node;
    node *p11=new node;
    node *p12=new node;

    //方法二：如果找不到类型的基类，则需要手动重载运算符
    //比如写了个 myVector 继承 std::vector ，我们当然不可能让 std::vector 去继承 MemoryPool
    class myVector:public std::vector<int>{
    public:
        myVector(int length):std::vector<int>(length){
            char c[20];
            sprintf(c,"myVector address:%x",this);
            cout<<c<<endl;
        }
        void *operator new(size_t count){
            return OY::MemoryPool<myVector>::operator new(count);
        }
        void operator delete(void* ptr){
            OY::MemoryPool<myVector>::operator delete(ptr);
            //OY::MemoryPool<myVector>::recycle((myVector*)ptr);
        }
    };
    //可以看到 vec1,vec2,vec3 在内存上是连续的（间隔 24 个字节是 vector 体积）
    myVector* vec1=new myVector(100);
    myVector* vec2=new myVector(200);
    myVector* vec3=new myVector(50);
}
```

```
#输出如下
node address:c55d6f70
node address:c55d6f71
node address:c55d6f72
node address:c55d6f72
node address:c55d1600
node address:c55d1601
node address:c55d1602
node address:c55d1603
node address:c55d6f90
node address:c55d6f91
node address:c55d6f92
node address:c55d6f93
myVector address:c54c2490
myVector address:c54c24a8
myVector address:c54c24c0

```

