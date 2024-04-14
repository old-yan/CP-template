# CP-template
C++ template files for competitive programming.

#### 1.模板库特点：

1. ##### 兼容性高。

   支持 `gcc` ，`clang` ，`MSVC` 等不同编译器，`C++11` 之后可以使用。

2. ##### 使用方便。

   `FOR` 宏定义？ `i64` 缩写？编程老手都认识？ `No` ！本模板库中，不会使用这些花里胡哨的缩写，也不会假定使用者是老手。本模板，让任何码风的人都不会感到不适应。

   各个模板之间，尽量减少依赖关系，代码界限分明，在复制粘贴、提交代码时十分清晰便捷。

   在最新版本的 `TEST` 文件夹里，提供了本地运行代码，以及在若干 `oj` 题目上提交时的代码。

3. ##### 封装性好。

   模板往往以类的形式存在，通过成员方法来进行操作；在遇到需要同时建立多个树状数组、多个线段树等问题时，可以轻松适应。

4. ##### 拓展性好。

   可以在当前模板的基础上进行再次封装，例如区间排序平衡树就是在平衡树的基础上封装而成的。

5. ##### 零成本抽象。

   这可以说是最重要的一点，很多写算法模板库的人，写出来的模板非常狭隘，稍微一换场景就会损失效率。比如设计平衡树，结果直接默认写成 `map` ，那在遇到要以 `set` 处理的问题时，显然就要白白添加一个没用的 `value_type` ，这是本模板库不能接受的。本模板库一定会写成既可以拓展为 `set` 也可以拓展为 `map` 且均为最佳效率的形式。

#### 2.设计原则：

1.  `C++ style, not C style` 。

2. 代码格式化:
   
    ```
    { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Attach, AllowShortIfStatementsOnASingleLine: true, IndentCaseLabels: true, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false ,AllowShortCaseLabelsOnASingleLine: true,AllowShortLoopsOnASingleLine: true,AllowShortBlocksOnASingleLine: true}
    ```
    
3. 文件宏命名为：双下划线 + `OY` + 双下划线 + 模板名全称大写 + 双下划线
    模板参数命名为：大写每个单词首字母；
    类命名为：大写每个单词首字母；
    类外的编译期常量命名为：全大写+下划线分割；
    类内的编译期常量命名为：全小写+下划线分割；
    成员变量命名为：`m` +下划线分割单词
    成员函数命名为：下划线分割单词
    静态变量命名为： `s` +下划线+下划线分割单词
    形式参数命名为：下划线分割单词

4.  `C` 的函数不使用 `std` 前缀。

5. 对不保证隐式类型转换成功的场景，使用显式类型转换。

      如果代码出现 `bug` 或者设计问题，欢迎指出

#### 3.优秀的执行效率：

​	本模板库的数据结构，拥有极其优秀的运行速度。例如：

​	截止 `2024.04.14`，最快的 `K` 短路 https://www.luogu.com.cn/problem/P2483

​	截止 `2024.04.14`，最快的虚树 https://www.luogu.com.cn/problem/P2495

​	截止 `2024.04.14`，最快的区间排序线段树 https://www.luogu.com.cn/problem/P2824

​	截止 `2024.04.14`，最快的带懒更新的可并堆 https://www.luogu.com.cn/problem/P3261

​	截止 `2024.04.14`，最快的树上线性基 https://www.luogu.com.cn/problem/P3292

​	截止 `2024.04.14`，最快的带懒更新的线段树 https://www.luogu.com.cn/problem/P3373

​	截止 `2024.04.14`，最快的类树状数组结构 https://www.luogu.com.cn/problem/P3374

​	截止 `2024.04.14`，最快的最近公共祖先在线查询 https://www.luogu.com.cn/problem/P3379

​	截止 `2024.04.14`，最快的回滚并查集 https://www.luogu.com.cn/problem/P3402

​	截止 `2024.04.14`，最快的乘法逆元 https://www.luogu.com.cn/problem/P3811

​	截止 `2024.04.14`，最快的静态区间最值查询 https://www.luogu.com.cn/problem/P3865

​	截止 `2024.04.14`，最快的李超线段树 https://www.luogu.com.cn/problem/P4097

​	截止 `2024.04.14`，最快的 `FMT/FWT` https://www.luogu.com.cn/problem/P4717

​	截止 `2024.04.14`，最快的 `2-SAT` https://www.luogu.com.cn/problem/P4782

​	截止 `2024.04.14`，最快的类欧几里得算法 https://www.luogu.com.cn/problem/P5170

​	截止 `2024.04.14`，最快的回滚 `KMP` https://www.luogu.com.cn/problem/P5287

​	截止 `2024.04.14`，最快的线性基线段树 https://www.luogu.com.cn/problem/P5607

​	截止 `2024.04.14`，最快的 `Stoer-Wagner`算法 https://www.luogu.com.cn/problem/P5632

​	截止 `2024.04.14`，最快的动态树 https://www.luogu.com.cn/problem/P5649

​	截止 `2024.04.14`，最快的子序列自动机 https://www.luogu.com.cn/problem/P5826

​	截止 `2024.04.14`，最快的笛卡尔树 https://www.luogu.com.cn/problem/P5854

​	截止 `2024.04.14`，最快的势能线段树 https://www.luogu.com.cn/problem/P6242

​	截止 `2024.04.14`，最快的点双连通分量 https://www.luogu.com.cn/problem/P8435

​	截止 `2024.04.14`，最快的边双连通分量 https://www.luogu.com.cn/problem/P8436

​	截止 `2024.04.14`，最快的最小树形图算法 https://www.luogu.com.cn/problem/U210116

​	截止 `2024.04.14`，最快的 `GCD` 卷积 https://www.luogu.com.cn/problem/U151263

​	截止 `2024.04.14`，最快的树状数组 https://www.luogu.com.cn/problem/U187320

​	截止 `2024.04.14`，最快的势能线段树 https://uoj.ac/problem/170

​	截止 `2024.04.14`，最快的二维树状数组 [https://loj.ac/p/133](https://loj.ac/p/133)，[https://loj.ac/p/134](https://loj.ac/p/134)，[https://loj.ac/p/135](https://loj.ac/p/135)

​	截止 `2024.04.14`，最快的二维 `ST` 表 http://acm.hdu.edu.cn/showproblem.php?pid=2888

​	由于这样的例子实在太多，故只选最具有代表性的模板列出。

#### 4.力扣输入输出模板使用方法：

1. 目前支持的编译器有 `clang` / `gcc` / `MSVC` 。
2. 包含 `LeetcodeIO.h` 头文件；
3. 在包含头文件之前，加一行 `#define OY_LOCAL` 作为本地的标志；（也可以在编译参数里加 `-DOY_LOCAL`）
4. 在运行目录下放入 `in.txt` 和 `out.txt` 作为输入输出文件；如果找不到运行目录，可以随便输出一个字符串，看看 `out.txt` 生成到了哪里。
5. `Solution` 类的使用：需要在第二行注册要运行的成员方法名；
6. 自定义 `Class` 类的使用：需要在第一行注册类名+构造函数的所有参数类型；需要在第二行注册类名+所有要用到的成员方法名

使用时可以参考两张 `png` 图片示例。

#### 5.FAQ

1. 我的编程环境非常老旧，看到你的模板库代码花里胡哨的，能运行起来吗？

   本模板库现已放宽对语言环境的要求，绝大多数模板， `gcc` 和 `clang` 在 `C++11` 之后均可使用； `MSVC` 在 `C++14` 之后均可使用（暂无 `C++11` 测试环境）。只要你的 `C++` 语言标准在 `C++11` 之后，均可以使用我的模板库。

2. 在很多模板里，看到 `MAX_NODE` 参数，这是什么意思？

   在模板中，经常会需要一个很大的数组。为了使各种模板避免受到环境下内存分配速度的影响，所以数据结构大多使用了静态内存池，类的静态变量中有个很大的数组，不断地分配子数组给模板实例使用。只要看到类内有 `s_buffer` 的存在，即表示有内存池。

3. 在各种数据结构里，填写的 `MAX_NODE` 是否越大越好？如果是多组测试，是否每组测试重新构造一个数据结构对象就会触发 $O(MAXNODE)$ 的初始化导致超时？

   以下回答针对你的结点类型为平凡类型的情况（无构造函数，无初始值）。

    `MAX_NODE` 相关联的是结点内存池的大小，所以并不会出现每次构造一个数据结构对象，就导致内存池初始化的情况。

    `MAX_NODE` 并非越大越好，当 `MAX_NODE` 过大时，编译可能会失败。只要编译能通过，那么在此范围内 `MAX_NODE` 多大都没关系，也不会有任何的时间开销。

4. 我用整数做平衡树结点键值跑得非常快；换成 `std::string` 做平衡树结点键值，为什么程序突然变得很慢？甚至在很小的样例上都很慢？

   由于模板库内使用静态变量做内存池，静态变量某种意义上就是全局变量，所以在程序启动时就会对所有的结点进行初始化。而 `std::string` 对象即使是空的，也存在初始化代价；如果 `MAX_NODE` 过大，就会占用很长时间。
   
   一般来说，推荐使用平凡类型作为结点的成员变量。由于 `C++` 中，平凡类型的全局变量、静态变量初始化不消耗运行时，所以当你的数据结构的结点类型为平凡类型时，即使开再大的内存池也不会产生一丁点的运行时间。反之，如果你给结点设置了构造、析构，或者给某个成员变量设置了初始值，那么内存池的初始化就会占用时间。

5. 为什么在 `oj` (主要指 `codeforces` ) 提交代码时，提示如下？

   ```
   Can't compile file:
   Compiled file is too large
   ```

   首先需要知道，我的模板通过类似于 `MAX_NODE` 这样的模板参数控制一个模板最大的数组空间。这种方式产生的静态数组，并非是在运行期在堆上申请的，而是在编译时直接占用程序体积。

   而一些平台，例如 `codeforces` 对生成的程序大小有限制，有时候 `MAX_NODE` 过大，会生成超出大小限制的程序而无法通过编译。此时问题很好解决，将 `s_buffer` 从结点数组类型修改为结点指针类型，然后将类外的 `s_buffer` 初始化改写为 `s_buffer = new 【结点类型】[MAX_NODE]` 即可。

   例如，如果想对大小为 `500000` 的 `int` 区间进行最值维护，则需要约 `10000000` 个结点。当 `MAX_NODE = 10000000` 时， `ST` 表因为 `MAX_NODE` 过大而无法通过 `codeforces` 编译，则需要做如下修改：

   第 `46` 行修改为

   ```
   static node *s_buffer;
   ```

   第 `134` 行修改为

   ```
   typename Table<Node, MAX_NODE>::node *Table<Node, MAX_NODE>::s_buffer = new typename Table<Node, MAX_NODE>::node[MAX_NODE];
   ```

   此时即可通过 `codeforces` 编译。


6. 既然不会反复初始化内存池，那么多组数据之间是否会产生影响？

   不会。内存池里的结点只会被使用一次，上一组的数据使用的结点不会在下一组数据里被复用。
   
7. 线段树只能有求和的功能吗？

   本模板库非常重视模板的泛化程度，所以各种数据结构均支持通过传递运算符实现自由的操作。例如，对于线段树来说，可以传递 `lambda` 定义运算符来重载树中的信息聚合运算；或者通过定义一个结构体，并重载其括号运算符，达到同样的效果。其他的容器也往往如此。

8. 线段树模板参数一大堆，填写起来老是报错？连类型名字都不能完整写出来该怎么办？

   为了防止定义各种千奇百怪运算符的使用者在使用模板时，因为无法描述出模板的完整类型名称而困扰，所以特意编写了 `make_` 系列函数。如同 `std::make_pair` 以及 `std::make_tuple` 一般，只需要填写少量参数即可创建出复杂类型的模板。例如， `make_ZkwTree` 可以用来创建 `Zkw` 线段树；只要打出 `make_ZkwTree` 之后跟随 `IDE` 的智能提示进行相应的填写即可。
   
9. 用 `make_ZkwTree` 可以创建一颗线段树；但是如果我要在 `std::vector` 里存放十颗线段树，我还是得把类型全称写出来，可是我写不出来，怎么办？

   既然用 `make_ZkwTree` 可以创建出一颗线段树，那么可以用 `using NickName = decltype(make_ZkwTree<...>(...));` 来捕获这棵树的类型，并给它起个别名。接下来即可用 `std::vector<NickName>` 的方式存储十颗线段树。

   