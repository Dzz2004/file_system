# 简易虚拟文件系统（Simple Virtual File System）

> **本项目实现了一个基于虚拟磁盘的简单单用户文件系统模拟器，支持基本的文件与目录操作、持久化存储，以及命令行交互操作。**

---
## 功能特性

* 使用虚拟磁盘按块管理存储（1MB 总容量，1024 个块）
* 基于 inode 的文件结构，每个 inode 使用直接块指针存储数据
* 树状目录结构，支持多级子目录
* 文件操作：创建、读取、写入（覆盖/追加）、删除
* 目录操作：创建目录、删除空目录、切换目录、列出目录、显示当前路径
* 持久化支持：可保存并加载完整磁盘数据及文件系统元信息
* 命令行交互界面，操作直观易用
---
## 项目结构

```
.
├── include/                     # 头文件目录
│   ├── directory.h
│   ├── disk.h
│   ├── fileop.h
│   ├── fs.h
│   ├── inode.h
│   └── inode_manager.h
│
├── src/                         # 源代码目录
│   ├── directory.cpp
│   ├── disk.cpp
│   ├── fileop.cpp
│   ├── fs.cpp
│   ├── inode.cpp
│   ├── inode_manager.cpp
│   └── main.cpp
│
├── test/                        # 单元测试目录
│   ├── test_directory.cpp
│   ├── test_disk.cpp
│   ├── test_fs.cpp
│   └── test_inode.cpp
│
├── CMakeLists.txt               # CMake 构建脚本（如有）
└── README.md                    # 项目说明文档

```
---
## 编译与运行（环境：WSL Ubuntu 22.04, g++ 11.4）

在开始之前，请确保您的系统已安装支持 C++17 标准的编译器（例如 g++ 11.4）并处于 Linux 环境下（本项目已在 WSL Ubuntu 22.04 + g++ 11.4 环境测试通过）。以下是项目的安装和运行步骤：
1. **获取代码**： 将代码克隆到本地。
```bash
git clone https://github.com/Dzz2004/file_system.git
```
2. 使用cmake进行项目构建，项目以提供```cmakelists.txt```文件为基础。这里不再给出具体代码，可以使用cmake命令行或vscode的CMake工具进行构建。
3. 可执行文件将生成在 `build` 目录下，构建目标target为file_system。
```bash
./build/file_system
```

---

## 支持的命令

| 命令            | 说明              |
| ------------- | --------------- |
| `new`         | 新建一个空的文件系统      |
| `sfs`         | 从已有虚拟磁盘文件加载文件系统 |
| `exit`        | 退出系统并保存虚拟磁盘     |
| `mkdir <路径>`  | 创建子目录           |
| `rmdir <目录>`  | 删除空目录           |
| `ls [路径]`     | 显示目录内容          |
| `cd <路径>`     | 切换当前目录          |
| `pwd`         | 显示当前所在路径        |
| `create <文件>` | 创建新文件（会要求输入内容）  |
| `read <文件>`   | 读取并显示文件内容       |
| `write <文件>`  | 覆盖写入文件内容        |
| `append <文件>` | 追加内容到文件末尾       |
| `delete <文件>` | 删除指定文件          |
| `save <文件>`   | 将当前虚拟磁盘保存到指定文件  |
| `load <文件>`   | 从指定文件加载虚拟磁盘     |

## 示例交互流程

```
> new
> mkdir /home/user
> cd /home/user
> create readme.txt
> append readme.txt
> read readme.txt
> cd /
> ls /home
> save vdisk_final.dat
```

## 注意事项

* 虚拟磁盘保存于 `.dat` 文件，文件系统结构保存于 `.dat.meta` 文件
* 退出前若未执行 `save` 或未正常退出，数据将不会被保存
* 默认启动时会尝试加载 `vdisk_final.dat`，找不到则启动新系统

## 作者
作者：Zhuangzhi Dong
本项目为BIT操作系统课程实验的一部分。

---
