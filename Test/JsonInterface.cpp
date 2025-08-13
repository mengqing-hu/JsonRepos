#include "JsonInterface.h"
#include <iostream>

// 构建树的递归函数
std::shared_ptr<TreeNode> buildTree(const json& j, const std::string& key, const std::shared_ptr<TreeNode>& parent) {
    std::string type;
    if (j.is_object()) type = "Object";
    else if (j.is_array()) type = "Array";
    else if (j.is_string()) type = "String";
    else if (j.is_number()) type = "Number";
    else if (j.is_boolean()) type = "Boolean";
    else type = "Null";

    auto node = std::make_shared<TreeNode>(key, type);
    if (parent) node->parent = parent;

    if (j.is_object()) {
        for (auto& [childKey, childValue] : j.items()) {
            node->children.push_back(buildTree(childValue, childKey, node));
        }
    } else if (j.is_array()) {
        int index = 0;
        for (const auto& childValue : j) {
            node->children.push_back(buildTree(childValue, "index_" + std::to_string(index++), node));
        }
    }
    return node;
}

// 计数 HFData 和 LFData 的函数
void countHFAndLFData(const json& j, int& hfCount, int& lfCount) {
    if (j.is_object()) {
        for (auto& [key, value] : j.items()) {
            if (key == "HFData") hfCount++;
            if (key == "LFData") lfCount++;
            countHFAndLFData(value, hfCount, lfCount);
        }
    } else if (j.is_array()) {
        for (const auto& value : j) {
            countHFAndLFData(value, hfCount, lfCount);
        }
    }
}

// 打印树结构到文件
void printTreeToFile(const std::shared_ptr<TreeNode>& root, const std::string& outputFilePath) {
    std::ofstream ofs(outputFilePath);
    if (!ofs.is_open()) {
        std::cerr << "无法创建输出文件: " << outputFilePath << "\n";
        return;
    }

    std::function<void(const std::shared_ptr<TreeNode>&, int)> printTreeRecursively = [&](const std::shared_ptr<TreeNode>& node, int depth) {
        if (!node) return;
        std::string indent(depth * 2, ' ');
        auto p = node->parent.lock();
        ofs << indent << "Key: " << node->key << ", Type: " << node->type
            << "  (Parent Key: " << (p ? p->key : "null") << ")\n";

        for (const auto& child : node->children) {
            printTreeRecursively(child, depth + 1);
        }
    };

    printTreeRecursively(root, 0);
    ofs.close();
}