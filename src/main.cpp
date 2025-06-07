// main.cpp
#include "fs.h"
#include "fileop.h"
#include <iostream>
#include <fstream>

int main() {
    FileSystemContext fsCtx;
    FileOp fileOp(fsCtx);

    const std::string diskFile = "vdisk_final.dat";

    // 加载虚拟磁盘和元数据
    std::ifstream check(diskFile, std::ios::binary);
    if (check.good()) {
        try {
            fsCtx.load(diskFile);
        } catch (...) {
            std::cerr << "Failed to load existing file system. Starting fresh.\n";
            fsCtx = FileSystemContext(); // 初始化新系统
        }
    } else {
        std::cout << "No existing file system found. You may use `new` to create one.\n";
    }

    // 启动命令行界面
    fileOp.run();

    // 保存虚拟磁盘和元数据
    try {
        fsCtx.save(diskFile);
    } catch (const std::exception& e) {
        std::cerr << "Failed to save file system: " << e.what() << std::endl;
    }

    return 0;
}
