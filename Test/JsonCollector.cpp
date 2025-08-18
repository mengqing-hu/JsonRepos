#include "JsonCollector.h"
#include "include/json.hpp" // Include nlohmann::json

// 递归解析 JSON 数据并构建 TreeNode 树
tmw::SharedPtr<TreeNode> parseJsonToTreeNode(const std::string& name, const json& j) {
    auto node = tmw::MakeShared<TreeNode>(name);

    if (j.is_object()) {
        std::vector<tmw::SharedPtr<TreeNode>> children;
        for (auto it = j.begin(); it != j.end(); ++it) {
            children.push_back(parseJsonToTreeNode(it.key(), it.value()));
        }
        node->setChildren(children);
    } else if (j.is_array()) {
        std::vector<tmw::SharedPtr<TreeNode>> children;
        for (size_t i = 0; i < j.size(); ++i) {
            children.push_back(parseJsonToTreeNode(std::to_string(i), j[i]));
        }
        node->setChildren(children);
    } else {
        node->setValue(tmw::ValueVariant(j.dump())); // 将值存储为字符串
    }

    return node;
}

// 根据名称查找节点
tmw::SharedPtr<TreeNode> JsonCollector::findNodeByName(const std::string& name) const {
    std::queue<tmw::SharedPtr<TreeNode>> nodeQueue;
    nodeQueue.push(root_);

    while (!nodeQueue.empty()) {
        auto current = nodeQueue.front();
        nodeQueue.pop();

        if (current->getName() == name) {
            return current;
        }

        for (const auto& child : current->getChildren()) {
            nodeQueue.push(child);
        }
    }

    return nullptr; // 未找到节点
}

// 初始化树结构（从 JSON 数据构建）
void JsonCollector::initializeFromJson(const json& j) {
    std::vector<tmw::SharedPtr<TreeNode>> children; // Temporary vector to hold children
    for (auto it = j.begin(); it != j.end(); ++it) {
        children.push_back(parseJsonToTreeNode(it.key(), it.value()));
    }
    root_->setChildren(children); // Set the children vector using setChildren()
}

// 打印树结构
void JsonCollector::printTreeStructure() const {
    std::queue<std::pair<tmw::SharedPtr<TreeNode>, int>> nodeQueue;
    nodeQueue.push({root_, 0});

    while (!nodeQueue.empty()) {
        auto [current, depth] = nodeQueue.front();
        nodeQueue.pop();

        std::cout << std::string(depth * 2, ' ') << current->getName() << "\n";

        for (const auto& child : current->getChildren()) {
            nodeQueue.push({child, depth + 1});
        }
    }
}

// 查询对应节点的子节点数量
size_t JsonCollector::getChildNodeCount(const std::string& nodeName) const {
    auto node = findNodeByName(nodeName);
    if (!node) {
        std::cerr << "节点未找到: " << nodeName << "\n";
        return 0;
    }

    return node->getChildren().size();
}

// 查询文件中含有对应节点名称的数量
size_t JsonCollector::countNodeOccurrences(const std::string& nodeName) const {
    size_t count = 0;
    std::queue<tmw::SharedPtr<TreeNode>> nodeQueue;
    nodeQueue.push(root_);

    while (!nodeQueue.empty()) {
        auto current = nodeQueue.front();
        nodeQueue.pop();

        if (current->getName() == nodeName) {
            count++;
        }

        for (const auto& child : current->getChildren()) {
            nodeQueue.push(child);
        }
    }

    return count;
}

// 查询对应节点的子节点名称和值
void JsonCollector::printChildNodeDetails(const std::string& nodeName) const {
    auto node = findNodeByName(nodeName);
    if (!node) {
        std::cerr << "节点未找到: " << nodeName << "\n";
        return;
    }

    const auto& children = node->getChildren();
    if (children.empty()) {
        std::cout << "节点 " << nodeName << " 没有子节点。\n";
        return;
    }

    std::cout << "节点 " << nodeName << " 的子节点名称:\n";
    for (const auto& child : children) {
        std::cout << "  节点名称: " << child->getName() << "\n";
    }
}

void JsonCollector::calculateChildNodeCount(const std::string& nodeName) const {
    auto node = findNodeByName(nodeName);
    if (!node) {
        std::cerr << "节点未找到: " << nodeName << "\n";
        return;
    }

    const auto& children = node->getChildren();
    size_t count = children.size();

    std::cout << "节点 " << nodeName << " 的子节点数量: " << count << "\n";
}

void JsonCollector::calculateNodeNameCount(const std::string& parentNodeName, const std::string& targetNodeName) const {
    auto parentNode = findNodeByName(parentNodeName);
    if (!parentNode) {
        std::cerr << "父节点未找到: " << parentNodeName << "\n";
        return;
    }

    const auto& children = parentNode->getChildren();
    size_t count = 0;

    for (const auto& child : children) {
        if (child->getName() == targetNodeName) {
            count++;
        }
    }

    std::cout << "节点 " << parentNodeName << " 下名称为 \"" << targetNodeName << "\" 的子节点数量: " << count << "\n";
}

void JsonCollector::viewNodeDetailsAsTree(const std::string& nodeName) const {
    auto node = findNodeByName(nodeName);
    if (!node) {
        std::cerr << "节点未找到: " << nodeName << "\n";
        return;
    }

    std::cout << "节点 " << nodeName << " 的详细信息（以树结构显示）:\n";

    // Recursive lambda function to print the tree structure
    std::function<void(const tmw::SharedPtr<TreeNode>&, int, const std::string&, bool)> printNodeRecursive = [&](const tmw::SharedPtr<TreeNode>& currentNode, int depth, const std::string& parentName, bool isArray) {
        const auto& children = currentNode->getChildren();

        if (children.empty()) {
            // Leaf node: Display name and value
            std::cout << std::string(depth * 2, ' ') << "节点名称: " << currentNode->getName();
            std::string value = currentNode->getValue().ToString();
            if (!value.empty()) {
                std::cout << ", 值: " << value;
            }
            std::cout << "\n";
        } else {
            // Non-leaf node: Display name only
            if (isArray) {
                for (size_t i = 0; i < children.size(); ++i) {
                    std::cout << std::string(depth * 2, ' ') << parentName << "[" << i << "]\n";
                    printNodeRecursive(children[i], depth + 1, parentName, false);
                }
            } else {
                std::cout << std::string(depth * 2, ' ') << "节点名称: " << currentNode->getName() << "\n";
                for (const auto& child : children) {
                    printNodeRecursive(child, depth + 1, currentNode->getName(), currentNode->getValue().ToString() == "array");
                }
            }
        }
    };

    // Start recursion from the root node
    printNodeRecursive(node, 0, nodeName, false);
}

void JsonCollector::viewPayloadLFDataAsTree() const {
    auto payloadNode = findNodeByName("Payload");
    if (!payloadNode) {
        std::cerr << "节点未找到: Payload\n";
        return;
    }

    std::cout << "节点 Payload 下的 LFData 信息（以树结构显示）:\n";

    std::function<void(const tmw::SharedPtr<TreeNode>&, int)> printLFDataRecursive = [&](const tmw::SharedPtr<TreeNode>& currentNode, int depth) {
        if (currentNode->getName() == "LFData") {
            std::cout << std::string(depth * 2, ' ') << "节点名称: " << currentNode->getName() << "\n";

            for (const auto& child : currentNode->getChildren()) {
                std::cout << std::string((depth + 1) * 2, ' ') << "值: " << child->getValue().ToString() << "\n";
            }
        }

        for (const auto& child : currentNode->getChildren()) {
            printLFDataRecursive(child, depth + 1);
        }
    };

    printLFDataRecursive(payloadNode, 0);
}

void JsonCollector::viewPayloadHFDataAsTree() const {
    auto headerNode = findNodeByName("Header");
    if (!headerNode) {
        std::cerr << "节点未找到: Header\n";
        return;
    }

    auto signalListHFDataNode = headerNode->findChildByName("SignalListHFData");
    if (!signalListHFDataNode) {
        std::cerr << "节点未找到: SignalListHFData\n";
        return;
    }

    // Parse SignalListHFData from the Header node
    json signalListHFData;
    try {
        signalListHFData = json::parse(signalListHFDataNode->getValue().ToString());
    } catch (const std::exception& e) {
        std::cerr << "解析 SignalListHFData 时发生异常: " << e.what() << "\n";
        return;
    }

    auto payloadNode = findNodeByName("Payload");
    if (!payloadNode) {
        std::cerr << "节点未找到: Payload\n";
        return;
    }

    std::cout << "节点 Payload 下的 HFData 信息（以树结构显示）:\n";

    // Parse SignalListHFData to create column mappings
    std::unordered_map<int, json> columnMapping;
    for (size_t i = 0; i < signalListHFData.size(); ++i) {
        columnMapping[i + 1] = signalListHFData[i]; // Skip index 0 (HFProbeCounter)
    }

    // Recursive lambda function to process HFData
    std::function<void(const tmw::SharedPtr<TreeNode>&, int)> processHFDataRecursive = [&](const tmw::SharedPtr<TreeNode>& currentNode, int depth) {
        if (currentNode->getName() == "HFData") {
            for (const auto& row : currentNode->getChildren()) {
                auto hfDataRowNode = tmw::MakeShared<TreeNode>("HFData_Row");

                // Ensure row has children
                if (row->getChildren().empty()) {
                    std::cerr << "HFData 行没有子节点\n";
                    continue;
                }

                // Add HFProbeCounter
                hfDataRowNode->addChild(tmw::MakeShared<TreeNode>("HFProbeCounter", row->getChildren()[0]->getValue()));

                for (size_t col = 1; col < row->getChildren().size(); ++col) {
                    // Ensure columnMapping contains the column
                    if (columnMapping.find(col) == columnMapping.end()) {
                        std::cerr << "列号 " << col << " 在 SignalListHFData 中未找到\n";
                        continue;
                    }

                    const auto& signal = columnMapping[col];
                    std::string name = signal["Name"];
                    std::string axis = signal["Axis"];
                    std::string group = name.substr(0, name.find('|'));

                    auto groupNode = hfDataRowNode->findOrCreateChild(group);
                    groupNode->addChild(tmw::MakeShared<TreeNode>(axis, row->getChildren()[col]->getValue()));
                }

                // Print the constructed TreeNode
                std::function<void(const tmw::SharedPtr<TreeNode>&, int)> printTreeNode = [&](const tmw::SharedPtr<TreeNode>& node, int depth) {
                    std::cout << std::string(depth * 2, ' ') << node->getName();
                    if (!node->getValue().ToString().empty()) {
                        std::cout << ": " << node->getValue().ToString();
                    }
                    std::cout << "\n";
                    for (const auto& child : node->getChildren()) {
                        printTreeNode(child, depth + 1);
                    }
                };

                printTreeNode(hfDataRowNode, depth);
            }
        }

        for (const auto& child : currentNode->getChildren()) {
            processHFDataRecursive(child, depth + 1);
        }
    };

    processHFDataRecursive(payloadNode, 0);
}

void JsonCollector::viewPayloadHFData() const {
    auto payloadNode = findNodeByName("Payload");
    if (!payloadNode) {
        std::cerr << "节点未找到: Payload\n";
        return;
    }

    std::cout << "节点 Payload 下的 HFData 信息:\n";

    // Recursive lambda function to print HFData
    std::function<void(const tmw::SharedPtr<TreeNode>&, int)> printHFDataRecursive = [&](const tmw::SharedPtr<TreeNode>& currentNode, int depth) {
        if (currentNode->getName() == "HFData") {
            std::cout << std::string(depth * 2, ' ') << "节点名称: " << currentNode->getName() << "\n";
            for (const auto& row : currentNode->getChildren()) {
                std::cout << std::string((depth + 1) * 2, ' ') << "[ ";
                for (const auto& value : row->getChildren()) {
                    std::cout << value->getValue().ToString() << " ";
                }
                std::cout << "]\n";
            }
        }

        for (const auto& child : currentNode->getChildren()) {
            printHFDataRecursive(child, depth + 1);
        }
    };

    printHFDataRecursive(payloadNode, 0);
}

void JsonCollector::viewPayloadLFData() const {
    auto payloadNode = findNodeByName("Payload");
    if (!payloadNode) {
        std::cerr << "节点未找到: Payload\n";
        return;
    }

    std::cout << "节点 Payload 下的 LFData 信息:\n";

    // Recursive lambda function to print LFData
    std::function<void(const tmw::SharedPtr<TreeNode>&, int)> printLFDataRecursive = [&](const tmw::SharedPtr<TreeNode>& currentNode, int depth) {
        if (currentNode->getName() == "LFData") {
            std::cout << std::string(depth * 2, ' ') << "节点名称: " << currentNode->getName() << "\n";
            for (const auto& child : currentNode->getChildren()) {
                std::cout << std::string((depth + 1) * 2, ' ') << "值: {\n";
                for (const auto& field : child->getChildren()) {
                    std::cout << std::string((depth + 2) * 2, ' ') << field->getName() << ": " << field->getValue().ToString() << "\n";
                }
                std::cout << std::string((depth + 1) * 2, ' ') << "}\n";
            }
        }

        for (const auto& child : currentNode->getChildren()) {
            printLFDataRecursive(child, depth + 1);
        }
    };

    printLFDataRecursive(payloadNode, 0);
}

