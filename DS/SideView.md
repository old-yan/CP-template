### 一、模板类别

​	数据结构：侧视图。

​	练习题目：

1. [P4198 楼房重建](https://www.luogu.com.cn/problem/P4198)


### 二、模板功能


#### 1.建立模板

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `typename Compare` ，表示比较函数的类型。

   构造参数 `size_type length` ，表示区间长度。
   
   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。
   
   构造参数 `Tp min` ，表示元素最小值，或者说背景值。默认为 `std::numeric_limits<Tp>::min()` 。

2. 时间复杂度

     $O(n)$ 。

3. 备注

   本模板处理的问题为侧视图问题，即将所有物品排成一队，大的能够遮住小的，查询能够看到的物品数。
   
   本模板为在线模板，支持单点修改，和区间查询。
   
   若 `Compare` 为 `std::greater<>` ，即处理小物品能够遮住大物品的问题，构造参数的 `Tp min` 就需要传递最大值。

#### 2.建立模板

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到表里。

#### 3.单点值修改(modify)

1. 数据类型

   输入参数 `size_type i` ，表示要修改的点的下标。
   
   输入参数 `const Tp &val` ，表示要修改为的点值。

2. 时间复杂度

    $O(\log^2 n)$ 。
   
   
#### 4.查询前缀可以看到的建筑(presum)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的前缀范围。
   
   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

    $O(\log^2 n)$ 。
    
3. 备注

   本方法表示查询从下标 `0` 处进行观察，在下标 `[0, i]` 范围内能够观察到的建筑物数量。
   
   换句话说，就是 `[0, i]` 下标范围内的不同的前缀最大值数量。
   
#### 5.查询区间内可以看到的建筑(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左边界。
   
   输入参数 `size_type right` ，表示要查询的区间的右边界。（闭区间）
   
   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

    $O(\log^2 n)$ 。
    
3. 备注

   本方法表示查询从下标 `left` 处进行观察，在下标 `[left, right]` 范围内能够观察到的建筑物数量。
   
   换句话说，就是 `[left, right]` 下标范围内的不同的前缀最大值数量。
   
#### 6.获取全部区间可以看到的建筑(query_all)

1. 数据类型

   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。


### 三、模板示例

```c++
#include "DS/SideView.h"
#include "IO/FastIO.h"

int main() {
    OY::SV::Table<int> S(4);
    S.modify(0, 100);
    S.modify(1, 120);
    S.modify(2, 80);
    S.modify(3, 140);

    // 在 [100, 120, 80, 140]，只能看到三个建筑
    cout << "building in view[100, 120, 80, 140]: " << S.presum(3) << endl;

    S.modify(2, 130);
    // 在 [100, 120, 130, 140]，可以看到四个建筑
    cout << "building in view[100, 120, 130, 140]: " << S.presum(3) << endl;

    S.modify(0, 125);
    // 在 [125, 120, 130]，可以看到两个建筑
    cout << "building in view[125, 120, 80]: " << S.presum(2) << endl;
    // 在 [120, 130, 140]，可以看到三个建筑
    cout << "building in view[120, 130, 140]: " << S.query(1, 3) << endl;
}
```

```
#输出如下
building in view[100, 120, 80, 140]: 3
building in view[100, 120, 130, 140]: 4
building in view[125, 120, 80]: 2
building in view[120, 130, 140]: 3

```

