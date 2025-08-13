#ifndef JSON_INTERFACE_H
#define JSON_INTERFACE_H

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "include/json.hpp"

using json = nlohmann::json;

// TreeNode 结构体定义
struct TreeNode {
    std::string key;
    std::string type;
    std::weak_ptr<TreeNode> parent;
    std::vector<std::shared_ptr<TreeNode>> children;

    explicit TreeNode(const std::string& k, const std::string& t) : key(k), type(t) {}
};

// 接口函数声明
std::shared_ptr<TreeNode> buildTree(const json& j, const std::string& key = "", const std::shared_ptr<TreeNode>& parent = nullptr);
void countHFAndLFData(const json& j, int& hfCount, int& lfCount);
void printTreeToFile(const std::shared_ptr<TreeNode>& root, const std::string& outputFilePath);

#endif // JSON_INTERFACE_H