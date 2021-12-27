#include <bits/stdc++.h>
#include"IO/LeetcodeIO.h"
using namespace std;

class MedianFinder {
public:
	//295题：双顶堆，找数据流中位数
    priority_queue<int> dowm;
    priority_queue<int, vector<int>, greater<int>> up;
    MedianFinder() {}
    void addNum(int num) {
        if (dowm.empty() || num <= dowm.top())
        {
            dowm.push(num);
            while (dowm.size() > up.size() + 1)
            {
                up.push(dowm.top());
                dowm.pop();
            }
        }
        else
        {
            up.push(num);
            while (dowm.size() < up.size())
            {
                dowm.push(up.top());
                up.pop();
            }
        }
    }
    double findMedian() {
        if ((dowm.size() + up.size()) % 2) return dowm.top();
        else return (dowm.top() + up.top()) / 2.0;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_CLASS(MedianFinder);
    REGISTER_MEMBERFUNCTION_CLASS(MedianFinder,addNum,findMedian);
    while (true) {
        executor.constructClass();
        while (executor) {
            executor.executeClass();
        }
    }
}