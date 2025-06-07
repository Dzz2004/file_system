#include "inode_manager.h"
#include <stdexcept>

InodeManager::InodeManager() : nextInodeId(1) {} // inode 0 通常保留给根目录

int InodeManager::allocateInode(Inode::FileType type) {
    int id = nextInodeId++;
    Inode node;
    node.inodeId = id;
    node.type = type;
    inodeTable[id] = node;
    return id;
}

Inode* InodeManager::getInode(int inodeId) {
    auto it = inodeTable.find(inodeId);
    if (it != inodeTable.end()) {
        return &(it->second);
    }
    return nullptr;
}

void InodeManager::deleteInode(int inodeId) {
    inodeTable.erase(inodeId);
}


void InodeManager::serialize(std::ostream& out) {
    out.write(reinterpret_cast<char*>(&nextInodeId), sizeof(nextInodeId));
    size_t count = inodeTable.size();
    out.write(reinterpret_cast<char*>(&count), sizeof(count));
    for (const auto& [id, inode] : inodeTable) {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        out.write(reinterpret_cast<const char*>(&inode), sizeof(Inode));
    }
}

void InodeManager::deserialize(std::istream& in) {
    inodeTable.clear();
    in.read(reinterpret_cast<char*>(&nextInodeId), sizeof(nextInodeId));
    size_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    for (size_t i = 0; i < count; ++i) {
        int id;
        Inode inode;
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        in.read(reinterpret_cast<char*>(&inode), sizeof(Inode));
        inodeTable[id] = inode;
    }
}


