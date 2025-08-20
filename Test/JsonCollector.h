#ifndef JSON_COLLECTOR_H
#define JSON_COLLECTOR_H

#include "TreeNode.h"
#include "include/json.hpp"
#include "pointers.h"
#include "value_variant.h"
#include <iostream>
#include <memory>
#include <queue>

using json = nlohmann::json;

class JsonCollector: public std::enable_shared_from_this<JsonCollector> {
private:
    tmw::SharedPtr<TreeNode> root_; // 根节点

public:
    // 构造函数
    JsonCollector() : root_(tmw::MakeShared<TreeNode>("JsonRoot")) {}

    // 获取根节点
    tmw::SharedPtr<TreeNode> getRoot() const {
        return root_;
    }

    // 根据名称查找节点
    tmw::SharedPtr<TreeNode> findNodeByName(const std::string& name) const;

    // 初始化树结构（从 JSON 数据构建）
    void initializeFromJson(const json& j);

    size_t countNodeOccurrences(const std::string &nodeName) const;

    void calculateChildNodeCount(const std::string &nodeName) const;

    void viewNodeDetailsAsTree(const std::string &nodeName) const;

    void viewPayloadHFData() const;
    void viewPayloadLFData() const;

};

#endif // JSON_COLLECTOR_H