#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <memory> // 使用 std::shared_ptr 和 std::weak_ptr
#include "value_variant.h" // 使用 ValueVariant
#include "pointers.h" // 使用 tmw::SharedPtr 和 tmw::WeakPtr
#include <string>
#include <vector>
#include <iostream>

class TreeNode : public std::enable_shared_from_this<TreeNode> {
private:
    std::string name_; // 节点名称
    tmw::ValueVariant value_; // 节点值（支持多种类型）
    tmw::WeakPtr<TreeNode> parent_; // 父节点
    std::vector<tmw::SharedPtr<TreeNode>> children_; // 子节点列表

public:
    // 构造函数
    TreeNode();
    TreeNode(const std::string& name, const tmw::ValueVariant& value = tmw::ValueVariant());

    // 析构函数
    virtual ~TreeNode();

    // Getter 和 Setter 方法
    std::string getName() const;
    // void setName(const std::string& name);

    tmw::ValueVariant getValue() const;
    void setValue(const tmw::ValueVariant& value);

    tmw::SharedPtr<TreeNode> getParent() const;
    // void setParent(const tmw::SharedPtr<TreeNode>& parentNode);

    const std::vector<tmw::SharedPtr<TreeNode>>& getChildren() const;
    void setChildren(const std::vector<tmw::SharedPtr<TreeNode>>& children);

    std::shared_ptr<TreeNode> findChildByName(const std::string &name) const;

    void addChild(const std::shared_ptr<TreeNode> &child);
    
    std::shared_ptr<TreeNode> findOrCreateChild(const std::string &name);
};

#endif // TREE_NODE_H