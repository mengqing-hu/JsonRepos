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
    // tmw::SharedPtr<TreeNode> root_; // 根节点

    // // 动态 HF 信号缓存
    // mutable std::vector<std::string> hfSignals;

    // // 三个向量分别存 Name / Axis / Type
    // // mutable std::vector<std::string> hfNames;
    // // mutable std::vector<std::string> hfAxes;
    // // mutable std::vector<std::string> hfTypes;
    // // 辅助函数：从 Header.SignalListHFData 提取信号
    // void extractHFSignalsFromHeader() const;
    
    tmw::SharedPtr<TreeNode> root_; 
    mutable tmw::SharedPtr<TreeNode> hfSignals; // 指向 Header.SignalListHFData

    void extractHFSignalsFromHeader() const;

public:
    JsonCollector() : root_(tmw::MakeShared<TreeNode>("JsonRoot")) {}

    tmw::SharedPtr<TreeNode> getRoot() const {
        return root_;
    }

    tmw::SharedPtr<TreeNode> findNodeByName(const std::string& name) const;
    void initializeFromJson(const json& j);

    size_t countNodeOccurrences(const std::string &nodeName) const;
    void calculateChildNodeCount(const std::string &nodeName) const;
    void viewNodeDetailsAsTree(const std::string &nodeName) const;

    void viewPayloadHFData() const;
    void viewPayloadLFData() const;

    // 新函数：带信号名的 HFData 输出
    void viewPayloadHFDataWithSignals() const;
};

#endif // JSON_COLLECTOR_H
