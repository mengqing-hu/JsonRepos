#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include "include/json.hpp"

using json = nlohmann::json;

// -------- TreeNode 定义：孩子用 shared_ptr，父指针用 weak_ptr --------
struct TreeNode {
    std::string key;                                         // 键名
    std::string type;                                        // 类型
    std::weak_ptr<TreeNode> parent;                          // 弱引用，避免父子循环
    std::vector<std::shared_ptr<TreeNode>> children;         // N 叉孩子

    explicit TreeNode(const std::string& k, const std::string& t) : key(k), type(t) {}
};

// -------- 递归构建：从 json 节点构建 TreeNode --------
std::shared_ptr<TreeNode> buildTree(const json& j, const std::string& key = "", const std::shared_ptr<TreeNode>& parent = nullptr) {
    // 判断类型
    std::string type;
    if (j.is_object()) type = "Object";
    else if (j.is_array()) type = "Array";
    else if (j.is_string()) type = "String";
    else if (j.is_number()) type = "Number";
    else if (j.is_boolean()) type = "Boolean";
    else type = "Null";

    auto node = std::make_shared<TreeNode>(key, type);
    if (parent) node->parent = parent;                       // 只赋 weak_ptr，不增加引用计数

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

// -------- 打印树（缩进显示），输出键和类型到文件 --------
void printTreeToFile(const std::shared_ptr<TreeNode>& node, std::ofstream& ofs, int depth = 0) {
    if (!node) return;
    std::string indent(depth * 2, ' ');
    auto p = node->parent.lock();
    ofs << indent
        << "Key: " << node->key
        << ", Type: " << node->type
        << "  (Parent Key: " << (p ? p->key : "null") << ")\n";

    for (const auto& ch : node->children) {
        printTreeToFile(ch, ofs, depth + 1);
    }
}

int main() {
    try {
        // 1) 读取 JSON 文件
        std::ifstream ifs("Data DMU/DMU_5X_TWIN_20250203-122907347/DMU_5X_TWIN_6c8d211d-1585-4def-b09a-97a3eb376267_20250203-122907663.json");
        if (!ifs.is_open()) {
            std::cerr << "无法打开 JSON 文件\n";
            return 1;
        }
        json j;
        ifs >> j;

        // 2) 构建树（根的 key 为空，parent 为空）
        auto root = buildTree(j);

        // 3) 打开输出文件
        std::ofstream ofs("output.txt");
        if (!ofs.is_open()) {
            std::cerr << "无法创建输出文件\n";
            return 1;
        }

        // 4) 打印树结构到文件
        ofs << "=== 打印 JSON 键和类型 ===\n";
        printTreeToFile(root, ofs);

        std::cout << "JSON 键和类型已输出到 output.txt\n";

    } catch (const std::exception& e) {
        std::cerr << "异常: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
