### 一、模板类别

​	数据结构：快速输入输出

### 二、模板功能

备注：

1. 只要将本模板放在文件中，就可以通过宏定义将原有的 `cin` 、`cout` 替换掉，之后可以使用 `cin` 和 `cout` 符号进行快速的读写。

2. 关于输入输出的路径，首先会验证是否存在 `INPUT_FILE` 和 `OUTPUT_FILE` 的宏定义；如果存在，则以其作为输入输出路径。如果不存在 `INPUT_FILE` 和 `OUTPUT_FILE` 的宏定义，会验证 `OY_LOCAL` 宏定义；如果存在 `OY_LOCAL` ，则以 `in.txt` 和 `out.txt` 作为输入输出路径。如果所有的宏都没找到，则以空字符串作为输入输出路径（即从控制台进行输入输出）。
    所以如果想要修改输入输出文件的路径，可以通过编译时添加宏/引入本头文件之前添加宏来指定路径。输入文件的宏为 `INPUT_FILE` ，输出文件的宏为 `OUTPUT_FILE` 。

   例如：

   ``````
   g++ -DOY_LOCAL -DINPUT_FILE=\"in2.txt\" -DOUTPUT_FILE=\"out2.txt\" main.cpp -o main.exe
   ``````

   通过此命令进行编译后，会读入 `in2.txt` 的内容作为输入，将输出写到 `out2.txt` 。

   **注意：** `MSVC` 的项目配置中添加宏定义不需要反斜杠。

3. 由于输入输出是以自定义类的形式实现，所以不仅仅可以用于单文件输入、单文件输出。比如，想要对比一下标准程序和自己的程序的输出结果，那么可以再声明一个

   ```
   outputHelper cout2("out2.txt")
   ```

   将自己的结果通过 `cout​` 输出到 `out.txt`，将标准程序的结果通过 `cout2` 输出到 `out2.txt` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"

int main() {

    // 读入一个整数 123
    int x;
    cin >> x;
    cout << x << '\n';

    // 读入一个字符串 "abc"
    std::string s;
    cin >> s;
    cout << s << '\n';

    // 读入一行字符串，失败，只读了 "this"
    std::string line;
    cin >> line;
    cout << line << '\n';

    // 读入一行字符串，成功，" is a line"
    getline(cin, line);
    cout << line << '\n';

    // 读入一行字符串，成功，"this is another line"
    getline(cin, line);
    cout << line << '\n';

    if (cin >> s)
        cout << "still have input\n";
    else
        cout << "no more input\n";
}
```

```
#以下为 "in.txt" 中的内容
123
abc
this is a line
this is another line
```

```
#以下为程序运行之后， "out.txt" 中的内容
123
abc
this
 is a line
this is another line
no more input

```
