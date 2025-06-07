#include "directory.h"
#include <iostream>

int main() {
    // 创建根目录
    Directory root("root", 0);

    // 添加子目录 /root/home 和 /root/etc
    Directory* home = root.addSubdir("home", 1);
    Directory* etc = root.addSubdir("etc", 2);

    // 在 /root/home 添加用户目录 user1 和 user2
    Directory* user1 = home->addSubdir("user1", 3);
    Directory* user2 = home->addSubdir("user2", 4);

    // 添加文件到 /root/home/user1
    user1->addFile("readme.txt", 100);
    user1->addFile("main.cpp", 101);

    // 添加文件到 /root/etc
    etc->addFile("config.ini", 200);

    // 显示结构
    std::cout << "Root path: " << root.getPath() << std::endl;
    root.listContents();

    std::cout << "\n/home path: " << home->getPath() << std::endl;
    home->listContents();

    std::cout << "\n/home/user1 path: " << user1->getPath() << std::endl;
    user1->listContents();

    std::cout << "\n/etc path: " << etc->getPath() << std::endl;
    etc->listContents();

    // 查找和删除
    std::cout << "\nFind main.cpp in user1: inode = " << user1->findFile("main.cpp") << std::endl;
    user1->removeFile("main.cpp");
    user1->listContents();

    return 0;
}
