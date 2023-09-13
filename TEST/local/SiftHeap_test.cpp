#include "DS/SiftHeap.h"
#include "IO/FastIO.h"

int main() {
    // 如果编号对应的价值存放在 vector 里
    std::vector<long long> values{10, 20, 30, 50, 40};
    // 这是一个 [0, 4] 编号范围内、以 values 为价值判断依据的大根堆
    auto value_heap = OY::make_SiftHeap(5, values, std::less<long long>{});
    for (int i = 0; i < 5; i++) value_heap.push(i);
    // 现在价值最大的是 3 号
    cout << "top is: " << value_heap.top() << endl;
    // 我们减小一下 3 号的价值，然后令其下沉
    values[3] = 5;
    value_heap.sift_down(3);
    // 现在价值最大的是 4 号
    cout << "top is: " << value_heap.top() << endl;
    // 我们增大一下 2 号的价值，然后令其上浮
    values[2] = 60;
    value_heap.sift_up(2);
    // 现在价值最大的是 2 号
    cout << "top is: " << value_heap.top() << endl;

    // 经过了这么多操作，堆的大小还是 5
    cout << "size = " << value_heap.size() << endl;

    // 如果编号对应的价值存放在定长数组里，仍然可以以类似方式构造
    std::string values2[5] = {"banana", "apple", "cat", "egg", "dog"};
    // 这是一个 [0, 4] 编号范围内、以 values2 为价值判断依据的小根堆
    auto value_heap2 = OY::make_SiftHeap(5, values2, std::greater<std::string>{});
    for (int i = 0; i < 5; i++) value_heap2.push(i);
    // 现在字典序最小的是 1 号
    cout << "top is: " << value_heap2.top() << endl;

    // 当然，如果某个编号的价值很难用一个值来衡量，你可以用匿名函数来自定义它的价值
    // 比如，某个编号的价值取决于数学分数和语文分数之和，那么可以这么写
    long long maths[] = {95, 62, 38, 71, 98};
    long long chinese[] = {30, 56, 80, 66, 75};
    // 这表示，编号的价值就在于自己编号本身；而编号与编号比较大小时，才去查看两个数组的值。
    // 这种写法的比较任务，实际落在了 comp 身上
    auto self = [](int i) { return i; };
    auto comp = [&](int x, int y) { return maths[x] + chinese[x] < maths[y] + chinese[y]; };
    OY::SiftHeap<decltype(self), decltype(comp)> students(5, self, comp);
    // 也可以这么写：
    // 这表示，编号的价值就在于自己编号本身；而编号与编号比较大小时，才去查看两个数组的值。
    // 这种写法的比较任务，实际落在了 getsum 身上
    auto getsum = [&](int i) { return maths[i] + chinese[i]; };
    OY::SiftHeap<decltype(getsum), std::less<long long>> students2(5, getsum);
    // 不管那种写法，本质都一样，最后都能得出 4 号学生总分最高
    for (int i = 0; i < 5; i++) {
        students.push(i);
        students2.push(i);
    }
    cout << students.top() << " == " << students2.top() << endl;
}
/*
#输出如下
top is: 3
top is: 4
top is: 2
size = 5
top is: 1
4 == 4
*/