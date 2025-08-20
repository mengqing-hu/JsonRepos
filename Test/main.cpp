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
        collector.initializeFromJson(j);

        collector.viewPayloadHFDataWithSignals();
        // collector.calculateChildNodeCount("Payload");
        // std::cout << "--------------- " << "\n";
        // collector.viewNodeDetailsAsTree("Header");

        // // 统计 Payload 节点下 LFData 的数量
        // size_t lfDataCount = collector.countNodeOccurrences("LFData");
        // std::cout << "Payload 节点下 LFData 的数量: " << lfDataCount << "\n";

        // // 统计 Payload 节点下 HFData 的数量
        // size_t hfDataCount = collector.countNodeOccurrences("HFData");
        // std::cout << "Payload 节点下 HFData 的数量: " << hfDataCount << "\n";

        // std::cout << "--------------- " << "\n";
        // collector.getRoot();
        // std::cout << "--------------- " << "\n";
        // // 查看 Payload 节点下的 LFData 信息
        // collector.viewPayloadLFData();
        // // 查看 Payload 节点下的 HFData 信息
        // collector.viewPayloadHFData();


    } catch (const std::exception& e) {
        std::cerr << "发生异常: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
