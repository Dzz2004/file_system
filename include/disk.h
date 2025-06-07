/**
 * @file disk.h
 * @brief Disk management functions.
 * @details This header file contains declarations for functions that manage disk operations,
 * including reading and writing sectors, and managing disk geometry.
 * @note This file is part of a larger project that implements a disk management system.
 * @author Zhaungzhi Dong
 * @date 2025-6-4
 */

#ifndef DISK_H 
#define DISK_H

#include <string>
#include <fstream>
#include <cstring>

class DiskManager {
public:
    static constexpr int BLOCK_SIZE = 1024;          // 每块大小为 1024 字节
    static constexpr int BLOCK_COUNT = 1024;         // 总共 1024 块，共 1MB

    DiskManager();

    void loadDisk(const std::string& filename);  // 从文件加载虚拟磁盘
    void saveDisk(const std::string& filename);  // 将虚拟磁盘保存到文件

    int allocateBlock();      // 分配一个空闲块，返回块索引，失败返回 -1
    void freeBlock(int idx);  // 释放指定块
    char* getBlock(int idx);  // 获取块的指针

private:
    char disk[BLOCK_COUNT][BLOCK_SIZE];     // 虚拟磁盘内存
    bool blockBitmap[BLOCK_COUNT];          // 块使用位图
};

#endif // DISK_H
