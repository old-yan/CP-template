### 一、模板类别

​	数学：字典树。

### 二、模板功能

​	本模板的类名为 `TrieNode<_Mapping,_Base>` ，以下用 `node` 代称。

​	字典树模板使用起来尤其要求灵活。可以在力扣 `208` ，`212` ，`472` ，`648` ，`677` ，`720` ，`804` ，`820` ，`1032` 等题目做练习。

#### 1.构造字典树

1. 数据类型

   模板参数 `typename _Mapping` ，表示序列中的元素和下标的映射规则。默认为 `TrieLowerMapping` 即将小写字母依次映射到 `[0, 25]` 。

   模板参数 `typename _Base` ，表示结点继承的基类。默认为 `TrieDefaultBase` 。 

   构造参数 `node *__parent` ，表示父结点。默认为 `nullptr` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板为字典树。理论上也适用于非字符串的序列。

   第一个模板参数 `_Mapping` 传递为一个仿函数。在这个仿函数中，使用静态常函数 `range` 指定子结点的下标范围；重载括号运算符，指定字符和下标之间的映射规则。默认情况下为 `TrieLowerMapping` ，将 `26` 个小写字母依次映射到 `[0, 25]` 范围。

   第二个模板参数 `_Base` 传递为一个结构体。这个结构体里包含了每个字典树结点所要添加、维护的属性。默认情况下为 `TrieDefaultBase` ，包含一个 `m_isEnd` 属性，初始值为 `false` 。

   在使用时，常常只需要显式构造根节点，而根节点的父结点为空，所以不需要显式传参。

#### 2.获取子结点

1. 数据类型

   输入参数 `int __i` ，表示要获取的子结点的下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法在子结点为空时，会新建子结点，然后返回之。所以，如果不想新建子结点而只想试探子结点是否为空，请直接访问 `m_child` 数组。

#### 3.插入序列

1. 数据类型

   输入参数 `_Iterator __first` ，表示要插入的序列的起始位置。

   输入参数 `_Iterator __last` ，表示要插入的序列的结束位置。（开区间）

   返回类型 `node *` ，表示插入的结点。

2. 时间复杂度

   $O(n)$ 。

#### 4.插入序列

1. 数据类型

   输入参数 `_Sequence __sequence` ，表示要插入的序列。

   返回类型 `node *` ，表示插入的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   插入的序列必须要支持 `begin` 和 `end` 成员方法，以提供迭代器。

#### 5.查询序列

1. 数据类型

   输入参数 `_Iterator __first` ，表示要查询的序列的起始位置。

   输入参数 `_Iterator __last` ，表示要查询的序列的结束位置。（开区间）

   返回类型 `node *` ，表示查询的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   如果查询的序列不在树中，返回 `nullptr` 。

#### 6.查询序列

1. 数据类型

   输入参数 `_Sequence __sequence` ，表示要插入的序列。

   返回类型 `node *` ，表示查询的结点。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   插入的序列必须要支持 `begin` 和 `end` 成员方法，以提供迭代器。

   如果查询的序列不在树中，返回 `nullptr` 。

#### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "STR/Trie.h"

int main() {
    //最简单的字典树，可以插入字符串，可以查找字符串、查找前缀
    std::string words[] = {"a", "app", "banana", "apple", "banned"};
    //在默认情况下，会将 'a'~'z' 映射到 [0, 25] 作为孩子结点的下标
    using node = OY::TrieNode<>;
    node *root = new node;
    for (std::string &word : words) {
        auto it = root->insert(word);
        it->m_isEnd = true;
    }
    auto it = root->find(std::string("apple"));
    if (it and it->m_isEnd) cout << "\"apple\" in the Trie\n";
    it = root->find(std::string("apps"));
    if (!it or !it->m_isEnd) cout << "\"apps\" not in the Trie\n";
    it = root->find(std::string("ban"));
    if (it and !it->m_isEnd) cout << "\"ban\" in the Trie as a prefix\n";

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
    using node2 = OY::TrieNode<MyMap, MyNode>;
    std::string A[] = {"12", "159", "ba99", "7666", "3feaaa", "111", "0f0"};
    std::string B[] = {"159", "111", "0e0", "766"};
    node2 *root2 = new node2;
    //把 A 数组的字符串都插入到字典树里
    for (std::string &word : A) {
        auto it = root2->insert(word);
        if (it->m_isEnd) continue;
        it->m_isEnd = true;
        //插入之后需要从叶到根维护一下 size 属性
        for (; it; it = it->m_parent)
            it->m_size++;
    }
    //把 B 数组的字符串删除掉
    for (std::string &word : B) {
        //删除的方式可以有很多种写法，这里用递归的方式写一下
        // auto dfs = [&](auto self, node2 *&cur, int i) -> bool {
        //     if (!cur) return false;
        //     if (i == word.size()) {
        //         if (!cur->m_isEnd) return false;
        //         cur->m_isEnd = false;
        //         cur->m_size--;
        //         return true;
        //     } else {
        //         node2 *&child = cur->m_child[node2::s_map(word[i])];
        //         if (!child) return false;
        //         bool res = self(self, child, i + 1);
        //         if (!child->m_size) {
        //             delete child;
        //             child = nullptr;
        //         }
        //         if (res) cur->m_size--;
        //         return res;
        //     }
        // };
        // dfs(dfs, root2, 0);
        //如果觉得递归不好看，可以写成迭代
        node2 *it = root2->find(word);
        if (!it || !it->m_isEnd) continue;
        it->m_isEnd = false;
        for (int i = word.size() - 1; i >= 0; i--) {
            node2 *parent = it->m_parent;
            if (!--it->m_size) {
                delete it;
                parent->m_child[node2::s_map(word[i])] = nullptr;
            }
            it = parent;
        }
        //从 for 循环出来的时候 it 必等于 root2
        it->m_size--;
    }
    cout << "after delete, there are " << root2->m_size << " string in the Trie\n";
}
```

```
#输出如下
"apple" in the Trie
"apps" not in the Trie
"ban" in the Trie as a prefix
after delete, there are 5 string in the Trie

```

