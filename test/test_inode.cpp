#include "inode.h"
#include "disk.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstring>

void printTime(time_t t) {
    std::tm* tm_ptr = std::localtime(&t);
    std::cout << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
}

int main() {
    DiskManager disk;
    Inode inode;
    inode.inodeId = 1;
    inode.type = Inode::FILE;

    const char* msg = "Hello from Inode!";
    int len = std::strlen(msg) + 1;

    if (inode.writeData(disk, msg, len)) {
        std::cout << "Write succeeded." << std::endl;
    } else {
        std::cerr << "Write failed." << std::endl;
    }

    char buffer[1024] = {0};
    if (inode.readData(disk, buffer, sizeof(buffer))) {
        std::cout << "Read content: " << buffer << std::endl;
    } else {
        std::cerr << "Read failed." << std::endl;
    }

    std::cout << "Inode ID: " << inode.inodeId << std::endl;
    std::cout << "Type: " << (inode.type == Inode::FILE ? "FILE" : "DIRECTORY") << std::endl;
    std::cout << "Size: " << inode.size << " bytes\n";
    std::cout << "Block Count: " << inode.blockCount << std::endl;
    std::cout << "Direct Blocks: ";
    const int* blocks = inode.getBlocks();
    for (int i = 0; i < inode.blockCount; ++i) {
        std::cout << blocks[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Create Time: ";
    printTime(inode.createTime);
    std::cout << "\nModify Time: ";
    printTime(inode.modifyTime);
    std::cout << std::endl;

    return 0;
}
