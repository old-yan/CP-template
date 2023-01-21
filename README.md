# CP-template
C++ template files for competitive programming.

#### 1.设计原则：

1. C++ style, not C style。
2. 代码格式化:
    { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Attach, AllowShortIfStatementsOnASingleLine: true, IndentCaseLabels: true, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All, FixNamespaceComments: false ,AllowShortCaseLabelsOnASingleLine: true,AllowShortLoopsOnASingleLine: true,AllowShortBlocksOnASingleLine: true}
3. 文件宏命名为：双下划线+OY+双下划线+模板名全大写+双下划线
    模板参数命名为：单下划线+大写每个单词首字母；
    类命名为：大写每个单词首字母
    编译期常量命名为：全小写+下划线分割；
    成员变量命名为：m+下划线+驼峰命名法
    成员函数命名为：m+下划线+驼峰命名法
    静态成员变量命名为：s+下划线+驼峰命名法
    形式参数命名为：双下划线+驼峰命名法
4. 必要时使用 explicit 修饰单参数构造；必要时使用 const ；必要时使用 constexpr 修饰编译期方法；必要时使用 size_t 类型。
5.  C 的函数不使用 std 前缀。
6. 对不保证隐式类型转换成功的场景，使用显示类型转换。
7. 文件宏 end 处添加定位注释

如果代码出现 bug 或者设计问题，欢迎指出

#### 2.力扣输入输出模板使用方法：

1. 首先确定使用的编译器是 clang 或者 gcc ；如果是 MSVC（Visual Studio 的编译器），可能需要跟着提示逐步修改直到没报错；C++标准为 C++17。
2. 包含 LeetcodeIO.h 头文件；
3. 在包含头文件之前，加一行 #define OY_LOCAL 作为本地的标志；（也可以在编译参数里加 -DOY_LOCAL）
4. 在最终生成的 exe 同一目录下放入 in.txt 和 out.txt 作为输入输出文件
5. Solution 类的使用：需要在第二行注册要运行的成员方法名；
6. 自定义 Class 类的使用：需要在第一行注册类名+构造函数的所有参数类型；需要在第二行注册类名+所有要用到的成员方法名

使用时可以参考两张 `png` 图片示例。