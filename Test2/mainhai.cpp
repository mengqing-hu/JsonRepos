#include "JsonInterface.h"
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

        // 调用功能函数一：查看 JSON 文件根节点
        viewJsonRoot(j);

        // 调用功能函数二：查看 Header 子节点
        if (j.contains("Header")) {
            viewHeader(j["Header"]);
        }

        // 调用功能函数三：查看 Footer 子节点
        if (j.contains("Footer")) {
            viewFooter(j["Footer"]);
        }

        // // 调用功能函数四：查看 Payload 子节点
        // if (j.contains("Payload")) {
        //     viewPayload(j["Payload"]);

        //     // 调用新增功能函数：计算 Payload 总节点数
        //     calculatePayloadNodeCount(j["Payload"]);

        //     for (const auto& item : j["Payload"]) {
        //         // 调用查看 LFData 子节点的函数
        //         if (item.contains("LFData")) {
        //             viewLFData(item["LFData"]);

        //             // 调用新增功能函数：计算 LFData 子节点数
        //             calculateLFDataNodeCount(item["LFData"]);
        //         }

        //         // 调用查看 HFData 子节点的函数
        //         if (item.contains("HFData")) {
        //             viewHFData(item["HFData"]);

        //             // 调用新增功能函数：计算 HFData 子节点数
        //             calculateHFDataNodeCount(item["HFData"]);
        //         }

        //         // 调用新增功能函数：计算 HFTimestamp 子节点数
        //         if (item.contains("HFTimestamp")) {
        //             calculateHFTimestampNodeCount(item["HFTimestamp"]);
        //         }
        //     }
        // }
    } catch (const std::exception& e) {
        std::cerr << "发生异常: " << e.what() << "\n";
        return 1;
    }

    return 0;
}