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


## 面试题 7: 重建二叉树

题目描述:输入某二叉树的前序遍历和中序遍历的结果，请重建该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。<br>

解法分析: 前序遍历的第一个数值实际上就是根节点的数值, 然后根据中序遍历的结果, 可以得将后面的序列分别划分到左子树和右子树中.
```C++
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return buildTreeHelper(preorder,0,inorder,0,preorder.size());
    }
private:
    TreeNode* buildTreeHelper(vector<int>& preorder, size_t preHead, 
                              vector<int>& inorder, size_t inHead, size_t length);
};

TreeNode* Solution::buildTreeHelper (vector<int>& preorder, size_t preHead,
                            vector<int>& inorder, size_t inHead, size_t length){
    if(length == 0 ) return nullptr;
    else {
        int rootVal = preorder[preHead];
        TreeNode* root = new TreeNode;
        root->val = rootVal;
        size_t rootIndex{inHead};
        size_t leftLen{};
        while(inorder[rootIndex] != rootVal){
            rootIndex ++;
            ++leftLen;
        }
        root->left = buildTreeHelper(preorder,preHead+1, inorder, inHead,leftLen);
        root->right = buildTreeHelper(preorder,preHead+1+leftLen,
                                      inorder,inHead+1+leftLen,length-leftLen-1 );
        return root;
    }
}
```
在解题的时候, 一开始我们使用的是留个参数, preHear, preTail, inHead, inTail. 然后写起来有一些麻烦, 后来发现, 前序和中序打出来的长度是一样的, 少一个参数后实际上写下标的时候就会方便很多.

## 面试题 8: 二叉树的下一个节点

题目描述: 给出二叉数和其中一个节点, 如何找出中序遍历的下一个节点(数中有指向左右子树指针和指向父节点指针).<br>
解题思路,将节点的类型分成三类:
1. 该节点有右子树, 显然它的下一个打印的节点就是右子数中最左的节点
2. 该节点没有右子树, 且该节点位于父节点的左边, 那么打印的就是其父节点
3. 该节点没有右子树, 且该节点位于父节点的右边, 那么就一直向上遍历, 直到找到一个节点, 恰好它是父节点的左子数, 打印该节点

## 面试题 9: 使用两个栈实现队列
题目描述：用两个栈实现一个队列。队列的声明如下，请实现它的两个函数appendTail 和deleteHead, 分别完成在队列尾部插入节点和在队列头部删除节点的功能。<br>
解题思路:
使用两个两个栈, 一个 stack1 和 stack2, 平时往stcak1 插入元素, 如果想要删除元素,如果 2 是空的, 将 1 中的元素全部压入 2, 两个 LIFO 就实现了一个 FIFO, 如果 2 不空, 直接弹出栈顶.
```C++
class CQueue {
public:
    CQueue() {
    }
    void appendTail(int value) {
        stack1.push(value);
    }
    
    int deleteHead() {
        if(stack2.empty()){
            while(!stack1.empty()){
                int data = stack1.top();
                stack1.pop();
                stack2.push(data);
            }
        }
        if(stack2.empty()) return -1;
        int head = stack2.top();
        stack2.pop();
        return head;
    }
private:
    stack<int> stack1;
    stack<int> stack2;
};
```

## 面试题 11: 旋转数组的最小数字
不同的排序算法应用场合也不尽相同, 快排的总体平均效率是最好的, 但也不是在任何时候都是最优的算法, 如果面试官要求实现一个排序算法, 一定要问清楚这个拍讯的应用环境是什么, 有哪些约束条件. <br>
题目描述: 把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。输入一个递增排序的数组的一个旋转，输出旋转数组的最小元素。例如，数组{3, 4, 5, 1, 2} 为{l,2,3,4,5} 的一个旋转，该数组的最小值为1.<br>
解题思路: 
1. 本题给出的数组在一定程度上是有序的, 因此可以使用二分法寻找这个最小元素
2. 使用两个指针指向数组的第一个元素和最后一个元素, 第一个元素应该是大于等于最后一个元素的(特例:排序数组的0号元素搬最后面, 整个数组旋转后刚好也是有序的)
3. 取中间的元素和前面的第一个元素 以及后面的最后一个元素对比, 然后递归查找子数组,总的来说, **第一个指针总是指向前面递增数组的元素, 第二个指针总是指向后面递增数组的元素**
```C++
// 下面给出一个递归实现, 可以用 while 循环节约递归需要的程序调用栈
class Solution {
public:
    int minArray(vector<int>& numbers) {
        if(numbers[0] < numbers[numbers.size()-1]) return numbers[0];
        else return minArrayHelper(numbers,0,numbers.size()-1);
    }
    int minArrayHelper(vector<int>& number, size_t ptr1,size_t ptr2);
    int minInOrder(vector<int>& number, size_t ptr1, size_t ptr2);
};
int Solution::minArrayHelper(vector<int>& number, size_t ptr1, size_t ptr2){
    if(ptr2 - ptr1 <= 1) return number[ptr2];
    else {
        size_t mid = (ptr1 + ptr2) / 2;
        if(number[ptr1] == number[ptr2] && number[ptr1] == number[mid])
            return minInOrder(number, ptr1,ptr2);
        if(number[mid] >= number[ptr1] ) 
            return minArrayHelper(number, mid, ptr2);
        else if(number[mid] <= number[ptr2])
            return minArrayHelper(number,ptr1,mid);
        return number[mid];
    }
}
int Solution::minInOrder(vector<int>& number, size_t ptr1, size_t ptr2){
    int result = number[ptr1];
    for(size_t i = ptr1; i <= ptr2; ++i){
        if(number[i] < result) 
            result = number[i];
    }
    return result;
}
```
这个题目解答的过程中需要考虑解决两个特殊的情况:
1. 数组刚好是有序的, 旋转一轮之后又回来了
2. 数组中相等的元素比较多, index1, index2 和 middle 指向的元素相同(这时候无法判断, 只能从头往后扫描)

## 面试题 12 : 矩阵中的路径
回溯可以看成是暴力发的升级版本, 适合解决由多个步骤组成的问题, 并且每个步骤都有多个选项, 当我们在某一步选择了其中一个选项时，就进入下一步，然后又面临新的选项。解决问题的过程可以形象地用树表示. <br>
题目描述: ：请设计一个函数，用来判断在一个矩阵中是否存在一条包含某字符串所有字符的路径。路径可以从矩阵中的任意一格开始，每一步可以在矩阵中向左、右、上、下移动一格。如果一条路径经过了矩阵的某一格，那么该路径不能再次进入该格子。<br>
* 矩阵中任意选取一个格子作为路径的起点
* 某个格子字符为 ch, 且对应 str 的第 i 个字符, 若不想等, 则这个格子不可能在路径上
* 若相当等, 就到临近的地方找第 i+1 个字符

```C++
class Solution {
public:
    bool exist(vector<vector<char>>& board, string word) {
        // 遇到特殊的情况, 直接返回 false
        if(board.size()==0 || board[0].size()==0 || word.length() == 0) return false;
        else {
            size_t chaIndex = 0;
            for(size_t i=0; i < board.size(); ++i){
                for(size_t j=0; j < board[0].size(); ++j){
                    if(existHelper(board, i, j, word, chaIndex)){
                        return true;
                    }
                }
            }
            return false;
        }

    }
    bool existHelper(vector<vector<char>>& board, size_t row, size_t col, const string& word, size_t chaIndex){
        // base case: charIndex == word.length()
        if(chaIndex == word.length()) return true;
        if( row < board.size() && col < board[0].size() && board[row][col] == word[chaIndex])
        {
            ++chaIndex;
            char temp = board[row][col];
            board[row][col] = '\0';
            if (existHelper(board, row, col-1,word,chaIndex) ||
                existHelper(board, row, col+1,word,chaIndex) ||
                existHelper(board, row-1, col,word,chaIndex) ||
                existHelper(board, row+1, col,word,chaIndex)) 
              return true;

            else{
                --chaIndex;
                board[row][col] = temp;
            }
        }
        return false;
    }
};
```

写这个题目时候有几点需要注意:
* 在书中使用一个 bool* 存放已经访问过的位置的信息, 我使用 `vector<vetcot<bool>>` 用类似的方法发现时间不够
* 可以在原数组中将访问过的元素设置为 `\0`, 如果失败的话就改回来, 这样看起来能节约不少时间空间
* 这里的回溯过程就是将 chaIndex 减小, 同时将已经访问过的点改回原来的值

## 面试题 13: 机器人的运动范围
题目描述:题目：地上有一个m 行n 列的方格。一个机器人从坐标(0, 0) 的格子开始移动，它每次可以向左、右、上、下移动一格，但不能进入行坐标和列坐标的数位之和大千k 的格子。例如，当k 为18 时，机器人能够进入方格(35, 37) ，因为3+5+3+7= 18 。但它不能进入方格(35 , 38) ， 因为3+5+3+8= 19 。请问该机器人能够到达多少个格子？<br>
解题思路: 
* 初始化 `vector<vetcot<bool>>(m,vector<bool>(n,false))` 用于存储访问过的点
* 如果当前点可行, 将访问信息设置为 true, sum + 1, 然后将周围的点全部累加起来, 计算过的就不会再算一次了

```C++
class Solution {
public:
    int movingCount(int m, int n, int k) {
        if(m <= 0 || n <= 0 || k < 0) return 0;
        else if(k == 0) return 1;
        else{
            vector<vector<bool>> marked = vector<vector<bool>>(m,vector<bool>(n,false)) ;
            return count(marked,0,0,m,n,k);
        }
    }
private:
    int getDigitsSum(int k);
    bool canCount(int row, int col, int m, int n,int k,vector<vector<bool>>& marked);
    int count(vector<vector<bool>>& marked, int row, int col, int m, int n, int k);
};
int Solution::getDigitsSum(int k){
    int digitSum{0};
    while(k>0){
        digitSum += (k % 10);
        k /= 10;
    }
    return digitSum;
}
bool Solution::canCount(int row, int col, int m, int n, int k,vector<vector<bool>>& marked){
    if(row >= 0 && row <=m-1 && col >= 0 && col <= n-1 && getDigitsSum(row)+ getDigitsSum(col) <=k && !marked[row][col] )
        return true;
    else return false;
}
int Solution::count(vector<vector<bool>>& marked, int row, int col, int m, int n, int k){
    int sum = 0;
    if(canCount(row,col,m,n, k,marked)){
        marked[row][col] = true;
        sum = 1 + count(marked, row+1, col, m,n,k) + count(marked, row-1, col, m,n,k) 
                + count(marked, row, col+1, m,n,k) + count(marked, row, col-1, m,n,k);
    }
    return sum;
}
```
做这个题目的时候, 最常见的额问题就是将 m,n,k 搞混, leetcode 默认给的命名不是特别好
## KMP 模式匹配算法

假设有 Source 字符串 S("abcababca") 和 Target 字符串 T("abcabx"), T 的首字母"a" 与第二位 "b" 以及第三位 "c" 都是不想等的, 可以忽略朴素匹配算法的某些判断. i 值不回溯, 就是不能表笑, 需要考虑的变化就是 j 值了, **j 值得大小取决于当前字符之前的串的前后缀相似程度**:
前缀, 如一个字符串 "abcd"就有三个前缀"a, ab , abc"(不包含自己), 后缀("d, cd, bcd"), 所谓的next[j] 的值就是前后缀相同的最长大小. 然后我们可以根据这个 next[] 数组跳过很多个 i 值回溯的部分, 因此程序的时间复杂度就从 $O((n-m+1)*m)$ 变到 $O(n+m)$