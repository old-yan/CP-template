### 一、模板类别

​	数据结构：模拟力扣平台的输入输出

### 二、模板功能

备注：

1. 本文件建立在 `FastIO.h` 头文件基础上。

2. 本地调试的输入输出文件路径默认与 `InputHelper` 及 `OutputHelper` 的默认路径相同。可以通过 `LeetcodeInputHelper::setStream` 和 `LeetcodeOutputHelper::setStream` 指定输入输出的流，也就是指定一个 `InputHelper` 或者 `OutputHelper` 。

3. 逐行调试时，构造函数位于 `_LeetcodeConstructorFactory::operator()` 函数中，成员函数位于 `_LeetcodeMemberFunction::exec` 函数中，可以在相关处打断点；但是一般来说在主文件内打断点就足够了。

4. 构建 `Solution` 模式代码的体系：

   ```
   #include "IO/LeetcodeIO.h"
   using namespace std;
   
   class Solution {
   public:
       vector<int> twoSum(vector<int> &nums, int target) {
           int n = nums.size();
           for (int i = 0; i < n; ++i) {
               for (int j = i + 1; j < n; ++j) {
                   if (nums[i] + nums[j] == target) {
                       return {i, j};
                   }
               }
           }
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
   #include "IO/LeetcodeIO.h"
   using namespace std;
   
   class MKAverage {
   public:
       // 第1825题：求出MK平均值
       MKAverage(int m, int k) {}
       void addElement(int num) {}
       int findK(int k) {
           return -1;
       }
       int calculateMKAverage() {
           return -1;
       }
   };
   
   int main() {
       REGISTER_CONSTRUCTOR_CLASS(MKAverage, int, int);                          // 本行填写类名、构造函数的所有参数类
       REGISTER_MEMBERFUNCTION_CLASS(MKAverage, addElement, calculateMKAverage); // 本行填写类名、要调用的成员方法名
   
       while (true) {
           executor.constructClass();
           while (executor) {
               executor.executeClass();
           }
       }
   }
   ```
   
7. 有了 `FastIO.h` 和 `LeetcodeIO.h`，其实可以将力扣网站的输入输出劫持。输出文件需要重定位到 `./user.out` 。只需要在 `main` 函数之前执行我们自定义的输入、计算、输出，然后调用 `exit(0)` 退出程序，我们就完成了一波暗度陈仓，可以自行探索如何实现。

### 三、模板示例

```c++
#include "IO/LeetcodeIO.h"
using namespace std;

class Solution {
public:
    // 第110题：平衡二叉树，很简单
    int getDepth(TreeNode *root, bool &flag) {
        if (root == nullptr) return 0;
        int leftDepth = getDepth(root->left, flag);
        int rightDepth = getDepth(root->right, flag);
        if (abs(leftDepth - rightDepth) > 1) flag = false;
        return 1 + max(leftDepth, rightDepth);
    }
    bool isBalanced(TreeNode *root) {
        bool flag = true;
        int depth = getDepth(root, flag);
        return flag;
    }
};

int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(isBalanced);//这里填写方法名
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

```
#include "IO/LeetcodeIO.h"
using namespace std;

class Solution {
public:
    // 第 95 题. 不同的二叉搜索树，随便抄了份代码
    TreeNode *copy_tree(TreeNode *tmp, int j) {
        if (tmp == nullptr) return nullptr;
        TreeNode *left = copy_tree(tmp->left, j);
        TreeNode *right = copy_tree(tmp->right, j);
        TreeNode *node = new TreeNode(tmp->val + j, left, right);
        return node;
    }
    vector<TreeNode *> generateTrees(int n) {
        cout << "n = " << n << endl;
        vector<vector<TreeNode *>> dp(n + 1);
        dp[0] = {nullptr};
        dp[1] = {new TreeNode(1)};
        for (int i = 2; i <= n; i++)
            for (int j = 1; j <= i; j++)
                for (auto &tmp : dp[i - j]) {
                    TreeNode *rson = copy_tree(tmp, j);
                    for (auto &lson : dp[j - 1])
                        dp[i].push_back(new TreeNode(j, lson, rson));
                }
        return dp[n];
    }
};

int main() {
    OY::OutputHelper cout2("out2.txt");
    OY::LeetcodeOutputHelper::setStream(cout2); // 修改输出文件
    // 之后调用 cout 时，cout 在 out.txt 输出，力扣返回值在 out2.txt 输出
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(generateTrees);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
```

```
#以下为 "in.txt" 中的内容
3
2
1

```

```
#以下为程序运行之后， "out.txt" 中的内容
n = 3
n = 2
n = 1

```

```
#以下为程序运行之后， "out2.txt" 中的内容
[[1, null, 2, null, 3], [1, null, 3, 2], [2, 1, 3], [3, 1, null, null, 2], [3, 2, null, 1]]
[[1, null, 2], [2, 1]]
[[1]]

```