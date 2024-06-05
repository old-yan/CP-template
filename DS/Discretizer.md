### 一、模板类别

​	数据结构：离散化数组

​	练习题目：

1. [P3201 [HNOI2009] 梦幻布丁](https://www.luogu.com.cn/problem/P3201)
2. [P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)


### 二、模板功能

#### 1.建立离散化数组

1. 数据类型

   模板参数 `typename Tp`​ ，表示离散化的元素类型。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本数据结构继承自 `std::vector` ，所以可以使用 `std::vector` 的成员方法。比如，为了减少数组的扩容复制次数，可以使用 `reserve` 方法预留空间。
   
   以下只介绍特有的方法。


#### 2.输入元素(operator<<)

1. 数据类型

   输入参数 `const Tp &item` ，表示要参与离散化的元素。

2. 时间复杂度

   $O(1)$ 。
   

#### 3.输入元素数组(operator<<)

1. 数据类型

   参数 `const std::vector<Tp> &items` ，表示要参与离散化的元素所组成的数组。

2. 时间复杂度

   $O(n)$  。

#### 4.输入元素二维数组(operator<<)

1. 数据类型

   参数 `const std::vector<std::vector<Tp>> &items` ，表示要参与离散化的元素所组成的二维数组。

2. 时间复杂度

   $O(n)$  。

#### 5.输入元素对(operator<<)

1. 数据类型

   参数 `const std::pair<Tp, Tp> &items` ，表示要参与离散化的元素所组成的对。

2. 时间复杂度

   $O(1)$  。

#### 6.输入元素定长数组(operator<<)

1. 数据类型

   参数 `const std::array<Tp, N> &items` ，表示要参与离散化的元素所组成的定长数组。

2. 时间复杂度

   $O(n)$  。

#### 7.准备(prepare)

1. 数据类型

2. 时间复杂度

   $O(n\log (n))$ 。
   
3. 备注

   本方法会对数组内的元素排序、去重，之后数组就不再接受新元素。


#### 8.获取排名(rank)

1. 数据类型

   输入参数 `const Tp &item`，表示要查询离散化编号的元素

2. 时间复杂度

   $O(log(n))$ 。
   
3. 备注

   如果该元素不在集合中，则返回若该元素在集合中，将会处于的名次。


### 三、模板示例

```c++
#include "DS/Discretizer.h"
#include "IO/FastIO.h"

int main() {
    OY::Discretizer<int> D;
    D << 11 << 22 << 33;
    std::vector<int> vec{15, 25, 35};
    std::vector<std::vector<int>> vec_2d{{100, 200}, {400, 300}, {15, 25}};
    std::pair<int, int> p{5, 300};
    std::array<int, 5> arr{4, 5, 6, 7, 8};
    D << vec << vec_2d << p << arr;
    // 准备后不会再接受数据
    D.prepare();
    for (int i = 0; i < D.size(); i++) {
        cout << "D[" << i << "]=\t" << D[i] << endl;
    }
    cout << "rank of " << 24 << "=" << D.rank(24) << endl;
    cout << "rank of " << 25 << "=" << D.rank(25) << endl;
    cout << "rank of " << 26 << "=" << D.rank(26) << endl;
}
```

```
#输出如下
D[0]=	4
D[1]=	5
D[2]=	6
D[3]=	7
D[4]=	8
D[5]=	11
D[6]=	15
D[7]=	22
D[8]=	25
D[9]=	33
D[10]=	35
D[11]=	100
D[12]=	200
D[13]=	300
D[14]=	400
rank of 24=8
rank of 25=8
rank of 26=9
```

