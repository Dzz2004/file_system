#ifndef INODE_MANAGER_H
#define INODE_MANAGER_H

#include "inode.h"
#include <unordered_map>

class InodeManager {
public:
    InodeManager();

    int allocateInode(Inode::FileType type);
    Inode* getInode(int inodeId);
    void deleteInode(int inodeId);
    
    void serialize(std::ostream& out);
    void deserialize(std::istream& in);


private:
    int nextInodeId;
    std::unordered_map<int, Inode> inodeTable;
};



#endif // INODE_MANAGER_H
