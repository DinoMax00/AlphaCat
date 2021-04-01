/*****************************************************************//**
 * \file   ucci.h
 * \brief  ucci协议头文件
 * 
 * \author AlphaCat
 * \date   March 2021
 *********************************************************************/
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
    /// 棋盘
    Board board;
    /// 读入的命令字符串
    std::string commandStr;
    /// 命令字符串分割后存放的Vec
    std::vector<std::string> commandVec;
    
    /**
     * @brief 获取一条命令并存入commandStr
     * 
     */
    void getCommand();

    /**
     * @brief 对命令字符串按空格进行分割并存入commandVec
     * 
     */
    void split();

    /**
     * @brief 清空命令字符串与命令Vec
     * 
     */
    void clear();

    /**
     * @brief 更新当前执子
     * 
     */
    void updWhichPlayer();

public:
    /**
     * @brief 对外调用引擎预启动的接口
     * 
     */
    void bootEngine();

    /**
     * @brief 对外调用引擎运行的接口
     * 
     */
    void run();
};



#endif