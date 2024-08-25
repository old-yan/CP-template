### 一、模板类别

​	数据结构：侧视图。

​	练习题目：

1. [P4198 楼房重建](https://www.luogu.com.cn/problem/P4198)
2. [#515. 【UR #19】前进四](https://uoj.ac/problem/515)



### 二、模板功能


#### 1.建立模板

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `typename Compare` ，表示比较函数的类型。

   构造参数 `size_type length` ，表示区间长度。

2. 时间复杂度

     $O(1)$ 。

3. 备注

   本模板处理的问题为侧视图问题，即将所有物品排成一队，大的能够遮住小的，查询能够看到的物品数。
   
   本模板为离线模板，支持单点修改，和区间查询。

#### 2.添加单点值修改(add_modify)

1. 数据类型

   输入参数 `size_type i` ，表示要修改的点的下标。
   
   输入参数 `const Tp &val` ，表示要修改为的点值。

2. 时间复杂度

    $O(1)$ 。
   
   
#### 3.添加前缀范围内查询(add_query)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的前缀范围。

2. 时间复杂度

    $O(1)$ 。
    
3. 备注

   本方法表示查询从下标 `0` 处进行观察，在下标 `[0, i]` 范围内能够观察到的建筑物数量。
   
   换句话说，就是 `[0, i]` 下标范围内的不同的前缀最大值数量。
   
#### 4.获取解(solve)

1. 数据类型

   输入参数 `const Tp &min` ，表示元素的最小值。
   
   输入参数 `const Tp &max` ，表示元素的最大值。

2. 时间复杂度

    $O(n\log n)$ 。

3. 备注

   本方法获取所有前缀查询的解。
   
   参数 `min` 要求小于等于所有元素的值；参数 `max` 要求严格大于所有元素的值。
   
   如果传递 `Compare` 为 `std::greater<>` ，即想查询不同的前缀最小值数量，则要求参数 `min` 大于等于所有元素的值；参数 `max` 严格小于所有元素的值。也就是说， `min`  `max` 需要根据比较函数的具体重载来传递。

### 三、模板示例

```c++
#include "DS/OfflineSideView.h"
#include "IO/FastIO.h"

int main() {
    OY::OFFLINESV::Solver<int> S(4);
    S.add_modify(0, 100);
    S.add_modify(1, 120);
    S.add_modify(2, 80);
    S.add_modify(3, 140);

    // 在 [100, 120, 80, 140]，只能看到三个建筑
    S.add_query(3);

    S.add_modify(2, 130);

    // 在 [100, 120, 130, 140]，可以看到四个建筑
    S.add_query(3);

    S.add_modify(0, 125);
    // 在 [125, 120, 130]，可以看到两个建筑
    S.add_query(2);

    auto res = S.solve();
    cout << "building in view[100, 120, 80, 140]: " << res[0] << endl;
    cout << "building in view[100, 120, 130, 140]: " << res[1] << endl;
    cout << "building in view[125, 120, 80]: " << res[2] << endl;
}
```

```
#输出如下
building in view[100, 120, 80, 140]: 3
building in view[100, 120, 130, 140]: 4
building in view[125, 120, 80]: 2

```

