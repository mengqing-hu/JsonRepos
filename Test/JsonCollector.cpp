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

// void JsonCollector::extractHFSignalsFromHeader() const {
//     hfSignals.clear();

//     auto headerNode = findNodeByName("Header");
//     if (!headerNode) {
//         std::cerr << "节点未找到: Header\n";
//         return;
//     }

//     // 在 Header 下找 SignalListHFData
//     auto signalListNode = headerNode->findChildByName("SignalListHFData");
//     if (!signalListNode) {
//         std::cerr << "节点未找到: SignalListHFData\n";
//         return;
//     }

//     // 遍历 SignalListHFData 的子节点
//     for (const auto& signalNode : signalListNode->getChildren()) {
//         auto nameNode = signalNode->findChildByName("Name");
//         if (nameNode) {
//             hfSignals.push_back(nameNode->getValue().ToString());
//         }
//     }

//     std::cout << "已提取 HF 信号数量: " << hfSignals.size() << "\n";
// }
// void JsonCollector::viewPayloadHFDataWithSignals() const {
//     // 先动态提取信号名
//     if (hfSignals.empty()) {
//         extractHFSignalsFromHeader();
//     }

//     auto payloadNode = findNodeByName("Payload");
//     if (!payloadNode) {
//         std::cerr << "节点未找到: Payload\n";
//         return;
//     }

//     std::cout << "Payload 下 HFData 与信号对应信息:\n";

//     std::function<void(const tmw::SharedPtr<TreeNode>&)> printHFDataRecursive =
//     [&](const tmw::SharedPtr<TreeNode>& currentNode) {
//         if (currentNode->getName() == "HFData") {
//             for (const auto& row : currentNode->getChildren()) {
//                 const auto& values = row->getChildren();

//                 std::cout << "---- HFData Row ----\n";
//                 for (size_t i = 0; i < values.size(); ++i) {
//                     std::string signalName = (i < hfSignals.size()) ? hfSignals[i] : "Unknown";
//                     std::cout << signalName << " = " << values[i]->getValue().ToString() << "\n";
//                 }
//             }
//         }

//         for (const auto& child : currentNode->getChildren()) {
//             printHFDataRecursive(child);
//         }
//     };

//     printHFDataRecursive(payloadNode);
// }
void JsonCollector::extractHFSignalsFromHeader() const {
    auto headerNode = findNodeByName("Header");
    if (!headerNode) {
        std::cerr << "节点未找到: Header\n";
        return;
    }

    hfSignals = headerNode->findChildByName("SignalListHFData");
    if (!hfSignals) {
        std::cerr << "节点未找到: SignalListHFData\n";
        return;
    }

    std::cout << "已找到 HF 信号数组，数量: " << hfSignals->getChildren().size() << "\n";
}

void JsonCollector::viewPayloadHFDataWithSignals() const {
    if (!hfSignals) {
        extractHFSignalsFromHeader();
    }
    if (!hfSignals) {
        return;
    }

    auto payloadNode = findNodeByName("Payload");
    if (!payloadNode) {
        std::cerr << "节点未找到: Payload\n";
        return;
    }

    std::cout << "Payload 下 HFData 与信号对应信息:\n";

    std::function<void(const tmw::SharedPtr<TreeNode>&)> printHFDataRecursive =
    [&](const tmw::SharedPtr<TreeNode>& currentNode) {
        if (currentNode->getName() == "HFData") {
            for (const auto& row : currentNode->getChildren()) {
                const auto& values = row->getChildren();

                std::cout << "---- HFData Row ----\n";
                for (size_t i = 0; i < values.size(); ++i) {
                    if (i < hfSignals->getChildren().size()) {
                        auto signalNode = hfSignals->getChildren()[i];
                        std::string name, axis, type;

                        auto nameNode = signalNode->findChildByName("Name");
                        if (nameNode) name = nameNode->getValue().ToString();

                        auto axisNode = signalNode->findChildByName("Axis");
                        if (axisNode) axis = axisNode->getValue().ToString();

                        auto typeNode = signalNode->findChildByName("Type");
                        if (typeNode) type = typeNode->getValue().ToString();

                        std::cout << name << " (" << axis << ", " << type << ")"
                                  << " = " << values[i]->getValue().ToString() << "\n";
                    } else {
                        std::cout << "UnknownSignal[" << i << "] = " << values[i]->getValue().ToString() << "\n";
                    }
                }
            }
        }

        for (const auto& child : currentNode->getChildren()) {
            printHFDataRecursive(child);
        }
    };

    printHFDataRecursive(payloadNode);
}


// void JsonCollector::viewPayloadHFDataWithSignals() const {
//     auto payloadNode = findNodeByName("Payload");
//     if (!payloadNode) {
//         std::cerr << "节点未找到: Payload\n";
//         return;
//     }

//     std::cout << "Payload 下 HFData 与信号对应信息:\n";

//     // 递归查找 HFData 节点
//     std::function<void(const tmw::SharedPtr<TreeNode>&)> printHFDataRecursive =
//     [&](const tmw::SharedPtr<TreeNode>& currentNode) {
//         if (currentNode->getName() == "HFData") {
//             for (const auto& row : currentNode->getChildren()) {
//                 const auto& values = row->getChildren();

//                 std::cout << "---- HFData Row ----\n";
//                 for (size_t i = 0; i < values.size(); ++i) {
//                     std::string signalName = (i < hfSignals.size()) ? hfSignals[i] : "Unknown";
//                     std::cout << signalName << " = " << values[i]->getValue().ToString() << "\n";
//                 }
//             }
//         }

//         for (const auto& child : currentNode->getChildren()) {
//             printHFDataRecursive(child);
//         }
//     };

//     printHFDataRecursive(payloadNode);
// }
