# JsonRepos
# 项目主要使用C++ nlohmann/json

## 用于读取一个 JSON 文件并解析其内容，构建一个树形结构来表示 JSON 数据的层次关系。程序的主要功能包括：

1. 定义树节点 (TreeNode)：
使用 shared_ptr 和 weak_ptr 实现树形结构，避免循环引用。
每个节点包含键名 (key)、类型 (type)、父节点指针 (parent)，以及子节点列表 (children)。

2. 递归构建树 (buildTree)：
根据 JSON 数据的层次结构递归构建树。
支持对象 (Object)、数组 (Array)、字符串 (String)、数字 (Number)、布尔值 (Boolean) 和空值 (Null) 类型。

3. 打印树结构到文件 (printTreeToFile)：
将树结构以缩进格式输出到一个 .txt 文件中。
输出每个节点的键名、类型，以及父节点的键名。

4. 主程序 (main)：
读取指定的 JSON 文件。
构建树结构。
将树结构输出到 output.txt 文件。



## Test文件夹
设计一个接口操作json文件实现以下功能： 
1. 利用nlohmann/json，使用TreeNode结构，使用weak_ptr和shared_ptr
2. 将每个功能设计成一个函数可以进行调用，现在先设计两个功能，每次调用对应函数执行对应功能：
  功能一：实现Payload中的HFData和LFData的计数并能在函数调用后输出结果
  功能二：将树结构以缩进格式输出到一个 .txt 文件中。
### 程序运行
clang++ -std=gnu++17 -I./include JsonInterface.cpp main.cpp -o main && ./main
