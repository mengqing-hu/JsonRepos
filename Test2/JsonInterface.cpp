#include "JsonInterface.h"
#include <iostream>
#include <iomanip>

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

// 显示 JSON 文件的结构
void displayJsonStructure(const json& j, const std::string& parentKey) {
    if (j.is_object()) {
        for (auto& [key, value] : j.items()) {
            std::cout << parentKey << "." << key << ": " << (value.is_object() ? "Object" : value.is_array() ? "Array" : "Value") << "\n";
            displayJsonStructure(value, parentKey + "." + key);
        }
    } else if (j.is_array()) {
        for (size_t i = 0; i < j.size(); ++i) {
            displayJsonStructure(j[i], parentKey + "[" + std::to_string(i) + "]");
        }
    }
}

// 查看 JSON 文件根节点
void viewJsonRoot(const json& j) {
    std::cout << "JSON 文件根节点:\n";
    if (j.contains("Header")) {
        std::cout << "- Header: Object\n";
    }
    if (j.contains("Footer")) {
        std::cout << "- Footer: Object\n";
    }
    if (j.contains("Payload")) {
        std::cout << "- Payload: Object\n";
    }
}

// 查看 Header 子节点
void viewHeader(const json& header) {
    std::cout << "Header 子节点:\n";
    for (auto& [key, value] : header.items()) {
        std::cout << "- " << key << ": " << (value.is_object() ? "Object" : value.is_array() ? "Array" : "Value") << "\n";
    }
}

// 查看 Footer 子节点
void viewFooter(const json& footer) {
    std::cout << "Footer 子节点:\n";
    for (auto& [key, value] : footer.items()) {
        std::cout << "- " << key << ": " << (value.is_object() ? "Object" : value.is_array() ? "Array" : "Value") << "\n";
    }
}

// 查看 Payload 子节点
void viewPayload(const json& payload) {
    std::cout << "Payload 子节点:\n";
    for (size_t i = 0; i < payload.size(); ++i) {
        const auto& item = payload[i];
        if (item.contains("LFData")) {
            std::cout << "- LFData: Array\n";
        }
        if (item.contains("HFTimestamp")) {
            std::cout << "- HFTimestamp: Object\n";
        }
        if (item.contains("HFData")) {
            std::cout << "- HFData: Array\n";
        }
    }
}

// 计算 Payload 中的总节点数
void calculatePayloadNodeCount(const json& payload) {
    size_t totalNodes = 0;
    for (const auto& item : payload) {
        totalNodes += item.size();
    }
    std::cout << "Payload 总节点数: " << totalNodes << "\n";
}

// 计算 LFData 子节点数
void calculateLFDataNodeCount(const json& lfData) {
    size_t nodeCount = lfData.size();
    std::cout << "LFData 子节点数: " << nodeCount << "\n";
}

// 计算 HFData 子节点数
void calculateHFDataNodeCount(const json& hfData) {
    size_t nodeCount = hfData.size();
    std::cout << "HFData 子节点数: " << nodeCount << "\n";
}

// 计算 HFTimestamp 子节点数
void calculateHFTimestampNodeCount(const json& hftimestamp) {
    size_t nodeCount = hftimestamp.size();
    std::cout << "HFTimestamp 子节点数: " << nodeCount << "\n";
}

// 查看 HFData 子节点
void viewHFData(const json& hfData) {
    std::cout << "HFData 子节点:\n";
    for (size_t i = 0; i < hfData.size(); ++i) {
        std::cout << "- Row " << i << ": [";
        for (const auto& value : hfData[i]) {
            std::cout << value << ", ";
        }
        std::cout << "]\n";
    }
}

// 查看 LFData 子节点
void viewLFData(const json& lfData) {
    std::cout << "LFData 子节点:\n";
    for (size_t i = 0; i < lfData.size(); ++i) {
        const auto& record = lfData[i];
        std::cout << "- Record " << i << ":\n";
        for (auto& [key, value] : record.items()) {
            std::cout << "  " << key << ": " << value << "\n";
        }
    }
}