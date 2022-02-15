### 一、模板类别

​	数据结构：`kd` 树

### 二、模板功能

#### 1.建立 KD 树

1. 数据类型

   模板参数 `typename _Tp`​ ，表示点坐标的数据类型。

   模板参数 `typename _Fp` ，表示点的附带数据。

   模板参数 `typename _Operation` ，表示点的附带数据的运算。默认为 `std::plus<_Fp>` ，即 `_Fp` 元素的加法类。

   模板参数 `int _K`​ ，表示维度。默认为 `2` ，表示平面。

   模板参数 `int _N` ，表示叶结点大小。当一个叶结点大小超过这个值时，会进行分裂。默认为 `31` 。要求至少为 `5` 。

   构造参数 `_Operation __op` ，表示具体的区间操作函数。默认为 `_Operation` 类的默认实例。本参数接收类型有：普通函数，函数指针，仿函数，匿名函数，泛型函数。

   构造参数 `_Fp __defaultValue` ，表示附带数据的默认值，默认值为 `_Fp` 类的默认实例。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   `KD` 树管理的是 `k` 维空间中的点集，通过在各个维度不断二分的方法进行管理。

   一般来说，`_Fp` 可以设定为 `int`  类型，表示在这个坐标位置上的点的数量。然后令 `_Operation` 为 `_Fp` 类的加法类，表示点的数量之间进行加法运算。这样就便于通过方法函数查询某些区域内的点的总数。当然，在不统计点数的时候可以将 `_Fp` 属性和 `_Operation`运算忽略或者重载。
   
   `KD` 树处理的问题五花八门，所以只能覆盖一些常调用的方法，其他方法需要照葫芦画瓢进行编写。
   
   `KD` 树随机数据下运行速度非常快，但在数据特意针对 `KD` 树的极限情况下表现不佳。

#### 2.清空

1. 数据类型

2. 时间复杂度

   $O(1)$ 。


#### 3.预备插入点

1. 数据类型

   输入参数 ` const std::initializer_list<_Tp> &__pos` ，表示要插入的点的坐标。

   输入参数 `_Fp __value​` ，表示要插入的点所携带的信息。默认为 `1` ，表示当前坐标上的点数为 `1` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法可以高效地批量插入点，在调用本方法之前请确保树为空树。

   本方法调用之后，该点并不会立刻出现在树中。只有在调用下一个方法之后，点才能出现在树中。
   
   之所以不像别的数据结构一样设立 `reset` 方法以及从区间构建树的方法，是因为在树外很难以统一的形式在一个区间保存点信息。所以设立本方法和下一个方法，配合实现。

#### 4.预备建树

1. 数据类型

2. 时间复杂度

   $O(n \cdot \log n)$ 。

3. 备注

   本方法将之前预备插入的所有点构建为平衡的树。

#### 5.插入点

1. 数据类型

   输入参数 ` const std::initializer_list<_Tp> &__pos` ，表示要插入的点的坐标。

   输入参数 `_Fp __value​` ，表示要插入的点所携带的信息。默认为 `1` ，表示当前坐标上的点数为 `1` 。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法用于灵活插入结点。

   本方法调用后，该点会立刻出现在树中。

#### 6.删除点

1. 数据类型

   输入参数 ` const std::initializer_list<_Tp> &__pos` ，表示要删除的点的坐标。

   返回类型 `bool` ，表示是否删除成功。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法根据坐标去删除点；如果有多个点符合坐标要求，只删除一个。

#### 7.查找点

1. 数据类型

   输入参数 ` const std::initializer_list<_Tp> &__pos` ，表示要查找的点的坐标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法根据坐标去查找点；如果有多个点符合坐标要求，只返回一个。

#### 8.查询区域中的点数

1. 数据类型

   输入参数 `const _Range &__range`，表示要搜索的区域。

   返回类型 `int` ，表示区域中的点数。

2. 时间复杂度

   $O(\log^{k} n)$ 。

3. 备注

   本方法要求 `_Range` 类支持 `intersect` 、`contain_rect` 、`contain_point` 三种方法。对于不同的区域，这三个方法的实现也有不同，目前已给出矩形区域和圆形区域的 `_Range` 类的写法，可以参考。

#### 9.查询矩形区域中的点数

1. 数据类型

   输入参数 `const KDTreeRectRange<_Tp,_K> &__range` ，表示要搜索的矩形。

2. 时间复杂度

   同上。

3. 备注

   本方法实际上是在调用上一方法。

   具体写法参见示例。

#### 10.查询圆形区域中的点数

1. 数据类型

   模板参数 `typename _TpDistance` ，表示距离的数据类型。默认为 `_Tp` 。

   输入参数 `const KDTreeCircleRange<_Tp,_K,_TpDistance> &__range` ，表示要搜索的圆形。

2. 时间复杂度

   同上。

3. 备注

   本方法实际上是在调用上上方法。

   具体写法参见示例。

#### 11.查询最近点

1. 数据类型

   模板参数 `typename _TpDistance` ，表示距离的数据类型。

   模板参数 `bool _DropZero` ，表示是否忽略掉距离为零的点。

   模板参数 `typename _DistanceConvert` ，表示对每一维度的距离所进行的变形的变形函数的类。

   输入参数 `const std::initializer_list<_Tp> &__pos` ，表示源点。

   输入参数 `_DistanceConvert __conv` ，表示对每一维度的距离所进行的变形的具体函数。

2. 时间复杂度

   $O(n^{1-\frac 1 k})$，其中 `k` 表示维数。

#### 12.查询最近点（欧式距离）

1. 数据类型

   模板参数 `typename _TpDistance=_Tp` ，表示距离的数据类型。

   输入参数 `const std::initializer_list<_Tp> &__pos` ，表示源点。

2. 时间复杂度

   同上。

3. 备注

   本方法实际上是在调用上一方法，且排除距离为零的点。传递的变形函数为求平方函数。返回的也是最近点的距离的平方。

#### 13.查询最近点（曼哈顿距离）

1. 数据类型

   模板参数 `typename _TpDistance=_Tp` ，表示距离的数据类型。

   输入参数 `const std::initializer_list<_Tp> &__pos` ，表示源点。

2. 时间复杂度

   同上。

3. 备注

   本方法实际上是在调用上一方法，且排除距离为零的点。传递的变形函数为求绝对值函数。

#### 14.查询最远点

1. 数据类型

   模板参数 `typename _TpDistance` ，表示距离的数据类型。

   模板参数 `typename _DistanceConvert` ，表示对每一维度的距离所进行的变形的变形函数的类。

   输入参数 `const std::initializer_list<_Tp> &__pos` ，表示源点。

   输入参数 `_DistanceConvert __conv` ，表示对每一维度的距离所进行的变形的具体函数。

2. 时间复杂度

   同上。

#### 15.查询最远点（欧氏距离）

1. 数据类型

   模板参数 `typename _TpDistance=_Tp` ，表示距离的数据类型。

   输入参数 `const std::initializer_list<_Tp> &__pos` ，表示源点。

2. 时间复杂度

   同上。

3. 备注

   本方法实际上是在调用上一方法。传递的变形函数为求平方函数。返回的也是最远点的距离的平方。

#### 16.查询最远点（曼哈顿距离）

1. 数据类型

   模板参数 `typename _TpDistance=_Tp` ，表示距离的数据类型。

   输入参数 `const std::initializer_list<_Tp> &__pos` ，表示源点。

2. 时间复杂度

   同上。

3. 备注

   本方法实际上是在调用上一方法。传递的变形函数为求绝对值函数。

###  三、模板示例

```c++
#include "DS/KDTree.h"
#include "IO/FastIO.h"

int main() {
    //默认的 KT 树就是二维的
    OY::KDTree<int, int, std::plus<int>, 2, 5> T;
    //使用预备方法，批量插入一大堆点
    T.prepared_insert({1, 1});
    T.prepared_insert({1, 4});
    T.prepared_insert({1, 2});
    T.prepared_insert({2, 3});
    T.prepared_insert({2, 1});
    T.prepared_insert({3, 2});
    T.prepared_insert({4, 3});
    T.prepared_insert({4, 1});
    T.prepared_insert({6, 3});
    T.prepared_insert({6, 2});
    T.prepared_insert({6, 1});
    T.prepared_insert({5, 4});
    T.prepared_insert({5, 2});
    T.prepared_build();

    //查询横坐标 [1,6], 纵坐标 [2,4] 矩形里的点数
    cout << "1<=x<=6, 2<=y<=4, points number: " << T.queryNumber_rect({{1, 2}, {6, 4}}) << endl;
    //查询以 [1, 1] 为圆心，5 为半径的圆形里的点数
    cout << "center=[1,1], radius=5, points number: " << T.queryNumber_circle({{1, 1}, 25}) << endl;
    //查询到原点的曼哈顿距离最小的点有多近
    cout << "minimum Manhattan-distance to [0, 0] :" << T.queryClosest_Manhattan<long long>({0, 0}) << endl;
    //查询到原点的欧式距离最小的点有多近
    cout << "minimum Euclidean-distance to [0, 0] :" << std::sqrt(T.queryClosest_Euclidean<long long>({0, 0})) << endl;
    //查询到原点的曼哈顿距离最大的点有多远
    cout << "maximum Manhattan-distance to [0, 0] :" << T.queryFurthest_Manhattan<long long>({0, 0}) << endl;
    //查询到原点的欧式距离最大的点有多近
    cout << "maximum Euclidean-distance to [0, 0] :" << std::sqrt(T.queryFurthest_Euclidean<long long>({0, 0})) << endl;
    //查询树中有没有原点
    auto it = T.find({0, 0});
    cout << (it ? "[0, 0] found\n" : "[0, 0] not found\n");

    //*******************************************************************************
    //以下演示如何编写自定义方法，求到 [3, 1] 的欧式距离最接近 4 的点的坐标
    //首先可以取出树的根
    using treenode = decltype(T)::treenode;
    treenode *root = T.m_root;
    int x = 3, y = 1;
    double ans = 999;
    std::pair<int, int> ans_pos;
    //先写个 get_distance 函数
    auto get_distance = [&](int xx, int yy) {
        xx -= x;
        yy -= y;
        return std::sqrt(xx * xx + yy * yy);
    };
    auto is_closer_to_4 = [](double dis1, double dis2) {
        return std::abs(dis1 - 4) < std::abs(dis2 - 4);
    };
    //书写方法，第一个参数传递匿名函数本身，第二个参数传递当前结点，第三个参数表示当前结点的切割维度
    auto dfs = [&](auto self, treenode *cur, int i) {
        if (!cur) return;
        //注意要剪枝，如果没有剪枝那么相当于搜索所有的点，这个数据结构将毫无价值
        if (!cur->contain_point({x, y})) {
            //把四个角的距离算出来
            double dis1 = get_distance(cur->min[0], cur->min[1]);
            double dis2 = get_distance(cur->min[0], cur->max[1]);
            double dis3 = get_distance(cur->max[0], cur->min[1]);
            double dis4 = get_distance(cur->max[0], cur->max[1]);
            //如果四个角的距离都比 4 大，说明最有希望接近 4 的就是那个近角
            if (dis1 >= 4 and dis2 >= 4 and dis3 >= 4 and dis4 >= 4) {
                double mindis = std::min({dis1, dis2, dis3, dis4});
                //如果连近角都无法刷新答案，那直接告别这个树结点
                if (!is_closer_to_4(mindis, ans)) return;
            }
            //如果四个角的距离都比 4 小，说明最有希望接近 4 的就是那个远角
            if (dis1 <= 4 and dis2 <= 4 and dis3 <= 4 and dis4 <= 4) {
                double maxdis = std::max({dis1, dis2, dis3, dis4});
                //如果连远角都无法刷新答案，那直接告别这个树结点
                if (!is_closer_to_4(maxdis, ans)) return;
            }
        }

        //有左孩子，那说明非叶结点，往下搜
        if (cur->lchild) {
            //递归到子结点的顺序也可以考虑考虑
            //取左孩子的中心和右孩子中心，看哪个离 4 近就先搜谁
            double l_dis = get_distance((cur->lchild->min[0] + cur->lchild->max[0]) / 2.0, (cur->lchild->min[1] + cur->lchild->max[1]) / 2.0);
            double r_dis = get_distance((cur->rchild->min[0] + cur->rchild->max[0]) / 2.0, (cur->rchild->min[1] + cur->rchild->max[1]) / 2.0);
            if (is_closer_to_4(l_dis, r_dis)) {
                self(self, cur->lchild, (i + 1) % 2);
                self(self, cur->rchild, (i + 1) % 2);
            } else {
                self(self, cur->rchild, (i + 1) % 2);
                self(self, cur->lchild, (i + 1) % 2);
            }
        }
        //如果是叶结点，那么暴力遍历它的所有点
        else {
            for (auto p : cur->points) {
                double dis = get_distance(p->pos[0], p->pos[1]);
                if (is_closer_to_4(dis, ans)) {
                    ans = dis;
                    ans_pos = {p->pos[0], p->pos[1]};
                }
            }
        }
    };
    dfs(dfs, root, 0);
    cout << "ans point is:" << ans_pos.first << " " << ans_pos.second << endl;
    cout << "its distance to [3,1] is:" << ans << endl;
}
```

```
#输出如下
1<=x<=6, 2<=y<=4, points number: 9
center=[1,1], radius=5, points number: 11
minimum Manhattan-distance to [0, 0] :2
minimum Euclidean-distance to [0, 0] :1.414214
maximum Manhattan-distance to [0, 0] :9
maximum Euclidean-distance to [0, 0] :6.708204
[0, 0] not found
ans point is:6 3
its distance to [3,1] is:3.605551

```

