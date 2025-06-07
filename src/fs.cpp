#include "fs.h"

FileSystemContext::FileSystemContext(){
        int rootInodeId = inodeManager.allocateInode(Inode::DIRECTORY);
        root = std::make_unique<Directory>("", rootInodeId, nullptr);
        current = root.get();
}



std::vector<std::string> FileSystemContext::splitPath(const std::string& path){
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string item;
    while (std::getline(ss, item, '/')) {
        if (!item.empty()) parts.push_back(item);
    }
    return parts;
}

Directory* FileSystemContext::traverse(const std::string& path, bool createMissing) {
    Directory* dir = (path.empty() || path[0] != '/') ? current : root.get();
    auto parts = splitPath(path);
    for (const auto& part : parts) {
        Directory* next = dir->findSubdir(part);
        if (!next) {
            if (createMissing) {
                int newInode = inodeManager.allocateInode(Inode::DIRECTORY);
                next = dir->addSubdir(part, newInode);
            } else {
                return nullptr;
            }
        }
        dir = next;
    }
    return dir;
}

void FileSystemContext::mkdir(const std::string& path) {
    if (!traverse(path, true)) {
        std::cerr << "mkdir failed: invalid path " << path << std::endl;
    }
}

void FileSystemContext::cd(const std::string& path) {
    Directory* dest = traverse(path);
    if (dest) {
        current = dest;
    } else {
        std::cerr << "cd failed: path not found " << path << std::endl;
    }
}

std::string FileSystemContext::pwd()  {
    return current->getPath();
}

void FileSystemContext::ls(const std::string& path) {
    Directory* dir = path.empty() ? current : traverse(path);
    if (dir) {
        dir->listContents();
    } else {
        std::cerr << "ls failed: path not found " << path << std::endl;
    }
}

void FileSystemContext::createFile(const std::string& name, const std::string& content) {
    if (current->findFile(name) != -1) {
        std::cerr << "createFile failed: file already exists" << std::endl;
        return;
    }
    int newInode = inodeManager.allocateInode(Inode::FILE);
    Inode* inode = inodeManager.getInode(newInode);
    if (!inode->writeData(diskManager, content.c_str(), content.size() + 1)) {
        std::cerr << "createFile failed: write error" << std::endl;
        inodeManager.deleteInode(newInode);
        return;
    }
    current->addFile(name, newInode);
}

void FileSystemContext::readFile(const std::string& name){
    int inodeId = current->findFile(name);
    if (inodeId == -1) {
        std::cerr << "readFile failed: file not found" << std::endl;
        return;
    }
    Inode* inode = inodeManager.getInode(inodeId);
    if (!inode) {
        std::cerr << "readFile failed: inode not found" << std::endl;
        return;
    }
    char buffer[4096] = {0};
    if (!inode->readData(diskManager, buffer, sizeof(buffer))) {
        std::cerr << "readFile failed: read error" << std::endl;
        return;
    }
    // 逐字符打印buffer内容，遇到结束符且后面还有内容就跳过
    std::cout << "File content of '" << name << "':" << std::endl;
    for (int i = 0; i < inode->size && i < sizeof(buffer); ++i) {
        if (buffer[i] == '\0' && i + 1 < inode->size) {
            continue; // 跳过结束符后面的内容
        }
        std::cout << buffer[i];
    }
    std::cout << std::endl;

}

void FileSystemContext::rm(const std::string& name) {
    int inodeId = current->findFile(name);
    if (inodeId == -1) {
        std::cerr << "rm failed: file not found" << std::endl;
        return;
    }
    Inode* inode = inodeManager.getInode(inodeId);
    if (!inode || inode->type != Inode::FILE) {
        std::cerr << "rm failed: not a file" << std::endl;
        return;
    }
    inode->clearData(diskManager); // 清除文件数据
    inodeManager.deleteInode(inodeId);
    current->removeFile(name);
}

void FileSystemContext::rmdir(const std::string& name) {
    Directory* target = current->findSubdir(name);
    if (!target) {
        std::cerr << "rmdir failed: directory not found" << std::endl;
        return;
    }
    if (!target->getPath().empty() && (target->getPath() != "/") && !target->getParent()) {
        std::cerr << "rmdir failed: invalid directory" << std::endl;
        return;
    }
    std::string fullPath = target->getPath();
    if (!target->isDirEmpty()) {
        std::cerr << "rmdir failed: only empty directories can be removed" << std::endl;
        return;
    }
    current->removeSubdir(name);
}

void FileSystemContext::appendFile(const std::string& name, const std::string& content) {
    int inodeId = current->findFile(name);
    Inode* inode = inodeManager.getInode(inodeId);
    if (inodeId == -1 || inode == nullptr) {
        std::cerr << "appendFile failed: file not found or inode missing" << std::endl;
        return;
    }
    if (!inode || inode->type != Inode::FILE) {
        std::cerr << "appendFile failed: file not found or not a file" << std::endl;
        return;
    }
    char buffer[4096] = {0};
    if (inode->readData(diskManager, buffer, sizeof(buffer))) {
        std::string fullData(buffer, inode->size); // 用 size 显式构造
        fullData += content;
        inode->writeData(diskManager, fullData.c_str(), fullData.size() + 1);
    } else {
        std::cerr << "appendFile failed: read error" << std::endl;
    }
}

void FileSystemContext::overwriteFile(const std::string& name, const std::string& content) {
    int inodeId = current->findFile(name);
    Inode* inode = inodeManager.getInode(inodeId);
    if (inodeId == -1 || inode == nullptr) {
        std::cerr << "appendFile failed: file not found or inode missing" << std::endl;
        return;
    }
    if (!inode || inode->type != Inode::FILE) {
        std::cerr << "overwriteFile failed: file not found or not a file" << std::endl;
        return;
    }
    for (int i = 0; i < inode->blockCount; ++i) {
        diskManager.freeBlock(inode->directBlocks[i]);
    }
    inode->blockCount = 0;
    inode->writeData(diskManager, content.c_str(), content.size() + 1);
}

void FileSystemContext::save(const std::string& filename){
    diskManager.saveDisk(filename);
    std::ofstream meta(filename + ".meta", std::ios::binary);
    inodeManager.serialize(meta);
    root->serialize(meta);
    meta.close();
    std::cout << "Disk saved to " << filename << std::endl;
}

void FileSystemContext::load(const std::string& filename) {
    diskManager.loadDisk(filename);
    std::ifstream meta(filename + ".meta", std::ios::binary);
    if (!meta) {
        std::cerr << "Failed to load metadata: " << filename << ".meta" << std::endl;
        return;
    }
    inodeManager.deserialize(meta);
    root = std::make_unique<Directory>("/", 0);
    root->deserialize(meta);
    current = root.get();
    meta.close();
    std::cout << "Disk loaded from " << filename << std::endl;
}
