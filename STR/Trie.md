### 一、模板类别

​	序列：字典树。

### 二、模板功能

​	字典树模板使用起来尤其要求灵活。可以在力扣 `208` ，`212` ，`472` ，`648` ，`677` ，`720` ，`804` ，`820` ，`1032` 等题目做练习。

#### 1.构造字典树

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Info` ，表示结点维护的信息的类。默认为 `TrieDefaultInfo` 。 

   模板参数 `uint32_t _PoolSize` ，表示节点内存池大小。默认为 `1000000` 。 

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板为字典树。理论上也适用于非字符串的序列。

   第一个模板参数 `_Mapping` 传递为一个仿函数。在这个仿函数中，使用静态常函数 `range` 指定子结点的下标范围；重载括号运算符，指定字符和下标之间的映射规则。默认情况下为 `TrieLowerMapping` ，将 `26` 个小写字母依次映射到 `[0, 25]` 范围。

   第二个模板参数 `_Info` 传递为一个结构体。这个结构体里包含了每个字典树结点所要添加、维护的属性。默认情况下为 `TrieDefaultInfo` ，包含一个 `m_isEnd` 属性，初始值为 `false` 。

   第三个模板参数`_PoolSize` 至少要达到总字符个数。注意本模板在运行过程中不会回收空间，也不会动态扩容，所以 `_PoolSize` 一开始要开大一点避免空间不够用。这是为了提高速度的考虑而设计的。

   **备注：** 本模板在实际使用过程中，不仅仅需要知道字典树的方法，有时还需要知道字典树结点的方法。

   1. 字典树结点中，不直接保存数据，只保存一个下标，通过这个下标可以在数据区找到结点对应的数据。所以一个结点所占空间为四个字节。
   2. 结点由 `uint32_t` 构造而来，结点也可以类型转换到 `uint32_t` 。
   3. 由于在分配结点时，跳过了下标为 `0` 的结点，所以下标 `0` 表示空结点。结点可以强制类型转换到 `bool` ，当且仅当空结点时返回 `false` 。
   4. 调用 `parent()` 方法，可以找到父节点。
   5. 调用 `child(uint32_t)` ，可以找到某个子结点。如果为空，则返回空结点。
   6. 调用 `childGet(uint32_t)` ，可以找到某个子结点。如果为空，则新建子结点，然后再返回之。
   7. 插入序列、查找序列，与字典树的相关方法相同。比如你已经从字典树的根，按照序列 `abc` 走到某个结点位置；现在需要找到 `abcdef` 对应的结点，此时没有必要调用字典树的方法，而是要从刚才走到的位置调用结点的方法，按照序列 `def` 往下走即可。
   8. 结点重载了 `operator->`，所以虽然结点中没有存数据，但是仍然可以用 `->` 控制数据。

#### 2.插入序列

1. 数据类型

   输入参数 `_Iterator __first` ，表示要插入的序列的起始位置。

   输入参数 `_Iterator __last` ，表示要插入的序列的结束位置。（开区间）

   返回类型 `TrieNode` ，表示插入的结点。

2. 时间复杂度

   $O(n)$ 。

#### 3.插入序列

1. 数据类型

   输入参数 `_Sequence __sequence` ，表示要插入的序列。

   返回类型 `TrieNode` ，表示插入的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   插入的序列必须要支持 `begin` 和 `end` 成员方法，以提供迭代器。

#### 4.查询序列

1. 数据类型

   输入参数 `_Iterator __first` ，表示要查询的序列的起始位置。

   输入参数 `_Iterator __last` ，表示要查询的序列的结束位置。（开区间）

   返回类型 `TrieNode` ，表示查询的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   如果查询的序列不在树中，返回空结点。

#### 5.查询序列

1. 数据类型

   输入参数 `_Sequence __sequence` ，表示要插入的序列。

   返回类型 `TrieNode` ，表示查询的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   插入的序列必须要支持 `begin` 和 `end` 成员方法，以提供迭代器。

   如果查询的序列不在树中，返回空结点。

#### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/Trie.h"

int main() {
    //最简单的字典树，可以插入字符串，可以查找字符串、查找前缀
    std::string words[] = {"a", "app", "banana", "apple", "banned"};
    //在默认情况下，会将 'a'~'z' 映射到 [0, 25] 作为孩子结点的下标
    OY::Trie trie;
    for (std::string &word : words) {
        auto it = trie.insert(word);
        it->m_isEnd = true;
    }
    if (auto it = trie.find(std::string("apple")); it and it->m_isEnd)
        cout << "\"apple\" in the Trie\n";
    if (auto it = trie.find(std::string("apps")); !it or !it->m_isEnd)
        cout << "\"apps\" not in the Trie\n";
    if (auto it = trie.find(std::string("ban")); it and !it->m_isEnd)
        cout << "\"ban\" in the Trie as a prefix\n";

    //************************************************************************
    //可以实现复杂功能的字典树，需要自己传入自定义的模板参数
    //比如我的字符串是十六进制数，只包含数字和 'a'~'f'
    struct MyMap {
        static constexpr int range() { return 16; }
        int operator()(char c) {
            if (std::isdigit(c))
                return c - '0';
            else
                return c - 'a' + 10;
        }
    };
    //我的字典树要实现删除功能，所以要增加一个 size 属性，初始值为 0
    struct MyNode {
        bool m_isEnd = false;
        int m_size = 0;
    };
    OY::Trie<MyMap, MyNode> mytrie;
    std::string A[] = {"12", "159", "ba99", "7666", "3feaaa", "111", "0f0"};
    std::string B[] = {"159", "111", "0e0", "766"};
    //把 A 数组的字符串都插入到字典树里
    for (std::string &word : A) {
        auto it = mytrie.insert(word);
        if (it->m_isEnd) continue;
        it->m_isEnd = true;
        //插入之后需要从叶到根维护一下 size 属性
        for (; it; it = it.parent())
            it->m_size++;
    }
    //把 B 数组的字符串删除掉
    for (std::string &word : B) {
        //删除的方式可以有很多种写法，这里用递归的方式写一下
        // auto dfs = [&](auto self, auto &cur, int i) -> bool {
        //     if (!cur) return false;
        //     if (i == word.size()) {
        //         if (!cur->m_isEnd) return false;
        //         cur->m_isEnd = false;
        //         cur->m_size--;
        //         return true;
        //     } else {
        //         auto &child = cur.child(MyMap()(word[i]));
        //         if (!child) return false;
        //         bool res = self(self, child, i + 1);
        //         if (!child->m_size) {
        //             child = 0;
        //         }
        //         if (res) cur->m_size--;
        //         return res;
        //     }
        // };
        // dfs(dfs, mytrie.m_root, 0);

        //如果觉得递归不好看，可以写成迭代
        auto it = mytrie.find(word);
        if (!it || !it->m_isEnd) continue;
        it->m_isEnd = false;
        for (int i = word.size() - 1; i >= 0; i--) {
            auto parent = it.parent();
            if (!--it->m_size) {
                parent.child(MyMap()(word[i])) = 0;
            }
            it = parent;
        }
        //从 for 循环出来的时候 it 必等于 root2
        it->m_size--;
    }
    cout << "after delete, there are " << mytrie.m_root->m_size << " string in the Trie\n";
}
```

```
#输出如下
"apple" in the Trie
"apps" not in the Trie
"ban" in the Trie as a prefix
after delete, there are 5 string in the Trie

```

