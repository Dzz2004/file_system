#include "disk.h"


DiskManager::DiskManager() {
    std::memset(disk, 0, sizeof(disk));
    std::memset(blockBitmap, 0, sizeof(blockBitmap));
}

void DiskManager::loadDisk(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return;

    in.read(reinterpret_cast<char*>(disk), sizeof(disk));
    in.read(reinterpret_cast<char*>(blockBitmap), sizeof(blockBitmap));
    in.close();
}

void DiskManager::saveDisk(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) return;

    out.write(reinterpret_cast<const char*>(disk), sizeof(disk));
    out.write(reinterpret_cast<const char*>(blockBitmap), sizeof(blockBitmap));
    out.close();
}

int DiskManager::allocateBlock() {
    for (int i = 0; i < BLOCK_COUNT; ++i) {
        if (!blockBitmap[i]) {
            blockBitmap[i] = true;
            std::memset(disk[i], 0, BLOCK_SIZE); // 可选清空块内容
            return i;
        }
    }
    return -1; // 无空闲块可用
}

void DiskManager::freeBlock(int idx) {
    if (idx >= 0 && idx < BLOCK_COUNT) {
        blockBitmap[idx] = false;
        std::memset(disk[idx], 0, BLOCK_SIZE); // 可选清空内容
    }
}

char* DiskManager::getBlock(int idx) {
    if (idx >= 0 && idx < BLOCK_COUNT) {
        return disk[idx];
    }
    return nullptr;
}
