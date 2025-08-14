#include "JsonInterface.h"
#include <iostream>
#include <fstream>

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

        // 功能一：计数 HFData 和 LFData
        int hfCount = 0, lfCount = 0;
        countHFAndLFData(j, hfCount, lfCount);
        std::cout << "HFData 数量: " << hfCount << "\n";
        std::cout << "LFData 数量: " << lfCount << "\n";

        // 功能二：打印树结构到文件
        auto root = buildTree(j);
        std::string outputFilePath = "output.txt";
        printTreeToFile(root, outputFilePath);
        std::cout << "树结构已输出到: " << outputFilePath << "\n";

    } catch (const std::exception& e) {
        std::cerr << "异常: " << e.what() << "\n";
        return 1;
    }
    return 0;
}