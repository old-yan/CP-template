# CP-template
C++ template files for competitive programming.

#### 1.设计原则：

1. C++ style, not C style；
2. 代码最后格式化，即使自己觉得不好看；
3. 驼峰命名法；
4. 模板类名为：下划线+大写首字母
5. 参数名为：双下划线+小写
6. 必要时使用 explicit ；
7. 必要时使用 const；

如果代码出现 bug 或者设计问题，欢迎指出

#### 2.力扣输入输出模板使用方法：

1. 首先确定使用的编译器是 clang 或者 gcc ；如果是 MSVC（Visual Studio 的编译器），可能需要跟着提示逐步修改直到没报错；C++标准为 C++17。
2. 包含 LeetcodeIO.h 头文件；
3. 在包含头文件之前，加一行 #define OY_LOCAL 作为本地的标志；（也可以在编译参数里加 -DOY_LOCAL）
4. 在最终生成的 exe 同一目录下放入 in.txt 和 out.txt 作为输入输出文件
5. Solution 类的使用：需要在第二行注册要运行的成员方法名；
6. 自定义 Class 类的使用：需要在第一行注册类名+构造函数的所有参数类型；需要在第二行注册类名+所有要用到的成员方法名

使用时可以参考两张 `png` 图片示例。