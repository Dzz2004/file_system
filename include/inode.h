#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>
#include <ctime>

#include "disk.h" 

class Inode {
public:
    static const int DIRECT_BLOCKS = 8; // 直接块数量，可调整

    enum FileType {
        FILE,
        DIRECTORY
    };

    Inode();

    int inodeId;                  // i-node编号
    FileType type;               // 文件类型
    int size;                    // 文件大小（字节）
    int blockCount;              // 实际使用的块数
    int directBlocks[DIRECT_BLOCKS]; // 直接块指针
    time_t createTime;           // 创建时间
    time_t modifyTime;           // 修改时间

    void addBlock(int blockIdx);
    const int* getBlocks() const;

    // 与磁盘交互的读写接口
    bool writeData(DiskManager& disk, const char* data, int length);
    bool readData(DiskManager& disk, char* buffer, int maxLength) const;
    void clearData(DiskManager& disk);
};

#endif // INODE_H
