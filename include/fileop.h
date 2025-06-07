#pragma once

#include "fs.h"
#include <string>
#include <vector>

class FileOp {
public:
    explicit FileOp(FileSystemContext& fsCtx);

    void run();                              // 启动命令行 REPL
    void executeCommand(const std::string& line); // 处理单行命令

private:
    FileSystemContext& fs;

    void printHelp() const;
    std::vector<std::string> split(const std::string& str, char delim) const;
};
