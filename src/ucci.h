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
    
    // ȷ����һ��
    bool moved = false;
    // ����������ַ���
    std::string commandStr;
    // �����ַ����ָ��Ľ��
    std::vector<std::string> commandVec;
    // ��������
    void getCommand();
    // �ָ��ַ���
    void split();
    // ����ַ���������vector
    void clear();
    // ���½�ɫ
    void updWhichPlayer();
public:
    Board board;
    // ��������
    void bootEngine();
    // ��ʽ����
    void run();
};



#endif