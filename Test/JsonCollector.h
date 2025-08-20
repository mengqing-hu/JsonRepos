#ifndef JSON_COLLECTOR_H
#define JSON_COLLECTOR_H

#include "TreeNode.h"
#include "include/json.hpp"
#include "pointers.h"
#include "value_variant.h"
#include <iostream>
#include <memory>
#include <queue>
#include <vector>
#include <string>

using json = nlohmann::json;

class JsonCollector: public std::enable_shared_from_this<JsonCollector> {
private:
    tmw::SharedPtr<TreeNode> root_; // 根节点

    // 内置 HF 信号表
    inline static const std::vector<std::string> hfSignals = {
        "CYCLE",
        "DES_POS|1", "DES_POS|2", "DES_POS|3", "DES_POS|4", "DES_POS|5", "DES_POS|6",
        "TORQUE|1", "TORQUE|2", "TORQUE|3", "TORQUE|4", "TORQUE|5", "TORQUE|6",
        "ENC2_POS|1", "ENC2_POS|2", "ENC2_POS|3", "ENC2_POS|4", "ENC2_POS|5", "ENC2_POS|6",
        "ENC1_POS|1", "ENC1_POS|2", "ENC1_POS|3", "ENC1_POS|4", "ENC1_POS|5", "ENC1_POS|6",
        "CURRENT|1", "CURRENT|2", "CURRENT|3", "CURRENT|4", "CURRENT|5", "CURRENT|6",
        "CONT_DEV|1", "CONT_DEV|2", "CONT_DEV|3", "CONT_DEV|4", "CONT_DEV|5", "CONT_DEV|6"
    };

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

    // 新函数：打印带信号名的 HFData
    void viewPayloadHFDataWithSignals() const;
};

#endif // JSON_COLLECTOR_H
