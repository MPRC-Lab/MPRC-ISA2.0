#include <vector>
#include <iostream>
#include <string>
using namespace std;

void getNext(vector<int>& next, string& pattern){
    for (int i = 1; i < pattern.size() + 1; ++i){
        int k = next[i-1];
        if (k == -1){
            continue;
        }
        if (pattern[i-1] == pattern[k]){
            next[i] = k + 1;
        }
        else {
            while(pattern[i-1] != pattern[k] && k > 0){
                k = next[k];
            }
            next[i] = (k > 0) ? k : 0;
        }
    }
    return;
}

int KMP(string& pattern, string& target){
    int i = 0, j = 0;
    if (pattern.empty() || pattern.size() > target.size()){
        return 0;
    }
    //因为这里要计算所有匹配的数目，因为下面注释中的原因，next数组要多加一个元素
    vector<int> next(pattern.size() + 1, 0);
    next[0] = -1;
    getNext(next, pattern);
    int res = 0;
    //这里这个等于是不能少的
    while (j <= target.size()){
        //当找到一个匹配后，就假设pattern[pattern.size()]也存在，且发生不匹配
        if (i == pattern.size()){
            ++res;
            i = next[i];
            continue;
        }
        if (pattern[i] == target[j]){
            ++i;
            ++j;
        }
        else if (next[i] == -1) {
            i = 0;
            ++j;
        }
        else {
            i = next[i];
        }
    }
    return res;
}
