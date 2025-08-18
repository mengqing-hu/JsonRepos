#include "JsonCollector.h"
#include <fstream>
#include <iostream>
#include "include/json.hpp"

using json = nlohmann::json;

int main() {
    try {
        // 读取 JSON 文件
        std::string jsonFilePath = "Data DMU/DMU_5X_TWIN_20250203-122907347/DMU_5X_TWIN_6c8d211d-1585-4def-b09a-97a3eb376267_20250203-122907663.json";
        std::ifstream ifs(jsonFilePath);
        if (!ifs.is_open()) {
            std::cerr << "无法打开 JSON 文件: " << jsonFilePath << "\n";
            return 1;
        }
        json j;
        ifs >> j;

        // 创建 JsonCollector 实例
        JsonCollector collector;

        // 将 JSON 数据解析为树结构
        if (j.contains("Header")) {
            collector.addRootChild("Header");
            for (auto& [key, value] : j["Header"].items()) {
                collector.addChildToNode(collector.findNodeByName("Header"), key, tmw::ValueVariant(value.dump()));
            }
        }

        if (j.contains("Footer")) {
            collector.addRootChild("Footer");
            for (auto& [key, value] : j["Footer"].items()) {
                collector.addChildToNode(collector.findNodeByName("Footer"), key, tmw::ValueVariant(value.dump()));
            }
        }

        if (j.contains("Payload")) {
            collector.addRootChild("Payload");
            for (const auto& item : j["Payload"]) {
                if (item.contains("LFData")) {
                    collector.addChildToNode(collector.findNodeByName("Payload"), "LFData", tmw::ValueVariant(item["LFData"].dump()));
                }
                if (item.contains("HFData")) {
                    collector.addChildToNode(collector.findNodeByName("Payload"), "HFData", tmw::ValueVariant(item["HFData"].dump()));
                }
                if (item.contains("HFTimestamp")) {
                    collector.addChildToNode(collector.findNodeByName("Payload"), "HFTimestamp", tmw::ValueVariant(item["HFTimestamp"].dump()));
                }
            }
        }

        // 查看 JSON 文件根节点
        collector.getRoot()->printNodeInfo();

        // 打印某节点的值
        collector.printNodeValue("LFData");
        collector.printNodeValue("HFData");
        collector.printNodeValue("Header");

        // 根据路径查找节点并打印值
        std::vector<std::string> path = {"Payload", "LFData"};
        auto node = collector.findNodeByPath(path);
        if (node) {
            std::cout << "Node found by path: " << node->getName() << ", Value: " << node->getValue().ToString() << "\n";
        } else {
            std::cout << "Node not found by path.\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "发生异常: " << e.what() << "\n";
        return 1;
    }

    return 0;
}