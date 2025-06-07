#include "disk.h"
#include <iostream>

int main() {
    DiskManager dm;

    // 测试块分配
    int block1 = dm.allocateBlock();
    int block2 = dm.allocateBlock();
    if (block1 != -1 && block2 != -1) {
        std::cout << "Allocated blocks: " << block1 << ", " << block2 << std::endl;

        // 写入数据
        char* b1 = dm.getBlock(block1);
        char* b2 = dm.getBlock(block2);
        strcpy(b1, "Hello, Block1!");
        strcpy(b2, "Hello, Block2!");
    } else {
        std::cout << "Failed to allocate blocks." << std::endl;
        return 1;
    }

    // 保存到文件
    dm.saveDisk("vdisk.dat");
    std::cout << "Disk saved to vdisk.dat" << std::endl;

    // 清空再加载
    DiskManager dm2;
    dm2.loadDisk("vdisk.dat");
    std::cout << "Disk loaded from vdisk.dat" << std::endl;

    // 验证数据
    char* r1 = dm2.getBlock(block1);
    char* r2 = dm2.getBlock(block2);
    std::cout << "Read from block " << block1 << ": " << r1 << std::endl;
    std::cout << "Read from block " << block2 << ": " << r2 << std::endl;

    // 释放块
    dm2.freeBlock(block1);
    dm2.freeBlock(block2);
    std::cout << "Freed blocks: " << block1 << ", " << block2 << std::endl;

    return 0;
}
