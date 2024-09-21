#include "DS/AVL.h"
#include "IO/FastIO.h"

/*
 * 普通名次树用法
 */
void test() {
    cout << "test of normal avl:\n";
    // 想指定自定义的元素排序规则和结点总数的话，就传递参数。否则可以直接用默认的
    OY::AVLMultiset<int, std::less<int>> S;
    using node = decltype(S)::node;
    S.insert_by_key(400);
    S.insert_by_key(300);
    S.insert_by_key(200);
    S.insert_by_key(100);
    S.insert_by_key(200);
    S.insert_by_key(200);
    S.insert_by_key(500);
    S.insert_by_key(400);
    cout << S << '\n';

    // 如果使用 insert_by_rank，可以在指定位置插入元素。但是可能会破坏树的有序性质
    S.insert_by_rank(999, 2);
    cout << S << '\n';

    // 删除元素也有按照值和按照位置两种方式
    S.erase_by_rank(2);
    cout << S << '\n';

    S.erase_by_key(200);
    cout << S << '\n';

    // 树的分裂也有按照值和按照位置两种方式
    auto S2 = S.split_by_rank(3);
    cout << S << ' ' << S2 << '\n';
    // 注意， join 之后，S2 就不可用了
    S.join(S2);
    cout << S << '\n';

    auto S3 = S.split_by_key(400);
    cout << S << ' ' << S3 << '\n';
    // 注意， join 之后，S3 就不可用了
    S.join(S3);
    cout << S << '\n';

    // 如果两棵树的值域有交错，可以使用 merge 进行合并
    decltype(S) S4;
    S4.insert_by_key(50);
    S4.insert_by_key(250);
    S4.insert_by_key(550);
    cout << S << ' ' << S4 << '\n';
    // 注意， merge 之后，S4 就不可用了
    S4.do_for_each([&](node *p) {
        p->m_lc = p->m_rc = 0;
        S.insert_node_by_key(p);
    });
    cout << S << '\n';

    // root 获取根节点
    auto root = S.root();
    if (!root->is_null()) cout << "root = " << root->get() << '\n';

    // kth 按照位置获取结点
    auto p = S.kth(4);
    cout << "kth(4) = " << p->get() << '\n';

    // rank 按照值获取排名
    auto k = S.rank(250);
    cout << "rank(250) = " << k << '\n';

    // smaller_bound 按照值获取前驱结点
    auto sb = S.smaller_bound(250);
    if (!sb->is_null()) cout << "smaller_bound(250) = " << sb->get() << '\n';

    // lower_bound 按照值获取前驱结点
    auto lb = S.lower_bound(250);
    if (!lb->is_null()) cout << "lower_bound(250) = " << lb->get() << '\n';

    // upper_bound 按照值获取前驱结点
    auto ub = S.upper_bound(250);
    if (!ub->is_null()) cout << "upper_bound(250) = " << ub->get() << '\n';

    // 线性遍历全树
    int index = 0;
    S.do_for_each([&](node *p) { cout << "No." << index++ << ": " << p->get() << endl; });

    cout << '\n';
}

template <typename Node>
struct node_pushup {
    // 必须声明 key_type
    using key_type = int;
    int m_key;
    int m_sum;
    // 必须有 get set 方法
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    // 这个方法名字必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_key;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
};
/*
 * 如何利用 pushup 进行区间查询
 */
void test_pushup() {
    cout << "test of pushup avl:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::AVL::Tree<node_pushup> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想查询 {2000,4000,3000} 的和
    auto S3 = S.split_by_rank(5);
    auto S2 = S.split_by_rank(2);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    cout << "sum of " << S2 << " = " << S2.root()->m_sum << '\n';
    // 查询完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

template <typename Node>
struct node_pushdown {
    // 必须声明 key_type 和 value_type
    using key_type = int;
    int m_key;
    int m_inc;
    // 必须有 get set 方法
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    // 这个方法名字必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->m_key += m_inc, lchild->m_inc += m_inc;
            if (!rchild->is_null()) rchild->m_key += m_inc, rchild->m_inc += m_inc;
            m_inc = 0;
        }
    }
};
/*
 * 如何利用 pushdown 进行区间查询
 */
void test_pushdown() {
    cout << "test of pushdown avl:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::AVL::Tree<node_pushdown> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想让 {1000,2000,4000} 都增加个 100
    auto S3 = S.split_by_rank(4);
    auto S2 = S.split_by_rank(1);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    S2.root()->m_inc += 100, S2.root()->m_key += 100;
    // 修改完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

template <typename Node>
struct node_pushup_pushdown {
    // 必须声明 key_type
    using key_type = int;
    int m_key;
    int m_inc;
    int m_sum;
    // 必须有 get set 方法
    void set(int key) { m_key = key; }
    int get() const { return m_key; }
    // 方便起见，多写一个 add 方法。本方法名字当然可以随便起
    void add(int v) {
        m_key += v, m_inc += v, m_sum += v * ((Node *)this)->m_sz;
    }
    // 这个方法名字必须叫 pushup
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_key;
        if (!lchild->is_null()) m_sum += lchild->m_sum;
        if (!rchild->is_null()) m_sum += rchild->m_sum;
    }
    // 这个方法名字必须叫 pushdown
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add(m_inc);
            if (!rchild->is_null()) rchild->add(m_inc);
            m_inc = 0;
        }
    }
};
/*
 * 如何利用 pushup+pushdown 进行区间修改和查询
 */
void test_pushup_pushdown() {
    cout << "test of pushup+pushdown avl:\n";
    int arr[6] = {5000, 1000, 2000, 4000, 3000, 2000};
    // 此时我们可以无视树的有序性质，完全按照位置来进行操作
    OY::AVL::Tree<node_pushup_pushdown> S;
    for (int a : arr) {
        S.insert_by_rank(a, S.size());
    }
    // 假如我们想让 {1000,2000,4000} 都增加个 100
    auto S3 = S.split_by_rank(4);
    auto S2 = S.split_by_rank(1);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    S2.root()->add(100);
    // 修改完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    // 假如我们想查询 {2000,4000,3000} 的和
    S3 = S.split_by_rank(5);
    S2 = S.split_by_rank(2);
    cout << S << ' ' << S2 << ' ' << S3 << '\n';
    cout << "sum of " << S2 << " = " << S2.root()->m_sum << '\n';
    // 查询完毕记得把后面俩树连接回去
    S.join(S2), S.join(S3);
    cout << S << '\n';
    cout << '\n';
}

template <typename Node>
struct count_node {
    // 必须声明 key_type
    using key_type = std::string;
    std::string m_key;
    int m_count = 0;
    // 必须有 get set 方法
    void set(std::string key) { m_key = key; }
    std::string get() const { return m_key; }
    template <typename Ostream>
    friend Ostream &operator<<(Ostream &out, const count_node<Node> &x) {
        return out << "(" << x.m_key << "," << x.m_count << ")";
    }
};
/*
 * 如何利用 avl 写一个字典/哈希表/counter
 */
void test_counter() {
    cout << "test of avl counter:\n";
    // 假如我们的这个字典统计水果数量
    OY::AVL::Tree<count_node> S;
    using node = decltype(S)::node;
    S.insert_by_key("apple");
    S.insert_by_key("orange");
    S.insert_by_key("banana");
    cout << S << '\n';
    // 默认插入时 m_count 为零，需要手动修改
    S.modify_by_key("apple", [](node *p) { p->m_count = 10; });
    S.modify_by_key("orange", [](node *p) { p->m_count = 5; });
    S.modify_by_key("banana", [](node *p) { p->m_count = 8; });
    cout << S << '\n';

    OY::AVL::Tree<count_node> S2;
    S2.insert_by_key("peach", [](node *p) { p->m_count = 20; });
    S2.insert_by_key("melon", [](node *p) { p->m_count = 3; });
    S2.insert_by_key("apple", [](node *p) { p->m_count = 1; });
    S2.insert_by_key("orange", [](node *p) { p->m_count = 7; });
    cout << S2 << '\n';

    // 我们希望两个 counter 合并的时候，同类的 m_count 相加
    cout << S << ' ' << S2 << '\n';
    S2.do_for_each([&](node *p) {
        p->m_lc = p->m_rc = 0;
        if (!S.modify_by_key(p->get(), [&](node *q) { q->m_count += p->m_count; })) S.insert_node_by_key(p);
    });
    cout << S << '\n';
    cout << '\n';
}

/*
借用 make_AVL 制造具有类似线段树功能的平衡树，但是区间修改和区间查询只针对 key 属性
*/
void test_custom() {
    /*
    类似普通线段树
    这是一颗乘法统计树
    */
    auto S = OY::make_AVL<int, std::less<int>>(std::multiplies<int>());
    S.insert_by_rank(30, 0);
    S.insert_by_rank(20, 1);
    S.insert_by_rank(50, 2);
    S.insert_by_rank(10, 3);
    S.insert_by_rank(7, 2);
    auto S2 = S.split_by_rank(2);
    cout << S << ' ' << S.root()->m_info << '\n';
    cout << S2 << ' ' << S2.root()->m_info << '\n';
    S.join(S2);
    cout << S << ' ' << S.root()->m_info << '\n';

    /*
    类似带懒惰标记的线段树
    这是一颗支持区间乘的乘积树
    */

    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto op = [](double x, double y) { return x * y; };
    auto map = [](double x, double y, int size) { return y * std::pow(x, size); };
    auto com = [](double x, double y) { return x * y; };
    auto S3 = OY::make_lazy_AVL<double, double, true, std::less<double>>(op, map, com, 1);
#else
    struct {
        double operator()(double x, double y) const { return x * y; }
    } op;
    struct {
        double operator()(double x, double y, int size) const { return y * std::pow(x, size); }
    } map;
    struct {
        double operator()(double x, double y) const { return x * y; }
    } com;
    auto S3 = OY::make_lazy_AVL<double, double, true, std::less<double>>(op, map, com, 1);
#endif
    S3.insert_by_rank(3.0, 0);
    S3.insert_by_rank(2.0, 1);
    S3.insert_by_rank(5.0, 2);
    S3.insert_by_rank(1.0, 3);
    S3.insert_by_rank(7.0, 2);
    cout << S3 << ' ' << S3.root()->m_info << '\n';
    auto S4 = S3.split_by_rank(2);
    S4.root()->modify(2.0);
    cout << S3 << ' ' << S3.root()->m_info << '\n';
    cout << S4 << ' ' << S4.root()->m_info << '\n';
    S3.join(S4);
    cout << S3 << ' ' << S3.root()->m_info << '\n';
}

int main() {
    test();
    test_pushup();
    test_pushdown();
    test_pushup_pushdown();
    test_counter();
    test_custom();
}
/*
#输出如下
test of normal avl:
{100, 200, 200, 200, 300, 400, 400, 500}
{100, 200, 999, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200} {300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300} {400, 400, 500}
{100, 200, 200, 300, 400, 400, 500}
{100, 200, 200, 300, 400, 400, 500} {50, 250, 550}
{50, 100, 200, 200, 250, 300, 400, 400, 500, 550}
root = 300
kth(4) = 250
rank(250) = 4
smaller_bound(250) = 200
lower_bound(250) = 250
upper_bound(250) = 300
No.0: 50
No.1: 100
No.2: 200
No.3: 200
No.4: 250
No.5: 300
No.6: 400
No.7: 400
No.8: 500
No.9: 550

test of pushup avl:
{5000, 1000} {2000, 4000, 3000} {2000}
sum of {2000, 4000, 3000} = 9000
{5000, 1000, 2000, 4000, 3000, 2000}

test of pushdown avl:
{5000} {1000, 2000, 4000} {3000, 2000}
{5000, 1100, 2100, 4100, 3000, 2000}

test of pushup+pushdown avl:
{5000} {1000, 2000, 4000} {3000, 2000}
{5000, 1100, 2100, 4100, 3000, 2000}
{5000, 1100} {2100, 4100, 3000} {2000}
sum of {2100, 4100, 3000} = 9200
{5000, 1100, 2100, 4100, 3000, 2000}

test of avl counter:
{(apple,0), (banana,0), (orange,0)}
{(apple,10), (banana,8), (orange,5)}
{(apple,1), (melon,3), (orange,7), (peach,20)}
{(apple,10), (banana,8), (orange,5)} {(apple,1), (melon,3), (orange,7), (peach,20)}
{(apple,11), (banana,8), (melon,3), (orange,12), (peach,20)}

{30, 20} 600
{7, 50, 10} 3500
{30, 20, 7, 50, 10} 2100000
{3.000000, 2.000000, 7.000000, 5.000000, 1.000000} 210.000000
{3.000000, 2.000000} 6.000000
{14.000000, 10.000000, 2.000000} 280.000000
{3.000000, 2.000000, 14.000000, 10.000000, 2.000000} 1680.000000

*/