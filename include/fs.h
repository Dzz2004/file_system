#ifndef FILESYSTEM_CONTEXT_H
#define FILESYSTEM_CONTEXT_H

#include "directory.h"
#include "inode_manager.h"
#include "disk.h"
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <memory>

class FileSystemContext {
public:
    FileSystemContext();

    void mkdir(const std::string& path);
    void ls(const std::string& path = "");
    void cd(const std::string& path);
    std::string pwd();

    void createFile(const std::string& name, const std::string& content);
    void readFile(const std::string& name);

    void rm(const std::string& name);                 // 删除文件
    void rmdir(const std::string& name);              // 删除目录（空目录）
    void appendFile(const std::string& name, const std::string& content);
    void overwriteFile(const std::string& name, const std::string& content);

    void save(const std::string& filename);     // 保存虚拟磁盘到文件
    void load(const std::string& filename);           // 从文件加载虚拟磁盘

private:
    std::unique_ptr<Directory> root;
    Directory* current;
    InodeManager inodeManager;
    DiskManager diskManager;

    Directory* traverse(const std::string& path, bool createMissing = false);
    std::vector<std::string> splitPath(const std::string& path);
};

#endif // FILESYSTEM_CONTEXT_H
