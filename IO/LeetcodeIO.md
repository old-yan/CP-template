### 一、模板类别

​	数据结构：模拟力扣平台的输入输出

### 二、模板功能

备注：

1. 本文件建立在 `FastIO.h` 头文件基础上；

2. 通过本模板放在文件中，可以通过宏定义快速的构建起输入输出体系；

3. 本地调试的输入输出文件路径默认为执行目录下的 `in.txt` 和 `out.txt`；

4. 逐行调试时，构造函数位于 `_LeetcodeConstructorFactory::operator()` 函数中，成员函数位于 `_LeetcodeMemberFunction::exec` 函数中，可以在相关处打断点；但是一般来说在主文件内打断点就足够了。

5. 构建 `Solution` 模式代码的体系：

   ```
   #include <bits/stdc++.h>
   #include "IO/LeetcodeIO.h"//这个头文件一定放最后，因为它把 cin 替换了
   using namespace std;
   
   class Solution {
   public:
   	//第1题：两数之和
       vector<int> twoSum(vector<int>& nums, int target) {
           return {};
       }
   };
   
   int main() {
       REGISTER_CONSTRUCTOR_SOLUTION;
       REGISTER_MEMBERFUNCTION_SOLUTION(twoSum);
       while (true) {
           executor.constructSolution();
           executor.executeSolution();
       }
   }
   ```

6. 构建 `Class` 模式代码的体系：

   ```
   #include <bits/stdc++.h>
   #include"IO/LeetcodeIO.h"
   using namespace std;
   
   class MKAverage {
   public:
   	//第1825题：求出MK平均值
       MKAverage(int m, int k) {
       }    
       void addElement(int num) {
       }
       int findK(int k){
           return -1;
       }
       int calculateMKAverage() {
           return -1;
       }
   };
   
   int main() {
       REGISTER_CONSTRUCTOR_CLASS(MKAverage,int,int);
       REGISTER_MEMBERFUNCTION_CLASS(MKAverage,addElement,calculateMKAverage);
   
       while (true) {
           executor.constructClass();
           while (executor) {
               executor.executeClass();
           }
       }
   }
   ```

7. 有了 `FastIO.h` 和 `LeetcodeIO.h`，其实可以将力扣的输入输出劫持。输出文件需要重定位到 `./user.out` 。只需要在 `main` 函数之前执行我们自定义的输入、计算、输出，然后调用 `exit(0)` 退出程序，我们就完成了一波暗度陈仓，可以自行探索如何实现。

### 三、模板示例

```c++
#include <bits/stdc++.h>
#include"IO/LeetcodeIO.h"
using namespace std;

class Solution {
public:
    //第110题：平衡二叉树，很简单
    int getDepth(TreeNode* root, bool& flag)
    {
        if(root == nullptr) return 0;
        int leftDepth = getDepth(root->left, flag);
        int rightDepth = getDepth(root->right, flag);
        if(abs(leftDepth - rightDepth) > 1) flag = false;
        return 1 + max(leftDepth, rightDepth);
    }
    bool isBalanced(TreeNode* root) {
        bool flag = true;
        int depth = getDepth(root, flag);
        return flag;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(isBalanced);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
```

```
#以下为 "in.txt" 中的内容
[3,9,20,null,null,15,7]
[1,2,2,3,3,null,null,4,4]
[]
```

```
#以下为程序运行之后， "out.txt" 中的内容
true
false
true
```

```
#include <bits/stdc++.h>
#include"IO/LeetcodeIO.h"
using namespace std;

class MedianFinder {
public:
	//第295题：双顶堆，找数据流中位数
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
```

```
#以下为 "in.txt" 中的内容
["MedianFinder","addNum","addNum","findMedian","addNum","findMedian"]
[[],[1],[2],[],[3],[]]
["MedianFinder","addNum","addNum","findMedian","addNum","findMedian"]
[[],[1],[2],[],[3],[]]
```

```
#以下为程序运行之后， "out.txt" 中的内容
[null,null,null,1.50000,null,2.00000]
[null,null,null,1.50000,null,2.00000]
```

