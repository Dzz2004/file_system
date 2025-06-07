# 简易文件系统模拟器设计说明

该仓库实现了一个使用 C++ 编写的**简易文件系统模拟器**。其核心组件包括磁盘块管理、inode 管理、目录树以及命令行交互模块。本文对主要类结构、功能关系和调用流程进行统一说明。

---

## 1. 类结构与关系总览

### `DiskManager`（磁盘管理器）

管理一个固定大小的虚拟磁盘，支持块分配与释放操作，并维护块位图。

```cpp
class DiskManager {
public:
    static constexpr int BLOCK_SIZE = 1024;   // 每块大小为 1024 字节
    static constexpr int BLOCK_COUNT = 1024;  // 总共 1024 块，共 1MB

    int allocateBlock();          // 分配空闲块
    void freeBlock(int idx);      // 释放指定块
    char* getBlock(int idx);      // 获取块内容指针

private:
    char disk[BLOCK_COUNT][BLOCK_SIZE];  // 虚拟磁盘空间
    bool blockBitmap[BLOCK_COUNT];       // 块使用位图
};
```

---

### `Inode`（索引节点）

表示文件或目录节点，每个 inode 最多引用 8 个直接块。

```cpp
class Inode {
public:
    static const int DIRECT_BLOCKS = 8;
    enum FileType { FILE, DIRECTORY };

    int inodeId;
    FileType type;
    int size;
    int blockCount;
    int directBlocks[DIRECT_BLOCKS];

    bool writeData(DiskManager& disk, const char* data, int length);
    bool readData(DiskManager& disk, char* buffer, int maxLength) const;
    void clearData(DiskManager& disk);
};
```

---

### `InodeManager`（索引节点管理器）

分配和管理 inode 表。

```cpp
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
```

---

### `Directory`（目录）

表示层次化目录结构及其中的文件，使用 inodeId 映射到文件。

```cpp
struct DirEntry {
    enum EntryType { FILE, DIRECTORY } type;
    std::string name;
    int inodeId;
};

class Directory {
public:
    Directory(const std::string& name, int inodeId, Directory* parent = nullptr);
    
    Directory* addSubdir(const std::string& name, int inodeId);
    void addFile(const std::string& name, int inodeId);
    
    Directory* findSubdir(const std::string& name);
    int findFile(const std::string& name) const;

    void serialize(std::ostream& out);
    void deserialize(std::istream& in);

private:
    std::string dirName;
    int inodeId;
    Directory* parentDir;
    std::vector<std::unique_ptr<Directory>> subdirs;
    std::vector<DirEntry> files;
};
```

---

### `FileSystemContext`（文件系统上下文）

整合上述各组件，提供对文件系统的高层操作接口。

```cpp
class FileSystemContext {
public:
    FileSystemContext();

    void mkdir(const std::string& path);
    void ls(const std::string& path = "");
    void cd(const std::string& path);
    std::string pwd();

    void createFile(const std::string& name, const std::string& content);
    void readFile(const std::string& name);
    void rm(const std::string& name);
    void rmdir(const std::string& name);

    void appendFile(const std::string& name, const std::string& content);
    void overwriteFile(const std::string& name, const std::string& content);

    void save(const std::string& filename);
    void load(const std::string& filename);

private:
    std::unique_ptr<Directory> root;
    Directory* current;
    InodeManager inodeManager;
    DiskManager diskManager;

    Directory* traverse(const std::string& path, bool createMissing = false);
    std::vector<std::string> splitPath(const std::string& path);
};
```

---

### `FileOp`（命令行操作器）

命令行接口，用于解析用户命令并调用 `FileSystemContext` 的方法。

```cpp
class FileOp {
public:
    explicit FileOp(FileSystemContext& fsCtx);
    void run(); // 启动交互式命令循环
    void executeCommand(const std::string& line);

private:
    FileSystemContext& fs;

    void printHelp() const;
    std::vector<std::string> split(const std::string& str, char delim) const;
};
```

---

### `main.cpp`

创建 `FileSystemContext`，尝试加载磁盘数据，启动命令行交互，退出前保存状态。

```cpp
int main() {
    FileSystemContext fsCtx;
    FileOp fileOp(fsCtx);
    const std::string diskFile = "vdisk_final.dat";

    ...
    fileOp.run();
    ...
    fsCtx.save(diskFile);
    return 0;
}
```

---

## 2. 调用关系与运行流程

1. 程序启动后 `main()` 创建 `FileSystemContext` 并尝试加载已有的磁盘数据。

2. 启动 `FileOp::run()` 进入 REPL（命令行循环），持续读取用户命令。

3. 用户命令通过 `executeCommand()` 解析并调用 `FileSystemContext` 对应方法。

4. 文件系统操作通过如下模块完成：

   * 目录结构由 `Directory` 管理；
   * inode 由 `InodeManager` 分配和存储；
   * 实际数据读写通过 `DiskManager` 实现；
   * 如创建文件：

     ```cpp
     int newInode = inodeManager.allocateInode(Inode::FILE);
     Inode* inode = inodeManager.getInode(newInode);
     inode->writeData(diskManager, content.c_str(), content.size() + 1);
     current->addFile(name, newInode);
     ```

5. 退出时调用 `save()` 序列化所有状态，确保文件系统数据持久化。

---

## 3. 关键函数设计

### DiskManager

* `allocateBlock()`：扫描位图找到空闲块，清零后返回其索引。
* `freeBlock()`：释放指定块并可选清空内容。

### Inode

* `writeData()`：清除原有数据，计算所需块数，逐块写入内容。
* `readData()`：从块中按顺序读取文件内容。
* `clearData()`：释放该 inode 引用的所有块。

### InodeManager

* `allocateInode()`：创建新 inode 并加入表中。
* `serialize()/deserialize()`：实现 inode 表的持久化。

### Directory

* 创建、查找、删除子目录与文件；
* `serialize()`/`deserialize()`：用于保存/加载目录树结构。

### FileSystemContext

* `mkdir`/`cd`/`ls` 等命令均依赖 `traverse()` 解析路径；
* `createFile`/`readFile` 等操作借助 `InodeManager` 和 `DiskManager` 完成内容管理；
* `rm`/`rmdir` 删除文件/目录并释放 inode；
* `appendFile`/`overwriteFile` 允许修改已有文件；
* `save/load`：保存/恢复文件系统状态（磁盘、inode、目录树）。

### FileOp

* 实现命令解析与调度；
* 使用 `split()` 解析输入命令行，调用 `FileSystemContext` 执行。

---

## 4. 运行交互流程

```text
程序启动 → 加载磁盘状态 → 启动命令行循环（REPL） → 用户输入命令
        → 解析命令 → 文件系统操作 → 返回执行结果
        → 用户输入 exit → 触发 save() → 所有状态写入磁盘文件
```

---

## 5. 总结：模块化分层结构

```
用户输入 <-> FileOp <-> FileSystemContext
                    ├── Directory（目录树）
                    ├── InodeManager（inode 表）
                    └── DiskManager（块存储）
                          ↓
                   持久化 save/load
```

### 各模块职责明确：

* **DiskManager**：抽象底层块分配与管理；
* **Inode**：记录文件元数据和块引用；
* **InodeManager**：分配/释放 inode，支持序列化；
* **Directory**：构建层次化命名空间；
* **FileSystemContext**：提供统一的高层接口；
* **FileOp**：实现用户交互界面。


