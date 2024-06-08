#include "IO/LeetcodeIO.h"
#include "STR/Trie.h"
using namespace std;

/*
[648. 单词替换](https://leetcode.cn/problems/replace-words/)
*/
/**
 * 本题为字典树模板题
 */

class Solution {
public:
    string replaceWords(vector<string> &dictionary, string sentence) {
        using Trie = OY::StaticTrie<OY::Trie::BaseNodeWrap, 26>;
        Trie S;
        for (auto &s : dictionary) S.insert_lower(s)->m_is_end = true;
        stringstream ss(sentence);
        string ans;
        while (ss >> sentence) {
            auto find = [&] -> int {
                auto cur = 0;
                for (int i = 0; i < sentence.size(); i++) {
                    auto son = S.get_node(cur)->get_child(sentence[i] - 'a');
                    if (!son) break;
                    cur = son;
                    if (S.get_node(cur)->m_is_end) return i + 1;
                }
                return sentence.size();
            };
            if (!ans.empty()) ans += ' ';
            ans += sentence.substr(0, find());
        }
        return ans;
    }
};

#ifdef OY_LOCAL
int main() {
    REGISTER_CONSTRUCTOR_SOLUTION;
    REGISTER_MEMBERFUNCTION_SOLUTION(replaceWords);
    while (true) {
        executor.constructSolution();
        executor.executeSolution();
    }
}
#endif