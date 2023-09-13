#include "IO/LeetcodeIO.h"
using namespace std;

class MedianFinder {
public:
    // 第295题：双顶堆，找数据流中位数
    priority_queue<int> dowm;
    priority_queue<int, vector<int>, greater<int>> up;
    MedianFinder() {}
    void addNum(int num) {
        if (dowm.empty() || num <= dowm.top()) {
            dowm.push(num);
            while (dowm.size() > up.size() + 1) {
                up.push(dowm.top());
                dowm.pop();
            }
        } else {
            up.push(num);
            while (dowm.size() < up.size()) {
                dowm.push(up.top());
                up.pop();
            }
        }
    }
    double findMedian() {
        if ((dowm.size() + up.size()) % 2)
            return dowm.top();
        else
            return (dowm.top() + up.top()) / 2.0;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_CLASS(MedianFinder);                        // 注意这里是无参构造，所以只需要填类名
    REGISTER_MEMBERFUNCTION_CLASS(MedianFinder, addNum, findMedian); // 这里填写类名和要调用的成员方法名
    while (true) {
        executor.constructClass();
        while (executor) {
            executor.executeClass();
        }
    }
}
/*
#以下为 "in.txt" 中的内容
["MedianFinder","addNum","addNum","findMedian","addNum","findMedian"]
[[],[1],[2],[],[3],[]]
["MedianFinder","addNum","addNum","findMedian","addNum","findMedian"]
[[],[1],[2],[],[3],[]]
*/
/*
#以下为程序运行之后， "out.txt" 中的内容
[null,null,null,1.50000,null,2.00000]
[null,null,null,1.50000,null,2.00000]
*/