#include "directory.h"
#include <iostream>
#include <algorithm>

Directory::Directory(const std::string& name, int id, Directory* parent)
    : dirName(name), inodeId(id), parentDir(parent) {}

Directory* Directory::addSubdir(const std::string& name, int id) {
    for (const auto& d : subdirs) {
        if (d->dirName == name) {
            std::cerr << "Subdirectory already exists: " << name << std::endl;
            return nullptr;
        }
    }
    subdirs.emplace_back(std::make_unique<Directory>(name, id, this));
    return subdirs.back().get();
}

void Directory::addFile(const std::string& name, int id) {
    for (const auto& f : files) {
        if (f.name == name) {
            std::cerr << "File already exists: " << name << std::endl;
            return;
        }
    }
    files.emplace_back(name, id, DirEntry::FILE);
}

Directory* Directory::findSubdir(const std::string& name) {
    for (const auto& d : subdirs) {
        if (d->dirName == name) {
            return d.get();
        }
    }
    return nullptr;
}

int Directory::findFile(const std::string& name) const {
    for (const auto& f : files) {
        if (f.name == name) {
            return f.inodeId;
        }
    }
    return -1;
}

void Directory::removeSubdir(const std::string& name) {
    auto it = std::remove_if(subdirs.begin(), subdirs.end(), [&](const std::unique_ptr<Directory>& d) {
        return d->dirName == name;
    });
    if (it != subdirs.end()) {
        subdirs.erase(it, subdirs.end());
    } else {
        std::cerr << "Subdirectory not found: " << name << std::endl;
    }
}

void Directory::removeFile(const std::string& name) {
    auto it = std::remove_if(files.begin(), files.end(), [&](const DirEntry& f) {
        return f.name == name;
    });
    if (it != files.end()) {
        files.erase(it, files.end());
    } else {
        std::cerr << "File not found: " << name << std::endl;
    }
}

void Directory::listContents() const {
    std::cout << "Directory: " << dirName << std::endl;
    for (const auto& d : subdirs) {
        std::cout << "  [DIR]  " << d->dirName << " (inode: " << d->inodeId << ")" << std::endl;
    }
    for (const auto& f : files) {
        std::cout << "  [FILE] " << f.name << " (inode: " << f.inodeId << ")" << std::endl;
    }
}

std::string Directory::getPath() const {
    if (parentDir == nullptr) return "/"; // 根目录
    std::string parentPath = parentDir->getPath();
    return parentPath + (parentPath == "/" ? "" : "/") + dirName;
}

bool Directory::isDirEmpty() const {
    return subdirs.empty() && files.empty();
}

Directory* Directory::getParent() const {
    return parentDir;
}

const std::string& Directory::getName() const {
    return dirName;
}

int Directory::getInodeId() const {
    return inodeId;
}

// directory.cpp

void Directory::serialize(std::ostream& out) {
    size_t nameLen = dirName.size();
    out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    out.write(dirName.c_str(), nameLen);
    out.write(reinterpret_cast<const char*>(&inodeId), sizeof(inodeId));

    // 写入子目录数量
    size_t subdirCount = subdirs.size();
    out.write(reinterpret_cast<const char*>(&subdirCount), sizeof(subdirCount));
    for (const auto& sub : subdirs) {
        sub->serialize(out);
    }

    // 写入文件数量
    size_t fileCount = files.size();
    out.write(reinterpret_cast<const char*>(&fileCount), sizeof(fileCount));
    for (const auto& entry : files) {
        size_t nameLen = entry.name.size();
        out.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        out.write(entry.name.c_str(), nameLen);
        out.write(reinterpret_cast<const char*>(&entry.inodeId), sizeof(entry.inodeId));
        int type = static_cast<int>(entry.type);
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
    }
}

void Directory::deserialize(std::istream& in) {
    size_t nameLen;
    in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    dirName.resize(nameLen);
    in.read(&dirName[0], nameLen);
    in.read(reinterpret_cast<char*>(&inodeId), sizeof(inodeId));

    // 子目录
    size_t subdirCount;
    in.read(reinterpret_cast<char*>(&subdirCount), sizeof(subdirCount));
    subdirs.clear();
    for (size_t i = 0; i < subdirCount; ++i) {
        auto sub = std::make_unique<Directory>("", 0, this);
        sub->deserialize(in);
        subdirs.push_back(std::move(sub));
    }

    // 文件项
    size_t fileCount;
    in.read(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));
    files.clear();
    for (size_t i = 0; i < fileCount; ++i) {
        size_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        std::string name(len, '\0');
        in.read(&name[0], len);
        int inodeId, typeInt;
        in.read(reinterpret_cast<char*>(&inodeId), sizeof(inodeId));
        in.read(reinterpret_cast<char*>(&typeInt), sizeof(typeInt));
        files.emplace_back(name, inodeId, static_cast<DirEntry::EntryType>(typeInt));
    }
}


