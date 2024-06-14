#include "DS/PairHeap.h"
#include "IO/FastIO.h"

// 基础堆测试
void test_normal() {
    // 默认均为大根堆
    OY::VectorPairHeap<int, std::less<int>> big_root;
    for (int a : {1, 9, 3, 7, 5}) {
        big_root.push(a);
    }
    while (!big_root.empty()) {
        cout << big_root.top() << endl;
        big_root.pop();
    }

    // 传递比较参数，修改为字符串的小根堆
    OY::VectorPairHeap<std::string, std::greater<std::string>> small_root;
    for (std::string s : {"apple", "erase", "cat", "dog", "banana"}) {
        small_root.push(s);
    }
    while (!small_root.empty()) {
        cout << small_root.top() << endl;
        small_root.pop();
    }

    // 相比普通的堆，新增了 join 功能，用于将两个堆合并到一个堆
    OY::VectorPairHeap<int, std::less<int>> heap1;
    OY::VectorPairHeap<int, std::less<int>> heap2;
    for (int a : {1, 3, 5, 7, 9}) heap1.push(a);
    for (int a : {2, 4, 6, 8, 10}) heap2.push(a);
    heap1.join(heap2);
    while (!heap1.empty()) {
        cout << heap1.top() << ' ';
        heap1.pop();
        if (heap1.empty()) cout << endl;
    }
}

// 如何写出一个求全体和的堆
template <typename Node>
struct Node_pushup {
    using value_type = int;
    int m_val, m_sum;
    void set(int val) { m_val = val; }
    int get() const { return m_val; }
    // 本方法名必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
};
void test_pushup() {
    OY::PHeap::Heap<Node_pushup> S;
    S.push(1);
    S.push(2);
    S.push(3);
    S.push(4);
    cout << S.root()->m_sum << endl;
}

// 如何写出一个支持全体增值的堆
template <typename Node>
struct Node_pushdown {
    using value_type = int;
    int m_val, m_inc;
    // 为方便起见，定义一个函数，同时让 m_val 和 m_inc 增值。本函数名随意
    void add(int inc) { m_val += inc, m_inc += inc; }
    void set(int val) { m_val = val; }
    int get() const { return m_val; }
    // 本方法名必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
};
void test_pushdown() {
    OY::PHeap::Heap<Node_pushdown> S;
    S.push(1);
    S.push(2);
    S.push(3);
    S.push(4);
    S.root()->add(100);
    while (!S.empty()) cout << S.top() << ' ', S.pop();
    cout << endl;
}

// 自然而然的，我们可以写出既支持全体修改，也支持查询全体聚合值的堆
template <typename Node>
struct Node_pushup_pushdown {
    using value_type = int;
    int m_val, m_inc, m_size, m_sum;
    // 为方便起见，定义一个函数，同时让 m_val, m_inc, m_sum 增值。本函数名随意
    void add(int inc) { m_val += inc, m_inc += inc, m_sum += inc * m_size; }
    void set(int val) { m_val = val; }
    int get() const { return m_val; }
    // 本方法名必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
    // 本方法名必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_val, m_size = 1;
        if (!lchild->is_null()) m_sum += lchild->m_sum, m_size += lchild->m_size;
        if (!rchild->is_null()) m_sum += rchild->m_sum, m_size += rchild->m_size;
    }
};
void test_pushup_pushdown() {
    OY::PHeap::Heap<Node_pushup_pushdown> S1;
    for (int i = 0; i < 10; i++) S1.push(i * 200);
    OY::PHeap::Heap<Node_pushup_pushdown> S2;
    for (int i = 0; i < 10; i++) S2.push(i * 200 + 100);

    S1.root()->add(11);
    S2.root()->add(22);
    S1.join(S2);
    S1.root()->add(44);
    cout << S1.root()->m_sum << endl;
    while (!S1.empty()) cout << S1.top() << ' ', S1.pop();
    cout << endl;
}

int main() {
    test_normal();
    test_pushup();
    test_pushdown();
    test_pushup_pushdown();
}
/*
#输出如下
9
7
5
3
1
apple
banana
cat
dog
erase
10 9 8 7 6 5 4 3 2 1
10
104 103 102 101
20210
1966 1855 1766 1655 1566 1455 1366 1255 1166 1055 966 855 766 655 566 455 366 255 166 55

*/