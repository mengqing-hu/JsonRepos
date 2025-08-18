#ifndef JSONINTERFACE_H
#define JSONINTERFACE_H

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "include/json.hpp"
using json = nlohmann::json;

// TreeNode 结构体定义
struct TreeNode : public std::enable_shared_from_this<TreeNode> {
    std::string key;
    std::string type;
    std::weak_ptr<TreeNode> parent;
    std::vector<std::shared_ptr<TreeNode>> children;

    explicit TreeNode(const std::string& k, const std::string& t) : key(k), type(t) {}
};

// 接口函数声明
std::shared_ptr<TreeNode> buildTree(const json& j, const std::string& key = "", const std::shared_ptr<TreeNode>& parent = nullptr);
void viewJsonRoot(const json& j);
void viewHeader(const json& header);
void viewFooter(const json& footer);
void viewPayload(const json& payload);
void viewHFData(const json& hfData);
void viewLFData(const json& lfData);


void calculatePayloadNodeCount(const json& payload);
void calculateLFDataNodeCount(const json& lfData);
void calculateHFDataNodeCount(const json& hfData);
void calculateHFTimestampNodeCount(const json& hftimestamp);




#endif 