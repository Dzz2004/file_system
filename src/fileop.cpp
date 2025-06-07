#include "fileop.h"
#include <iostream>
#include <sstream>
#include <algorithm>

FileOp::FileOp(FileSystemContext& fsCtx) : fs(fsCtx) {}

void FileOp::run() {
    std::cout << "Simple File System. Type 'help' for commands." << std::endl;
    std::string line;
    while (true) {
        std::cout << fs.pwd() << "> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;
        if (line == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        executeCommand(line);
    }
}

void FileOp::executeCommand(const std::string& line) {
    auto tokens = split(line, ' ');
    if (tokens.empty()) return;

    const std::string& cmd = tokens[0];
    try {
        if (cmd == "exit") {
            return;
        } else if (cmd == "help") {
            printHelp();
        } else if (cmd == "pwd") {
            std::cout << fs.pwd() << std::endl;
        } else if (cmd == "ls") {
            if (tokens.size() == 1) fs.ls();
            else fs.ls(tokens[1]);
        } else if (cmd == "cd" && tokens.size() > 1) {
            fs.cd(tokens[1]);
        } else if (cmd == "mkdir" && tokens.size() > 1) {
            fs.mkdir(tokens[1]);
        } else if (cmd == "rmdir" && tokens.size() > 1) {
            fs.rmdir(tokens[1]);
        } else if (cmd == "create" && tokens.size() > 2) {
            fs.createFile(tokens[1], tokens[2]);
        } else if (cmd == "read" && tokens.size() > 1) {
            fs.readFile(tokens[1]);
        } else if (cmd == "append" && tokens.size() > 2) {
            fs.appendFile(tokens[1], tokens[2]);
        } else if (cmd == "overwrite" && tokens.size() > 2) {
            fs.overwriteFile(tokens[1], tokens[2]);
        } else if (cmd == "rm" && tokens.size() > 1) {
            fs.rm(tokens[1]);
        } else if (cmd == "save" && tokens.size() > 1) {
            fs.save(tokens[1]);
        } else if (cmd == "load" && tokens.size() > 1) {
            fs.load(tokens[1]);
        } else {
            std::cout << "Unknown or invalid command. Type 'help' for help." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

std::vector<std::string> FileOp::split(const std::string& str, char delim) const {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        if (!token.empty()) result.push_back(token);
    }
    return result;
}

void FileOp::printHelp() const {
    std::cout << "Available commands:\n"
              << "  help                         Show this help message\n"
              << "  exit                         Exit the system\n"
              << "  pwd                          Show current directory\n"
              << "  ls [path]                    List directory contents\n"
              << "  cd <path>                    Change directory\n"
              << "  mkdir <path>                 Create a directory\n"
              << "  rmdir <name>                 Remove an empty directory\n"
              << "  create <name> <content>      Create a file with content\n"
              << "  read <name>                  Read file content\n"
              << "  append <name> <content>      Append content to a file\n"
              << "  overwrite <name> <content>   Overwrite file content\n"
              << "  rm <name>                    Delete a file\n"
              << "  save <filename>              Save virtual disk\n"
              << "  load <filename>              Load virtual disk\n";
}
