#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <vector>
#include <memory>

struct DirEntry {
    enum EntryType {
        FILE,
        DIRECTORY
    } type;

    std::string name; // 文件或目录名
    int inodeId;      // inode 编号（对文件）或子目录的 inode

    DirEntry(const std::string& n, int id, EntryType t)
        : type(t), name(n), inodeId(id) {}
};

class Directory {
public:
    Directory(const std::string& name, int inodeId, Directory* parent = nullptr);

    Directory* addSubdir(const std::string& name, int inodeId);
    void addFile(const std::string& name, int inodeId);

    Directory* findSubdir(const std::string& name);
    int findFile(const std::string& name) const;

    void removeSubdir(const std::string& name);
    void removeFile(const std::string& name);

    void listContents() const;
    std::string getPath() const;
    bool isDirEmpty() const;

    Directory* getParent() const;
    const std::string& getName() const;
    int getInodeId() const;

    
    void serialize(std::ostream& out);
    void deserialize(std::istream& in);


private:
    std::string dirName;
    int inodeId;
    Directory* parentDir;
    std::vector<std::unique_ptr<Directory>> subdirs;
    std::vector<DirEntry> files;
};

#endif // DIRECTORY_H
