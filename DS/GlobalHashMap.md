### 一、模板类别

​	数据结构：全局哈希表。

​	练习题目：

1. [How many](https://acm.hdu.edu.cn/showproblem.php?pid=2609)
2. [1488. 避免洪水泛滥](https://leetcode.cn/problems/avoid-flood-in-the-city/)
3. [P2580 于是他错误的点名开始了](https://www.luogu.com.cn/problem/P2580)
4. [P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
5. [P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
6. [P3879 [TJOI2010] 阅读理解](https://www.luogu.com.cn/problem/P3879)
7. [P4305 [JLOI2011] 不重复数字](https://www.luogu.com.cn/problem/P4305)
8. [P8306 【模板】字典树](https://www.luogu.com.cn/problem/P8306)
9. [P10996 【MX-J3-T3】Tuple](https://www.luogu.com.cn/problem/P10996)
10. [P10998 【MX-J3-T3+】Tuple+](https://www.luogu.com.cn/problem/P10998)
11. [J. Range Sets](https://qoj.ac/contest/1399/problem/7641)
12. [Associative Array](https://judge.yosupo.jp/problem/associative_array)(https://github.com/yosupo06/library-checker-problems/issues/376)
13. [Point Set Range Frequency](https://judge.yosupo.jp/problem/point_set_range_frequency)(https://github.com/yosupo06/library-checker-problems/issues/769)
14. [Number of Subsequences](https://judge.yosupo.jp/problem/number_of_subsequences)(https://github.com/yosupo06/library-checker-problems/issues/811)



### 二、模板功能

#### 1.建立哈希表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示哈希表中下标编号的变量类型。

   类型设定 `typename node` ，表示哈希表中的结点类型；当表为 `UnorderedSet`  时，仅存有键值；当表为 `UnorderedMap` 时，还存有映射值。

   类型设定 `typename pair` ，表示插入数据时的返回类型， `m_ptr` 属性表示插入到的结点位置； `m_flag` 表示插入是否成功。

   模板参数 `typename KeyType` ，表示键类型。

   模板参数 `typename MappedType` ，表示映射到的值类型。

   模板参数 `bool MakeRecord` ，表示是否对哈希表里插入过的结点进行记录。
   
   模板参数 `size_type BUFFER`​ ，表示哈希表的结点数为 `BUFFER` 。


2. 时间复杂度

   当开在全局时， $O(1)$ ；否则为 $O(n)$ 。

3. 备注

   本哈希表要求键类型 `KeyType` 有相应的 `Hash<KeyType>` 哈希函数存在。对于大多数内置的整数类型，不需要自己手写哈希函数；对于字符串类型，也不需要自己手写哈希函数；其他类型可以参考 `oj` 里的范例代码书写哈希函数。

   本哈希表名为“全局哈希表”，意为本数据结构更推荐开在全局，且不推荐同时开太多个哈希表；因为本数据结构不支持动态扩容，而是一开始就把所有空间固定下来，所以如果开太多的话会有严重的时空效率问题。当然，对于内置类型，将哈希表开在全局，不会有任何时空效率问题。

   本哈希表分为 `UnorderedSet` 和 `UnorderedMap` 两版；在不涉及映射关系的情况下选择前者即可。


#### 2.预留空间(reserve)

1. 数据类型

   输入参数 `size_type count` ，表示预留的空间大小。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本方法用于为记录预留空间；即便不预留空间也不会妨碍使用。

   本方法仅在 `MakeRecord` 为 `true` 时使用。

#### 3.查询结点数(size)

1. 数据类型

   返回类型 `size_type` ，表示目前插入的结点数。

2. 时间复杂度

    $O(1)$ 。

#### 4.查询是否为空(empty)

1. 数据类型

   返回类型 `bool` ，表示哈希表是否为空。

2. 时间复杂度

    $O(1)$ 。

#### 5.清空(clear)

1. 数据类型

2. 时间复杂度

    $O(n)$ ，此处 `n` 指插入的结点数。

3. 备注

   本方法仅当 `MakeRecord` 为 `true` 时使用。

#### 6.对每个结点调用回调(do_for_each)

1. 数据类型

   输入参数 `Callback &&call` ，表示对每个结点调用的回调函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本方法用于遍历哈希表中的每个结点。

   本方法仅当 `MakeRecord` 为 `true` 时使用。

#### 7.插入(insert)

1. 数据类型

   输入参数 `const KeyType &key` ，表示要插入的键值。

   返回类型 `pair` ，表示插入到的位置，以及插入是否成功。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   当键值已存在时，返回已存在的结点位置，且视为插入失败。

#### 8.查询(find)

1. 数据类型

   输入参数 `const KeyType &key` ，表示要查询的键值。

   返回类型 `node*` ，表示查询到的位置。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   当键值不存在时，返回空结点。

#### 9.插入否则赋值(insert_or_assign)

1. 数据类型

   输入参数 `const KeyType &key` ，表示要插入的键值。

   输入参数 `const MappedType &mapped` ，表示要插入的映射值。

   返回类型 `pair` ，表示插入的结果。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法试图进行 `insert` ，如果 `insert` 成功，就把映射值初始化为 `mapped` ；如果失败，也将映射值修改为 `mapped` 。

   本方法仅当表类型为 `UnorderedMap` 时使用。

#### 10.插入否则忽略(insert_or_ignore)

1. 数据类型

   输入参数 `const KeyType &key` ，表示要插入的键值。

   输入参数 `const MappedType &mapped` ，表示要插入的映射值。

   返回类型 `pair` ，表示插入的结果。

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法试图进行 `insert` ，如果 `insert` 成功，就把映射值初始化为 `mapped` ；如果失败，则不做修改。

   本方法仅当表类型为 `UnorderedMap` 时使用。

#### 11.获取映射值(get)

1. 数据类型

   输入参数 `const KeyType &key` ，表示要查询的键值。

   输入参数 `const MappedType &_default` ，表示默认的映射值。

   返回类型 `MappedType ，表示查询结果。`

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法在哈希表中查询映射值；如果查询不到就返回默认值。

   即使查询不到，本方法也不会在表中插入新结点。

   本方法仅当表类型为 `UnorderedMap` 时使用。

#### 12.获取映射值(get)

1. 数据类型

   输入参数 `const KeyType &key` ，表示要查询的键值。

   返回类型 `MappedType ，表示查询结果。`

2. 时间复杂度

   均摊 $O(1)$ 。

3. 备注

   本方法在哈希表中查询映射值。

   本方法须保证哈希表中有相应结点。

   本方法仅当表类型为 `UnorderedMap` 时使用。


### 三、模板示例

```c++
#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"

// 注意，对象必须开在全局
OY::GHASH::UnorderedSet<int, true, 1009> S;
void test_int_set() {
    S.insert(100);
    S.insert(300);
    S.insert(200);
    S.insert(300);
    cout << "S.size() = " << S.size() << endl;
    cout << "S: ";
    S.do_for_each([](decltype(S)::node *p) {
        cout << p->m_key << ' ';
    });
    cout << endl;

    S.clear();
    cout << "S.size() = " << S.size() << endl
         << endl;
}

using pii = std::pair<int, int>;
namespace OY {
    namespace GHASH {
        template <>
        struct Hash<pii> {
            uint64_t operator()(const pii &x) const {
                // 把两个 int 连起来当成一个 long long 来求哈希
                return Hash<long long>()(((long long)(x.first) << 32) + x.second);
            }
        };
    }
}
// 注意，对象必须开在全局
OY::GHASH::UnorderedSet<pii, true, 1009> S2;
void test_pair_set() {
    S2.insert({100, 200});
    S2.insert({300, 400});
    S2.insert({100, 400});
    S2.insert({300, 400});
    S2.insert({300, 200});
    cout << "S2.size() = " << S2.size() << endl;
    cout << "S2: ";
    S2.do_for_each([](decltype(S2)::node *p) {
        cout << p->m_key.first << ',' << p->m_key.second << ' ';
    });
    cout << endl;

    S2.clear();
    cout << "S2.size() = " << S2.size() << endl
         << endl;
}

// 注意，对象必须开在全局
OY::GHASH::UnorderedMap<std::string, int, true, 1009> S3;
void test_map() {
    S3["apple"] += 10;
    S3["banana"] += 5;
    S3["apple"] -= 2;
    S3["orange"] += 10;
    S3.do_for_each([](decltype(S3)::node *p) {
        cout << p->m_key << ": " << p->m_mapped << endl;
    });
}

int main() {
    test_int_set();
    test_pair_set();
    test_map();
}
```

```
#输出如下
S.size() = 3
S: 100 300 200 
S.size() = 0

S2.size() = 4
S2: 100,200 300,400 100,400 300,200 
S2.size() = 0

apple: 8
banana: 5
orange: 10

```

