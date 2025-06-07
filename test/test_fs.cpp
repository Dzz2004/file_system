#include "fs.h"
#include <iostream>

int main() {
    FileSystemContext fs;

    std::cout << "[mkdir /home/user]" << std::endl;
    fs.mkdir("/home/user");

    std::cout << "[cd /home/user]" << std::endl;
    fs.cd("/home/user");

    std::cout << "[pwd] => " << fs.pwd() << std::endl;

    std::cout << "[createFile readme.txt]" << std::endl;
    fs.createFile("readme.txt", "This is a test file created in /home/user directory.");

    std::cout << "[createFile hello.txt]" << std::endl;
    fs.createFile("hello.txt", "Hello from the file system simulator!");

    std::cout << "[ls]" << std::endl;
    fs.ls();

    std::cout << "[appendFile readme.txt]" << std::endl;
    fs.appendFile("readme.txt", " Appending more content.");

    std::cout << "[readFile readme.txt]" << std::endl;
    fs.readFile("readme.txt");

    std::cout << "[overwriteFile hello.txt]" << std::endl;
    fs.overwriteFile("hello.txt", "This file has been overwritten.");

    std::cout << "[readFile hello.txt]" << std::endl;
    fs.readFile("hello.txt");

    std::cout << "[rm readme.txt]" << std::endl;
    fs.rm("readme.txt");
    std::cout << "[ls]" << std::endl;
    fs.ls();

    std::cout << "[cd /]" << std::endl;
    fs.cd("/");
    std::cout << "[ls /home]" << std::endl;
    fs.ls("/home");

    std::cout << "[rmdir /home/user]" << std::endl;
    fs.cd("/home");
    fs.rmdir("user");
    std::cout << "[ls /home]" << std::endl;
    fs.ls("/home");

    std::cout << "[save vdisk.dat]" << std::endl;
    fs.save("vdisk.dat");

    std::cout << "[load vdisk.dat]" << std::endl;
    fs.load("vdisk.dat");

    std::cout << "[pwd] => " << fs.pwd() << std::endl;

    return 0;
}