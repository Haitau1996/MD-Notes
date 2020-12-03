# 剑指Offer: 名企面试官精讲典型编程题

## 数组中的重复数字

## 面试题 4: 二维数组中的查找

题目描述: 在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。<br>

解题思路, 选取数组右上角的数字:
1. 如果数字等于 target , 查找过程结束
2. 如果数字大于 target , 可以忽略这一列
3. 如果数字小于 target , 可以忽略这一行

尾递归很容易在编译器那边变成循环,递归实现如下:
```C++
class Solution {
public:
    static bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int column = static_cast<int> (matrix[0].size());
        return SearchMatrixHelper(matrix, target, 0, column-1);
    }
    static bool SearchMatrixHelper(vector<vector<int>>& matrix, int target, int row, int column);
};
bool Solution::SearchMatrixHelper(vector<vector<int>>& matrix, int target, int row, int column){
    // base case: if column <= 0 or row >= matrix.size, return false
    if(column < 0 || row >= matrix.size()) return false;
    if(matrix[row][column] == target) return true;
    if(matrix[row][column] > target ) return SearchMatrixHelper(matrix, target, row, column-1);
    else return SearchMatrixHelper(matrix, target, row+1, column);
};
```
实现的时候需要注意的问题:
1. 行是从 0 开始, 而列是从 matrix.size() - 1 开始
2. 行是向下增加, 列是向左减小 
## 面试题 5: 替换空格

题目描述:请实现一个函数，把字符串中的每个空格替换成"％20"。例如，输 入 "We are happy.”, 则 输 出 "We%20are%20happy." <br>
常规解法: 从前向后扫描, 每次遇到空格, 就把后面的所有字符往后挪动三位, 然后写入, 每次需要挪动 O(n) 个字符, 要是有 O(n) 个空格, 总时间复杂度就是 $O(n^2)$. <br>
时间复杂度为 $O(n)$ 的解法:
1. 首先扫描一遍确定新串的长度
2. 用两个指针, 一个指向新串尾巴, 一个指向老串尾巴 
3. 从后向前扫描, 如果是老指针指的是一个正常字符就复制后往前移动
4. 遇到空格之后只挪新的指针, 如果新老指针相同说明就全部挪动完毕

//todo: add code here

## 面试题 7: 重建二叉树

题目描述:输入某二叉树的前序遍历和中序遍历的结果，请重建该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。<br>

解法分析: 前序遍历的第一个数值实际上就是根节点的数值, 然后根据中序遍历的结果, 可以得将后面的序列分别划分到左子树和右子树中.


## KMP 模式匹配算法

假设有 Source 字符串 S("abcababca") 和 Target 字符串 T("abcabx"), T 的首字母"a" 与第二位 "b" 以及第三位 "c" 都是不想等的, 可以忽略朴素匹配算法的某些判断. i 值不回溯, 就是不能表笑, 需要考虑的变化就是 j 值了, **j 值得大小取决于当前字符之前的串的前后缀相似程度**:
前缀, 如一个字符串 "abcd"就有三个前缀"a, ab , abc"(不包含自己), 后缀("d, cd, bcd"), 所谓的next[j] 的值就是前后缀相同的最长大小. 然后我们可以根据这个 next[] 数组跳过很多个 i 值回溯的部分, 因此程序的时间复杂度就从 $O((n-m+1)*m)$ 变到 $O(n+m)$