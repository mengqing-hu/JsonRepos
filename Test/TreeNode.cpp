#include "TreeNode.h"

// 构造函数
TreeNode::TreeNode() = default;

TreeNode::TreeNode(const std::string& name, const tmw::ValueVariant& value)
    : name_(name), value_(value) {}

// 析构函数
TreeNode::~TreeNode() {}

// Getter 和 Setter 方法
std::string TreeNode::getName() const {
    return name_;
}

// void TreeNode::setName(const std::string& name) {
//     name_ = name;
// }

tmw::ValueVariant TreeNode::getValue() const {
    return value_;
}

void TreeNode::setValue(const tmw::ValueVariant& value) {
    value_ = value;
}

tmw::SharedPtr<TreeNode> TreeNode::getParent() const {
    return parent_.lock();
}

// void TreeNode::setParent(const tmw::SharedPtr<TreeNode>& parentNode) {
//     parent_ = parentNode;
// }

const std::vector<tmw::SharedPtr<TreeNode>>& TreeNode::getChildren() const {
    return children_;
}

void TreeNode::setChildren(const std::vector<tmw::SharedPtr<TreeNode>>& children) {
    children_ = children;
}

// Add a child node
void TreeNode::addChild(const std::shared_ptr<TreeNode>& child) {
    children_.push_back(child);
}

// Find a child node by name
std::shared_ptr<TreeNode> TreeNode::findChildByName(const std::string& name) const {
    for (const auto& child : children_) {
        if (child->getName() == name) {
            return child; // Return the existing child if found
        }
    }
    return nullptr; // Return nullptr if no child with the given name is found
}

// Find or create a child node by name
std::shared_ptr<TreeNode> TreeNode::findOrCreateChild(const std::string& name) {
    for (const auto& child : children_) {
        if (child->getName() == name) {
            return child; // Return the existing child if found
        }
    }

    // Create a new child node if not found
    auto newChild = std::make_shared<TreeNode>(name);
    children_.push_back(newChild);
    return newChild;
}