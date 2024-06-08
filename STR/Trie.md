### 一、模板类别

​	序列：字典树。

  练习题目：

1. [208. 实现 Trie (前缀树)](https://leetcode.cn/problems/implement-trie-prefix-tree)
2. [648. 单词替换](https://leetcode.cn/problems/replace-words/)
3. [P2580 于是他错误的点名开始了](https://www.luogu.com.cn/problem/P2580)
4. [P8306 【模板】字典树](https://www.luogu.com.cn/problem/P8306)



### 二、模板功能


#### 1.构造字典树

1. 数据类型

   模板参数 `typename Node` ，表示字典树的结点类型。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   对于字符集有限的情况，可以使用 `StaticChildGetter` 封装的结点，通过数组存储孩子结点；对于字符集较大的情况，可以自己手写使用哈希表或者平衡树存储孩子的结点。

   **注意：**

   因为本字典树是使用 `std::vector<node>` 作为内存池的，所以当动态数组扩容后，原有的指针可能会失效。此时需要使用结点索引重新获取结点指针。如果想避免指针意外失效，请在初始时 `reserve` 足够大的内存空间，这样不会有动态扩容的发生。

#### 2.获取结点(get_node)

1. 数据类型

   输入参数 `size_type index` ，表示结点编号。

2. 时间复杂度

    $O(1)$ 。

#### 3.插入字符串(insert)

1. 数据类型

   输入参数 `size_type length` ，表示要插入的字符串的长度。

   输入参数 `Mapping &&mapping` ，表示要插入的字符串从下标到元素的映射。

   输入参数 `Modify &&modify` ，表示对插入路径上的结点依次调用的回调函数。默认为 `Ignore` 类实例，表示不做操作。

   返回类型 `node*` ，表示插入字符串的结点位置。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   **注意：**

   回调函数会包括根结点和尾结点处进行调用。

#### 4.插入小写字符串(insert_lower)

1. 数据类型

   输入参数 `const std::string &s` ，表示要插入的字符串。

   输入参数 `Modify &&modify` ，表示对插入路径上的结点依次调用的回调函数。默认为 `Ignore` 类实例，表示不做操作。

   返回类型 `node*` ，表示插入字符串的结点位置。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法仅用于插入小写字母构成的字符串，且按照 `a` 对应 `0` ， `z` 对应 `25` 来进行插入。

   **注意：**

   回调函数会包括根结点和尾结点处进行调用。

#### 5.搜索字符串(find)

1. 数据类型

   输入参数 `size_type length` ，表示要搜索的字符串的长度。

   输入参数 `Mapping &&mapping` ，表示要搜索的字符串从下标到元素的映射。

   返回类型 `node*` ，表示搜索的字符串的结点位置。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   如果查询的字符串不在字典树中，返回空结点。

#### 6.搜索小写字符串(find_lower)

1. 数据类型

   输入参数 `const std::string &s` ，表示要搜索的小写字符串。

   返回类型 `node*` ，表示搜索的字符串的结点位置。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   如果查询的字符串不在字典树中，返回空结点。

   本方法仅用于搜索小写字母构成的字符串，且按照 `a` 对应 `0` ， `z` 对应 `25` 来进行搜索。

#### 7.上溯字符串(trace)

1. 数据类型

   输入参数 `size_type length` ，表示要处理的字符串的长度。

   输入参数 `Mapping &&mapping` ，表示要处理的字符串从下标到元素的映射。

   输入参数 `Modify &&modify` ，表示对路径上的结点依次调用的回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法用于对一个字符串在字典树里的路径上的每个结点调用的回调函数。

   与 `insert` 方法不同的是：本方法的回调为在树中从低到高（根）调用；而 `insert` 的回调是从高（根）到低调用。

   **注意：**

   调用本方法时，最好保证字符串对应的路径已存在；否则只会从现有的最低点开始调用回调。

#### 8.上溯小写字符串(trace_lower)

1. 数据类型

   输入参数 `const std::string &s` ，表示要处理的小写字符串。

   输入参数 `Modify &&modify` ，表示对路径上的结点依次调用的回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法用于对一个字符串在字典树里的路径上的每个结点调用的回调函数。

   与 `insert_lower` 方法不同的是：本方法的回调为在树中从低到高（根）调用；而 `insert` 的回调是从高（根）到低调用。

   本方法仅用于处理小写字母构成的字符串，且按照 `a` 对应 `0` ， `z` 对应 `25` 来进行搜索。

   **注意：**

   调用本方法时，最好保证字符串对应的路径已存在；否则只会从现有的最低点开始调用回调。

#### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/Trie.h"

void test() {
    // 最简单的字典树，可以插入字符串，可以查找字符串、查找前缀
    std::string words[] = {"a", "app", "banana", "apple", "banned"};
    // 在默认情况下，会将 'a'~'z' 映射到 [0, 25] 作为孩子结点的下标
    OY::StaticTrie<> trie;
    for (std::string &word : words) {
        auto it = trie.insert_lower(word);
        it->m_is_end = true;
    }
    auto it = trie.find_lower(std::string("apple"));
    if (it->m_is_end)
        cout << "\"apple\" in the Trie\n";
    it = trie.find_lower(std::string("apps"));
    if (!it or !it->m_is_end)
        cout << "\"apps\" not in the Trie\n";
    it = trie.find_lower(std::string("ban"));
    if (it and !it->m_is_end)
        cout << "\"ban\" in the Trie as a prefix\n";
    cout << endl;
}

int main() {
    test();
}
```

```
#输出如下
"apple" in the Trie
"apps" not in the Trie
"ban" in the Trie as a prefix


```

