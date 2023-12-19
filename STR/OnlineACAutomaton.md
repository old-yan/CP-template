### 一、模板类别

​	序列：在线 `AC` 自动机。

​	练习题目：

1. [GRE Words Revenge](https://acm.hdu.edu.cn/showproblem.php?pid=4787)


### 二、模板功能

​	本模板（下文简称 `OnlineACAM` ）为离线 `ACAutomaton` （下文简称 `ACAM` ）模板的在线版。`ACAM` 模板需要在插入所有字符串后，统一进行预处理，再进行查询；而 `OnlineACAM` 经过对初级模板的二进制优化封装，可以支持边插入边查询。在 `OnlineACAM` 中，存在多个大小不一的 `ACAM` ，每当较小的 `ACAM` 达到一定大小时，就与较大的合并重构。

​	在本模板中，一个结点内置的信息有： `m_dep` ，表示结点在 `Trie` 树上的深度，结点 `0` 的深度为 `0` ； `m_length` ，表示结点作为插入的串的末端结点，其值等于串的长度。

​	类型设定 `size_type = uint32_t` ，表示大小、索引的类型。

​	类型设定 `ACAM_type` ，表示底层的 `ACAM` 的类型。

#### 1.插入模式序列(insert)

1. 数据类型

   输入参数 `Iterator first` ，表示模式序列的区间开始位置。

   输入参数 `Iterator last` ，表示模式序列的区间结尾位置。（开区间）

   输入参数 `Mapping &&map` ，表示序列中元素到孩子索引的映射。

   返回类型 `std::pair<ACAM_type *, size_type>` ，前者表示序列被插入到的 `ACAM` 的指针；后者表示序列被插入后，末端结点的编号。

2. 时间复杂度

   均摊 $O(m\log n)$ ，此处 `m` 指模式串长度， `n` 指所有模式串的数量。
   
3. 备注

    `Mapping &&map` 参数的存在，是为了可能的空间节省。例如，如果插入的字符串全部都是小写字母，就可以映射到 `[0, 26)` 区间；如果插入的字符串全部都是数字字符，就可以映射到 `[0, 10)` 区间

#### 2.插入小写字母序列(insert_lower)

1. 输入类型

   输入参数 `const std::string &s` ，表示即将被插入自动机里的字符串。

   返回类型 `std::pair<ACAM_type *, size_type>` ，前者表示序列被插入到的 `ACAM` 的指针；后者表示序列被插入后，末端结点的编号。

2. 时间复杂度

    均摊 $O(n\log n)$ ，此处 `n` 指模式串长度。

3. 备注

   显然，插入小写字母组成的字符串是最常见的问题类型。在这种情况下，可以将元素映射到 `[0, 26)` 区间。

#### 3.清空(clear)

1. 输入类型

2. 时间复杂度

    $O(1)$ 。

#### 4.查询大小(size)

1. 输入类型

   返回类型 `size_type` ，表示底层含有几个 `ACAM` 。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   **注意：** 本函数并不能返回插入了几个字符串。

#### 5.查询是否为空(empty)

1. 输入类型

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

#### 6.查询是否包含某序列(contains)

1. 输入参数

   输入参数 `Iterator first` ，表示模式序列的区间开始位置。

   输入参数 `Iterator last` ，表示模式序列的区间结尾位置。（开区间）

   输入参数 `Mapping &&map` ，表示序列中元素到孩子索引的映射。

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   均摊 $O(m\log n)$ ，此处 `m` 指模式串长度， `n` 指所有模式串的数量。

3. 备注

   `Mapping &&map` 参数的存在，是为了可能的空间节省。例如，如果插入的字符串全部都是小写字母，就可以映射到 `[0, 26)` 区间；如果插入的字符串全部都是数字字符，就可以映射到 `[0, 10)` 区间

#### 7.获取某个自动机(get_ACAM)

1. 输入类型

   输入参数 `size_type ac_index` ，表示自动机的索引。

   返回类型 `const ACAM_type *` ，表示查询的自动机的指针。

2. 时间复杂度

    $O(1)$ 。

#### 8.对每个自动机调用回调函数(do_for_each_ACAM)

1. 输入类型

   输入参数 `Callback &&call` ，表示对每个自动机调用的回调函数。

2. 时间复杂度

    $O(\log n)$ ，此处 `n` 表示。插入的序列数量。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/OnlineACAutomaton.h"

struct Node {
    std::string m_str;
    const std::string &get() const { return m_str; }
    void set(const std::string &str) { m_str = str; }
};
int main() {
    using OnlineACAM = OY::ONLINEAC::Automaton<Node, 26>;
    using ACAM = OnlineACAM::ACAM_type;

    OnlineACAM AC;

    // 本函数可以插入一个串，并在结点上做标记
    auto insert_lower = [&](const std::string &s) {
        auto res = AC.insert_lower(s);
        auto ptr_ac = res.first;
        int pos = res.second;
        ptr_ac->get_node(pos)->set(s);
    };

    // 搜寻、展示所有的 border
    auto show_border = [&](const std::string &s) {
        auto call = [&](const ACAM &ac) {
            int last_pos = 0;
            for (char c : s) last_pos = ac.next(last_pos, c - 'a');
            while (!ac.get_node(last_pos)->m_str.empty()) {
                cout << s << "'s border: " << ac.get_node(last_pos)->m_str << endl;
                last_pos = ac.query_fail(last_pos);
            }
        };
        cout << "try to find " << s << ":\n";
        for (int i = 0; i < AC.size(); i++) {
            cout << "In " << i << "-th ACAM:\n";
            call(*AC.get_ACAM(i));
        }
        cout << endl;
    };

    // 搜寻、展示所有的 border，第二种实现，但是不会区分 ACAM 的索引
    auto show_border_2 = [&](const std::string &s) {
        auto call = [&](const ACAM &ac) {
            int last_pos = 0;
            for (char c : s) last_pos = ac.next(last_pos, c - 'a');
            while (!ac.get_node(last_pos)->m_str.empty()) {
                cout << s << "'s border: " << ac.get_node(last_pos)->m_str << endl;
                last_pos = ac.query_fail(last_pos);
            }
        };
        cout << "try to find " << s << ":\n";
        AC.do_for_each_ACAM(call);
        cout << endl;
    };

    insert_lower("a");
    insert_lower("c");
    insert_lower("abc");
    show_border("xababc");

    insert_lower("ab");
    insert_lower("babc");
    insert_lower("abd");
    insert_lower("bc");
    show_border_2("xababc");
}
```

```
#输出如下
try to find xababc:
In 0-th ACAM:
xababc's border: abc
In 1-th ACAM:
xababc's border: c

try to find xababc:
xababc's border: bc
xababc's border: babc
xababc's border: abc
xababc's border: c

```

