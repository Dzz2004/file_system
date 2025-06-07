#include "inode.h"
#include "disk.h"
#include <cstring>
#include <stdexcept>
#include <algorithm>

Inode::Inode()
    : inodeId(-1), type(FILE), size(0), blockCount(0) {
    std::memset(directBlocks, -1, sizeof(directBlocks));
    createTime = std::time(nullptr);
    modifyTime = createTime;
}

void Inode::addBlock(int blockIdx) {
    if (blockCount >= DIRECT_BLOCKS) {
        throw std::runtime_error("Exceeded maximum number of direct blocks");
    }
    directBlocks[blockCount++] = blockIdx;
    modifyTime = std::time(nullptr);
}

const int* Inode::getBlocks() const {
    return directBlocks;
}

bool Inode::writeData(DiskManager& disk, const char* data, int length) {
    // 清楚之前的数据
    clearData(disk);
    
    int blocksNeeded = (length + DiskManager::BLOCK_SIZE - 1) / DiskManager::BLOCK_SIZE;
    std::string content = data ? std::string(data, length) : "";
    if (blocksNeeded > DIRECT_BLOCKS) return false;

    for (int i = 0; i < blocksNeeded; ++i) {
        int blk = disk.allocateBlock();
        if (blk == -1) return false;
        addBlock(blk);

        int copySize = std::min(DiskManager::BLOCK_SIZE, length - i * DiskManager::BLOCK_SIZE);
        std::memcpy(disk.getBlock(blk), content.c_str() + i * DiskManager::BLOCK_SIZE, copySize);
        std::string blockContent = std::string(disk.getBlock(blk), copySize);
    }
    size = length;
    modifyTime = std::time(nullptr);
    return true;
}

bool Inode::readData(DiskManager& disk, char* buffer, int maxLength) const {
    if (size > maxLength) return false;

    int offset = 0;
    for (int i = 0; i < blockCount; ++i) {
        int remaining = size - offset;
        if (remaining <= 0) break;

        int copySize = std::min(DiskManager::BLOCK_SIZE, remaining);
        std::string blockContent = std::string(disk.getBlock(directBlocks[i]), copySize);
        std::memcpy(buffer + offset, disk.getBlock(directBlocks[i]), copySize);
        offset += copySize;
    }
    std::string content = std::string(buffer, offset);
    return true;
}

void Inode::clearData(DiskManager& disk) {
    for (int i = 0; i < blockCount; ++i) {
        disk.freeBlock(directBlocks[i]);
    }
    std::memset(directBlocks, -1, sizeof(directBlocks));
    blockCount = 0;
    size = 0;
    modifyTime = std::time(nullptr);
}