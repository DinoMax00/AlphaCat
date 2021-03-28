#ifndef UCCI_H 
#define UCCI_H

#include <iostream>
#include <string.h>
#include <vector>

#include "board.h"
#include "base.h"

class UcciEngine
{
private:
    
    // 确定哪一边
    bool moved = false;
    // 读入的命令字符串
    std::string commandStr;
    // 命令字符串分割后的结果
    std::vector<std::string> commandVec;
    // 读入命令
    void getCommand();
    // 分割字符串
    void split();
    // 清空字符串与命令vector
    void clear();
    // 更新角色
    void updWhichPlayer();
public:
    Board board;
    // 引擎启动
    void bootEngine();
    // 正式启动
    void run();
};



#endif